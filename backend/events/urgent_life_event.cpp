#include "events.hpp"
#include <iostream>

int UrgentLifeEventLogger::PromptUrgentLifeEvent(UrgentLifeEvent *event)
{
    if (!urgent_life_event_callback)
    {
        std::cout << "Urgent life event " << event->title << " could not be prompted because the frontend has not set an urgent life event callback." << std::endl;
    }

    return urgent_life_event_callback(event);
}

void UrgentLifeEventLogger::RegisterUrgentLifeEventCallback(int(*callback)(UrgentLifeEvent*))
{
    urgent_life_event_callback = callback;
}
