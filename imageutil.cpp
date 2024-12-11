#include "imageutil.h"
#include <QTextStream>
#include <QDebug>
#include <cmath>
#include <fstream>

ImageUtil::ImageUtil() {}

// Other
void ImageUtil::printToDebug(const QImage& img)
{
    const uchar* imgData = img.bits();
    for (int row = 0; row < img.height(); row++)
    {
        for (int col = 0; col < img.width(); col++)
        {
            int offset = (row * img.width() + col) * 3;
            qDebug() << imgData[offset] << "," << imgData[offset+1] << "," << imgData[offset+2] << " ";
        }
        qDebug() << '\n';
    }
}

// Reading
QImage* ImageUtil::parseP1(const QByteArray &ba)
{
    QImage* img;
    QTextStream stream(ba);
    QString buffer;
    int width;
    int height;
    int currentByte;

    stream >> buffer;

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    width = buffer.toInt();

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    height = buffer.toInt();

    // raster
    img = new QImage(width, height, QImage::Format_RGB888);
    uchar* imgData = img->bits();
    for (int i = 0; i < width * height; i++)
    {
        stream >> currentByte;
        if (currentByte == 1)
        {
            imgData[3*i] = (uchar)0;
            imgData[3*i + 1] = (uchar)0;
            imgData[3*i + 2] = (uchar)0;
        }
        else
        {
            imgData[3*i + 0] = (uchar)255;
            imgData[3*i + 1] = (uchar)255;
            imgData[3*i + 2] = (uchar)255;
        }
    }

    return img;
}

QImage* ImageUtil::parseP2(const QByteArray &ba)
{
    QImage* img;
    QTextStream stream(ba);
    QString buffer;
    int width;
    int height;
    int depth;
    int currentByte;

    stream >> buffer;

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    width = buffer.toInt();

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    height = buffer.toInt();

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    depth = buffer.toInt();

    // raster
    img = new QImage(width, height, QImage::Format_RGB888);
    uchar* imgData = img->bits();
    for (int i = 0; i < width * height; i++)
    {
        stream >> currentByte;
        imgData[3*i] = (uchar)(255 * currentByte/depth);
        imgData[3*i + 1] = (uchar)(255 * currentByte/depth);
        imgData[3*i + 2] = (uchar)(255 * currentByte/depth);
    }

    return img;
}

QImage* ImageUtil::parseP3(const QByteArray &ba)
{
    QImage* img;
    QTextStream stream(ba);
    QString buffer;
    int width;
    int height;
    int depth;
    int currentByte;

    stream >> buffer;

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    width = buffer.toInt();

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    height = buffer.toInt();

    stream >> buffer;
    while (buffer[0] == '#')
    {
        stream.readLine();
        stream >> buffer;
    }
    depth = buffer.toInt();

    // raster
    img = new QImage(width, height, QImage::Format_RGB888);
    uchar* imgData = img->bits();
    for (int i = 0; i < width * height * 3; i++)
    {
        stream >> currentByte;
        imgData[i] = (uchar)(255 * currentByte/depth);
    }

    return img;
}

// Writing
void ImageUtil::saveP1(const QImage& img, QString fileName)
{
    std::ofstream out(fileName.toStdString());
    out << "P1";
    out << img.width() << img.height();
    const uchar* imgData = img.constBits();
    for (int i = 0; i < img.width() * img.height(); i++)
    {
        if (imgData[3*i] + imgData[3*i + 1] + imgData[3*i + 2] >= 384) out << 1;
        else out << 0;
    }
}

void ImageUtil::saveP2(const QImage& img, QString fileName)
{
    std::ofstream out(fileName.toStdString());
    out << "P2";
    out << img.width() << img.height();
    out << 255;
    const uchar* imgData = img.constBits();
    for (int i = 0; i < img.width() * img.height(); i++)
    {
        out << (imgData[3*i] + imgData[3*i + 1] + imgData[3*i + 2]) / 3;
    }
}

void ImageUtil::saveP3(const QImage& img, QString fileName)
{
    std::ofstream out(fileName.toStdString());
    out << "P3";
    out << img.width() << img.height();
    out << 255;
    const uchar* imgData = img.constBits();
    for (int i = 0; i < img.width() * img.height() * 3; i++)
    {
        out << imgData[i];
    }
}

// Filters
QImage* ImageUtil::desaturate(QImage* img)
{
    QImage* newImg = new QImage;
    *newImg = img->copy();
    uchar* imgData = newImg->bits();
    for (int i = 0; i < newImg->width() * newImg->height(); i++)
    {
        int level = (double)imgData[i*3] * 0.299 + (double)imgData[i*3 + 1] * 0.587 + (double)imgData[i*3 + 2] * 0.114;
        imgData[i*3] = (uchar)level;
        imgData[i*3 + 1] = (uchar)level;
        imgData[i*3 + 2] = (uchar)level;
    }
    return newImg;
}

QImage* ImageUtil::invert(QImage* img)
{
    QImage* newImg = new QImage;
    *newImg = img->copy();
    uchar* imgData = newImg->bits();
    for (int i = 0; i < newImg->width() * newImg->height() * 3; i++)
    {
        imgData[i] = (uchar)(255 - imgData[i]);
    }
    return newImg;
}

QImage* ImageUtil::contrast(QImage* img, double multiplier)
{
    QImage* newImg = new QImage;
    *newImg = img->copy();
    uchar* imgData = newImg->bits();
    double arr[256];
    for (int x = 0; x < 256; x++)
    {
        double localX = (double)x / 255;
        double exp = std::exp(multiplier * (localX - 0.5));
        arr[x] = (exp / (exp + 1)) * 255;
        qDebug() << x << ": " << arr[x];
    }

    for (int i = 0; i < newImg->width() * newImg->height() * 3; i++)
    {
        // double x = (double)imgData[i] / 255;
        // double result = std::pow(x, std::pow(10, multiplier));

        imgData[i] = (uchar)(arr[imgData[i]]);
    }
    return newImg;
}


QImage* ImageUtil::brightness(QImage* img, double multiplier)
{
    QImage* newImg = new QImage;
    *newImg = img->copy();
    uchar* imgData = newImg->bits();
    double arr[256];
    for (int x = 0; x < 256; x++)
    {
        double localX = (double)x / 255;
        double exp = std::exp(multiplier * (localX - 0.5));
        arr[x] = (exp / (exp + 1)) * 255;
        qDebug() << x << ": " << arr[x];
    }

    for (int i = 0; i < newImg->width() * newImg->height() * 3; i++)
    {
        // double x = (double)imgData[i] / 255;
        // double result = std::pow(x, std::pow(10, multiplier));

        imgData[i] = (uchar)(arr[imgData[i]]);
    }
    return newImg;
}
