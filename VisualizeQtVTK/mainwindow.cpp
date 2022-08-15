#include "mainwindow.h"
#pragma execution_character_set("utf-8")

#include "qfiledialog.h"
#include "qstring.h"
#include "qaction.h"
#include "qvector.h"
#include "qmenu.h"
#include "qaction.h"
#include "qdebug.h"
#include "qpushbutton.h"
#include "qslider.h"
#include "qaction.h"
#include "qlabel.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.loadSTL, &QAction::triggered, this, &MainWindow::onLoadSTL);
    connect(ui.loadCSV, &QAction::triggered, this, &MainWindow::onLoadCSV);
    connect(ui.dropAllData, &QAction::triggered, ui.myQVTKWidget, &MyQVTKWidget::DropAll);

    connect(ui.myQVTKWidget, &MyQVTKWidget::featuresLoaded, [=](QStringList column_name) {
        for (QString feature : column_name)
        {
            QAction* action = new QAction(feature);
            ui.visualizeMenu->addAction(action);
        }
        });
    connect(ui.visualizeMenu, &QMenu::triggered, [=](QAction* action) {
        ui.myQVTKWidget->ColourData(action->text());
        });

    connect(ui.saveImage, &QAction::triggered, this, &MainWindow::onSaveImage);

    /*QLabel* copyRights = new QLabel(this);
    QLabel* thanks = new QLabel(this);
    copyRights->setText(tr("Copyright (C) 2022 NWPU-张璞玉. All rights reserved. "));
    thanks->setText(tr(" 基于对刘冬生成果的整理 "));
    ui.statusBar->addPermanentWidget(copyRights);
    ui.statusBar->addPermanentWidget(thanks);*/
}

void MainWindow::onLoadSTL()
{
    QString stl_file = QFileDialog::getOpenFileName(
        this, tr("Open .stl file"), "./", "STL file (*.stl)");
    if (stl_file.isEmpty()) return;

    ui.myQVTKWidget->LoadSTL(stl_file);
}

void MainWindow::onLoadCSV()
{
    QString csv_file = QFileDialog::getOpenFileName(
        this, tr("Open .csv file"), "./", "CSV file (*.csv)");
    if (csv_file.isEmpty()) return;

    ui.myQVTKWidget->LoadData(csv_file);
    ui.myQVTKWidget->ColourData(3);
}

void MainWindow::onSaveImage()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save image to"),
        "./", tr("Images (*.png *.jpeg *.jpg)"));
    if (path.isEmpty()) return;

    QPixmap pixmap = ui.myQVTKWidget->grab();
    QImage image = pixmap.toImage();
    image.save(path);
}
