#ifndef DAILYDATA_H
#define DAILYDATA_H
#include<QVector>
#include"dailydatason.h"

class dailyData
{
public:

    double YCoord = 0;
    QVector<dailyDataSon> son;
    bool operator <(const dailyData &m)const{
        return YCoord > m.YCoord;
    }
};

#endif // DAILYDATA_H
