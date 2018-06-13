#include "weatherreader.h"

#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QSettings>

#include <QDebug>
WeatherReader::WeatherReader()
{
    QSettings qSettings{"artur00231", "Weather_log"};
    qSettings.beginGroup("Settings");
    path = QString(qSettings.value("path", tr("")).toByteArray());
    time = qSettings.value("time", 1).toInt() * 1000;
    qSettings.endGroup();

    weather.update_time = QDateTime::currentDateTime().toSecsSinceEpoch();

    file_name = tr("Weather_log_") + QDateTime::currentDateTime().toString("dd_MM_yyyy_HH_mm_ss.") + tr(".txt");

    connect(&qTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
    connect(&save_timer, SIGNAL(timeout()), this, SLOT(logWeather()));

    qTimer.start(500);
    save_timer.start(time);

}

WeatherReader::~WeatherReader()
{
    serial_port.reset();
    qThread.wait();
}

void WeatherReader::updateSerialPort(QSerialPortInfo & qSerialPortInfo)
{
    if (qSerialPortInfo.isValid())
    {
        serial_port.reset();
        qThread.wait();
        serial_port.reset(new SerialPort{qSerialPortInfo, qThread});

        serial_port->moveToThread(&qThread);
        qThread.start();

        qDebug() << "Port changed to: " << qSerialPortInfo.portName();
    }
    else
    {
        serial_port.reset();

        qDebug() << "Port closed";
    }

}

Weather WeatherReader::getWeather()
{
    Weather weather{0.0f, 0, 0, false, QDateTime::currentDateTime().toSecsSinceEpoch()};

    if (!serial_port)
    {
        return weather;
    }

    raw_data += QString{ serial_port->getData() };

    QRegularExpression re;
    re.setPattern(pattern);

    QRegularExpressionMatch match = re.match(raw_data);

    while (match.hasMatch())
    {
        weather.main_teperature = match.captured(1).toFloat();
        weather.humidity = match.captured(3).toInt();
        weather.second_teperature = match.captured(2).toInt();
        weather.updated = true;

        raw_data.remove(raw_data.indexOf(re), match.capturedLength());
        match = re.match(raw_data);
    }

    if (raw_data.size() > 50)
    {
        raw_data.clear();
    }

    return weather;
}

void WeatherReader::portChanged(QSerialPortInfo qSerialPortInfo)
{
    updateSerialPort(qSerialPortInfo);
}

void WeatherReader::updateWeather(Weather & weather)
{
    weather = this->weather;
}

void WeatherReader::timeOut()
{
    Weather weather = getWeather();

    if (weather.updated)
    {
        this->weather = weather;

        counter = 0;
        return;
    }

    if (weather.update_time - this->weather.update_time > 2)
    {
        this->weather.updated = false;

        if (counter > 10)
        {
            emit needReconnect();

            counter = 0;
        }

        ++counter;
    }


}

void WeatherReader::settingsUpdated()
{
    QSettings qSettings{"artur00231", "Weather_log"};
    qSettings.beginGroup("Settings");
    path = QString(qSettings.value("path", tr("")).toByteArray());
    time = qSettings.value("time", 1).toInt() * 1000;
    qSettings.endGroup();

    file_name = tr("Weather_log_") + QDateTime::currentDateTime().toString("dd_MM_yyyy_HH_mm_ss.") + tr(".txt");
    save_timer.setInterval(time);
}

void WeatherReader::saveToFile()
{
    if (path == "")
    {
        return;
    }

    QDir qDir{ path };

    if (!qDir.exists())
    {
        return;
    }


    QFile qFile(path + "/" + file_name);
    if (!qFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        throw std::runtime_error{ "Cannot open file" };
    }

    QTextStream qTextStream{ &qFile };

    qTextStream << QDateTime::fromSecsSinceEpoch(weather.update_time).toString("dd.MM.yyyy HH:mm:ss") << ";" << weather.humidity << ";" << weather.main_teperature << ";" << weather.second_teperature << "\n";

    qFile.flush();
    qFile.close();

}

void WeatherReader::logWeather()
{
    if (QDateTime::currentDateTime().toSecsSinceEpoch() - weather.update_time <= 1 && weather.updated)
    {
        saveToFile();
    }
    else
    {
        save_timer.setInterval(500);

        return;
    }

    if (time != save_timer.interval())
    {
        save_timer.setInterval(time);
    }
}
