#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Sadece NEC protokolünü kullanarak hafıza tasarrufu sağlıyoruz
#define DECODE_NEC 
#include <IRremote.hpp>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- AYARLAR ---
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 10
#define RECV_PIN 2

// --- DEĞİŞKENLER ---
int currentCols = 0;
unsigned long lastDataTime = 0;
const int timeoutLimit = 3000;
bool runningStats = false;
bool lcdAcik = true;
bool activeTurks = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MD_Parola mp = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_MAX72XX *mx; // mx'i işaretçi yaparak Parola'nın içindekini kullanacağız

void setup() {
  IrReceiver.begin(RECV_PIN);
  Serial.begin(9600);
  
  lcd.init();
  lcd.noBacklight();
  lcdAcik = false;

  mp.begin();
  mx = mp.getGraphicObject(); // Parola'nın kullandığı alt kütüphane nesnesini alıyoruz
  mx->control(MD_MAX72XX::INTENSITY, 1);
  mx->clear();
  
  mp.displayScroll("#ACTIVETURKS", PA_CENTER, PA_SCROLL_LEFT, 30);
}

void loop() {
  // --- IR KUMANDA KONTROLÜ ---
  if (IrReceiver.decode()) {
    uint32_t basilanKod = IrReceiver.decodedIRData.decodedRawData;
    
    if (basilanKod == 0xBA45FF00) { activeTurks = true; }
    if (basilanKod == 0xB946FF00) { activeTurks = false; }
    
    // LCD Backlight toggle
    if (basilanKod == 0xE916FF00) {
      lcdAcik = !lcdAcik;
      lcdAcik ? lcd.backlight() : lcd.noBacklight();
    }
    
    // Dot Matrix Shutdown toggle
    if (basilanKod == 0xF20DFF00) {
      static bool dotKapali = false;
      dotKapali = !dotKapali;
      mx->control(MD_MAX72XX::SHUTDOWN, dotKapali ? MD_MAX72XX::ON : MD_MAX72XX::OFF);
    }
    IrReceiver.resume();
  }

  // --- SERIAL VERİ OKUMA ---
  if (Serial.available() > 0) {
    int cpuVal = Serial.parseInt();
    int ramVal = Serial.parseInt();
    int gpuVal = Serial.parseInt();

    if (Serial.read() == '\n' || true) {
      lastDataTime = millis();
      if (!runningStats) {
        mx->clear();
        lcd.clear();
        if(lcdAcik) lcd.backlight();
        runningStats = true;
      }

      // 1. Satır: CPU ve RAM (Yan yana)
      lcd.setCursor(0, 0);
      lcd.print(F("CPU:%")); 
      lcd.print(cpuVal);
      lcd.print(F("  ")); // Eski basamaktan kalan izi temizlemek için boşluk

      lcd.setCursor(9, 0); // RAM'i 9. karakterden başlatıyoruz
      lcd.print(F("RAM:%")); 
      lcd.print(ramVal);
      lcd.print(F(" "));

      // 2. Satır: GPU
      lcd.setCursor(0, 1);
      lcd.print(F("GPU:%")); 
      lcd.print(gpuVal);
      lcd.print(F("    "));
      updateBar(gpuVal);
    }
  }

  // --- ZAMAN AŞIMI VE ANİMASYONLAR ---
  if (millis() - lastDataTime > timeoutLimit) {
    if (runningStats) {
      mx->clear();
      lcd.clear();
      lcd.print(F("#ACTIVETURKS"));
      delay(1000);
      lcd.noBacklight();
      lcdAcik = false;
      runningStats = false;
    }

    if (activeTurks) {
      if (mp.displayAnimate()) mp.displayReset();
    } else {
      matrixRain();
    }
  }
}

void updateBar(int percent) {
  int targetCols = map(percent, 0, 100, 0, 32);
  while (currentCols != targetCols) {
    if (currentCols < targetCols) {
      mx->setColumn(currentCols, 0xFF);
      currentCols++;
    } else {
      currentCols--;
      mx->setColumn(currentCols, 0x00);
    }
    delay(20);
  }
}

void matrixRain() {
  static uint8_t rain[32];
  static unsigned long lastRainMove = 0;
  if (millis() - lastRainMove < 80) return;
  lastRainMove = millis();

  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  mx->clear();
  for (int x = 0; x < 32; x++) {
    if (rain[x] > 0) {
      mx->setPoint(rain[x] - 1, x, true);
      if (rain[x] < 8) rain[x]++; else rain[x] = 0;
    } else {
      if (random(0, 30) == 1) rain[x] = 1;
    }
  }
  mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}
