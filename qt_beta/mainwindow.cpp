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
    watcher->addPath("/home/pistiz/Dokumentumok/prog2/qt/samu/samu.txt");

    ui->setupUi(this);

    MainWindow::makePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileChanged(const QString & path)
{
    watcher->addPath(path);
    QMessageBox::information(0, "path", path);
    MainWindow::modositasok++;
    QMessageBox::information(0, "info", QString::number(MainWindow::modositasok));

    QFile file("/home/pistiz/Dokumentumok/prog2/qt/samu/samu.txt");

    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0, "info", file.errorString());

    QTextStream in(&file);
    for (int i=0; i<MainWindow::modositasok-1; i++)
        in.readLine();
    QString szoveg = in.readLine();
    QMessageBox::information(0, "info", szoveg);

    file.close();
}

void MainWindow::makePlot()
{
    /*
    QRegExp rx("(\\ |\\n)");
    QStringList query = szoveg.split(rx);
    for (int i=0; i<query.size(); i++)
        QMessageBox::information(0, "info", query[i]);

    int szohossz[3];
    for (int i=0; i<3; i++)
        szohossz[i] = query[i].size();
    for (int i=0; i<3; i++)
        QMessageBox::information(0, "info", QString::number(szohossz[i]));*/

    // create empty bar chart objects:
    QCPBars *regen = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *nuclear = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *fossil = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(regen);
    ui->customPlot->addPlottable(nuclear);
    ui->customPlot->addPlottable(fossil);
    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    fossil->setName("Fossil fuels");
    pen.setColor(QColor(255, 131, 0));
    fossil->setPen(pen);
    fossil->setBrush(QColor(255, 131, 0, 50));
    nuclear->setName("Nuclear");
    pen.setColor(QColor(1, 92, 191));
    nuclear->setPen(pen);
    nuclear->setBrush(QColor(1, 92, 191, 50));
    regen->setName("Regenerative");
    pen.setColor(QColor(150, 222, 0));
    regen->setPen(pen);
    regen->setBrush(QColor(150, 222, 0, 70));
    // stack bars ontop of each other:
    nuclear->moveAbove(fossil);
    regen->moveAbove(nuclear);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    labels << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";
    ui->customPlot->xAxis->setAutoTicks(false);
    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVector(ticks);
    ui->customPlot->xAxis->setTickVectorLabels(labels);
    ui->customPlot->xAxis->setTickLabelRotation(60);
    ui->customPlot->xAxis->setSubTickCount(0);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->setRange(0, 8);

    // prepare y axis:
    ui->customPlot->yAxis->setRange(0, 12.1);
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
    QVector<double> fossilData, nuclearData, regenData;
    fossilData  << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
    nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
    regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    regen->setData(ticks, regenData);

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
}
