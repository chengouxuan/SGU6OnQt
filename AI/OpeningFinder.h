#pragma once

#include <Windows.h>
#include <vector>
#include "UtilityTypes.h"

class OpeningFinder {

public:
    bool find(Board board, bool isBlack, DMove &dMove);
};
