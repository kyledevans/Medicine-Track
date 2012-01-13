/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>

#include "medicationrecord.h"

#include "globals.h"
#include "alertinterface.h"

MedicationRecord::MedicationRecord():
	id(SQL::Undefined_ID),
	instruction_id(SQL::Undefined_ID),
	form(FORM::Undefined),
	active(false),
	exists(false)
{
}

/*
SELECT name, generic, manufacturer, ndc, form, strength, amount, active, instructions
FROM drugs
WHERE id = 'SOME_VAR';
*/
bool MedicationRecord::retrieve(int newId)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT name, generic, manufacturer, ndc, form, strength, amount, active, instructions FROM drugs WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	name = model->record(0).value(0).toString();
	generic = model->record(0).value(1).toString();
	manufacturer = model->record(0).value(2).toString();
	ndc = model->record(0).value(3).toString();
	form = model->record(0).value(4).toString();
	strength = model->record(0).value(5).toString();
	amount = model->record(0).value(6).toString();
	active = model->record(0).value(7).toBool();
	instructions = model->record(0).value(8).toString();
	exists = true;

	delete model;
	return true;
}
