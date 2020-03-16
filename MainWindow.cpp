#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Widgets/Background.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _uiBackground = new Background(this);
    this->setCentralWidget(_uiBackground);
}

MainWindow::~MainWindow()
{
    delete ui;
}

