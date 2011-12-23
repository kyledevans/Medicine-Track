#include <QString>

#include "patientsearch.h"
#include "ui_patientsearch.h"

PatientSearch::PatientSearch(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PatientSearch)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
}

PatientSearch::~PatientSearch()
{
    delete ui;
}

void PatientSearch::initiateSearch()
{
	// If the text fields are empty, don't do anything.
	if (		(ui->firstNameField->text() != QString(""))
			||	(ui->lastNameField->text() != QString(""))) {
		QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);

		model->setQuery(QString("SELECT id, first, last FROM patients WHERE first LIKE '%")
				   += ui->firstNameField->text()
				   += QString("%' AND last LIKE '%")
				   += ui->lastNameField->text()
				   += QString("%';"));
		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("First name"));
		model->setHeaderData(2, Qt::Horizontal, tr("Last name"));

		ui->resultTable->setModel(model);
	}
}
