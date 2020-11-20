#include <iostream>
#include <fstream>

std::ofstream outfile("");

outfile << stockItems << std::endl;

outfile.close();

void StockItemListModel::addItem( const StockItem & s ) {
    // This emits a signal to warn the listView that extra rows will be added
    emit beginInsertRows( QModelIndex(), stockItems.size()-1, stockItems.size()-1 );
    // Add the extra item to the list
    stockItems.push_back( s );
    // Emits a signal to say rows have been added.
    emit endInsertRows();
}

void savefiledialog::on_action_Save_triggered(){

    //create file dialog
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "C://", tr("Text files (*.txt)"));
