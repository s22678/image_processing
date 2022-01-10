#pragma once

#include <string>
#include <filesystem> // C++17 

namespace fs = std::filesystem;
using namespace std;

static const string ARG_INT_RESOLUTION = "-rs";
static const string ARG_STR_INPUT = "-i";
static const string ARG_INT_BINARY_IMAGE = "-ib";
static const string ARG_INT_ROTATE_IMAGE = "-r";
static const string ARG_STR_OUTPUT = "-o";
static const string ARG_BOOL_NEGATIVE = "-n";
static const string ARG_BOOL_IMAGE_DILATION = "-d";
static const string ARG_BOOL_IMAGE_EROSION = "-e"; 
static const string ARG_BOOL_GRADIENT_IMAGE = "-g";
static const string ARG_BOOL_REDUCE_NOISE = "-dn";
static const string ARG_BOOL_APPLY_BLUR = "-b";
static const string ARG_BOOL_DISPLAY_HELP = "-h";

struct Args {   
	std::vector<std::string>& instructions;

	void parse(int argc, char* argv[]);
	bool validate() const;

	Args();
	Args(std::vector<std::string>& instr);
};
