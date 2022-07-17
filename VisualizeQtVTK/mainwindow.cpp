#include "mainwindow.h"

#include "qfiledialog.h"
#include "qstring.h"
#include "qaction.h"
#include "qvector.h"
#include "qmenu.h"
#include "qaction.h"
#include "qdebug.h"
#include "qpushbutton.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setCentralWidget(qVTKWidget_);

    visualizer_->SetWidget(qVTKWidget_);

    connect(ui.loadSTL, &QAction::triggered, this, &MainWindow::onLoadSTL);
    connect(ui.loadCSV, &QAction::triggered, this, &MainWindow::onLoadCSV);
    connect(ui.dropAllData, &QAction::triggered, visualizer_, &MyQtVTKVisualizer::DropAll);
}

void MainWindow::onLoadSTL()
{
    QString stl_file = QFileDialog::getOpenFileName(
        this, tr("Open .stl file"), "../data", "STL file (*.stl)");

    if (stl_file.isEmpty()) return;

    visualizer_->LoadSTL(stl_file);
}

void MainWindow::onLoadCSV()
{
    QString csv_file = QFileDialog::getOpenFileName(
        this, tr("Open .csv file"), "./", "CSV file (*.csv)");

    if (csv_file.isEmpty()) return;

    visualizer_->LoadData(csv_file);
    visualizer_->ColourData(feature_index_);

    connect(ui.nextFeature, &QAction::triggered, [=]() {
        feature_index_++;
        if (feature_index_ >= visualizer_->column_name().size())
            feature_index_ = 3;
        visualizer_->ColourData(feature_index_);
        });
    connect(ui.prevFeature, &QAction::triggered, [=]() {
        feature_index_--;
        if (feature_index_ < 3)
            feature_index_ = visualizer_->column_name().size() - 1;
        visualizer_->ColourData(feature_index_);
        });
}
