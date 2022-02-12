#include "FontReaderBitmapRef.h"

namespace aRibeiro {

    void FontReaderBitmapRef::read(aRibeiro::BinaryReader * reader) {
        top = reader->readInt16();
        left = reader->readInt16();
        bitmapBounds.read(reader);
    }

}
