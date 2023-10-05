#include "events.hpp"
#include <iostream>

void YearLogger::SetStartingYear(int starting_year)
{
    this->starting_year = starting_year;
}

void YearLogger::AddToYearLog(int year, std::string message)
{
    unsigned long index = year - starting_year;
    YearLog year_log;

    if (log.size() < index + 1)
    {
        year_log.year = year;
        year_log.content = message;
        log.push_back(year_log);
    }
    else
    {
        year_log = log[index];
        year_log.content.append(message);
        log[index] = year_log;
    }

}

void YearLogger::SetCurrentYear(int year)
{
    current_year = year;
    AddToThisYearLog("\n");
}

void YearLogger::AddToThisYearLog(std::string message)
{
    AddToYearLog(current_year, message);
}
