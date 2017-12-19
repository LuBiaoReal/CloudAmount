#ifndef DATAPROCESS_H
#define DATAPROCESS_H
#include"dailyData.h"
#include"dailydatason.h"
#include<QVector>
#include<QPointF>
#include"vectorcontent.h"
#include"vectorcontentson.h"

class DataProcess
{
public:
    DataProcess(QVector<QStringList>,QString, QVector<VectorContent>);
    QVector<QStringList> monthsFiles;
    QString locatePath;//放置文件的路径
    QVector<QPointF> baseCoord;
    QVector<QVector<dailyData>> oneMonthData;
};

#endif // DATAPROCESS_H
