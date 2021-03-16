#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);

    //Action Open File
    connect( ui->actionFileOpen, &QAction::triggered, this, &MainWindow::on_actionFileOpen_triggered );

    //Checkbox functions
    connect( ui->checkBoxClip, &QCheckBox::clicked, this, &MainWindow::handleClipCheckbox );
    connect( ui->checkBoxShrink, &QCheckBox::clicked, this, &MainWindow::handleShrinkCheckbox );

    //Pushbutton functions
    connect( ui->pushButtonBGColor, &QPushButton::released, this, &MainWindow::handleBGColorButton );
    connect( ui->pushButtonModelColor, &QPushButton::released, this, &MainWindow::handleModelColorButton );
    connect( ui->pushButtonModelShape, &QPushButton::released, this, &MainWindow::handleModelShapeButton );
    connect( ui->pushButtonView, &QPushButton::released, this, &MainWindow::handleViewButton );

    //Slider function
    connect( ui->sliderIntensity, &QSlider::valueChanged, this, &MainWindow::handleIntensitySlider );

    //Pushbutton shapes
    connect( ui->pushButtonHexa, &QPushButton::released, this, &MainWindow::handleHexaButton );
    connect( ui->pushButtonTetra, &QPushButton::released, this, &MainWindow::handleTetraButton );
    connect( ui->pushButtonPyra, &QPushButton::released, this, &MainWindow::handlePyraButton );
	

	
    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    ui->qvtkWidget->SetRenderWindow( renderWindow );			// note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createHexahedron(){
	
	// For the hexahedron; setup the coordinates of eight points.
	// The two faces must be in counter clockwise order as viewed from the outside.
	this->pointCoordinates.push_back({{0.0, 0.0, 0.0}}); // Face 1
	this->pointCoordinates.push_back({{1.0, 0.0, 0.0}});
	this->pointCoordinates.push_back({{1.0, 1.0, 0.0}});
	this->pointCoordinates.push_back({{0.0, 1.0, 0.0}});
	this->pointCoordinates.push_back({{0.0, 0.0, 1.0}}); // Face 2
	this->pointCoordinates.push_back({{1.0, 0.0, 1.0}});
	this->pointCoordinates.push_back({{1.0, 1.0, 1.0}});
	this->pointCoordinates.push_back({{0.0, 1.0, 1.0}});

	// Create a hexahedron from the points.
	vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();

	for (auto i = 0; i < this->pointCoordinates.size(); ++i)
	{
		this->points->InsertNextPoint(pointCoordinates[i].data());
		hex->GetPointIds()->SetId(i, i);
	}

	this->cells->InsertNextCell(hex);
	this->uGrid->SetPoints(this->points);
	this->uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

}

void MainWindow::createTetrahedron(){
	
	this->pointCoordinates.push_back({{0.0, 0.0, 0.0}});
	this->pointCoordinates.push_back({{1.0, 0.0, 0.0}});
	this->pointCoordinates.push_back({{1.0, 1.0, 0.0}});
	this->pointCoordinates.push_back({{0.0, 1.0, 1.0}});
	
	// Create a tetrahedron from the points.
	vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();

	for (auto i = 0; i < this->pointCoordinates.size(); ++i)
	{
		this->points->InsertNextPoint(pointCoordinates[i].data());
		tetra->GetPointIds()->SetId(i, i);
	}

	this->cells->InsertNextCell(tetra);
	this->uGrid->SetPoints(this->points);
	this->uGrid->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());

}

void MainWindow::createPyramid(){
	
	this->pointCoordinates.push_back({{1.0, 1.0, 1.0}}); 
	this->pointCoordinates.push_back({{-1.0, 1.0, 1.0}});
	this->pointCoordinates.push_back({{-1.0, -1.0, 1.0}});
	this->pointCoordinates.push_back({{1.0, -1.0, 1.0}});
	this->pointCoordinates.push_back({{0.0, 0.0, 0.0}});
	
	// Create a pyramid from the points.
	vtkSmartPointer<vtkPyramid> pyra = vtkSmartPointer<vtkPyramid>::New();

	for (auto i = 0; i < this->pointCoordinates.size(); ++i)
	{
		this->points->InsertNextPoint(pointCoordinates[i].data());
		pyra->GetPointIds()->SetId(i, i);
	}

	this->cells->InsertNextCell(pyra);
	this->uGrid->SetPoints(this->points);
	this->uGrid->InsertNextCell(pyra->GetCellType(), pyra->GetPointIds());

}

void MainWindow::shrinkFilterFunc(){
	shrinkFilter->SetShrinkFactor(0.2);
	shrinkFilter->Update();
}

void MainWindow::clipFilterFunc(){
	vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
	planeLeft->SetOrigin(0.0, 0.0, 0.0);
	planeLeft->SetNormal(-1.0, 0.0, 0.0);
	clipFilter->SetClipFunction(planeLeft.Get());
}

void MainWindow::refreshRender(){
}


//Open file from directory
void MainWindow::on_actionFileOpen_triggered(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open STL File"), "./", tr("STL Files(*.stl)"));


        if (fileName.isEmpty()){
        // if file name in previous step does not exist, return
            return;
        }

        else {

    // Visualize

    //Reader
    vtkSmartPointer<vtkSTLReader> reader =
      vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(fileName.toLatin1().data());
    reader->Update();

    //Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    //Actor
    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //Renderer
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

    /*
    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    */

    renderer->AddActor(actor);
    renderer->SetBackground(.3, .6, .3); // Background color green

    //renderWindow->Render();
    //renderWindowInteractor->Start();

        }

}

void MainWindow::handleClipCheckbox(int){
}

void MainWindow::handleShrinkCheckbox(int){	
}

void MainWindow::handleBGColorButton(){
	
	renderer->SetBackground( colors->GetColor3d("Red").GetData() );	
}

void MainWindow::handleModelColorButton(){	
}

void MainWindow::handleModelShapeButton(){	
}

void MainWindow::handleViewButton(){
}

void MainWindow::handleIntensitySlider(int){	
}

void MainWindow::handleHexaButton(){	

	this->pointCoordinates.clear();
	this->points->Reset();
	this->points->Squeeze();
	this->cells->Reset();
	this->cells->Squeeze();
	this->uGrid->Reset();
	this->uGrid->Squeeze();
	
	createHexahedron();
	
	this->objectType = 2;
	
	if (ui->clipCheck->isChecked()||ui->shrinkCheck->isChecked()){
		if(ui->clipCheck->isChecked()){
			this->clipFilter->SetInputData(this->uGrid);
			this->clipFilterFunc();
		}
		if(ui->shrinkCheck->isChecked()){
			if(ui->clipCheck->isChecked()){
				this->shrinkFilter->SetInputConnection(this->clipFilter->GetOutputPort());
			}
			else{
				this->shrinkFilter->SetInputData(this->uGrid);
			}
			this->initShrinkFilter();
			this->activeMapper->SetInputConnection(this->shrinkFilter->GetOutputPort());
		}
		else{
			this->activeMapper->SetInputConnection(this->clipFilter->GetOutputPort());
		}
	}
	else{
		this->activeMapper->SetInputData(this->uGrid);
	}
	this->refreshRender();
	
}

void MainWindow::handleTetraButton(){	

	this->pointCoordinates.clear();
	this->points->Reset();
	this->points->Squeeze();
	this->cells->Reset();
	this->cells->Squeeze();
	this->uGrid->Reset();
	this->uGrid->Squeeze();
	
	createTetrahedron();
	
	this->objectType = 2;
	
	if (ui->clipCheck->isChecked()||ui->shrinkCheck->isChecked()){
		if(ui->clipCheck->isChecked()){
			this->clipFilter->SetInputData(this->uGrid);
			this->clipFilterFunc();
		}
		if(ui->shrinkCheck->isChecked()){
			if(ui->clipCheck->isChecked()){
				this->shrinkFilter->SetInputConnection(this->clipFilter->GetOutputPort());
			}
			else{
				this->shrinkFilter->SetInputData(this->uGrid);
			}
			this->initShrinkFilter();
			this->activeMapper->SetInputConnection(this->shrinkFilter->GetOutputPort());
		}
		else{
			this->activeMapper->SetInputConnection(this->clipFilter->GetOutputPort());
		}
	}
	else{
		this->activeMapper->SetInputData(this->uGrid);
	}
	this->refreshRender();
	
}

void MainWindow::handlePyraButton(){
	
	this->pointCoordinates.clear();
	this->points->Reset();
	this->points->Squeeze();
	this->cells->Reset();
	this->cells->Squeeze();
	this->uGrid->Reset();
	this->uGrid->Squeeze();
	
	createPyramid();
	
	this->objectType = 2;
	
	if (ui->clipCheck->isChecked()||ui->shrinkCheck->isChecked()){
		if(ui->clipCheck->isChecked()){
			this->clipFilter->SetInputData(this->uGrid);
			this->clipFilterFunc();
		}
		if(ui->shrinkCheck->isChecked()){
			if(ui->clipCheck->isChecked()){
				this->shrinkFilter->SetInputConnection(this->clipFilter->GetOutputPort());
			}
			else{
				this->shrinkFilter->SetInputData(this->uGrid);
			}
			this->initShrinkFilter();
			this->activeMapper->SetInputConnection(this->shrinkFilter->GetOutputPort());
		}
		else{
			this->activeMapper->SetInputConnection(this->clipFilter->GetOutputPort());
		}
	}
	else{
		this->activeMapper->SetInputData(this->uGrid);
	}
	this->refreshRender();
	
}





























