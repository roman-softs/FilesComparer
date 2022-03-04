#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "files_comparer_widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(new FilesComparerWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

