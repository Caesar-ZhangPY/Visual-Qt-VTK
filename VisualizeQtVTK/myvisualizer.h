#pragma once

#include "qobject.h"
#include "qstring.h"
#include "qvector.h"
#include "qmap.h"
#include "qdebug.h"
#include "qdir.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qmap.h"

#include "QVTKOpenGLWidget.h"

#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkMapper.h"
#include "vtkAxesActor.h"
#include "vtkScalarBarActor.h"

#include "vtkSmartPointer.h"

#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

class MyQtVTKVisualizer : public QObject
{
	Q_OBJECT

public:
	MyQtVTKVisualizer(QObject* parent = nullptr);
	~MyQtVTKVisualizer();

	void LoadSTL(QString stl_file);

	void LoadData(QString csv_file);

	void ColourData(int feature_index = 3);
	void ColourData(QString feature_name);
	
	void DropAll();

	void RefreshWidgetRender();

	void SetWidget(QVTKOpenGLWidget* widget);

	QMap<QString, int> column_name();

private:

	vtkSmartPointer<vtkAxesActor> axes_actor_ 
		= vtkSmartPointer<vtkAxesActor>::New();

	vtkSmartPointer<vtkActor> stl_actor_ 
		= vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkActor> points_actor_ 
		= vtkSmartPointer<vtkActor>::New();

	vtkSmartPointer<vtkScalarBarActor> scalarbar_actor_ 
		= vtkSmartPointer<vtkScalarBarActor>::New();

	vtkSmartPointer<vtkRenderer> renderer_ 
		= vtkSmartPointer<vtkRenderer>::New();

	QVTKOpenGLWidget* widget_ = nullptr;

	QVector<QVector<double>> data_frame_;
	QMap<QString, int> column_name_;
};