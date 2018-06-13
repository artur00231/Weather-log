#include "serialport.h"

#include <QDebug>

SerialPort::SerialPort(const QSerialPortInfo & qSerialPortInfo, QThread & qThread) : qSerialPortInfo{ qSerialPortInfo }
{
    qSerialPort.setPort(qSerialPortInfo);

    if (!qSerialPort.open(QIODevice::ReadOnly))
    {
        qSerialPort.close();

        throw std::invalid_argument{"Cannot open serial port"};
    }

    qSerialPort.setBaudRate(QSerialPort::Baud9600);
    qSerialPort.setDataBits(QSerialPort::Data8);

    connect(&qThread, SIGNAL(started()), this, SLOT(start()));
}

SerialPort::~SerialPort()
{
    QMutexLocker lock{ &qMx };
    stop_flag = true;
    lock.unlock();

    while (isWorking()) {
        continue;
    }
}

void SerialPort::loop()
{
    while (work)
    {
        QMutexLocker lock{ &qMx };

        if (qSerialPort.waitForReadyRead(50))
        {
            QByteArray recived_data;
            recived_data = qSerialPort.readAll();

            raw_data.append(recived_data);
        }

        if (stop_flag)
        {
            break;
        }

        lock.unlock();
    }

}

QByteArray SerialPort::getData()
{
    QMutexLocker lock{ &qMx };

    QByteArray data_copy;
    data_copy = raw_data;
    raw_data.clear();

    lock.unlock();

    return data_copy;

}

bool SerialPort::isWorking() const
{
    QMutexLocker lock{ &qMx };
    return work;
}

bool SerialPort::isReciving() const
{
    QMutexLocker lock{ &qMx };
    return reciving;
}

void SerialPort::start()
{
    work = true;
    stop_flag = false;
    raw_data.clear();
    reciving = false;

    loop();

    stop();

    work = false;

    this->thread()->quit();

    return;
}

void SerialPort::moveToThread(QThread *qThread)
{
    qSerialPort.moveToThread(qThread);
    QObject::moveToThread(qThread);
}

void SerialPort::stop()
{
    if (qSerialPort.isOpen())
    {
        qSerialPort.close();
    }
}
