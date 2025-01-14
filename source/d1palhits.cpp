#include "d1palhits.h"

D1PalHits::D1PalHits(D1Gfx *g)
    : gfx(g)
{
    D1PalHits::buildPalHits();
}

D1PalHits::D1PalHits(D1Gfx *g, D1Min *m, D1Til *t, D1Sol *s)
    : isLevelCel(true)
    , gfx(g)
    , min(m)
    , til(t)
    , sol(s)
{
    D1PalHits::buildPalHits();
    this->buildSubtilePalHits();
    this->buildTilePalHits();
}

D1PALHITS_MODE D1PalHits::getMode()
{
    return this->mode;
}

void D1PalHits::setMode(D1PALHITS_MODE m)
{
    this->mode = m;
}

void D1PalHits::buildPalHits()
{
    D1GfxFrame *frame = nullptr;
    quint8 paletteIndex = 0;

    // Go through all frames
    this->framePalHits.clear();
    this->allFramesPalHits.clear();
    for (int i = 0; i < this->gfx->getFrameCount(); i++) {
        QMap<quint8, quint32> frameHits;

        // Get frame pointer
        frame = this->gfx->getFrame(i);

        // Go through every pixels of the frame
        for (int jx = 0; jx < frame->getWidth(); jx++) {
            for (int jy = 0; jy < frame->getHeight(); jy++) {
                // Retrieve the color of the pixel
                paletteIndex = frame->getPixel(jx, jy).getPaletteIndex();

                // Add one hit to the frameHits and allFramesPalHits maps
                if (frameHits.contains(paletteIndex))
                    frameHits.insert(paletteIndex, frameHits.value(paletteIndex) + 1);
                else
                    frameHits.insert(paletteIndex, 1);

                if (frameHits.contains(paletteIndex))
                    this->allFramesPalHits.insert(paletteIndex, frameHits.value(paletteIndex) + 1);
                else
                    this->allFramesPalHits.insert(paletteIndex, 1);
            }
        }

        this->framePalHits[i] = frameHits;
    }
}

void D1PalHits::buildSubtilePalHits()
{
    // Go through all sub-tiles
    this->subtilePalHits.clear();
    for (int i = 0; i < this->min->getSubtileCount(); i++) {
        QMap<quint8, quint32> subtileHits;

        // Retrieve the CEL frame indices of the current sub-tile
        QList<quint16> &celFrameIndices = this->min->getCelFrameIndices(i);

        // Go through the CEL frames
        for (quint16 frameIndex : celFrameIndices) {
            frameIndex--;

            // Go through the hits of the CEL frame and add them to the subtile hits
            QMapIterator<quint8, quint32> it2(this->framePalHits.value(frameIndex));
            while (it2.hasNext()) {
                it2.next();
                subtileHits.insert(it2.key(), it2.value());
            }
        }

        this->subtilePalHits[i] = subtileHits;
    }
}

void D1PalHits::buildTilePalHits()
{
    // Go through all tiles
    this->tilePalHits.clear();
    for (int i = 0; i < this->til->getTileCount(); i++) {
        QMap<quint8, quint32> tileHits;

        // Retrieve the sub-tile indices of the current tile
        QList<quint16> &subtileIndices = this->til->getSubtileIndices(i);

        // Go through the sub-tiles
        for (quint16 subtileIndex : subtileIndices) {
            // Go through the hits of the sub-tile and add them to the tile hits
            QMapIterator<quint8, quint32> it2(this->subtilePalHits.value(subtileIndex));
            while (it2.hasNext()) {
                it2.next();
                tileHits.insert(it2.key(), it2.value());
            }
        }

        this->tilePalHits[i] = tileHits;
    }
}

quint32 D1PalHits::getIndexHits(quint8 colorIndex)
{
    if (this->mode == D1PALHITS_MODE::ALL_COLORS)
        return 1;

    if (this->mode == D1PALHITS_MODE::ALL_FRAMES) {
        if (!this->allFramesPalHits.contains(colorIndex))
            return 0;

        return this->allFramesPalHits[colorIndex];
    }

    return 0;
}

quint32 D1PalHits::getIndexHits(quint8 colorIndex, quint32 itemIndex)
{
    if (this->mode == D1PALHITS_MODE::CURRENT_TILE) {
        if (!this->tilePalHits.contains(itemIndex) || !this->tilePalHits[itemIndex].contains(colorIndex))
            return 0;

        return this->tilePalHits[itemIndex][colorIndex];
    } else if (this->mode == D1PALHITS_MODE::CURRENT_SUBTILE) {
        if (!this->subtilePalHits.contains(itemIndex) || !this->subtilePalHits[itemIndex].contains(colorIndex))
            return 0;

        return this->subtilePalHits[itemIndex][colorIndex];
    } else if (this->mode == D1PALHITS_MODE::CURRENT_FRAME) {
        if (!this->framePalHits.contains(itemIndex) || !this->framePalHits[itemIndex].contains(colorIndex))
            return 0;

        return this->framePalHits[itemIndex][colorIndex];
    }

    return 0;
}
