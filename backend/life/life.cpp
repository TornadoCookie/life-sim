#include "life.hpp"
#include <iostream>
#include <memory>
#include <cmath>

PlayerLife::PlayerLife(UrgentLifeEventLogger *u_logger, YearLogger *y_logger)
{
    nation_generator = new NationGenerator;
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

LifeStats random_life_stats()
{
    LifeStats ret;
    ret.appearance = rand() % 100;
    ret.fitness = rand() % 100;
    ret.intelligence = rand() % 100;
    ret.musical = rand() % 100;
    ret.painting = rand() % 100;
    ret.social = rand() % 100;
    ret.happiness = 100;
    return ret;
}

void add_potential(std::string potential, bool *done, YearLogger *y_logger)
{
    if (!*done)
    {
        y_logger->AddToThisYearLog("You have the potential to be:\n");
        *done = true;
    }

    y_logger->AddToThisYearLog(string_format("- %s\n", potential.c_str()));
}

void PlayerLife::StartRandomLife()
{
    int random_gender_number = rand(), acceptable_range;
    bool done = false;
    UrgentLifeEvent birth_event;

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

    stats = random_life_stats();

    birth_event.default_option = 1;
    birth_event.options = {"Okay"};
    birth_event.title = "A New Life";

    std::string gender_str = gender == Gender::Male ? "male" : "female";
    std::string nation_the_str = nation.shouldHaveTheBefore  ? "the " : " ";
    birth_event.content = string_format("You were born a %s, in%s%s.\n\nYou have been named %s %s.\n\nYou are %s.",
        gender_str.c_str(), nation_the_str.c_str(), nation.name.c_str(),
        first_name.c_str(), last_name.c_str(), nation.demonym.c_str());
    urgent_life_event_logger->PromptUrgentLifeEvent(birth_event);

    year_logger->AddToThisYearLog(
        string_format("You were born a %s, in%s%s.\n\nYour name is %s %s.\n\n",
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

    if (stats.fitness > 60) add_potential("In the military", &done, year_logger);
    if (stats.intelligence > 60) add_potential("An academic", &done, year_logger);
    if (stats.musical > 60) add_potential("A musician", &done, year_logger);
    if (stats.painting > 60) add_potential("An artist", &done, year_logger);
    if (stats.social > 60) add_potential("A social media influencer", &done, year_logger);
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
        UrgentLifeEvent evt;
        evt.title = "Feelings";
        evt.content = "What is your sexuality?";
        evt.options = {"Heterosexual"};
        evt.default_option = 1;

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

/* Real chances from http://www.bandolier.org.uk/booth/Risk/dyingage.html */
float get_chance_of_death(int age, Gender gender)
{
    if (gender == Gender::Male)
    {
        if (age > 85) return 1.0f/6;
        if (age > 75) return 1.0f/15;
        if (age > 65) return 1.0f/42;
        if (age > 55) return 1.0f/112;
        if (age > 45) return 1.0f/279;
        if (age > 35) return 1.0f/663;
        if (age > 25) return 1.0f/1215;
        if (age > 15) return 1.0f/1908;
        if (age >  5) return 1.0f/8333;
        if (age >  1) return 1.0f/4386;
        return 1.0f/177;
    }
    else
    {
        if (age > 85) return 1.0f/7;
        if (age > 75) return 1.0f/21;
        if (age > 65) return 1.0f/65;
        if (age > 55) return 1.0f/178;
        if (age > 45) return 1.0f/421;
        if (age > 35) return 1.0f/1106;
        if (age > 25) return 1.0f/2488;
        if (age > 15) return 1.0f/4132;
        if (age >  5) return 1.0f/10417;
        if (age >  1) return 1.0f/227;
        return 1/193;
    }
    return 1.0f;
}

bool should_die(Life *life)
{
    return rand() % 100 < get_chance_of_death(life->age, life->gender) * 100;
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

    if (should_die(life))
    {
        if (life == plr)
            plr->Die(CauseOfDeath::NaturalCauses);
        else
            life->Die(CauseOfDeath::NaturalCauses);
    }
}

bool set_lover(PlayerLife *plr, UrgentLifeEventLogger *u_logger, YearLogger *y_logger)
{
    Life *lover = plr->GetLover();

    int acceptable_range;
    if (plr->age <= 20) acceptable_range = 1;
    else if (plr->age <= 30) acceptable_range = 3;
    else if (plr->age <= 40) acceptable_range = 4;
    else acceptable_range = 5;
    /* Make lover's age within acceptable_range years of player's age */
    lover->age = rand() % (acceptable_range << 1);
    lover->age -= lover->age >> 1;
    lover->age += plr->age;
    lover->nation = plr->nation;

    if (plr->gender == Gender::Male)
        lover->gender = Gender::Female;
    else
        lover->gender = Gender::Male;

    lover->first_name = name_generator->GetRandomFirstName(lover->nation, lover->gender);
    lover->last_name = name_generator->GetRandomLastName(lover->nation, lover->gender);

    lover->stats = random_life_stats();

    std::string gender_str = lover->gender == Gender::Male ? "male" : "female";

    UrgentLifeEvent evt;
    evt.title = "Love <3";
    evt.content = string_format("You have met a %s called %s %s.\n\nAge: %d.\nNationality: %s.\n\nAppearance: %d.", 
    gender_str.c_str(), lover->first_name.c_str(), lover->last_name.c_str(), lover->age, lover->nation.demonym.c_str(), lover->stats.appearance);
    evt.options = {"Go For A Date", "Leave It"};
    evt.default_option = 1;
    int option = u_logger->PromptUrgentLifeEvent(evt);

    if (option == 2)
        return false;

    if (lover->stats.appearance > 50 && abs(plr->stats.appearance - lover->stats.appearance) > 20)
    {
        UrgentLifeEvent rejection_evt;
        rejection_evt.title = "</3";
        rejection_evt.content = string_format("You were rejected by %s.", lover->first_name.c_str());
        rejection_evt.options = {"Okay"};
        rejection_evt.default_option = 1;
        u_logger->PromptUrgentLifeEvent(rejection_evt);
        y_logger->AddToThisYearLog(string_format("I was rejected by %s.\n", lover->first_name.c_str()));
        return false;
    }

    UrgentLifeEvent acceptance_evt;
    acceptance_evt.title = "True love";
    acceptance_evt.content = string_format("You are in a relationship with %s.", lover->first_name.c_str());
    acceptance_evt.options = {"Okay"};
    acceptance_evt.default_option = 1;
    u_logger->PromptUrgentLifeEvent(acceptance_evt);

    y_logger->AddToThisYearLog(string_format("I'm now in a relationship with %s.\n", lover->first_name.c_str()));

    lover->job.Randomize();
    return true;
}

void PlayerLife::AgeUp()
{
    age_up_life(this, this, year_logger, urgent_life_event_logger);
    age_up_life(this, mother, year_logger, urgent_life_event_logger);
    age_up_life(this, father, year_logger, urgent_life_event_logger);

    if ((int)sexuality && !lover)
    {
        int lover_chance = rand() % 100;
        if (lover_chance < 10)
        {
            lover = new Life;
            if (!set_lover(this, urgent_life_event_logger, year_logger))
                delete lover;
        }
    }
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
    name_generator->SetCanUseCJK(can);
}

Life *PlayerLife::GetLover()
{
    return lover;
}
