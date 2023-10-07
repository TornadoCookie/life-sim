#include "life.hpp"
#include <iostream>
#include <memory>

PlayerLife::PlayerLife(UrgentLifeEventLogger *u_logger, YearLogger *y_logger)
{
    nation_generator = new NationGenerator;
    name_generator = new NameGenerator;
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

    if (random_gender_number & 1) gender = Gender::Male;
    else gender = Gender::Female;

    nation = nation_generator->GetRandomNation();

    loading_screen_callback(1, 4);
    first_name = name_generator->GetRandomFirstName(nation, gender);

    loading_screen_callback(2, 4);
    last_name = name_generator->GetRandomLastName(nation, gender);
    age = 0;

    loading_screen_callback(3, 4);
    mother = new Life;
    mother->first_name = name_generator->GetRandomFirstName(nation, Gender::Female);
    mother->last_name = last_name;
    mother->nation = nation;
    mother->age = rand() % 35 + 15; /* In range 15-50 */

    loading_screen_callback(4, 4);
    father = new Life;
    father->first_name = name_generator->GetRandomFirstName(nation, Gender::Male);
    father->last_name = last_name;
    father->nation = nation;

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
    year_logger->AddToThisYearLog(
        string_format("You are the son of:\n%s %s, %d.\n%s %s, %d.\n\n",
        father->first_name.c_str(), father->last_name.c_str(), father->age,
        mother->first_name.c_str(), mother->last_name.c_str(), mother->age));
}

void age_up_life(Life *life)
{
    if (life->is_dead) return;
    
    life->age++;
    if (life->age > 65)
        life->chance_of_dying++;
    if (rand() % 100 < life->chance_of_dying)
        life->Die(CauseOfDeath::NaturalCauses);
}

void PlayerLife::AgeUp()
{
    age_up_life(this);
    age_up_life(mother);
    age_up_life(father);
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
}

void PlayerLife::SetCanUseCJK(bool can)
{
    name_generator->can_use_cjk = can;
}
