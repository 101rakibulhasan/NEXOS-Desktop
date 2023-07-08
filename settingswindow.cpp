#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "mainwindow.h"

#include <QDebug>
#include <QRegularExpression>
#include <QStringList>
#include <QFileInfo>

settingswindow::settingswindow(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent), mainWindow(mainWindow),
    ui(new Ui::settingswindow)
{
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    this->setFixedSize(720,225);

    ui->mode->addItem("Image");
    ui->mode->addItem("Video");
    ui->mode->addItem("Animation");

    QObject::connect(ui->mode, QOverload<int>::of(&QComboBox::activated), [&](int index) {

        if(index == 0)
        {
            newType = "image";
        }else if(index == 1)
        {
            newType = "video";
        }else if(index == 2)
        {
            newType = "animation";
        }

        ui->filePath->setText("");
        newFile = "";
    });
}

settingswindow::~settingswindow()
{
    delete ui;
}

void settingswindow::on_cancel_clicked()
{
    this->close();
}

void settingswindow::on_apply_clicked()
{
    newFile = ui->filePath->text();
//    if(newFile != "")
//    {
//        QFileInfo checkFile(newFile);
//        if(checkFile.isFile())
//        {
//            newFile = ui->filePath->text();
//        }else
//        {
//            newFile = "";
//            ui->filePath->text() = "";
//        }
//    }

//    if(!newFile.isNull())
//    {
        QFile file(mainWindow->settingPath);
        QString line;
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream in(&file);
            QStringList lines;
            while (!in.atEnd()) {
                line = in.readLine();
                lines.append(line);
                if (line.contains("type: ")) {
                    if (!lines.isEmpty()) {
                        QString line = lines.last();
                        int start = line.indexOf("\"") + 1;
                        int end = line.lastIndexOf("\"");
                        //QString oldValue = line.mid(start, end - start);

                        line.replace(start, end - start, newType);

                        // Replace the modified line in the list
                        lines[lines.size() - 1] = line;
                    }
                }
            }

            file.resize(0);  // Clear the file content
            for (const QString& line : lines) {
                in << line << "\n";
            }

            in.seek(0);
            lines.clear();

            QString search;

            if(newType == "image")
            {
                search = "imagePath: ";
            }else if(newType == "video")
            {
                search = "videoPath: ";
            }else if(newType == "animation")
            {
                search = "animPath: ";
            }

            if(newFile == "")
            {
                if(newType == "image")
                {
                    newFile = mainWindow->imageDefPath;
                }else if(newType == "video")
                {
                    newFile = mainWindow->videoDefPath;
                }else if(newType == "animation")
                {
                    newFile = mainWindow->animDefPath;
                }
            }

            //qDebug() << search;

            while (!in.atEnd()) {
                line = in.readLine();
                lines.append(line);
                if (line.contains(search)) {
                    if (!lines.isEmpty()) {
                        QString line = lines.last();
                        int start = line.indexOf("\"") + 1;
                        int end = line.lastIndexOf("\"");

                        line.replace(start, end - start, newFile);

                        // Replace the modified line in the list
                        lines[lines.size() - 1] = line;
                    }
                }
            }

            file.resize(0);  // Clear the file content
            for (const QString& line : lines) {
                in << line << "\n";
            }
        }

        file.close();
    //}

    mainWindow->setVariables();
}



void settingswindow::on_browse_clicked()
{
    if(newType == "image")
    {
        newFile = QFileDialog::getOpenFileName(nullptr, "Open Image", QString(), "Image Files (*.png *.jpg *.jpeg)");
    }else if(newType == "video")
    {
        newFile = QFileDialog::getOpenFileName(nullptr, "Open Video", QString(), "Video Files (*.mp4 *.webm *.mkv)");
    }else if(newType == "animation")
    {
        newFile = QFileDialog::getOpenFileName(nullptr, "Open HTML", QString(), "HTML Files (*.html)");
    }
    ui->filePath->setText(newFile);
}


void settingswindow::on_filePath_textChanged(const QString &arg1)
{

}

