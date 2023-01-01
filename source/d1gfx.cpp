#include "d1gfx.h"

D1GfxPixel::D1GfxPixel(bool t, quint8 pi)
    : transparent(t)
    , paletteIndex(pi)
{
}

bool D1GfxPixel::isTransparent() const
{
    return this->transparent;
}

quint8 D1GfxPixel::getPaletteIndex()
{
    return this->paletteIndex;
}

quint16 D1GfxFrame::getWidth()
{
    return this->width;
}

quint16 D1GfxFrame::getHeight()
{
    return this->height;
}

D1GfxPixel D1GfxFrame::getPixel(quint16 x, quint16 y)
{
    if (x < this->width && y < this->height)
        return this->pixels[y][x];

    return {};
}

bool D1GfxFrame::isClipped()
{
    return this->clipped;
}

bool D1Gfx::isFrameSizeConstant()
{
    if (this->frames.isEmpty()) {
        return false;
    }

    quint16 frameWidth = this->frames[0].getWidth();
    quint16 frameHeight = this->frames[0].getHeight();

    for (int i = 1; i < this->frames.count(); i++) {
        if (this->frames[i].getWidth() != frameWidth
            || this->frames[i].getHeight() != frameHeight)
            return false;
    }

    return true;
}

// builds QImage from a D1CelFrame of given index
QImage D1Gfx::getFrameImage(quint16 frameIndex)
{
    if (this->palette == nullptr || frameIndex >= this->frames.count())
        return QImage();

    D1GfxFrame &frame = this->frames[frameIndex];

    QImage image = QImage(
        frame.getWidth(),
        frame.getHeight(),
        QImage::Format_ARGB32);

    for (int y = 0; y < frame.getHeight(); y++) {
        for (int x = 0; x < frame.getWidth(); x++) {
            D1GfxPixel d1pix = frame.getPixel(x, y);

            QColor color;
            if (d1pix.isTransparent())
                color = QColor(Qt::transparent);
            else
                color = this->palette->getColor(d1pix.getPaletteIndex());

            image.setPixel(x, y, color.rgba());
        }
    }

    return image;
}

D1CEL_TYPE D1Gfx::getType()
{
    return this->type;
}

QString D1Gfx::getFilePath()
{
    return this->gfxFilePath;
}

D1Pal *D1Gfx::getPalette()
{
    return this->palette;
}

void D1Gfx::setPalette(D1Pal *pal)
{
    this->palette = pal;
}

int D1Gfx::getGroupCount()
{
    return this->groupFrameIndices.count();
}

QPair<quint16, quint16> D1Gfx::getGroupFrameIndices(quint16 groupIndex)
{
    if (!this->groupFrameIndices.empty() && groupIndex < this->groupFrameIndices.count())
        return this->groupFrameIndices[groupIndex];

    return qMakePair(0, 0);
}

int D1Gfx::getFrameCount()
{
    return this->frames.count();
}

D1GfxFrame *D1Gfx::getFrame(quint16 frameIndex)
{
    if (frameIndex >= this->frames.count())
        return nullptr;

    return &this->frames[frameIndex];
}

quint16 D1Gfx::getFrameWidth(quint16 frameIndex)
{
    if (frameIndex >= this->frames.count())
        return 0;

    return this->frames[frameIndex].getWidth();
}

quint16 D1Gfx::getFrameHeight(quint16 frameIndex)
{
    if (frameIndex >= this->frames.count())
        return 0;

    return this->frames[frameIndex].getHeight();
}