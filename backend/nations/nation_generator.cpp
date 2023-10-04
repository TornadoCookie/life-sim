#include "nations.hpp"

NationGenerator::NationGenerator()
{

}

Nation NationGenerator::GetRandomNation()
{
    Nation nation;
    nation.name = "United States of America";
    nation.demonym = "American";
    return nation;
}
