#include "jobs.hpp"
#include "nations.hpp"
#include <iostream>

#define N_JOBS 10

Employer::Employer(Nation nation, void(*loading_screen_callback)(int,int))
{
    this->nation = nation;
    this->loading_screen_callback = loading_screen_callback;
    available_jobs.reserve(N_JOBS);
    for (int i = 0; i < N_JOBS; i++)
    {
        Job *job = new Job;
        available_jobs.push_back(job);
    }
}

void Employer::Refresh()
{
    for (int i = 0; i < N_JOBS; i++)
    {
        loading_screen_callback(i+1, N_JOBS + 1);
        available_jobs[i]->nation = nation;
        available_jobs[i]->Randomize();
    }
}
