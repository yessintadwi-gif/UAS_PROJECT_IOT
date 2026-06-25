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

// =================================================
// DEKLARASI TEMPAT SAMPAH PINTAR (PIN 7, 8, 9, A0)
// =================================================
Servo servoTrash;
const int pin ServoTrash = 7; // Servo Sampah -> Pin D7
const int trigpin = 8;        // Ultrasonik Trig -> Pin D8
const int echopin = 9;        // Ultrasonik Echo -> Pin D9
const int soilPIN = A0;       // Sensor Kelembaban -> Pin A0

long duration;
int distance;
int soil;
int fsoil;

int maxDryValue = 10;     // Batas Kelembaban sampah (> 10% = basah)
int Ultra_Distance = 15;  // Jarak deteksi objek (15 cm)

unsigned long waktuMulaiTrash = 0;
unsigned long waktuLamaUltrasonik = 0;
bool trashSedangMemisah = false;

// =================================================
// DEKLARASI KONTROL 3 LAMPU LED (PIN 10, 11, 12)
// =================================================
const int led_1 = 10; // LED 1 ➔ Pin D10
const int led_2 = 11; // LED 2 ➔ Pin D11
const int led_3 = 12; // LED 3 ➔ Pin D12

// Variabel pembatas waktu cetak data di serial monitor
unsigned long waktuLamaCetak = 0;

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

  // --- SETUP SMART TRASH ---
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, OUTPUT);
  servoTrash.attach(pinServoTrash);
  servoTrash,write(90); //Posisi standby tengah

  // --- SETUP LAMPU ---
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);

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

// TEMPAT SAMPAH PINTAR (MEMISAH)
if (millis() - waktuLamaUltrasonik >= 200 && !trashSedangMemisah)
{
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  duration = PulseIn(echoPin, HIGH, 26000);
  distance = duration *0.034 / 2;
  waktuLamaUltrasonik = millis();

  if (distance < Ultra_Distance && distance > 1)
  {
    delay(75);
    soil = analogRead(soilPin);
    soil = constrain (soil, 485, 1023);
    fsoil = map(soil, 485, 1023, 100, 0);

    if (fsoil > maxDryValue)
    {
      Serial.print("-> [SAMPAH] Terdeteksi BASAH(");
      Serial.print(fsoil);
      Serial.println("%).Memilah ke KIRI (10 derajat).");
      servoTrash.write(10)
    }
    else
    {
      Serial.print("-> [SAMPAH] Terdeteksi KERING(");
      Serial.print(fsoil);
      Serial.println("%). Memilah ke KANAN (170 derajat).");
      servoTrash.write (170);
    }

    waktuMulaiTrash = millis();
    trashSedangMemisah = true;
  }
}

if (trashSedangMemisah && (millis() - waktuMulaiTrash)) {
  servoTrash.write(90);
  trashSedangMemisah = false;
  Serial.println("-> [SAMPAH] Selesai. Servo Trash Standby (90 derajat).");
}
  
// SERIAL KONTROL 3 LAMPU (LOGIKA RESERVED)
if (Serial.available() > 0)
{
  char perintah = Serial.read();

  // PERBAIKAN LOGIKA: 4 = MENYALA SEMUA
  if (perintah == '4') {
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
    digitalWrite(led_3, HIGH);
    Serial.printIn("➔ [LAMPU] PERINTAH 4: SEMUA LAMPU MENYALA");
  }
  // PERBAIKAN LOGIKA: 5 = MATI SEMUA
  else if (perintah == '5') {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
    Serial.printIn("➔ [LAMPU] PERINTAH 5: SEMUA LAMPU MATI");
  }
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