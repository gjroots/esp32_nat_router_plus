/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_ota_ops.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "router_globals.h"
#include "ota_handler.h"

static const char *TAG = "OTA handler";

//-----------------------------------------------------------------------------
esp_err_t ota_update_handler(httpd_req_t *req)
{

  esp_err_t err;
  char buf[256];
  int remaining = req->content_len;
  esp_ota_handle_t update_handle = 0;
  const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
  const esp_partition_t *running_partition = esp_ota_get_running_partition();

  if (!update_partition)
  {
    ESP_LOGE(TAG, "Failed to get update partition");
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "Writing partition: type %d, subtype %d, offset 0x%08lx\n",
           update_partition->type, update_partition->subtype, update_partition->address);
  ESP_LOGI(TAG, "Running partition: type %d, subtype %d, offset 0x%08lx\n",
           running_partition->type, running_partition->subtype, running_partition->address);

  err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to begin OTA (%s)", esp_err_to_name(err));
    return err;
  }

  while (remaining > 0)
  {
    int ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
    if (ret <= 0)
    {
      if (ret == HTTPD_SOCK_ERR_TIMEOUT)
      {
        continue;
      }
      ESP_LOGE(TAG, "Failed to receive OTA data (%s)", esp_err_to_name(ret));
      esp_ota_abort(update_handle);
      return ESP_FAIL;
    }
    remaining -= ret;
    err = esp_ota_write(update_handle, buf, ret);
    if (err != ESP_OK)
    {
      ESP_LOGE(TAG, "Failed to write OTA data (%s)", esp_err_to_name(err));
      esp_ota_abort(update_handle);
      return ESP_FAIL;
    }
  }

  err = esp_ota_end(update_handle);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to end OTA (%s)", esp_err_to_name(err));
    esp_ota_abort(update_handle);
    return ESP_FAIL;
  }

  err = esp_ota_set_boot_partition(update_partition);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to set boot partition (%s)", esp_err_to_name(err));
    esp_ota_abort(update_handle);
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "OTA Success! Rebooting\n");

  httpd_resp_set_status(req, HTTPD_200);
  httpd_resp_send(req, NULL, 0);

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  esp_restart();

  return ESP_OK;
}

//-----------------------------------------------------------------------------
/* handling ota update process */
void ota_update_task(void *Param)
{
  const esp_partition_t *running = esp_ota_get_running_partition();
  esp_ota_img_states_t ota_state;
  if (esp_ota_get_state_partition(running, &ota_state) == ESP_OK)
  {
    if (ota_state == ESP_OTA_IMG_PENDING_VERIFY)
    {
      // Validate image some how, then call:
      esp_ota_mark_app_valid_cancel_rollback();
      // If needed: esp_ota_mark_app_invalid_rollback_and_reboot();
    }
  }

  const uint32_t task_delay_ms = 10;
  while (1)
  {
    vTaskDelay(task_delay_ms / portTICK_PERIOD_MS);
  }
}

void ota_update_init(void)
{
  xTaskCreate(ota_update_task, "ota_update_task", configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL);
}