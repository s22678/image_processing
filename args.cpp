#include <algorithm>
#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include "args.h"   

/**
 * @brief drukuje pomoc z pliku help_info z tego samego katalogu
 * 
 */
void print_help()
{
    std::ifstream f("./help_info");

    if (f.is_open())
	{
		std::cout << f.rdbuf();
	}
	std::cout << "\n\n";
}

/**
 * @brief zamien wszystkie znaki lancucha na lowercase
 * 
 * @param char* str 
 * @return char* 
 */
inline char* to_lower_case(char* str)
{
	for (auto ptr = str; *ptr != '\0'; ptr++)
	{
		*ptr = tolower(*ptr);
	}
	return str;
}

/**
 * @brief rozbij ciąg przekazanych flag do programu na flagi. porownuje kolejne elementy argv[] z lancuchami zdefiniowanymi w naglowku, a potem dodaje odpowiednie polecenia do kolejki
 * 
 * @param int argc - liczba argumentow (przekazana z main)
 * @param char*argv[] - argumenty (przekazane z main)
 */
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
		else if (ARG_BOOL_DISPLAY_HELP.compare(argv[i]) == 0 || argc == 1)
		{
			instructions.push_back(ARG_BOOL_DISPLAY_HELP);
		}
		else
		{
			std::cout << "Nierozpoznano polecenia: " << argv[i] << '\n';
			print_help();
			exit(0);
		}
	}
}

/**
 * @brief przechodzi po kolei przez vector instrukcji i wykrywa bledy ktore mogly byc podane w parametrach (np brak pliku, brak podanej rozdzielczosci flagi -rs, niepoprawne wartosci itp ).
 * 
 * @return bool 
 */
bool Args::validate() const
{
	bool status = true;

	if (instructions.size() == 0)
	{
		print_help();
		return false;
	}
  
	int i = 0;
	while (i < instructions.size())
	{
		std::string command = instructions[i++];
		if (command == ARG_BOOL_DISPLAY_HELP)
		{
			if (instructions.size() > 1)
			{
				cerr << "flaga -h nie może być łączona z innymi flagami " << endl;
				print_help();
				return false;
			}
			else
			{
				print_help();
				return false;
			}
		}
		else if (command == ARG_INT_RESOLUTION)
		{
			try
			{
				int x = stoi(instructions[i++]);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << ": podany zly parametr zmiany rozdzielczosci, podaj liczbe calkowita";
			}
			
			try
			{
				int y = stoi(instructions[i++]);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << ": podany zly parametr zmiany rozdzielczosci, podaj liczbe calkowita";
			}

			if (y == 0 || y > 5000 || x == 0 || x > 5000)
			{
				cerr << "Bledna wartosc argumentu. wartosc powinna byc pomiedzy 1 a 5000 " << ARG_INT_RESOLUTION << endl;
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
				std::cout << "Bledna wartosc argumentu! plik pusty albo nie istnieje (albo nie ma do niego dostepu) " << ARG_STR_INPUT << endl;
				status = false;
			}
		}
		else if (command == ARG_STR_OUTPUT)
		{
			fs::path output = fs::path(instructions[i++]);

			if (output.extension() != ".pgm" && output.extension() != ".pbm") // Heed the dot.
			{
				std::cout << output.filename() << " zly plik, podaj scieze pliku flagi -o z rozszerzeniem pbm albo pgm" << endl; // Output: e.g. "myFile.cfg is an invalid type"
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
	}
	return status;
}
 
Args::Args(std::vector<std::string>& instr)
	: instructions(instr)
{

}