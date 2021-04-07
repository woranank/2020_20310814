#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) 
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);
	
	// Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    ui->qvtkWidget->SetRenderWindow( renderWindow );
	

    //Action Open File
    connect( ui->actionFileOpen, &QAction::triggered, this, &MainWindow::on_actionFileOpen_triggered );


    //Checkbox functions
    connect( ui->clip_Filter, &QCheckBox::clicked, this, &MainWindow::handleClipFilter );
    connect( ui->shrink_Filter, &QCheckBox::clicked, this, &MainWindow::handleShrinkFilter );

    //Pushbutton functions
    connect( ui->model_Color, &QPushButton::released, this, &MainWindow::handleModelColor );
    connect( ui->model_View, &QPushButton::released, this, &MainWindow::handleModelView );

    //Slider function
    connect( ui->light_Intensity, &QSlider::valueChanged, this, &MainWindow::handleLightIntensity );
	

}

MainWindow::~MainWindow()
{
    delete ui;
}


//Open file from directory
void MainWindow::on_actionFileOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files(*.stl)"));
	
	if (fileName.isEmpty())
	{
		// if file name in previous step does not exist, return
		return;
    }

    else 
	{
		//Visualize
		//Reader
		
		reader->SetFileName(fileName.toLatin1().data());
		reader->Update();

		//Mapper
		mapper->SetInputConnection(reader->GetOutputPort());

		//Actor
		actor->SetMapper(mapper);

		//Renderer
		ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

		renderer->AddActor(actor);
		renderer->SetBackground(.3, .6, .3); // Background color green 	
	}
}

void MainWindow::handleClipFilter(bool checked)
{
	if (checked)
	{
		// this will apply a clipping plane whose normal is the x-axis that crosses the x-axis at x=0
		planeLeft->SetOrigin(0.0, 0.0, 0.0);
		planeLeft->SetNormal(-1.0, 0.0, 0.0);
		clipFilter->SetInputConnection( SOURCE->GetOutputPort() ) ;
		clipFilter->SetClipFunction( planeLeft.Get() );
		mapper->SetInputConnection( clipFilter->GetOutputPort() );
	}
	ui->qtvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::handleShrinkFilter(bool checked)
{
	if (checked){
		shrinkFilter->SetInputConnection( SOURCE->GetOutputPort() );
		shrinkFilter->SetShrinkFactor(.8);
		shrinkFilter->Update();
		mapper->SetInputConnection( shrinkFilter->GetOutputPort() );
	}
	ui->qtvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::handleModelColor()
{
	actor->GetProperty()->SetColor( colors->GetColor3d("Red").GetData() );
	actor->SetMapper(mapper);

	ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
	renderer->AddActor(actor);
}

//void MainWindow::handleModelView(){
//}

void MainWindow::handleLightIntensity(){

    //Render light intensity
    
    light->SetLightTypeToSceneLight();
    light->SetPosition( 5, 5, 15 );
    light->SetPositional( true );
    light->SetConeAngle( 10 );
    light->SetFocalPoint( 0, 0, 0 );
    light->SetDiffuseColor( 1, 1, 1 );
    light->SetAmbientColor( 1, 1, 1 );
    light->SetSpecularColor( 1, 1, 1 );
    light->SetIntensity( 0.5 );

    // now add actors
    renderer->AddActor(actor);
    renderer->AddLight(light);
    renderer->SetBackground(colors->GetColor3d("Red").GetData());

    // Render
    ui->vtkWidget->GetRenderWindow()->Render();
    // add the light to the renderer after Render was called
    renderer->AddLight( light );
}




















