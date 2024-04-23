#include <driver/gpio.h>
// Include FreeRTOS for delay
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>


//Motor 1: Outer Shaft
#define INPUT_1 2
#define INPUT_2 1
#define ENABLE_1 42

//Motor 2: Inner Shaft
#define INPUT_3 5
#define INPUT_4 6
#define ENABLE_2 7

#define MTR_1_SPEED 115
#define MTR_2_SPEED 125

bool mtr_1_clockwise = false;
bool mtr_1_counterclockwise = false;
bool mtr_2_clockwise = false;
bool mtr_2_counterclockwise = false;



  void mtr_1_task(void *pvParamters)
  {
    TickType_t xLastWakeTimeMTR_1;
    xLastWakeTimeMTR_1 = xTaskGetTickCount();

    for(;;)
    {
      xTaskDelayUntil(&xLastWakeTimeMTR_1, 4500/portTICK_PERIOD_MS);
      //clockwise direction
      digitalWrite(INPUT_1, HIGH);
      digitalWrite(INPUT_2, LOW);
      analogWrite(ENABLE_1, MTR_1_SPEED);

      xTaskDelayUntil(&xLastWakeTimeMTR_1, 4500/portTICK_PERIOD_MS);
      //counter-clockwise direction
      digitalWrite(INPUT_1, LOW);
      digitalWrite(INPUT_2, HIGH);
      analogWrite(ENABLE_1, MTR_1_SPEED);
      
    }
  }



  void mtr_2_task(void *pvParamters)
  {
    TickType_t xLastWakeTimeMTR_1;
    xLastWakeTimeMTR_1 = xTaskGetTickCount();

    for(;;)
    {
      xTaskDelayUntil(&xLastWakeTimeMTR_1, 3500/portTICK_PERIOD_MS);
      //clockwise direction
      digitalWrite(INPUT_3, HIGH);
      digitalWrite(INPUT_4, LOW);
      analogWrite(ENABLE_2, MTR_2_SPEED);

      xTaskDelayUntil(&xLastWakeTimeMTR_1, 3500/portTICK_PERIOD_MS);
      //counter-clockwise direction
      digitalWrite(INPUT_3, LOW);
      digitalWrite(INPUT_4, HIGH);
      analogWrite(ENABLE_2, MTR_2_SPEED);
      
    }
  }




void setup()
{

  //Motor-1: Outer
  pinMode(INPUT_1, OUTPUT); // gnd or +v for direction
  pinMode(INPUT_2, OUTPUT); // gnd or +v for direction
  pinMode(ENABLE_1, OUTPUT); // motor speed

  //Motor-2: Inner
  pinMode(INPUT_3, OUTPUT); // gnd or +v for direction
  pinMode(INPUT_4, OUTPUT); // gnd or +v for direction
  pinMode(ENABLE_2, OUTPUT); // motor speed



  //set all signals to low for no motor movement
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);

  xTaskCreate(mtr_1_task, "Motor 1 Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  xTaskCreate(mtr_2_task, "Motor 2 Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);


  ///Motor Control test
  //test motor functionality in one direction-MTR1
  // analogWrite(ENABLE_1, 125);
  // digitalWrite(INPUT_1, HIGH);
  // digitalWrite(INPUT_2, LOW);

  //test motor functionality in one direction-MTR2
  // analogWrite(ENABLE_2, 125);
  // digitalWrite(INPUT_1, HIGH);
  // digitalWrite(INPUT_2, LOW);



  while (1);
}

void loop()
{
  
}
