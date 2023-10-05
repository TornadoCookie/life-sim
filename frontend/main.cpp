#include "interface.hpp"
#include <iostream>

int get_input(int default_option, int max_option)
{
    char option;

    std::cin >> option;
    if (option == '\n')
        option = default_option;
    option -= '0';
    if (option > max_option)
        option = default_option;

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
        std::cout << "[" << ((urgent_life_event->default_option == i) ? "*" : " ");
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

void job_menu(Interface *interface)
{

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

int main()
{
    Interface *interface = new Interface;
    bool aged_up = true;
    YearLog log;
    int option;

    interface->RegisterUrgentLifeEventCallback(handle_urgent_life_event);
    interface->StartRandomLife();

    while (1)
    {
        if (aged_up)
        {
            log = interface->GetLatestYearLog();
            print_year_log(interface, log);
        }

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

    return 0;
}
