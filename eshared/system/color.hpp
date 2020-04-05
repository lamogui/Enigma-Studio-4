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
 *   Copyright � 2003-2012 Brain Control, all rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once
#include "extern/Enigma/eshared/system/types.hpp"
#include "sys_builddefines.h"

#if defined( PROUT_ENABLE_TLS )
enum eColorConst
{
    eCOL_RED,
    eCOL_GREEN,
    eCOL_BLUE,
    eCOL_ORANGE,
    eCOL_YELLOW,
    eCOL_PURPLE,
    eCOL_CYAN,
    eCOL_PINK,
    eCOL_WHITE,
    eCOL_BLACK,
    eCOL_GRAY,
    eCOL_DARKGRAY,
    eCOL_LIGHTGRAY,
 };
#endif // defined( PROUT_ENABLE_TLS )

// 8-bit per channel integer color
class eColor
{
public:
    eColor();
    eColor(eU8 nr, eU8 ng, eU8 nb, eU8 na=255);
#if defined( PROUT_ENABLE_TLS )
    eColor(eColorConst cc);
#endif // defined( PROUT_ENABLE_TLS )

    void        set(eU8 nr, eU8 ng, eU8 nb);
    void        set(eU8 nr, eU8 ng, eU8 nb, eU8 na);
    eU8         grayScale() const;
    void        toGrayScale();
    void        scale (const eColor &c);
    void        minComponents(const eColor &c);
    void        maxComponents(const eColor &c);
    void        average(const eColor &c);
    void        difference(const eColor &c);
    eColor      lerp(const eColor &to, eF32 t) const;
    eU32        toArgb() const;

    eColor      operator + (const eColor &c) const;
    eColor      operator - (const eColor &c) const;
    eColor      operator * (eF32 s) const;
    eColor &    operator += (const eColor &c);
    eColor &    operator -= (const eColor &c);
    eColor &    operator *= (eF32 s);
    eU8 &       operator [] (eInt index);
    eBool       operator == (const eColor &c) const;
    eBool       operator != (const eColor &c) const;

    const eU8 & operator [] (eInt index) const;

    friend eColor operator * (eF32 s, const eColor &c)
    {
        return c*s;
    }

public:
    // non-inlinable functions
    void toHsv(eInt &h, eInt &s, eInt &v);
    void fromHsv(eInt h, eInt s, eInt v);

public:
    union
    {
        struct
        {
            eU8    r; // LSB
            eU8    g;
            eU8    b;
            eU8    a; // MSB
        };

        eU32       abgr;
    };
};

