#ifndef DAILYDISPERSEDATA_H
#define DAILYDISPERSEDATA_H
class dailyDisperseData
{
public:
    double CoordY;
    double CoordX;
    double CloudAmount;
    double len = -1;
    bool operator <(const dailyDisperseData &m)const{
        return CoordY > m.CoordY;}
    dailyDisperseData& operator =(const dailyDisperseData other)
    {
        this->CoordX = other.CoordX;
        this->CoordY = other.CoordY;
        this->CloudAmount = other.CloudAmount;
        return *this;
    }
};
#endif // DAILYDISPERSEDATA_H
