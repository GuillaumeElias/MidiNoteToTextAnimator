/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Courier__Prime_Sans_ttf;
    const int            Courier__Prime_Sans_ttfSize = 118012;

    extern const char*   fullscbutton_png;
    const int            fullscbutton_pngSize = 611;

    extern const char*   fullscbuttonback_png;
    const int            fullscbuttonback_pngSize = 620;

    extern const char*   restartbutton_png;
    const int            restartbutton_pngSize = 545;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 4;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
