import os
import re
import datetime

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# 工程根目录 = script 的上一级
ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, ".."))

# VERSION 在工程根目录
IN_FILE = os.path.join(ROOT, "VERSION")

# version.inc 生成到 script/generate/
OUT_DIR = os.path.join(SCRIPT_DIR, "generate")
OUT_FILE = os.path.join(OUT_DIR, "version.inc")

os.makedirs(OUT_DIR, exist_ok=True)

text = open(IN_FILE, "r", encoding="utf-8").read()

def get_value(key, default):
    m = re.search(rf"^{key}\s*=\s*(.+)$", text, re.MULTILINE)
    return m.group(1).strip() if m else default

def bytes_to_asm_array(s, size):
    b = s.encode("ascii")
    if len(b) >= size:
        raise ValueError(f'"{s}" is too long, max {size - 1} chars')
    b = b + b"\0"
    b = b.ljust(size, b"\0")
    return ",".join(f"0x{x:02X}" for x in b)

fw_type = get_value("TYPE", "#APP")
board = get_value("BOARD", "S32K144")
version = get_value("VERSION", "1.0.0.a")
timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")

with open(OUT_FILE, "w", encoding="utf-8") as f:
    f.write("/* Auto generated, do not edit */\n")
    f.write(".set FW_MAGIC,  0xA55AA55A\n")
    f.write(".set FW_LENGTH, 0xFFFFFFFF\n")
    f.write(".set FW_CRC32,  0xFFFFFFFF\n\n")

    f.write(".macro FW_TYPE_DATA\n")
    f.write(f"    .byte {bytes_to_asm_array(fw_type, 16)}\n")
    f.write(".endm\n\n")

    f.write(".macro FW_BOARD_DATA\n")
    f.write(f"    .byte {bytes_to_asm_array(board, 16)}\n")
    f.write(".endm\n\n")

    f.write(".macro FW_VERSION_DATA\n")
    f.write(f"    .byte {bytes_to_asm_array(version, 16)}\n")
    f.write(".endm\n\n")

    f.write(".macro FW_TIMESTAMP_DATA\n")
    f.write(f"    .byte {bytes_to_asm_array(timestamp, 16)}\n")
    f.write(".endm\n")
