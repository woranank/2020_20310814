#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "stockitemlistmodel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    StockItemListModel stockList;

public slots:
    void handleAddButton();
    void handleEditButton();
    void handleRemoveButton();
    void on_action_Save_triggered();

signals:
    void statusUpdateMessage( const QString & message, int timeout );

//private slots:
    //void on_action_Save_triggered();
};




#endif // MAINWINDOW_H
