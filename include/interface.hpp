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

/* Main interface class between frontend and backend */
class Interface {
    public:
    Interface();
    void CallInterfaceFunction(int function, void *arg, void *result);
    void StartRandomLife();
    void AgeUp();
    void RegisterUrgentLifeEventCallback(int(*callback)(UrgentLifeEvent*));
    std::vector<YearLog> GetYearLog();
    int GetBirthYear();
    YearLog GetLatestYearLog();
    std::string GetCV();
    std::string GetCVBlurb();
    EducationLevel GetEducationLevel();
    bool CanGoBackToUniversity();
    void Dropout();
    void GoBackToUni();
    int GetAge();
    int GetAgeAtYear(int year);
    void RegisterLoadingScreenCallback(void(*callback)(int,int));
    void SetCanUseCJK(bool can);
    int GetJobListSize();
    std::string GetJobTitle(int job_id);
    int GetJobPay(int job_id);
    std::string GetJobRequirements(int job_id);
    std::string GetCompanyDetails(int job_id);
    void ApplyForJob(int job_id);
    void RefreshJobList();
};

#endif /* _LIFE_INTERFACE_ */
