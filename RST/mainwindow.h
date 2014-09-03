#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QLabel>
#include "sketchpad.h"
#include "addpoint.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void read();
    void refresh();
    void clean();
    void lmst();
    void smst();
    void zmst();
    void updateStatusBar(int, QString);
    void setAddPoint(bool, int, int);
    void addPoint();
    void zoomIn();
    void zoomOut();
    void help();

protected:
    void keyPressEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *);

private:
    Ui::MainWindow *ui;
    SketchPad *sketchpad;
    QAction *readAction;
    QAction *refreshAction;
    QAction *cleanAction;
    QAction *lmstAction;
    QAction *smstAction;
    QAction *zmstAction;
    QAction *addPointAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *helpAction;
    QMenu *fileMenu;
    QMenu *algoMenu;
    QMenu *pointMenu;
    QToolBar *helpToolBar;
    QToolBar *fileToolBar;
    QToolBar *algoToolBar;
    QToolBar *pointToolBar;
    QString filename;
    QLabel *statusLabel;
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

};

#endif // MAINWINDOW_H
