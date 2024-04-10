#include <ft2build.h>
#include FT_FREETYPE_H
#include <fstream>
#include <iostream>
#include <stdio.h>

int main()
{
    FT_Library library;
    FT_Face face;
    long int i = 0;
    const char *font_file = "Arial Unicode MS.ttf";
    const char *output_file = "supported_characters.txt";

    // 初始化 FreeType 库
    if (FT_Init_FreeType(&library))
    {
        std::cerr << "Error: Unable to initialize FreeType library.\n";
        return 1;
    }

    // 加载 Arial Unicode MS 字体
    if (FT_New_Face(library, font_file, 0, &face))
    {
        std::cerr << "Error: Unable to load font file.\n";
        FT_Done_FreeType(library);
        return 1;
    }

    // 打开输出文件
    std::ofstream outfile(output_file);
    if (!outfile.is_open())
    {
        std::cerr << "Error: Unable to open output file.\n";
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return 1;
    }

    // 迭代 Unicode 码点
    for (FT_ULong charcode = 0; charcode < 0x10FFFF; ++charcode)
    {
        // 检查字符是否支持
        if (FT_Get_Char_Index(face, charcode))
        {
            // UTF-8 编码转换
            char utf8_bytes[5] = {0};
            if (charcode < 0x80)
            {
                utf8_bytes[0] = static_cast<char>(charcode);
            }
            else if (charcode < 0x800)
            {
                utf8_bytes[0] = static_cast<char>(0xC0 | ((charcode >> 6) & 0x1F));
                utf8_bytes[1] = static_cast<char>(0x80 | (charcode & 0x3F));
            }
            else if (charcode < 0x10000)
            {
                utf8_bytes[0] = static_cast<char>(0xE0 | ((charcode >> 12) & 0x0F));
                utf8_bytes[1] = static_cast<char>(0x80 | ((charcode >> 6) & 0x3F));
                utf8_bytes[2] = static_cast<char>(0x80 | (charcode & 0x3F));
            }
            else
            {
                utf8_bytes[0] = static_cast<char>(0xF0 | ((charcode >> 18) & 0x07));
                utf8_bytes[1] = static_cast<char>(0x80 | ((charcode >> 12) & 0x3F));
                utf8_bytes[2] = static_cast<char>(0x80 | ((charcode >> 6) & 0x3F));
                utf8_bytes[3] = static_cast<char>(0x80 | (charcode & 0x3F));
            }

            // 写入 UTF-8 字符到文件
            outfile << utf8_bytes;

            // 每行写入 10 个字符
            if (++i % 10 == 0)
                outfile << "\n";
        }
    }

    std::cout << "Total supported characters: " << i << std::endl;

    // 关闭输出文件
    outfile.close();

    // 释放 FreeType 资源
    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return 0;
}
