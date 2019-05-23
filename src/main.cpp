#include <mbed.h>
#include <Clapi.h>

Clapi *api = new Clapi(USBTX, USBRX);

#define CMD_SUM 1
#define CMD_TEST 2

void listener(int code, int argsCount, float args[]) {
    if (code == CMD_HANDSHAKE) {
        api ->query("device_id", "nucleo")->send();
    }
    if (code == CMD_SUM) {
        api ->query("sum", args[0]+args[1])->send();
    }
    if (code == CMD_TEST) {
        api ->query("string", "hello_world")
            ->query("int", 12345)
            ->query("float", 3.51f)
            ->query("bool", true)
            ->send();
    }
}

int main() {
    api->setMessageListener(listener);
    //api->query("device_id", "nucleo")->send();

    while(1) {
        wait(1);
    }
}