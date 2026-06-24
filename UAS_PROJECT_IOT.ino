#include <Servo.h>

// ===============================
// JEMURAN OTOMATIS
// ===============================
Servo servoJemuran;
const int pinRelayHujan   = 2;
const int pinServoJemuran = 4;

int statusRelayHujan;
int statusJemuranTerakhir = -1;

// Konfigurasi sudut servo jemuran 90 derajat
int sudutTutup = 90;
int sudutBuka = 0;

unsigned long waktuLamaCuaca = 0; // Pembatas waktu baca sensor hujan

// ===============================
// SETUP
// ===============================

void setup() {
  Serial.begin(9600);

  // SETUP JEMURAN
  pinMode(pinRelayHujan, INPUT_PULLUP);
  servoJemuran.attach(pinServoJemuran);

  // Ambil data awal sebelum menentukan posisi
  statusRelayHujan = digitalRead(pinRelayHujan);
  if(statusRelayHujan == LOW) {
    servoJemuran.write(sudutTutup);
    statusJemuranTerakhir = 1;
  } else {
    servoJemuran.write(sudutBuka);
    statusJemuranTerakhir = 0;
  }
  delay(500);

  Serial.println("============================================");
  Serial.println("  SMART HOME SYSTEM - LIGHT REVERSED LOGIC  ");
  Serial.println("============================================");
}

// =================================
// LOOP
// =================================

void loop() {

  // JEMURAN OTOMATIS
  if (millis() - waktuLamaCuaca >= 1000) {
    statusRelayHujan = digitalRead(pinRelayHujan);

    if (statusRelayHujan == LOW)
    {
      if (statusJemuranTerakhir != 1) {
        servoJemuran.write(sudutTutup);
        statusJemuranTerakhir = 1;
        Serial.println("➔ [CUACA EVENT] Hujan Terdeteksi! Servo Jemuran LOCK TUTUP (90 derajat)");
      }
    }
    else
    {
      if (statusJemuranTerakhir != 0) {
        servoJemuran.write(sudutBuka);
        statusJemuranTerakhir = 0;
        Serial.println("➔ [CUACA EVENT] Cuaca Kering! Servo Jemuran LOCK BUKA (0 derajat)");
      }
    }
    waktuLamaCuaca = millis();
  }

  // ===============================
  // LIVE MONITOR DATA (SETIAP 2 DETIK)
  // ===============================

  if (millis() - waktuLamaCetak >= 2000) {
    Serial.print("📡 Live -> Relay Hujan: ");
    Serial.print(statusRelayHujan == LOW ? "BASAH/LOW" : "KERING/HIGH");
    Serial.print(" | Jarak Trash: ");
    Serial.Print(distance);
    Serial.println(" cm");
    waktuLamaCetak = millis();  
    }
}