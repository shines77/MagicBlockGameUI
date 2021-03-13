#pragma once

#ifndef NO_MINMAX
#define NO_MINMAX
#endif
#include <algorithm>    // For std::max()

static CRect rcScale9PSprite[9] = {
    // LeftTop
    { 1, 1, 5, 5 },
    // Top£Íiddle
    { 6, 1, 10, 5 },
    // RightTop
    { 11, 1, 15, 5 },
    // LeftMiddle
    { 1, 6, 5, 10 },
    // Center
    { 6, 6, 10, 10 },
    // RightMiddle
    { 11, 6, 15, 10 },
    // LeftBottom
    { 1, 11, 5, 16 },
    // BottomMiddle
    { 6, 11, 10, 16 },
    // RightBottom
    { 11, 11, 15, 16 }
};

static const COLORREF kDefaultBGColor = RGB(0xE8, 0xE8, 0xE8);

class Scale9Sprite
{
public:
    enum Location {
        First,
        LeftTop = First,
        TopMiddle,
        RightTop,
        LeftMiddle,
        Center,
        RightMiddle,
        LeftBottom,
        BottomMiddle,
        RightBottom,
        Last
    };

    static const COLORREF kDefaultBGColor = 0x12345678U;

private:
    HBRUSH          hBrushBG_;
    HBRUSH          hBrushBtnFace_;
    CBitmapHandle   sprite_;
    const CRect *   rects_;
    COLORREF        bgColor_;

public:
    Scale9Sprite() : hBrushBG_(NULL), hBrushBtnFace_(NULL), rects_(NULL), bgColor_(kDefaultBGColor) {
        this->hBrushBtnFace_ = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    }

    virtual ~Scale9Sprite() {
        this->Destroy();
    }

    void Destroy() {
        if (this->hBrushBG_ != NULL) {
            ::DeleteObject(this->hBrushBG_);
            this->hBrushBG_ = NULL;
        }
        if (this->hBrushBtnFace_ != NULL) {
            ::DeleteObject(this->hBrushBtnFace_);
            this->hBrushBtnFace_ = NULL;
        }
        this->sprite_ = NULL;
        this->rects_ = NULL;
    }

    void SetSprite(CBitmapHandle sprite, const CRect * rects, COLORREF bgColor) {
        this->sprite_ = sprite;
        this->rects_ = rects;
        this->bgColor_ = bgColor;
        if (this->hBrushBG_ != NULL) {
            ::DeleteObject(this->hBrushBG_);
        }
        this->hBrushBG_ = ::CreateSolidBrush(bgColor);
    }

    void DrawBackgroud(CDC & dc, CDC & dcMem, CRect & rect) {
        this->DrawBackgroud(dc.m_hDC, dcMem.m_hDC, rect);
    }

    void DrawBackgroud(CDCHandle dc, CDC & dcMem, CRect & rect) {
        this->DrawBackgroud(dc, dcMem.m_hDC, rect);
    }

    void DrawBackgroud(CDCHandle dc, CDCHandle dcMem, CRect & rect) {
        if (this->bgColor_ != kDefaultBGColor) {
            if (dc.m_hDC != NULL) {
                if (this->hBrushBG_ != NULL) {
                    CRect rcBG(rect);
                    rcBG.left += this->rects_[LeftMiddle].Width();
                    rcBG.top  += this->rects_[TopMiddle].Height();
                    rcBG.right  -= this->rects_[RightMiddle].Width();
                    rcBG.bottom -= this->rects_[BottomMiddle].Height();
                    dc.FillRect(&rcBG, this->hBrushBG_);
                }
            }
        }
    }

    void DrawFrame(CDC & dc, CDC & dcMem, CRect & rect) {
        this->DrawFrame(dc.m_hDC, dcMem.m_hDC, rect);
    }

    void DrawFrame(CDCHandle dc, CDC & dcMem, CRect & rect) {
        this->DrawFrame(dc, dcMem.m_hDC, rect);
    }

    void DrawFrame(CDCHandle dc, CDCHandle dcMem, CRect & rect) {
        if (this->sprite_.m_hBitmap != NULL && this->rects_ != NULL) {
            if (dc.m_hDC != NULL && dcMem.m_hDC != NULL) {
                HBITMAP hBitmapOld = dcMem.SelectBitmap(this->sprite_.m_hBitmap);
                if (hBitmapOld != NULL) {
                    // LeftTop
                    int nLeftTopWidth = (std::min)(this->rects_[LeftTop].Width(), rect.Width() / 2);
                    int nLeftTopHeight = (std::min)(this->rects_[LeftTop].Height(), rect.Height() / 2);
                    // RightTop
                    int nRightTopWidth = (std::min)(this->rects_[RightTop].Width(), rect.Width() - nLeftTopWidth);
                    int nRightTopHeight = (std::min)(this->rects_[RightTop].Height(), rect.Height() - nLeftTopHeight);
                    // LeftBottom
                    int nLeftBottomWidth = (std::min)(this->rects_[LeftBottom].Width(), rect.Width() / 2);
                    int nLeftBottomHeight = (std::min)(this->rects_[LeftBottom].Height(), rect.Height() / 2);
                    // RightBottom
                    int nRightBottomWidth = (std::min)(this->rects_[RightBottom].Width(), rect.Width() - nLeftBottomWidth);
                    int nRightBottomHeight = (std::min)(this->rects_[RightBottom].Height(), rect.Height() - nLeftBottomHeight);

                    // TopMiddle
                    int nTopM_Width = rect.Width() - (this->rects_[LeftTop].Width() + this->rects_[RightTop].Width());
                    int nTopM_Height = (std::min)(this->rects_[TopMiddle].Height(), rect.Height() / 2);
                    // LeftMiddle
                    int nLeftM_Width = (std::min)(this->rects_[LeftMiddle].Width(), rect.Width() / 2);
                    int nLeftM_Height = rect.Height() - (this->rects_[LeftTop].Height() + this->rects_[LeftBottom].Height());
                    // RightMiddle
                    int nRightM_Width = (std::min)(this->rects_[RightMiddle].Width(), rect.Width() / 2);
                    int nRightM_Height = rect.Height() - (this->rects_[RightTop].Height() + this->rects_[RightBottom].Height());
                    // BottomMiddle
                    int nBottomM_Width = rect.Width() - (this->rects_[LeftBottom].Width() + this->rects_[RightBottom].Width());
                    int nBottomM_Height = (std::min)(this->rects_[BottomMiddle].Height(), rect.Height() / 2);

                    // TopMiddle
                    if (nTopM_Width > 0 && nTopM_Height > 0) {
                        BOOL paintOK = dc.StretchBlt(rect.left + nLeftTopWidth,
                                                     rect.top,
                                                     nTopM_Width,
                                                     nTopM_Height,
                                                     dcMem.m_hDC,
                                                     this->rects_[TopMiddle].left,
                                                     this->rects_[TopMiddle].top,
                                                     this->rects_[TopMiddle].Width(),
                                                     nTopM_Height,
                                                     SRCCOPY);
                    }

                    // LeftMiddle
                    if (nLeftM_Width > 0 && nLeftM_Height > 0) {
                        BOOL paintOK = dc.StretchBlt(rect.left,
                                                     rect.top + nLeftTopHeight,
                                                     nLeftM_Width,
                                                     nLeftM_Height,
                                                     dcMem.m_hDC,
                                                     this->rects_[LeftMiddle].left,
                                                     this->rects_[LeftMiddle].top,
                                                     nLeftM_Width,
                                                     this->rects_[LeftMiddle].Height(),
                                                     SRCCOPY);
                    }

                    // RightMiddle
                    if (nRightM_Width > 0 && nRightM_Height > 0) {
                        BOOL paintOK = dc.StretchBlt(rect.right - nRightM_Width,
                                                     rect.top + nRightTopHeight,
                                                     nRightM_Width,
                                                     nRightM_Height,
                                                     dcMem.m_hDC,
                                                     this->rects_[RightMiddle].right - nRightM_Width,
                                                     this->rects_[RightMiddle].top,
                                                     nRightM_Width,
                                                     this->rects_[RightMiddle].Height(),
                                                     SRCCOPY);
                    }

                    // BottomMiddle
                    if (nBottomM_Width > 0 && nBottomM_Height > 0) {
                        BOOL paintOK = dc.StretchBlt(rect.left + nLeftBottomWidth,
                                                     rect.bottom - nLeftBottomHeight,
                                                     nBottomM_Width,
                                                     nBottomM_Height,
                                                     dcMem.m_hDC,
                                                     this->rects_[BottomMiddle].left,
                                                     this->rects_[BottomMiddle].bottom - nBottomM_Height,
                                                     this->rects_[BottomMiddle].Width(),
                                                     nBottomM_Height,
                                                     SRCCOPY);
                    }

                    // LeftTop
                    if (nLeftTopWidth > 0 && nLeftTopHeight > 0) {
                        BOOL paintOK = dc.BitBlt(rect.left,
                                                 rect.top,
                                                 nLeftTopWidth,
                                                 nLeftTopHeight,
                                                 dcMem.m_hDC,
                                                 this->rects_[LeftTop].left,
                                                 this->rects_[LeftTop].top,
                                                 SRCCOPY);
                    }

                    // RightTop
                    if (nRightTopWidth > 0 && nRightTopHeight > 0) {
                        BOOL paintOK = dc.BitBlt(rect.right - nRightTopWidth,
                                                 rect.top,
                                                 nRightTopWidth,
                                                 nRightTopHeight,
                                                 dcMem.m_hDC,
                                                 this->rects_[RightTop].left,
                                                 this->rects_[RightTop].top,
                                                 SRCCOPY);
                    }

                    // LeftBottom
                    if (nLeftBottomWidth > 0 && nLeftBottomHeight > 0) {
                        BOOL paintOK = dc.BitBlt(rect.left,
                                                 rect.bottom - nLeftBottomHeight,
                                                 nLeftBottomWidth,
                                                 nLeftBottomHeight,
                                                 dcMem.m_hDC,
                                                 this->rects_[LeftBottom].left,
                                                 this->rects_[LeftBottom].top,
                                                 SRCCOPY);
                    }

                    // RightBottom
                    if (nRightBottomWidth > 0 && nRightBottomHeight > 0) {
                        BOOL paintOK = dc.BitBlt(rect.right - nRightBottomWidth,
                                                 rect.bottom - nRightBottomHeight,
                                                 nRightBottomWidth,
                                                 nRightBottomHeight,
                                                 dcMem.m_hDC,
                                                 this->rects_[RightBottom].left,
                                                 this->rects_[RightBottom].top,
                                                 SRCCOPY);
                    }

                    dcMem.SelectBitmap(hBitmapOld);
                    hBitmapOld = NULL;
                }

                // Draw corner edge
                if (this->hBrushBtnFace_ != NULL) {
                    CRect rcPoint = rect;

                    rcPoint.left = rect.left;
                    rcPoint.right = rect.left + 1;
                    rcPoint.top = rect.top;
                    rcPoint.bottom = rect.top + 1;
                    dc.FillRect(&rcPoint, this->hBrushBtnFace_);

                    rcPoint.left = rect.right - 1;
                    rcPoint.right = rect.right;
                    rcPoint.top = rect.top;
                    rcPoint.bottom = rect.top + 1;
                    dc.FillRect(&rcPoint, this->hBrushBtnFace_);

                    rcPoint.left = rect.left;
                    rcPoint.right = rect.left + 1;
                    rcPoint.top = rect.bottom - 1;
                    rcPoint.bottom = rect.bottom;
                    dc.FillRect(&rcPoint, this->hBrushBtnFace_);

                    rcPoint.left = rect.right - 1;
                    rcPoint.right = rect.right;
                    rcPoint.top = rect.bottom - 1;
                    rcPoint.bottom = rect.bottom;
                    dc.FillRect(&rcPoint, this->hBrushBtnFace_);
                }
            }
        }        
    }
};
