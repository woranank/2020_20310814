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
#include <vtkUnstructuredGrid.h>

#include <vtkShrinkFilter.h>
#include <vtkClipDataSet.h>

#include <vtkHexahedron.h>
#include <vtkTetra.h>
#include <vtkPyramid.h>

#include <array>
#include <vector>

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
	
	// function to create and disply hexahedron
	// Hexahedron pushbutton, activated when clicked.
	void on_hexa_clicked();
	
	// function to create and disply pyrmid
	// pyramid pushbutton, activated when clicked.
	void on_pyramid_clicked();
	
	// function to create and disply tetrahedron
	// tetrahedron pushbutton, activated when clicked.
	void on_tetra_clicked();
	
	
	
	// function to open file 
	// icon on bar, activated when openfile icon is triggered.
	void on_actionFileOpen_triggered();
	
	// function to change model color
	// Model Color pushbutton, activated when clicked.
	void on_modelColor_clicked();
	
	// function to change background color
	// Change BG color pushbutton, activated when clicked.
	void on_bgColor_clicked();
	
	// function to change model view to top view set up
	// Model View pushbutton, activated when clicked.
	void on_modelView_clicked();
	
	// function to apply clip filter to model
	// Clip filter checkbox, activated when checkbox is checked.
	void on_filterClip_clicked(bool checked);
	
	// function to apply shrink filter to model
	// Shrink filter checkbox, activated when checkbox is checked.
	void on_filterShrink_clicked(bool checked);
	
	// function to change model's light intensity
	// Light Intensity slider bar, activated whe slider is moved.
	// light intensity depends on the pointer's position.
	void on_lightSlider_sliderMoved(int position);
	
	// integer to indicate model's type
	// to be able to apply the right function of each filter.
	// type 0: Cube Source: initial model.
	// type 1: Hexahedron, Tetrahedron, Pyramid: model from coordinates. 
	// type 2: STL file: model loaded from STL files.
	int type=0;



private:
    Ui::MainWindow *ui;
	
	// declare function's name and type
	vtkSmartPointer<vtkCubeSource> cubeSource;
	vtkSmartPointer<vtkPoints> points;
	vtkSmartPointer<vtkUnstructuredGrid> uGrid;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
    vtkSmartPointer<vtkNamedColors> colors;
    vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkSTLReader> reader;
	vtkSmartPointer<vtkLight> light;
	vtkSmartPointer<vtkCamera> camera;
	vtkSmartPointer<vtkPlane> planeLeft;
	vtkSmartPointer<vtkClipDataSet> clipFilter; 
	vtkSmartPointer<vtkShrinkFilter> shrinkFilter;
	vtkSmartPointer<vtkCellArray> cells;
	std::vector<std::array<double, 3>> pointCoordinates;
		
};

#endif // MAINWINDOW_H
