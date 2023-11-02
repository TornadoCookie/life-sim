/* interface.hpp - Frontend-backend interface */
#ifndef _LIFE_INTERFACE_
#define _LIFE_INTERFACE_

#include <vector>
#include <string>

/* Level of education */
enum class EducationLevel {
    Uneducated,
    Elementary,
    Secondary,
    College,
    University
};

/* Life event that must be addressed before doing anything else. */
class UrgentLifeEvent {
    public:
    std::vector<std::string> options;
    std::string title;
    std::string content;
    unsigned long default_option;
};

/* Log of the year */
class YearLog {
    public:
    int year;
    std::string content;
};

class Interface_Events {
    public:
    Interface_Events();
    Interface_Events(void *reserved);
    void RegisterUrgentLifeEventCallback(int(*callback)(UrgentLifeEvent));
    std::vector<YearLog> GetYearLog();
    YearLog GetLatestYearLog();
};

class Interface_Life {
    public:
    Interface_Life();
    Interface_Life(void *reserved);
    void StartRandomLife();
    void AgeUp();
    int GetBirthYear();
    int GetAge();
    int GetAgeAtYear(int year);
    bool IsDead();
};

class Interface_Education {
    public:
    Interface_Education();
    Interface_Education(void *reserved);
    std::string GetCV();
    std::string GetCVBlurb();
    EducationLevel GetEducationLevel();
    bool CanGoBackToUniversity();
    void Dropout();
    void GoBackToUni();
};

class Interface_Jobs {
    public:
    Interface_Jobs();
    Interface_Jobs(void *reserved);
    int GetJobListSize();
    std::string GetJobTitle(int job_id);
    int GetJobPay(int job_id);
    std::string GetJobRequirements(int job_id);
    std::string GetCompanyDetails(int job_id);
    void ApplyForJob(int job_id);
    void RefreshJobList();
};

/* Main interface class between frontend and backend */
class Interface {
    public:
    Interface();
    Interface_Events events;
    Interface_Life life;
    Interface_Education education;
    Interface_Jobs jobs;

    void RegisterLoadingScreenCallback(void(*callback)(int,int));
    void SetCanUseCJK(bool can);
    void RefreshNameList();
    void SetIsOffline(bool offline);

    private:
    void *reserved;
};

#endif /* _LIFE_INTERFACE_ */
