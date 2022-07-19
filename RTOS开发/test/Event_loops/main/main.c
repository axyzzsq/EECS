/* esp_event (event loop library) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "event_source.h"
#include "esp_event_base.h"

static const char* TAG = "user_event_loops";

// Event loops
esp_event_loop_handle_t loop_with_task;

/* Event source task related definitions */
ESP_EVENT_DEFINE_BASE(TASK_EVENTS);

TaskHandle_t g_task;

static void task_iteration_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    // Two types of data can be passed in to the event handler: the handler specific data and the event-specific data.
    //
    // The handler specific data (handler_args) is a pointer to the original data, therefore, the user should ensure that
    // the memory location it points to is still valid when the handler executes.
    //
    // The event-specific data (event_data) is a pointer to a deep copy of the original data, and is managed automatically.

    char* loop;

    if (handler_args == loop_with_task) 
    {
        loop = "loop_with_task";
    } 
    ESP_LOGI(TAG, "handling %s:%s from %s", base, "TASK_ITERATION_EVENT", loop);
}

static void task_event_source(void* args)
{
    while (1)
    {
        ESP_ERROR_CHECK(esp_event_post_to(loop_with_task, TASK_EVENTS, TASK_ITERATION_EVENT, NULL, NULL, portMAX_DELAY));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Example main */
void app_main(void)
{
    ESP_LOGI(TAG, "setting up");

    esp_event_loop_args_t loop_with_task_args = {
        .queue_size = 5,
        .task_name = "loop_task", // task will be created
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };
    // Create the event loops
    ESP_ERROR_CHECK(esp_event_loop_create(&loop_with_task_args, &loop_with_task));
    // Register the handler for task iteration event. Notice that the same handler is used for handling event on different loops.
    // The loop handle is provided as an argument in order for this example to display the loop the handler is being run on.
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(loop_with_task, TASK_EVENTS, TASK_ITERATION_EVENT, task_iteration_handler, loop_with_task, NULL));
    ESP_LOGI(TAG, "starting event source");
    // Create the event source task with the same priority as the current task
    xTaskCreate(task_event_source, "task_event_source", 2048, NULL, uxTaskPriorityGet(NULL), NULL);
    ESP_LOGI(TAG, "starting application task");
}
