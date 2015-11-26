#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileSystemModel"
#include <QFileSystemWatcher>
#include <iostream>
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
    watcher->addPath("../../védés/3/KR-BI_Samu/uj/amminadab/sentences");

    ui->setupUi(this);

    MainWindow::makePlot(QString::fromStdString(""));

    alanyData.remove(0);
    allitmanyData.remove(0);
    targyData.remove(0);
    labels.remove(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*QFile& initFile(const QString& path)
{
    QFile file(path);
    return file;
}*/

void MainWindow::fileChanged(const QString & path)
{
    watcher->addPath(path);
    //QMessageBox::information(0, "path", path);
    modositasok++;
    QMessageBox::information(0, "info", QString::number(modositasok));

    QFile file("../../védés/3/KR-BI_Samu/uj/amminadab/sentences");

    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0, "info", file.errorString());

    QTextStream in(&file);
    for (int i=0; i<modositasok-1; i++)
        in.readLine();
    label = in.readLine();
    QMessageBox::information(0, "info", label);

    file.close();

    QRegExp rx("(\\ |\\n)");
    query = label.split(rx);
    for (int i=0; i<query.size(); i++)
        QMessageBox::information(0, "info", query[i]);

    for (int i=0; i<3; i++)
        szohossz[i] = query[i].size();
    for (int i=0; i<3; i++)
        QMessageBox::information(0, "info", QString::number(szohossz[i]));

    makePlot(label);
}

void MainWindow::makePlot(QString label)
{
    // create empty bar chart objects:
    QCPBars *alany = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *allitmany = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *targy = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(alany);
    ui->customPlot->addPlottable(allitmany);
    ui->customPlot->addPlottable(targy);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.8);
    alany->setName("Alany");
    pen.setColor(QColor(150, 222, 0));
    alany->setPen(pen);
    alany->setBrush(QColor(150, 222, 0, 70));
    allitmany->setName("Állítmány");
    pen.setColor(QColor(1, 92, 191));
    allitmany->setPen(pen);
    allitmany->setBrush(QColor(1, 92, 191, 50));
    targy->setName("Tárgy");
    pen.setColor(QColor(255, 131, 0));
    targy->setPen(pen);
    targy->setBrush(QColor(255, 131, 0, 50));
    // stack bars ontop of each other:
    allitmany->moveAbove(alany);
    targy->moveAbove(allitmany);

    // prepare x axis with country labels:
    QVector<double> ticks;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    labels << label;
    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickLabelRotation(45);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(3, 3);
    ui->customPlot->xAxis->grid()->setVisible(false);
    ui->customPlot->xAxis->setRange(0, 8);

    // prepare y axis:
    ui->customPlot->yAxis->setRange(0, 20.1);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
    ui->customPlot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
    ui->customPlot->yAxis->grid()->setSubGridVisible(false);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    ui->customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    ui->customPlot->yAxis->grid()->setSubGridPen(gridPen);

    // Add data:
    alanyData << szohossz[0];
    allitmanyData << szohossz[1];
    targyData << szohossz[2];
    QVector<double> newticks;
    newticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    targy->setData(newticks, targyData);
    allitmany->setData(newticks, allitmanyData);
    alany->setData(newticks, alanyData);

    // setup legend:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customPlot->update();
}
