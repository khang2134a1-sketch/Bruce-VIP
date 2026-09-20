#include "vip.h"

static VipTheme theme = THEME_DARK;

void vip_theme_set(VipTheme t) { theme = t; }
VipTheme vip_theme_get() { return theme; }

void vip_theme_draw_header(const char* title) {
    switch (theme) {
        case THEME_DARK:
            u8g2.drawBox(0, 0, 128, 11);
            u8g2.setDrawColor(0);
            break;
        case THEME_LIGHT:
            u8g2.drawFrame(0, 0, 128, 11);
            break;
        case THEME_RETRO:
            u8g2.drawBox(0, 0, 128, 11);
            u8g2.setDrawColor(0);
            break;
        case THEME_NEON:
            u8g2.drawBox(0, 0, 128, 11);
            u8g2.setDrawColor(0);
            break;
        case THEME_MATRIX:
            u8g2.drawBox(0, 0, 128, 11);
            u8g2.setDrawColor(0);
            break;
    }
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(2, 9, title);
    u8g2.setDrawColor(1);
}

void vip_theme_draw_footer(const char* text) {
    u8g2.setFont(u8g2_font_5x7_tf);
    u8g2.drawStr(0, 63, text);
}

void vip_theme_draw_progress(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    int w = (percent * 124) / 100;
    u8g2.drawFrame(2, 28, 124, 8);
    u8g2.drawBox(2, 28, w, 8);
}