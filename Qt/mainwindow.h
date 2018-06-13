#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

#include <shared_data.h>
#include <statemanager.h>
#include <weatherreader.h>
#include <options.h>
#include <display.h>

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
    void on_actionView_1_triggered();

    void on_actionVIew_3_triggered();

    void on_actionRefrashe_triggered();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<StateManager> state_manager;
    std::unique_ptr<WeatherReader> weatherReader;

    std::unique_ptr<Options> options;
    std::unique_ptr<Display> display;
};

#endif // MAINWINDOW_H
