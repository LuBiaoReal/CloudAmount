#ifndef DAILYDISPERSEDATA_H
#define DAILYDISPERSEDATA_H
class dailyDisperseData
{
public:
    double CoordY;
    double CoordX;
    double CloudAmount;
    bool operator <(const dailyDataSon &m)const{
        return XCoord > m.XCoord;
};
#endif // DAILYDISPERSEDATA_H
