#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    /*
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    QPixmap image = QPixmap("/Users/air/Documents/Computer Graphics/CGCourseWork/doge.jpeg");
    image = image.scaledToHeight(image.height() * 0.5f);
    image = image.scaledToWidth(image.width() * 0.5f);

    QGraphicsPixmapItem item(image);
    scene.addItem(&item);

    view.show();
    */

    Widget w;
    w.show();

    return a.exec();
}
