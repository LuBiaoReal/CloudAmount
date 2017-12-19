#ifndef DAILYDATASON_H
#define DAILYDATASON_H
class dailyDataSon
{
public:
    double XCoord = 0;
    double CloudMount = 0;
    bool operator <(const dailyDataSon &m)const{
        return XCoord > m.XCoord;
    }
};
#endif // DAILYDATASON_H
