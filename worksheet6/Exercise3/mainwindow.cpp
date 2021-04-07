#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi( this );
	
	// Now need to create a VTK render window and link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow( renderWindow );
	
	
    connect( ui->actionFileOpen, &QAction::triggered, this, &MainWindow::on_actionFileOpen_triggered );
	connect( ui->modelColor, &QPushButton::released, this, &MainWindow::on_modelColor_released );
	//connect( ui->modelView, &QPushButton::clicked, this, &MainWindow::on_modelView_clicked );
	//connect( ui->filterClip, &QAbstractButton::clicked, this, &MainWindow::on_filterClip_clicked );
	//connect( ui->filterShrink, &QAbstractButton::clicked, this, &MainWindow::on_filterShrink_clicked );


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionFileOpen_triggered()
{
	// Create a model from ReadSTL
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files(*.stl)"));
	
	if ( fileName.isEmpty() )
	{
		// if file name in previous step does not exist, return
		return;
    }

    else 
	{
		//Visualize
		//Reader
		reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(fileName.toLatin1().data());
		reader->Update();

		// Create a mapper that will hold the model's geometry in a format suitable for rendering
		mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputConnection(reader->GetOutputPort());

		// Create an actor that is used to set the model's properties for rendering and place it in the window
		actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->EdgeVisibilityOn();

		// Create a renderer, and render window
		renderer = vtkSmartPointer<vtkRenderer>::New();
		ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

		renderer->AddActor(actor);
		
		//colors = vtkSmartPointer<vtkNamedColors>::New();
		//renderer->SetBackground( colors->GetColor3d("Silver").GetData() );
		renderer->SetBackground(.3, .6, .3); // Background color green 	
		
		/*
		// Setup the renderers's camera
		renderer->ResetCamera();
		renderer->GetActiveCamera()->Azimuth(30);
		renderer->GetActiveCamera()->Elevation(30);
		renderer->ResetCameraClippingRange();*/
	}
}

void MainWindow::on_modelColor_released()
{
	QColor color = QColorDialog::getColor(Qt::white, this);
	
	if ( color.isValid() )
	{
		actor->GetProperty()->SetColor(color.redF(),color.greenF(),color.blueF());
		ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );
	}		
}

/*
void MainWindow::on_filterClip_clicked(bool checked)
{
	if (checked)
	{
		// this will apply a clipping plane whose normal is the x-axis that crosses the x-axis at x=0
		vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
		planeLeft->SetOrigin(0.0, 0.0, 0.0);
		planeLeft->SetNormal(-1.0, 0.0, 0.0);
	
		vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
		vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
		clipFilter->SetInputConnection( reader->GetOutputPort() ) ;
		clipFilter->SetClipFunction( planeLeft.Get() );
		
		vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputConnection( clipFilter->GetOutputPort() );
	}
	
	ui->qvtkWidget->GetRenderWindow()->Render();
}*/
	



















