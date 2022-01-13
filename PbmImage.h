#pragma once 

#include "Image.h"
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

class PbmImage : public Image
{
private:
    int width;
    int height;
    int resolution;
    std::vector<uint16_t> pixels;
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

    enum image_type
    {
        P1,
        P4
    } it;

public:

    // metody zewnętrzne
    PbmImage(const std::string& filename);
    void ReadHeader(std::ifstream& input);
    void ReadPixels(std::ifstream& input);
    void ResizeImage(int new_width, int new_height);
    void RotateImage(float ang);
    void ErodeImage();
    void DilateImage();
    void NegativeImage();
    void SaveImage(const std::string& filename);

    // metody wewnętrzne
    int len_trim(std::string s);
    void extract_word(std::string s, std::string& s1, std::string& s2);
    image_part CheckBorderType(const int& y, const int& x);
    
    template<typename T>
    void BubbleSort(std::vector<T>& vec);

    template<class T>
    uint16_t compare_pixels(char lv, char rv, T func);
};