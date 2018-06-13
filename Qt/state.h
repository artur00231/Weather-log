#ifndef STATE_H
#define STATE_H

#include <QWidget>
#include <QStatusBar>
#include <QObject>


#include <shared_data.h>

class State : public QObject
{
    Q_OBJECT

public:
     State() = default;
     virtual ~ State() = default;

    State(const State & state) = delete;
    State& operator=(const State & state) = delete;

    State(State && state) = default;
    State& operator=(State && state) = default;

    virtual void set(QWidget *centralWidget, QStatusBar *qStatusBar) = 0;
    virtual void unset() = 0;
};

#endif // STATE_H
