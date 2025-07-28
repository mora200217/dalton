#ifndef TF02_PRO
#define TF02_PRO
// ==== Comandos ====
static const uint8_t FIRMWARE_VERSION[]  = {0x5A, 0x04, 0x01, 0x5F};
static const uint8_t SYSTEM_RESET[]      = {0x5A, 0x04, 0x02, 0x60};
static const uint8_t TRIGGER[]           = {0x5A, 0x04, 0x04, 0x62};
static const uint8_t OUTPUT_CM[]         = {0x5A, 0x05, 0x01, 0x65};
static const uint8_t OUTPUT_STRING_M[]   = {0x5A, 0x05, 0x02, 0x66};
static const uint8_t OUTPUT_MM[]         = {0x5A, 0x05, 0x06, 0x6A};

static const uint8_t FACTORY_RESET[]     = {0x5A, 0x04, 0x10, 0x6E};
static const uint8_t SAVE_SETTINGS[]     = {0x5A, 0x04, 0x11, 0x6F};

// ==== Respuestas ==== 
static const uint8_t FACTORY_RESET_RESPONSE_S[]    = {0x5A, 0x05, 0x10, 0x00, 0x0F};
static const uint8_t FACTORY_RESET_RESPONSE_F[]    = {0x5A, 0x05, 0x10, 0x01, 0x70};


// ==== Puerto serial ====
static HardwareSerial &lidarSerial = Serial1;


// ==== Iniciar comunicacion serial ====
static void TF02_PRO_init_uart(unsigned long baudrate = 115200) {
    lidarSerial.begin(baudrate);
    Serial.begin(115200);
    while (!Serial); 
}


// ==== Enviar comando por UART ====
static void TF02_PRO_send_command(const uint8_t *cmd, size_t len) {
    lidarSerial.write(cmd, len);
}


// ==== Leer respuesta UART (con timeout) ====
static int TF02_PRO_read_response(uint8_t *buffer, size_t len, unsigned long timeout_ms = 1000) {
    unsigned long start = millis();
    size_t index = 0;

    while (index < len && (millis() - start < timeout_ms)) {
        if (lidarSerial.available()) {
            buffer[index++] = lidarSerial.read();
        }
    }

    return index;  // devuelve cuántos bytes se leyeron
}

// ==== Imprimir respuesta ====
static void TF02_PRO_print_response(uint8_t *response, int len) {
     for (int i = 0; i < len; ++i) {
        Serial.print("0x");
        if (response[i] < 0x10) Serial.print("0");
        Serial.print(response[i], HEX);
        Serial.print(" ");
    }
}

// ==== Configurar frame rate (1 a 1000 Hz) ====
static void TF02_PRO_set_frame_rate(uint16_t hz) {
    if (hz < 1) hz = 1;
    if (hz > 1000) hz = 1000;

    uint8_t cmd[6];
    cmd[0] = 0x5A;
    cmd[1] = 0x06;
    cmd[2] = 0x03;
    cmd[3] = hz & 0xFF;         // LL
    cmd[4] = (hz >> 8) & 0xFF;  // HH
    cmd[5] = (cmd[0] + cmd[1] + cmd[2] + cmd[3] + cmd[4]) & 0xFF;  // SU = suma simple

    TF02_PRO_send_command(cmd, 6);
}


// ==== Configurar baud rate ====
static void TF02_PRO_set_baud_rate(uint32_t baud) {
    uint8_t cmd[8];
    cmd[0] = 0x5A;
    cmd[1] = 0x08;
    cmd[2] = 0x06;

    cmd[3] = (baud >>  0) & 0xFF;  // H1
    cmd[4] = (baud >>  8) & 0xFF;  // H2
    cmd[5] = (baud >> 16) & 0xFF;  // H3
    cmd[6] = (baud >> 24) & 0xFF;  // H4

    cmd[7] = (cmd[0] + cmd[1] + cmd[2] + cmd[3] + cmd[4] + cmd[5] + cmd[6]) & 0xFF; // SU
    
    TF02_PRO_send_command(cmd, 8);
}



// ==== Formateo de fabrica ====
static void TF02_PRO_factory_reset() {
    uint8_t respuesta[5];

    TF02_PRO_send_command(FACTORY_RESET, sizeof(FACTORY_RESET));
    int len = TF02_PRO_read_response(respuesta, sizeof(respuesta));

    if (respuesta == FACTORY_RESET_RESPONSE_S){
        Serial.print(" Formateo exitoso ");

    }else if(respuesta == FACTORY_RESET_RESPONSE_F){
        Serial.print(" Formateo fallido ");

    }else{
        Serial.print(" Error en transmision ");
        TF02_PRO_print_response(respuesta,len);
    }
}




#endif 
