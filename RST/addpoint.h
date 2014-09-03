#ifndef ADDPOINT_H
#define ADDPOINT_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class AddPoint : public QDialog
{
    Q_OBJECT
public:
    explicit AddPoint(QWidget *parent = 0);
    
signals:
    void confirmed(bool, int, int);
public slots:
    void closeOk();
    void closeNotOk();
private:
    QLabel* infoLabel;
    QLabel* xLabel;
    QLabel* yLabel;
    QLineEdit* xCoordinate;
    QLineEdit* yCoordinate;
    QPushButton* confirmButton;
    QPushButton* cancelButton;
    void createLayout();
};

#endif // ADDPOINT_H
