#include "interface.hpp"
#include "life.hpp"
#include "events.hpp"
#include <iostream>

PlayerLife *current_life;
YearLogger *year_logger;
UrgentLifeEventLogger *urgent_life_event_logger;
int current_year;


Interface::Interface()
{
    year_logger = new YearLogger;
    urgent_life_event_logger = new UrgentLifeEventLogger;
    current_life = new PlayerLife(urgent_life_event_logger, year_logger);
}

void Interface::CallInterfaceFunction(int function, void *arg, void *result)
{

}

void Interface::StartRandomLife()
{
    srand(time(NULL));

    current_year = rand() % 80 + 1970;
    year_logger->SetStartingYear(current_year);
    year_logger->SetCurrentYear(current_year);

    current_life->StartRandomLife();
}

void Interface::AgeUp()
{
    current_year++;
    year_logger->SetCurrentYear(current_year);
    current_life->AgeUp();
}

void Interface::RegisterUrgentLifeEventCallback(int(*callback)(UrgentLifeEvent*))
{
    urgent_life_event_logger->RegisterUrgentLifeEventCallback(callback);
}

std::vector<YearLog> Interface::GetYearLog()
{
    return year_logger->log;
}

int Interface::GetBirthYear()
{
    return current_year - current_life->age;
}

YearLog Interface::GetLatestYearLog()
{
    return year_logger->log[year_logger->log.size()-1];
}

std::string Interface::GetCV()
{
    return current_life->education.GenerateCV();
}

std::string Interface::GetCVBlurb()
{
    return "You don't have any qualifications.";
}

EducationLevel Interface::GetEducationLevel()
{
    return current_life->education.current_schooling;
}

bool Interface::CanGoBackToUniversity()
{
    return false;
}

void Interface::Dropout()
{

}

void Interface::GoBackToUni()
{

}

int Interface::GetAge()
{
    return current_life->age;
}

int Interface::GetAgeAtYear(int year)
{
    return year - (current_year - current_life->age);
}

void Interface::RegisterLoadingScreenCallback(void(*callback)(int,int))
{
    current_life->RegisterLoadingScreenCallback(callback);
}
