#include "mbed.h"
#include "board_freedom.h"

// main() runs in its own thread in the OS
int main()
{    	
    // open val 1700
    // close val 900
    unsigned int pwm_open=1700;
    unsigned int pwm_closed=900;
    board_init();
 	pwm0.period_ms(20);
    while(true)
    {
    pwm0.pulsewidth_us(pwm_open);
    ThisThread::sleep_for(1s);
    
    for (int i=0; i < 100; i++)
    {
                pwm0.pulsewidth_us(pwm_open - i* 975/100);
                ThisThread::sleep_for(40ms);
    }
    ThisThread::sleep_for(1s);
    }

}

