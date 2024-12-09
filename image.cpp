#include "image.h"
#include <QDebug>

Image::Image() {}

void Image::parseP1(const QByteArray &ba)
{
    QList<QByteArray> baList = ba.split('\n');

    for (auto i = baList.cbegin(), end = baList.cend(); i != end; ++i)
    {
        if (*i[0] == '#') continue;
        qDebug() << *i;
    }

    QTextStream stream(ba);
    QString buffer;
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
    int bit;
    image = new int**[height];
    for (int row = 0; row < height; row++)
    {
        image[row] = new int*[width];
        for (int col = 0; col < width; col++)
        {
            image[row][col] = new int[3];
            stream >> bit;
            if (bit == 1)
            {
                image[row][col][0] = 255;
                image[row][col][1] = 255;
                image[row][col][2] = 255;
            }
            else
            {
                image[row][col][0] = 0;
                image[row][col][1] = 0;
                image[row][col][2] = 0;
            }
        }
    }
}

void Image::printImage()
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            qDebug() << image[row][col][0] << "," << image[row][col][1] << "," << image[row][col][2] << " ";
        }
        qDebug() << '\n';
    }
}
