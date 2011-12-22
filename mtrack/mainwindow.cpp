#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTableView>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// SQL test code

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.1.99");
	db.setDatabaseName("epc");
	db.setUserName("kyle");
	db.setPassword("kobalt");
	db.open();

	/*QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery("SELECT id, first, last FROM patients;");
	model->setHeaderData(0, Qt::Horizontal, tr("id"));
	model->setHeaderData(1, Qt::Horizontal, tr("first"));
	model->setHeaderData(2, Qt::Horizontal, tr("last"));

	QTableView *view = new QTableView(ui->centralWidget);
	view->setModel(model);
	view->show();*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
