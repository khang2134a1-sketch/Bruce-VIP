#include "vip.h"

static Preferences  vip_prefs;
static bool         vip_ready = false;
static VipTheme     current_theme = THEME_DARK;
static uint8_t      vip_brightness = 255;

// ================= INIT =================
void vip_init() {
    if (vip_ready) return;

    pinMode(VIP_BUZZER_PIN, OUTPUT);
    pinMode(VIP_LED_PIN, OUTPUT);
    pinMode(VIP_ESC_PIN, INPUT_PULLUP);
    digitalWrite(VIP_LED_PIN, LOW);

    randomSeed(esp_random());
    vip_prefs.begin("vip", false);

    vip_load_config();

    // Khởi động: chơi nhạc + animation
    vip_jingle();
    vip_anim_boot();

    vip_ready = true;
}

void vip_loop() {
    // Chỗ trống cho tác vụ nền (ví dụ: cập nhật clock)
}

// ================= CONFIG =================
void vip_load_config() {
    current_theme = (VipTheme)vip_prefs.getUChar("theme", THEME_DARK);
    vip_brightness = vip_prefs.getUChar("bright", 255);
}

void vip_save_config() {
    vip_prefs.putUChar("theme", (uint8_t)current_theme);
    vip_prefs.putUChar("bright", vip_brightness);
}

// ================= MENU =================
void vip_open_menu() {
    const char* items[] = {
        "Animations",
        "Clock",
        "System Info",
        "Files",
        "QR Code",
        "WiFi Scan",
        "I2C Scan",
        "GPIO Test",
        "Themes",
        "About",
        "Exit"
    };
    const int n = sizeof(items) / sizeof(items[0]);
    int sel = 0;

    while (true) {
        u8g2.clearBuffer();
        vip_theme_draw_header("VIP Zone");

        for (int i = 0; i < n; i++) {
            int y = 16 + i * 8;
            if (y > 62) break;
            if (i == sel) {
                u8g2.drawBox(0, y - 7, 128, 8);
                u8g2.setDrawColor(0);
            }
            u8g2.drawStr(4, y, items[i]);
            u8g2.setDrawColor(1);
        }

        vip_theme_draw_footer("UP/DOWN/OK/ESC");
        u8g2.sendBuffer();

        // Đọc nút — thay bằng API nút thật của Bruce
        int key = vip_read_key();      // -1: ESC, 0: none, 1: OK, 2: UP, 3: DOWN
        if (key == -1) return;
        if (key == 2) sel = (sel - 1 + n) % n;
        if (key == 3) sel = (sel + 1) % n;
        if (key == 1) {
            switch (sel) {
                case 0: {
                    VipAnimType t = ANIM_RADAR;
                    vip_anim_play(t, 6000);
                    break;
                }
                case 1: vip_tool_clock();            break;
                case 2: vip_tool_system_monitor();   break;
                case 3: vip_tool_file_manager();     break;
                case 4: vip_tool_qr_generator();     break;
                case 5: vip_tool_wifi_scanner();     break;
                case 6: vip_tool_i2c_scanner();      break;
                case 7: vip_tool_gpio_test();        break;
                case 8: {
                    current_theme = (VipTheme)((current_theme + 1) % 5);
                    vip_save_config();
                    break;
                }
                case 9: vip_tool_about();            break;
                case 10: return;
            }
        }
        delay(80);
    }
}

// ================= STUB: Đọc nút =================
// Thay thế bằng API thật của Bruce, ví dụ: checkEscPress(), checkSelPress()...
int vip_read_key() {
    // -1: ESC, 0: none, 1: OK, 2: UP, 3: DOWN
    // Ở đây chỉ là stub minh họa.
    static unsigned long last = 0;
    if (millis() - last < 100) return 0;
    last = millis();

    // Bạn cần map các chân nút thực tế ở đây.
    // Ví dụ: nếu ESC (GPIO 0) được nhấn:
    if (digitalRead(VIP_ESC_PIN) == LOW) return -1;
    return 0;
}

// ================= HELPER =================
bool vip_esc_pressed() {
    return digitalRead(VIP_ESC_PIN) == LOW;
}

void vip_delay(uint32_t ms) {
    uint32_t start = millis();
    while (millis() - start < ms) {
        if (vip_esc_pressed()) return;
        delay(5);
    }
}

int vip_battery_percent() {
    // Nếu board có mạch đọc pin, thay bằng analogRead thực tế.
    // Bruce thường có getBatteryLevel() — dùng hàm đó.
    return 100;
}

void vip_beep(int freq, int ms) {
    if (VIP_BUZZER_PIN < 0) return;
    tone(VIP_BUZZER_PIN, freq, ms);
    delay(ms);
    noTone(VIP_BUZZER_PIN);
}

void vip_jingle() {
    vip_beep(523, 100);
    vip_beep(659, 100);
    vip_beep(784, 100);
    vip_beep(1047, 200);
}