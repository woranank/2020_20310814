#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkCubeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>

#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QColor>
#include <QColorDialog>
#include <QSlider>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	 
	void on_actionFileOpen_triggered();
	void on_modelColor_clicked();
	void on_bgColor_clicked();
	void on_modelView_clicked();
	void on_filterClip_clicked(bool checked);
	void on_filterShrink_clicked(bool checked);
	void on_lightSlider_sliderMoved(int position);
	
	

private:
    Ui::MainWindow *ui;
	
	vtkSmartPointer<vtkCubeSource> cubeSource;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkNamedColors> colors;
    vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkSTLReader> reader;
	vtkSmartPointer<vtkLight> light;
	vtkSmartPointer<vtkCamera> camera;
	vtkSmartPointer<vtkPlane> planeLeft;
	
	vtkSmartPointer<vtkClipDataSet> clipFilter; 
	vtkSmartPointer<vtkShrinkFilter> shrinkFilter;
	
};

#endif // MAINWINDOW_H
