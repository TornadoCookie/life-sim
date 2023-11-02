#include "interface.hpp"
#include "interface_private.hpp"
#include <memory>
#include <stdexcept>

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

Interface_Jobs::Interface_Jobs(){}

Interface_Jobs::Interface_Jobs(void *reserved)
{
    state = (interface_reserved *)reserved;
}

int Interface_Jobs::GetJobListSize()
{
    return state->current_life->employer->available_jobs.size();
}

std::string Interface_Jobs::GetJobTitle(int job_id)
{
    std::string ret = state->current_life->employer->available_jobs[job_id]->industry;
    ret.append(state->current_life->employer->available_jobs[job_id]->title);

    return ret;
}

int Interface_Jobs::GetJobPay(int job_id)
{
    return state->current_life->employer->available_jobs[job_id]->pay;
}

std::string Interface_Jobs::GetJobRequirements(int job_id)
{
    return state->current_life->employer->available_jobs[job_id]->GetJobRequirementString();
}

std::string Interface_Jobs::GetCompanyDetails(int job_id)
{
    return state->current_life->employer->available_jobs[job_id]->GetCompanyDetailsString();
}

bool is_vowel(char c)
{
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

void Interface_Jobs::ApplyForJob(int job_id)
{
    Job *job = state->current_life->employer->available_jobs[job_id];
    bool result = job->Apply();
    UrgentLifeEvent evt;

    if (result)
    {
        evt.title = "Nice One";
        std::string a_an = is_vowel(job->industry.data()[0]) ? "n " : " ";
        evt.content = string_format("You got the job as a%s%s %s at %s.", a_an.c_str(), job->industry.c_str(), job->title.c_str(), job->company.name.c_str());
        evt.options = {"Okay"};
        evt.default_option = 1;
    }
    else
    {
        evt.title = "Bad News";
        evt.content = "You didn't get an interview.";
        evt.options = {"Okay"};
        evt.default_option = 1;
    }

    state->urgent_life_event_logger->PromptUrgentLifeEvent(evt);
}

void Interface_Jobs::RefreshJobList()
{
    state->current_life->employer->Refresh();
}
