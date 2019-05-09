#include <mbed.h>
#include <Clapi.h>

Clapi api(USBTX, USBRX, CLAPPY_BAUD_115200);

void listener(int code, int argsCount, float args[]) {

}

int main() {
    api.setMessageListener(listener);

    while(1) {
        sleep();
    }
}