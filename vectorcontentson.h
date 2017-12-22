#ifndef VECTORCONTENTSON_H
#define VECTORCONTENTSON_H
#include<QVector>

class VectorContentSon
{
public:
    double CoordX;
    QVector<double> CloudAount;
    VectorContentSon& operator =(const VectorContentSon &other)
    {
        //千万不要返回一个局部对象给引用
        this->CoordX = other.CoordX;
        this->CloudAount = other.CloudAount;
        return *this;
    }
};

#endif // VECTORCONTENTSON_H
