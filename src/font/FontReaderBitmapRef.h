#ifndef fontreader_bitmap_ref_h__
#define fontreader_bitmap_ref_h__

#include <aRibeiroCore/aRibeiroCore.h>
#include <aRibeiroData/BinaryWriter.h>
#include <aRibeiroData/BinaryReader.h>
#include <aRibeiroData/AtlasRect.h>

namespace aRibeiro {

    /// \brief Store a glyph render information
    ///
    /// Each glyph needs a starting point reference (top,left) and <br />
    /// the sprite rectangle inside the atlas (bitmapBounds).
    ///
    /// With this information you can render non-monospace character types.
    ///
    /// \author Alessandro Ribeiro
    ///
    struct FontReaderBitmapRef {
        int16_t top;///< glyph origin top (y coord).
        int16_t left;///< glyph origin left (x coord).
        AtlasRect bitmapBounds;///< sprite rectangle inside the atlas.

        /// \brief Read this glyph metrics from a #aRibeiro::BinaryReader
        ///
        /// Example:
        ///
        /// \code
        /// #include <aRibeiroCore/aRibeiroCore.h>
        /// #include <aRibeiroData/aRibeiroData.h>
        /// using namespace aRibeiro;
        ///
        /// FontReaderBitmapRef bitmapRef = ...;
        ///
        /// ...
        ///
        /// BinaryReader reader;
        /// reader.readFromFile("file.input");
        ///
        /// bitmapRef.read(&reader);
        ///
        /// reader.close();
        /// \endcode
        ///
        /// \author Alessandro Ribeiro
        /// \param reader The #aRibeiro::BinaryReader instance
        ///
        void read(aRibeiro::BinaryReader * reader);
    };

}

#endif
