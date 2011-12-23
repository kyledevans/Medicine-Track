#include "patientsearch.h"
#include "ui_patientsearch.h"

PatientSearch::PatientSearch(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PatientSearch)
{
    ui->setupUi(this);

	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery("SELECT id, first, last FROM patients;");
	model->setHeaderData(0, Qt::Horizontal, tr("id"));
	model->setHeaderData(1, Qt::Horizontal, tr("first"));
	model->setHeaderData(2, Qt::Horizontal, tr("last"));

	ui->resultTable->setModel(model);
}

PatientSearch::~PatientSearch()
{
    delete ui;
}
