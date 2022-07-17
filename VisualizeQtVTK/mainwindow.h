#pragma once

#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include "QVTKOpenGLWidget.h"

#include "myvisualizer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

    void onLoadSTL();
    void onLoadCSV();

private:
    Ui::MainWindowClass ui;

    QVTKOpenGLWidget* qVTKWidget_ = new QVTKOpenGLWidget();
    MyQtVTKVisualizer* visualizer_ = new MyQtVTKVisualizer();

    int feature_index_ = 3;
};
