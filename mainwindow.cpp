#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "desktopcm.h"
#include "settingswindow.h"

#include <QPixmap>
#include <QScreen>
#include <QPainter>
#include <QPainterPath>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPoint>
#include <QUrl>
#include <QProcess>

#include <QDir>
#include <QTextStream>

#include <QMediaPlayer>
#include <QVideoWidget>


//#include <QWebEnginePage>
//#include <QWebEngineProfile>
//#include <QtWebEngineWidgets/QtWebEngineWidgets>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    //setWindowFlags(Qt::Window | Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //setAttribute(Qt::WA_ShowWithoutActivating);

    ui->setupUi(this);

    MainWindow::ReadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReadSettings()
{
    QProcess getuser ;
    getuser.start("whoami");
    getuser.waitForFinished();
    QString user = getuser.readAllStandardOutput().trimmed();
    appPath = "/home/"+ user + "/.nexina/Desktop/";
    settingPath = appPath + "setting.conf";

    QDir dir = QFileInfo(settingPath).dir();
    if (!dir.exists()) {
        if (!dir.mkpath(dir.absolutePath())) {
            //qDebug() << "Failed to create directories.";
        }
        //qDebug() << "Directories created.";
    }

    MainWindow::setDefaultSettings();

    MainWindow::setVariables();

}

void MainWindow::setDefaultSettings()
{
    // copy video to app directory
    videoDefPath = appPath + videoDefPath;
    QFile copyVideo(":/wallpapers/crystals.mp4");
    copyVideo.copy(videoDefPath);

    animDefPath = appPath + animDefPath;
    QFile copyAnim(":/wallpapers/box.html");
    copyAnim.copy(animDefPath);

    //set changing variable to default variable
    imagePath = imageDefPath;
    videoPath = videoDefPath;
    animPath = animDefPath;

    QFile settings(settingPath);
    if(!settings.exists())
    {
        if(settings.open(QIODevice::WriteOnly)) {
            QTextStream out(&settings);
            QString text = "type: \""+ wallpaperMode +"\"\nimagePath: \"" + imagePath + "\"\nvideoPath: \""+ videoPath +"\"\nanimPath: \"" + animPath + "\"";
            out << text;

            settings.close();
            MainWindow::setVariables();
        } else {
            MainWindow::RunWallpaper();
        }
    }
}

void MainWindow::setVariables()
{
    QFile file(settingPath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains("type: ")) {
                int firstIndex = line.indexOf("\"") + 1;
                int lastIndex = line.lastIndexOf("\"");
                wallpaperMode = line.mid(firstIndex, lastIndex-firstIndex);

            }
        }

        in.seek(0);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains("imagePath: ")) {
                int firstIndex = line.indexOf("\"") + 1;
                int lastIndex = line.lastIndexOf("\"");
                imagePath = line.mid(firstIndex, lastIndex-firstIndex);
            }
        }

        in.seek(0);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains("videoPath: ")) {
                int firstIndex = line.indexOf("\"") + 1;
                int lastIndex = line.lastIndexOf("\"");
                videoPath = line.mid(firstIndex, lastIndex-firstIndex);
            }
        }

        in.seek(0);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.contains("animPath: ")) {
                int firstIndex = line.indexOf("\"") + 1;
                int lastIndex = line.lastIndexOf("\"");
                animPath = line.mid(firstIndex, lastIndex-firstIndex);
            }
        }

        file.close();
    } else {
        qDebug() << "Failed to open file for reading.";
    }
    MainWindow::RunWallpaper();
}

void MainWindow::RunWallpaper()
{
    if(wallpaperMode == "image"){
        MainWindow::runwallpaperasimage();
    }else if(wallpaperMode == "animation")
    {
        MainWindow::runwallpaperasanimation();
    }else if(wallpaperMode == "video"){
        MainWindow::runwallpaperasvideo();
    }
}

void MainWindow::runwallpaperasimage()
{
    QGraphicsView *graphicsView = new QGraphicsView();
    setCentralWidget(graphicsView);


    QScreen* screen = QGuiApplication::primaryScreen();
    int screenwidth = screen->geometry().width();
    int screenheight = screen->geometry().height();
    this->setGeometry(0,0,screenwidth,screenheight);
    graphicsView->setGeometry(0,0,screenwidth,screenheight);


    QImage image;
    QString file = imagePath;
    image.load(file);
    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsPixmapItem * pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(pixmapItem);
    scene->setSceneRect(0,0,screenwidth,screenheight);
    graphicsView->setScene(scene);
    QSize viewSize = graphicsView->size();
    pixmapItem->setPixmap(QPixmap::fromImage(image).scaled(viewSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::runwallpaperasvideo()
{
    //video_player
    QVideoWidget *videoWidget = new QVideoWidget(this);
    QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setVideoOutput(videoWidget);
    this->setCentralWidget(videoWidget);
    mediaPlayer->setSource(QUrl::fromLocalFile(videoPath));
    mediaPlayer->setLoops(-1);
    mediaPlayer->play();
}

void MainWindow::runwallpaperasanimation()
{
    QWebEngineView *webView = new QWebEngineView(this);
    setCentralWidget(webView);
    QWebEngineProfile* profile = webView->page()->profile();
    profile->setHttpCacheType(QWebEngineProfile::NoCache);

    profile->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, false);
    profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false);
    profile->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    profile->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, false);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
    profile->settings()->setAttribute(QWebEngineSettings::NavigateOnDropEnabled, false);
    profile->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    profile->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, false);
    profile->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    profile->settings()->setAttribute(QWebEngineSettings::WebGLEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::TouchIconsEnabled, false);
    profile->settings()->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, false);
    profile->settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, false);
    profile->settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, false);
    profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);

    connect(profile, &QWebEngineProfile::downloadRequested,
            this, &MainWindow::cancelDownloadRequest);

    QWebEnginePage *webpage = new QWebEnginePage(profile, webView);
    webView->setPage(webpage);
    webView->setDisabled(true);
    webView->showFullScreen();

    webView->load(QUrl::fromLocalFile(animPath));
    //webView->setHtml("<html><body style=\"margin:0;padding:0;border:0;border-radius:0px;\"><img src=\"qrc:/wallpapers/cyclical_1080p.png\"></body></html>");
    webView->show();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        openContextMenu();
        event->accept();
    }else if (event->button() == Qt::LeftButton)
    {
        event->ignore();
    }
}

void MainWindow::openContextMenu()
{
    desktopcm* cm;
    cm = new desktopcm(this);
    QPoint mousePosition = QCursor::pos();

    // Set the position of the context menu relative to the mouse position
    cm->move(mousePosition);
    cm->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore(); // Ignore the close event
}

void MainWindow::cancelDownloadRequest(QWebEngineDownloadRequest* request)
{
    request->cancel();
}

void MainWindow::updateUI()
{
    MainWindow::ReadSettings(); // Assuming the label in MainWindow is named 'label'
}



void MainWindow::openChooseWallpaper()
{
    settingswindow *swView = new settingswindow(this);
    if(!swView->isVisible())
    {
        swView->show();
    }else
    {
        swView->close();
    }
}
