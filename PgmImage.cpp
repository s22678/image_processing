#include "PgmImage.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <cmath>
#define M_PI 3.14159265358979323846

PgmImage::PgmImage(const std::string& filename)
{
    std::ifstream input_stream(filename);

    if (input_stream.is_open()) // gdy plik zostal otwarty wykonujemy ponizsze operacje
    {
        ReadHeader(input_stream); // czytam naglowek pliku
        
        pixels = std::vector<short>(width * height); // rezerwujemy odpowiednia ilosc pamieci w wektorze

        ReadPixels(input_stream); // wczytujemy piksele z obrazu
    }
    else
    {
        std::cout << "Error. Unable to open " << filename << std::endl;
    }

    input_stream.close(); 
}

void PgmImage::PrintImage()
{
    std::cout << "P2\n";
    std::cout << width;
    std::cout << " ";
    std::cout << height << "\n";
    std::cout << grey_scale << "\n";

    for (unsigned int i = 0; i < width*height; ++i)
    {
        std::cout << pixels[i] << " ";
        if (i % width) std::cout << "\n";
    }
}

void PgmImage::SaveImage(const std::string& filename)
{
    std::ofstream input_stream(filename.c_str(), std::ios::out | std::ios::binary);
    if (input_stream.is_open())
    {
        input_stream << "P2\n";
        input_stream << width;
        input_stream << " ";
        input_stream << height << "\n";
        input_stream << grey_scale << "\n";

        short aux;
        for (unsigned int j = 0; j < height; j++)
        {
            for (unsigned int i = 0; i < width; ++i)
            {
                aux = pixels[j*width + i];
                input_stream << aux << " ";
            }
            input_stream << "\n";
        }
    }
    else
    {
        std::cout << "Error. Unable to open " << filename << std::endl;
    }
    input_stream.close();
}

void PgmImage::RotateImage(float angle)
{
    // tworzymy tymczasowy wektor ktory bedzie przechowywac przetworzony obraz
    std::vector<short>temp(width * height); 
    
    // konwertujemy stopnie na radiany,
    // funkcja takie jak sin czy cos ze standardowej biblioteki
    // jako parametr przyjmuja liczbe w radianach
    float _angle = (-angle * M_PI) / 180;
    
    // znajdujemy srodkowy piksel,
    // bedzie to pozycja wokol ktorej bedziemy wykonywac obrot 
    float xs = width / 2.0;
    float ys = height / 2.0;

     for (int y = 0; y < height; y++)
        {
         for (int x = 0; x < width; x++)
         {
             // wyznaczamy nowa pozycje piksela zgodnie z wzorem zawartym w instrukcji pdf
             int xp = int(cos(_angle) * (x - xs) - sin(_angle) * (y - ys) + xs);
             int yp = int(sin(_angle) * (x - xs) + cos(_angle) * (y - ys) + ys);
            
             // sprawdzamy ktore z pikseli powinny zostac umieszczone w obrazie wynikowym
             if ((0 <= xp && xp < width) && ((0 <= yp && yp < height)))
                 temp[y * width + x] = pixels[yp * width + xp];
         }
     } 
     // podmieniamy piksele obrazu
     pixels = temp;   
}

void PgmImage::ResizeImage(int new_width, int new_height)
{
    // tworzymy tymczasowy wektor o rozmiarze nowego obrazu
    std::vector<short>temp(new_width * new_height);

    // szukamy wspolczynnika skalowania dla osi poziomej i pionowej
    float x_scale = width / (float)new_width;
    float y_scale = height / (float)new_height;

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            // szukamy jaki piksel w oryginalnym obrazie
            // bedzie odpowiadal pikselowi w nowym wiekszym obrazie
            // zgodnie z algorytmem nearest neighbor interpolation
            int xp = floor(x * x_scale);
            int yp = floor(y * y_scale);

            // umieszczamy piksel w przetworzonym obrazie
            temp[y * new_width + x] = pixels[yp * width + xp];
        }
    }

    // podmieniamy rozmiaru obrazu
    width = new_width;
    height = new_height;

    // podmieniamy piksele obrazu na nowe
    pixels = temp;
}


void PgmImage::BinaryImage(const short& treshold)
{
    if (treshold > grey_scale && treshold < 0)
    {
        std::cout << "Error. Invalid binary treshold. The value must be between 0 - " << grey_scale << std::endl;
    }
    else
    {
        // porownjemy piksel z progiem i ustawiamy odpowiednia wartosc dla niego
        std::for_each(pixels.begin(), pixels.end(), [&](short& x) { if (x > treshold) { return x = grey_scale; } else { return x = 0; } });
    }
}

// sortowanie babelkowe dla filtru medianowego
template<typename T>
void PgmImage::BubbleSort(std::vector<T>& v)
{
    for (size_t i = 0; i < v.size() - 1; ++i)
    {
        for (size_t j = 0; j < v.size() - i - 1; ++j)
        {
            if (v.at(j) > v.at(j + 1))
            {
                std::swap(v.at(j), v.at(j + 1));
            }
        }
    }
}

// filtr medianowy
void PgmImage::ReduceNoise()
{
    // tworzymy tymczasowy wektor na przetworzone piksele
    std::vector<short>temp(width * height);

    // wektor bedzie przechowywal piksele z okna 3x3
    // ktore pozniej zostana posortowane
    std::vector<uint16_t> median_filter(9); 

    int i = 0;
    int j = 0;

    for (uint32_t y = 1; y < height - 1; y++)
    {
        for (uint32_t x = 1; x < width - 1; x++)
        {
            i = 0;

            for (uint32_t a = 0; a < 3; a++)
            {
                for (uint32_t b = 0; b < 3; b++)
                {
                    int xx = x + b - 1;
                    int yy = y + a - 1;

                    // wrzucamy piksele tablicy
                    median_filter[i++] = pixels[yy * width + xx];
                }
            }

            // sortujemy piksele rosnaco
            BubbleSort(median_filter);

            // wybieramy srodkowy piksel i umieszczamy go w nowej tablicy z pikselami
            temp[j++] = median_filter[4];
        }
    }

    // obraz po przeprowadzeniu konwolucji jest o 2 piksele mniejszy niz wczesniej
    width  -= 2;
    height -= 2;

    // podmieniamy piksele obrazu na nowe
    pixels = temp;
}

void PgmImage::ApplyBlur(const std::string& method)
{ 
    // tworzymy tymczasowy wektor na przetworzone piksele
    std::vector<short>temp(width * height);

    // tworzymy maske
    const int MASK_HEIGHT_SIZE = 3;
    const int MASK_WIDTH_SIZE = 3;

    std::vector<float>mask(MASK_WIDTH_SIZE * MASK_HEIGHT_SIZE);

    // suma wag, przez którą dzielimy każdy piksel
    float coefficient = 0.0;

    // wartości macierzy maski dobrane w zależności od metody rozmycia
    if (method == "average")
    {
        mask = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
        coefficient = 9.0;
    }

    if (method == "gauss")
    {
        mask = {1.0, 2.0, 1.0, 2.0, 4.0, 2.0, 1.0, 2.0, 1.0};
        coefficient = 16.0;
    }
     
    for (uint32_t y = 1; y < height - 1; y++)
    {
        for (uint32_t x = 1; x < width - 1; x++)
        { 
            float pixel_value = 0;
             
            for (uint32_t a = 0; a < MASK_HEIGHT_SIZE; a++)
            {
                for (uint32_t b = 0; b < MASK_WIDTH_SIZE; b++)
                { 
                    int xx = x + b - 1;
                    int yy = y + a - 1;

                    pixel_value += pixels[yy * width + xx] * mask[a * MASK_WIDTH_SIZE + b];
                }
            }

            // dzielimy otrzymany kolor przez współczynnik (sume wartości macierzy maski) czyli usredniamy wynik
            pixel_value /= coefficient;
             
            // umieszczamy nowy 'rozmyty' piksel w tablicy
            temp[y * width + x] = static_cast<short>(pixel_value);
        }
    }

    // podmieniamy piksele obrazu na nowe
    pixels = temp;
}

void PgmImage::ReadHeader(std::ifstream& input)
{
    std::string line;
    std::string rest;
    int step;
    std::string word;

    step = 0;

    while (1)
    {
        std::getline(input, line); 

        if (input.eof()) 
        {
            std::cout << "error" << std::endl;
            exit(1);
        }

        if (line[0] == '#') // jesli # to komentarz, wiec pomijamy linie do konca
        {
            continue;
        }

        if (step == 0)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
            
            line = rest;

            // sprawdzamy naglowek pliku, powinien skalda sie z dwoch znakow - P2
            if ((word[0] != 'P' && word[0] != 'p') ||
                word[1] != '2')
            {
                std::cout << "error" << std::endl;
                exit(1);
            }
            step = 1;
        }

        // pobieramy kolejny parametr - szerokosc obrazu z naglowka
        if (step == 1)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
            
            width = atoi(word.c_str());
            line = rest;
            step = 2;
        }

        // pobieramy kolejny parametr - wysokosc obrazu z naglowka
        if (step == 2)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
           
            height = atoi(word.c_str());
            line = rest;
            step = 3;
        }

        // pobieramy kolejny parametr - sklae szarosci z naglowka
        if (step == 3)
        {
            extract_word(line, word, rest);

            if (len_trim(word) == 0)
                continue;
            
            grey_scale = atoi(word.c_str());
            break;
        }

    }

    return;
}


void PgmImage::extract_word(std::string s, std::string& s1, std::string& s2)
{
    int i, mode, len;

    len = s.length();
    s1 = s2 = ""; 

    mode = 1;

    for (i = 0; i < len; i++)
    {
        if (mode == 1)
        {
            if (s[i] != ' ')
                mode = 2;
        }
        else if (mode == 2)
        {
            if (s[i] == ' ')
                mode = 3;
        }
        else if (mode == 3)
        {
            if (s[i] != ' ')
                mode = 4;
        }
        if (mode == 2)
        {
            s1 = s1 + s[i];
        }
        else if (mode == 4)
        {
            s2 = s2 + s[i];
        }
    }

    return;
}
void PgmImage::GradientImage()
{
    float G1[3][3] = {
            {1,0,-1},
            {2,0,-2},
            {1,0,-1}
    };

    float G2[3][3] = {
            {1,2,1},
            {0,0,0},
            {-1,-2,-1}
    };

    std::vector<short>temp(width * height);
       

    for (uint32_t y = 1; y < height - 1; y++)
    {
        for (uint32_t x = 1; x < width - 1; x++)
        {

            float output_color = 0;
            float color1 = 0;
            float color2 = 0;

            for (uint32_t i = 0; i < 3; i++)
            {
                for (uint32_t j = 0; j < 3; j++)
                {
                    int xx = x + i - 1;
                    int yy = y + j - 1;
                     
                    color1 += pixels[yy * width + xx] * G1[i][j];
                    color2 += pixels[yy * width + xx] * G2[i][j];

                }
            }
            output_color = sqrt(color1 * color1 + color2 * color2);


            if (output_color > grey_scale)
                output_color = grey_scale;
            else if (output_color < 0)
                output_color  = 0;
             
            temp[y * width + x] = static_cast<short>(output_color);

        }
    }

    pixels = temp;
}

void PgmImage::NegativeImage()
{
    std::for_each(pixels.begin(), pixels.end(), [&](short& x) { x = grey_scale - x; return x; });
}

int PgmImage::len_trim(std::string s)
{
    int n;

    n = s.length();

    while (0 < n)
    {
        if (s[n - 1] != ' ')
            return n;
        n = n - 1;
    }

    return n;
}

void PgmImage::ReadPixels(std::ifstream& input)
{
    int idx = 0;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            short px;
            input >> px;
            pixels[idx++] = px; 
        }
    }

    return;
}