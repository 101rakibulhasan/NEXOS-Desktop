#include "desktopcm.h"
#include "ui_desktopcm.h"
#include "settingswindow.h"

#include <QStringListModel>
#include <QDebug>

desktopcm::desktopcm(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent), mainWindow(mainWindow),
    ui(new Ui::desktopcm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    if(this->isHidden())
    {
        this->close();
    }

    model = new QStandardItemModel(this);

    ui->listView->setModel(model);

    int desktopcm_width = this->width();
    int desktopcm_height = this->height();
    ui->listView->resize(desktopcm_width,desktopcm_height);

    QString changeWallpaper_text = "Change Wallpaper";
    QStandardItem *changeWallpaper_item = new QStandardItem(changeWallpaper_text);
    changeWallpaper_item->setEditable(false);
    model->appendRow(changeWallpaper_item);

    QStandardItem *appeareance_item = new QStandardItem;
    appeareance_item->setData(QIcon(":/path/to/icon.png"), Qt::DecorationRole);
    appeareance_item->setData("Appearance", Qt::DisplayRole);
    appeareance_item->setEditable(false); // Set text non-editable
    model->appendRow(appeareance_item);

    QString display_text = "Display";
    QStandardItem *display_item = new QStandardItem(display_text);
    display_item->setEditable(false);
    model->appendRow(display_item);

    QStandardItem *setting_item = new QStandardItem;
    setting_item->setData(QIcon(":/path/to/icon.png"), Qt::DecorationRole);
    setting_item->setData("Setting", Qt::DisplayRole);
    setting_item->setEditable(false); // Set text non-editable
    model->appendRow(setting_item);



    connect(ui->listView, &QListView::clicked, this, &desktopcm::handleItemClick);

}

desktopcm::~desktopcm()
{
    delete ui;
}

// Step 4: Define the slot function that will be called when an item is clicked (same as previous example)
void desktopcm::handleItemClick(const QModelIndex &index)
{
    QStandardItem *clickedItem = model->itemFromIndex(index);
    QString itemText = clickedItem->text();
    //int itemValue = itemText.toInt();

    if(itemText == "Change Wallpaper")
    {
        mainWindow->openChooseWallpaper();
    }

    this->close();

    qDebug() << itemText;
}
