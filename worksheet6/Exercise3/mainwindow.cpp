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
	connect( ui->modelColor, &QPushButton::clicked, this, &MainWindow::on_modelColor_clicked );
	connect( ui->bgColor, &QPushButton::clicked, this, &MainWindow::on_bgColor_clicked );
	connect( ui->modelView, &QPushButton::clicked, this, &MainWindow::on_modelView_clicked );
	connect( ui->filterClip, &QAbstractButton::clicked, this, &MainWindow::on_filterClip_clicked );
	connect( ui->filterShrink, &QAbstractButton::clicked, this, &MainWindow::on_filterShrink_clicked );
	connect( ui->lightSlider, &QSlider::sliderMoved, this, &MainWindow::on_lightSlider_sliderMoved );
	
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

		// Add light
		light = vtkSmartPointer<vtkLight>::New();
        light->SetLightTypeToHeadlight();
        /*light->SetPosition( 5, 5, 15 );
		light->SetPositional( true );
		light->SetConeAngle( 10 );
		light->SetFocalPoint( 0, 0, 0 );
		light->SetDiffuseColor( 1, 1, 1 );
		light->SetAmbientColor( 1, 1, 1 );
		light->SetSpecularColor( 1, 1, 1 );*/
		light->SetIntensity( 1 );
		
		// Create an actor that is used to set the model's properties for rendering and place it in the window
		actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		colors = vtkSmartPointer<vtkNamedColors>::New();
	    actor->GetProperty()->SetColor( colors->GetColor3d("White").GetData() );
		actor->GetProperty()->EdgeVisibilityOn();

		// Create a renderer, and render window
		renderer = vtkSmartPointer<vtkRenderer>::New();
		ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

		renderer->AddActor(actor);
		renderer->AddLight(light);
		renderer->SetBackground( colors->GetColor3d("Silver").GetData() );
		
		// Setup the renderers's camera
		renderer->ResetCamera();
		renderer->GetActiveCamera()->Azimuth(30);
		renderer->GetActiveCamera()->Elevation(30);
		renderer->ResetCameraClippingRange();
	}
}

void MainWindow::on_modelColor_clicked()
{
	QColor color = QColorDialog::getColor(Qt::white, this);
	
	if ( color.isValid() )
	{
		actor->GetProperty()->SetColor(color.redF(),color.greenF(),color.blueF());
		ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );
	}
}

void MainWindow::on_bgColor_clicked()
{
	QColor color = QColorDialog::getColor(Qt::white, this);
	
	if ( color.isValid() )
	{
		renderer->SetBackground(color.redF(),color.greenF(),color.blueF());
	}		
}

void MainWindow::on_modelView_clicked()
{
	camera = vtkSmartPointer<vtkCamera>::New();
	camera->SetPosition(0, 0, 20);
	camera->SetFocalPoint(0, 0, 0);
  
	// Create a renderer, render window, and interactor
	vtkSmartPointer<vtkRenderer>::New();
	renderer->SetActiveCamera(camera);
	
}

void MainWindow::on_lightSlider_sliderMoved(int position)
{
	position = ui->lightSlider->value();
	light->SetIntensity((float)(position) / 100);
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}


void MainWindow::on_filterClip_clicked(bool checked)
{
	if (checked)
	{
		planeLeft = vtkSmartPointer<vtkPlane>::New();
		planeLeft->SetOrigin(0.0, 0.0, 0.0);
		planeLeft->SetNormal(-1.0, 0.0, 0.0);
	
		clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
		clipFilter->SetInputConnection( reader->GetOutputPort() ) ;
		clipFilter->SetClipFunction( planeLeft.Get() );
		
		mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputConnection( clipFilter->GetOutputPort() );
	}
	
	//ui->qvtkWidget->GetRenderWindow()->Render();
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );
}

void MainWindow::on_filterShrink_clicked(bool checked)
{
	if (checked)
	{
		shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
		shrinkFilter->SetInputConnection( reader->GetOutputPort() );
		shrinkFilter->SetShrinkFactor(.8);
		shrinkFilter->Update();
		mapper->SetInputConnection( shrinkFilter->GetOutputPort() );
	}
	else
	{
		shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
		shrinkFilter->SetInputConnection( reader->GetOutputPort() );
		shrinkFilter->SetShrinkFactor(1);
		shrinkFilter->Update();
		mapper->SetInputConnection( shrinkFilter->GetOutputPort() );
	}
	
	ui->qvtkWidget->GetRenderWindow()->Render();
	//ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );
}


MainWindow::~MainWindow()
{
    delete ui;
}

	



















