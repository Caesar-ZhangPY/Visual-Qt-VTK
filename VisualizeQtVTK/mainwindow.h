#pragma once
#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include "QVTKOpenGLWidget.h"

#include "myqvtkwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

    void onLoadSTL();
    void onLoadCSV();
    void onSaveImage();

private:
    Ui::MainWindowClass ui;

    QString copy_right_ = "Copyright (C) 2022 NWPU-Zhang Puyu. All rights reserved.";
};
