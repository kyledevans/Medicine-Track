/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alterprescriptionwizard.h"
#include "ui_alterprescriptionwizard.h"

AlterPrescriptionWizard::AlterPrescriptionWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterPrescriptionWizard)
{
    ui->setupUi(this);
}

/* Scratch SQL:
SELECT *
FROM shipments
JOIN drugs ON drugs.id = shipments.drug_id
WHERE shipments.active = '1'
AND drugs.active = 1;

*/
AlterPrescriptionWizard::~AlterPrescriptionWizard()
{
    delete ui;
}
