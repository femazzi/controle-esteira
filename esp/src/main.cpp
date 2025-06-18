#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Definição dos pinos
const int MOTOR_PIN = 26;  // Pino para controle do motor
const int IR_SENSOR_PIN = 27;  // Pino do sensor IR

// Variáveis globais
volatile unsigned long pecasContadas = 0;
volatile unsigned long ultimaContagem = 0;
const unsigned long DEBOUNCE_DELAY = 500;  // Delay de 500ms para evitar contagens duplas
bool motorLigado = false;

// Configurações do Access Point
const char* ap_ssid = "Esteira-ESP32";  // Nome da rede Wi-Fi que será criada
const char* ap_password = "12345678";    // Senha da rede Wi-Fi que será criada
IPAddress local_ip(192,168,4,1);         // IP do ESP32
IPAddress gateway(192,168,4,1);          // Gateway
IPAddress subnet(255,255,255,0);         // Máscara de sub-rede

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
    
    // Configuração do pino do motor como saída digital
    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, HIGH);  // Inicia desligado
    
    // Configuração do pino do sensor IR como entrada
    pinMode(IR_SENSOR_PIN, INPUT);
    
    // Anexa a interrupção ao sensor IR
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN), contarPeca, FALLING);
    
    // Inicializa o SPIFFS
    if(!SPIFFS.begin(true)) {
        Serial.println("Erro ao montar SPIFFS");
        return;
    }

    // Configuração do Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(ap_ssid, ap_password);

    Serial.println("Access Point iniciado");
    Serial.print("SSID: ");
    Serial.println(ap_ssid);
    Serial.print("IP: ");
    Serial.println(local_ip);

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
        digitalWrite(MOTOR_PIN, motorLigado ? LOW : HIGH);  // Liga com LOW ou desliga com HIGH
        request->send(200, "application/json", "{\"status\": \"" + String(motorLigado ? "ligado" : "desligado") + "\"}");
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "{\"pecas\":" + String(pecasContadas) + 
                     ",\"motor\":\"" + String(motorLigado ? "ligado" : "desligado") + "\"}";
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