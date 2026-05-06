#include <check.h>
#include <stdio.h>

#include "test.h"
#include "error.h"
#include "sector.h"
#include "unixv6fs.h"

#define TEMP_FILE "cs202-test.txt"

void create_single_sector_file(uint8_t* data, const char* dump_file){
	FILE* f = fopen(dump_file, "w");
	fwrite(data, 1, SECTOR_SIZE, f);
	fclose(f);
}

START_TEST(sector_read_null_param){
	start_test_print;

	ck_assert_invalid_arg(sector_read(NULL, 1, (void*)1));
	ck_assert_invalid_arg(sector_read((void*)1, 1, NULL));
	ck_assert_invalid_arg(sector_read(NULL, 1, NULL));

	end_test_print;
}
END_TEST

START_TEST(sector_out_of_bounds){
	start_test_print;

	uint8_t sector[SECTOR_SIZE] = {0};
	create_single_sector_file(sector,  DATA_DIR "/dump.sector_out_of_bounds.uv6");
	FILE *f = fopen( DATA_DIR "/dump.sector_out_of_bounds.uv6", "r");
	ck_assert_int_eq(sector_read(f, 2, (void*)1), ERR_IO);

	end_test_print;
}
END_TEST

START_TEST(sector_valid){
	start_test_print;

	uint8_t expected[SECTOR_SIZE] = {0};
	uint8_t sector[SECTOR_SIZE] = {0};
	strcpy((char*)expected, "hello world!");

	create_single_sector_file(expected, DATA_DIR "/dump.sector_valid.uv6");
	FILE *f = fopen(DATA_DIR "/dump.sector_valid.uv6", "r");

	ck_assert_int_eq(sector_read(f, 0, sector), ERR_NONE);
	ck_assert_mem_eq(sector, expected, SECTOR_SIZE);
}
END_TEST

Suite* sector_test_suite(){
	Suite* s = suite_create("Tests for sector layer");

	Add_Test(s,  sector_read_null_param);
	Add_Test(s,  sector_out_of_bounds);
	Add_Test(s,  sector_valid);

	return s;
}

TEST_SUITE(sector_test_suite)
