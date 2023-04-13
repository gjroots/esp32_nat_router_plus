/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#include "led_handler.h"
#include "hardware_handler.h"
#include "router_globals.h"
#include "button_handler.h"

//-----------------------------------------------------------------------------
void item_selected_success(void)
{

    for (int i = 0; i < 4; i++)
    {
        hardware_toggle_led();
        vTaskDelay(50 / portTICK_PERIOD_MS);
        hardware_toggle_led();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    IsMenuEnable = false;
}

//-----------------------------------------------------------------------------
void led_status_task(void *pvParameters)
{
    while (1)
    {
        if (IsLedEnable || IsMenuEnable)
        {
            if (IsMenuEnable)
            {
                hardware_turn_on_led();

                // Blinking pattern for menu mode
                for (int i = 0; i < menu_count; i++)
                {
                    hardware_toggle_led();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    hardware_toggle_led();
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                }
            }
            else
            {
                (ap_connect) ? hardware_turn_on_led() : hardware_turn_off_led();

                // Blinking pattern for client mode
                for (int i = 0; i < connect_count; i++)
                {
                    hardware_toggle_led();
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                    hardware_toggle_led();
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                }
            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else
        {
            hardware_turn_off_led();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}

//-----------------------------------------------------------------------------
void start_led_status_task(void)
{
    xTaskCreate(led_status_task, "led_status_task", configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL);
}

//-----------------------------------------------------------------------------