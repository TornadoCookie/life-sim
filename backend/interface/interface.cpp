#include "interface.hpp"
#include "life.hpp"
#include "events.hpp"
#include <iostream>
#include <memory>

PlayerLife *current_life;
YearLogger *year_logger;
UrgentLifeEventLogger *urgent_life_event_logger;
int current_year;

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

Interface::Interface()
{
    name_generator = new NameGenerator;
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

    /* Reset Year Logger */
    delete year_logger;
    year_logger = new YearLogger;

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

void Interface::SetCanUseCJK(bool can)
{
    current_life->SetCanUseCJK(can);
}

int Interface::GetJobListSize()
{
    return current_life->employer->available_jobs.size();
}

std::string Interface::GetJobTitle(int job_id)
{
    std::string ret = current_life->employer->available_jobs[job_id]->industry;
    ret.append(current_life->employer->available_jobs[job_id]->title);

    return ret;
}

int Interface::GetJobPay(int job_id)
{
    return current_life->employer->available_jobs[job_id]->pay;
}

std::string Interface::GetJobRequirements(int job_id)
{
    return current_life->employer->available_jobs[job_id]->GetJobRequirementString();
}

std::string Interface::GetCompanyDetails(int job_id)
{
    return current_life->employer->available_jobs[job_id]->GetCompanyDetailsString();
}

bool is_vowel(char c)
{
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

void Interface::ApplyForJob(int job_id)
{
    Job *job = current_life->employer->available_jobs[job_id];
    bool result = job->Apply();
    UrgentLifeEvent *evt = new UrgentLifeEvent;

    if (result)
    {
        evt->title = "Nice One";
        std::string a_an = is_vowel(job->industry.data()[0]) ? "n " : " ";
        evt->content = string_format("You got the job as a%s%s %s at %s.", a_an.c_str(), job->industry.c_str(), job->title.c_str(), job->company.name.c_str());
        evt->options = {"Okay"};
        evt->default_option = 1;
    }
    else
    {
        evt->title = "Bad News";
        evt->content = "You didn't get an interview.";
        evt->options = {"Okay"};
        evt->default_option = 1;
    }

    urgent_life_event_logger->PromptUrgentLifeEvent(evt);
}

void Interface::RefreshJobList()
{
    current_life->employer->Refresh();
}

bool Interface::IsDead()
{
    return current_life->is_dead;
}
