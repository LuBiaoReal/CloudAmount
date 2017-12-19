#include "dataprocess.h"
#include<QFile>
#include<QTextStream>
#include<QtAlgorithms>
#include"dailydispersedata.h"

DataProcess::DataProcess(QVector<QStringList> monthsFiles,
                         QString locatePath, QVector<VectorContent> baseCoord)
{
    this->monthsFiles = monthsFiles;
    this->locatePath = locatePath;
    this->baseCoord = baseCoord;
}
//对每个文件里的数据进行操作（快速排序、取出对应的纬度范围等）
void DataProcess::readFile(QString filePath)
{
    QVector<dailyDisperseData> _dailyData;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line=in.readLine();
        //解析每读出来一行的路径
        dailyDisperseData _data;
        QStringList lineDataList = line.split(",");
        QString y =lineDataList.at(0);
        _data.CoordY = y.toDouble();
        QString x = lineDataList.at(1);
        _data.CoordX = x.toDouble();
        QString c =lineDataList.at(2);
        _data.CloudAmount = c.toDouble()/100;
       // QVector<double,double,double> dailyData;
        _dailyData.append(_data);
    }
    file.close();
    qSort(_dailyData.begin(),_dailyData.end());


}
void DataProcess
