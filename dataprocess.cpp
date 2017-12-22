#include "dataprocess.h"
#include<QFile>
#include<QTextStream>
#include<QtAlgorithms>
#include"dailydispersedata.h"
#include"vectorcontent.h"
#include<QMultiMap>
//调用process接口就可以进行整个的操作了
DataProcess::DataProcess(QVector<QStringList> monthsFiles,
                         QString locatePath, QVector<VectorContent> baseCoord)
{
    this->monthsFiles = monthsFiles;
    this->locatePath = locatePath;
    this->baseCoord = baseCoord;
}
//对每个文件里的数据进行操作（快速排序、取出对应的纬度范围等）
QVector<dailyDisperseData> DataProcess::readFile(QString filePath)
{
    QVector<dailyDisperseData> _dailyData;
    //QVector<dailyData> preprocessData;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return _dailyData;
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
        _dailyData.append(_data);
    }
    file.close();
    qSort(_dailyData.begin(),_dailyData.end());
   // oneMonthData.append(preprocessData);*/
    return _dailyData;
}
/*
 *  QVector<QStringList> monthsFiles;//放置所读出来一年文件的路径
    QString locatePath;//放置处理后文件的路径
    QVector<VectorContent> baseCoord;//标准坐标的容器
*/
//处理文件进程
void DataProcess::process()
{

    //读每个月的文件夹的文件路径表
    for(int i=0;i<monthsFiles.length();i++)
    {
        QStringList monthFiles=monthsFiles.at(i);
        //读取每个月文件路径表中的数据文件路径
        QVector<VectorContent> monthCloud = baseCoord;
        foreach (QString str, monthFiles)
        {
            //想要实现的效果是：读文件、预处理文件、插值
            QVector<dailyDisperseData> dailyVector = readFile(str);
            QVector<dailyDisperseData> rangeData = preprocess(dailyVector);
            insertAmount(rangeData,monthCloud);
            //insertAmount(dailyVector,&monthCloud);
        }
        //对每个月的云量求一个总和，个数，平均
        QVector<VectorContent> finishedData = averageMonth(monthCloud);
        //保存文件
        saveData(finishedData,i+1);
    }
}
//对每个月的云量求一个平均
QVector<VectorContent> DataProcess::averageMonth(QVector<VectorContent> niceData)
{
    QVector<VectorContent> finishedData = baseCoord;
    for(int i = 0;i < niceData.length();i++)
    {
        for(int j = 0;j < niceData.at(i).son.length();j++)
        {

            double sumC = 0;
            int countC = niceData.at(i).son.at(j).CloudAount.length();
            for(int k = 0;k < countC;k++)
            {
                sumC += niceData.at(i).son.at(j).CloudAount.at(k);
            }
            double averageC = 0;
            if(countC == 0);
            else
            {
                averageC = sumC/countC;
            }
            finishedData[i].son[j].CloudAount.append(sumC);
            finishedData[i].son[j].CloudAount.append(countC);
            finishedData[i].son[j].CloudAount.append(averageC);
        }
    }
    return finishedData;
}
//预处理
QVector<dailyDisperseData> DataProcess::preprocess(QVector<dailyDisperseData> fileData)
{
    double maxStanderY = baseCoord.at(0).CoordY + 1;
    double minStanderY = baseCoord.at(baseCoord.size()-1).CoordY - 1;
    QVector<dailyDisperseData> rangeData;
    for(int i = 0;i < fileData.length();i++)
    {
        if(fileData.at(i).CoordY < maxStanderY && fileData.at(i).CoordY > minStanderY)
        {
            rangeData.append(fileData.at(i));
        }
    }
    return rangeData;
}
//完成一个文件的插值
void DataProcess::insertAmount(QVector<dailyDisperseData> data,QVector<VectorContent> &monthCloud)
{
    //第一步：取出来标准带点的经纬度正负0.01范围内的所有的离散点
    double maxY = data.at(0).CoordX;
    double minY = data.at(data.size() - 1).CoordY;
    for(int i = 0;i < monthCloud.length();i++)
    {
        double standarY = monthCloud.at(i).CoordY;
        QVector<VectorContentSon> standerYC = monthCloud.at(i).son;
        if(standarY <= maxY && standarY >= minY)
        {
            //如果标准纬度在目标文件范围内，就开始遍历标准经度
            for(int l = 0; l < standerYC.length();l++)
            {
                QVector<dailyDisperseData> dotArea;
                double standerX = standerYC.at(l).CoordX;
                for(int j=0;j<data.length();j++)
                {
                    //将范围内的点放入重新放到一个容器中

                    if((data.at(j).CoordY <  standarY + 0.1)
                            && (data.at(j).CoordY > standarY - 0.1)
                            && (data.at(j).CoordX > standerX - 0.1)
                            && (data.at(j).CoordX < standerX + 0.1))
                    {
                        dailyDisperseData tmp;
                        tmp = data.at(j);
                        dotArea.append(tmp);
                    }
                 }
                //第二步：找到四个最近的点（单独封装一个函数）
                QMultiMap<double,dailyDisperseData> lenMap=findPoints(dotArea,standerX,standarY);
                QVector<double> len;
                QVector<dailyDisperseData> PVector;
                int i=0;
                for(QMultiMap<double,dailyDisperseData>::iterator it = lenMap.begin();
                        (it != lenMap.end())&& (it != lenMap.begin()+3);it++)
                {
                    len.append(it.key());
                    PVector.append(it.value());
                    i++;
                }
                //第三步：进行插值
                if(i == 4)
                {
                    //距离标准点的长度
                    double l1 = len.at(0);
                    double l2 = len.at(1);
                    double l3 = len.at(2);
                    double l4 = len.at(3);
                    double s = l1 + l2 + l3 + l4;
                    //四个点对应的云量
                    double c1 = PVector.at(0).CloudAmount;
                    double c2 = PVector.at(1).CloudAmount;
                    double c3 = PVector.at(2).CloudAmount;
                    double c4 = PVector.at(3).CloudAmount;
                    if(l1 == 0)//若是直接在标准点上面
                    {
                        monthCloud[i].son[l].CloudAount.append(c1);
                    }
                    else
                    {
                        //对四个点进行插值
                        double c = (l4/s)*c1 + (l3/s)*c2 + (l2/s)*c3 +(l1/s)*c4;
                        monthCloud[i].son[l].CloudAount.append(c);
                    }
                }
            }
        }
    }


}
//找四个最近的点
QMultiMap<double,dailyDisperseData> DataProcess::findPoints(QVector<dailyDisperseData> inArea,double X,double Y)
{
    QMultiMap<double,dailyDisperseData> tmp;
    double standarY = X;
    double standarX = Y;
    //不够四个就直接返回
    if(inArea.length() < 4)
    {
        return tmp;
    }
    else
    {
        for(int i = 0;i < inArea.length();i++)
        {
            double Ax = inArea.at(i).CoordX;
            double Ay = inArea.at(i).CoordY;
            double l = sqrt((Ax - standarX)*(Ax - standarX)+(Ay - standarY)*(Ay - standarY));
            tmp.insert(l,inArea.at(i));
        }
    }
    return tmp;
}
//快速排序算法
/*
void quickSort(int s[], int l, int r)
{
   if (l< r)
   {
       int i = l, j = r, x = s[l];
       while (i < j)
       {
           while(i < j && s[j]>= x) // 从右向左找第一个小于x的数
               j--;
           if(i < j)
               s[i++] = s[j];
           while(i < j && s[i]< x) // 从左向右找第一个大于等于x的数
               i++;
           if(i < j)
               s[j--] = s[i];
       }
       s[i] = x;
       quickSort(s, l, i - 1); // 递归调用
       quickSort(s, i + 1, r);
   }
}  */
//存储处理完的文件
void DataProcess::saveData(QVector<VectorContent> text,int i)
{
    QString path = locatePath + QString::number(i) + ".csv";
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream in(&file);
    in <<"XCoord,YCoord,,CloudAmount"<<endl;
    for(int i = 0;i < text.length();i++)
    {
        for(int j = 0;j < text.at(i).son.length();j++)
        {
            in << text.at(i).son.at(j).CoordX << ","
               << text.at(i).CoordY <<",";
            for(int k = 0;k < text.at(i).son.at(j).CloudAount.length();k++)
            {
                in << text.at(i).son.at(j).CloudAount.at(k)<<",";
            }
            in << endl;
        }
    }
    file.close();
}

