#include "jobs.hpp"

/* TODO: Make this actually random. */
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
