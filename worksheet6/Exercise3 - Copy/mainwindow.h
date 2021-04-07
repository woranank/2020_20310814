#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
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
#include <vtkUnstructuredGrid.h>

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
	//void on_modelColor_clicked();
	void on_filterClip_clicked(bool checked);
	
	
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	
	
	
};

#endif // MAINWINDOW_H
