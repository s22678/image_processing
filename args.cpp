#include <algorithm>
#include <iostream>
#include <string> 
#include <vector>
#include "args.h"   

inline char* to_lower_case(char* str)
{
	for (auto ptr = str; *ptr != '\0'; ptr++)
	{
		*ptr = tolower(*ptr);
	}
	return str;
}

void Args::parse(int argc, char* argv[])
{
	for (int i = 1; i < argc ; i++)
	{
		if (ARG_INT_RESOLUTION.compare(argv[i]) == 0)
		{
			int x = stoi(argv[++i]); 
			int y = stoi(argv[++i]);  

			instructions.push_back(ARG_INT_RESOLUTION);
			instructions.push_back(std::to_string(x));
			instructions.push_back(std::to_string(y));
		}
		else if (ARG_STR_INPUT.compare(argv[i]) == 0)
		{
			std::string input = to_lower_case(argv[++i]); 
	
			instructions.push_back(ARG_STR_INPUT);
			instructions.push_back(input);
		}  
		else if (ARG_INT_BINARY_IMAGE.compare(argv[i]) == 0)
		{
			int threshold = stoi(argv[++i]); 
			 
			instructions.push_back(ARG_INT_BINARY_IMAGE);
			instructions.push_back(std::to_string(threshold));
		}
		else if (ARG_INT_ROTATE_IMAGE.compare(argv[i]) == 0)
		{
			int rotation = stoi(argv[++i]); 

			instructions.push_back(ARG_INT_ROTATE_IMAGE);
			instructions.push_back(std::to_string(rotation));
		}
		else if (ARG_BOOL_NEGATIVE.compare(argv[i]) == 0)
		{
			instructions.push_back(ARG_BOOL_NEGATIVE);
		}
		else if (ARG_BOOL_IMAGE_DILATION.compare(argv[i]) == 0)
		{
			instructions.push_back(ARG_BOOL_IMAGE_DILATION);
		}
		else if (ARG_BOOL_IMAGE_EROSION.compare(argv[i]) == 0)
		{
			instructions.push_back(ARG_BOOL_IMAGE_EROSION); 
		}
		else if (ARG_STR_OUTPUT.compare(argv[i]) == 0)
		{ 
			std::string method = to_lower_case(argv[++i]);

			instructions.push_back(ARG_STR_OUTPUT);
			instructions.push_back(method);
		}
		else if (ARG_BOOL_GRADIENT_IMAGE.compare(argv[i]) == 0)
		{
			std::string method = to_lower_case(argv[++i]);
			instructions.push_back(ARG_BOOL_GRADIENT_IMAGE);
			instructions.push_back(method);
		}
		else if (ARG_BOOL_REDUCE_NOISE.compare(argv[i]) == 0)
		{
			instructions.push_back(ARG_BOOL_REDUCE_NOISE);
		}
		else if (ARG_BOOL_APPLY_BLUR.compare(argv[i]) == 0)
		{
			std::string method = to_lower_case(argv[++i]);

			instructions.push_back(ARG_BOOL_APPLY_BLUR);
			instructions.push_back(method);
		}
		else if (ARG_BOOL_DISPLAY_HELP.compare(argv[i]) == 0)
		{
			instructions.push_back(ARG_BOOL_DISPLAY_HELP);
		}
		else
		{
			std::cout << "Nierozpoznano polecenia: " << argv[i] << std::endl;
			exit(0);
		}
	}
}

bool Args::validate() const
{
	bool status = true;
  
	int i = 0;
	while (i < instructions.size())
	{
		std::string command = instructions[i++];
		if (command == ARG_BOOL_DISPLAY_HELP)
		{
			if (instructions.size() > 1)
			{
				cerr << "flaga -h nie może być łączona z innymi flagami " << endl;
				std::cout << "Wydrukuj pomoc" << std::endl;
				return false;
			}
			else
			{
				std::cout << "Wydrukuj pomoc" << std::endl;
			}
		}
		else if (command == ARG_INT_RESOLUTION)
		{
			int x = stoi(instructions[i++]);
			int y = stoi(instructions[i++]);

			if (y == 0 || x == 0)
			{
				cerr << "Bledna wartosc argumentu " << ARG_INT_RESOLUTION << endl;
				status = false;
			}
		}
		else if (command == ARG_STR_INPUT)
		{
			fs::path input = fs::path(instructions[i++]);

			if (input.extension() != ".pgm" && input.extension() != ".pbm") // Heed the dot.
			{
				std::cout << input.filename() << " is an invalid input type." << endl; // Output: e.g. "myFile.cfg is an invalid type"
				status = false;
			}
			else if (input.empty() || !std::filesystem::exists(input))
			{
				std::cout << "Bledna wartosc argumentu " << ARG_STR_INPUT << endl;
				status = false;
			}
		}
		else if (command == ARG_STR_OUTPUT)
		{
			fs::path output = fs::path(instructions[i++]);

			if (output.extension() != ".pgm" && output.extension() != ".pbm") // Heed the dot.
			{
				std::cout << output.filename() << " is an invalid output type." << endl; // Output: e.g. "myFile.cfg is an invalid type"
				status = false;
			}
		}
		else if (command == ARG_INT_BINARY_IMAGE)
		{
			int threshold = stoi(instructions[i++]);

			if (threshold < 0 || threshold > 65535)
			{
				cerr << "Bledna wartosc argumentu " << ARG_INT_BINARY_IMAGE << endl;
				status = false;
			}
		}
		else if (command == ARG_BOOL_APPLY_BLUR)
		{
			std::string method = instructions[i++];

			std::cout << "metoda: " << method << std::endl;

			if (method != "average" && method != "gauss")
			{
				cerr << "Zla metoda rozmycia. Wybierz 'gauss' albo 'average' " << " nie mozna zastosowac flagi " << ARG_BOOL_APPLY_BLUR << endl;
				status = false;
			}
		}
		else if (command == ARG_BOOL_GRADIENT_IMAGE)
		{
			std::string method = instructions[i++];

			std::cout << "metoda: " << method << std::endl;

			if (method != "1" && method != "2" && method != "3")
			{
				cerr << "Zly filtr gradientu. Wybierz '1' dla metody Sobela, '2' dla Prewitta, albo '3' dla Robertsa " << " nie mozna zastosowac flagi " << ARG_BOOL_APPLY_BLUR << endl;
				status = false;
			}
		}
		else if (command == ARG_BOOL_IMAGE_DILATION)
		{
			cerr << "Obraz wejsciowy nie jest obrazem binarnym " << " nie mozna zastosowac flagi " << ARG_BOOL_IMAGE_DILATION << endl;
			status = false;
		}
		else if (command == ARG_BOOL_IMAGE_EROSION)
		{
			cerr << "Obraz wejsciowy nie jest obrazem binarnym " << " nie mozna zastosowac flagi " << ARG_BOOL_IMAGE_EROSION << endl;
			status = false;
		}
	}
	return status;
}
 
Args::Args(std::vector<std::string>& instr)
	: instructions(instr)
{

}