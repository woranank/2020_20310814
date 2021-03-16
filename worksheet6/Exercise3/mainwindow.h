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
	
	void handleClipCheckbox(int);
	void handleShrinkCheckbox(int);
	
	void handleBGColorButton();
	void handleModelColorButton();
	void handleModelShapeButton();
	void handleViewButton();
	
	void handleIntensitySlider(int);
	
	void handleHexaButton();
	void handleTetraButton();
	void handlePyraButton();
	
	void createHexahedron();
	void createTetrahedron();
	void createPyramid();
	
	void shrinkFilterFunc();
	void clipFilterFunc();
	
	void refreshRender();
	
	int objectType = 0;
	
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> hexs = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkUnstructuredGrid> uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	std::vector<std::array<double, 3>> pointCoordinates;
	
};

#endif // MAINWINDOW_H
