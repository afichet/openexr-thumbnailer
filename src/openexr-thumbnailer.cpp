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

#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfHeader.h>
#include <OpenEXR/ImfPreviewImage.h>
#include <OpenEXR/ImfChromaticitiesAttribute.h>

#include <cmath>

#define USE_PREVIEW

double to_sRGB(double rgb_color)
{
    const double a = 0.055;
    if (rgb_color < 0.0031308)
        return 12.92 * rgb_color;
    else
        return (1.0 + a) * std::pow(rgb_color, 1.0 / 2.4) - a;
}

unsigned char *load_exr(const char *path, int *width, int *height)
{
    Imf::RgbaInputFile f(path);
    unsigned char *img = nullptr;

#ifdef USE_PREVIEW
    if (f.header().hasPreviewImage()) {
        const Imf::PreviewImage &preview = f.header().previewImage();
        *width  = preview.width();
        *height = preview.height();
        img     = new unsigned char[3 * (*width) * (*height)];

        for (int i = 0; i < (*width) * (*height); i++) {
            img[3*i + 0] = preview.pixels()[i].r;
            img[3*i + 1] = preview.pixels()[i].g;
            img[3*i + 2] = preview.pixels()[i].b;
        }
    } else {
#endif // USE_PREVIEW
        Imf::Array2D<Imf::Rgba> pixels;

        Imath::Box2i       dw = f.dataWindow();
        int                w  = dw.max.x - dw.min.x + 1;
        int                h  = dw.max.y - dw.min.y + 1;
        *width                = w;
        *height               = h;
        img                   = new unsigned char[3 * w * h];

        pixels.resizeErase(h, w);
        f.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * w, 1, w);
        f.readPixels(dw.min.y, dw.max.y);

        // Check if there is specific chromaticities tied to the color representation in this part.
        const Imf::ChromaticitiesAttribute *c = f.header().findTypedAttribute<Imf::ChromaticitiesAttribute>("chromaticities");

        Imf::Chromaticities chromaticities;

        if (c != nullptr) {
            chromaticities = c->value();
        }

        // Handle custom chromaticities
        Imath::M44f RGB_XYZ = Imf::RGBtoXYZ(chromaticities, 1.f);
        Imath::M44f XYZ_RGB = Imf::XYZtoRGB(Imf::Chromaticities(), 1.f);

        Imath::M44f conversionMatrix = RGB_XYZ * XYZ_RGB;


        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Imath::V3f rgb(pixels[y][x].r, pixels[y][x].g, pixels[y][x].b);
                rgb *= conversionMatrix;

                img[3*(y*w + x) + 0] =
                    (unsigned char)(255.0*std::min(1.0, std::max(0.0, to_sRGB(rgb.x))));
                img[3*(y*w + x) + 1] =
                    (unsigned char)(255.0*std::min(1.0, std::max(0.0, to_sRGB(rgb.y))));
                img[3*(y*w + x) + 2] =
                    (unsigned char)(255.0*std::min(1.0, std::max(0.0, to_sRGB(rgb.z))));
            }
        }
#ifdef USE_PREVIEW
    }
#endif // USE_PREVIEW

    return img;
}

extern "C" void free_buff(guchar *pixels, gpointer data)
{
    delete[] pixels;
}

extern "C" GdkPixbuf *file_to_pixbuf(const char *path, GError **error)
{
    int            width, height;
    unsigned char *pixels = NULL;
    GdkPixbuf *    pixbuf;


    pixels = load_exr(path, &width, &height);
    pixbuf = gdk_pixbuf_new_from_data(
        pixels,
        GDK_COLORSPACE_RGB,
        FALSE,
        8,
        //height, width,
        width,
        height,
        3 * width,
        free_buff,
        NULL);

    // The pointer must remain valid,
    // do not: `delete[] pixels;`

    return pixbuf;
}
