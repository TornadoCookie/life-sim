#ifndef _JOBS_
#define _JOBS_

#include <string>
#include "school.hpp"
#include "names.hpp"

enum class JobLevel {
    NoExperience,
    SecondarySchool,
    College,
    University,
    UniversityX10Years,
    UniversityX25Years,
    SpecializedUniversityDegree
};

class Company {
    public:
    std::string name;
    int founded;
    std::string ceo_name;
    int employees;
    std::string industry;
    int company_value;

    Nation nation;

    void Randomize();
    NameGenerator *name_generator;
};

class Job {
    public:
    int pay;
    std::string industry;
    std::string title;
    JobLevel level;
    UniversityClassType specialization;
    Company company;
    Nation nation;

    Job();
    bool Apply();
    std::string GetJobRequirementString();
    std::string GetCompanyDetailsString();
    void Randomize();
};

class Employer {
    public:
    Employer(Nation nation, void(*loading_screen_callback)(int,int));
    std::vector<Job *> available_jobs;
    void Refresh();

    void(*loading_screen_callback)(int,int);

    private:
    Nation nation;
};

#endif /* _JOBS_ */
