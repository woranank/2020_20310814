#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // standard call to setup Qt UI (same as previously)
    ui->setupUi( this );
	
	// Now need to create a VTK render window and link it to the QtVTK widget
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	ui->qvtkWidget->SetRenderWindow( renderWindow );
	
	
	
	// Set up and assign ui input connection to each function name 
	// depending on each Q type (pushbutton, slider, checkbox)
	connect( ui->hexa, &QAbstractButton::clicked, this, &MainWindow::on_hexa_clicked );
	connect( ui->tetra, &QAbstractButton::clicked, this, &MainWindow::on_tetra_clicked );
	connect( ui->pyramid, &QAbstractButton::clicked, this, &MainWindow::on_pyramid_clicked );
	
    connect( ui->actionFileOpen, &QAction::triggered, this, &MainWindow::on_actionFileOpen_triggered );
	connect( ui->modelColor, &QPushButton::clicked, this, &MainWindow::on_modelColor_clicked );
	connect( ui->bgColor, &QPushButton::clicked, this, &MainWindow::on_bgColor_clicked );
	connect( ui->modelView, &QPushButton::clicked, this, &MainWindow::on_modelView_clicked );
	connect( ui->filterClip, &QAbstractButton::clicked, this, &MainWindow::on_filterClip_clicked );
	connect( ui->filterShrink, &QAbstractButton::clicked, this, &MainWindow::on_filterShrink_clicked );
	connect( ui->lightSlider, &QSlider::sliderMoved, this, &MainWindow::on_lightSlider_sliderMoved );
	
	
	
	// Now use the VTK libraries to render something. To start with you can copy-paste the cube example code, there are comments to show where the code must be modified.
	//--------------------------------------------- Code From Example 1 --------------------------------------------------------------------------
	// Create a cube using a vtkCubeSource
	cubeSource = vtkSmartPointer<vtkCubeSource>::New();

	// Create a mapper that will hold the cube's geometry in a format suitable for rendering
	mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputConnection( cubeSource ->GetOutputPort() );
	
	// Add light
	light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToHeadlight();
	light->SetIntensity( 1 );

	// Create an actor that is used to set the cube's properties for rendering and place it in the window
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	// Add actor's color
	colors = vtkSmartPointer<vtkNamedColors>::New();
	actor->GetProperty()->SetColor( colors->GetColor3d("White").GetData() );

	// Create a renderer, and render window
	renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
	ui->qvtkWidget->GetRenderWindow()->AddRenderer( renderer );									// ###### ask the QtVTKOpenGLWidget for its renderWindow ######

	// Link a renderWindowInteractor to the renderer (this allows you to capture mouse movements etc)  ###### Not needed with Qt ######
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow( ui->vtkWidget->GetRenderWindow() );				

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->AddLight(light);
	renderer->SetBackground( colors->GetColor3d("Silver").GetData() );

	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();

	// Render and interact
	//renderWindow->Render();					// ###### Not needed with Qt ######
	//renderWindowInteractor->Start();			// ###### Not needed with Qt ######
	//--------------------------------------------- /Code From Example 1 -------------------------------------------------------------------------*/
}


//FUNCTION TO CREATE AND DISPLAY HEXAHEDRON
void MainWindow::on_hexa_clicked()
{
	// Remove actor and clear points from function prior
	renderer->RemoveActor(actor);
	pointCoordinates.clear();
	
	// Set up type of model (type 1 are hexahedron, tetrahedron and pyramid)
	type = 1;
	
	// Create new color variable
	colors = vtkSmartPointer<vtkNamedColors>::New();

	// For the hexahedron; setup the coordinates of eight points.
	// The two faces must be in counter clockwise order as viewed from the
	// outside.
	//std::vector<std::array<double, 3>> pointCoordinates;
	pointCoordinates.push_back({{-0.5, -0.5, -0.5}}); // Face 1
	pointCoordinates.push_back({{0.5, -0.5, -0.5}});
	pointCoordinates.push_back({{0.5, 0.5, -0.5}});
	pointCoordinates.push_back({{-0.5, 0.5, -0.5}});
	pointCoordinates.push_back({{-0.5, -0.5, 0.5}}); // Face 2
	pointCoordinates.push_back({{0.5, -0.5, 0.5}});
	pointCoordinates.push_back({{0.5, 0.5, 0.5}}); 
	pointCoordinates.push_back({{-0.5, 0.5, 0.5}});
	
	// Create the points.
	points = vtkSmartPointer<vtkPoints>::New();

	// Create a hexahedron from the points.
	vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();

	for (auto i = 0; i < pointCoordinates.size(); ++i)
	{
		points->InsertNextPoint(pointCoordinates[i].data());
		hex->GetPointIds()->SetId(i, i);
	}

	// Add the hexahedron to a cell array.
	cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(hex);

	// Add the points and hexahedron to an unstructured grid.
	uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	uGrid->SetPoints(points);
	uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
	
	// Visualize.
	mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(uGrid);
	
	// Add light
	light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToHeadlight();
	light->SetIntensity( 1 );

	// Add actor's color
	actor = vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();
	
	// Add the actor and light to the scene
	renderer->AddActor(actor);
	renderer->AddLight(light);
	
	// Set background's color
	renderer->SetBackground(colors->GetColor3d("Silver").GetData());
	
	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}


//FUNCTION TO CREATE AND DISPLAY TETRAHEDRON
void MainWindow::on_tetra_clicked()
{
	// Remove actor and clear points from function prior
	renderer->RemoveActor(actor);
	pointCoordinates.clear();
	
	// Set up type of model (type 1 are hexahedron, tetrahedron and pyramid)
	type = 1;
	
	// Create new color variable
	colors = vtkSmartPointer<vtkNamedColors>::New();


	// For the tetrahedron; setup the coordinates of four points.
	pointCoordinates.push_back({{0.0, 0.0, 0.0}}); 
	pointCoordinates.push_back({{1.0, 0.0, 0.0}});
	pointCoordinates.push_back({{1.0, 1.0, 0.0}});
	pointCoordinates.push_back({{0.0, 1.0, 1.0}});

	// Create the points.
	points = vtkSmartPointer<vtkPoints>::New();

	// Create a tetrahedron from the points.
	vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();

	for (auto i = 0; i < pointCoordinates.size(); ++i)
	{
		points->InsertNextPoint(pointCoordinates[i].data());
		tetra->GetPointIds()->SetId(i, i);
	}

	// Add the tetrahedron to a cell array.
	cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(tetra);

	// Add the points and tetrahedron to an unstructured grid.
	uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	uGrid->SetPoints(points);
	uGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
	
	// Visualize.
	mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(uGrid);
	
	// Add light
	light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToHeadlight();
	light->SetIntensity( 1 );

	// Add actor's color
	actor = vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	// Add the actor and light to the scene
	renderer->AddActor(actor);
	renderer->AddLight(light);
	
	// Set background's color
	renderer->SetBackground( colors->GetColor3d("Silver").GetData() );
	
	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	
	ui->qvtkWidget->GetRenderWindow()->Render();
}

// FUNCTION TO CREATE AND DISPLAY PYRAMID
void MainWindow::on_pyramid_clicked()
{	
	// Remove actor and clear points from function prior
	renderer->RemoveActor(actor);
	pointCoordinates.clear();
	
	// Set up type of model (type 1 are hexahedron, tetrahedron and pyramid)
	type = 1;
	
	// Create new color variable
	colors = vtkSmartPointer<vtkNamedColors>::New();

	// For the pyramid; setup the coordinates of five points.
	pointCoordinates.push_back({{1.0, 1.0, 1.0}});
	pointCoordinates.push_back({{-1.0, 1.0, 1.0}});
	pointCoordinates.push_back({{-1.0, -1.0, 1.0}});
	pointCoordinates.push_back({{1.0, -1.0, 1.0}});
	pointCoordinates.push_back({{0.0, 0.0, 0.0}});
	
	// Create the points.
	points = vtkSmartPointer<vtkPoints>::New();

	// Create a pyramid from the points.
	vtkSmartPointer<vtkPyramid> pyr = vtkSmartPointer<vtkPyramid>::New();
	
	for (auto i = 0; i < pointCoordinates.size(); ++i)
	{
		points->InsertNextPoint(pointCoordinates[i].data());
		pyr->GetPointIds()->SetId(i, i);
	}

	// Add the pyramid to a cell array.
	cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(pyr);

	// Add the points and hexahedron to an unstructured grid.
	uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	uGrid->SetPoints(points);
	uGrid->InsertNextCell(pyr->GetCellType(), pyr->GetPointIds());
	
	// Visualize.
	mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(uGrid);
	
	// Add light
	light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToHeadlight();
	light->SetIntensity( 1 );

	// Add actor's color
	actor = vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->SetColor( colors->GetColor3d("White").GetData() );
	actor->SetMapper(mapper);
	actor->GetProperty()->EdgeVisibilityOn();

	// Add the actor and light to the scene
	renderer->AddActor(actor);
	renderer->AddLight(light);
	
	// Set background's color
	renderer->SetBackground( colors->GetColor3d("Silver").GetData() );
	
	// Setup the renderers's camera
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}


// FUNCTION TO OPEN STL FILE FROM LOCAL FOLDER
void MainWindow::on_actionFileOpen_triggered()
{
	// Set up type of model (type 2 are STL files)
	type = 2;
	
	
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

		// Add actor and light to the scene
		renderer->AddActor(actor);
		renderer->AddLight(light);
		
		// Set background's color
		renderer->SetBackground( colors->GetColor3d("Silver").GetData() );
		
		// Setup the renderers's camera
		renderer->ResetCamera();
		renderer->GetActiveCamera()->Azimuth(30);
		renderer->GetActiveCamera()->Elevation(30);
		renderer->ResetCameraClippingRange();
		
		// Render
		ui->qvtkWidget->GetRenderWindow()->Render();
	}
}


// FUNCTION TO CHANGE MODEL'S COLOR 
void MainWindow::on_modelColor_clicked()
{
	// Get QColorDialog to choose color
	QColor color = QColorDialog::getColor(Qt::white, this);
	
	// When color is selected, set model the specified color
	if ( color.isValid() )
	{
		actor->GetProperty()->SetColor(color.redF(),color.greenF(),color.blueF());
	}
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}


// FUNCTION TO CHANGE BACKGROUND COLOR
void MainWindow::on_bgColor_clicked()
{
	// Get QColorDialog to choose color
	QColor color = QColorDialog::getColor(Qt::white, this);
	
	// When color is selected, set backgound the specified color
	if ( color.isValid() )
	{
		renderer->SetBackground(color.redF(),color.greenF(),color.blueF());
	}		
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}

// FUNCTION TO CHANGE MODEL VIEW TO top view
void MainWindow::on_modelView_clicked()
{
	// set model view to top view
	camera = vtkSmartPointer<vtkCamera>::New();
	camera->SetPosition(0, 0, 20);
	camera->SetFocalPoint(0, 0, 0);
	
	// Create a renderer, render window, and interactor
	vtkSmartPointer<vtkRenderer>::New();
	renderer->SetActiveCamera(camera);
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}

// SLIDER FUNCTION TO CHANGE LIGHT INTENSITY ON THE MODEL
void MainWindow::on_lightSlider_sliderMoved(int position)
{
	// "position" will indicate the position of pointer on slider bar
	// and calculate to give value (ratio out of 100)
	position = ui->lightSlider->value();
	light->SetIntensity((float)(position) / 100);
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}


//CLIP FUNCTION
void MainWindow::on_filterClip_clicked(bool checked)
{
	// if clip filter is checked
	if (checked)
	{	
		// Plane is created to set clip origin at (0,0,0) and cut at x-axis
		planeLeft = vtkSmartPointer<vtkPlane>::New();
		planeLeft->SetOrigin(0.0, 0.0, 0.0);
		planeLeft->SetNormal(-1.0, 0.0, 0.0);
		
		// Create clipfilter
		clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
		
		// Check if shrink filter is checked, 
		// if yes, shrink filter will also be applied
		if (ui->filterShrink->isChecked())
		{			
			clipFilter->SetInputConnection( shrinkFilter->GetOutputPort() );
		}
		
		else
		{	
			// Different type of model use different function, therefore
			// type is indicated.
			if (type==0){
				cubeSource = vtkSmartPointer<vtkCubeSource>::New();
				clipFilter->SetInputConnection( cubeSource->GetOutputPort() ) ;
			}
			else if (type==1){
				clipFilter->SetInputData( uGrid ) ;
			}
			else{
				clipFilter->SetInputConnection( reader->GetOutputPort() ) ;
			}
		}
		
		clipFilter->SetClipFunction( planeLeft.Get() );
		clipFilter->Update();
		
		mapper->SetInputConnection( clipFilter->GetOutputPort() );
	}
	
	// when clip filter is unchecked, bring model back to initial mode.
	else
	{
		if (ui->filterShrink->isChecked())
		{			
			mapper->SetInputConnection( shrinkFilter->GetOutputPort() );
		}
		else
		{
			if (type==0){
				cubeSource = vtkSmartPointer<vtkCubeSource>::New();
				mapper->SetInputConnection( cubeSource->GetOutputPort() );
			}
			else if (type==1){
				mapper->SetInputData(uGrid);
			}
			else{
				mapper->SetInputConnection( reader->GetOutputPort() );
			}
		}
	}
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
}


// SHRINK FUNCTION
void MainWindow::on_filterShrink_clicked(bool checked)
{
	if (checked)
	{
		shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
		
		if (ui->filterClip->isChecked())
		{			
			shrinkFilter->SetInputConnection( clipFilter->GetOutputPort() );
		}
		
		else
		{
			// Different type of model use different function, therefore
			// type is indicated.
			if (type==0){
				cubeSource = vtkSmartPointer<vtkCubeSource>::New();
				shrinkFilter->SetInputConnection( cubeSource->GetOutputPort() ) ;
			}
			else if (type==1){
				shrinkFilter->SetInputData( uGrid ) ;
			}
			else{
				shrinkFilter->SetInputConnection( reader->GetOutputPort() ) ;
			}
		}
		
		// Reduce shrink factor to 0.5
		shrinkFilter->SetShrinkFactor(0.5);
		shrinkFilter->Update();
		
		mapper->SetInputConnection( shrinkFilter->GetOutputPort() );
	}
	
	// when clip filter is unchecked, bring model back to initial mode.
	else
	{
		shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
		
		if (ui->filterClip->isChecked())
		{			
			shrinkFilter->SetInputConnection( clipFilter->GetOutputPort() );
		}
		
		else
		{
			if (type==0){
				cubeSource = vtkSmartPointer<vtkCubeSource>::New();
				shrinkFilter->SetInputConnection( cubeSource->GetOutputPort() ) ;
			}
			else if (type==1){
				shrinkFilter->SetInputData( uGrid ) ;
			}
			else{
				shrinkFilter->SetInputConnection( reader->GetOutputPort() ) ;
			}
		}
		
		// return filter factor to initial 1
		shrinkFilter->SetShrinkFactor(1);
		shrinkFilter->Update();
		
		mapper->SetInputConnection( shrinkFilter->GetOutputPort() );
	}
	
	// Render
	ui->qvtkWidget->GetRenderWindow()->Render();
	
}


// DESTRUCTOR
MainWindow::~MainWindow()
{
    delete ui;
}

	



















