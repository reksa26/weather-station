//DHT
#include "DHT.h"
DHT dht11(0, DHT11);
int suhu, kelembaban;
//Rain
byte rain = 10;
int nilaiRain;
String kondisiCuaca;
//LCD
#include"LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);
//WiFi
#include"ESP8266WiFi.h"
const char* ssid = "BLNO-U01HNTMyRw";
const char* pass = "jarujaru25";

#include"ESP8266HTTPClient.h"
const char* server = "http:192.168.43.54";

void setup() {
  Serial.begin(9600);
  //DHT
  dht11.begin();
  //Rain
  pinMode(rain, INPUT);
  //LCD
  lcd.begin();
  //WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("Terhubung");
}

void loop() {
  //DHT
  suhu = dht11.readTemperature();
  kelembaban = dht11.readHumidity();

  //Rain
  nilaiRain = digitalRead(rain);
  if (nilaiRain == 1) {
    kondisiCuaca = "Cerah";
  } if (nilaiRain == 0) {
    kondisiCuaca = "Hujan";
  }

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print("  Kelembaban: ");
  Serial.print(kelembaban);
  Serial.print("  Kondisi Cuaca: ");
  Serial.println(kondisiCuaca);

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(suhu);
  lcd.print(" Humd: ");
  lcd.print(kelembaban);

  lcd.setCursor(0, 1);
  lcd.print("kondisi: ");
  lcd.print(kondisiCuaca);

  String URL = String("") + server + "/stasiunCuaca/input.php?suhu=" + suhu + "&kelembaban=" + kelembaban + "&kondisiCuaca=" + kondisiCuaca;
  Serial.println(URL);
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(URL);
    int httpCode = http.GET();
    if(httpCode > 0){
      String data = http.getString();
      Serial.println(data);
    }
    http.end();
  }
  delay(5000);
}
