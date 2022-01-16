#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <algorithm>

#include "args.h"
#include "PgmImage.h"
#include "PbmImage.h"
#include "Image.h"

// funkcja odpowiedzialna za przetworzenie wszystkich instrukcji
// znajdujacych sie w wektorze
void GenerateImage(std::vector<string>& instructions)
{
    Image* pgm = nullptr;

    for (int i = 0; i < instructions.size(); ++i)
    {
        if (instructions[i] == "-i" && i < instructions.size())
        {
            std::string filename = instructions[i+1];
            std::transform(filename.begin(), filename.end(), filename.begin(), [](unsigned char c){ return std::tolower(c); });
            const char* pch = strrchr(filename.c_str(), '.');
            std::string filetype = filename.substr(pch - filename.c_str() + 1);

            if (filetype == "pgm")
            {
                pgm = new PgmImage(filename);
            }
            else if (filetype == "pbm")
            {
                pgm = new PbmImage(filename);
            }
        }
        // else
        // {
        //     throw std::invalid_argument("Nie znaleziono sciezki do pliku");
        // }
    }

    if (pgm == nullptr)
    {
        std::cout << "cos poszlo nie tak" << '\n';
        exit(1);
    }
    int i = 0;

    while (i < instructions.size())
    {
        std::string command = instructions[i++];

        // if (command == ARG_STR_INPUT)
        // {
        //     pgm = new PgmImage(instructions[i++]);
        // }
        if (command == ARG_INT_RESOLUTION)
        {
            int x = stoi(instructions[i++]);
            int y = stoi(instructions[i++]);
           
            pgm->ResizeImage(x, y); 
        }
        else if (command == ARG_INT_BINARY_IMAGE)
        {
            int threshold = stoi(instructions[i++]);

             static_cast<PgmImage*>(pgm)->BinaryImage(threshold); 
        }
        else if (command == ARG_BOOL_NEGATIVE)
        {
            pgm->NegativeImage();
        }
        else if (command == ARG_INT_ROTATE_IMAGE)
        { 
            int angle = stoi(instructions[i++]);

            pgm->RotateImage(angle);
        }
        else if (command == ARG_BOOL_GRADIENT_IMAGE)
        {
            std::string method = instructions[i++];
            static_cast<PgmImage*>(pgm)->GradientImage(method);
        }
        else if (command == ARG_BOOL_REDUCE_NOISE)
        { 
             static_cast<PgmImage*>(pgm)->ReduceNoise();
        }
        else if (command == ARG_BOOL_APPLY_BLUR)
        {
            std::string method = instructions[i++];
             static_cast<PgmImage*>(pgm)->ApplyBlur(method);
        }
        else if (command == ARG_STR_OUTPUT)
        {  
            pgm->SaveImage(instructions[i++]);
        }
		else if (command == ARG_BOOL_IMAGE_DILATION)
		{
			 static_cast<PbmImage*>(pgm)->DilateImage();
		}
		else if (command == ARG_BOOL_IMAGE_EROSION)
		{
			 static_cast<PbmImage*>(pgm)->ErodeImage();
		}
    }
}

int main(int argc, char* argv[])
{
    // wektor ma za zadanie przechowac instrukcje z danymi podane
    // w konsoli przez uzytkownika
    // w takiej kolejnosci w jakiej zostaly wprowadzone do konsoli
    std::vector<string> instructions;

    Args args(instructions);
    args.parse(argc, argv); // parsujemy tekst podany w konsoli na pojedyncze polecenia i dane
                            // informacje te laduja w wektorze instructions

    // jesli uzytkownik wprowadzil poprawne polecenie, ponizszy kod zostanie wykonany
    if (args.validate())
    {
        std::cout << "Trwa wykonywanie operacji na obrazie..." << std::endl;

        GenerateImage(instructions); // generujemy obraz wedlug podanych polecen
        return 0;
    } 

    std::cout << "Polecenia nie zostaly wykonane" << std::endl;
    return 0;
}