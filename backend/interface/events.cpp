#include "interface.hpp"
#include "events.hpp"
#include "interface_private.hpp"

Interface_Events::Interface_Events(){}

Interface_Events::Interface_Events(void *reserved)
{
    state = (interface_reserved *)reserved;
}

void Interface_Events::RegisterUrgentLifeEventCallback(int(*callback)(UrgentLifeEvent))
{
    state->urgent_life_event_logger->RegisterUrgentLifeEventCallback(callback);
}

std::vector<YearLog> Interface_Events::GetYearLog()
{
    return state->year_logger->log;
}

YearLog Interface_Events::GetLatestYearLog()
{
    return state->year_logger->log[state->year_logger->log.size()-1];
}

