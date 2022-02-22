#include "superitem.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene* scene = new QGraphicsScene(0, 0, 500, 500);
    scene->setBackgroundBrush(Qt::black);
    QGraphicsView view(scene);
    SuperItem* super = new SuperItem;
    scene->addItem(super);
    super->setFocus();
//    view.setWindowTitle(QApplication::translate("simpleanchorlayout", "Mirana Waris"));
    view.setWindowTitle( "Mirana Waris");
    view.setWindowIcon(QIcon("gstar.png"));
    view.show();
    return a.exec();
}
