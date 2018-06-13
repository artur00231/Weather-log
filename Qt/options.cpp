#include "options.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSerialPortInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QByteArray>

Options::Options()
{
    QSettings qSettings{"artur00231", "Weather_log"};
    qSettings.beginGroup("Settings");
    auto_reconnect = qSettings.value("auto_reconnect", false).toBool();
    path = QString(qSettings.value("path", tr("")).toByteArray());
    time = qSettings.value("time", 1).toInt();
    qSettings.endGroup();
}

void Options::set(QWidget *centralWidget, QStatusBar *qStatusBar)
{
    this->centralWidget = centralWidget;
    this->qStatusBar = qStatusBar;

    setupUI();
    loadAvailablePorts();
}

void Options::unset()
{
    disconnect(qCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAutoReconnect(bool)));
    disconnect(qPushButton, SIGNAL(clicked()), this, SLOT(setDir()));
    disconnect(qComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setPort(int)));
    disconnect(qSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setTime(int)));
    qDeleteAll(centralWidget->children());
}

void Options::setupUI()
{
    qVBoxLayout = new QVBoxLayout{};
    qComboBox = new QComboBox{};
    qCheckBox = new QCheckBox{};
    qPushButton = new QPushButton{"Select"};
    qLineEdit = new QLineEdit{ path };
    qLineEdit->setReadOnly(true);
    qSpinBox = new QSpinBox{};
    qSpinBox->setMinimum(1);
    qSpinBox->setValue(time);


    centralWidget->setLayout(qVBoxLayout);

    auto qHBoxLayout = new QHBoxLayout{};
    auto qWidget = new QWidget{};
    qWidget->setLayout(qHBoxLayout);
    qVBoxLayout->addWidget(qWidget);
    qHBoxLayout->addWidget(new QLabel{"Active serial port:"});
    qHBoxLayout->addWidget(qComboBox);


    qHBoxLayout = new QHBoxLayout{};
    qWidget = new QWidget{};
    qWidget->setLayout(qHBoxLayout);
    qVBoxLayout->addWidget(qWidget);
    qHBoxLayout->addWidget(new QLabel{"Automatic reconnect:"});
    qHBoxLayout->addWidget(qCheckBox);
    qCheckBox->setChecked(auto_reconnect);


    qHBoxLayout = new QHBoxLayout{};
    qWidget = new QWidget{};
    qWidget->setLayout(qHBoxLayout);
    qVBoxLayout->addWidget(qWidget);
    qHBoxLayout->addWidget(new QLabel{"Save directory:"}, 1);
    qHBoxLayout->addWidget(qLineEdit, 3);
    qHBoxLayout->addWidget(qPushButton, 1);

    qHBoxLayout = new QHBoxLayout{};
    qWidget = new QWidget{};
    qWidget->setLayout(qHBoxLayout);
    qVBoxLayout->addWidget(qWidget);
    qHBoxLayout->addWidget(new QLabel{"Time between weather saves"});
    qHBoxLayout->addWidget(qSpinBox);


    connect(qCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAutoReconnect(bool)));
    connect(qComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setPort(int)));
    connect(qPushButton, SIGNAL(clicked()), this, SLOT(setDir()));
    connect(qSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setTime(int)));
}

void Options::loadAvailablePorts()
{   
    disconnect(qComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setPort(int)));

    auto ports = QSerialPortInfo::availablePorts();
    int qComboBox_index = (qSerialPortInfo.isValid() ? -1 : 0);

    qComboBox->clear();
    qComboBox->addItem("NULL");

    int i = 1;
    for (auto && x : ports)
    {
        if (qComboBox_index == -1)
        {
            if (qSerialPortInfo.portName() == x.portName())
            {
                qComboBox_index = i;
            }
        }

        qComboBox->addItem(x.portName());
        i++;
    }

    QString message = QString::number(ports.size());
    message += ports.size() == 1 ? " Port is ready" : " Ports are ready";
    qStatusBar->showMessage(message, 3600);

    if (qComboBox_index != -1)
    {
        qComboBox->setCurrentIndex(qComboBox_index);
    }

    connect(qComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setPort(int)));
}

void Options::setPort(int index)
{
    auto text = qComboBox->currentText();

    if (text == "NULL")
    {
        qStatusBar->showMessage("Port selected: " + text, 3600);
        qSerialPortInfo = QSerialPortInfo{};

        emit portChanged(qSerialPortInfo);
        return;
    }

    auto ports = QSerialPortInfo::availablePorts();
    auto it = std::find_if(ports.begin(), ports.end(), [&text](const QSerialPortInfo & port) {return port.portName() == text;});


    if (it != ports.end())
    {
        qStatusBar->showMessage("Port selected: " + text, 3600);
        qSerialPortInfo = *it;

        emit portChanged(qSerialPortInfo);
        return;
    }

    qComboBox->setCurrentIndex(0);
    qStatusBar->showMessage("ERROR", 3600);
    QApplication::beep();
    QMessageBox::critical(qComboBox, tr("ERROR"), tr("Cannot open chosen port"));
}

void Options::setAutoReconnect(bool auto_reconnect)
{
    this->auto_reconnect = auto_reconnect;

    QSettings qSettings{"artur00231", "Weather_log"};
    qSettings.beginGroup("Settings");
    qSettings.setValue("auto_reconnect", true);
    qSettings.endGroup();
}

void Options::reconnect()
{
    if(!auto_reconnect)
    {
        return;
    }

    if (!qSerialPortInfo.isValid())
    {
        return;
    }

    qStatusBar->showMessage("Reconnecting", 1000);
    emit portChanged(qSerialPortInfo);
}

void Options::refresh()
{
    loadAvailablePorts();
}

void Options::setDir()
{
    QString path = QFileDialog::getExistingDirectory();

    qLineEdit->setText(path);
    this->path = path;

    QSettings qSettings{"artur00231", "Weather_log"};
    qSettings.beginGroup("Settings");
    qSettings.setValue("path", path.toLocal8Bit());
    qSettings.endGroup();

    emit settingsChanged();
}

void Options::setTime(int time)
{
    this->time = time;

    QSettings qSettings{"artur00231", "Weather_log"};
    qSettings.beginGroup("Settings");
    qSettings.setValue("time", time);
    qSettings.endGroup();

    emit settingsChanged();
}
