#pragma once 

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

class Image
{
public:
    virtual void ResizeImage(int new_width, int new_height) = 0;
    virtual void RotateImage(float ang) = 0;
    virtual void NegativeImage() = 0;
    virtual void SaveImage(const std::string& filename) = 0;
    
    // metody wewnętrzne
    virtual void ReadHeader(std::ifstream& input) = 0;
    virtual void ReadPixels(std::ifstream& input) = 0;

    virtual int len_trim(std::string s) = 0;
    virtual void extract_word(std::string s, std::string& s1, std::string& s2) = 0;
};