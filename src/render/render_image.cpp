#include <Python.h>

#include "render/render_image.h"
#include "ui/canvas.h"

#include "cpp/shape.h"
#include "util/region.h"
#include "tree/render.h"

#include "formats/png.h"

RenderImage::RenderImage(Shape* shape, QObject* parent)
    : QObject(parent), bounds(shape->bounds),
      image(shape->bounds.xmax - shape->bounds.xmin,
            shape->bounds.ymax - shape->bounds.ymin,
            QImage::Format_RGB32)
{
    render(shape);
}

RenderImage::~RenderImage()
{
    for (auto p = pixmaps.begin(); p != pixmaps.end(); ++p)
    {
        p.key()->scene->removeItem(p.value());
        delete p.value();
    }
}

void RenderImage::render(Shape *shape)
{
    image.fill(0x000000);

    uint8_t* depth8(new uint8_t[image.width() * image.height()]);
    uint8_t** depth8_rows(new uint8_t*[image.height()]);

    uint8_t* shades8(new uint8_t[image.width() * image.height()]);
    uint8_t** shades8_rows(new uint8_t*[image.height()]);
    for (int i=0; i < image.height(); ++i)
    {
        depth8_rows[i] = depth8 + (image.width() * i);
        shades8_rows[i] = shades8 + (image.width() * i);
    }
    memset(depth8, 0, image.width() * image.height());
    memset(shades8, 0, image.width() * image.height());


    Region r = (Region) {
            .imin=0, .jmin=0, .kmin=0,
            .ni=(uint32_t)image.width(), .nj=(uint32_t)image.height(),
            .nk=uint32_t(shape->bounds.zmax - shape->bounds.zmin)
    };

    build_arrays(&r, shape->bounds.xmin, shape->bounds.ymin, shape->bounds.zmin,
                     shape->bounds.xmax, shape->bounds.ymax, shape->bounds.zmax);
    int halt=0;
    render8(shape->tree.get(), r, depth8_rows, &halt);
    shaded8(shape->tree.get(), r, depth8_rows, shades8_rows, &halt);

    free_arrays(&r);

    for (int j=0; j < image.height(); ++j)
    {
        for (int i=0; i < image.width(); ++i)
        {
            uint8_t pix = depth8_rows[j][i];
            if (pix)
            {
                image.setPixel(i, j, shades8_rows[j][i]);
            }
            else
            {
                image.setPixel(i, j, 0xff0000);
            }
        }
    }

    delete [] depth8;
    delete [] depth8_rows;

    delete [] shades8;
    delete [] shades8_rows;
}

void RenderImage::addToCanvas(Canvas *canvas)
{
    QGraphicsPixmapItem* pix = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    pix->setPos(bounds.xmin, bounds.ymin);
    canvas->scene->addItem(pix);
    pixmaps[canvas] = pix;
}
