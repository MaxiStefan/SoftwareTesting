#include "mbed.h" 
#include "board_freedom.h"
#include "oled_ssd1322.h"

// main() runs in its own thread in the OS
int main()
{ 
    // Initialize the board (and the oled screen)
    board_init();
    pwm3.write(0.1);
    float duty = 0.1;
    pwm3.period_ms(10);

    while(true){
        ThisThread::sleep_for(1000ms);
        if(duty < 0.9){            
            duty = duty + 0.1;
            pwm3.write(duty);
        }
    }


}

