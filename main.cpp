#include "PgmImage.h"
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include "args.h" 
  
bool is_file_supported(const std::string& filename, std::string& filetype)
{
    const char* pch = strrchr(filename.c_str(), '.');
    filetype = filename.substr(pch - filename.c_str() + 1);
    if (filetype == "pgm")
    {
        return true;
    }

    return false;
}

template <typename T>
T* load_image(const std::string& filename)
{
    std::string filetype = "";
    if (is_file_supported(filename, filetype))
    {
        if (filetype == "pgm")
        {
            return new PgmImage(filename);
        }
    }
    return nullptr;
}

// funkcja odpowiedzialna za przetworzenie wszystkich instrukcji
// znajdujacych sie w wektorze
void GenerateImage(std::vector<string>& instructions)
{ 
    PgmImage* pgm = NULL; 
     
    int i = 0;

    while (i < instructions.size())
    {
        std::string command = instructions[i++];

        if (command == ARG_STR_INPUT)
        {
            pgm = new PgmImage(instructions[i++]);
        }
        else if (command == ARG_INT_RESOLUTION)
        {
            int x = stoi(instructions[i++]);
            int y = stoi(instructions[i++]);
           
            pgm->ResizeImage(x, y); 
        }
        else if (command == ARG_INT_BINARY_IMAGE)
        {
            int threshold = stoi(instructions[i++]);

            pgm->BinaryImage(threshold); 
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
            pgm->GradientImage();
        }
        else if (command == ARG_BOOL_REDUCE_NOISE)
        { 
            pgm->ReduceNoise();
        }
        else if (command == ARG_BOOL_APPLY_BLUR)
        {
            std::string method = instructions[i++];
            pgm->ApplyBlur(method);
        }
        else if (command == ARG_STR_OUTPUT)
        {  
            pgm->SaveImage(instructions[i++]);
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