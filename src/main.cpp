#include <mbed.h>
#include <Clapi.h>

Clapi api(USBTX, USBRX);

void listener(int code, int argsCount, float args[]) {
    if (code == 1) {
        printf("kek");
    }
}

int main() {
    api.setMessageListener(listener);

    while(1) {
        sleep();
    }
}