#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSTLReader.h>
#include <vtkCubeSource.h>

#include <vtkHexahedron.h>
#include <vtkTetra.h>
#include <vtkPyramid.h>

#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>
#include <vtkPlane.h>

#include <QFileDialog>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
	
	void on_actionFileOpen_triggered();
	
	void handleClipFilter(bool checked);
	void handleShrinkFilter(bool checked);
	
	void handleModelColor();
	void handleModelView();
	
	void handleLightIntensity();
	
	int objectType = 0;
	
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	
	/*
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> hexs = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkUnstructuredGrid> uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	*/
	
	vtkSmartPointer<vtkShrinkFilter> shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
	vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
	vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
	
};

#endif // MAINWINDOW_H
