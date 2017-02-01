#include "mainwindow.h"
#include <QPixmap>
#include <QIcon>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h> // atof
#include <unistd.h> // usleep


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    // Size of the main window ==> all other sizes relative to these
    int width = 1400;
    int height = 600;

    setFixedSize(width, height);

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

    backPushButton->setCheckable(true);
    forwardPushButton->setCheckable(true);

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

    fromSpinBox = new QSpinBox();
    toSpinBox = new QSpinBox();

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
    QObject::connect(currentSpinBox, SIGNAL(valueChanged(int)), SLOT(updateGraph()));
    QObject::connect(backPushButton, SIGNAL(clicked()), SLOT(playGraph()));
    QObject::connect(forwardPushButton, SIGNAL(clicked()), SLOT(playGraph()));

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

        rawData.push_back(spectrum);
    };


    fromSpinBox->setValue(0);
    fromSpinBox->setMaximum(rawData.size()-1);

    toSpinBox->setValue(rawData.size()-1);
    toSpinBox->setMaximum(rawData.size()-1);

    currentSpinBox->setMaximum(rawData.size()-1);


    dataCustomPlot->graph(0)->setData(wavelength,rawData[0]);
    dataCustomPlot->rescaleAxes();
    dataCustomPlot->replot();

}




void MainWindow::updateGraph()
{
    // Updates graph so that the data displayed corresponds to the data selected
    // in the currentSpinBox.

    int index = currentSpinBox->value();
    dataCustomPlot->graph(0)->setData(wavelength,rawData[index]);
    dataCustomPlot->rescaleAxes();
    dataCustomPlot->replot();
}




void MainWindow::playGraph()
{
    // Display all spectra as a video. Play forward for direction +1 and
    // backward for direction -1. Only play as long as button is clicked.
    int direction;
    if(backPushButton->isChecked()){
        direction = -1;
    }else{
        if(forwardPushButton->isChecked()){
            direction = 1;
       }else{
            direction = 0;
        }
    }



    if(direction == 0){
        // do nothing
    }else{
        if(direction == 1){
           while(currentSpinBox->value() < rawData.size()-1){
                updateGraph();
                int currentValue = currentSpinBox->value() + 1;
                currentSpinBox->setValue(currentValue);
                usleep(50000);
            }
        }else{
            while(currentSpinBox->value() > 1){
                updateGraph();
                int currentValue = currentSpinBox->value() - 1;
                currentSpinBox->setValue(currentValue);
                usleep(50000);
            }
        }
    }

    backPushButton->setChecked(false);
    forwardPushButton->setChecked(false);

}
