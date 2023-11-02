/* interface_private.hpp - Private functions for the interface */
#ifndef _INTERFACE_PRIVATE_
#define _INTERFACE_PRIVATE_

#include "events.hpp"
#include "life.hpp"

struct interface_reserved {
    YearLogger *year_logger;
    UrgentLifeEventLogger *urgent_life_event_logger;
    PlayerLife *current_life;
    int current_year;
};
extern interface_reserved *state;

#endif /* _INTERFACE_PRIVATE_ */
