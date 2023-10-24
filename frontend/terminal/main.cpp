#include "interface.hpp"
#include <iostream>
#include <getopt.h>

int get_input(int default_option, int max_option)
{
    char option;

    option = std::cin.get();
    if (option == '\n')
        return default_option;
    option -= '0';
    if (option > max_option)
        return default_option;

    std::cin.get();

    return option;
}

int handle_urgent_life_event(UrgentLifeEvent *urgent_life_event)
{
    unsigned long i;
    int option;

    std::cout << "--" << urgent_life_event->title << "--" << std::endl;
    std::cout << urgent_life_event->content << std::endl;
    std::cout << "Your options: " << std::endl;

    for (i = 0; i < urgent_life_event->options.size(); i++)
    {
        std::cout << "[" << ((urgent_life_event->default_option == i+1) ? "*" : " ");
        std::cout << i+1 << "] " << urgent_life_event->options[i] << std::endl;
    }
    std::cout << "Your option: ";
    option = get_input(urgent_life_event->default_option, urgent_life_event->options.size());
    return option;
}

void print_year_log(Interface *interface, YearLog log)
{
    std::cout << interface->GetAgeAtYear(log.year) << " Years Old - " << log.year << std::endl;
    std::cout << log.content << std::endl;
}

void show_full_life_log(Interface *interface)
{
    std::vector<YearLog> full_year_log = interface->GetYearLog();
    unsigned long i;

    for (i = 0; i < full_year_log.size(); i++)
    {
        print_year_log(interface, full_year_log[i]);
    }
}

void cv_menu(Interface *interface)
{
    while (1)
    {
        std::cout << "Your CV:" << std::endl;
        std::cout << interface->GetCV() << std::endl;
        std::cout << interface->GetCVBlurb() << std::endl;

        std::cout << "Your options: " << std::endl;
        std::cout << "[" << (interface->GetEducationLevel() != EducationLevel::Uneducated ? " " : "X") << "1] Drop out of school";
        std::cout << std::endl;
        std::cout << "[X2] Educational development" << std::endl;
        std::cout << "[" << (interface->CanGoBackToUniversity() ? " " : "X") << "3] Go back to";
        std::cout << " university" << std::endl;
        std::cout << "[*4] Main menu" << std::endl;

        std::cout << "Your option: ";
        switch(get_input(4, 4))
        {
            case 1:
            interface->Dropout();
            break;
            case 3:
            interface->GoBackToUni();
            break;
            case 4:
            return;
            break;
        }
    }
}

void current_job_menu(Interface *interface)
{
    std::cout << "Do this later." << std::endl;
}

void selected_job(Interface *interface, int job_id)
{
    while (1)
    {
        std::cout << "Your options: " << std::endl;
        std::cout << "[ 1] View job requirements" << std::endl;
        std::cout << "[ 2] View company details" << std::endl;
        std::cout << "[ 3] Apply for job" << std::endl;
        std::cout << "[*4] Go back" << std::endl;
        std::cout << "Your option: ";

        switch(get_input(4, 4))
        {
            case 1:
            std::cout << "Requirements: " << interface->GetJobRequirements(job_id) << std::endl;
            break;
            case 2:
            std::cout << interface->GetCompanyDetails(job_id) << std::endl;
            break;
            case 3:
            interface->ApplyForJob(job_id);
            break;
            case 4:
            return;
        }
    }
}

void available_job_menu(Interface *interface)
{
    static bool refreshed = false;
    if (!refreshed)
    {
        std::cout << "Refreshing Job List..." << std::endl;
        interface->RefreshJobList();
        refreshed = true;
    }

    while (1)
    {
        std::cout << "Here are the available jobs:" << std::endl;

        for (int i = 0; i < interface->GetJobListSize(); i++)
        {
            std::cout << i+1 << ": " << interface->GetJobTitle(i) << " ($" << interface->GetJobPay(i) << ")" << std::endl;
        }

        std::cout << "Your options: " << std::endl;
        std::cout << "[ 1] Select job" << std::endl;
        std::cout << "[ 2] Refresh list" << std::endl;
        std::cout << "[*3] Go back" << std::endl;
        std::cout << "Your option: ";

        switch(get_input(3, 3))
        {
            case 1:
            std::cout << "What job would you like to select? ";
            selected_job(interface, get_input(-1, interface->GetJobListSize())-1);
            case 2:
            interface->RefreshJobList();
            break;
            case 3:
            return;
        }
    }
}

void job_history_menu(Interface *interface)
{
    std::cout << "Do this later." << std::endl;
}

void job_menu(Interface *interface)
{
    while (1)
    {
        std::cout << "Your options: " << std::endl;
        std::cout << "[ 1] View current job" << std::endl;
        std::cout << "[ 2] View available jobs" << std::endl;
        std::cout << "[ 3] View job history" << std::endl;
        std::cout << "[X4] View careers" << std::endl;
        std::cout << "[*5] Main menu" << std::endl;

        std::cout << "Your option: ";
        switch(get_input(4, 5))
        {
            case 1:
            current_job_menu(interface);
            break;
            case 2:
            available_job_menu(interface);
            break;
            case 3:
            job_history_menu(interface);
            break;
            case 5:
            return;
        }
    }
}

void relationship_menu(Interface *interface)
{

}

void property_menu(Interface *interface)
{

}

void life_stats_menu(Interface *interface)
{

}

void settings_menu(Interface *interface)
{

}

void handle_main_menu_option(Interface *interface, int option, bool *aged_up)
{
    switch (option)
    {
        case 1:
            interface->AgeUp();
            *aged_up = true;
        break;
        case 2:
            show_full_life_log(interface);
        break;
        case 3:
            cv_menu(interface);
        break;
        case 4:
            job_menu(interface);
        break;
        case 5:
            relationship_menu(interface);
        break;
        case 6:
            property_menu(interface);
        break;
        case 7:
            life_stats_menu(interface);
        break;
        case 8:
            settings_menu(interface);
        break;
        case 9:
            exit(EXIT_SUCCESS);
        break;
    }
}

void handle_loading_screen(int now, int left)
{
    std::cout << "Loading... (" << now << "/" << left << ")" << std::endl;
}

void start_over(Interface *);

void dead_menu(Interface *interface)
{
    while (1)
    {
        std::cout << "Your options:" << std::endl;
        std::cout << "[X1] View obituary" << std::endl;
        std::cout << "[ 2] Start over" << std::endl;
        std::cout << "[ 3] Quit" << std::endl;
        std::cout << "Your option:";

        switch(get_input(-1, 3))
        {
            case 2:
            start_over(interface);
            case 3:
            exit(EXIT_SUCCESS);
        }
    }
}

void start_over(Interface *interface)
{
    bool aged_up = true;
    YearLog log;
    int option;

    interface->StartRandomLife();

    while (1)
    {
        if (aged_up)
        {
            log = interface->GetLatestYearLog();
            print_year_log(interface, log);
        }

        if (interface->IsDead())
            dead_menu(interface);

        std::cout << "Your options:" << std::endl;
        std::cout << "[*1] Age Up" << std::endl;
        std::cout << "[ 2] View full life log" << std::endl;
        std::cout << "[ 3] View CV" << std::endl;
        std::cout << "[ 4] View Jobs" << std::endl;
        std::cout << "[ 5] View Relationships" << std::endl;
        std::cout << "[ 6] View Properties" << std::endl;
        std::cout << "[ 7] View Life Stats" << std::endl;
        std::cout << "[ 8] View Settings" << std::endl;
        std::cout << "[ 9] Quit" << std::endl;
        std::cout << "Your option: ";

        option = get_input(1, 9);
        aged_up = false;

        handle_main_menu_option(interface, option, &aged_up);
        
    }
}

void help()
{
    std::cout << PACKAGE_STRING << "\n";
    std::cout << "--help, -h: Show this help menu.\n--regen-names, -r: Regenerate the name list.\n--offline: Don't poll the internet for new names." << std::endl;
    exit(EXIT_SUCCESS);
}

bool regenerate_names = false;
bool is_offline = false;

void handle_long_arg(std::string arg)
{
    if (arg == "regen-names")
    {
        regenerate_names = true;
    }
    else if (arg == "help")
    {
        help();
    }
    else if (arg == "offline")
    {
        is_offline = true;
    }
}

int main(int argc, char **argv)
{
    Interface *interface;
    
    int c;

    while (1)
    {
        int option_index = 0;

        static struct option long_options[] = {
            {"regen-names", no_argument, 0, 'r'},
            {"help", no_argument, 0, 'h'},
            {"offline", no_argument, 0, 0},
            {0, 0, 0, 0}
        };

        c = getopt_long(argc, argv, "hr", long_options, &option_index);

        if (c == -1) break;

        switch(c)
        {
            case 0: handle_long_arg(std::string(long_options[option_index].name)); break;
            case '?':
            case 'h': help(); break;
            case 'r': regenerate_names = true; break;
        }
    }

    interface = new Interface;

    interface->SetIsOffline(is_offline);
    interface->SetCanUseCJK(false);
    interface->RegisterUrgentLifeEventCallback(handle_urgent_life_event);
    interface->RegisterLoadingScreenCallback(handle_loading_screen);

    if (regenerate_names)
        interface->RefreshNameList();

    start_over(interface);

    return 0;
}
