#include <driver/gpio.h>
// Include FreeRTOS for delay
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>

#define GRN_LED 1
#define YLW_LED 2
#define WHT_LED 42

bool green_led_on = true;
bool yellow_led_on = false;
bool white_led_on = false;

void green_led_task(void *pvParameters)
{
  TickType_t xLastWakeTimeGreen;
  xLastWakeTimeGreen = xTaskGetTickCount();
  for (;;)
  {
    xTaskDelayUntil(&xLastWakeTimeGreen, 100 / portTICK_PERIOD_MS);
    if (green_led_on)
    {
      digitalWrite(GRN_LED, LOW);
      green_led_on = false;
    }
    else
    {
      digitalWrite(GRN_LED, HIGH);
      green_led_on = true;
    }
  }
}

void yellow_led_task(void *pvParameters)
{
  TickType_t xLastWakeTimeYellow;
  xLastWakeTimeYellow = xTaskGetTickCount();
  for (;;)
  {
    xTaskDelayUntil(&xLastWakeTimeYellow, 50 / portTICK_PERIOD_MS);
    if (yellow_led_on)
    {
      digitalWrite(YLW_LED, LOW);
      yellow_led_on = false;
    }
    else
    {
      digitalWrite(YLW_LED, HIGH);
      yellow_led_on = true;
    }
  }
}

void white_led_task(void *pvParameters)
{
  TickType_t xLastWakeTimeWhite;
  xLastWakeTimeWhite = xTaskGetTickCount();
  for (;;)
  {
    xTaskDelayUntil(&xLastWakeTimeWhite, 1000 / portTICK_PERIOD_MS);
    if (white_led_on)
    {
      digitalWrite(WHT_LED, LOW);
      white_led_on = false;
    }
    else
    {
      digitalWrite(WHT_LED, HIGH);
      white_led_on = true;
    }
  }
}

void setup()
{
  pinMode(GRN_LED, OUTPUT);
  pinMode(YLW_LED, OUTPUT);
  pinMode(WHT_LED, OUTPUT);

  digitalWrite(GRN_LED, HIGH);
  digitalWrite(YLW_LED, LOW);
  digitalWrite(WHT_LED, LOW);

  xTaskCreate(green_led_task, "Green LED Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(yellow_led_task, "Yellow LED Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  xTaskCreate(white_led_task, "White LED Task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

  while (1);
}

void loop()
{
  
}