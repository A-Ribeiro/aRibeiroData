#include "FontReaderGlyph.h"


namespace aRibeiro {
    void FontReaderGlyph::read(aRibeiro::BinaryReader * reader) {
        charcode = reader->readUInt32();
        advancex = reader->readFloat();
        face.read(reader);
        stroke.read(reader);
    }
}
