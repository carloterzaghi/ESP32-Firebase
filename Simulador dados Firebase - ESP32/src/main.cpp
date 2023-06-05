#include <Arduino.h>
#include <Wifi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define WIFI_SSID "COLOCAR SSID DO WIFI"
#define WIFI_PASSWORD "COLOCAR SENHA DO WIFI"

#define API_KEY "COLOCAR API_KEY DO FIREBASE"
#define DATABASE_URL "COLOCAR DATABASE_URL DO FIREBASE"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
float voltage = 0.0;

void setup() {
  Serial.begin(115200);
  pinMode(21, INPUT_PULLUP);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
      delay(300);
      Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if(Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Sign Up OK");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

int n = 0;
void loop() {
  int buttonState = digitalRead(21);
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    if (buttonState == HIGH) {
    n = 0;
  }
  else{
    n = 1;
  }
  Serial.print("Estágio Botão: ");
  Serial.println(n);
  if (Firebase.RTDB.setInt(&fbdo, "Sensor/botao_estd", n)){

  }else{
    Serial.print("[DB] FAILED: ");
    Serial.println(fbdo.errorReason());
  }
  }
}
