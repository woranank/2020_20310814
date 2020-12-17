#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QAbstractItemModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stockitem.h"
#include "edititemdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Link the ListModel to the ListView
    ui->listView->setModel( &stockList );
    // Tell this list view to only accept single selections
    ui->listView->setSelectionBehavior( QAbstractItemView::SelectItems );

    // Connect the released() signal of the addButton, editButton, removeButton objects to the handle(Add/Edit/Remove)Button slot in this object
    connect( ui->addButton, &QPushButton::released, this, &MainWindow::handleAddButton );
    connect( ui->editButton, &QPushButton::released, this, &MainWindow::handleEditButton );
    connect( ui->removeButton, &QPushButton::released, this, &MainWindow::handleRemoveButton );

	// Connect to show status message
    connect( this, &MainWindow::statusUpdateMessage, ui->statusBar, &QStatusBar::showMessage );
}

MainWindow::~MainWindow() {
    delete ui;
}

// Add function
void MainWindow::handleAddButton() {
    QModelIndexList selectedList;

    selectedList = ui->listView->selectionModel()->selectedIndexes();

    StockItem item;

    if( selectedList.length() == 1 ) {
		// If list already existed, insert item after the selected item 
        stockList.insertItem( item, selectedList[0]);
    }
    else{
		// If list does not exist, add a new item
        stockList.addItem(item);
    }
	
	// Display latest action, add button was clicked
    emit statusUpdateMessage( QString("Add button was clicked"), 0 );
}

// Edit function
void MainWindow::handleEditButton() {
	
	// When edit button is clicked, an edit window will appear
    EditItemDialog dialog( this );
	
    QModelIndexList selectedList;

    selectedList = ui->listView->selectionModel()->selectedIndexes();

    if( selectedList.length() == 1 ) {
        // selectedList is a list of all selected items in the listView. Since we set its
        // behaviour to single selection, were only interested in the first selecteded item.
        StockItem item = stockList.getItem( selectedList[0] );

        if( dialog.runDialog( item ) ) {
            // user clicked ok, need to update item in list...
            stockList.setItem( item, selectedList[0] );
        }
    } else {
        emit statusUpdateMessage( QString("No item selected to edit!"), 0 );
    }
}

// Remove function
void MainWindow::handleRemoveButton() {
	
	// display latest action (remove)
    emit statusUpdateMessage( QString("Remove button was clicked"), 0 );
	
    QModelIndexList selectedList;

    selectedList = ui->listView->selectionModel()->selectedIndexes();

    if(selectedList.length() == 1){
		// if selected item exists, the item may be deleted
        stockList.removeItem(selectedList[0]);
    } 

}


// save as text file function
void MainWindow::on_action_Save_triggered(){

	// display latest action(save file)
    emit statusUpdateMessage( QString("Text file saved"), 0 );

    // savefiledialog file name
	// C:// indicates initial location when dialog pop up
	// Save as text files, .txt
    QString fileName = QFileDialog::getSaveFileName(this, "Save file", "C://", "Text files (*.txt)");

    // create file
    if (fileName.isEmpty())
			// if file name in previous step does not exist, return
            return;
    else {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);

        // file content
        QTextStream out(&file);
		
		// text file initial title
        out << "---Stocklist---" << "\n";
		
		// print out each item name in the stocklist
        for (int i=0; i<stockList.rowCount(); i++){
            out << stockList.getItemName(i) << "\n";
        }

        file.close();
    }
}


