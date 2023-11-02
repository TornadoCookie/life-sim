#include "interface.hpp"
#include "interface_private.hpp"

Interface_Education::Interface_Education(){}

Interface_Education::Interface_Education(void *reserved)
{
    state = (interface_reserved *)reserved;
}

std::string Interface_Education::GetCV()
{
    return state->current_life->education.GenerateCV();
}

std::string Interface_Education::GetCVBlurb()
{
    return "You don't have any qualifications.";
}

EducationLevel Interface_Education::GetEducationLevel()
{
    return state->current_life->education.current_schooling;
}

bool Interface_Education::CanGoBackToUniversity()
{
    return false;
}

void Interface_Education::Dropout()
{

}

void Interface_Education::GoBackToUni()
{

}
