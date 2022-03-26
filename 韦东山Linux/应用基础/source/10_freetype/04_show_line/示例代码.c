int main(int argc, char **argv)
{
	FT_Library	  library; /* 对应freetype库 */
	FT_Face 	  face;    /* 对应字体文件 */
	FT_GlyphSlot  slot;    /* 对应字符的处理结果: 含glyph和位图 */
	FT_Glyph      glyph;   /* 对应字符经过处理后的glyph即外形、轮廓 */
	FT_BBox       bbox;    /* 字符的外框 */
    FT_Vector     pen;     /* 字符的原点 */

	error = FT_Init_FreeType( &library ); /* 初始化freetype库 */
	
	error = FT_New_Face(library, font_file, 0, &face ); /* 加载字体文件 */

	slot = face->glyph; /* 插槽: 字体的处理结果保存在这里 */

	FT_Set_Pixel_Sizes(face, 24, 0); /* 设置大小 */

	/* 确定坐标 */
	pen.x = 0; /* 单位: 1/64 像素 */
	pen.y = 0; /* 单位: 1/64 像素 */

    FT_Set_Transform( face, 0, &pen); /* 变形 */

	/* 根据font_code加载字符, 得到新的glyph和位图, 结果保存在slot中 */
    error = FT_Load_Char(face, font_code, FT_LOAD_RENDER); 

	error = FT_Get_Glyph(slot, &glyph); /* 从slot中得到新的glyph */
		
	FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox ); /* 从glyph中得到字符外框 */
		
    draw_bitmap(&slot->bitmap, lcd_x, lcd_y);  /* 位图也保存在slot->bitmap中 */

	return 0;	
}


