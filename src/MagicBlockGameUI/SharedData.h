#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mutex>
#include <vector>
#include <queue>

#include "Board.h"
#include "BoardString.h"
#include "Move.h"

static const UINT TargetX = 3;
static const UINT TargetY = 3;
static const UINT BoardX = 5;
static const UINT BoardY = 5;

template <UINT BoardX, UINT BoardY, UINT TargetX, UINT TargetY>
struct SharedData {
    HWND parent;
    std::mutex uiMutex;

    Board<TargetX, TargetY> targetBoard;
    Board<BoardX, BoardY>   playerBoard;

    BoardString<TargetX, TargetY>   targetStr;
    BoardString<BoardX, BoardY>     playerStr;

    std::vector<MoveInfo> move_path;

    SharedData() {}
    ~SharedData() {}
};
