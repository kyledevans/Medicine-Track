#include "medicationsframe.h"
#include "ui_medicationsframe.h"

MedicationsFrame::MedicationsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MedicationsFrame)
{
    ui->setupUi(this);
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}
