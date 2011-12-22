#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQueryModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // SQL test code
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.1.99");
    db.setDatabaseName("epc");
    db.setUserName("kyle");
    db.setPassword("kobalt");
    db.open();

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM patients");

    delete model;

    return a.exec();
}
