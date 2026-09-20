#include "vip.h"

// ================= CLOCK =================
void vip_tool_clock() {
    while (true) {
        if (vip_esc_pressed()) return;
        u8g2.clearBuffer();
        vip_theme_draw_header("Clock");

        unsigned long s = millis() / 1000;
        char buf[16];
        sprintf(buf, "%02lu:%02lu:%02lu", (s / 3600) % 24, (s / 60) % 60, s % 60);
        u8g2.setFont(u8g2_font_logisoso16_tf);
        u8g2.drawStr(20, 40, buf);

        u8g2.setFont(u8g2_font_6x10_tf);
        char bat[16];
        sprintf(bat, "Bat: %d%%", vip_battery_percent());
        u8g2.drawStr(2, 60, bat);

        char rssi[16];
        sprintf(rssi, "RSSI: %d", WiFi.RSSI());
        u8g2.drawStr(70, 60, rssi);

        u8g2.sendBuffer();
        delay(500);
    }
}

// ================= SYSTEM MONITOR =================
void vip_tool_system_monitor() {
    while (true) {
        if (vip_esc_pressed()) return;
        u8g2.clearBuffer();
        vip_theme_draw_header("System");

        u8g2.setFont(u8g2_font_6x10_tf);

        char buf[32];
        sprintf(buf, "Free heap: %u B", ESP.getFreeHeap());
        u8g2.drawStr(0, 18, buf);

        sprintf(buf, "Flash: %u MB", ESP.getFlashChipSize() / (1024 * 1024));
        u8g2.drawStr(0, 28, buf);

        sprintf(buf, "Uptime: %lu s", millis() / 1000);
        u8g2.drawStr(0, 38, buf);

        sprintf(buf, "CPU: %u MHz", getCpuFrequencyMhz());
        u8g2.drawStr(0, 48, buf);

        sprintf(buf, "SDK: %s", ESP.getSdkVersion());
        u8g2.drawStr(0, 58, buf);

        u8g2.sendBuffer();
        delay(1000);
    }
}

// ================= FILE MANAGER =================
void vip_tool_file_manager() {
    File root = LittleFS.open("/");
    if (!root) {
        u8g2.clearBuffer();
        u8g2.drawStr(0, 20, "FS not mounted");
        u8g2.sendBuffer();
        vip_delay(1500);
        return;
    }

    File f = root.openNextFile();
    while (true) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        vip_theme_draw_header("Files (LittleFS)");
        u8g2.setFont(u8g2_font_6x10_tf);

        int y = 18;
        for (int i = 0; i < 5; i++) {
            if (!f) break;
            char buf[32];
            snprintf(buf, sizeof(buf), "%-14s %5u", f.name(), f.size());
            u8g2.drawStr(0, y, buf);
            y += 9;
            f = root.openNextFile();
        }
        if (!f) {
            u8g2.drawStr(0, y, "-- End of files --");
        }
        u8g2.sendBuffer();
        delay(100);
    }
}

// ================= QR GENERATOR =================
// Cần cài thư viện "qrcode" hoặc dùng thư viện có sẵn của Bruce
#ifdef VIP_QR_ENABLED
#include <qrcode.h>
void vip_tool_qr_generator() {
    const char* text = "https://github.com/pr3y/Bruce";
    QRCode qrcode;
    uint8_t qrcodeBytes[512];

    if (qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, text) != 0) {
        u8g2.clearBuffer();
        u8g2.drawStr(0, 20, "QR Error");
        u8g2.sendBuffer();
        vip_delay(1500);
        return;
    }

    u8g2.clearBuffer();
    int scale = 64 / qrcode.size;
    if (scale < 1) scale = 1;
    int offset = (128 - qrcode.size * scale) / 2;

    for (int y = 0; y < qrcode.size; y++) {
        for (int x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y))
                u8g2.drawBox(offset + x * scale, y * scale, scale, scale);
        }
    }
    u8g2.sendBuffer();
    vip_delay(4000);
}
#else
void vip_tool_qr_generator() {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 20, "QR lib not installed");
    u8g2.drawStr(0, 35, "Enable VIP_QR_ENABLED");
    u8g2.sendBuffer();
    vip_delay(2000);
}
#endif

// ================= WIFI SCANNER (CHỈ QUÉT) =================
void vip_tool_wifi_scanner() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(200);

    int n = WiFi.scanNetworks();
    if (n <= 0) {
        u8g2.clearBuffer();
        u8g2.drawStr(0, 20, "No WiFi found");
        u8g2.sendBuffer();
        vip_delay(1500);
        return;
    }

    int sel = 0;
    while (true) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        vip_theme_draw_header("WiFi Scan");
        u8g2.setFont(u8g2_font_5x7_tf);

        int y = 14;
        for (int i = 0; i < n; i++) {
            if (y > 60) break;
            char buf[32];
            snprintf(buf, sizeof(buf), "%-18s %d", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
            if (i == sel) {
                u8g2.drawBox(0, y - 6, 128, 7);
                u8g2.setDrawColor(0);
            }
            u8g2.drawStr(0, y, buf);
            u8g2.setDrawColor(1);
            y += 8;
        }
        u8g2.sendBuffer();

        // Chỉ để xem, không có tấn công.
        delay(200);
    }
    WiFi.scanDelete();
}

// ================= I2C SCANNER =================
#include <Wire.h>
void vip_tool_i2c_scanner() {
    Wire.begin();
    u8g2.clearBuffer();
    vip_theme_draw_header("I2C Scanner");
    u8g2.setFont(u8g2_font_6x10_tf);
    int y = 18;
    int found = 0;

    for (uint8_t addr = 1; addr < 127; addr++) {
        if (vip_esc_pressed()) return;
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            char buf[16];
            sprintf(buf, "Found: 0x%02X", addr);
            if (y < 60) u8g2.drawStr(0, y, buf);
            y += 9;
            found++;
        }
        if (y > 62) {
            u8g2.sendBuffer();
            vip_delay(800);
            u8g2.clearBuffer();
            vip_theme_draw_header("I2C Scanner");
            u8g2.setFont(u8g2_font_6x10_tf);
            y = 18;
        }
    }
    if (found == 0) u8g2.drawStr(0, 30, "No I2C device");
    u8g2.sendBuffer();
    vip_delay(2500);
}

// ================= GPIO TEST =================
void vip_tool_gpio_test() {
    // Cảnh báo: chỉ dùng để test LED/nút, KHÔNG nối tải lớn.
    u8g2.clearBuffer();
    vip_theme_draw_header("GPIO Test");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 20, "Toggle LED pin...");
    u8g2.sendBuffer();

    pinMode(VIP_LED_PIN, OUTPUT);
    for (int i = 0; i < 6; i++) {
        if (vip_esc_pressed()) break;
        digitalWrite(VIP_LED_PIN, HIGH);
        delay(200);
        digitalWrite(VIP_LED_PIN, LOW);
        delay(200);
    }
}

// ================= ABOUT =================
void vip_tool_about() {
    u8g2.clearBuffer();
    vip_theme_draw_header("About");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 18, "Bruce VIP Addon");
    u8g2.drawStr(0, 28, "Version: " VIP_VERSION);
    u8g2.drawStr(0, 38, "Chip: ESP32-C3");
    u8g2.drawStr(0, 48, "Author: DIY");
    u8g2.drawStr(0, 58, "Esc to exit");
    u8g2.sendBuffer();
    while (!vip_esc_pressed()) delay(50);
}