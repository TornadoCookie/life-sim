/* events.hpp - Events and logging */
#ifndef _EVENTS_
#define _EVENTS_

#include "interface.hpp"

class YearLogger {
    public:
    void SetStartingYear(int starting_year);
    void AddToYearLog(int year, std::string message);
    void AddToThisYearLog(std::string message);
    void SetCurrentYear(int year);
    
    std::vector<YearLog> log;

    private:
    int starting_year;
    int current_year;
};

class UrgentLifeEventLogger {
    public:
    int PromptUrgentLifeEvent(UrgentLifeEvent *event);
    void RegisterUrgentLifeEventCallback(int(*callback)(UrgentLifeEvent*));

    private:
    int (*urgent_life_event_callback)(UrgentLifeEvent*);
};

#endif /* _EVENTS_ */
