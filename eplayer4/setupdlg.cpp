/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   This file is part of
 *       ______        _                             __ __
 *      / ____/____   (_)____ _ ____ ___   ____ _   / // /
 *     / __/  / __ \ / // __ `// __ `__ \ / __ `/  / // /_
 *    / /___ / / / // // /_/ // / / / / // /_/ /  /__  __/
 *   /_____//_/ /_//_/ \__, //_/ /_/ /_/ \__,_/     /_/.   
 *                    /____/                              
 *
 *   Copyright © 2003-2012 Brain Control, all rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "extern/Enigma/eplayer4/setupdlg.hpp"

#ifdef PROUT_SHOWSETUP_DIALOG
#include "system/sys_assert.hpp"
#include "extern/Enigma/eshared/system/runtime.hpp"
#include "extern/Enigma/eshared/system/rect.hpp"
#include "extern/Enigma/eshared/system/array.hpp"

#ifdef PROUT_WIN32
#include "platforms/win32/sys_file_win32.hpp"
#endif // PROUT_WIN32

#include "renderbase/Graphics.hpp"

enum eSetupDlgWidgetInfos
{
    eBTN_WIDTH  = 135,
    eBTN_HEIGHT = 25,
    eCB_HEIGHT  = 20,
};

static HWND createButton(const eChar *text, eBool checkBox, const eRect &r, HWND parent)
{
    HWND hwnd = CreateWindowA("button", text, WS_CHILD|WS_VISIBLE|(checkBox ? BS_AUTOCHECKBOX : 0),
                             r.left, r.top, r.getWidth(), r.getHeight(), parent, nullptr, nullptr, nullptr);
    passert(hwnd, "Setup Window creation failed !" );
    SendMessage(hwnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(1, 0));
    return hwnd;
}

static LRESULT CALLBACK dlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static eArray<HWND> resBtns;
    static HWND windowedBtn, vsyncBtn;
    static eSetup *setup = nullptr;

    if (msg == WM_CREATE)
    {
        setup = (eSetup *)((CREATESTRUCT *)lparam)->lpCreateParams;

        for (eU32 i=0; i<eGfx->getResolutionCount(); i++)
        {
            const eSize &res = eGfx->getResolution(i);
            eChar buf[32];
            eStrCopy(buf, eIntToStr(res.width));
            eStrAppend(buf, "  x  ");
            eStrAppend(buf, eIntToStr(res.height));

            HWND btn = createButton(buf, eFALSE, eRect(0, i*eBTN_HEIGHT, eBTN_WIDTH, (i+1)*eBTN_HEIGHT), hwnd);
            resBtns.append(btn);
        }

        const eU32 y = eGfx->getResolutionCount()*eBTN_HEIGHT;
        windowedBtn = createButton("Windowed", eTRUE, eRect(eCB_HEIGHT, y, eBTN_WIDTH, y+eBTN_HEIGHT), hwnd);
        vsyncBtn = createButton("V-Sync", eTRUE, eRect(eCB_HEIGHT, y+eCB_HEIGHT, eBTN_WIDTH, y+2*eBTN_HEIGHT), hwnd);
        return 0;
    }
    else if (msg == WM_COMMAND)
    {
        if (HIWORD(wparam) == BN_CLICKED)
        {
            for (eU32 i=0; i<resBtns.size(); i++)
            {
                if (lparam == (LPARAM)resBtns[i])
                {
                    setup->fullScreen = (SendMessage(windowedBtn, BM_GETCHECK, 0, 0) != BST_CHECKED);
                    setup->vsync = (SendMessage(vsyncBtn, BM_GETCHECK, 0, 0) != BST_CHECKED);
                    setup->res = eGfx->getResolution(i);
                    DestroyWindow(hwnd);
                    PostQuitMessage(IDOK);
                }
            }
        }

        return 0;
    }
    else if (msg == WM_DESTROY)
    {
        PostQuitMessage(IDCANCEL);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

eBool eShowSetupDialog( eSetup &setup )
{
    WNDCLASSA wc;
    eMemSet(&wc, 0, sizeof(wc));
    wc.style = CS_HREDRAW|CS_VREDRAW;
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.lpfnWndProc = dlgProc;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "Demo setup";

    RECT r;
    r.left = r.top = 0;
    r.right = eBTN_WIDTH;
    r.bottom = eGfx->getResolutionCount()*eBTN_HEIGHT+2*eCB_HEIGHT+5;
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

    const ATOM res = RegisterClassA(&wc);
    passert(res,"Failed to register class Demo setup");
    const HWND hwnd = CreateWindowA("Demo Setup", "Prout Engine", WS_SYSMENU|WS_VISIBLE,
                                   CW_USEDEFAULT, CW_USEDEFAULT, r.right-r.left,
                                   r.bottom-r.top, NULL, NULL, NULL, (ePtr)&setup);
		passert(hwnd, "Failed to create window for demo setup" );
    MSG msg;
    
    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    while (msg.message != WM_QUIT);

    // has user clicked a resolution button or not?
    return (msg.wParam == IDOK ? eTRUE : eFALSE);

}

#endif // PROUT_SHOWSETUP_DIALOG