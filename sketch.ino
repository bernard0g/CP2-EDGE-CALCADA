
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// Pinos
int ledPinVerd = 13; // Verde
int ledPinAm   = 12; // Amarelo
int ledPinVer  = 8;  // Vermelho
int buzzer     = 7;
int ldrpin     = A0;

unsigned long tempoAnterior = 0;
const long intervalo = 5000;

void setup() {
  pinMode(ledPinVerd, OUTPUT);
  pinMode(ledPinAm, OUTPUT);
  pinMode(ledPinVer, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
}

void tocarBuzzer(int duracao) {
  for (int i = 0; i < duracao / 400; i++) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
}

void loop() {
  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    float somaTemp = 0;
    float somaUmi = 0;
    int somaLuz = 0;

    for (int i = 0; i < 5; i++) {
      somaTemp += dht.readTemperature();
      somaUmi += dht.readHumidity();
      somaLuz += analogRead(ldrpin);
      delay(100);
    }

    float temp = somaTemp / 5.0;
    float umi = somaUmi / 5.0;
    int luz = somaLuz / 5;

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" | Umi: ");
    Serial.print(umi);
    Serial.print(" | Luz: ");
    Serial.println(luz);

    lcd.clear();

    // Luminosidade
    if (luz < 300) {
      digitalWrite(ledPinVerd, HIGH);
      digitalWrite(ledPinAm, LOW);
      digitalWrite(ledPinVer, LOW);
      tocarBuzzer(200);
      lcd.setCursor(0, 0);
      lcd.print("Ambiente escuro");
    } else if (luz >= 300 && luz <= 700) {
      digitalWrite(ledPinVerd, LOW);
      digitalWrite(ledPinAm, HIGH);
      digitalWrite(ledPinVer, LOW);
      tocarBuzzer(200);
      lcd.setCursor(0, 0);
      lcd.print("Ambiente com media");
      lcd.setCursor(0, 1);
      lcd.print("luz");
    } else {
      digitalWrite(ledPinVerd, LOW);
      digitalWrite(ledPinAm, LOW);
      digitalWrite(ledPinVer, HIGH);
      tocarBuzzer(2000);
      lcd.setCursor(0, 0);
      lcd.print("Ambiente muito");
      lcd.setCursor(0, 1);
      lcd.print("CLARO");
    }

    delay(5000);
    lcd.clear();

    // Umidade
    if (umi >= 50 && umi <= 70) {
      digitalWrite(ledPinVer, LOW);
      tocarBuzzer(200);
      lcd.setCursor(0, 0);
      lcd.print("Umidade OK");
    } else if (umi < 50) {
      digitalWrite(ledPinVer, HIGH);
      tocarBuzzer(2000);
      lcd.setCursor(0, 0);
      lcd.print("Umidade Baixa");
    } else {
      digitalWrite(ledPinVer, HIGH);
      tocarBuzzer(2000);
      lcd.setCursor(0, 0);
      lcd.print("Umidade Alta");
    }
    lcd.setCursor(0, 1);
    lcd.print("Umid: ");
    lcd.print(umi, 1);
    lcd.print("%");

    delay(5000);
    lcd.clear();

    // Temperatura
    if (temp >= 10 && temp <= 15) {
      digitalWrite(ledPinAm, LOW);
      tocarBuzzer(200);
      lcd.setCursor(0, 0);
      lcd.print("Temperatura OK");
    } else if (temp < 10) {
      digitalWrite(ledPinAm, HIGH);
      tocarBuzzer(2000);
      lcd.setCursor(0, 0);
      lcd.print("Temp. Baixa");
    } else {
      digitalWrite(ledPinAm, HIGH);
      tocarBuzzer(2000);
      lcd.setCursor(0, 0);
      lcd.print("Temp. Alta");
    }
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print("C");

    delay(5000);
    lcd.clear();
  }
}
