#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QLabel>
#include <QImage>
#include <QPainter>
 
QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

// Расширяем класс QGraphicsView
class ImageViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    //вернуть обратно в приват 36 строка!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    double getvaluescallfactor();


    QScrollArea *scrollArea;
 
signals:

private slots:
    void slotAlarmTimer();  /* слот для обработчика переполнения таймера
                             * в нём будет производиться перерисовка
                             * виджета
                             * */
    void open();
    void zoomIn();
    void zoomOut();
    void copy();
    void turn();
    //void paste();
    //void normalSize();
    //void fitToWindow();

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    //void setImage(const QImage &newImage);


    QGraphicsItemGroup  *group_1;   // Объявляем первую группу элементов
    QGraphicsItemGroup  *group_2;   // Объявляем вторую группу элементов

    QGraphicsScene      *scene;     // Объявляем сцену для отрисовки
    QLabel              *imageLabel;
    QGraphicsPixmapItem *item;

 
    /* Таймер для задержки отрисовки.
     * Дело в том, что aпри создании окна и виджета
     * необходимо  время, чтобы родительский слой
     * развернулся, чтобы принимать от него адекватные параметры
     * ширины и высоты
     * */
    QTimer              *timer;
    double              scaleFactor = 1;
    QImage              *newImage;

private:
    /* Перегружаем событие изменения размера окна,
     * чтобы перехватывать его
     * */
    void resizeEvent(QResizeEvent *event);
    /* Метод для удаления всех элементов
     * из группы элементов
     * */
    void deleteItemsFromGroup(QGraphicsItemGroup *group_1);
};
 
#endif // IMAGEVIEWER_H
