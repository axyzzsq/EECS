# Nordic nRF52透传

## 例程

```c
#include "nrf.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_ble_scan.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_saadc.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "ble_nus.h"
#include "ble_conn_state.h"

#define DEVICE_NAME "Nordic BLE Example"
#define NUS_SERVICE_UUID_TYPE BLE_UUID_TYPE_VENDOR_BEGIN /**< UUID type for the Nordic UART Service (vendor specific). */

// Create a new instance of the Nordic UART Service
BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */

// Function to handle BLE events
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    switch (p_ble_evt->header.evt_id)
    {
        // When the device is connected to a BLE central device
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected");
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        
        // When the device is disconnected from a BLE central device
        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected");
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        
        default:
            // No implementation needed.
            break;
    }
}

// Function to initialize BLE
static void ble_stack_init(void)
{
    ret_code_t err_code;

    // Initialize SoftDevice
    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure BLE stack using the default settings
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &cfg_ble);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register BLE event handler
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

// Function to initialize Nordic UART Service
static void nus_service_init(void)
{
    ret_code_t err_code;

    // Create a new instance of the Nordic UART Service
    ble_nus_init_t nus_init;
    memset(&nus_init, 0, sizeof(nus_init));

    // Set UUID type to vendor specific
    nus_init.uuid_type = NUS_SERVICE_UUID_TYPE;

    // Set the maximum length of the RX and TX characteristics
    nus_init.max_rx_char_len = BLE_NUS_MAX_RX_CHAR_LEN;
    nus_init.max_tx_char_len = BLE_NUS_MAX_TX_CHAR_LEN;

    // Initialize the Nordic UART Service
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}

// Function to send data over the Nordic UART Service
static void nus_send_data(uint8_t * data, uint16_t len)
{
    ret_code_t err_code;
    
    // Check if there is an active connection
    if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        // Send data over the Nordic UART Service
        err_code = ble_nus_data_send(&m_nus, data, &len, m_conn_handle);
        if (err_code == NRF_SUCCESS)
        {
            NRF_LOG_INFO("Data sent");
        }
        else
        {
            NRF_LOG_INFO("Error sending data: %d", err_code);
        }
    }
}

// Function to initialize the logging module
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

int main(void)
{
    // Initialize logging
    log_init();
    // Initialize BLE stack
    ble_stack_init();

    // Initialize Nordic UART Service
    nus_service_init();

    // Main loop
    while (true)
    {
        // Wait for events
        nrf_pwr_mgmt_run();
    }
}
```

此代码初始化 BLE 堆栈并设置可用于通过 BLE 将数据传输到手机应用程序的 Nordic UART 服务。

 `ble_evt_handler` 函数处理事件，例如设备何时与 BLE 中央设备连接或断开连接。

 `nus_send_data` 函数可用于通过 Nordic UART 服务发送数据。

 请注意，此代码只是一个示例，可能需要修改以适合的特定应用。 还需要创建一个可以连接到 Nordic nRF52 芯片并通过 BLE 接收数据的手机应用程序。