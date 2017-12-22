#include "progresswidget.h"
#include "ui_progresswidget.h"

progressWidget::progressWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::progressWidget)
{
    ui->setupUi(this);
}

progressWidget::~progressWidget()
{
    delete ui;
}
