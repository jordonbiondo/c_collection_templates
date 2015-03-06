src_dir=../src

all: coverage run_unit_tests

unit_tests: cct_tests
	gcc -O0 -g -Wall -pedantic -std=c99 cct_tests.c -o cct_tests

coverage:
	gcc -O0 -g -fprofile-arcs -ftest-coverage -Wall -pedantic -std=c99 cct_tests.c -o cct_coverage_tests

run_unit_tests: unit_tests
	./cct_tests -v
