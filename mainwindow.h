#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imageviewer.h>
#include <QImage>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;


private slots:

    void slotAlarmTimer();
    //void about();

private:    


    ImageViewer    *myPicture;
    QTimer              *timer;
};
#endif // MAINWINDOW_H
