/* life.hpp - Main simulation of aging up */
#ifndef _LIFE_
#define _LIFE_

#include "nations.hpp"
#include "names.hpp"
#include "events.hpp"
#include "school.hpp"
#include "jobs.hpp"

class LifeStats {
    public:
    int happiness;
    int appearance;
    int fitness;
    int intelligence;
    int musical;
    int painting;
    int social;

    bool isGoingToGym;
    bool isStudying;
    bool isTakingMusicLessons;
    bool isTakingArtLessons;
    bool isPublicSpeaking;
};

class LifeFinance {
    public:
    int bank_balance;
    int net_worth;
    int mothly_outgoings;
    int annual_salary;
};

enum class CauseOfDeath {
    NaturalCauses,
    Suicide
};

enum class Sexuality {
    NotChosen,
    Heterosexual,
    Max
};

class Life {
    public:
    std::string first_name;
    std::string last_name;
    Nation nation;
    int age;
    Gender gender;
    float chance_of_dying;
    bool is_dead;
    LifeStats stats;
    LifeFinance finance;
    LifeEducation education;
    CauseOfDeath cause_of_death;
    Job job;
    Sexuality sexuality;
    void Die(CauseOfDeath cause);
};

class PlayerLife : public Life {
    public:
    PlayerLife(UrgentLifeEventLogger *u_logger, YearLogger *y_logger);
    void StartRandomLife();
    void AgeUp();
    Life *GetMother();
    Life *GetFather();
    Life *GetLover();
    void Die(CauseOfDeath cause);
    void RegisterLoadingScreenCallback(void(*callback)(int,int));
    void SetCanUseCJK(bool can);

    Employer *employer;

    private:
    NationGenerator *nation_generator;
    NameGenerator *name_generator;
    UrgentLifeEventLogger *urgent_life_event_logger;
    YearLogger *year_logger;
    Life *mother;
    Life *father;
    Life *lover;
    void(*loading_screen_callback)(int,int);
};

#endif /* _LIFE_ */
