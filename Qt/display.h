#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QTimer>

#include <state.h>
#include <shared_data.h>

class Display : public State
{
    Q_OBJECT
public:
    explicit Display();

    void set(QWidget *centralWidget, QStatusBar *qStatusBar) override;
    void unset() override;


signals:
    void updateWeather(Weather&);

public slots:
    void timeOut();

private:
    void setupUI();

    QTimer qTimer;

    QHBoxLayout *qHBoxLayout;
    QLCDNumber *temp_1, *temp_2, *humi;

    QWidget *centralWidget;
    QStatusBar *qStatusBar;

    bool active{ true };
    int counter{ 0 };
};

#endif // DISPLAY_H
