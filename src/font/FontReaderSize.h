#ifndef fontreader_size_h__
#define fontreader_size_h__

#include <aRibeiroCore/aRibeiroCore.h>
#include <aRibeiroData/BinaryWriter.h>
#include <aRibeiroData/BinaryReader.h>

namespace aRibeiro {

    /// \brief Store a size information (width, height)
    ///
    /// \author Alessandro Ribeiro
    ///
    struct FontReaderSize {
        uint32_t w;///<width
        uint32_t h;///<height

        /// \brief Read the size information from a #aRibeiro::BinaryReader
        ///
        /// Example:
        ///
        /// \code
        /// #include <aRibeiroCore/aRibeiroCore.h>
        /// #include <aRibeiroData/aRibeiroData.h>
        /// using namespace aRibeiro;
        ///
        /// FontReaderSize size = ...;
        ///
        /// ...
        ///
        /// BinaryReader reader;
        /// reader.readFromFile("file.input");
        ///
        /// size.read(&reader);
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
