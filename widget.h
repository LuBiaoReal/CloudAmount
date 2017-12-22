#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include"vectorcontent.h"
#include"vectorcontentson.h"
/*class dailyData
{
public:
    double YCoord = 0;
    double XCoord = 0;
    double CloudMount = 0;
    bool operator <(const dailyData &m)const{
        return YCoord > m.YCoord;
    }
};
*/
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString targetPath;
    QString locatePath;
    QStringList readMonthsFiles();
    QStringList readDaysFiles(QString);
    void readFile(QString);
    void parserline(QString);
    QVector<VectorContent> standerCoord;
    QVector<int> test;

public slots:
    void slotTargetLocate_btn_licked();
    void slotSetLocate_btn_clicked();
    void slotRequiry_btn_clicked();
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
