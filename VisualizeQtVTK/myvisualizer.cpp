#include "myvisualizer.h"

#include <algorithm>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingFreeType)

#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkMapper.h"
#include "vtkPoints.h"

#include "vtkSTLReader.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyVertex.h"
#include "vtkDoubleArray.h"
#include "vtkTextActor.h"
#include "vtkAxesActor.h"
#include "vtkAxisActor2D.h"
#include "vtkCaptionActor2D.h"
#include "vtkTextProperty.h"
#include "vtkLookupTable.h"
#include "vtkUnstructuredGrid.h"
#include "vtkScalarBarActor.h"
#include "vtkDataSetMapper.h"
#include "vtkProperty.h"

MyQtVTKVisualizer::MyQtVTKVisualizer(QObject *parent)
	: QObject(parent)
{     
    // 设置坐标轴
    axes_actor_->SetTotalLength(20, 20, 20);
    axes_actor_->SetTipTypeToCone();
    axes_actor_->AxisLabelsOn();

    axes_actor_->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axes_actor_->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axes_actor_->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);

    axes_actor_->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axes_actor_->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axes_actor_->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 1, 0);

    axes_actor_->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axes_actor_->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axes_actor_->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 1);

    renderer_->AddActor(axes_actor_);

    // 设置渐变背景
    renderer_->SetBackground(0.1, 0.2, 0.4);
    renderer_->SetBackground2(1, 1, 1);
    renderer_->SetGradientBackground(1);
}

MyQtVTKVisualizer::~MyQtVTKVisualizer()
{
}

void MyQtVTKVisualizer::LoadSTL(QString stl_file)
{
    if (stl_file.isEmpty()) return;
    
    vtkSmartPointer<vtkSTLReader> stl_reader = 
        vtkSmartPointer<vtkSTLReader>::New();
    stl_reader->SetFileName(stl_file.toLatin1());

    vtkSmartPointer<vtkPolyDataMapper> stl_mapper 
        = vtkSmartPointer<vtkPolyDataMapper>::New();
    stl_mapper->SetInputConnection(stl_reader->GetOutputPort());

    stl_actor_->SetMapper(stl_mapper);

    renderer_->AddActor(stl_actor_);

    RefreshWidgetRender();
}

void MyQtVTKVisualizer::DropAll()
{
    renderer_->RemoveActor(stl_actor_);
    renderer_->RemoveActor(points_actor_);
    renderer_->RemoveActor(scalarbar_actor_);

    RefreshWidgetRender();
}

void MyQtVTKVisualizer::RefreshWidgetRender()
{
    if (widget_ != nullptr)
        widget_->GetRenderWindow()->Render();
}


void MyQtVTKVisualizer::ColourData(int feature_index)
{
    auto min_scalar = std::min_element(data_frame_.begin(), data_frame_.end(), 
        [=](QVector<double> p, QVector<double> q)->bool {return p[feature_index] < q[feature_index]; })
        ->at(feature_index);
    auto max_scalar = std::max_element(data_frame_.begin(), data_frame_.end(), 
        [=](QVector<double> p, QVector<double> q)->bool {return p[feature_index] < q[feature_index]; })
        ->at(feature_index);
    
    // 几何数据
    vtkSmartPointer<vtkPoints> points 
        = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < data_frame_.size(); i++)
        points->InsertPoint(i, data_frame_[i][0], data_frame_[i][1], data_frame_[i][2]);

    //拓扑数据
    vtkSmartPointer<vtkPolyVertex> poly_vertex   //单元：多顶点，是O维单元的组合，
        = vtkSmartPointer<vtkPolyVertex>::New();
    //设置ID个数并分配存储ID的内存,必须设置Id个数，否则可以编译，不能运行 
    poly_vertex->GetPointIds()->SetNumberOfIds(data_frame_.size());
    for (int i = 0; i < data_frame_.size(); i++)
        //第一个参数是几何point的ID号，第2个参数是拓扑中的Id号
        poly_vertex->GetPointIds()->SetId(i, i);

    //属性数据
    vtkSmartPointer<vtkDoubleArray> points_scalars
        = vtkSmartPointer<vtkDoubleArray>::New();
    for (int i = 0; i < data_frame_.size(); i++)
        //第1个参数是points点的Id，第2个参数是该点的属性值
        points_scalars->InsertTuple1(i, data_frame_[i][feature_index]);
    
    // 颜色表
    vtkSmartPointer<vtkLookupTable> lut     
        = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(data_frame_.size());    //颜色个数
    lut->SetNumberOfColors(256);
    lut->SetHueRange(0.7, 0);       //颜色色调
    lut->SetAlphaRange(1.0, 1.0);   //颜色饱和度
    lut->SetValueRange(1.0, 1.0);
    lut->SetSaturationRange(0.8, 0.8);  //颜色亮度
    lut->SetRange(min_scalar, max_scalar);
    lut->Build();

    // 将以上三部分数据组合成一个结构为vtkUnstructureGrid数据集
    vtkSmartPointer<vtkUnstructuredGrid> grid 
        = vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid->Allocate(1, 1);
    grid->SetPoints(points);
    grid->InsertNextCell(poly_vertex->GetCellType(), poly_vertex->GetPointIds()); //设置映射器
    grid->GetPointData()->SetScalars(points_scalars);

    vtkSmartPointer<vtkDataSetMapper> points_mapper 
        = vtkSmartPointer<vtkDataSetMapper>::New();
    // 根据属性数据的最大、最小值，将颜色表和属性数据关联
    points_mapper->SetInputData(grid);
    points_mapper->ScalarVisibilityOn();
    points_mapper->SetScalarRange(min_scalar, max_scalar);
    points_mapper->SetLookupTable(lut);
    points_mapper->SetColorModeToDefault();

    scalarbar_actor_->GetTitleTextProperty()->SetColor(0.1, 0.1, 0.1);
    scalarbar_actor_->GetTitleTextProperty()->SetFontSize(5);
    scalarbar_actor_->GetTitleTextProperty()->ItalicOff();
    // _scalarBar_actor->GetTitleTextProperty()->BoldOff();
    scalarbar_actor_->SetLookupTable(points_mapper->GetLookupTable());
    scalarbar_actor_->SetTitle(column_name_.key(feature_index).toLatin1());
    scalarbar_actor_->SetNumberOfLabels(5); //设置5个标签
    scalarbar_actor_->SetHeight(0.70);
    scalarbar_actor_->SetWidth(0.10);

    points_actor_->SetMapper(points_mapper);
    points_actor_->GetProperty()->SetRepresentationToPoints();
    points_actor_->GetProperty()->SetPointSize(3);

    renderer_->AddActor(points_actor_);
    renderer_->AddActor2D(scalarbar_actor_);

    RefreshWidgetRender();
}

void MyQtVTKVisualizer::ColourData(QString feature_name)
{
    ColourData(column_name_.value(feature_name));
}

void MyQtVTKVisualizer::SetWidget(QVTKOpenGLWidget* widget)
{
    widget_ = widget;
    widget->GetRenderWindow()->AddRenderer(renderer_);
    widget->GetRenderWindow()->Render();
}

void MyQtVTKVisualizer::LoadData(QString csv_file)
{
    QDir dir = QDir::current();
    QFile file(dir.filePath(csv_file));

    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "OPEN FILE FAILED";

    QTextStream* ostream = new QTextStream(&file);				//文本流
    QStringList row_list = ostream->readAll().split("\n");		//每行以\n区分

    // 读取第一行的列名
    QStringList first_row = row_list.at(0).split(",");
    for (int i = 0; i < first_row.count(); i++)
        column_name_.insert(first_row[i], i);
    
    // 读取数据
    for (int j = 1; j < row_list.count(); j++)
    {
        QStringList cell_list = row_list.at(j).split(",");      //一行中的单元格以，区分
        QVector<double> point;
        for (QString cell : cell_list)
            point.push_back(cell.toDouble());
        data_frame_.push_back(point);
    }

    // 防止最后一行有个回车
    if (data_frame_.last().count() != data_frame_.first().count())
        data_frame_.pop_back();

    file.close();
}

QMap<QString, int> MyQtVTKVisualizer::column_name()
{
    return this->column_name_;
}


