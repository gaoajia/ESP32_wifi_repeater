
#include "handler.h"
#include "scan.h"

#include "router_globals.h"

static const char *TAG = "ScanHandler";

static char BALLOT_BOX[] = "&#9744;";
static char BALLOT_BOX_WITH_CHECK[] = "&#9745;";

void fillInfoData(char **db, char **symbol, char **textColor)
{
    *db = realloc(*db, 5);
    wifi_ap_record_t apinfo;
    memset(&apinfo, 0, sizeof(apinfo));
    if (esp_wifi_sta_get_ap_info(&apinfo) == ESP_OK)
    {
        sprintf(*db, "%d", apinfo.rssi);
        *symbol = BALLOT_BOX_WITH_CHECK;
        *textColor = findTextColorForSSID(apinfo.rssi);
        ESP_LOGI(TAG, "RSSI: %d", apinfo.rssi);
        ESP_LOGI(TAG, "SSID: %s", apinfo.ssid);
    }
    else
    {
        sprintf(*db, "%d", 0);
        *textColor = "info";
        *symbol = BALLOT_BOX;
    }
}

esp_err_t scan_download_get_handler(httpd_req_t *req)
{

    if (isLocked())
    {
        return unlock_handler(req);
    }

    httpd_req_to_sockfd(req);

    extern const char scan_start[] asm("_binary_scan_html_start");

    closeHeader(req);

    ESP_LOGI(TAG, "Requesting scan page");

    esp_err_t ret = httpd_resp_send(req, scan_start, HTTPD_RESP_USE_STRLEN);
    fillNodes();
    return ret;
}
