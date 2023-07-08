#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "mainwindow.h"


namespace Ui {
class settingswindow;
}

class settingswindow : public QDialog
{
    Q_OBJECT

public:
    explicit settingswindow(MainWindow* mainWindow, QWidget *parent = nullptr);
    ~settingswindow();

private slots:


    void on_cancel_clicked();

    void on_apply_clicked();

    void on_browse_clicked();

    void on_filePath_textChanged(const QString &arg1);

private:
    Ui::settingswindow *ui;

    MainWindow* mainWindow;

    QString newType= "image";
    QString newFile;

};

#endif // SETTINGSWINDOW_H
