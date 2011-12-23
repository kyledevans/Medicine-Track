#include <QString>
#include <QtDebug>

#include "patientsearch.h"
#include "ui_patientsearch.h"

PatientSearch::PatientSearch(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PatientSearch)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
}

PatientSearch::~PatientSearch()
{
    delete ui;
}

void PatientSearch::initiateSearch()
{
	QString query;
	// If the text fields are empty, don't do anything.
	if (		(ui->firstNameField->text() != QString(""))
			||	(ui->lastNameField->text() != QString(""))
			||	(ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31"))) {
		QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);

		query = QString("SELECT id, last, first, dob FROM patients WHERE first LIKE '%")
				+= ui->firstNameField->text()
				+= QString("%' AND last LIKE '%")
				+= ui->lastNameField->text()
				+= QString("%'");
		if (ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31")) {
			query += QString(" AND dob = '")
				+= ui->dobField->date().toString("yyyy-MM-dd")
				+= QString("';");
		} else {
			query += QString(";");
		}
		qDebug() << query;

		model->setQuery(query);
		model->setHeaderData(0, Qt::Horizontal, tr("ID"));
		model->setHeaderData(1, Qt::Horizontal, tr("Last name"));
		model->setHeaderData(2, Qt::Horizontal, tr("First name"));
		model->setHeaderData(3, Qt::Horizontal, tr("D.O.B."));

		ui->resultTable->setModel(model);
	}
}

void PatientSearch::resetPressed()
{
	ui->dobField->setDate(QDate(1970, 1, 31));	// Default date is 1/31/1970
}
