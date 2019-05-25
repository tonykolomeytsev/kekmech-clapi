#include <mbed.h>
#include <Clapi.h>

Clapi::Clapi(RawSerial &serial, const char* device_id)  {
    this->s = &serial;
    this->device_id = const_cast<char*>(device_id);
}

Clapi::Clapi(PinName tx, PinName rx, const char* device_id) {
    this->s = new RawSerial(tx, rx, CLAPPY_BAUD); //MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    this->device_id = const_cast<char*>(device_id);
}

Clapi::~Clapi() {
    delete this->s;
}

void Clapi::setMessageListener(void (*listener)(int code, int argsCount, float args[])) {
    this->listener = listener;
    s->attach(callback(this, &Clapi::processInput));
}

void Clapi::processInput() {
    // так как метод вызовется прерыванием, у нас в буфере уже будет лежать байт, читаем его
    int code = s->getc();
    s->attach(0); // выключаем прерывание
    while (!s->readable()); // waiting
    int argsCount = s->getc();

    if (argsCount == 0) {
        invokeSystemListener(code, argsCount, NULL); // в случае, если пришла какая-то системная команда, обработаем её
        if (listener != NULL) this->listener(code, argsCount, NULL);
    } else {
        // выделяем память под массив агрументов
        float *args = new float[argsCount];
        for (int i = 0; i < argsCount; i++) {
            union { char bin[4]; float f; } doubleAsBytes;

            // просто последовательно пихаем 4 байта в double ячейку
            for (int j = 0; j < 4; j++) {
                while (!s->readable()); // waiting
                doubleAsBytes.bin[j] = static_cast<int>(s->getc());
            }
            // засовываем расшифрованный double в аргументы
            args[i] = doubleAsBytes.f;
        }
        if (listener != NULL) this->listener(code, argsCount, args);
        delete[] args;
    }
    s->attach(callback(this, &Clapi::processInput)); // возвращаем прерывание
}

void Clapi::checkFirstQuery() {
    if (isFirstQuery) {
        s->putc('{');
    } else {
        s->putc(',');
    }
    isFirstQuery = false;
}

Clapi* Clapi::query(const char *key, const char *value) {
    checkFirstQuery();
    s->printf("\"%s\":\"%s\"", key, value);
    return this;
}

Clapi* Clapi::query(const char *key, const int value) {
    checkFirstQuery();
    s->printf("\"%s\":%d", key, value);
    return this;
}

Clapi* Clapi::query(const char *key, const float value) {
    checkFirstQuery();
    s->printf("\"%s\":%f", key, value);
    return this;
}

Clapi* Clapi::query(const char *key, const bool value) {
    checkFirstQuery();
    s->printf("\"%s\":%s", key, value ? "true" : "false");
    return this;
}

Clapi* Clapi::query(const char *key, const char value) {
    checkFirstQuery();
    s->printf("\"%s\":%c", key, value);
    return this;
}

Clapi* Clapi::response(const int code) {
    checkFirstQuery();
    s->printf("\"code\":%d", code);
    return this;
}

void Clapi::send(const int code) {
    checkFirstQuery();
    s->printf("\"code\":%d}\r\n", code);
    isFirstQuery = false;
}

void Clapi::send() {
    s->puts("}\r\n");
    isFirstQuery = true;
}

void Clapi::invokeSystemListener(int code, int argsCount, float args[]) {
    switch (code) {
    case CMD_HANDSHAKE:
        if (this->device_id != NULL)
            this->query("device_id", this->device_id)
                ->send(CMD_HANDSHAKE);
        break;
    }
}