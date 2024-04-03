#include "hpdf.h"
#include <stdio.h>
#include <stdlib.h>

#define OUTPUT_FILENAME "output.pdf"
#define FONT_SIZE 12
#define CHAR_WIDTH 12
#define CHAR_HEIGHT 12
#define MARGIN_X 50
#define MARGIN_Y 750
#define CHARS_PER_PAGE 200 // 每页字符数
#define CHARS_PER_LINE 10  // 每行字符数

int main()
{
    HPDF_Doc pdf;
    HPDF_Font font;
    HPDF_Page page;
    const char *font_name;
    int char_count = 0;
    int page_num = 1;
    int line_count = 0;

    // 创建 PDF 文档对象
    pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        printf("Error: Unable to create PDF document.\n");
        return EXIT_FAILURE;
    }

    // 加载 Arial Unicode MS TrueType 字体文件
    font_name = HPDF_LoadTTFontFromFile(pdf, "Arial Unicode MS.ttf", HPDF_TRUE);
    if (!font_name)
    {
        printf("Error: Failed to load TrueType font.\n");
        HPDF_Free(pdf);
        return EXIT_FAILURE;
    }

    // 获取字体对象
    font = HPDF_GetFont(pdf, font_name, NULL);
    if (!font)
    {
        printf("Error: Failed to get font.\n");
        HPDF_Free(pdf);
        return EXIT_FAILURE;
    }

    // 开始写入文本
    page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);

    for (int i = 0x20; i <= 0xFFFF; ++i)
    {
        HPDF_UNICODE unicode = (HPDF_UNICODE)i;

        char buffer[3] = {(char)(unicode & 0xFF), (char)((unicode >> 8) & 0xFF), '\0'};
        if (HPDF_Page_TextOut(page, MARGIN_X + (char_count % CHARS_PER_LINE) * CHAR_WIDTH, MARGIN_Y - (char_count / CHARS_PER_LINE) * CHAR_HEIGHT, buffer) != HPDF_OK)
        {
            printf("Error: Unable to write text to page.\n");
            HPDF_Free(pdf);
            return EXIT_FAILURE;
        }
        char_count++;
        if (char_count % CHARS_PER_PAGE == 0)
        {
            HPDF_Page_EndText(page);
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);
            char_count = 0;
        }
    }

    // 结束写入文本
    HPDF_Page_EndText(page);

    // 保存 PDF 到文件
    if (HPDF_SaveToFile(pdf, OUTPUT_FILENAME) != HPDF_OK)
    {
        printf("Error: Unable to save PDF file.\n");
        HPDF_Free(pdf);
        return EXIT_FAILURE;
    }

    // 清理并释放资源
    HPDF_Free(pdf);

    printf("PDF created successfully with %d pages.\n", page_num);
    return EXIT_SUCCESS;
}
// #include "hpdf.h"
// #include <stdio.h>
// #include <stdlib.h>

// #define OUTPUT_FILENAME "test.pdf"
// #define FONT_SIZE 12
// #define MARGIN_X 50
// #define MARGIN_Y 750

// int main()
// {
//     HPDF_Doc pdf;
//     HPDF_Font font;
//     HPDF_Page page;

//     // 创建 PDF 文档对象
//     pdf = HPDF_New(NULL, NULL);
//     if (!pdf)
//     {
//         printf("Error: Unable to create PDF document.\n");
//         return EXIT_FAILURE;
//     }

//     // 创建一页 PDF 页面
//     page = HPDF_AddPage(pdf);
//     if (!page)
//     {
//         printf("Error: Unable to create PDF page.\n");
//         HPDF_Free(pdf);
//         return EXIT_FAILURE;
//     }

//     // 设置页面尺寸和方向
//     HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

//     // 获取标准字体对象
//     font = HPDF_GetFont(pdf, "Helvetica", NULL);
//     if (!font)
//     {
//         printf("Error: Unable to get font.\n");
//         HPDF_Free(pdf);
//         return EXIT_FAILURE;
//     }

//     // 开始写入文本
//     HPDF_Page_BeginText(page);
//     HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);
//     HPDF_Page_TextOut(page, MARGIN_X, MARGIN_Y, "Hello, World!");
//     HPDF_Page_EndText(page);

//     // 保存 PDF 到文件
//     if (HPDF_SaveToFile(pdf, OUTPUT_FILENAME) != HPDF_OK)
//     {
//         printf("Error: Unable to save PDF file.\n");
//         HPDF_Free(pdf);
//         return EXIT_FAILURE;
//     }

//     // 清理并释放资源
//     HPDF_Free(pdf);

//     printf("PDF created successfully.\n");
//     return EXIT_SUCCESS;
// }
