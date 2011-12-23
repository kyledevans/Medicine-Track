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

void PrescriptionsFrame::resetPressed()
{
	ui->filledField->setDate(QDate(1970, 1, 31));	// Default date is 1/31/1970
	ui->dobField->setDate(QDate(1970, 1, 31));
}
