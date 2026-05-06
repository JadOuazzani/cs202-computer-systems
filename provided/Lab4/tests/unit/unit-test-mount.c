#include <check.h>
#include "test.h"

#include "error.h"
#include "mount.h"

#include <stdio.h>
#include <assert.h>

#define SIMPLE_DISK DATA_DIR "/simple.uv6"
#define FIRST_DISK  DATA_DIR "/first.uv6"
#define AIW_DISK    DATA_DIR "/aiw.uv6"
#define BROKEN_DIR_DISK DATA_DIR "/broken_dir.uv6"

void create_file(const char* filename){
	fclose(fopen(filename, "w"));
}

START_TEST(mount_null_param)
{
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_invalid_arg(mountv6(NULL, &fs));
	ck_assert_invalid_arg(mountv6(SIMPLE_DISK, NULL));
	ck_assert_invalid_arg(mountv6(NULL, NULL));

	end_test_print;
}
END_TEST

START_TEST(mount_inexisting_file)
{
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_int_eq(mountv6("asldfjaslkdfhj.uv6", &fs), ERR_IO);

	end_test_print;
}
END_TEST

START_TEST(mount_bad_boot_sector)
{
	start_test_print;

	struct unix_filesystem fs = {0};
	ck_assert_int_eq(mountv6(DATA_DIR "/bad_boot.uv6", &fs), ERR_BAD_BOOT_SECTOR);

	end_test_print;
}
END_TEST

START_TEST(mount_empty_file)
{
	start_test_print;

	create_file("dump");
	struct unix_filesystem fs = {0};
	ck_assert_int_eq(mountv6( "dump", &fs), ERR_IO);
	remove("dump");

	end_test_print;
}
END_TEST

START_TEST(mount_no_sb)
{
	start_test_print;

	create_file("dump");
	struct unix_filesystem fs = {0};
	ck_assert_int_eq(mountv6(DATA_DIR "/no_sb.uv6", &fs), ERR_IO);
	remove("dump");

	end_test_print;
}
END_TEST

START_TEST(mount_valid)
{
	start_test_print;

	struct superblock expected = {
			(uint16_t) 32,
			(uint16_t) 1024,
			(uint16_t) 0,
			(uint16_t) 0,
			(uint16_t) 2,
			(uint16_t) 34,
			(uint16_t) 0,
			(uint16_t) 0,

			(uint8_t) 0,
			(uint8_t) 0,
			(uint8_t) 0,
			(uint8_t) 0,
			(uint16_t) 0,
			(uint16_t) 0,
	};

	struct unix_filesystem fs = {0};
	ck_assert_err_none(mountv6(SIMPLE_DISK, &fs));
	ck_assert_mem_eq(&fs.s, &expected, sizeof (expected));
    ck_assert_err_none(umountv6(&fs));

	end_test_print;
}
END_TEST

START_TEST(unmount_null_param){
	start_test_print;

	ck_assert_invalid_arg(umountv6(NULL));

	end_test_print;
}
END_TEST

Suite* mount_test_suite(){
	Suite* s = suite_create("Tests for disk (un)mount");

	Add_Test(s, mount_null_param);
	Add_Test(s, mount_inexisting_file);
	Add_Test(s, mount_bad_boot_sector);
	Add_Test(s, mount_empty_file);
	Add_Test(s, mount_no_sb);
	Add_Test(s, unmount_null_param);
	Add_Test(s, mount_valid);

	return s;
}

TEST_SUITE(mount_test_suite)
