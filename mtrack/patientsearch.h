#ifndef PATIENTSEARCH_H
#define PATIENTSEARCH_H

#include <QFrame>

namespace Ui {
    class PatientSearch;
}

class PatientSearch : public QFrame
{
    Q_OBJECT

public:
    explicit PatientSearch(QWidget *parent = 0);
    ~PatientSearch();

public slots:
	void initiateSearch();
	void resetPressed();

private:
    Ui::PatientSearch *ui;
};

#endif // PATIENTSEARCH_H
