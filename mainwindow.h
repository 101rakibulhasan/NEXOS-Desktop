
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QString settingPath = "";
    void setVariables();
    void openChooseWallpaper();
    void setDefaultSettings();

    QString wallpaperMode = "image";
    QString imageDefPath = ":/wallpapers/cyclical_1080p.png";
    QString animDefPath = "box.html";
    QString videoDefPath = "crystals.mp4";

    ~MainWindow();

public slots:
    void updateUI();

private:
    Ui::MainWindow *ui;

    QString imagePath = "";
    QString animPath = "";
    QString videoPath = "";

    QString appPath = "";


    void ReadSettings();
    void RunWallpaper();
    void runwallpaperasimage();
    void runwallpaperasvideo();
    void runwallpaperasanimation();
    void openContextMenu();
    void mousePressEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);


    void cancelDownloadRequest(QWebEngineDownloadRequest* request);



};
#endif // MAINWINDOW_H
