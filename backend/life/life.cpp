#include "life.hpp"
#include <iostream>

PlayerLife::PlayerLife(UrgentLifeEventLogger *u_logger, YearLogger *y_logger)
{
    nation_generator = new NationGenerator;
    name_generator = new NameGenerator;
    urgent_life_event_logger = u_logger;
    year_logger = y_logger;
}

void PlayerLife::StartRandomLife()
{
    int random_gender_number = rand(), acceptable_range;
    UrgentLifeEvent *birth_event = new UrgentLifeEvent;

    if (random_gender_number & 1) gender = Gender::Male;
    else gender = Gender::Female;

    nation = nation_generator->GetRandomNation();

    first_name = name_generator->GetRandomFirstName(nation, gender);
    last_name = name_generator->GetRandomLastName(nation, gender);
    age = 0;

    mother = new Life;
    mother->first_name = name_generator->GetRandomFirstName(nation, Gender::Female);
    mother->last_name = last_name;
    mother->nation = nation;
    mother->age = rand() % 35 + 15; /* In range 15-50 */

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
    birth_event->content = "You were born a ";
    birth_event->content.append(gender == Gender::Male ? "male" : "female");
    birth_event->content.append(", in ");
    birth_event->content.append(nation.shouldHaveTheBefore  ? "the " : " ");
    birth_event->content.append(nation.name);
    birth_event->content.append(".\n\nYou have been named ");
    birth_event->content.append(first_name);
    birth_event->content.append(" ");
    birth_event->content.append(last_name);
    birth_event->content.append(".\n\nYou are ");
    birth_event->content.append(nation.demonym);
    birth_event->content.append(".");

    urgent_life_event_logger->PromptUrgentLifeEvent(birth_event);

    year_logger->AddToThisYearLog("You were born a ");

    if (gender == Gender::Male)
        year_logger->AddToThisYearLog("male");
    else if (gender == Gender::Female)
        year_logger->AddToThisYearLog("female");

    year_logger->AddToThisYearLog(", in ");
    if (nation.shouldHaveTheBefore)
        year_logger->AddToThisYearLog("the ");
    year_logger->AddToThisYearLog(nation.name);
    year_logger->AddToThisYearLog(".\n\nYour name is ");
    year_logger->AddToThisYearLog(first_name);
    year_logger->AddToThisYearLog(" ");
    year_logger->AddToThisYearLog(last_name);
    year_logger->AddToThisYearLog(".\n\n");

    year_logger->AddToThisYearLog("You are the son of:\n");

    year_logger->AddToThisYearLog(father->first_name);
    year_logger->AddToThisYearLog(" ");
    year_logger->AddToThisYearLog(father->last_name);
    year_logger->AddToThisYearLog(", ");
    year_logger->AddToThisYearLog(std::to_string(father->age));
    year_logger->AddToThisYearLog(".\n");

    year_logger->AddToThisYearLog(mother->first_name);
    year_logger->AddToThisYearLog(" ");
    year_logger->AddToThisYearLog(mother->last_name);
    year_logger->AddToThisYearLog(", ");
    year_logger->AddToThisYearLog(std::to_string(mother->age));
    year_logger->AddToThisYearLog(".\n\n");
}

void age_up_life(Life *life)
{
    life->age++;
    if (life->age > 65)
        life->chance_of_dying++;
    if (rand() % 100 < life->chance_of_dying)
        life->is_dead = true;
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
