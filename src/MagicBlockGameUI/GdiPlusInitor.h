#pragma once

#include <gdiplus.h>

class GdiPlusInitor {
public:
    static ULONG_PTR s_gdiplusToken;

    GdiPlusInitor() {
    }

    ~GdiPlusInitor() {
    }

    static BOOL Startup() {
        if (s_gdiplusToken == NULL) {
            Gdiplus::Status status;
            Gdiplus::GdiplusStartupInput StartupInput = { 0 };
            Gdiplus::GdiplusStartupOutput StartupOutput = { 0 };
            StartupInput.GdiplusVersion = 1;
            StartupInput.SuppressBackgroundThread = FALSE;
            StartupInput.SuppressExternalCodecs = FALSE;
            status = Gdiplus::GdiplusStartup(&s_gdiplusToken, &StartupInput, &StartupOutput);
            return (status == Gdiplus::Ok);
        }

        return FALSE;
    }

    static void Shutdown() {
        if (s_gdiplusToken != NULL) {
            Gdiplus::GdiplusShutdown(s_gdiplusToken);
            s_gdiplusToken = NULL;
        }
    }
};
