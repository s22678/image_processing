#pragma once 

#include "Image.h"

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

class PgmImage : public Image
{
private:
    int width;
    int height;
    int resolution;
    short grey_scale;
    std::vector<short> pixels;
public:
    PgmImage(const std::string& filename);
    void ReadHeader(std::ifstream& input);
    void ReadPixels(std::ifstream& input);
    void ResizeImage(int new_width, int new_height);
    void RotateImage(float ang);
    void BinaryImage(const short& treshold);
    void ApplyBlur(const std::string& method);
    void ReduceNoise();
    void NegativeImage();
    void GradientImage(const std::string& method);
    void SaveImage(const std::string& filename);

    // metody wewnętrzne
    int len_trim(std::string s);
    void extract_word(std::string s, std::string& s1, std::string& s2);

    template<typename T>
    void BubbleSort(std::vector<T>& vec);
};