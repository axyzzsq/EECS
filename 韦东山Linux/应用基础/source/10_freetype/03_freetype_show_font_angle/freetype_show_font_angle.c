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
struct fb_var_screeninfo var;	/* Current var */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;


/**********************************************************************
 * 函数名称： lcd_put_pixel
 * 功能描述： 在LCD指定位置上输出指定颜色（描点）
 * 输入参数： x坐标，y坐标，颜色
 * 输出参数： 无
 * 返 回 值： 会
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2020/05/12	     V1.0	  zh(angenao)	      创建
 ***********************************************************************/ 
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
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2020/05/12	     V1.0	  zh(angenao)	      创建
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


int main(int argc, char **argv)
{
	wchar_t *chinese_str = L"繁";

	FT_Library	  library;
	FT_Face 	  face;
	int error;
    FT_Vector     pen;
	FT_GlyphSlot  slot;
	int font_size = 24;
	FT_Matrix	  matrix;				  /* transformation matrix */
	double		  angle;

	if (argc < 3)
	{
		printf("Usage : %s <font_file> <angle> [font_size]\n", argv[0]);
		return -1;
	}

	angle  = ( 1.0* strtoul(argv[2], NULL, 0) / 360 ) * 3.14159 * 2;	   /* use 25 degrees	 */

	if (argc == 4)
		font_size = strtoul(argv[3], NULL, 0);
		
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

	/* 显示矢量字体 */
	error = FT_Init_FreeType( &library );			   /* initialize library */
	/* error handling omitted */
	
	error = FT_New_Face( library, argv[1], 0, &face ); /* create face object */
	/* error handling omitted */	
	slot = face->glyph;

	FT_Set_Pixel_Sizes(face, font_size, 0);

	/* 确定座标:
	 */
	pen.x = 0;
	pen.y = 0;

	/* set up matrix */
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

    /* set transformation */
    FT_Set_Transform( face, &matrix, &pen);

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, chinese_str[0], FT_LOAD_RENDER );
	if (error)
	{
		printf("FT_Load_Char error\n");
		return -1;
	}
	
    draw_bitmap( &slot->bitmap,
                 var.xres/2,
                 var.yres/2);

	return 0;	
}

