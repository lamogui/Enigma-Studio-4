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

struct eSetup
{
    eSetup() :
        vsync(eFALSE),
        fullScreen(eFALSE),
        res(1280, 720)
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
