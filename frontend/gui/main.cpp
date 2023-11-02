#include "interface.hpp"
#include <iostream>

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
        interface->life.AgeUp();
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

int loading_now, loading_left;
bool loading;

void loading_screen_callback(int now, int left)
{
    loading = now != left;
    loading_now = now;
    loading_left = left;
}

int main()
{
    Interface *interface = new Interface;
    int menu_in;
    float progress_bar_value;

    InitWindow(640, 480, PACKAGE_STRING);

    interface->SetCanUseCJK(false);
    interface->RegisterLoadingScreenCallback(loading_screen_callback);
    interface->life.StartRandomLife();

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

        if (loading && !GuiIsLocked())
        {
            GuiLock();
        }
        else if (!loading && GuiIsLocked())
        {
            GuiUnlock();
        }

        if (loading && progress_bar_value != loading_now * loading_left)
        {
            progress_bar_value = loading_now / loading_left;
            DrawRectangle(0, 0, 640, 480, (Color){0, 0, 0, 128});
            GuiWindowBox((Rectangle){280, 144, 216, 80}, "Loading...");
            GuiProgressBar((Rectangle){288, 184, 200, 24}, NULL, NULL, &progress_bar_value, 1, loading_left);
        }

        EndDrawing();
    }
    return 0;
}
