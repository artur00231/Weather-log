#ifndef WEATHERREADER_H
#define WEATHERREADER_H

#include <QString>
#include <QSerialPortInfo>
#include <QThread>
#include <QObject>
#include <QTimer>

#include <serialport.h>
#include <shared_data.h>

class WeatherReader : public QObject
{
    Q_OBJECT

public:
    WeatherReader();
    ~WeatherReader();

    WeatherReader(const WeatherReader&) = delete;
    WeatherReader& operator=(const WeatherReader&) = delete;

    WeatherReader(WeatherReader&&) = default;
    WeatherReader& operator=(WeatherReader&&) = default;

    void updateSerialPort(QSerialPortInfo & qSerialPortInfo);
    Weather getWeather();

public slots:
    void portChanged(QSerialPortInfo);
    void updateWeather(Weather&);

private slots:
    void timeOut();
    void settingsUpdated();
    void logWeather();

signals:
    void needReconnect();

protected:
    void saveToFile();

private:
    QString raw_data;
    QString pattern{ R"({([0-9.-]*);([0-9.]*);([0-9.]*)})" };
    QString path;
    QString file_name;

    std::unique_ptr<SerialPort> serial_port;
    QThread qThread;


    bool updated{ true };
    int counter{ 0 };
    Weather weather;
    QTimer qTimer;

    QTimer save_timer;
    int time;
};

#endif // WEATHERREADER_H
