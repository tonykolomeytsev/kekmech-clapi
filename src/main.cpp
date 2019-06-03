#include <mbed.h>
#include <Clapi.h>

Clapi *api = new Clapi(USBTX, USBRX, "nucleo");
DigitalOut led1(PF_12);
DigitalOut led2(PD_15);
DigitalOut led3(PD_14);
DigitalOut led4(PA_7);
DigitalOut led5(PF_13);
DigitalOut *leds = new DigitalOut[5]{led1, led2, led3, led4, led5};

#define CMD_SUM 1
#define CMD_TEST 2
#define CMD_SQRT 4

void listener(int code, int argsCount, float args[]) {
    if (code == CMD_SUM) {
        api ->query("sum", args[0]+args[1])->send(CMD_SUM);
    }
    if (code == CMD_TEST) {
        api ->query("string", "hello_world")
            ->query("int", 12345)
            ->query("float", 3.51f)
            ->query("bool", true)
            ->send(CMD_TEST);
    }
    if (code == CMD_SQRT) {
        wait(0.1);
        led1 = !led1;
        api ->query("sqrt", sqrt(args[0]))->send(CMD_SQRT);
    }
    if (code == 3) {
        int lednum = static_cast<int>(args[0]);
        leds[lednum] = !leds[lednum];
    }
}

int main() {
    api->setMessageListener(listener);

    while(1) {
        wait(1);
    }
}