#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    options.reset(new Options{});
    display.reset(new Display{});
    weatherReader.reset(new WeatherReader{});

    state_manager.reset(new StateManager{centralWidget(), statusBar()});

    state_manager->set(display.get());

    connect(options.get(), SIGNAL(portChanged(QSerialPortInfo)), weatherReader.get(), SLOT(portChanged(QSerialPortInfo)));
    connect(display.get(), SIGNAL(updateWeather(Weather&)), weatherReader.get(), SLOT(updateWeather(Weather&)));
    connect(weatherReader.get(), SIGNAL(needReconnect()), options.get(), SLOT(reconnect()));
    connect(options.get(), SIGNAL(settingsChanged()), weatherReader.get(), SLOT(settingsUpdated()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionView_1_triggered()
{
    if (state_manager->get() == display.get())
    {
        return;
    }

    state_manager->set(display.get());
}

void MainWindow::on_actionVIew_3_triggered()
{
    if (state_manager->get() == options.get())
    {
        return;
    }

    state_manager->set(options.get());
}

void MainWindow::on_actionRefrashe_triggered()
{
    if (state_manager->get() == options.get())
    {
        options->refresh();
    }
}
