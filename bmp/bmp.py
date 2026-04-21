from PIL import Image
import os

# ===================== 配置项 =====================
IMAGE_WIDTH = 128       # OLED宽度
IMAGE_HEIGHT = 64       # OLED高度
BMP_FOLDER = "bmp_files"  # 存放BMP图片的文件夹
OUTPUT_FOLDER = "output" # 输出C文件的文件夹
# ==================================================

# 创建输出文件夹
if not os.path.exists(OUTPUT_FOLDER):
    os.makedirs(OUTPUT_FOLDER)

if not os.path.exists(BMP_FOLDER):
    os.makedirs(BMP_FOLDER)
    print(f"已创建 {BMP_FOLDER} 文件夹，请把BMP图片放进去再运行！")
    exit()

def convert_bmp_to_oled_array(image_path):
    """
    把128x64 BMP转为OLED 128*8数组格式
    格式：const char name[128][8]
    排列：垂直8点一字节，从左到右，从上到下
    """
    # 打开图片并转为灰度图
    img = Image.open(image_path).convert('L')
    img = img.resize((IMAGE_WIDTH, IMAGE_HEIGHT))  # 强制128x64
    
    # 二值化（黑白）
    threshold = 128
    pixels = [0 if p < threshold else 1 for p in img.getdata()]
    
    # 生成OLED数据（核心）
    oled_data = []
    for x in range(IMAGE_WIDTH):       # 从左到右 128列
        col_data = []
        for page in range(8):         # 8页（每页8行）
            byte = 0
            for y_in_page in range(8):# 垂直8点
                y = page * 8 + y_in_page
                idx = y * IMAGE_WIDTH + x
                if pixels[idx]:
                    byte |= (1 << y_in_page)
            col_data.append(byte)
        oled_data.append(col_data)
    
    return oled_data

def generate_c_code(oled_data, var_name):
    """生成C语言数组字符串"""
    c_code = f"const char {var_name}[{IMAGE_WIDTH}][8] = {{\n"
    
    for col in oled_data:
        hex_str = ", ".join([f"0x{b:02X}" for b in col])
        c_code += f"    {{{hex_str}}},\n"
    
    c_code = c_code.rstrip(",\n") + "\n};\n"
    return c_code

def main():
    # 遍历bmp文件夹所有文件
    for filename in os.listdir(BMP_FOLDER):
        if filename.lower().endswith(".bmp"):
            print(f"正在转换：{filename}")
            
            # 生成变量名（去掉后缀，合法C变量名）
            var_name = os.path.splitext(filename)[0]
            var_name = var_name.replace(" ", "_").replace("-", "_")
            
            # 转换
            img_path = os.path.join(BMP_FOLDER, filename)
            data = convert_bmp_to_oled_array(img_path)
            c_code = generate_c_code(data, var_name)
            
            # 保存到文件
            out_path = os.path.join(OUTPUT_FOLDER, f"{var_name}.h")
            with open(out_path, "w", encoding="utf-8") as f:
                f.write(c_code)
            
            print(f"✅ 转换完成：{out_path}\n")

if __name__ == "__main__":
    main()