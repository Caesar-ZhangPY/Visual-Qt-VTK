#pragma once
#include "qmap.h"
#include "qvector.h"
#include "qwidget.h"

#include "QVTKOpenGLWidget.h"

#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkMapper.h"
#include "vtkAxesActor.h"
#include "vtkScalarBarActor.h"

#include "vtkSmartPointer.h"

#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

/// @brief 基于QVTKOpenGLWidget的显示渲染窗口
/// @details 可直接在ui design中将widget控件提升为MyQVTKWidget进行拖拽、拉伸等设计。
/// 可载入stl文件并在本窗体中显示，
/// 同时可载入csv格式的离散点坐标文件，根据指定特征对离散点渲染颜色并显示在本窗体中
class MyQVTKWidget : public QVTKOpenGLWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数，初始化坐标轴actor，设置窗体颜色背景，并关联VTK渲染管线的各个流程环节
	/// @param parent Qt对象父类，通常指定为本窗体所在窗口
	MyQVTKWidget(QWidget* parent = nullptr);
	~MyQVTKWidget();

	/// @brief 载入stl文件
	/// @param stl_file stl文件路径
	void LoadSTL(QString stl_file);
	/// @brief 载入csv格式的离散点数据文件
	/// @details 离散点格式要求：从第二行开始，每一行即描述一个点，
	/// 第1、2、3列分别为x、y、z坐标，第4列及以后，每一列为这个点的一个特征。
	/// 第一行为各列的索引名
	/// @param csv_file csv文件路径
	void LoadData(QString csv_file);
	/// @brief 根据指定特征值渲染离散点颜色
	/// @param feature_index 特征索引，即csv文件中特征所在列数（从0开始计数）
	void ColourData(int feature_index = 3);
	/// @brief 根据指定特征值渲染离散点颜色
	/// @param feature_index 特征名称
	void ColourData(QString feature_name);
	
	/// @brief 卸载全部数据
	void DropAll();
	/// @brief 刷新窗体，重新渲染
	void RefreshWidgetRender();

protected:
	/// @brief 重写鼠标双击事件，左键双击缩小散点直径，右键双击增大散点直径
	virtual void mouseDoubleClickEvent(QMouseEvent* mouse_event) override;

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

	QVector<QVector<double>> data_frame_;
	QMap<QString, int> column_name_;

	unsigned int point_size_ = 3;

signals:
	void featuresLoaded(QStringList feature_list);
};