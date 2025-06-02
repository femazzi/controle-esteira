#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Definição dos pinos
const int MOTOR_PIN = 26;  // Pino para controle do motor
const int IR_SENSOR_PIN = 27;  // Pino do sensor IR
const int PWM_CHANNEL = 0;  // Canal PWM
const int PWM_FREQ = 5000;  // Frequência do PWM em Hz
const int PWM_RESOLUTION = 8;  // Resolução do PWM (8 bits = 0-255)

// Variáveis globais
volatile unsigned long pecasContadas = 0;
volatile unsigned long ultimaContagem = 0;
const unsigned long DEBOUNCE_DELAY = 500;  // Delay de 500ms para evitar contagens duplas
bool motorLigado = false;
int velocidadeMotor = 128;  // 50% de velocidade inicial (0-255)

// Configurações do WiFi
const char* ssid = "carai";
const char* password = "cam07251060";

// Criação do servidor web
AsyncWebServer server(80);

void IRAM_ATTR contarPeca() {
    unsigned long tempoAtual = millis();
    if (tempoAtual - ultimaContagem > DEBOUNCE_DELAY) {
        pecasContadas++;
        ultimaContagem = tempoAtual;
    }
}

void setup() {
    Serial.begin(115200);
    
    // Configuração do PWM
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, 0);  // Inicia desligado
    
    pinMode(IR_SENSOR_PIN, INPUT);
    
    // Anexa a interrupção ao sensor IR
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN), contarPeca, FALLING);
    
    // Inicializa o SPIFFS
    if(!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS");
        return;
    }

    // Conexão WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println(WiFi.localIP());

    // Rotas do servidor
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    // API endpoints
    server.on("/toggle-motor", HTTP_POST, [](AsyncWebServerRequest *request){
        motorLigado = !motorLigado;
        if (motorLigado) {
            ledcWrite(PWM_CHANNEL, velocidadeMotor);
        } else {
            ledcWrite(PWM_CHANNEL, 0);
        }
        request->send(200, "application/json", "{\"status\": \"" + String(motorLigado ? "ligado" : "desligado") + "\"}");
    });

    server.on("/velocidade", HTTP_POST, [](AsyncWebServerRequest *request){
        if (request->hasParam("valor", true)) {
            String valorStr = request->getParam("valor", true)->value();
            int novaVelocidade = valorStr.toInt();
            velocidadeMotor = map(novaVelocidade, 0, 100, 0, 255);
            if (motorLigado) {
                ledcWrite(PWM_CHANNEL, velocidadeMotor);
            }
            request->send(200, "application/json", "{\"velocidade\": " + String(novaVelocidade) + "}");
        } else {
            request->send(400, "application/json", "{\"erro\": \"Parâmetro 'valor' não encontrado\"}");
        }
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        int velocidadePorcentagem = map(velocidadeMotor, 0, 255, 0, 100);
        String json = "{\"pecas\":" + String(pecasContadas) + 
                     ",\"motor\":\"" + String(motorLigado ? "ligado" : "desligado") + "\"" +
                     ",\"velocidade\":" + String(velocidadePorcentagem) + "}";
        request->send(200, "application/json", json);
    });

    server.on("/reset-counter", HTTP_POST, [](AsyncWebServerRequest *request){
        pecasContadas = 0;
        request->send(200, "application/json", "{\"status\": \"reset\"}");
    });

    server.begin();
}

void loop() {
    // O loop principal fica vazio pois estamos usando interrupções e servidor assíncrono
    delay(10);
} 