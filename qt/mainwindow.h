#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include "QFileSystemModel"
#include "/home/pistiz/Letöltések/qcustomplot/qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void makePlot(QString label);
    void fileChanged(const QString& path);
private:
    Ui::MainWindow *ui;
    QFileSystemWatcher * watcher;
    int modositasok = 0;
    QStringList query;
    int szohossz[3] = {0};
    QString label;
    QVector<QString> labels;
    QVector<double> targyData, alanyData, allitmanyData;
};

#endif // MAINWINDOW_H
