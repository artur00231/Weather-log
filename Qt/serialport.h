#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMutex>
#include <QThread>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    SerialPort(const QSerialPortInfo & qSerialPort, QThread & qThread);
    ~SerialPort();

    SerialPort(const SerialPort&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    SerialPort(SerialPort&&) = default;
    SerialPort& operator=(SerialPort&&) = default;

    QByteArray getData();
    bool isWorking() const;
    bool isReciving() const;

    void moveToThread(QThread *qThread);

public slots:
    void start();

private:
    void loop();
    void stop();

    QSerialPort qSerialPort;
    QSerialPortInfo qSerialPortInfo;

    mutable QMutex qMx;
    bool work { true };
    bool reciving { false };
    bool stop_flag{ false };
    QByteArray raw_data;

};

#endif // SERIALPORT_H
