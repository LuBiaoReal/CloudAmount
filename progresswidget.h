#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>

namespace Ui {
class progressWidget;
}

class progressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit progressWidget(QWidget *parent = 0);
    ~progressWidget();
    void wholeProgress();
    void monthProgress();
    void pathDetail();

private:
    Ui::progressWidget *ui;
};

#endif // PROGRESSWIDGET_H
