#include "display.h"

#include <QLCDNumber>
#include <QLabel>

#include <QDebug>

Display::Display()
{

}

void Display::set(QWidget *centralWidget, QStatusBar *qStatusBar)
{
    this->centralWidget = centralWidget;
    this->qStatusBar = qStatusBar;

    setupUI();

    connect(&qTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
    qTimer.start(500);

    QTimer::singleShot(10, this, SLOT(timeOut()));
}

void Display::unset()
{
    qDeleteAll(centralWidget->children());

    qTimer.stop();
    disconnect(&qTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
}

void Display::setupUI()
{
    qHBoxLayout = new QHBoxLayout{};
    centralWidget->setLayout(qHBoxLayout);

    auto x = new QWidget{};
    auto y = new QVBoxLayout{};
    qHBoxLayout->addWidget(x);
    x->setLayout(y);
    y->addWidget(new QLabel{"Temperature [℃]"}, 1);
    temp_1 = new QLCDNumber{};
    temp_1->setSmallDecimalPoint(true);
    temp_1->display(0);
    temp_1->setDigitCount(4);
    y->addWidget(temp_1, 4);


    auto z = new QVBoxLayout{};
    auto i = new QWidget{};
    qHBoxLayout->addWidget(i);
    i->setLayout(z);

    x = new QWidget{};
    y = new QVBoxLayout{};
    z->addWidget(x);
    x->setLayout(y);
    y->addWidget(new QLabel{"Humidity [%]"}, 1);
    humi = new QLCDNumber{};
    humi->display(0);
    humi->setDigitCount(3);
    y->addWidget(humi, 3);

    x = new QWidget{};
    y = new QVBoxLayout{};
    z->addWidget(x);
    x->setLayout(y);
    y->addWidget(new QLabel{"Temperature [℃]"}, 1);
    temp_2 = new QLCDNumber{};
    temp_2->display(0);
    temp_2->setDigitCount(3);
    y->addWidget(temp_2, 4);

    if (!active)
    {
        temp_1->setEnabled(false);
        temp_2->setEnabled(false);
        humi->setEnabled(false);
    }

}

void Display::timeOut()
{
    Weather weather;
    emit updateWeather(weather);

    if (!weather.updated)
    {
        if (active)
        {
             temp_1->setEnabled(false);
             temp_2->setEnabled(false);
             humi->setEnabled(false);

             active = false;
        }

        return;
    }

    if (!active)
    {
        temp_1->setEnabled(true);
        temp_2->setEnabled(true);
        humi->setEnabled(true);

        active = true;
    }

     temp_1->display(weather.main_teperature);
     temp_2->display(weather.second_teperature);
     humi->display(weather.humidity);
}
