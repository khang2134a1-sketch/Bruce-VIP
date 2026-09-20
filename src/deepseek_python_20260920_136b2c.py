#!/usr/bin/env python3
"""
Script tự động:
1. Clone Bruce (nếu chưa có)
2. Tạo thư mục src/vip và copy file
3. Build firmware cho ESP32-C3
4. Nạp vào thiết bị
"""

import os
import sys
import shutil
import subprocess

BRUCE_REPO = "https://github.com/pr3y/Bruce.git"
BRUCE_DIR  = "Bruce"
VIP_DIR    = os.path.join(BRUCE_DIR, "src", "vip")
ENV        = "esp32-c3-devkitm-1"

def run(cmd, cwd=None, check=True):
    print(f"$ {' '.join(cmd) if isinstance(cmd, list) else cmd}")
    return subprocess.run(cmd, cwd=cwd, check=check, shell=isinstance(cmd, str))

def clone_bruce():
    if os.path.exists(BRUCE_DIR):
        print(f"✅ {BRUCE_DIR} đã tồn tại, bỏ qua clone.")
        return
    print("📥 Đang clone Bruce...")
    run(["git", "clone", BRUCE_REPO])

def copy_vip_files():
    print("📁 Đang tạo thư mục VIP...")
    os.makedirs(VIP_DIR, exist_ok=True)

    # Danh sách file VIP cần copy (đặt cùng thư mục với script này)
    files = ["vip.h", "vip_core.cpp", "vip_anim.cpp",
             "vip_tools.cpp", "vip_theme.cpp"]

    for f in files:
        src = f
        dst = os.path.join(VIP_DIR, f)
        if not os.path.exists(src):
            print(f"⚠️  Không tìm thấy {src}, bỏ qua.")
            continue
        shutil.copy(src, dst)
        print(f"   ✅ Copy {src} -> {dst}")

def build():
    print("🔨 Đang build firmware...")
    run(["pio", "run", "-e", ENV], cwd=BRUCE_DIR)

def upload():
    ans = input("📤 Nạp vào thiết bị ngay? (y/n): ").strip().lower()
    if ans == "y":
        run(["pio", "run", "-e", ENV, "-t", "upload"], cwd=BRUCE_DIR)
        print("✅ Đã nạp xong. Mở Serial Monitor bằng:")
        print(f"   cd {BRUCE_DIR} && pio device monitor -b 115200")

def main():
    print("=" * 55)
    print("  BUILD BRUCE + VIP ADDON CHO ESP32-C3")
    print("=" * 55)

    clone_bruce()
    copy_vip_files()
    build()
    upload()

    print("\n📂 Firmware đầu ra:")
    print(f"   {os.path.abspath(os.path.join(BRUCE_DIR, '.pio/build', ENV, 'firmware.bin'))}")

if __name__ == "__main__":
    main()