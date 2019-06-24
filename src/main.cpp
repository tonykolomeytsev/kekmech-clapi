#include <mbed.h>
#include <Clapi.h>
#include <Ethernet.h>

Clapi *api = new Clapi(USBTX, USBRX, "nucleo");
DigitalOut led1(PF_12);
DigitalOut led2(PD_15);
DigitalOut led3(PD_14);
DigitalOut led4(PA_7);
DigitalOut led5(PF_13);
DigitalOut *leds = new DigitalOut[5]{led1, led2, led3, led4, led5};

// Network interface
EthernetInterface net;


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
        api ->send(3);
    }
}

int main() {
    api->setMessageListener(listener);
    net.connect();
    
    // Show the network address
    const char *ip = net.get_ip_address();
    printf("IP address is: %s\n", ip ? ip : "No IP");

    // Open a socket on the network interface, and create a TCP connection to mbed.org
    TCPSocket socket;
    socket.open(&net);
    socket.connect("www.arm.com", 80);

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: www.arm.com\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);

    // Recieve a simple http response and print out the response line
    char rbuffer[64];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n")-rbuffer, rbuffer);

    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net.disconnect();
    printf("Done\n");

    while(1) {
        wait(1);
    }
}