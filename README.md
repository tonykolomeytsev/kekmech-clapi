# Clapi

A library for transferring data between STM32 Nucleo and Raspberry Pi using Serial. The library is intended to be used with ARM mbed OS v 5.1 and higher.

## Installing

Just copy the lib/Clapi folder to your ARM mbed OS project.

## Features

* Receiving request data in binary format
* JSON response to a request with easily add parameters without ugly string concatenations
* Automatic non-blocking request processing (interrupt on serial port).

## Waiting for request from Raspberry Pi

After the Raspberry Pi sends a message, the interrupt will fire and the listener function will be called. You can process the message.

Raspberry Pi can send multiple float arguments.

```
#include <mbed.h>
#include <Clapi.h>

// connect to Raspberry Pi via USB
Clapi api(USBTX, USBRX, CLAPPY_BAUD_115200);

// this function will be launched by interrupt
void listener(int code, int argsCount, float args[]) {
    if (code == 1) {
        // do something
    }
}

int main() {
    // you can use lambda intead function reference
    api.setMessageListener(listener);

    while(1) {
        sleep();
    }
}
```

## Create JSON response

Message for raspberry is a JSON.

JSON sending example:

```
api
    ->query("email", "foo@bar.com")
    ->query("tel", 12345678)
    ->query("is_pressed", false)
    ->send(); // <--- DO NOT FORGET TO WRITE send() !!!
```

The code from the example will send the following line to Raspberry Pi:

```
{
    "email":"foo@bar.com",
    "tel":12345678,
    "is_pressed":false,
}
```

You can transfer **int**, **float**, **string**, **char** and **bool** values. I advise you to look at the sample code in the main.cpp file.

## License

```
MIT License

Copyright (c) 2019 Anton Kolomeytsev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```