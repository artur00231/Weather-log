#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <QSerialPortInfo>
#include <memory>

struct Weather
{
   float main_teperature;
   int humidity;
   int second_teperature;

   bool updated;
   int update_time;
};

#endif // SHARED_DATA_H
