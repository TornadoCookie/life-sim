#include "life.hpp"
#include <iostream>
#include <memory>

PlayerLife::PlayerLife(UrgentLifeEventLogger *u_logger, YearLogger *y_logger)
{
    nation_generator = new NationGenerator;
    name_generator = new NameGenerator;
    employer = new Employer(nation, loading_screen_callback);
    urgent_life_event_logger = u_logger;
    year_logger = y_logger;
}

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

void PlayerLife::StartRandomLife()
{
    int random_gender_number = rand(), acceptable_range;
    UrgentLifeEvent *birth_event = new UrgentLifeEvent;

    is_dead = false;

    if (random_gender_number & 1) gender = Gender::Male;
    else gender = Gender::Female;

    nation = nation_generator->GetRandomNation();

    loading_screen_callback(1, 5);
    first_name = name_generator->GetRandomFirstName(nation, gender);

    loading_screen_callback(2, 5);
    last_name = name_generator->GetRandomLastName(nation, gender);
    age = 0;

    loading_screen_callback(3, 5);
    mother = new Life;
    mother->first_name = name_generator->GetRandomFirstName(nation, Gender::Female);
    mother->last_name = last_name;
    mother->nation = nation;
    mother->age = rand() % 35 + 15; /* In range 15-50 */
    mother->sexuality = Sexuality::Heterosexual;

    loading_screen_callback(4, 5);
    father = new Life;
    father->first_name = name_generator->GetRandomFirstName(nation, Gender::Male);
    father->last_name = last_name;
    father->nation = nation;
    father->sexuality = Sexuality::Heterosexual;

    loading_screen_callback(5, 5);
    if (mother->age <= 20) acceptable_range = 1;
    else if (mother->age <= 30) acceptable_range = 3;
    else if (mother->age <= 40) acceptable_range = 4;
    else acceptable_range = 5;
    /* Make father's age within acceptable_range years of mother's age */
    father->age = rand() % (acceptable_range << 1);
    father->age -= father->age >> 1;
    father->age += mother->age;

    birth_event->default_option = 1;
    birth_event->options = {"Okay"};
    birth_event->title = "A New Life";

    std::string gender_str = gender == Gender::Male ? "male" : "female";
    std::string nation_the_str = nation.shouldHaveTheBefore  ? "the " : " ";
    birth_event->content = string_format("You were born a %s, in %s%s.\n\nYou have been named %s %s.\n\nYou are %s.",
        gender_str.c_str(), nation_the_str.c_str(), nation.name.c_str(),
        first_name.c_str(), last_name.c_str(), nation.demonym.c_str());
    urgent_life_event_logger->PromptUrgentLifeEvent(birth_event);

    year_logger->AddToThisYearLog(
        string_format("You were born a %s in %s%s.\n\n Your name is %s %s.\n\n",
        gender_str.c_str(), nation_the_str.c_str(), nation.name.c_str(), first_name.c_str(),
        last_name.c_str()));
    
    if (mother->age < 20)
        year_logger->AddToThisYearLog("You were born to young parents.\n");
    if (mother->age > 40)
        year_logger->AddToThisYearLog("You were born to old parents.\n");

    year_logger->AddToThisYearLog(
        string_format("You are the son of:\n%s %s, %d.\n%s %s, %d.\n\n",
        father->first_name.c_str(), father->last_name.c_str(), father->age,
        mother->first_name.c_str(), mother->last_name.c_str(), mother->age));
}

std::string get_relation_to_player(Life *life, PlayerLife *plr)
{
    if (life == plr->GetMother()) return "mother";
    if (life == plr->GetFather()) return "father";
    return "relative";    
}

std::string sexuality_to_string(Sexuality sexuality)
{
    switch(sexuality)
    {
        case Sexuality::Heterosexual:
            return "Heterosexual";
        default:
            return "None";
    }
}

void set_sexuality(PlayerLife *plr, Life *life, YearLogger *y_logger, UrgentLifeEventLogger *u_logger)
{
    if (life == plr)
    {
        UrgentLifeEvent *evt = new UrgentLifeEvent;
        evt->title = "Feelings";
        evt->content = "What is your sexuality?";
        evt->options = {"Heterosexual"};
        evt->default_option = 1;

        int chosen = u_logger->PromptUrgentLifeEvent(evt);
        life->sexuality = (Sexuality)chosen;

        y_logger->AddToThisYearLog(string_format("I am %s.\n", sexuality_to_string(life->sexuality).c_str()));
    }
    else
    {
        int chance = (rand() % ((int)Sexuality::Max - 1)) + 1;
        life->sexuality = (Sexuality) chance;

        y_logger->AddToThisYearLog(string_format("%s is %s.\n", life->first_name.c_str(), sexuality_to_string(life->sexuality).c_str()));
    }
}

void age_up_life(PlayerLife *plr, Life *life, YearLogger *y_logger, UrgentLifeEventLogger *u_logger)
{
    if (life->is_dead) return;
    
    life->age++;

    if (life->age == 6)
    {
        if (life == plr)
            y_logger->AddToThisYearLog("I started school.\n");
        else
            y_logger->AddToThisYearLog(string_format("My %s started school.\n", get_relation_to_player(life, plr).c_str()));
        
        life->education.current_schooling = EducationLevel::Elementary;
    }

    if (life->age > 12 && life->sexuality == Sexuality::NotChosen)
    {
        int sexuality_chance = rand() % 100;
        if (sexuality_chance < 10)
            set_sexuality(plr, life, y_logger, u_logger);
    }

    if (life->age > 65)
        life->chance_of_dying++;
    if (rand() % 100 < life->chance_of_dying)
    {
        if (life == plr)
            plr->Die(CauseOfDeath::NaturalCauses);
        else
            life->Die(CauseOfDeath::NaturalCauses);
    }
}

void PlayerLife::AgeUp()
{
    age_up_life(this, this, year_logger, urgent_life_event_logger);
    age_up_life(this, mother, year_logger, urgent_life_event_logger);
    age_up_life(this, father, year_logger, urgent_life_event_logger);
}

Life *PlayerLife::GetMother()
{
    return mother;
}

Life *PlayerLife::GetFather()
{
    return father;
}

void Life::Die(CauseOfDeath cause)
{
    is_dead = true;
    cause_of_death = cause;
}

void PlayerLife::Die(CauseOfDeath cause)
{
    is_dead = true;
    year_logger->AddToThisYearLog("I died.\n");
    cause_of_death = cause;
}

void PlayerLife::RegisterLoadingScreenCallback(void(*callback)(int,int))
{
    loading_screen_callback = callback;
    employer->loading_screen_callback = callback;
}

void PlayerLife::SetCanUseCJK(bool can)
{
    name_generator->can_use_cjk = can;
}
