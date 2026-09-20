#ifndef VIP_H
#define VIP_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <Preferences.h>
#include <FS.h>

// ============ CẤU HÌNH ============
#define VIP_VERSION       "1.0.0"
#define VIP_BUZZER_PIN    3
#define VIP_LED_PIN       2
#define VIP_ESC_PIN       0

// ============ ENUM ============
enum VipAnimType {
    ANIM_BOOT,
    ANIM_RADAR,
    ANIM_MATRIX,
    ANIM_WAVE,
    ANIM_PULSE,
    ANIM_SNAKE,
    ANIM_STARFIELD,
    ANIM_PLASMA,
};

enum VipTheme {
    THEME_DARK,
    THEME_LIGHT,
    THEME_RETRO,
    THEME_NEON,
    THEME_MATRIX,
};

// ============ CORE ============
void vip_init();
void vip_loop();
void vip_open_menu();

// ============ ANIMATION ============
void vip_anim_play(VipAnimType type, uint32_t duration_ms = 5000);
void vip_anim_boot();
void vip_anim_radar(uint32_t dur);
void vip_anim_matrix(uint32_t dur);
void vip_anim_wave(uint32_t dur);
void vip_anim_pulse(uint32_t dur);
void vip_anim_snake(uint32_t dur);
void vip_anim_starfield(uint32_t dur);
void vip_anim_plasma(uint32_t dur);

// ============ TOOLS ============
void vip_tool_clock();
void vip_tool_system_monitor();
void vip_tool_file_manager();
void vip_tool_qr_generator();
void vip_tool_wifi_scanner();      // Chỉ quét, không tấn công
void vip_tool_i2c_scanner();
void vip_tool_gpio_test();
void vip_tool_battery_info();
void vip_tool_about();

// ============ THEME ============
void vip_theme_set(VipTheme t);
VipTheme vip_theme_get();
void vip_theme_draw_header(const char* title);
void vip_theme_draw_footer(const char* text);
void vip_theme_draw_progress(int percent);

// ============ HELPER ============
bool vip_esc_pressed();
void vip_delay(uint32_t ms);
int  vip_battery_percent();
void vip_beep(int freq, int ms);
void vip_jingle();
void vip_save_config();
void vip_load_config();

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

#endif