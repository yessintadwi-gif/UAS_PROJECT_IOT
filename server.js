const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");
const { Server } = require("socket.io");
const http = require("http");
const express = require("express");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.json());

app.get("/", (req, res) => {
    res.sendFile(__dirname + "/views/index.html");
});

// Indikator Koneksi Browser (Socket.io)
io.on("connection", (socket) => {
    console.log("🖥️  Client connected to Dashboard Web...");
    socket.on("disconnect", () => {
        console.log("🖥️  Client disconnected from Dashboard Web");
    });
});

// PERBAIKAN: Mengubah port listen ke 9000 agar aman dari tabrakan port lain
server.listen(7000, () => {
    console.log("🚀 Server Smart Home Pro running on port 7000!");
});

// Konektifitas Serial Arduino
const port = new SerialPort({
    path: "COM5", // Sesuaikan nomor COM yang sukses saat upload di Arduino IDE (COM3, COM4, dst)
    baudRate: 9600, // Kecepatan 9600 bps sesuai sketsa Arduino baru
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

// INDIKATOR STATUS CONNECT & DISCONNECT KABEL USB ARDUINO
port.on("open", () => {
    console.log("✅ Arduino Connected & Ready on " + port.settings.path);
});

port.on("close", () => {
    console.log("❌ Arduino Disconnected / Port Closed!");
});

port.on("error", (err) => {
    console.log("⚠️  Arduino Serial Error: ", err.message);
});

// 1. TERIMA DATA LOG DAN REAL-TIME SENSOR DARI ARDUINO KE DASHBOARD WEB VIA SOCKET
parser.on("data", (result) => {
    const cleanData = result.toString().trim();
    
    // Tampilkan log data murni dari Arduino di terminal VS Code
    console.log("Data dari Arduino -> ", cleanData);

    // Meneruskan seluruh data pemantauan cuaca dan sampah secara real-time ke web
    if (cleanData.includes("Live") || cleanData.includes("[CUACA]") || cleanData.includes("[SAMPAH]")) {
        io.emit("statusSistem", { message: cleanData });
    }
});

// 2. KONTROL UTAMA: API Endpoint Menerima Klik Tombol Web (1 s.d 5) Untuk Dikirim Ke Arduino via USB
app.post("/arduinoApi", (req, res) => {
    const data = req.body.data; // Menerima angka string ("1", "2", "3", "4", "5")

    port.write(data, (err) => {
        if (err) {
            console.log('Error saat mengirim perintah via Serial: ', err);
            return res.status(500).json({ error: "Gagal mentransmisikan data ke papan Arduino!" });
        }
        console.log("Web Dashboard mengirim perintah -> Karakter [" + data + "] sukses diteruskan ke Arduino.");
        res.json({ status: "success", message: "Perintah " + data + " berhasil dikirim." });
    });
});