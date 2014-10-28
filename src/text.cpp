#include <iostream>

#include "text.h"

using namespace std;

FT_Library Text::lib = NULL;

Text::Text() {
}

Text::Text(string text, int resolution) {
	init(text, resolution);
}

void Text::init(string text_in, int res_in) {
	text = text_in;
	resolution = res_in;
	set_font("/usr/share/fonts/dejavu/DejaVuSans.ttf"); //Default font
	spacing = 2;
	pixmap_created = false;
}

void Text::init_freetype() {
	FT_Init_FreeType(&lib);
}

void Text::generate_pixmap() {
	if (pixmap_created) {
		cerr << "Pixmap already created for " << text << endl;
		return;
	}
	
	int text_length = text.size();

	FT_Set_Pixel_Sizes(face, 0, resolution); //0 For pixel_width makes it equal to pixel_height
	
	width = 0;
	height = 0;
	for (int ic = 0; ic < text_length; ic++) { //First calculated size of pixmap
		FT_Load_Char(face, text[ic], FT_LOAD_RENDER);
	
		if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) { //Check if bitmap already exists or not
			//Render mode mono will give 1-bit monochrome bitmap, use FT_RENDER_MODE_NORMAL for full render
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
		}
	
		FT_Bitmap* bmap = &face->glyph->bitmap;
	
		int rows = bmap->rows;
		int cols = bmap->width;
		
		if (rows > height) {
			height = rows;
		}
		
		width += cols;
		if (ic != 0) {
			width += spacing;
		}
		
	}
	
	pixels = new int* [width];
	
	for (int i = 0; i < width; i++) {
		pixels[i] = new int [height];
	}
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			pixels[i][j] = 0;
		}
	}
	
	int offset = 0;
	for (int ic = 0; ic < text_length; ic++) {
		FT_Load_Char(face, text[ic], FT_LOAD_RENDER);
	
		if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) { //Check if bitmap already exists or not
			//Render mode mono will give 1-bit monochrome bitmap, use FT_RENDER_MODE_NORMAL for full render
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
		}
	
		FT_Bitmap* bmap = &face->glyph->bitmap;
	
		int rows = bmap->rows;
		int cols = bmap->width;
	
		//cout << "(" << cols << "," << rows << ")" << text[ic] << endl;
		
		for (int j = 0; j < rows; j++) {
			for (int i = 0; i < cols; i++) {
				int pix = bmap->buffer[i + j*cols];
				pixels[i + offset][j + height - rows] = pix;
			}
		}
		offset += cols + spacing;
	}
	
	pixmap_created = true;
}

void Text::set_font(std::string fontpath) {
	FT_New_Face(lib, fontpath.c_str(), 0, &face);
}

int Text::get_pixel(int i, int j) {
	if (!pixmap_created) {
		cerr << "Pixmap not created for " << text << endl;
		return 0;
	}
	
	if (i < 0 || i >= width || j < 0 || j >= height) {
		return 0;
	}
	
	return pixels[i][j];
}

