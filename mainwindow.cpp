#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myPicture = new ImageViewer();
    ui->gridLayout->addWidget(myPicture->scrollArea);

    timer = new QTimer();
    timer->setInterval(10);
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));
    timer->start();
    //привязка к клавишам
    connect(ui->exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action, SIGNAL(triggered()), myPicture, SLOT(open()));
    connect(ui->zoomIn, SIGNAL(clicked()), myPicture, SLOT(zoomIn()));
    connect(ui->zoomOut, SIGNAL(clicked()), myPicture, SLOT(zoomOut()));
    connect(ui->copy, SIGNAL(triggered()), myPicture, SLOT(copy()));
    //connect(ui->paste, SIGNAL(triggered()), myPicture, SLOT(paste()));
    connect(ui->turn, SIGNAL(clicked()), myPicture, SLOT(turn()));

}



MainWindow::~MainWindow()
{
    delete ui;
}
// ограничения zoomIn и zoomOut
void MainWindow::slotAlarmTimer()
{
    ui->zoomIn->setEnabled(myPicture->getvaluescallfactor() < 3.0);
    ui->zoomOut->setEnabled(myPicture->getvaluescallfactor() > 0.333);
}
