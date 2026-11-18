#include <cstdio>
#include<cstring>
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

#define SCLK    4
#define MOSI    5
#define MISO    10

static const char* TAG = "SPI_LOOPBACK";

extern "C" void app_main()
{
    spi_bus_config_t bus_cfg = {};
    bus_cfg.mosi_io_num = MOSI;
    bus_cfg.miso_io_num = MISO;
    bus_cfg.sclk_io_num = SCLK;
    bus_cfg.quadwp_io_num = -1;
    bus_cfg.quadhd_io_num = -1;
    bus_cfg.max_transfer_sz = 32;
    
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST,&bus_cfg,SPI_DMA_CH_AUTO));


    spi_device_interface_config_t dev_cfg = {};
    dev_cfg.clock_speed_hz = 1000*1000; //1MHz
    dev_cfg.mode = 0;                   //CPOL = 0,CPHA = 0
    dev_cfg.spics_io_num = -1;          // no cs pin needed for loopback
    dev_cfg.queue_size = 1;
    
    spi_device_handle_t dev;

    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST,&dev_cfg,&dev));

    uint8_t tx[4] = {0XDE,0XAD,0XBE,0XEF};
    uint8_t rx[4] = {0};

    spi_transaction_t t = {};
    t.length = 8*sizeof(tx);
    t.tx_buffer = tx;
    t.rx_buffer = rx;

    ESP_ERROR_CHECK(spi_device_transmit(dev,&t));

    ESP_LOGI(TAG," TX: %02x %02x %02x %02x",tx[0],tx[1],tx[2],tx[3]);
    ESP_LOGI(TAG," RX: %02x %02x %02x %02x",rx[0],rx[1],rx[2],rx[3]);


}