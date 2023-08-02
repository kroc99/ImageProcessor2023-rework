//
// Created by Kristian O'Connor on 3/10/23.
//
#pragma once
#include <iostream>

#include <vector>
using namespace std;

struct Header{
    char  idLength;
    char  colorMapType;
    char  dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char  colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char  bitsPerPixel;
    char  imageDescriptor;
};
struct pixel{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
struct image{
    Header header;
    vector<pixel> pixels;
};
