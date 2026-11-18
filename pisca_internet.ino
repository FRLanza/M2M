/*
 * Aluno: Fabio Lanza   RA: 22003887
 * Disciplina: M2M
 *
 * Descricao:
 *   O ESP8266 conecta ao Wi-Fi. Enquanto houver acesso a internet,
 *   ele pisca o LED interno 3 vezes, aguarda 4 segundos e repete.
 *   Se o Wi-Fi cair, o LED fica apagado e ele tenta reconectar.
 */

#include <ESP8266WiFi.h>

// ---- Configuracao da rede (somente 2.4 GHz) ----
const char* SSID  = "SUA_REDE";
const char* SENHA = "SUA_SENHA";

// ---- LED interno ----
// No NodeMCU / Wemos D1 mini o LED_BUILTIN e o GPIO2 e acende em LOW
const int LED = LED_BUILTIN;
const int TEMPO_PISCA = 200;          // ms aceso e ms apagado
const unsigned long PAUSA = 4000;     // 4 s entre as sequencias

// Testa se ha internet de verdade (nao so Wi-Fi) abrindo uma conexao TCP
bool temInternet() {
  WiFiClient cliente;
  cliente.setTimeout(2000);
  bool ok = cliente.connect("www.google.com", 80);
  cliente.stop();
  return ok;
}

void conectarWiFi() {
  Serial.print("Conectando a ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, SENHA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wi-Fi conectado! IP: ");
  Serial.println(WiFi.localIP());
}

void piscar3x() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, LOW);    // acende
    delay(TEMPO_PISCA);
    digitalWrite(LED, HIGH);   // apaga
    delay(TEMPO_PISCA);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);     // comeca apagado
  conectarWiFi();
}

void loop() {
  // Se perdeu o Wi-Fi, apaga o LED e reconecta
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    Serial.println("Wi-Fi caiu, reconectando...");
    conectarWiFi();
    return;
  }

  if (temInternet()) {
    piscar3x();
    delay(PAUSA);
  } else {
    Serial.println("Wi-Fi OK, mas sem acesso a internet");
    digitalWrite(LED, HIGH);
    delay(2000);
  }
}
