/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <lwip/dns.h>
#include <lwip/lwip_napt.h>
#include <arpa/inet.h>
#include <esp_netif.h>
#include "lwip/sockets.h"

#include "router_globals.h"
#include "initialization.h"
#include "wifi_init.h"
#include "wifi_event_handler.h"
#include "router_handler.h"
#include "wifi_handler.h"

static const char *TAG = "wifi_event_handler";
EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

#define WIFI_RETRY_COUNT 3
#define WIFI_RETRY_DELAY_MAX 8000
static int wifi_retry_count = 0;

void wifi_retry_handler(void)
{
    if (wifi_retry_count < WIFI_RETRY_COUNT)
    {
        ESP_LOGI(TAG, "Retrying WiFi connection (%d/%d)", wifi_retry_count + 1, WIFI_RETRY_COUNT);
        esp_wifi_scan_stop();
        if (!is_scanning_progress)
            esp_wifi_connect();
        wifi_retry_count++;
    }
    else
    {
        vTaskDelay(WIFI_RETRY_DELAY_MAX / portTICK_PERIOD_MS);
        ESP_LOGW(TAG, "Maximum number of WiFi connection retries reached (%d)", WIFI_RETRY_COUNT);
        wifi_retry_count = 0;
        wifi_retry_handler();
    }
}
static void wifi_disconnect_handler(const uint8_t reason)
{
    switch (reason)
    {
    case WIFI_REASON_AUTH_EXPIRE:
    case WIFI_REASON_HANDSHAKE_TIMEOUT:
    case WIFI_REASON_AUTH_FAIL:
    case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        ESP_LOGI(TAG, "Authentication failed");
        IsWifiAuthFail = true;
        wifi_retry_handler();
        break;
    default:
        IsWifiAuthFail = false;
        wifi_retry_handler();
        break;
    }
    ap_connect = false;
}

//-----------------------------------------------------------------------------
// Function to set DNS server
void set_dns_server(esp_netif_dns_info_t dnsIP)
{
    dhcps_offer_t opt_val = OFFER_DNS;
    esp_netif_dhcps_stop(wifiAP);
    if (IsCustomDnsEnable || has_static_ip)
    {
        dnsIP.ip.u_addr.ip4.addr = ipaddr_addr(customDNSip);
    }
    ESP_ERROR_CHECK(esp_netif_set_dns_info(wifiAP, ESP_NETIF_DNS_MAIN, &dnsIP));
    esp_netif_dhcps_option(wifiAP, ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, &opt_val, sizeof(opt_val));
    ESP_LOGI(TAG, "set dns to:" IPSTR, IP2STR(&(dnsIP.ip.u_addr.ip4)));
    esp_netif_dhcps_start(wifiAP);
}

//-----------------------------------------------------------------------------
// wifi event handler
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    esp_netif_dns_info_t dns;

    if (WIFI_EVENT == event_base && WIFI_EVENT_STA_START == event_id)
    {
        esp_wifi_connect();
    }
    else if (WIFI_EVENT == event_base && WIFI_EVENT_STA_DISCONNECTED == event_id)
    {
        wifi_event_sta_disconnected_t *disconnected = (wifi_event_sta_disconnected_t *)event_data;
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);

        ESP_LOGI(TAG, "WiFi disconnected");
        wifi_disconnect_handler(disconnected->reason);
    }
    else if (IP_EVENT == event_base && IP_EVENT_STA_GOT_IP == event_id)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        ap_connect = true;
        my_ip = event->ip_info.ip.addr;
        delete_portmap_tab();
        apply_portmap_tab();

        if (esp_netif_get_dns_info(wifiSTA, ESP_NETIF_DNS_MAIN, &dns) == ESP_OK)
        {
            set_dns_server(dns);
        }
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else if (WIFI_EVENT == event_base && WIFI_EVENT_AP_STACONNECTED == event_id)
    {
        connect_count++;
        ESP_LOGI(TAG, "%d. station connected", connect_count);
    }
    else if (WIFI_EVENT == event_base && WIFI_EVENT_AP_STADISCONNECTED == event_id)
    {
        if (!(connect_count <= 0))
            connect_count--;
        ESP_LOGI(TAG, "station disconnected - %d remain", connect_count);
    }
}

//-----------------------------------------------------------------------------
// initiating wifi event registry
void wifi_events_register_init(void)
{
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));
}
