#include "interface.hpp"
#include "life.hpp"
#include "events.hpp"
#include "interface_private.hpp"

interface_reserved *state;

Interface::Interface()
{
    state = new interface_reserved;
    state->year_logger = new YearLogger(0);
    state->urgent_life_event_logger = new UrgentLifeEventLogger;
    state->current_life = new PlayerLife(state->urgent_life_event_logger, state->year_logger);
    name_generator = new NameGenerator;
    reserved = (void *)state;

    events = Interface_Events(reserved);
    life = Interface_Life(reserved);
    education = Interface_Education(reserved);
    jobs = Interface_Jobs(reserved);
}

void Interface::RegisterLoadingScreenCallback(void(*callback)(int,int))
{
    state->current_life->RegisterLoadingScreenCallback(callback);
}

void Interface::SetCanUseCJK(bool can)
{
    state->current_life->SetCanUseCJK(can);
}

void Interface::RefreshNameList()
{
    name_generator->NameListFullRefresh();
}

void Interface::SetIsOffline(bool offline)
{
    name_generator->SetOfflineMode(offline);
}
