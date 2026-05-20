#include <check.h>
#include "test.h"
#include "error.h"
#include "unixv6fs.h"
#include "direntv6.h"

#define FIRST_DISK DATA_DIR "/first.uv6"
#define SIMPLE_DISK DATA_DIR "/simple.uv6"
#define BROKEN_DIR_DISK DATA_DIR "/broken_dir.uv6"

START_TEST(direntv6_opendir_null_param){
	start_test_print;

	ck_assert_invalid_arg(direntv6_opendir((void*)1, 0, NULL));
	ck_assert_invalid_arg(direntv6_opendir(NULL, 0, (void*)1));
	ck_assert_invalid_arg(direntv6_opendir(NULL, 0, NULL));

	end_test_print;
}
END_TEST

START_TEST(direntv6_opendir_invalid_dir){
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_err_none(mountv6(SIMPLE_DISK, &fs));

	struct directory_reader dir = {0};
	ck_assert_int_eq(direntv6_opendir(&fs, 3, &dir), ERR_INVALID_DIRECTORY_INODE);

    ck_assert_err_none(umountv6(&fs));
	end_test_print;
}
END_TEST

START_TEST(direntv6_opendir_unallocated_inode){
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_err_none(mountv6(SIMPLE_DISK, &fs));

	struct directory_reader dir = {0};
	ck_assert_int_eq(direntv6_opendir(&fs, 4, &dir), ERR_UNALLOCATED_INODE);

	ck_assert_err_none(umountv6(&fs));
	end_test_print;
}
END_TEST

START_TEST(direntv6_opendir_out_of_range){
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_err_none(mountv6(SIMPLE_DISK, &fs));

	struct directory_reader dir = {0};
	ck_assert_int_eq(direntv6_opendir(&fs, -1, &dir), ERR_INODE_OUT_OF_RANGE);

    ck_assert_err_none(umountv6(&fs));
	end_test_print;
}
END_TEST

START_TEST(direntv6_opendir_valid){
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_err_none(mountv6(SIMPLE_DISK, &fs));

	struct directory_reader dir = {0};
	ck_assert_err_none(direntv6_opendir(&fs, ROOT_INUMBER, &dir));

	struct inode expected_in = {0};
	expected_in.i_mode = 49152;
	expected_in.i_size1 = 16;
	expected_in.i_addr[0] = 35;

	ck_assert_ptr_eq(dir.fv6.u, &fs);
	ck_assert_int_eq(dir.fv6.i_number, ROOT_INUMBER);
	ck_assert_inode_eq(dir.fv6.i_node, expected_in);
	ck_assert_int_eq(dir.fv6.offset, 0);

	uint8_t null_block[sizeof(dir.dirs)] = {0};
	ck_assert_mem_eq(dir.dirs, null_block, sizeof(dir.dirs));
	ck_assert_int_eq(dir.cur, 0);
	ck_assert_int_eq(dir.last, 0);

    ck_assert_err_none(umountv6(&fs));
	end_test_print;
}
END_TEST

START_TEST(direntv6_readdir_null_param){
	start_test_print;

	ck_assert_invalid_arg(direntv6_readdir((void*)1, NULL, NULL));
	ck_assert_invalid_arg(direntv6_readdir(NULL, (void*)1, NULL));
	ck_assert_invalid_arg(direntv6_readdir(NULL, NULL, (void*)1));

	end_test_print;
}
END_TEST

START_TEST(direntv6_readdir_valid){
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_err_none(mountv6(SIMPLE_DISK, &fs));

	struct directory_reader dir = {0};
	ck_assert_err_none(direntv6_opendir(&fs, ROOT_INUMBER, &dir));

	char name[DIRENT_MAXLEN + 1] = {0};
	uint16_t inr;

	ck_assert_int_eq(direntv6_readdir(&dir, name, &inr), 1);
	ck_assert_str_eq(name, "tmp");
	ck_assert_int_eq(inr, 2);
	ck_assert_int_eq(dir.cur, 1);
	ck_assert_int_eq(dir.last, 1);

	ck_assert_int_eq(direntv6_readdir(&dir, name, &inr), 0);

    ck_assert_err_none(umountv6(&fs));
	end_test_print;
}
END_TEST

START_TEST(direntv6_dirlookup_null_params){
	start_test_print;

	ck_assert_invalid_arg(direntv6_dirlookup(NULL, ROOT_INUMBER, NON_NULL));
	ck_assert_invalid_arg(direntv6_dirlookup(NON_NULL, ROOT_INUMBER, NULL));
	ck_assert_invalid_arg(direntv6_dirlookup(NULL, ROOT_INUMBER, NULL));

	end_test_print;
}
END_TEST

START_TEST(direntv6_dirlookup_inexistant_file) {
	start_test_print;

	struct unix_filesystem u;
	ck_assert_err_none(mountv6(SIMPLE_DISK, &u));

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "foo"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/tmp/foo"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, 2, "/tmp"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, 2, "hi.txt"), ERR_NO_SUCH_FILE);

	ck_assert_err_none(umountv6(&u));

	end_test_print;
}
END_TEST

// Very tricky edge case if length is badly handled
START_TEST(direntv6_dirlookup_inexistant_file2){
	start_test_print;

	struct unix_filesystem u;
	ck_assert_err_none(mountv6(SIMPLE_DISK, &u));

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/t///"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "///tm/coucou.txt"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, 2, "coucou"), ERR_NO_SUCH_FILE);

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/mp///"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "///mp/coucou.txt"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "///p/coucou.txt"), ERR_NO_SUCH_FILE);

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/tmp1234"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "///tmp1234/coucou.txt"), ERR_NO_SUCH_FILE);
	ck_assert_err(direntv6_dirlookup(&u, 2, "coucou.txt.dat"), ERR_NO_SUCH_FILE);

	ck_assert_err_none(umountv6(&u));

	end_test_print;
}
END_TEST

START_TEST(direntv6_dirlookup_valid){
	start_test_print;

	struct unix_filesystem u;
	ck_assert_err_none(mountv6(SIMPLE_DISK, &u));

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "tmp"), 2);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/tmp"), 2);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "tmp/coucou.txt"), 3);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/tmp/coucou.txt"), 3);
	ck_assert_err(direntv6_dirlookup(&u, 2, "coucou.txt"), 3);
	ck_assert_err(direntv6_dirlookup(&u, 2, "/coucou.txt"), 3);

	ck_assert_err_none(umountv6(&u));

	end_test_print;
}
END_TEST

START_TEST(direntv6_dirlookup_valid_prefixes){
	start_test_print;

	struct unix_filesystem u;
	ck_assert_err_none(mountv6(FIRST_DISK, &u));

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/hello/net/testdata/igmp"), 120);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/hello/net/testdata/igmp6"), 122);

	ck_assert_err_none(umountv6(&u));

	end_test_print;
}
END_TEST

START_TEST(direntv6_dirlookup_multiple_path_tokens){
	start_test_print;

	struct unix_filesystem u;
	ck_assert_err_none(mountv6(SIMPLE_DISK, &u));

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "tmp///coucou.txt"), 3);
	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "//////tmp//coucou.txt"), 3);
	ck_assert_err(direntv6_dirlookup(&u, 2, "//coucou.txt"), 3);

	ck_assert_err_none(umountv6(&u));

	end_test_print;
}
END_TEST

START_TEST(direntv6_dirlookup_broken_dir){
	start_test_print;

	struct unix_filesystem u;
	ck_assert_err_none(mountv6(BROKEN_DIR_DISK, &u));

	ck_assert_err(direntv6_dirlookup(&u, ROOT_INUMBER, "/"), ROOT_INUMBER);
	ck_assert_fails(direntv6_dirlookup(&u, ROOT_INUMBER, "tmp///coucou.txt"));

	ck_assert_err_none(umountv6(&u));

	end_test_print;
}
END_TEST

Suite* direntv6_test_suite(){
	Suite* s = suite_create("Test for directory layer");

	Add_Test(s,  direntv6_opendir_null_param);
	Add_Test(s,  direntv6_opendir_invalid_dir);
	Add_Test(s,  direntv6_opendir_out_of_range);
	Add_Test(s,  direntv6_opendir_unallocated_inode);
	Add_Test(s,  direntv6_opendir_valid);

	Add_Test(s,  direntv6_readdir_null_param);
	Add_Test(s,  direntv6_readdir_valid);

	Add_Test(s,  direntv6_dirlookup_null_params);
	Add_Test(s,  direntv6_dirlookup_inexistant_file);
	Add_Test(s,  direntv6_dirlookup_inexistant_file2);
	Add_Test(s,  direntv6_dirlookup_valid);
	Add_Test(s,  direntv6_dirlookup_valid_prefixes);
	Add_Test(s,  direntv6_dirlookup_multiple_path_tokens);
	Add_Test(s,  direntv6_dirlookup_broken_dir);
	
	return s;
}

TEST_SUITE(direntv6_test_suite)
