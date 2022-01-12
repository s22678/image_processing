#pragma once 

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

class PbmImage
{
private:
    int width;
    int height;
    int resolution;
    std::vector<uint8_t> pixels;

public:
    enum image_part
    {
        LEFT_TOP,
        LEFT_BOTTOM,
        RIGHT_TOP,
        RIGHT_BOTTOM,
        LEFT_EDGE,
        TOP_EDGE,
        RIGHT_EDGE,
        BOTTOM_EDGE,
        CENTER
    };

    PbmImage(const std::string& filename);
    void ReadHeader(std::ifstream& input);
    void ReadPixels(std::ifstream& input);
    void ResizeImage(int new_width, int new_height);
    void RotateImage(float ang);
    void ErodeImage();
    image_part CheckBorderType(const int& y, const int& x);
    void DilateImage();
    void NegativeImage();
    void PrintImage();
    void SaveImage(const std::string& filename);
    int len_trim(std::string s);
    void extract_word(std::string s, std::string& s1, std::string& s2);

    template<typename T>
    void BubbleSort(std::vector<T>& vec);
};