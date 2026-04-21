import os

# ==================== 路径配置 ====================
root        = os.path.abspath(os.path.dirname(__file__))
bsp_inc     = os.path.join(root, "BSP", "Inc")
bsp_src     = os.path.join(root, "BSP", "Src")

app_inc     = os.path.join(root, "Application", "Inc")
app_src     = os.path.join(root, "Application", "Src")

# ==================== 处理函数 ====================
def process_files(src_dir, dst_dir):
    os.makedirs(dst_dir, exist_ok=True)

    for f in os.listdir(src_dir):
        # 只看 .c .h
        if not f.endswith((".c", ".h")):
            continue

        # 只处理 bsp_ 开头
        if not f.startswith("bsp_"):
            print(f"[忽略] {f}")
            continue

        # 替换成 app_
        new_f = "app_" + f[4:]
        dst_path = os.path.join(dst_dir, new_f)

        if os.path.exists(dst_path):
            print(f"[已存在] {new_f}")
            continue

        # 创建空文件
        open(dst_path, "w", encoding="utf-8").close()
        print(f"[创建] {new_f}")

# ==================== 执行 ====================
print("=== 处理 Inc ===")
process_files(bsp_inc, app_inc)

print("\n=== 处理 Src ===")
process_files(bsp_src, app_src)

print("\n✅ 全部完成！")