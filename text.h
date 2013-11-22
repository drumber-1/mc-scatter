#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

class Text {
		static FT_Library lib;
		FT_Face face;
		int** pixels;
		int resolution, spacing;
		void init(std::string, int);
		bool pixmap_created;
	public:
		std::string text;
		int width, height;
		Text();
		Text(std::string, int);
		static void init_freetype();
		void generate_pixmap();
		void set_font(std::string);
		int get_pixel(int, int);
};
