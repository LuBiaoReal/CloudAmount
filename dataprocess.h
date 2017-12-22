#ifndef DATAPROCESS_H
#define DATAPROCESS_H
#include<QVector>
#include<QPointF>
#include"vectorcontent.h"
#include"vectorcontentson.h"
#include"dailydispersedata.h"
#include"progresswidget.h"

class DataProcess
{
public:
    DataProcess(QVector<QStringList>,QString, QVector<VectorContent>);
    QVector<QStringList> monthsFiles;//放置所读出来一年文件的路径
    QString locatePath;//放置处理后文件的路径
    QVector<VectorContent> baseCoord;//标准坐标的容器（能够放置一个月云量的数据）
   // QVector<QVector<dailyData>> oneMonthData;

    progressWidget progressW;
    QVector<dailyDisperseData> readFile(QString filePath);
    void process();
    void insertAmount(QVector<dailyDisperseData>,QVector<VectorContent> &);
    QVector<dailyDisperseData> preprocess(QVector<dailyDisperseData>);
    void saveData(QVector<VectorContent>,int);
    QMultiMap<double,dailyDisperseData> findPoints(QVector<dailyDisperseData>,double X,double Y);
    QVector<VectorContent> averageMonth(QVector<VectorContent>);
};

#endif // DATAPROCESS_H
