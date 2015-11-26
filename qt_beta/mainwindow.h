#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
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
    void makePlot();
    void fileChanged(const QString& path);
private:
    Ui::MainWindow *ui;
    QFileSystemWatcher * watcher;
    int modositasok = 0;
};

#endif // MAINWINDOW_H
