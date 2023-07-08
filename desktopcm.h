#ifndef DESKTOPCM_H
#define DESKTOPCM_H

#include <QDialog>
#include <QStandardItem>
#include <QEvent>

#include "mainwindow.h"

namespace Ui {
class desktopcm;
}

class desktopcm : public QDialog
{
    Q_OBJECT

public:
    explicit desktopcm(MainWindow* mainWindow,QWidget *parent = nullptr);
    ~desktopcm();

private:
    Ui::desktopcm *ui;

    QStandardItemModel *model;

    void handleItemClick(const QModelIndex &index);

    void changeEvent(QEvent *event) override {
        if (event->type() == QEvent::WindowStateChange) {
            if (windowState() == Qt::WindowMinimized || !isVisible()) {
                // Close the window if it becomes minimized or hidden
                close();
            }
        }else if (event->type() == QEvent::FocusOut || !isActiveWindow()) {
            close();
        }
        QWidget::changeEvent(event);
    }

    MainWindow* mainWindow;
};

#endif // DESKTOPCM_H
