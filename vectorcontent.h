#ifndef VECTORCONTENT_H
#define VECTORCONTENT_H
#include<QVector>
#include"vectorcontentson.h"

class VectorContent
{
public:
    double CoordY;
    QVector<VectorContentSon> son;

};

#endif // VECTORCONTENT_H
