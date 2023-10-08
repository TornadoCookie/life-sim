#include "jobs.hpp"
#include "nations.hpp"
#include <iostream>
#include <memory>

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


std::string Job::GetJobRequirementString()
{
    switch (level)
    {
        case JobLevel::NoExperience:
            return "This job requires no qualifications or experience.";
        case JobLevel::SecondarySchool:
            return "This job requires a secondary school education.";
        case JobLevel::College:
            return "This job requires a college and university education.";
        case JobLevel::University:
            return "This job requires a university education.";
        case JobLevel::UniversityX10Years:
            return "This job requires a college and university education + 10 years of working experience.";
        case JobLevel::UniversityX25Years:
            return "This job requires a college and university education + 25 years of working experience.";
        default:
        break;
    }

    if (title == "Taxi Driver") return "This job requires a driving license.";
    
    switch(specialization)
    {
        case UniversityClassType::Computing:
            return "This job requires a computer science degree.";
        case UniversityClassType::Education:
            return "This job requires an education degree.";
        case UniversityClassType::Medicine:
            return "This job requires a medical degree.";
        case UniversityClassType::Science:
            return "This job requires a science degree.";
        case UniversityClassType::Veterinary:
            return "This job requires a veterinary degree.";
        default:
        break;
    }

    return "Error xD.";
}

bool Job::Apply()
{
    std::cout << "TODO: Actually check for job requirements" << std::endl;
    return false;
}

void Job::Randomize()
{
    pay = rand() % 100000;
    industry = "MISSING";
    title = "MISSING";
    level = JobLevel::NoExperience;
    company.nation = nation;
    company.Randomize();
}

std::string Job::GetCompanyDetailsString()
{
    return "Do this later.";
}

Job::Job()
{
    company.name_generator = new NameGenerator;
}

#define N_JOBS 10

Employer::Employer(Nation nation, void(*loading_screen_callback)(int,int))
{
    this->nation = nation;
    this->loading_screen_callback = loading_screen_callback;
    available_jobs.reserve(N_JOBS);
    for (int i = 0; i < N_JOBS; i++)
    {
        Job *job = new Job;
        available_jobs.push_back(job);
    }
    std::cout << "size=" << available_jobs.size() << ", capacity=" << available_jobs.capacity();
}

void Employer::Refresh()
{
    for (int i = 0; i < N_JOBS; i++)
    {
        loading_screen_callback(i+1, N_JOBS + 1);
        available_jobs[i]->nation = nation;
        available_jobs[i]->Randomize();
    }
}

void Company::Randomize()
{
    name = "COMPANY";
    founded = rand() % 100 + 1900;
    ceo_name = "John Doe";
    //ceo_name = string_format("%s %s", name_generator->GetRandomFirstName(nation, Gender::Male), name_generator->GetRandomLastName(nation, Gender::Male));
    employees = rand() % 500;
    company_value = rand() % 30000;
    industry = "N/A";
}
