#include <QApplication>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton *bouton = new QPushButton("Mon bouton entre en scène !");
    QGraphicsScene scene;
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    proxy->setWidget(bouton);
    scene.addItem(proxy);

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}
