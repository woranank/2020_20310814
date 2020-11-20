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

    // Connect the released() signal of the addButton object to the handleAddButton slot in this object
    connect( ui->addButton, &QPushButton::released, this, &MainWindow::handleAddButton );
    connect( ui->editButton, &QPushButton::released, this, &MainWindow::handleEditButton );
    connect( ui->removeButton, &QPushButton::released, this, &MainWindow::handleRemoveButton );

    connect( this, &MainWindow::statusUpdateMessage, ui->statusBar, &QStatusBar::showMessage );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleAddButton() {
    QModelIndexList selectedList;

    selectedList = ui->listView->selectionModel()->selectedIndexes();

    StockItem item;

    if( selectedList.length() == 1 ) {
        stockList.insertItem( item, selectedList[0]);
    }
    else{
        stockList.addItem(item);
    }

    emit statusUpdateMessage( QString("Add button was clicked"), 0 );
}


void MainWindow::handleEditButton() {
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

void MainWindow::handleRemoveButton() {

    emit statusUpdateMessage( QString("Remove button was clicked"), 0 );
    QModelIndexList selectedList;

    selectedList = ui->listView->selectionModel()->selectedIndexes();

    if(selectedList.length() == 1){
        stockList.removeItem(selectedList[0]);
    } 

}

void MainWindow::on_action_Save_triggered(){

    emit statusUpdateMessage( QString("Text file saved"), 0 );


    //savefiledialog file name
    QString fileName = QFileDialog::getSaveFileName(this, "Save file", "C://", "Text files (*.txt)");

    //create file
    if (fileName.isEmpty())
            return;
    else {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
            }

        //file content
        QTextStream out(&file);
        out << "---Stocklist---" << "\n";
        for (int i=0; i<stockList.rowCount(); i++){
            out << stockList.getItemName(i) << "\n";
        }
    }
}


