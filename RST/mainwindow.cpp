#include "sketchpad.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sketchpad(new SketchPad())
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    setCentralWidget(sketchpad);
    createActions();
    createMenus();
    createToolBars();
    showMaximized();
    createStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions() {
    readAction = new QAction(tr("&Read"), this);
    readAction -> setShortcut(tr("Ctrl+R"));
    refreshAction = new QAction(tr("&Refresh"), this);
    refreshAction -> setShortcut(tr("Ctrl+E"));
    cleanAction = new QAction(tr("&Clean"), this);
    cleanAction->setShortcut(tr("Ctrl+C"));
    smstAction = new QAction(tr("&SMST"), this);
    smstAction -> setShortcut(tr("Ctrl+S"));
    lmstAction = new QAction(tr("&LMST"), this);
    lmstAction -> setShortcut(tr("Ctrl+L"));
    zmstAction = new QAction(tr("&ZMST"), this);
    zmstAction -> setShortcut(tr("Ctrl+Z"));
    addPointAction = new QAction(tr("&Add Point"), this);
    addPointAction->setShortcut(tr("Ctrl+N"));
    zoomInAction = new QAction(tr("&Zoom In"), this);
    zoomInAction->setShortcut(tr("Ctrl+>"));
    zoomOutAction = new QAction(tr("Zoom Out"), this);
    zoomOutAction->setShortcut(tr("Ctrl+<"));
    helpAction = new QAction(tr("Help"), this);
    helpAction->setShortcut(tr("Ctrl+H"));
    connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(smstAction, SIGNAL(triggered()), this, SLOT(smst()));
    connect(lmstAction, SIGNAL(triggered()), this, SLOT(lmst()));
    connect(zmstAction, SIGNAL(triggered()), this, SLOT(zmst()));
    connect(addPointAction, SIGNAL(triggered()), this, SLOT(addPoint()));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));
    connect(cleanAction, SIGNAL(triggered()), this, SLOT(clean()));
}

void MainWindow::createMenus() {
    fileMenu = menuBar() -> addMenu(tr("&File"));
    fileMenu -> addAction(readAction);
    fileMenu -> addAction(refreshAction);
    fileMenu->addAction(cleanAction);
    algoMenu = menuBar() -> addMenu(tr("&Algorithms"));
    algoMenu -> addAction(smstAction);
    algoMenu -> addAction(lmstAction);
    algoMenu -> addAction(zmstAction);
    pointMenu = menuBar()->addMenu(tr("&Points"));
    pointMenu->addAction(addPointAction);
    pointMenu->addAction(zoomInAction);
    pointMenu->addAction(zoomOutAction);
}

void MainWindow::createToolBars() {
    helpToolBar = addToolBar(tr("&Help"));
    helpToolBar->addAction(helpAction);
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(readAction);
    fileToolBar -> addAction(refreshAction);
    fileToolBar->addAction(cleanAction);
    algoToolBar = addToolBar(tr("&Algorithms"));
    algoToolBar -> addAction(smstAction);
    algoToolBar -> addAction(lmstAction);
    algoToolBar -> addAction(zmstAction);
    pointToolBar = addToolBar(tr("&Points"));
    pointToolBar->addAction(addPointAction);
    pointToolBar->addAction(zoomInAction);
    pointToolBar->addAction(zoomOutAction);
}

void MainWindow::help() {
    QMessageBox::about(this, tr("Help"),
                       tr("<h2>Help</h2>"
                          "<p>Copyright 2013 Tony Soong.</p>"
                          "<p>Read(Ctrl+R): get point data from a file.</p>"
                          "<p>Refresh(Ctrl+E): clean all the lines and only include the lines</p>"
                          "<p>SMST(Ctrl+S), LMST(Ctrl+L), ZMST(Ctrl+Z): three different type of RST implementations</p>"
                          "<p>Zoom In(Ctrl+>) and Zoom Out(Ctrl+<) can also be done by the scroll wheel.</p>"
                          "<p>Moving can be done by Arrow Keys and Right Click & Drag</p>"
                          "<p>Add points by pressing Ctrl+N</p>"
                          "<p>Delete points by right click on the point</p>"
                          ));
}

void MainWindow::smst() {
    sketchpad -> smst();
}

void MainWindow::lmst() {
    sketchpad -> lmst();
}

void MainWindow::zmst() {
    sketchpad -> zmst();
}

void MainWindow::read() {
    QString path = QFileDialog::getOpenFileName(NULL, tr("Open File"), ".", tr("Text Files(*.txt)"));
    sketchpad -> read(path);
}

void MainWindow::refresh() {
    sketchpad -> refresh();
}

void MainWindow::clean() {
    sketchpad->clean();
}

void MainWindow::createStatusBar() {
    statusLabel = new QLabel(" Welcome to RST Display - created by Jiaming Song ");
    statusLabel -> setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(statusLabel);
    connect(sketchpad, SIGNAL(setStatusBar(int, QString)), this, SLOT(updateStatusBar(int, QString)));
    updateStatusBar(-1, tr(""));
}

void MainWindow::updateStatusBar(int x, QString method) {
    if(x == -1) {
        statusLabel->setText(" Welcome to RST Display - created by Jiaming Song ");
        return;
    }
    if(x == -2) {
        statusLabel->setText(tr("Read file from ") + method);
        return;
    }
    if(x == -3) {
        statusLabel->setText(tr("Did not read new file."));
        return;
    }
    QString msg = tr("Method = ") + method + tr("  ---  Result = ") + QString::number(x);
    statusLabel->setText(msg);
}

void MainWindow::addPoint() {
    AddPoint *ask = new AddPoint();
    connect(ask, SIGNAL(confirmed(bool, int, int)), this, SLOT(setAddPoint(bool, int, int)));
    ask->show();
}

void MainWindow::setAddPoint(bool ok, int x, int y) {
    if (!ok) return;
    sketchpad -> addPoint(x, y);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) {
        std::cout << "up" << std::endl;
        sketchpad->yView_set() -= 100;
    } else if (event->key() == Qt::Key_Down) {
        sketchpad->yView_set() += 100;
    } else if (event->key() == Qt::Key_Left) {
        sketchpad->xView_set() -= 100;
    } else if (event->key() == Qt::Key_Right) {
        sketchpad->xView_set() += 100;
    }
    update();
}

void MainWindow::zoomIn() {
    sketchpad->scale_set() /= 0.9;
    update();
}

void MainWindow::zoomOut() {
    sketchpad->scale_set() *= 0.9;
    update();
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    int x = event->x(), y = event->y();
    sketchpad->xFocus_set() = x;
    sketchpad->yFocus_set() = y;
    int steps = event->delta() / 120;
    sketchpad->scale_set() *= pow(0.9, -steps);
    update();
}
