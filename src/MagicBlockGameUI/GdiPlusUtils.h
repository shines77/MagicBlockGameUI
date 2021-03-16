#pragma once

#include <gdiplus.h>
#include <atlimage.h>

namespace gdiplus {

static LONG _gdiplus_errno = 0;

static LONG GetGdiPlusLastError()
{
    return _gdiplus_errno;
}

static void SetGdiPlusLastError(LONG error)
{
    _gdiplus_errno = error;
}

//
// See: http://www.voidcn.com/article/p-bygtnejy-zr.html
//
static Gdiplus::Image * LoadFromResource(LPCTSTR lpName, LPCTSTR lpType)
{
    HRSRC hResInfo = ::FindResource(_Module.get_m_hInst(), lpName, lpType);
    if (hResInfo == NULL) {
        gdiplus::SetGdiPlusLastError(-1);
        return NULL;
    }

    HGDIOBJ hResData = ::LoadResource(_Module.get_m_hInst(), hResInfo);
    if (hResData == NULL) {
        gdiplus::SetGdiPlusLastError(-2);
        return NULL;
    }

    DWORD dwResSize = ::SizeofResource(_Module.get_m_hInst(), hResInfo);
    if (dwResSize == 0) {
        gdiplus::SetGdiPlusLastError(-3);
        return NULL;
    }

    LPVOID lpResData = ::LockResource(hResData);
    if (lpResData == NULL) {
        gdiplus::SetGdiPlusLastError(-4);
        return NULL;
    }

    HGLOBAL hResBuffer = ::GlobalAlloc(GMEM_MOVEABLE, dwResSize);
    if (hResBuffer == NULL) {
        ::GlobalUnlock(hResData);
        gdiplus::SetGdiPlusLastError(-5);
        return NULL;
    }

    LPVOID lpResBuffer = ::GlobalLock(hResBuffer);
    if (lpResBuffer == NULL) {
        ::GlobalUnlock(hResBuffer);
        ::GlobalFree(hResBuffer);
        ::GlobalUnlock(hResData);
        gdiplus::SetGdiPlusLastError(-6);
        return NULL;
    }

    ::CopyMemory(lpResBuffer, lpResData, dwResSize);

    ::GlobalUnlock(hResData);

    Gdiplus::Image * pImage = NULL;

    LPSTREAM lpStream = NULL;
    if (::CreateStreamOnHGlobal(hResBuffer, FALSE, &lpStream) == S_OK) {
        if (lpStream != NULL) {
            pImage = Gdiplus::Image::FromStream(lpStream, FALSE);
            lpStream->Release();

            if (pImage)
                gdiplus::SetGdiPlusLastError(0);
            else
                gdiplus::SetGdiPlusLastError(-7);
        }
        else {
            gdiplus::SetGdiPlusLastError(-8);
        }
    }
    else {
        gdiplus::SetGdiPlusLastError(-9);
    }

    //::GlobalUnlock(hResBuffer);
    //::GlobalFree(hResBuffer);
    return pImage;
}

static Gdiplus::Image * LoadFromResource(UINT nResID, LPCTSTR lpType)
{
    LPCTSTR lpResName = MAKEINTRESOURCE(nResID);
    return gdiplus::LoadFromResource(lpResName, lpType);
}

static LONG DestroyImage(Gdiplus::Image * pImage)
{
    if (pImage != NULL) {
        delete pImage;
    }
    return 0;
}

} // namespace gdiplus
