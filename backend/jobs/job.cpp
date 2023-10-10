#include "jobs.hpp"
#include "nations.hpp"
#include <iostream>

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
