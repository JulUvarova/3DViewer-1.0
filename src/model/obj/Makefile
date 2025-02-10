.PHONY: all
all: 
	g++ -O2 obj_data.cc obj_parser_test.cc -o obj_parser_test -I../../include
	./obj_parser_test

profile: 
	g++ -O2 -pg obj_data.cc obj_parser_test.cc -o obj_parser_test
	./obj_parser_test
	gprof obj_parser_test gmon.out > profile.txt