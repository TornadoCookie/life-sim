#include "interface.hpp"
#include <iostream>

#ifdef HAVE_LIBRAYLIB

extern "C"
{
    #include <raylib.h>
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

void draw_lifelog_menu(Interface *interface)
{
    static Rectangle ScrollPanelScrollView = {};
    static Vector2 ScrollPanelScrollOffset = {};
    static Vector2 ScrollPanelBoundsOffset = {};

    GuiScrollPanel((Rectangle){24, 48, 464 - ScrollPanelBoundsOffset.x, 248 - ScrollPanelBoundsOffset.y}, std::string("Life Log").c_str(), (Rectangle){24, 48, 464, 248}, &ScrollPanelScrollOffset, &ScrollPanelScrollView);

    if (GuiButton((Rectangle){24, 296, 464, 24}, std::string("Age Up").c_str()))
        interface->AgeUp();
}

void draw_education_menu(Interface *interface)
{

}

void draw_career_menu(Interface *interface)
{

}

void draw_relationship_menu(Interface *interface)
{

}

void draw_properties_menu(Interface *interface)
{

}

void draw_lifestats_menu(Interface *interface)
{

}

void draw_settings_menu(Interface *interface)
{

}

int main()
{
    Interface *interface = new Interface;
    int menu_in;

    InitWindow(640, 480, PACKAGE_STRING);

    interface->SetCanUseCJK(false);
    interface->StartRandomLife();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        GuiToggleGroup((Rectangle){0, 0, 72, 24}, std::string("Life Log;Education;Careers;Relationships;Properties;Life Stats;Settings").c_str(), &menu_in);

        switch(menu_in)
        {
            case 0:
            draw_lifelog_menu(interface);
            break;
            case 1:
            draw_education_menu(interface);
            break;
            case 2:
            draw_career_menu(interface);
            break;
            case 3:
            draw_relationship_menu(interface);
            break;
            case 4:
            draw_properties_menu(interface);
            break;
            case 5:
            draw_lifestats_menu(interface);
            break;
            case 6:
            draw_settings_menu(interface);
            break;
        }

        EndDrawing();
    }
    return 0;
}

#else

int main()
{
    std::cout << PACKAGE_NAME << " was compiled without raylib support.\n";
    std::cout << "Please install raylib from your package manager or build it from source, then recompile." << std::endl;
    return 0;
}
 
#endif
