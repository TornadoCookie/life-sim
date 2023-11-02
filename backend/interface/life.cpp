#include "interface.hpp"
#include "interface_private.hpp"

Interface_Life::Interface_Life(){}

Interface_Life::Interface_Life(void *reserved)
{
    state = (interface_reserved *)reserved;
}

void Interface_Life::StartRandomLife()
{
    srand(time(NULL));

    state->current_year = rand() % 80 + 1970;
    state->year_logger->Reset(state->current_year);

    state->current_life->StartRandomLife();
}

void Interface_Life::AgeUp()
{
    state->current_year++;
    state->year_logger->SetCurrentYear(state->current_year);
    state->current_life->AgeUp();
}

int Interface_Life::GetBirthYear()
{
    return state->current_year - state->current_life->age;
}

int Interface_Life::GetAge()
{
    return state->current_life->age;
}

int Interface_Life::GetAgeAtYear(int year)
{
    return year - (state->current_year - state->current_life->age);
}

bool Interface_Life::IsDead()
{
    return state->current_life->is_dead;
}
