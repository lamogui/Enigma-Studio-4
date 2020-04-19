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

#pragma once
#include "sys_builddefines.h"
#include "extern/Enigma/eshared/system/types.hpp"
#include "extern/Enigma/eshared/system/point.hpp"

#define PROUT_TARGET_RESOLUTION_WIDTH 1920
#define PROUT_TARGET_RESOLUTION_HEIGHT 1080

#ifndef PROUT_FULLSCREEN_ONLY
struct eSetup
{
    eSetup() :
        vsync( true ),
        fullScreen( false ),
        res(PROUT_TARGET_RESOLUTION_WIDTH, PROUT_TARGET_RESOLUTION_HEIGHT)
    {
    }

    eSize   res;
    eBool   vsync;
    eBool   fullScreen;
};

#ifdef PROUT_SHOWSETUP_DIALOG

// returns false, if config dialog was canceled,
// otherwise true is returned (=> start demo)
eBool eShowSetupDialog(eSetup &setup);

#endif //PROUT_SHOWSETUP_DIALOG
#endif // !PROUT_FULLSCREEN_ONLY