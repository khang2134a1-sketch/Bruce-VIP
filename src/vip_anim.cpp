#include "vip.h"

// ================= BOOT =================
void vip_anim_boot() {
    // Frame 1: logo
    u8g2.clearBuffer();
    u8g2.drawBox(20, 10, 88, 44);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.drawStr(40, 48, "B");
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();
    vip_delay(400);

    // Frame 2: thanh sáng
    for (int x = 0; x < 128; x += 4) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        u8g2.drawBox(20, 10, 88, 44);
        u8g2.setDrawColor(0);
        u8g2.drawStr(40, 48, "B");
        u8g2.setDrawColor(1);
        u8g2.drawBox(x, 10, 2, 44);
        u8g2.sendBuffer();
        delay(12);
    }

    // Frame 3: tên
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(30, 30, "BRUCE");
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(15, 50, "VIP Edition v" VIP_VERSION);
    u8g2.sendBuffer();
    vip_delay(900);
}

// ================= RADAR =================
void vip_anim_radar(uint32_t dur) {
    uint32_t start = millis();
    float angle = 0;
    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        u8g2.drawCircle(64, 32, 28);
        u8g2.drawCircle(64, 32, 18);
        u8g2.drawCircle(64, 32, 8);
        u8g2.drawDisc(64, 32, 2);

        int x = 64 + 28 * cos(angle);
        int y = 32 + 28 * sin(angle);
        u8g2.drawLine(64, 32, x, y);

        // Mục tiêu giả lập
        u8g2.drawDisc(64 + 15, 32 - 10, 2);
        u8g2.drawDisc(64 - 12, 32 + 15, 2);

        u8g2.sendBuffer();
        angle += 0.15f;
        if (angle > 2 * PI) angle -= 2 * PI;
        delay(30);
    }
}

// ================= MATRIX =================
void vip_anim_matrix(uint32_t dur) {
    uint32_t start = millis();
    const char chars[] = "01BRUCEVIP";
    uint8_t drops[22];
    for (int i = 0; i < 22; i++) drops[i] = random(0, 64);

    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_5x7_tf);

        for (int col = 0; col < 22; col++) {
            int x = col * 6;
            int y = drops[col];
            char c = chars[random(0, sizeof(chars) - 1)];
            u8g2.drawChar(x, y, c);
            drops[col] += 4;
            if (drops[col] > 70) drops[col] = random(0, 10);
        }
        u8g2.sendBuffer();
        delay(60);
    }
}

// ================= WAVE =================
void vip_anim_wave(uint32_t dur) {
    uint32_t start = millis();
    float phase = 0;
    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        for (int x = 0; x < 128; x++) {
            int y = 32 + 20 * sin((x / 12.0) + phase);
            u8g2.drawPixel(x, y);
            u8g2.drawPixel(x, y + 1);
        }
        phase += 0.2f;
        u8g2.sendBuffer();
        delay(30);
    }
}

// ================= PULSE =================
void vip_anim_pulse(uint32_t dur) {
    uint32_t start = millis();
    int r = 0, dir = 1;
    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        u8g2.drawDisc(64, 32, r);
        u8g2.drawCircle(64, 32, r + 3);
        r += dir;
        if (r > 30 || r < 0) dir = -dir;
        u8g2.sendBuffer();
        delay(20);
    }
}

// ================= SNAKE =================
void vip_anim_snake(uint32_t dur) {
    uint32_t start = millis();
    const int maxLen = 20;
    int sx[maxLen], sy[maxLen];
    for (int i = 0; i < maxLen; i++) { sx[i] = 64; sy[i] = 32; }

    int dir = 0;   // 0R 1D 2L 3U
    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        for (int i = maxLen - 1; i > 0; i--) {
            sx[i] = sx[i - 1];
            sy[i] = sy[i - 1];
        }
        if (random(0, 10) == 0) dir = random(0, 4);
        switch (dir) {
            case 0: sx[0] += 2; break;
            case 1: sy[0] += 2; break;
            case 2: sx[0] -= 2; break;
            case 3: sy[0] -= 2; break;
        }
        if (sx[0] < 0) sx[0] = 127;
        if (sx[0] > 127) sx[0] = 0;
        if (sy[0] < 0) sy[0] = 63;
        if (sy[0] > 63) sy[0] = 0;

        u8g2.clearBuffer();
        for (int i = 0; i < maxLen; i++) {
            u8g2.drawDisc(sx[i], sy[i], (i == 0) ? 2 : 1);
        }
        u8g2.sendBuffer();
        delay(40);
    }
}

// ================= STARFIELD =================
void vip_anim_starfield(uint32_t dur) {
    uint32_t start = millis();
    const int N = 40;
    int px[N], py[N], pz[N];
    for (int i = 0; i < N; i++) {
        px[i] = random(-64, 64);
        py[i] = random(-32, 32);
        pz[i] = random(1, 64);
    }
    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        for (int i = 0; i < N; i++) {
            pz[i] -= 2;
            if (pz[i] <= 0) {
                px[i] = random(-64, 64);
                py[i] = random(-32, 32);
                pz[i] = 64;
            }
            int sx = 64 + (px[i] * 32) / pz[i];
            int sy = 32 + (py[i] * 32) / pz[i];
            if (sx >= 0 && sx < 128 && sy >= 0 && sy < 64)
                u8g2.drawPixel(sx, sy);
        }
        u8g2.sendBuffer();
        delay(40);
    }
}

// ================= PLASMA =================
void vip_anim_plasma(uint32_t dur) {
    uint32_t start = millis();
    float t = 0;
    while (millis() - start < dur) {
        if (vip_esc_pressed()) break;
        u8g2.clearBuffer();
        for (int y = 0; y < 64; y += 2) {
            for (int x = 0; x < 128; x += 2) {
                float v = sin(x / 16.0 + t) + sin(y / 8.0 + t) + sin((x + y) / 16.0 + t);
                if (v > 0.5) u8g2.drawPixel(x, y);
            }
        }
        u8g2.sendBuffer();
        t += 0.15f;
        delay(40);
    }
}

// ================= DISPATCHER =================
void vip_anim_play(VipAnimType type, uint32_t duration_ms) {
    switch (type) {
        case ANIM_BOOT:      vip_anim_boot();                break;
        case ANIM_RADAR:     vip_anim_radar(duration_ms);    break;
        case ANIM_MATRIX:    vip_anim_matrix(duration_ms);   break;
        case ANIM_WAVE:      vip_anim_wave(duration_ms);     break;
        case ANIM_PULSE:     vip_anim_pulse(duration_ms);    break;
        case ANIM_SNAKE:     vip_anim_snake(duration_ms);    break;
        case ANIM_STARFIELD: vip_anim_starfield(duration_ms);break;
        case ANIM_PLASMA:    vip_anim_plasma(duration_ms);   break;
    }
}