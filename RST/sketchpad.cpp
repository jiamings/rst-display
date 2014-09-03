#include "sketchpad.h"
#include "./lib/SMST/SMST.h"
#include "./lib/LMST/LMST.h"
#include "./lib/ZMST/ZMST.h"
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>
#include <fstream>
#include <cmath>
#include <iostream>

SketchPad::SketchPad(QWidget *parent) :
    QWidget(parent), xView(0), yView(0), scale(1.0),
    xFocus(0), yFocus(0), method(_Empty), rightStartPos(QPoint(0, 0)),
    leftStartPos(QPoint(0, 0))
{
    update();
}

QString s2q(const std::string &str) {
    return QString(QString::fromLocal8Bit(str.c_str()));
}

char* q2s(const QString &str) {
    return str.toLocal8Bit().data();
}

void SketchPad::read(QString path) {
    if (path.length() == 0) {
        emit setStatusBar(-3, tr(""));
    } else {
        fileName_ = q2s(path);
        points_.clear();
        lines_.clear();
        std::ifstream in(fileName_);
        int x, y;
        in >> x;
        while (in >> x >> y) {
            points_.push_back(Point(x, y));
        }
        xView = yView = xFocus = yFocus = 0;
        scale = 1.0;
        update();
        emit setStatusBar(-2, s2q(fileName_));
    }
}

void SketchPad::refresh() {
    method = _Empty;
    lines_.clear();
    xView = yView = xFocus = yFocus = 0;
    scale = 1.0;
    emit setStatusBar(0, tr("Refresh"));
    update();
}

void SketchPad::clean() {
    method = _Empty;
    lines_.clear();
    points_.clear();
    xView = yView = xFocus = yFocus = 0;
    scale = 1.0;
    emit setStatusBar(0, tr("Clean"));
    update();
}

void SketchPad::smst() {
    method = _SMST;
    SMST *Smst = new SMST();
    Smst -> setPoints(points_);
    Smst -> calculateMST();
    std::vector<Line> lines = Smst -> lines();
    lines_.clear();
    double len = 0;
    for (unsigned i = 0; i < lines.size(); i++) {
        Point a = points_[lines[i].start()];
        Point b = points_[lines[i].end()];
        lines_.push_back(DrawLine(a, b));
        len += sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
    }
    emit setStatusBar((int)len, tr("SMST"));
    update();
    delete Smst;
}

void SketchPad::lmst() {
    method = _LMST;
    LMST *Lmst = new LMST();
    Lmst -> setPoints(points_);
    Lmst -> init();
    std::vector<Line_L> lines = Lmst -> result();
    lines_.clear();
    for (unsigned i = 0; i < lines.size(); i++) {
        Point a = points_[lines[i].start()];
        Point b = points_[lines[i].end()];
        int x, y;
        if (lines[i].direction()) {
            x = b.x(); y = a.y();
        } else {
            x = a.x(); y = b.y();
        }
        lines_.push_back(DrawLine(a, Point(x, y)));
        lines_.push_back(DrawLine(b, Point(x, y)));
    }
    emit setStatusBar(Lmst->Result(), tr("LMST"));
    update();
    delete Lmst;
}

void SketchPad::zmst() {
    method = _ZMST;
    ZMST *Zmst = new ZMST();
    Zmst -> setPoints(points_);
    Zmst -> exec();
    std::vector<Line_Z> lines = Zmst -> lines();
    lines_.clear();
    for (unsigned i = 0; i < lines.size(); i++) {
        Point a = points_[lines[i].start()];
        Point b = points_[lines[i].end()];
        Point c = lines[i].mid_point();
        lines_.push_back(DrawLine(a, c));
        Point d;
        if (c.x() == a.x()) {
            d.x_set() = b.x();
            d.y_set() = c.y();
        } else if (c.y() == a.y()){
            d.x_set() = c.x();
            d.y_set() = b.y();
        }
        lines_.push_back(DrawLine(d, c));
        lines_.push_back(DrawLine(b, d));
    }
    update();
    delete Zmst;
}

void SketchPad::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // we have x, y, xview, yview, scale, xfocus, yfocus
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
    painter.fillRect(this->rect(), Qt::white);

    painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap,
                        Qt::MiterJoin));
    for (unsigned i = 0; i < lines_.size(); i++) {
        int x1 = lines_[i].start.x(), y1 = lines_[i].start.y(),
                x2 = lines_[i].end.x(), y2 = lines_[i].end.y();
        x1 = (x1 + xView - xFocus) * scale + xFocus;
        y1 = (y1 + yView - yFocus) * scale + yFocus;
        x2 = (x2 + xView - xFocus) * scale + xFocus;
        y2 = (y2 + yView - yFocus) * scale + yFocus;
        painter.drawLine(x1, y1, x2, y2);
    }
    painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap,
                        Qt::MiterJoin));
    for (unsigned i = 0; i < points_.size(); i++) {
        int x = points_[i].x(), y = points_[i].y();
        x = (x - xFocus + xView) * scale + xFocus;
        y = (y - yFocus + yView) * scale + yFocus;
        painter.drawPoint(x, y);
    }

}

Point SketchPad::calculatePaintPoint(int x, int y) {
    int xx = (x + xView - xFocus) * scale + xFocus;
    int yy = (y + yView - yFocus) * scale + yFocus;
    return Point(xx, yy);
}

Point SketchPad::calculateOriginalPoint(int x, int y) {
    int xx = (x - xFocus) / scale + xFocus - xView;
    int yy = (y - yFocus) / scale + yFocus - yView;
    return Point(xx, yy);
}

void SketchPad::addPoint(int x, int y) {
    Point a = Point(x, y);
    points_.push_back(a);
    reset();
    update();
}

void SketchPad::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightStartPos = event->pos();
        rightDragged = false;
        rightSelected = -1;
        for (unsigned i = 0; i < points_.size(); i++) {
            int x = points()[i].x();
            int y = points_[i].y();
            Point point = calculatePaintPoint(x, y);
            int xx = event->pos().x() - point.x();
            int yy = event->pos().y() - point.y();
            if (sqrt(xx * xx + yy * yy) <= 10.0) {
                rightSelected = i;
                break;
            } else if (sqrt(xx *xx + yy * yy) <= 30.0) {
                rightSelected = -2;
            }
        }

    } else if (event->button() == Qt::LeftButton) {
        leftStartPos = event->pos();
        for (unsigned i = 0; i < points_.size(); i++) {
            int x = points()[i].x();
            int y = points_[i].y();
            Point point = calculatePaintPoint(x, y);
            int xx = event->pos().x() - point.x();
            int yy = event->pos().y() - point.y();
            if (sqrt(xx * xx + yy * yy) <= 10.0) {
                leftClickOnPoint = true;
                leftStartPos = event->pos();
                leftClickPointSelected = i;
                break;
            }
        }
    }
}

void SketchPad::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::RightButton) {
        dragTransformation(rightStartPos, event->pos());
        rightStartPos = event->pos();
        rightDragged = true;
    } else if ((event->buttons() & Qt::LeftButton) && leftClickOnPoint) {
        dragPoint(leftClickPointSelected, leftStartPos, event->pos());
        leftStartPos = event->pos();
    }
}

void SketchPad::dragTransformation(QPoint start, QPoint end) {
    int x = end.x() - start.x();
    int y = end.y() - start.y();
    xView += x / scale;
    yView += y / scale;
    update();
}


void SketchPad::dragPoint(int label, QPoint start, QPoint end) {
    int x = end.x() - start.x();
    int y = end.y() - start.y();
    (points_[label]).x_set() += x / scale;
    (points_[label]).y_set() += y / scale;
    if (points_.size() < 30) {
        if (method == SketchPad::_SMST) {
            smst();
        } else if (method == SketchPad::_LMST) {
            lmst();
        } else if (method == SketchPad::_ZMST) {
            zmst();
        } else {
            refresh();
        }
        update();
    }
}

void SketchPad::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && leftClickOnPoint) {
        leftClickOnPoint = false;
        leftClickPointSelected = -1;
        reset();
    } else if (event->button() == Qt::RightButton) {
        if (!rightDragged) {
            if (rightSelected >= 0) {
                deletePoint(rightSelected);
            } else if (rightSelected == -1) {
                Point point = calculateOriginalPoint(event->pos().x(), event->pos().y());
                addPoint(point.x(), point.y());
            }
            reset();
        }
    }
    update();
}

void SketchPad::deletePoint(int x) {
    for (unsigned i = x; i < points_.size() - 1; i++) {
        points_[i] = points_[i + 1];
    }
    points_.pop_back();
    reset();
    update();
}

void SketchPad::reset() {
    if (method == SketchPad::_SMST) {
        smst();
    } else if (method == SketchPad::_LMST) {
        lmst();
    } else if (method == SketchPad::_ZMST) {
        zmst();
    } else {
        refresh();
    }
}
