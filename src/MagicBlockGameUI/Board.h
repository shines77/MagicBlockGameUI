#pragma once

#include <stdint.h>
#include <stddef.h>

template <UINT BoardX, UINT BoardY>
struct Board {
    static const UINT kGridSize = BoardX * BoardY;

    UINT grids[kGridSize];

    Board() {
        this->clear();
    }
    Board(const Board & other) {
        this->copy(other);
    }

    ~Board() {}

    Board & operator = (const Board & rhs) {
        if (this != & rhs) {
            for (UINT i = 0; i < kGridSize; i++) {
                this->grids[i] = rhs.grids[i];
            }
        }
        return *this;
    }

    void clear() {
        for (UINT n = 0; n < kGridSize; n++) {
            this->grids[n] = 0;
        }
    }

    void copy(const Board & other) {
        if (this != & other) {
            for (UINT n = 0; n < kGridSize; n++) {
                this->grids[n] = other.grids[n];
            }
        }
    }

    bool is_equal(const Board & other) const noexcept {
        for (UINT n = 0; n < kGridSize; n++) {
            if (this->grids[n] != other.grids[n])
                return false;
        }
        return true;
    }

    UINT getGrid(UINT x, UINT y) const {
        UINT pos = y * BoardY + x;
        return this->getGrid(pos);
    }

    UINT getGrid(UINT pos) const {
        ATLASSERT(pos >= 0 && pos < kGridSize);
        return this->grids[pos];
    }

    void setGrid(UINT x, UINT y, UINT color) {
        UINT pos = y * BoardY + x;
        return this->setGrid(pos, color);
    }

    void setGrid(UINT pos, UINT color) {
        ATLASSERT(pos >= 0 && pos < kGridSize);
        this->grids[pos] = color;
    }
};

template <UINT BoardX, UINT BoardY>
inline
bool operator == (const Board<BoardX, BoardY> & lhs, const Board<BoardX, BoardY> & rhs) noexcept  {
    return lhs.is_equal(rhs);
}

template <UINT BoardX, UINT BoardY>
inline
bool operator != (const Board<BoardX, BoardY> & lhs, const Board<BoardX, BoardY> & rhs) noexcept {
    return !(lhs.is_equal(rhs));
}
