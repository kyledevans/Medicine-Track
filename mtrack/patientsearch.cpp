#include "patientsearch.h"
#include "ui_patientsearch.h"

PatientSearch::PatientSearch(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PatientSearch)
{
    ui->setupUi(this);
}

PatientSearch::~PatientSearch()
{
    delete ui;
}
