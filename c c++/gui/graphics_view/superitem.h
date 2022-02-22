#ifndef SUPERITEM_H
#define SUPERITEM_H
#include<QtWidgets>

class SuperItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    SuperItem(QGraphicsItem *parent = nullptr);
    ~SuperItem();
protected:
    QRectF boundingRect() const override;
    void keyPressEvent(QKeyEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPointF lastPoint;
};
#endif // SUPERITEM_H
