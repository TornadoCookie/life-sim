#include "names.hpp"

NameGenerator::NameGenerator()
{

}

std::string NameGenerator::GetRandomFirstName(Nation nation, Gender gender)
{
    return gender == Gender::Male ? "John" : "Jane";
}

std::string NameGenerator::GetRandomLastName(Nation nation, Gender gender)
{
    return "Doe";
}
