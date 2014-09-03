#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QPaintEvent>
#include <string>
#include <vector>
#include <./lib/Headers/data_format.h>

class SketchPad : public QWidget
{
    Q_OBJECT
public:
    enum Methods{
        _Empty = 0,
        _SMST = 1,
        _LMST = 2,
        _ZMST = 3
    };
    explicit SketchPad(QWidget *parent = 0);
    void read(QString);
    void refresh();
    void clean();
    void smst();
    void lmst();
    void zmst();
    void addPoint(int, int);
    Point calculatePaintPoint(int, int);
    Point calculateOriginalPoint(int, int);
    std::vector<Point>& points() { return points_; }
    std::vector<DrawLine>& lines() { return lines_; }
    double& scale_set() { return scale; }
    //double& prescale_set() { return prescale; }
    int& xView_set() { return xView; }
    int& yView_set() { return yView; }
    int& xFocus_set() { return xFocus; }
    int& yFocus_set() { return yFocus; }
    Methods& method_set() { return method; }

signals:
    void setStatusBar(int, QString);
public slots:
    
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    char* fileName_;
    std::vector<Point> points_;
    std::vector<DrawLine> lines_;
    double scale;
    //double prescale;
    int xFocus;
    int yFocus;
    int xView;
    int yView;
    Methods method;
    void dragPoint(int, QPoint, QPoint);
    void dragTransformation(QPoint, QPoint);
    QPoint leftStartPos;
    bool leftClickOnPoint;
    bool rightDragged;
    int rightSelected;
    int leftClickPointSelected;
    QPoint rightStartPos;
    void deletePoint(int);
    void reset();
};

#endif // SKETCHPAD_H
