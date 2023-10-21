#include "nations.hpp"

NationGenerator::NationGenerator()
{

}

const std::vector<Nation> nations = {
    {.name = "Canada", .demonym = "Canadian", .api_name = "canada", .shouldHaveTheBefore = false},
    {.name = "China", .demonym = "Chinese", .api_name = "china", .shouldHaveTheBefore = false},
    {.name = "Germany", .demonym = "German", .api_name = "deutschland", .shouldHaveTheBefore = false},
    {.name = "United Kingdom", .demonym = "British", .api_name = "english", .shouldHaveTheBefore = true},
    {.name = "Spain", .demonym = "Spanish", .api_name = "espagne", .shouldHaveTheBefore = false},
    {.name = "France", .demonym = "French", .api_name = "france", .shouldHaveTheBefore = false},
    {.name = "Italy", .demonym = "Italian", .api_name = "italian", .shouldHaveTheBefore = false},
    {.name = "Poland", .demonym = "Polish", .api_name = "jezyk-polski", .shouldHaveTheBefore = false},
    {.name = "Mexico", .demonym = "Mexican", .api_name = "mexico", .shouldHaveTheBefore = false},
    {.name = "Netherlands", .demonym = "Dutch", .api_name = "nederlands", .shouldHaveTheBefore = true},
    {.name = "Portugal", .demonym = "Portuguese", .api_name = "portugues", .shouldHaveTheBefore = false},
    {.name = "Brazil", .demonym = "Brazillian", .api_name = "portugues", .shouldHaveTheBefore = false},
    {.name = "Turkey", .demonym = "Turkish", .api_name = "turkiye", .shouldHaveTheBefore = false},
    {.name = "United States", .demonym = "American", .api_name = "united-states", .shouldHaveTheBefore = true},
    {.name = "Vietnam", .demonym = "Vietnamese", .api_name = "viet-nam", .shouldHaveTheBefore = false},
    {.name = "Japan", .demonym = "Japanese", .api_name = "jp", .shouldHaveTheBefore = false},
    {.name = "South Korea", .demonym = "Korean", .api_name = "kr", .shouldHaveTheBefore = false},

};

Nation NationGenerator::GetRandomNation()
{
    return nations[rand() % nations.size()];
}

std::vector<Nation> NationGenerator::GetNationList()
{
    return nations;
}
