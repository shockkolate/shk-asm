#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include <shk/asm.h>

int main(int argc, char *argv[]) {
	char out_path[PATH_MAX];
	strcpy(out_path, "a.out");

	int opt;
	while((opt = getopt(argc, argv, "o:")) != -1) {
		switch(opt) {
		case 'o':
			strcpy(out_path, optarg);
			break;
		case '?':
			return 1;
		}
	}

	size_t in_count = argc - optind;
	if(in_count == 0) {
		std::cerr << argv[0] << ": no input files" << std::endl;
		return 1;
	}

	shk::assembler as;

	for(size_t i = 0; i < in_count; ++i) {
		char *in_path = argv[i + optind];
		std::cout << "in[" << i << "] = " << in_path << std::endl;

		std::ifstream is(in_path, std::ios::in);
		if(is.fail()) {
			std::cerr << argv[0] << ": " << in_path << ": failed to open" << std::endl;
			return 1;
		}

		if(!as.process(is)) {
			return 2;
		}
	}

	std::cout << "out_path = " << out_path << std::endl;
	std::ofstream os(out_path, std::ios::out | std::ios::binary | std::ios::trunc);

	as.resolve();
	as.encode(os);

	return 0;
}
