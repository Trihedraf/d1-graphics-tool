## TODO

### Add
- Each PAL/TRN view features the following display filter check boxes
    - Show: Frame only
        - Sub-tile only?
        - Tile only?
    - Show: Hits only
    - Show: Translated only
- PAL write support
    - Color range selection
    - Open color picker when double-clicking a color
    - "Save" and "Save as" items in the "Palette" menu
- TRN write support
    - Translate individual color by double-clicking (or single-click plus "Translate" button) the color to translate and clicking the target color
    - Translate range by selecting the range to translate, click the "Translate" button and select the target range (range must be the same size)
    - "Save translation 1", "Save translation 2", "Save translation 1 as", "Save translation 2 as" items in the "Palette" menu
- Rescale the palette when changing screen depending on DPI
- Add settings
    - Background color? (grey, green, magenta, cyan?)
    - Default zoom level (depending on CEL/CL2 type?)
        - By default: x2
        - Automatic dezoom when opening a bigger image?
- PowerShell release script which takes a Qt build folder as parameter
- Application icon
- PCX support
- GIF support

### Change
- Remove palette hit views, instead add this functionality in the default palette view (dropdown?).
- Include CEL/CL2 to TRN mapping in the program, especially for monsters.
- Remove CelView and LevelCelView dependencies from PalView by leveraging signals/slots.
- Rewrite level CEL frame type detection to leverage associated MIN file when available.

### Fix
- Last color of PAL/TRN not displayed as transparent??
