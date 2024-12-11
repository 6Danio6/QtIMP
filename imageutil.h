#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include <QImage>

class ImageUtil
{
public:
    ImageUtil();

    static QImage* parseP1(const QByteArray& ba);
    static QImage* parseP2(const QByteArray& ba);
    static QImage* parseP3(const QByteArray& ba);
    static QImage* parseP4(const QByteArray& ba);
    static QImage* parseP5(const QByteArray& ba);
    static QImage* parseP6(const QByteArray& ba);

    static void saveP1(const QImage& img, QString fileName);
    static void saveP2(const QImage& img, QString fileName);
    static void saveP3(const QImage& img, QString fileName);
    static void saveP4(const QImage& img, QString fileName);
    static void saveP5(const QImage& img, QString fileName);
    static void saveP6(const QImage& img, QString fileName);

    static void printToDebug(const QImage& img);

    static QImage* desaturate(QImage* img);
    static QImage* invert(QImage* img);
    static QImage* contrast(QImage* img);
    static QImage* brightness(QImage* img, double multiplier);
};

#endif // IMAGEUTIL_H
