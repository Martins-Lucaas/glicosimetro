const int LED1_PIN = 2;
const int LED2_PIN = 4;
const int ANALOG_PIN = 34;

const int LED1_CHANNEL = 0;
const int LED2_CHANNEL = 1;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 12; // 12 bits

// ajustáveis
int LED1_POWER = 2048; // 50% de potência (0-4095 para resolução de 12 bits)
int LED2_POWER = 3072; // 75% de potência
int LED1_INTERVAL = 1000; // Intervalo de 1 segundo
int LED2_INTERVAL = 1500; // Intervalo de 1.5 segundos

int ANALOG_READ_FREQ = 10; // 10 leitura em um segundo

void blinkLED1(void *parameter) {
  while (true) {
    // Liga o LED1
    ledcWrite(LED1_CHANNEL, LED1_POWER);
    vTaskDelay(pdMS_TO_TICKS(LED1_INTERVAL));
    
    // Desliga o LED1
    ledcWrite(LED1_CHANNEL, 0);
    vTaskDelay(pdMS_TO_TICKS(LED1_INTERVAL));
  }
}

void blinkLED2(void *parameter) {
  while (true) {
    // Liga o LED2
    ledcWrite(LED2_CHANNEL, LED2_POWER);
    vTaskDelay(pdMS_TO_TICKS(LED2_INTERVAL));
    
    // Desliga o LED2
    ledcWrite(LED2_CHANNEL, 0);
    vTaskDelay(pdMS_TO_TICKS(LED2_INTERVAL));
  }
}

void readAnalog(void *parameter) {
  const TickType_t xFrequency = pdMS_TO_TICKS(1000 / ANALOG_READ_FREQ);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (true) {
    int analogValue = analogRead(ANALOG_PIN);
    Serial.println(analogValue);
    
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {
  Serial.begin(115200);

  // Configuração do PWM para os LEDs
  ledcSetup(LED1_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(LED2_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED1_PIN, LED1_CHANNEL);
  ledcAttachPin(LED2_PIN, LED2_CHANNEL);

  // Criação das tarefas do FreeRTOS
  xTaskCreate(blinkLED1, "Blink LED1", 1024, NULL, 1, NULL);
  xTaskCreate(blinkLED2, "Blink LED2", 1024, NULL, 1, NULL);
  xTaskCreate(readAnalog, "Read Analog", 1024, NULL, 1, NULL);
}

void loop() {
}