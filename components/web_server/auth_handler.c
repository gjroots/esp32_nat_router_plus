/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#include <esp_log.h>
#include <sys/param.h>
#include <esp_tls_crypto.h>
#include "stdio.h"
#include "string.h"
#include "request_handler.h"
#include "auth_handler.h"
#include "nvs.h"
#include "cmd_nvs.h"
#include "router_globals.h"
#include "ota_handler.h"
#include "esp_http_server.h"
#include "utils.h"

char auth_buffer[512];
#define HTTPD_401 "401 Unauthorized"
#define HTTPD_403 "403 Forbidden"

int max_login_attempts;
int lock_out_time_minutes;

int failed_attempts_count = 0;
float block_start_time = 0;

#define BLOCKING_TIME_SEC (lock_out_time_minutes * 60) // convert minutes to seconds

basic_auth_info_t auth_info = {

};

//-----------------------------------------------------------------------------
// blok's the user for some time to avoid brute force attacks
static bool block_request(void)
{
  float current_time = system_uptime_s();
  // Reset block_start_time if blocking period has passed
  if ((block_start_time > 0) && ((block_start_time + BLOCKING_TIME_SEC) < current_time))
  {
    block_start_time = 0;
    failed_attempts_count = 0;
  }
  // printf("current time : %f, blocked time: %f\n", current_time, block_start_time);
  return (failed_attempts_count >= max_login_attempts && (block_start_time + BLOCKING_TIME_SEC) > current_time);
}

//-----------------------------------------------------------------------------
static char *http_auth_basic(char *username, char *password)
{
  int out;
  char user_info[128];
  static char digest[512];
  size_t n = 0;
  sprintf(user_info, "%s:%s", username, password);

  esp_crypto_base64_encode(NULL, 0, &n, (const unsigned char *)user_info, strlen(user_info));

  // 6: The length of the "Basic " string
  // n: Number of bytes for a base64 encode format
  // 1: Number of bytes for a reserved which be used to fill zero
  if (sizeof(digest) > (6 + n + 1))
  {
    strcpy(digest, "Basic ");
    esp_crypto_base64_encode((unsigned char *)digest + 6, n, (size_t *)&out, (const unsigned char *)user_info, strlen(user_info));
  }

  return digest;
}

//-----------------------------------------------------------------------------
esp_err_t authentication_handler(httpd_req_t *req, int req_id)
{

  if (block_request())
  {
    httpd_resp_set_status(req, HTTPD_403);
    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
  }

  size_t buf_len = httpd_req_get_hdr_value_len(req, "Authorization") + 1;
  if ((buf_len > 1) && (buf_len <= sizeof(auth_buffer)))
  {
    char auth_buffer[buf_len];
    if (httpd_req_get_hdr_value_str(req, "Authorization", auth_buffer, buf_len) == ESP_OK)
    {
      char *auth_credentials = http_auth_basic(auth_info.username, auth_info.password);
      if (!strncmp(auth_credentials, auth_buffer, buf_len))
      {
        failed_attempts_count = 0;
        httpd_resp_set_status(req, HTTPD_200);
        // httpd_resp_set_hdr(req, "Connection", "keep-alive");
        // Forward request to respective fields
        switch (req_id)
        {
        case POST_OTA_UPDATE_ID:
          return ota_update_handler(req);
        case COMMON_JS_ID:
          return common_js_response(req);
        case COMMON_DATA_ID:
          return common_data_request(req);
        default:
          return common_page_response_handler(req, req_id);
        }
      }
    }
  }
  printf("Not authenticated\n");
  failed_attempts_count++;
  // Update block_start_time only if blocking is initiated
  if (failed_attempts_count >= max_login_attempts && block_start_time == 0)
  {
    block_start_time = system_uptime_s();
  }

  httpd_resp_set_status(req, HTTPD_401);
  // httpd_resp_set_hdr(req, "Connection", "keep-alive");
  httpd_resp_set_hdr(req, "WWW-Authenticate", "Basic realm=\"Hello authenticate first\"");
  httpd_resp_send(req, NULL, 0);

  return ESP_OK;
}
