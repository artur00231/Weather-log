#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>
#include <QWidget>

#include <memory>

#include <state.h>

class StateManager
{
public:
    StateManager(QWidget *centralWidget, QStatusBar *qStatusBar);
    ~StateManager();

    StateManager(const StateManager& state_manager) = delete;
    StateManager& operator=(const StateManager& state_manager) = delete;

    StateManager(StateManager&& state_manager) = default;
    StateManager& operator=(StateManager&& state_manager) = default;

    void set(State *state);
    State* get();
    bool unset();

private:
    QWidget *centralWidget;
    QStatusBar *qStatusBar;
    State *current_state{ nullptr };
};

#endif // STATEMANAGER_H
