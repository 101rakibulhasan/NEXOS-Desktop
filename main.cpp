
#include "mainwindow.h"

#include <QApplication>

#include <QtWidgets>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QList<QScreen*> screens = QGuiApplication::screens();
    foreach (QScreen *screen, screens) {
        MainWindow *window = new MainWindow();
        window->setGeometry(screen->geometry());
        window->showMaximized();
        window->show();
    }

    //MainWindow w;

    //w.show();
    return a.exec();
}
