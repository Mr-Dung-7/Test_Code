/* UART Events Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"


/**
 * This example shows how to use the UART driver to handle special UART events.
 *
 * It also reads data from UART0 directly, and echoes it to console.
 *
 * - Port: UART2
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */

#define UART_NUM            UART_NUM_2

#define BUF_SIZE            (1024)
#define RX_BUF_SIZE         (BUF_SIZE)

static QueueHandle_t        uart2_queue;


void init_uart (void);
static void rx_task (void *arg);
static void tx_task (void *arg);


void app_main(void)
{
    // UART initialize
    init_uart();

    // Create a task to handler UART
    xTaskCreate(rx_task, "uart_rx_task", BUF_SIZE*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(tx_task, "uart_tx_task", BUF_SIZE*2, NULL, configMAX_PRIORITIES-1, NULL);
}


void init_uart (void)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    //Install UART driver, and get the queue.
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart2_queue, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    
    //Set UART pins
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART2_PIN_TX, UART2_PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}



static void rx_task (void *arg)
{
    static const char *rx_task_tag = "RX_TASK";

    uint8_t *data = (uint8_t*) malloc (RX_BUF_SIZE);

    while (1) 
    {
        const int rxBytes = uart_read_bytes(UART_NUM, data, RX_BUF_SIZE, 200 / portTICK_PERIOD_MS);
        if (rxBytes > 0) 
        {
            data[rxBytes] = 0;
            ESP_LOGI(rx_task_tag, "Read %d bytes: '%s'", rxBytes, data);
        }
    }

    free(data);
}


static void tx_task (void *arg)
{
    uint16_t num = 0;
    uint8_t *data = (uint8_t*) malloc (BUF_SIZE);

    static const char *tx_task_msg = "Hello from ESP32!\n";

    while (1)
    {
        uint32_t len = sprintf ((char*)data, *tx_task_msg %d\n, num++);

        uart_write_bytes(UART_NUM, data, len);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    free (data);
}

