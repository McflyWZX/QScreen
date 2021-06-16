#ifndef __SD_CARD_H
#define __SD_CARD_H 

#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string"

using std::string;

class SdCardBsp
{
private:
    bool hasCard = 0;
    bool SDbusy = 0;
    string mountPoint = "/sdcard";
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    sdmmc_slot_config_t slot_config;   
    sdmmc_card_t *card;
    sdmmc_host_t sdHost; 

public:
    SdCardBsp();
    void sdCardDet();
    void startDetCard();
    bool isHasCard(){return hasCard;}

};
#endif
