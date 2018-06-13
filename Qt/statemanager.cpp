#include "statemanager.h"

StateManager::StateManager(QWidget *centralWidget, QStatusBar *qStatusBar) :
    centralWidget{ centralWidget },
    qStatusBar{ qStatusBar }
{
}

StateManager::~StateManager()
{
}

void StateManager::set(State *state)
{
    unset();
    current_state = state;
    state->set(centralWidget, qStatusBar);
}

State* StateManager::get()
{
    return current_state;
}

bool StateManager::unset()
{
    if (current_state)
    {
       current_state->unset();

       return true;
    }

    return false;
}
