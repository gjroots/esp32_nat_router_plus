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
#include <linenoise/linenoise.h>
#include <argtable3/argtable3.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <lwip/opt.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/lwip_napt.h>
#include <esp_netif.h>
#include <netdb.h>
#include <esp_log.h>

#include "router_globals.h"
#include "initialization.h"
#include "router_handler.h"


static const char *TAG = "Router_handler";

struct portmap_table_entry
{
    u32_t daddr;
    u16_t mport;
    u16_t dport;
    u8_t proto;
    u8_t valid;
};
struct portmap_table_entry portmap_tab[IP_PORTMAP_MAX];


//-----------------------------------------------------------------------------
esp_err_t apply_portmap_tab()
{
    for (int i = 0; i < IP_PORTMAP_MAX; i++)
    {
        if (portmap_tab[i].valid)
        {
            ip_portmap_add(portmap_tab[i].proto, my_ip, portmap_tab[i].mport, portmap_tab[i].daddr, portmap_tab[i].dport);
        }
    }
    return ESP_OK;
}


//-----------------------------------------------------------------------------
esp_err_t delete_portmap_tab()
{
    for (int i = 0; i < IP_PORTMAP_MAX; i++)
    {
        if (portmap_tab[i].valid)
        {
            ip_portmap_remove(portmap_tab[i].proto, portmap_tab[i].mport);
        }
    }
    return ESP_OK;
}


//-----------------------------------------------------------------------------
void print_portmap_tab()
{
    for (int i = 0; i < IP_PORTMAP_MAX; i++)
    {
        if (portmap_tab[i].valid)
        {
            printf("%s", portmap_tab[i].proto == PROTO_TCP ? "TCP " : "UDP ");
            ip4_addr_t addr;
            addr.addr = my_ip;
            printf(IPSTR ":%d -> ", IP2STR(&addr), portmap_tab[i].mport);
            addr.addr = portmap_tab[i].daddr;
            printf(IPSTR ":%d\n", IP2STR(&addr), portmap_tab[i].dport);
        }
    }
}


//-----------------------------------------------------------------------------
esp_err_t get_portmap_tab()
{
    esp_err_t err;
    nvs_handle_t nvs;
    size_t len;

    err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs);
    if (err != ESP_OK)
    {
        return err;
    }
    err = nvs_get_blob(nvs, "portmap_tab", NULL, &len);
    if (err == ESP_OK)
    {
        if (len != sizeof(portmap_tab))
        {
            err = ESP_ERR_NVS_INVALID_LENGTH;
        }
        else
        {
            err = nvs_get_blob(nvs, "portmap_tab", portmap_tab, &len);
            if (err != ESP_OK)
            {
                memset(portmap_tab, 0, sizeof(portmap_tab));
            }
        }
    }
    nvs_close(nvs);

    return err;
}


//-----------------------------------------------------------------------------
esp_err_t add_portmap(u8_t proto, u16_t mport, u32_t daddr, u16_t dport)
{
    esp_err_t err;
    nvs_handle_t nvs;

    for (int i = 0; i < IP_PORTMAP_MAX; i++)
    {
        if (!portmap_tab[i].valid)
        {
            portmap_tab[i].proto = proto;
            portmap_tab[i].mport = mport;
            portmap_tab[i].daddr = daddr;
            portmap_tab[i].dport = dport;
            portmap_tab[i].valid = 1;

            err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs);
            if (err != ESP_OK)
            {
                return err;
            }
            err = nvs_set_blob(nvs, "portmap_tab", portmap_tab, sizeof(portmap_tab));
            if (err == ESP_OK)
            {
                err = nvs_commit(nvs);
                if (err == ESP_OK)
                {
                    ESP_LOGI(TAG, "New portmap table stored.");
                }
            }
            nvs_close(nvs);

            ip_portmap_add(proto, my_ip, mport, daddr, dport);

            return ESP_OK;
        }
    }
    return ESP_ERR_NO_MEM;
}


//-----------------------------------------------------------------------------
esp_err_t del_portmap(u8_t proto, u16_t mport)
{
    esp_err_t err;
    nvs_handle_t nvs;

    for (int i = 0; i < IP_PORTMAP_MAX; i++)
    {
        if (portmap_tab[i].valid && portmap_tab[i].mport == mport && portmap_tab[i].proto == proto)
        {
            portmap_tab[i].valid = 0;

            err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs);
            if (err != ESP_OK)
            {
                return err;
            }
            err = nvs_set_blob(nvs, "portmap_tab", portmap_tab, sizeof(portmap_tab));
            if (err == ESP_OK)
            {
                err = nvs_commit(nvs);
                if (err == ESP_OK)
                {
                    ESP_LOGI(TAG, "New portmap table stored.");
                }
            }
            nvs_close(nvs);

            ip_portmap_remove(proto, mport);
            return ESP_OK;
        }
    }
    return ESP_OK;
}

//-----------------------------------------------------------------------------