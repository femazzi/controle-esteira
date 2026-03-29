# 🏭 Sistema de Controle de Esteira com ESP32

Sistema de controle e monitoramento de esteira transportadora desenvolvido durante o curso de Mecatrônica. O projeto permite controlar o acionamento da esteira via interface web e realizar a contagem automática de peças utilizando sensor óptico de barreira.

## 📋 Sobre o Projeto

Este projeto implementa um sistema completo de automação para esteira transportadora, incluindo:

- **Controle remoto** do motor da esteira via Wi-Fi
- **Contagem automática** de peças através de sensor óptico
- **Interface web** responsiva e intuitiva
- **Modo Access Point** para conexão direta sem necessidade de rede Wi-Fi externa

## 🔧 Hardware Utilizado

- **Microcontrolador:** ESP32 (Espressif)
- **Sensor:** Sensor óptico de barreira (infravermelho)
- **Atuador:** Motor da esteira (controlado via GPIO)
- **Alimentação:** Conforme especificações do ESP32 e motor

### Pinagem

| Componente | Pino GPIO |
|-----------|-----------|
| Motor da Esteira | GPIO 26 |
| Sensor IR (Barreira) | GPIO 27 |

## 🛠️ Tecnologias Utilizadas

- **Plataforma:** PlatformIO (VS Code)
- **Framework:** Arduino para ESP32
- **Servidor Web:** ESPAsyncWebServer
- **Sistema de Arquivos:** SPIFFS
- **Frontend:** HTML5, CSS3, JavaScript

### Bibliotecas

```ini
- ESPAsyncWebServer-esphome @ ^3.0.0
- AsyncTCP @ ^1.1.1
```

## 🚀 Como Usar

### Pré-requisitos

1. **VS Code** instalado
2. **PlatformIO** extension instalada no VS Code
3. **Cabo USB** para conexão com o ESP32
4. **Driver** CH340/CP2102 (conforme seu modelo de ESP32)

### Instalação

1. Clone o repositório:
```bash
git clone https://github.com/femazzi/controle-esteira.git
cd controle-esteira/esp
```

2. Abra o projeto no VS Code com PlatformIO

3. Configure a porta COM no arquivo `platformio.ini`:
```ini
upload_port = COM4  ; Altere para sua porta
monitor_port = COM4
```

4. Compile e faça o upload do código:
   - Pressione `Ctrl+Shift+P` e selecione `PlatformIO: Upload`
   - Ou clique no ícone de seta na barra inferior do VS Code

5. Faça o upload do sistema de arquivos (SPIFFS):
   - Pressione `Ctrl+Shift+P` e selecione `PlatformIO: Upload File System image`
   - Isso enviará os arquivos HTML, CSS e JS para o ESP32

### Conexão

1. Após o upload, o ESP32 criará uma rede Wi-Fi própria:
   - **SSID:** `Esteira-ESP32`
   - **Senha:** `12345678`
   - **IP:** `192.168.4.1`

2. Conecte seu dispositivo (celular, tablet ou computador) à rede Wi-Fi criada

3. Acesse a interface web através do navegador:
   ```
   http://192.168.4.1
   ```

## 💡 Funcionalidades

### Interface Web

A interface web oferece:

- **Status em tempo real** do motor (ligado/desligado)
- **Indicador visual** do estado do motor
- **Contador de peças** com atualização automática
- **Botão de controle** para ligar/desligar o motor
- **Botão de reset** para zerar o contador
- **Design responsivo** para uso em dispositivos móveis

### Sistema de Contagem

- Utiliza **interrupção por hardware** para contagem precisa
- **Sistema de debounce** (500ms) para evitar contagens duplicadas
- Contagem acionada na **borda de descida** do sinal do sensor
- Atualização automática na interface web

### API REST

O sistema expõe os seguintes endpoints:

| Endpoint | Método | Descrição |
|----------|--------|-----------|
| `/` | GET | Página principal (interface web) |
| `/toggle-motor` | POST | Liga/desliga o motor |
| `/status` | GET | Retorna status do motor e contagem |
| `/reset-counter` | POST | Reseta o contador de peças |

## 📂 Estrutura do Projeto

```
controle-esteira/
├── esp/
│   ├── data/                  # Arquivos da interface web (SPIFFS)
│   │   ├── index.html        # Estrutura da página
│   │   ├── style.css         # Estilos da interface
│   │   └── script.js         # Lógica de interação
│   ├── src/
│   │   └── main.cpp          # Código principal do ESP32
│   └── platformio.ini        # Configurações do PlatformIO
└── README.md
```

## 🔌 Esquema de Ligação

```
ESP32
├── GPIO 26 ──→ Módulo Relé/Driver ──→ Motor
├── GPIO 27 ──→ Sensor IR (Saída Digital)
├── GND ──────→ GND comum
└── 3V3/5V ───→ Alimentação do sensor
```

> ⚠️ **Atenção:** Certifique-se de usar um módulo relé ou driver adequado para acionar o motor, nunca conecte motores diretamente aos pinos GPIO do ESP32.

## ⚙️ Configurações Personalizáveis

No arquivo `main.cpp`, você pode ajustar:

```cpp
// Credenciais do Access Point
const char* ap_ssid = "Esteira-ESP32";
const char* ap_password = "12345678";

// Configuração de IP
IPAddress local_ip(192,168,4,1);

// Tempo de debounce do sensor (em milissegundos)
const unsigned long DEBOUNCE_DELAY = 500;
```

## 🐛 Solução de Problemas

### O ESP32 não é reconhecido

- Verifique se o driver CH340/CP2102 está instalado
- Teste com outro cabo USB (alguns cabos são apenas para carga)
- Verifique a porta COM no Gerenciador de Dispositivos

### Não consigo conectar na rede Wi-Fi

- Aguarde cerca de 10-15 segundos após ligar o ESP32
- Verifique se a senha está correta: `12345678`
- Reinicie o ESP32

### A contagem não está funcionando

- Verifique a conexão do sensor no GPIO 27
- Teste o sensor com um multímetro
- Ajuste o tempo de debounce se necessário

### Upload do SPIFFS falha

- Certifique-se de que a pasta `data` existe
- Verifique se os arquivos HTML, CSS e JS estão na pasta `data`
- Tente fazer o upload novamente

## 📊 Monitor Serial

Para acompanhar os logs do sistema, abra o monitor serial no PlatformIO:

```
Pressione Ctrl+Shift+P → PlatformIO: Serial Monitor
```

Velocidade configurada: **115200 baud**

## 🎓 Aprendizados

Este projeto proporcionou experiência prática em:

- Programação de microcontroladores ESP32
- Desenvolvimento de servidores web embarcados
- Uso de interrupções por hardware
- Integração de sensores ópticos
- Desenvolvimento de interfaces web responsivas
- Uso do PlatformIO para projetos IoT

## 📝 Licença

Este projeto foi desenvolvido para fins educacionais durante o curso de Mecatrônica.

## 👤 Autor

Desenvolvido como projeto prático do curso de Mecatrônica.

---

**Nota:** Este é um projeto educacional. Para uso industrial, considere adicionar recursos de segurança adicionais e validação mais rigorosa.
