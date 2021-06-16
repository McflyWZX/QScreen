#include <stdio.h>
#include "sdCard.hpp"


void SdCardBsp::sdCardDet()
{
    esp_err_t ret = ESP_OK;
    ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &sdHost, &slot_config, &mount_config, &card);
    while (1)
    {
        if (ret != ESP_OK)
        {
            if (ret == ESP_FAIL)
            {
                ESP_LOGE("SD_FS", "Failed to mount filesystem. ");
            }
            else
            {
                ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &sdHost, &slot_config, &mount_config, &card);
                ESP_LOGE("SD_FS", "Failed :%s. ",
                         esp_err_to_name(ret));
            }
            hasCard = 0;
            gpio_set_level((gpio_num_t)17, 1);
        } else {
            hasCard = 1;
            gpio_set_level((gpio_num_t)17, 0);
            if(!SDbusy)
            {
                esp_vfs_fat_sdmmc_unmount();
                ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &sdHost, &slot_config, &mount_config, &card);
            }
        }
        vTaskDelay(400 / portTICK_PERIOD_MS);
    }
}

void detTaskHandle(void *mycard)
{
    ((SdCardBsp*)mycard)->sdCardDet();
}

void SdCardBsp::startDetCard()
{
    xTaskCreatePinnedToCore( &detTaskHandle, "sdCardDetTask", 90000, this, 2, NULL , tskNO_AFFINITY);
}

SdCardBsp::SdCardBsp()
{
    mount_config = {
        .format_if_mount_failed = false, //格式化内存卡如果文件系统挂载失败
        .max_files = 8,
        .allocation_unit_size = 16 * 1024};
    
    //ESP_LOGI("SD", "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    //ESP_LOGI("SD", "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdHost = host;

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_cd = (gpio_num_t)22;

    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;

    gpio_set_pull_mode((gpio_num_t)15, GPIO_PULLUP_ONLY); // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode((gpio_num_t)2, GPIO_PULLUP_ONLY);  // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode((gpio_num_t)4, GPIO_PULLUP_ONLY);  // D1, needed in 4-line mode only
    gpio_set_pull_mode((gpio_num_t)12, GPIO_PULLUP_ONLY); // D2, needed in 4-line mode only
    gpio_set_pull_mode((gpio_num_t)13, GPIO_PULLUP_ONLY); // D3, needed in 4- and 1-line modes

    gpio_pad_select_gpio((gpio_num_t)17);
    gpio_set_direction((gpio_num_t)17, GPIO_MODE_OUTPUT);

}
