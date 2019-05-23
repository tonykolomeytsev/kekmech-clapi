#include <mbed.h>

#define ARG_BYTES_COUNT 4
#define CLAPPY_BAUD     115200
#define CMD_HANDSHAKE   0x13

/**
 * Clapi предназначен для упрощения передачи данных между микрокронтроллером и одноплатным компьютером.
 * Используйте один экземпляр Clapi, работающий с Serial портом, подключенным к одноплатнику.
 * На стороне одноплатника используйте библиотеку kekmech-clapi-raspberry.
 **/
class Clapi {
private:
    RawSerial *s;
    void (*listener)(int code, int argsCount, float args[]) = NULL;
    bool isFirstQuery = true;
public:
    /**
     * Конструктор, которому можно передать ссылку на кастомный serial объект.
     **/
    Clapi(RawSerial &serial);

    /**
     * Конструктор, самостоятельно создающий serial объект подключенный к пинам tx, rx.
     **/
    Clapi(PinName tx, PinName rx);

    /**
     * Деструктор
     **/
    ~Clapi();

    /**
     * Метод, прикрепляющий слушателя входящих сообщений.
     * 
     * code - числовой код команды
     * argsCount - количество аргументов
     * args - массив аргументов
     **/
    void setMessageListener(void (*listener)(int code, int argsCount, float args[]));

    Clapi* query(const char *key, const char *value);
    Clapi* query(const char *key, const int value);
    Clapi* query(const char *key, const float value);
    Clapi* query(const char *key, const bool value);
    Clapi* query(const char *key, const char value);
    Clapi* response(const int code);
    void send();

protected:
    /**
     * При приеме сообщения, методу передается управление для расшифровки, после чего
     * расшифрованные данные поступают в listener.
     **/
    void processInput();
    
    void checkFirstQuery();
};