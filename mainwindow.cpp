#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //chart = new Chart(ui->centralwidget);
    //chart->resize(ui->centralwidget->size());
}

MainWindow::~MainWindow()
{
    delete ui;


}

