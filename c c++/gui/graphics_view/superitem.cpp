#include "superitem.h"

#define SIZE 100

SuperItem::SuperItem(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
//    setFlags(QGraphicsItem::ItemIsFocusable  | QGraphicsItem::ItemIsSelectable);
}

QRectF SuperItem::boundingRect() const
{
    return QRectF(10, 10, SIZE, SIZE);
}
void SuperItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::yellow);
    QGraphicsView *view = scene()->views().first();
    qDebug() << "View width:" << view->width() << "height:" << view->height() << "x:" << this->x() << "y:" << this->y();
    painter->drawRoundedRect(0, 0, SIZE, SIZE, 50, 50);
//    painter->setBrush(Qt::red);
//    painter->drawRoundedRect(SIZE/2, SIZE/2, SIZE/2, SIZE/2, 0, 0);
}
void SuperItem::keyPressEvent(QKeyEvent *event){

    QGraphicsView *view = scene()->views().first();

    switch(event->key()){
    case Qt::Key_Right:{
        if (this->x()  + 2 * SIZE < view->width())
            moveBy(SIZE, 0);
        else
            setPos(0, this->y());
        break;
    }
    case Qt::Key_Left:{
        if (this->x() - SIZE >= 0)
            moveBy(-SIZE, 0);
        else
            setPos(view->width() - SIZE, this->y());
        break;
    }
    case Qt::Key_Up:{
        if(this->y() - SIZE >= 0)
            moveBy(0, -SIZE);
        else
            setPos(this->x(), view->height() - SIZE);
        break;
    }
    case Qt::Key_Down:{
        if(this->y() + 2 * SIZE < view->height())
            moveBy(0, SIZE);
        else
            setPos(this->x(), 0);
        break;
    }

    }
    update();
}


SuperItem::~SuperItem()
{
}
