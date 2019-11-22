#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <errno.h>

#include <zlib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

//#include "gnome-thumbnailer-skeleton.h"

#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <cmath>

double
to_sRGB(double rgb_color) {
    const double a = 0.055;
    if (rgb_color < 0.0031308)
        return 12.92 * rgb_color;
    else
        return (1.0 + a) * std::pow(rgb_color, 1.0/2.4) - a;
}

unsigned char*
load_exr(const char* path,
		 int *width, int *height) {
	// MyStream stream(buffer, size, "test");
    Imf::Array2D<Imf::Rgba> pixels;

    Imf::RgbaInputFile f(path);
    Imath::Box2i dw1 = f.dataWindow();
    int w = dw1.max.x - dw1.min.x + 1;
    int h = dw1.max.y - dw1.min.y + 1;
    *width = w;
    *height = h;

    pixels.resizeErase(h, w);
    f.setFrameBuffer(&pixels[0][0] - dw1.min.x - dw1.min.y * w, 1, w);
    f.readPixels(dw1.min.y, dw1.max.y);

    unsigned char* img = new unsigned char[3*w*h];
	//unsigned char* img = calloc(sizeof(unsigned char), 3*w*h);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            img[3*(y*w + x) + 0] =
				(unsigned char)(255.0*std::min(1.0, std::max(0.0, to_sRGB(pixels[y][x].r))));
            img[3*(y*w + x) + 1] =
				(unsigned char)(255.0*std::min(1.0, std::max(0.0, to_sRGB(pixels[y][x].g))));
            img[3*(y*w + x) + 2] =
				(unsigned char)(255.0*std::min(1.0, std::max(0.0, to_sRGB(pixels[y][x].b))));
		}
    }

    return img;
}

extern "C"
void free_buff(guchar *pixels, gpointer data) {
	delete[] pixels;
}

extern "C" 
GdkPixbuf *
file_to_pixbuf (const char  *path,
		GError     **error)
{
    int width, height;
    unsigned char *pixels = NULL;
	GdkPixbuf *pixbuf;
	
	
	pixels = load_exr(path, &width, &height);
	pixbuf = gdk_pixbuf_new_from_data(pixels,
									  GDK_COLORSPACE_RGB,
									  FALSE,
									  8,
									  //height, width,
									  width, height,
									  3*width,
									  free_buff,
									  NULL);
	
	return pixbuf;
}
