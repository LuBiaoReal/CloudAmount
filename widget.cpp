#include "widget.h"
#include "ui_widget.h"
#include<QPushButton>
#include<QFile>
#include<QFileDialog>
#include<QFileInfo>
#include<QDir>
#include<QVector>
#include<QtAlgorithms>
#include<QTextStream>
//#include"dailyData.h"
#include"dataprocess.h"
#include<QtAlgorithms>


#include<QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->requiry_btn,SIGNAL(clicked(bool)),this,SLOT(slotRequiry_btn_clicked()));
    connect(ui->setLocate_btn,SIGNAL(clicked(bool)),this,SLOT(slotSetLocate_btn_clicked()));
    connect(ui->targetLocate_btn,SIGNAL(clicked()),this,SLOT(slotTargetLocate_btn_licked()));
    targetPath="";
    locatePath="";

    //读标准的点
    QFile baseCoord(":/new/prefix1/baseCoord.csv");
    if(!baseCoord.open(QIODevice::ReadOnly))
    {
    qDebug()<<"Can't open basic file..";
    return;
    }
    QTextStream stream(&baseCoord);
    QVector<QString> baseVector;
    while(!baseCoord.atEnd())
    {
        QString str = stream.readLine();
        baseVector.append(str);
    }
    baseCoord.close();

    //对整个坐标进行分类
    for(int i = 1;i < baseVector.length();i++)
    {
        QStringList baselist= baseVector.at(i).split(",");
        VectorContent content;
        if(i == 1)
        {
            content.CoordY =baselist.at(2).toDouble();
            VectorContentSon Son;
            Son.CoordX = baselist.at(1).toDouble();
            content.son.append(Son);
            standerCoord.append(content);
        }
        else
        {
            int length1 = standerCoord.size()-1;
           // int length2 = standerCoord.at(standerCoord.size()-1).son-1;
            if(standerCoord.at(length1).CoordY == baselist.at(2).toDouble())
            {
                VectorContentSon S;
                S.CoordX = baselist.at(1).toDouble();
                VectorContent &d = standerCoord[length1];//使用一个引用以便于修改
                //VectorContent &d = standerCoord.at(length1);//这种写法就是错误的；因为at返回的是一个const的值，无法修改
                d.son.append(S);
            }
            else
            {
                content.CoordY =baselist.at(2).toDouble();
                VectorContentSon Son;
                Son.CoordX = baselist.at(1).toDouble();
                content.son.append(Son);
                standerCoord.append(content);
            }
        }
    }
}

Widget::~Widget()
{
    delete ui;
}
void Widget::slotSetLocate_btn_clicked()
{
    QString str;
    locatePath=QFileDialog::getExistingDirectory(NULL,"选择解析文件路径",str);
    QFileInfo info(locatePath);
    str=info.absolutePath();
    qDebug()<<"locate:"<<locatePath;
}
void Widget::slotTargetLocate_btn_licked()
{
    //打开目标文件夹
    QString str;
    targetPath=QFileDialog::getExistingDirectory(NULL,"选择目标文件路径",str);
    QFileInfo info(targetPath);
    str=info.absolutePath();
    qDebug()<<"target:"<<targetPath;
}
void Widget::slotRequiry_btn_clicked()
{
    //遍历文件夹下的记录的多个月的文件夹目录的路径
    QStringList monthsFilesList=readMonthsFiles();

    int fileNumber=monthsFilesList.length();
    qDebug()<<"the number of files is:"<<fileNumber;
    qDebug()<<monthsFilesList;
    //遍历每一天的路径
    QVector<QStringList> monthsFilesVector;
    for(int i=0;i < monthsFilesList.length();i++)
    {
        QStringList daysFilesList;
        daysFilesList=readDaysFiles(monthsFilesList.at(i));
        monthsFilesVector.append(daysFilesList);
    }
   /*qSort(test.begin(),test.end(),qGreater<double>());
    for(int i=0;i<test.length();i++)
    {
        qDebug()<<"after"<<test.at(i)<<endl;
    }*/
    DataProcess process(monthsFilesVector,locatePath,standerCoord);
    process.process();
}
//读出有多少个月文件(只是读有多少个文件夹，而非读文件夹的内容)
QStringList Widget::readMonthsFiles()
{
    QDir dir(targetPath);
    QStringList filesPathList;
    QFileInfoList list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
   // dir.setFilter(QDir::Dirs | QDir::Files);
    //dir.setSorting(QDir::DirsFirst);
    foreach (QFileInfo filePath, list) {
//        if(filePath.fileName() == "." || filePath.fileName() =="..")//去除后退的两个文件夹的名字
//        ;
//        else{
        QString tmp=filePath.absoluteFilePath();
        filesPathList.append(tmp);
//        }
    }
    return filesPathList;
}
//读出每个月中的每个文件的路径
QStringList Widget::readDaysFiles(QString monthFile)
{
    QDir monthDir(monthFile);
    QStringList daliyDataList;
    QFileInfoList list = monthDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    qDebug()<<list.count();
    test.append(list.count());
    for(int i=0;i<list.length();i++)
    {
        QFileInfo info=list.at(i);
        QString suffix=info.suffix();
        if(QString::compare(suffix,QString("txt"),Qt::CaseInsensitive)==0)
        {
            QString data=info.absoluteFilePath();
            daliyDataList.append(data);
        }
    }
    return daliyDataList;
}
