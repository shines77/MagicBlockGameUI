#pragma once

#include <iostream>
#include <stack>
#include <string>

#include "Move.h"

#define WM_COMMAND_EXECUTE  (WM_USER + 0x01)

struct CmdType {
    enum T {
        Normal,
        Redo,
        Undo
    };
};

class Command
{
public:
    Command() {}
    virtual ~Command() {}

    virtual LRESULT Execute(HWND hwnd, CmdType::T cmdType = CmdType::Normal) = 0;
};

class MoveCommand final : public Command
{
public:
    typedef MoveInfo value_type;

private:
    value_type move_;

public:
    MoveCommand(const value_type & move) {
        this->move_ = move;
    }

    virtual ~MoveCommand() {}

    LRESULT Execute(HWND hwnd, CmdType::T cmdType = CmdType::Normal) final {
        LRESULT result = -1;
        if (hwnd != NULL && ::IsWindow(hwnd)) {
            result = ::SendMessage(hwnd, WM_COMMAND_EXECUTE, (WPARAM)cmdType, 0);
        }
        return result;
    }
};

class CommandManager
{
private:
    HWND                    hwnd_;
    std::stack<Command *>   undo_;
    std::stack<Command *>   redo_;

private:
    void ClearUndo() {
        while (!this->undo_.empty()) {
            delete this->undo_.top();
            this->undo_.pop();
        }
    }

    void ClearRedo() {
        while (!this->redo_.empty()) {
            delete this->redo_.top();
            this->redo_.pop();
        }
    }

public:
    CommandManager(HWND hwnd = NULL) : hwnd_(hwnd) {
    }

    virtual ~CommandManager() {
        this->ClearUndo();
        this->ClearRedo();
    }

    HWND GetOwner() const {
        return this->hwnd_;
    }

    void SetOwner(HWND hwnd) {
        this->hwnd_ = hwnd;
    }

    BOOL SetInitialBoard() {
        return TRUE;
    }

    BOOL AddCommand(Command * cmd) {
        if (cmd != NULL) {
            LRESULT result = cmd->Execute(this->hwnd_, CmdType::Normal);
            if (SUCCEEDED(result)) {
                this->undo_.push(cmd);
                this->ClearRedo();
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL MoveToFirst() {
        //
        return TRUE;
    }

    BOOL MoveToLast() {
        //
        return TRUE;
    }

    BOOL Undo() {
        if (this->undo_.empty()) {
            return FALSE;
        }

        Command * cmd = this->undo_.top();
        if (cmd != NULL) {
            this->redo_.push(cmd);
            this->undo_.pop();

            if (!this->undo_.empty()) {
                cmd = this->undo_.top();
                if (cmd != NULL) {
                    LRESULT result = cmd->Execute(this->hwnd_, CmdType::Undo);
                    return SUCCEEDED(result);
                }
            }
        }
        return FALSE;
    }

    BOOL Redo() {
        if (this->redo_.empty()) {
            return FALSE;
        }

        Command * cmd = this->redo_.top();
        if (cmd != NULL) {
            LRESULT result = cmd->Execute(this->hwnd_, CmdType::Redo);
            if (SUCCEEDED(result)) {
                this->undo_.push(cmd);
                this->redo_.pop();
                return TRUE;
            }
        }
        return FALSE;
    }
};
