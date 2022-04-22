#include "mbed.h"
#include "board_freedom.h"
#include "adc.h"
#include "oled_ssd1322.h"
#include <cstdint>
#include <cstdio>
#define MESSAGE_MAX_SIZE 150
// main() runs in its own thread in the OS


void powerControl(float temperature, DigitalOut *greenLed, DigitalOut *redLed, PwmOut *heater_power){
    if(temperature >= 34.8){
        *redLed = 0;
        *heater_power = 0;
    } 
    
    if(temperature <= 30.2){
        *redLed = 1;
        *heater_power = 1;            
    }

    if(temperature >= 30 && temperature <= 35 && *redLed == 0){
        *greenLed = 1;
    } else{
        *greenLed = 0;
    }
}

void displayFDRMMessage(char* message){
    u8g2_ClearBuffer(&oled);
    u8g2_DrawUTF8(&oled, 10, 10, message);
    u8g2_SendBuffer(&oled);
}

bool detectSensorWorking(uint16_t currentAnalogReading, long int *pastAverageReadings, int *readingCounter){

    printf("Current reading %d \n",currentAnalogReading);
    if(*readingCounter == 10){
            *readingCounter = 0;
            *pastAverageReadings = *pastAverageReadings / 10;
        } else{
            *pastAverageReadings += currentAnalogReading;
        }
    return currentAnalogReading >= 10000 || currentAnalogReading >= *pastAverageReadings;
}

int main() {
// Preamble
    board_init();
    u8g2_ClearBuffer(&oled);
    u8g2_SetFont(&oled, u8g2_font_6x12_mr);
    u8g2_SendBuffer(&oled);
    PwmOut heater_power(PTC2);

    DigitalOut redLed(PTB2); 
    redLed = 0;

    DigitalOut greenLed(PTB3);
    greenLed = 0;

    // Ready a single reusable buffer for writing text to.
    char message[MESSAGE_MAX_SIZE + 1];
    message[MESSAGE_MAX_SIZE] = '\0';

    long int average_analog_value = 0;
    int analog_readings_counter = 0;

    while (true)
    {      

        uint16_t analog_in_value = adc_read (ADC1_SE18);
        analog_readings_counter++;

            if(!detectSensorWorking(analog_in_value, &average_analog_value, &analog_readings_counter)){
                
                    snprintf(message, MESSAGE_MAX_SIZE, "Sensor is faulty or missing...");
                    displayFDRMMessage(message);

                    heater_power = 0;           
                    greenLed = 0;
                    redLed = !redLed;        
                    ThisThread::sleep_for(500ms);
            } else{
        
                    // Read analog and convert
                    float voltage = (analog_in_value  / 65535.0) * 3.0;
                    float temperature = (voltage * 1000 - 400) / 19.5;
                    
                    powerControl(temperature, &greenLed, &redLed, &heater_power);

                    snprintf(message, MESSAGE_MAX_SIZE, "Temperature is %5.02f Celsius\n" , temperature );

                    displayFDRMMessage(message);        
                
            }
       
    }
}