#include "addpoint.h"
#include <QtGui>
#include <QRegExpValidator>
#include <QGridLayout>
#include <QPushButton>

AddPoint::AddPoint(QWidget *parent) :
    QDialog(parent)
{
    QRegExp regExp("[1-9][0-9]*");
    infoLabel = new QLabel(tr("Set the coordinates for the new point"));
    xLabel = new QLabel(tr("x: "));
    yLabel = new QLabel(tr("y: "));
    xCoordinate = new QLineEdit();
    yCoordinate = new QLineEdit();
    xCoordinate->setValidator(new QRegExpValidator(regExp, this));
    yCoordinate->setValidator(new QRegExpValidator(regExp, this));
    confirmButton = new QPushButton(tr("Ok"));
    cancelButton = new QPushButton(tr("Cancel"));

    connect(confirmButton, SIGNAL(clicked()), this, SLOT(closeOk()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeNotOk()));

    createLayout();

}

void AddPoint::closeOk() {
    emit confirmed(true, xCoordinate->text().toInt(), yCoordinate->text().toInt());
    this->close();
}

void AddPoint::closeNotOk() {
    this->close();
}

void AddPoint::createLayout() {
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(6);
    layout->setSpacing(6);
    layout->addWidget(xLabel, 0, 0);
    layout->addWidget(yLabel, 1, 0);
    layout->addWidget(xCoordinate, 0, 1);
    layout->addWidget(yCoordinate, 1, 1);
    layout->addWidget(confirmButton, 2, 0);
    layout->addWidget(cancelButton, 2, 1);
}
