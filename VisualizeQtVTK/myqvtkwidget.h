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

/// @brief ����QVTKOpenGLWidget����ʾ��Ⱦ����
/// @details ��ֱ����ui design�н�widget�ؼ�����ΪMyQVTKWidget������ק���������ơ�
/// ������stl�ļ����ڱ���������ʾ��
/// ͬʱ������csv��ʽ����ɢ�������ļ�������ָ����������ɢ����Ⱦ��ɫ����ʾ�ڱ�������
class MyQVTKWidget : public QVTKOpenGLWidget
{
	Q_OBJECT

public:
	/// @brief ���캯������ʼ��������actor�����ô�����ɫ������������VTK��Ⱦ���ߵĸ������̻���
	/// @param parent Qt�����࣬ͨ��ָ��Ϊ���������ڴ���
	MyQVTKWidget(QWidget* parent = nullptr);
	~MyQVTKWidget();

	/// @brief ����stl�ļ�
	/// @param stl_file stl�ļ�·��
	void LoadSTL(QString stl_file);
	/// @brief ����csv��ʽ����ɢ�������ļ�
	/// @details ��ɢ���ʽҪ�󣺴ӵڶ��п�ʼ��ÿһ�м�����һ���㣬
	/// ��1��2��3�зֱ�Ϊx��y��z���꣬��4�м��Ժ�ÿһ��Ϊ������һ��������
	/// ��һ��Ϊ���е�������
	/// @param csv_file csv�ļ�·��
	void LoadData(QString csv_file);
	/// @brief ����ָ������ֵ��Ⱦ��ɢ����ɫ
	/// @param feature_index ������������csv�ļ�������������������0��ʼ������
	void ColourData(int feature_index = 3);
	/// @brief ����ָ������ֵ��Ⱦ��ɢ����ɫ
	/// @param feature_index ��������
	void ColourData(QString feature_name);
	
	/// @brief ж��ȫ������
	void DropAll();
	/// @brief ˢ�´��壬������Ⱦ
	void RefreshWidgetRender();

protected:
	/// @brief ��д���˫���¼������˫����Сɢ��ֱ�����Ҽ�˫������ɢ��ֱ��
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