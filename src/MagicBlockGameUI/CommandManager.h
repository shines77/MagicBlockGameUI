#pragma once

#include <iostream>
#include <stack>
#include <string>

#include "Move.h"

#define WM_COMMAND_EXECUTE  (WM_USER + 0x01)

struct CommandType {
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

    virtual LRESULT Execute(HWND hwnd, CommandType::T cmdType = CommandType::Normal) = 0;
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

    LRESULT Execute(HWND hwnd, CommandType::T cmdType = CommandType::Normal) final {
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
    std::stack<Command *>   undo_;
    std::stack<Command *>   redo_;
    HWND                    hwnd_;

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
    CommandManager(HWND hwnd) : hwnd_(hwnd) {
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
            LRESULT result = cmd->Execute(this->hwnd_);

            this->ClearRedo();
            this->undo_.push(cmd);
            return TRUE;
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

            cmd = this->undo_.top();
            LRESULT result = cmd->Execute(this->hwnd_, CommandType::Undo);
            return TRUE;
        }
        return FALSE;
    }

    BOOL Redo() {
        if (this->redo_.empty()) {
            return FALSE;
        }

        Command * cmd = this->redo_.top();
        if (cmd != NULL) {
            LRESULT result = cmd->Execute(this->hwnd_, CommandType::Redo);

            this->redo_.pop();
            this->undo_.push(cmd);
            return TRUE;
        }
        return FALSE;
    }
};
