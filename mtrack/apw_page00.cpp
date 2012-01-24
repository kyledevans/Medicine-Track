/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "apw_page00.h"
#include "ui_apw_page00.h"

APW_Page00::APW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::APW_Page00)
{
    ui->setupUi(this);
}

APW_Page00::~APW_Page00()
{
    delete ui;
}

void APW_Page00::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;
}

/* SQL without C++:
SELECT *
FROM shipments
JOIN drugs ON drugs.id = shipments.drug_id
WHERE shipments.active = '1'
AND drugs.active = 1;
*/
void APW_Page00::initiateSearch()
{

}
