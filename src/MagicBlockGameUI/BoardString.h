#pragma once

#include <stdint.h>
#include <stddef.h>

#include <string.h>
#include <string>

#include "Board.h"

template <UINT BoardX, UINT BoardY>
struct BoardString {
private:
    std::string str_;

public:
    BoardString() {}
    ~BoardString() {}

    bool import(const std::string & str) {
        this->str_ = str;
    }
};
