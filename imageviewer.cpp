#include "imageviewer.h"
 
ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView(parent)
    , scrollArea(new QScrollArea)
    , imageLabel(new QLabel)
{


    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);


 
    /* Немного поднастроим отображение виджета и его содержимого */
    //this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn); // Отключим скроллбар по горизонтали
    //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);   // Отключим скроллбар по вертикали
    this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);    // Растягиваем содержимое по виджету

    scene = new QGraphicsScene(this);
    this->setScene(scene);          // Устанавливаем сцену в виджет

    group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов
    group_2 = new QGraphicsItemGroup(); // Инициализируем вторую группу элементов

    timer = new QTimer();               // Инициализируем Таймер
    timer->setSingleShot(true);
    // Подключаем СЛОТ для отрисовки к таймеру
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));

    timer->start(50);                   // Стартуем таймер на 50 миллисекунд

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(this);
    //resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

}
 
ImageViewer::~ImageViewer()//деструктор
{

}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());
    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
    //qgraphicsscene перерисовка изменения сцены
    this->item->update();
    this->scene->update();
    this->update();
    //ограничения
//    mainwindow->zIn->setEnabled(scaleFactor < 3.0);
//    mainwindow->zOut->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}


void ImageViewer::open()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.png");
    newImage = new QImage;
    newImage->load(str);
    item = new QGraphicsPixmapItem;
    scene->addItem(item);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);

    pMyProxy = new QGraphicsProxyWidget(item);
    imageLabel->setPixmap(QPixmap::fromImage(*newImage));
    imageLabel->adjustSize();
    pMyProxy->setWidget(imageLabel);
    pMyProxy->setPos(item->boundingRect().center()-imageLabel->rect().center());

    scrollArea->setWidgetResizable(true);

    timer->start(50);



//    QString imagePath = QFileDialog::getOpenFileName(
//                    this,
//                    tr("Open File"),
//                    DIR,
//                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
//                    );
//      QImage * imageObject = new QImage();
//        imageObject->load(imagePath);
//      QPixmap  image = QPixmap::fromImage(*imageObject);
//      QGraphicsScene *  nscene = new QGraphicsScene(this);
//        nscene->addPixmap(image);
//        nscene->setSceneRect(image.rect());
//        this->setScene(nscene);

}

//void ImageViewer::setImage(const QImage &newImage)
//{

//    if (newImage.colorSpace().isValid())
//        newImage.convertToColorSpace(QColorSpace::SRgb);
//    imageLabel->setPixmap(QPixmap::fromImage(newImage));
////! [4]
//    scaleFactor = 1.0;

//    scrollArea->setVisible(true);
//    printAct->setEnabled(true);
//    fitToWindowAct->setEnabled(true);
//    updateActions();

//    if (!fitToWindowAct->isChecked())
//        imageLabel->adjustSize();
//}


void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(*newImage);
#endif // !QT_NO_CLIPBOARD
}

//static QImage clipboardImage()
//#ifndef QT_NO_CLIPBOARD
//{
//    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
//        if (mimeData->hasImage()) {
//            const QImage newImage = qvariant_cast<QImage>(mimeData->imageData());
//            if (!newImage.isNull())
//                return newImage;
//        }
//    }
//    return QImage();
//}
//#endif // !QT_NO_CLIPBOARD

//void ImageViewer::paste()
//{
//#ifndef QT_NO_CLIPBOARD
//    const QImage newImage = clipboardImage();
//    if (newImage.isNull()) {
//        statusBar()->showMessage(tr("No image in clipboard"));
//    } else {
//        setImage(newImage);
//        setWindowFilePath(QString());
//        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
//            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
//        statusBar()->showMessage(message);
//    }
//#endif // !QT_NO_CLIPBOARD
//}

void ImageViewer::turn()
{

    QPixmap rotatePixmap(newImage->size());//делаем размер
    rotatePixmap.fill(Qt::transparent);

    QPainter p(&rotatePixmap);
    p.setRenderHint(QPainter::Antialiasing); // сглаживание
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(newImage->size().width() / 2, newImage->size().height() / 2);
    p.rotate(90); // градус
    p.translate(-newImage->size().width() / 2, -newImage->size().height() / 2);
    p.drawPixmap(0, 0, (QPixmap::fromImage(*newImage)));
    p.end();
    *newImage = rotatePixmap.toImage();
    imageLabel->setPixmap(QPixmap::fromImage(*newImage));

    //imageLabel->adjustSize();

    //this->scene->setSceneRect(0,0, imageLabel->width(), imageLabel->height());;
    item->boundingRect().setWidth(newImage->height());
    item->boundingRect().setHeight(newImage->width());
    scene->setSceneRect(item->boundingRect());
    this->setSceneRect(scene->itemsBoundingRect());
    scrollArea->setWidgetResizable(true);

    timer->start(50);


//    this->item->update();
//    this->scene->update();
//    this->update();
}

void ImageViewer::slotAlarmTimer()
{
    //this->scene->setSceneRect(0,0, this->width(), this->height());
}


///* Этим методом перехватываем событие изменения размера виджет
void ImageViewer::resizeEvent(QResizeEvent *event)
{
    timer->start(50);   // Как только событие произошло стартуем таймер для отрисовки
    QGraphicsView::resizeEvent(event);  // Запускаем событие родителького класса
}

double ImageViewer::getvaluescallfactor()
{
    return scaleFactor;
}
 
/* Метод для удаления всех элементов из группы
 * */
void ImageViewer::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    /* Перебираем все элементы сцены, и если они принадлежат группе,
     * переданной в метод, то удаляем их
     * */
    foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
    }
}

void ImageViewer::mousePressEvent(QMouseEvent  *event)
{
   // Сохраняем координаты точки нажатия
   firstPoint = event->pos();
   if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRectF(firstPoint, QSize()).toRect());
    rubberBand->show();
}

void ImageViewer::mouseMoveEvent(QMouseEvent  *event)
{
    rubberBand->setGeometry(QRectF(firstPoint, event->pos()).toRect().normalized());
}

void ImageViewer::mouseReleaseEvent(QMouseEvent  *event)
{
    rubberBand->hide();
}
