#include "prescriptionsframe.h"
#include "ui_prescriptionsframe.h"

PrescriptionsFrame::PrescriptionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PrescriptionsFrame)
{
    ui->setupUi(this);
}

PrescriptionsFrame::~PrescriptionsFrame()
{
    delete ui;
}
