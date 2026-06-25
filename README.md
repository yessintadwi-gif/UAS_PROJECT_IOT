# SMART HOME (Project Akhir) 🏠🤖

Repositori ini berisi kode sumber untuk proyek **SMART HOME**. Pengembangan proyek ini dibagi menjadi dua komponen utama yang dikelola melalui percabangan (*branching*) Git secara terpisah untuk menjaga kerapian struktur kode.

---

## 📌 Penting: Struktur Percabangan (Branching)
Repositori ini memiliki dua branch utama dengan peran dan fungsi yang sepenuhnya terpisah:

### 1. 🌐 Branch: master (KHUSUS WEB DASHBOARD)
* **Fungsi:** Menyimpan seluruh kodingan aplikasi web berbasis **Node.js & Express**. 
* **Tujuan:** Menampilkan dashboard monitoring dari sistem Smart Home. 
* **Pengelola:** **Yessinta Dwi Rahayu** (Master/Owner).
* *Catatan: Di branch `master` ini Anda hanya akan menemukan berkas-berkas yang berkaitan dengan kebutuhan website dashboard.*

### 2. 🔌 Branch: main (KHUSUS HARDWARE & ARDUINO)
* **Fungsi:** Menyimpan seluruh kodingan sketsa (*sketch*), pustaka, dan konfigurasi perangkat keras IoT berbasis mikrokontroler Arduino/ESP.
* **Pengelola:** **Khansa Sahara** dan **Alyssa Deffra Putri Indaru**.
* *Catatan: Jika Anda mencari kodingan hardware/Arduino lengkap, Anda wajib berpindah ke branch main.*

---

## 📂 Struktur Berkas Branch 'master' (Web Dashboard)

Berikut adalah struktur file penyusun web dashboard yang berada di dalam branch `master` sesuai dengan repositori saat ini:

* **`server.js`** — Berkas utama backend Node.js / Express server untuk menangani logika web.
* **`views/`** — Folder tempat menyimpan file antarmuka (frontend) dashboard web (seperti `index.html`).
* **`smart_home_iot.zip`** — Berkas arsip kompresi yang berisi cadangan data proyek atau komponen terkait IoT Smart Home.
* **`package.json` & `package-lock.json`** — File konfigurasi dependensi, versi library, dan manifes proyek Node.js.
* **`node_modules/`** — Folder otomatis tempat menyimpan library pihak ketiga yang diinstal via npm.

---

## 📥 Cara Mengambil Kodingan (Clone & Switch Branch)

Gunakan perintah di bawah ini pada terminal Git Bash untuk mengunduh proyek dan beralih ke branch yang diinginkan:

### A. Proses Clone dan Sinkronisasi
```bash
$ git clone [https://github.com/yessintadwi-gif/UAS_PROJECT_IOT.git](https://github.com/yessintadwi-gif/UAS_PROJECT_IOT.git)
$ cd UAS_PROJECT_IOT
```

### B. Sinkronisasi Branch
```bash
$ git fetch origin
```

### C. Berpindah ke Komponen Web Dashboard (Branch ini)
```bash
$ git checkout master
```

### D. Berpindah ke Komponen Hardware / Arduino (Branch Main)
```bash
$ git checkout main
```

---

## 🚀 Tahapan Menjalankan Aplikasi Web Dashboard (Pada Branch 'master')

Pastikan Anda telah berada di dalam branch master sebelum mengeksekusi langkah-langkah berikut:

### Langkah 1: Pastikan posisi branch sudah benar (master)
```bash
$ git checkout master
```

### Langkah 2: Instalasi seluruh library/dependensi proyek website
```bash
$ npm install
```

### Langkah 3: Jalankan local server Node.js
```bash
$ node server.js
```

### Langkah 4: Buka web browser, lalu akses sistem melalui URL berikut
```text
http://localhost:7000
```
