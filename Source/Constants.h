#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

static const StringArray FONTS = {
    "Arial",
    "Courier New",
    "Comic Sans MS",
    "Garamond"
    "Georgia",
    "Times New Roman",
    "Verdana"
};

static const StringArray JUSTIFICATIONS = {
    "left",
    "right",
    "centre",
    "center left",
    "center right",
    "bottom left",
    "centred bottom",
    "bottom right",
    "top left",
    "top right",
    "centre top"
};

static const int JUSTIFICATIONS_ARRAY_FLAGS [] = {
    Justification::left,
    Justification::right,
    Justification::centred,
    Justification::centredLeft,
    Justification::centredRight,
    Justification::bottomLeft,
    Justification::centredBottom,
    Justification::bottomRight,
    Justification::topLeft,
    Justification::topRight,
    Justification::centredTop
};