#ifndef IMAGE_H
#define IMAGE_H

#include <QString>

class Image
{
private:
    int width;
    int height;
    int*** image;

public:
    void parseP1(const QByteArray& ba);
    void printImage();

    Image();

    // void setWidth();
    // void setHeight();
    // int getWidth();
    // int getHeight();
    // int*** getImage();
};

#endif // IMAGE_H
