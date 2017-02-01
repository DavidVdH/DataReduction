#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <qcustomplot.h>
#include <vector>
#include <QVector>


using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    // GUI related widgets
    QPushButton *filePushButton,*backPushButton, *forwardPushButton;
    QLineEdit *fileLineEdit;
    QSpinBox *currentSpinBox, *fromSpinBox, *toSpinBox;
    QLabel *navigationLabel;
    QFormLayout *setupFormLayout, *navigationFormLayout;
    QHBoxLayout *navigationHBoxLayout;
    QVBoxLayout *navigationVBoxLayout;
    QGroupBox *setupGroupBox, *navigationGroupBox;
    QCustomPlot *dataCustomPlot;

    // Containers for data storage
    const QVector<double> wavelength = QVector<double>() << 1 << 2 << 3 << 4 << 5 ;
    vector< QVector<double> > rawData;



public slots:
    void browseFiles();
};

#endif // MAINWINDOW_H
