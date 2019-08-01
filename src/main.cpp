#include <mbed.h>
//#include <Clapi.h>

//Clapi *api = new Clapi(USBTX, USBRX, "nucleo");
DigitalOut led1(PB_0);
DigitalOut led2(PB_7);
DigitalOut led3(PB_14);

Ticker pwmController;
volatile int counter = 0;
volatile int duty1 = 20;
volatile int duty2 = 20;
volatile int duty3 = 20;
float a = 0;


// void listener(int code, int argsCount, float args[]) {
    
// }

void updatePWM() {
    if (counter == 0) {
        led1 = 1;
        led2 = 1;
        led3 = 1;
    }
    if (counter == duty1) led1 = 0;
    if (counter == duty2) led2 = 0;
    if (counter == duty3) led3 = 0;
    counter = (counter + 1) % 101;
}

int main() {
    //api->setMessageListener(listener);
    pwmController.attach_us(&updatePWM, 10);
    
    while(true) {
        a += 0.001;
        duty1 = 50 * sin(a) + 50;
        duty2 = 50 * sin(a + 2) + 50;
        duty3 = 50 * sin(a + 4) + 50;
        if (a > 6.28318530718f) a = 0;
        //printf("%d\n", duty1);
        wait(0.0001);
    }
}