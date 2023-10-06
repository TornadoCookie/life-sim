#include "nations.hpp"

NationGenerator::NationGenerator()
{

}

Nation NationGenerator::GetRandomNation()
{
    Nation nation;
    nation.name = "United States of America";
    nation.demonym = "American";
    nation.api_name = "united-states";
    return nation;
}
