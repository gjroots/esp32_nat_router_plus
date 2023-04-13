/**
 * @author Jaya Satish
 * 
 *@copyright Copyright (c) 2023
 *Licensed under MIT 
 *
 */

#include "button_handler.h"
#include "hardware_handler.h"
#include "router_globals.h"
#include "led_handler.h"
#include "initialization.h"
#include "utils.h"
#include "nvm.h"
#include "web_server.h"


int menu_count = 0;
bool IsMenuEnable = false;


//-----------------------------------------------------------------------------
void menu_count_reset(void)
{
    menu_count = -1;
}


//-----------------------------------------------------------------------------
void selected_menu_item(void)
{
    switch (menu_count)
    {
    case 0:
        printf("selected menu mode\n");
        IsMenuEnable = true;
        break;
    case 1:
        printf("selected menu: 1\n");
        IsLedEnable = false;
        item_selected_success();
        break;
    case 2:
        printf("selected menu: 2\n");
        toggle_webserver();
        item_selected_success();
        break;
    case 3:
        printf("selected menu: 3\n");
        if (nvm_erase() == ESP_OK)
        {
            item_selected_success();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            restart();
        }
        break;
    default:
        break;
    }
}


//-----------------------------------------------------------------------------
void button_task(void *pvParameters)
{
    static float menu_reset_time_s = 0;
    while (1)
    {
        static float button_down_time_s = 0;
        vTaskDelay(100 / portTICK_PERIOD_MS);
        bool button_pressed = hardware_user_button_pressed();
        if (button_pressed)
        {
            if (button_down_time_s == 0)
            {
                printf("Button pressed!\n");
                menu_reset_time_s = 0;
                button_down_time_s = system_uptime_s();
                menu_reset_time_s = system_uptime_s();
            }
            else if (system_uptime_s() > (button_down_time_s + 3))
            {
                button_down_time_s += (60 * 60);
                selected_menu_item();
                menu_count_reset();
            }
        }
        else if ((system_uptime_s() > (menu_reset_time_s + 15)) && (IsMenuEnable))
        {
            printf("Button menu time out\n");
            menu_reset_time_s = 0;
            menu_count = 0;
            IsMenuEnable = false;
        }
        else if (button_down_time_s)
        {
            // printf("Button released!\n");
            button_down_time_s = 0;
            if (menu_count > 2 || !IsMenuEnable)
            {
                menu_count = 0;
                IsMenuEnable = false;
            }
            else
            {
                menu_count++;
            }
            printf("menu number : %d \n", menu_count);
        }
    }
}


//-----------------------------------------------------------------------------
void start_button_task(void)
{
    xTaskCreate(button_task, "Button_task", configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL);
}

//-----------------------------------------------------------------------------