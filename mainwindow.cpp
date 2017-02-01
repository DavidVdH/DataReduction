#include "mainwindow.h"
#include <QPixmap>
#include <QIcon>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h> // atof


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    // Size of the main window ==> all other sizes relative to these
    int width = 1400;
    int height = 600;

    setFixedSize(width, height);

    qDebug() << wavelength;

    // "Setup" layout and groupbox

    filePushButton = new QPushButton("Browse");
    fileLineEdit = new QLineEdit();
    fileLineEdit->setReadOnly(true);

    setupFormLayout = new QFormLayout();
    setupFormLayout->addRow("Data file:", fileLineEdit);
    setupFormLayout->addRow(filePushButton);

    setupGroupBox = new QGroupBox("Setup", this);
    setupGroupBox->setLayout(setupFormLayout);
    setupGroupBox->setGeometry(20,20,250,110);



    // "Navigation" layout and groupbox

    backPushButton = new QPushButton();
    forwardPushButton = new QPushButton();

    backPushButton->setIcon(QIcon(":/images/back.png"));
    forwardPushButton->setIcon(QIcon(":/images/forward.png"));

    currentSpinBox = new QSpinBox();
    currentSpinBox->setMinimum(0);

    navigationHBoxLayout = new QHBoxLayout();
    navigationHBoxLayout->addWidget(backPushButton);
    navigationHBoxLayout->addWidget(currentSpinBox);
    navigationHBoxLayout->addWidget(forwardPushButton);

    navigationLabel = new QLabel("Select the spectra you want to use during the datareduction:");
    navigationLabel->setWordWrap(true);

    fromSpinBox = new QSpinBox(); // !!!!!!! DEFAULT VALUES HAVE TO BE SET TO FIRST AND LAST WHEN FILE IS READ IN
    toSpinBox = new QSpinBox(); // !!!!!!! DEFAULT VALUES HAVE TO BE SET TO FIRST AND LAST WHEN FILE IS READ IN

    fromSpinBox->setMinimum(0);
    toSpinBox->setMinimum(0);

    navigationFormLayout = new QFormLayout();
    navigationFormLayout->addRow("From: ", fromSpinBox);
    navigationFormLayout->addRow("To: ", toSpinBox);

    navigationVBoxLayout = new QVBoxLayout();
    navigationVBoxLayout->addLayout(navigationHBoxLayout);
    navigationVBoxLayout->addWidget(navigationLabel);
    navigationVBoxLayout->addLayout(navigationFormLayout);

    navigationGroupBox = new QGroupBox("Navigation", this);
    navigationGroupBox->setLayout(navigationVBoxLayout);

    navigationGroupBox->setGeometry(20,140,250,200);




    // Setup data display environment

    dataCustomPlot = new QCustomPlot(this);
    dataCustomPlot->setGeometry(40+250,20,1090,560);
    dataCustomPlot->xAxis->setLabel("Wavelength (nm)");
    dataCustomPlot->yAxis->setLabel("I (arb.units)");
    dataCustomPlot->addGraph();




    // Connecting signals and slots

    QObject::connect(filePushButton, SIGNAL(clicked()), SLOT(browseFiles()));

}



void MainWindow::browseFiles()
{
    // When filePushButton is clicked this function is executed and prompts
    // user to select data file which is than stored in fileLineEdit.
    // Data is stored in vectors and range for datareduction is automatically
    // set to maximum range.

    fileLineEdit->setText(QFileDialog::getOpenFileName());

    string fileName = fileLineEdit->text().toStdString();
    ifstream f(fileName.c_str());
    string line;

    for (int i = 0; getline(f, line); ++i){

        QVector<double> spectrum;
        istringstream iss(line);

        do{
                std::string sub;
                iss >> sub;
                double tmp = atof(sub.c_str());
                spectrum.push_back(tmp);


        } while (iss);
        qDebug() << spectrum;
        rawData.push_back(spectrum);
    };


    fromSpinBox->setValue(0);
    toSpinBox->setValue(rawData.size());
    currentSpinBox->setMaximum(rawData.size());

    dataCustomPlot->graph(0)->setData(wavelength,rawData[0]);
    dataCustomPlot->rescaleAxes();
    dataCustomPlot->replot();


}


