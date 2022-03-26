#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

int fd_fb;
struct fb_var_screeninfo var;   /* Current var */
struct fb_fix_screeninfo fix;   /* Current fix */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;

/* color : 0x00RRGGBB */
void lcd_put_pixel(int x, int y, unsigned int color)
{
    unsigned char *pen_8 = fbmem+y*line_width+x*pixel_width;
    unsigned short *pen_16; 
    unsigned int *pen_32;   

    unsigned int red, green, blue;  

    pen_16 = (unsigned short *)pen_8;
    pen_32 = (unsigned int *)pen_8;

    switch (var.bits_per_pixel)
    {
        case 8:
        {
            *pen_8 = color;
            break;
        }
        case 16:
        {
            /* 565 */
            red   = (color >> 16) & 0xff;
            green = (color >> 8) & 0xff;
            blue  = (color >> 0) & 0xff;
            color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            *pen_16 = color;
            break;
        }
        case 32:
        {
            *pen_32 = color;
            break;
        }
        default:
        {
            printf("can't surport %dbpp\n", var.bits_per_pixel);
            break;
        }
    }
}

/**********************************************************************
 * 函数名称： draw_bitmap
 * 功能描述： 根据bitmap位图，在LCD指定位置显示汉字
 * 输入参数： x坐标，y坐标，位图指针
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2020/05/12        V1.0     zh(angenao)         创建
 ***********************************************************************/ 
void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max = x + bitmap->width;
    FT_Int  y_max = y + bitmap->rows;

    //printf("x = %d, y = %d\n", x, y);

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if ( i < 0      || j < 0       ||
                i >= var.xres || j >= var.yres )
            continue;

            //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
            lcd_put_pixel(i, j, bitmap->buffer[q * bitmap->width + p]);
        }
    }
}

int compute_string_bbox(FT_Face       face, wchar_t *wstr, FT_BBox  *abbox)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_BBox glyph_bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = face->glyph;

    /* 初始化 */
    bbox.xMin = bbox.yMin = 32000;
    bbox.xMax = bbox.yMax = -32000;

    /* 指定原点为(0, 0) */
    pen.x = 0;
    pen.y = 0;

    /* 计算每个字符的bounding box */
    /* 先translate, 再load char, 就可以得到它的外框了 */
    for (i = 0; i < wcslen(wstr); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(face, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(face, wstr[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 取出glyph */
        error = FT_Get_Glyph(face->glyph, &glyph);
        if (error)
        {
            printf("FT_Get_Glyph error!\n");
            return -1;
        }
        
        /* 从glyph得到外框: bbox */
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &glyph_bbox);

        /* 更新外框 */
        if ( glyph_bbox.xMin < bbox.xMin )
            bbox.xMin = glyph_bbox.xMin;

        if ( glyph_bbox.yMin < bbox.yMin )
            bbox.yMin = glyph_bbox.yMin;

        if ( glyph_bbox.xMax > bbox.xMax )
            bbox.xMax = glyph_bbox.xMax;

        if ( glyph_bbox.yMax > bbox.yMax )
            bbox.yMax = glyph_bbox.yMax;
        
        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    /* return string bbox */
    *abbox = bbox;
}


int display_string(FT_Face     face, wchar_t *wstr, int lcd_x, int lcd_y)
{
    int i;
    int error;
    FT_BBox bbox;
    FT_Vector pen;
    FT_Glyph  glyph;
    FT_GlyphSlot slot = face->glyph;

    /* 把LCD坐标转换为笛卡尔坐标 */
    int x = lcd_x;
    int y = var.yres - lcd_y;

    /* 计算外框 */
    compute_string_bbox(face, wstr, &bbox);

    /* 反推原点 */
    pen.x = (x - bbox.xMin) * 64; /* 单位: 1/64像素 */
    pen.y = (y - bbox.yMax) * 64; /* 单位: 1/64像素 */

    /* 处理每个字符 */
    for (i = 0; i < wcslen(wstr); i++)
    {
        /* 转换：transformation */
        FT_Set_Transform(face, 0, &pen);

        /* 加载位图: load glyph image into the slot (erase previous one) */
        error = FT_Load_Char(face, wstr[i], FT_LOAD_RENDER);
        if (error)
        {
            printf("FT_Load_Char error\n");
            return -1;
        }

        /* 在LCD上绘制: 使用LCD坐标 */
        draw_bitmap( &slot->bitmap,
                        slot->bitmap_left,
                        var.yres - slot->bitmap_top);

        /* 计算下一个字符的原点: increment pen position */
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    return 0;
}


int main(int argc, char **argv)
{
    wchar_t *wstr = L"百问网www.100ask.net";

    FT_Library    library;
    FT_Face       face;
    int error;
    FT_BBox bbox;
    int font_size = 24;
    int lcd_x, lcd_y;

    if (argc < 4)
    {
        printf("Usage : %s <font_file> <lcd_x> <lcd_y> [font_size]\n", argv[0]);
        return -1;
    }

    lcd_x = strtoul(argv[2], NULL, 0);      
    lcd_y = strtoul(argv[3], NULL, 0);      
    
    if (argc == 5)
        font_size = strtoul(argv[4], NULL, 0);      

    fd_fb = open("/dev/fb0", O_RDWR);
    if (fd_fb < 0)
    {
        printf("can't open /dev/fb0\n");
        return -1;
    }

    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
    {
        printf("can't get var\n");
        return -1;
    }

    if (ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix))
    {
        printf("can't get fix\n");
        return -1;
    }

    line_width  = var.xres * var.bits_per_pixel / 8;
    pixel_width = var.bits_per_pixel / 8;
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    fbmem = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if (fbmem == (unsigned char *)-1)
    {
        printf("can't mmap\n");
        return -1;
    }

    /* 清屏: 全部设为黑色 */
    memset(fbmem, 0, screen_size);

    error = FT_Init_FreeType( &library );              /* initialize library */
    
    error = FT_New_Face( library, argv[1], 0, &face ); /* create face object */

    FT_Set_Pixel_Sizes(face, font_size, 0);

    display_string(face, wstr, lcd_x, lcd_y);
    
    return 0;   
}

