#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkCubeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
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
#include <vtkPlane.h>

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QColor>
#include <QColorDialog>


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
	void on_modelColor_released();
	//void on_modelView_released();
	//void on_filterClip_clicked(bool checked);
	//void on_filterShrink_clicked(bool checked);
	

private:
    Ui::MainWindow *ui;
	
	vtkSmartPointer<vtkCubeSource> cubeSource;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkNamedColors> colors;
    vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkSTLReader> reader;
	
};

#endif // MAINWINDOW_H
