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
// global constants (don't change into
// constants, it's a size thing!)

#define eSQRT2          1.41421356237f
#define ePI             3.1415926535897932384626433832795f
#define eTWOPI          (ePI*2.0f)
#define eHALFPI         (ePI*0.5f)
#define eSQRPI          (ePI*ePI)
#define eINVHALFPI      (1.0f/eHALFPI)
#define eEXPONE         2.718281828459f
#define eALMOST_ZERO    0.00001f
#define eMAX_RAND       2147483647
#define eMAX_NAME_LEN   64
#define eMAX_PATH_LEN   256

// function macros

#define eELEMENT_COUNT(a)    (sizeof(a)/sizeof(a[0]))
#define eASSERT_ALIGNED16(x) eASSERT(eU32(x)%16 == 0)

#ifdef eDEBUG
    #define eASSERT(expr)                                   \
    {                                                       \
        if (!(expr))                                        \
            if (eShowAssertion(#expr, __FILE__, __LINE__))  \
                __asm int 3                                 \
    }
#else   
    #define eASSERT(x)
#endif

// overloaded new and delete operators. WinAPI
// versions for release build and memory tracking
// versions for debug build.
#if 1 //defined(eRELEASE) && defined(ePLAYER)
    ePtr eCDECL operator new(eU32 size);
    ePtr eCDECL operator new [] (eU32 size);
    void eCDECL operator delete(ePtr ptr);
    void eCDECL operator delete [] (ePtr ptr);
#else
    // warning: assumption here is that if there is a
    // symbol multiply defined in an .obj and a .lib
    // file, the compiler uses the symbol from the
    // .obj file. this is especially important when
    // externally linking 3rd-party libraries
    #undef new

    ePtr eCDECL operator new(eU32 size, const eChar *file, eU32 line);
    ePtr eCDECL operator new [] (eU32 size, const eChar *file, eU32 line);
    ePtr eCDECL operator new(eU32 size);

    void eCDECL operator delete(ePtr ptr);
    void eCDECL operator delete [] (ePtr ptr);

    // also overloaded delete operators to assure
    // that memory is freed when initialization
    // throws an exception
    void eCDECL operator delete (ePtr ptr, const eChar *file, eU32 line);
    void eCDECL operator delete [] (ePtr ptr, const eChar *file, eU32 line);

    #define new new(__FILE__, __LINE__)
#endif

#define eALLOC_STACK(type, count)  ((type *)_alloca((count)*sizeof(type))) // macro because it has to get inlined!

// non-inlineable functions

typedef void (* eLogHandler)(const eChar *msg, ePtr param);

#ifndef ePLAYER
eU64    eGetAllocatedMemory();
eU64    eGetTotalVirtualMemory();
void    eSetLogHandler(eLogHandler logHandler, ePtr param);
#endif

void    eWriteToLog(const eChar *msg);
void    eLeakDetectorStart();
void    eLeakDetectorStop();
eBool   eShowAssertion(const eChar *exp, const eChar *file, eU32 line);
void    eShowError(const eChar *error);
void    eFatal(eU32 exitCode);
ePtr    eAllocAlignedAndZero(eU32 size, eU32 alignment);
void    eFreeAligned(ePtr ptr);
ePtr    eMemRealloc(ePtr ptr, eU32 oldLength, eU32 newLength);
void    eMemSet(ePtr dst, eU8 val, eU32 count);
void    eMemCopy(ePtr dst, eConstPtr src, eU32 count);
void    eMemMove(ePtr dst, eConstPtr src, eU32 count);
eBool   eMemEqual(eConstPtr mem0, eConstPtr mem1, eU32 count);
void    eStrClear(eChar *str);
void    eStrCopy(eChar *dst, const eChar *src);
void    eStrNCopy(eChar *dst, const eChar *src, eU32 count);
eChar * eStrClone(const eChar *str);
eU32    eStrLength(const eChar *str);
eChar * eStrAppend(eChar *dst, const eChar *src);
eInt    eStrCompare(const eChar *str0, const eChar *str1);
eChar * eStrUpper(eChar *str);
eChar * eIntToStr(eInt val);
eChar * eFloatToStr(eF32 val);
eInt    eStrToInt(const eChar *str);
eF32    eStrToFloat(const eChar *str);
eBool   eIsAlphaNumeric(eChar c);
eBool   eIsDigit(eChar c);
eBool   eIsAlpha(eChar c);
void    eRandomize(eU32 seed);
eU32    eRandom();
eU32    eRandom(eU32 &seed);
eU32    eRandomSeed();
eF32    ePow(eF32 base, eF32 exp);
eF32    eSinH(eF32 x);
eF32    eCosH(eF32 x);
eF32    eTanH(eF32 x);
eF32    eASin(eF32 x);
eF32    eACos(eF32 x);
eF32    eExp(eF32 x);
eInt    eFloor(eF32 x);
eInt    eCeil(eF32 x);
eBool   eIsNumber(eF32 x);
eF32    eLog10(eF32 x);
eF32    eLog2(eF32 x);
eF32    eLn(eF32 x);
eF32    eSin(eF32 x);
eF32    eCos(eF32 x);
eF32    eTan(eF32 x);
eF32    eCot(eF32 x);
eF32    eATan(eF32 x);
eF32    eATan2(eF32 y, eF32 x);
eF32    eATanh(eF32 x);
void    eSinCos(eF32 x, eF32 &sine, eF32 &cosine);
eF32    eSqrt(eF32 x);
eF32    eInvSqrt(eF32 x);
eF32    eMod(eF32 a, eF32 b);
eBool   eIsFloatZero(eF32 x);
eBool   eAreFloatsEqual(eF32 x, eF32 y);
eBool   eIsNan(eF32 x);
eF32    eDegToRad(eF32 degrees);
eF32    eRadToDeg(eF32 radians);
eBool   eIsAligned(eConstPtr data, eU32 alignment);
eU32    eHashInt(eInt key);
eU32    eHashStr(const eChar *str);
eF32    eRound(eF32 x);
eU32    eRoundToMultiple(eU32 x, eU32 multiple);
eInt    eTrunc(eF32 x);
eInt    eRandom(eInt min, eInt max);
eInt    eRandom(eInt min, eInt max, eU32 &seed);
eF32    eRandomF();
eF32    eRandomF(eU32 &seed);
eF32    eRandomF(eF32 min, eF32 max);
eF32    eRandomF(eF32 min, eF32 max, eU32 &seed);
eU32    eNextPowerOf2(eU32 x);
eBool   eIsPowerOf2(eU32 x);
eBool   eClosedIntervalsOverlap(eInt start0, eInt end0, eInt start1, eInt end1);
eU16    eLoword(eU32 x);
eU16    eHiword(eU32 x);
eU8     eLobyte(eU16 x);
eU8     eHibyte(eU16 x);
eU32    eMakeDword(eU16 lo, eU16 hi);
eU16    eMakeWord(eU8 lo, eU8 hi);

#ifdef PROUT_IMGUI
eF64	eAToF( const eChar * _str );
eInt	eMemCompare( const void * _s1, const void * _s2, eInt _n );
void *	eMemChr( register const void * _src_void, int _c, size_t _length );
eChar	eToUpper( eChar _c );
int eStrNCompare(const eChar * _s1, const eChar * _s2, register size_t _n);
eChar * eStrStr( const eChar * s1, const eChar * s2 );
eBool	eIsSpace( eChar c );

#endif // PROUT_IMGUI

#define eLoword(x)          (eU16)(x&0xffff)
#define eHiword(x)          (eU16)((x>>16)&0xffff)
#define eLobyte(x)          (eU8)(x&0xff)
#define eHibyte(x)          (eU8)((x>>8)&0xff)
#define eMakeDword(lo, hi)  (((eU32)lo)|(((eU32)hi)<<16))
#define eMakeWord(lo, hi)   (((eU8)lo)|(((eU8)hi)<<8))

#define eVector3ToStr(val)  (eString("") + eFloatToStr(val.x) + eString(",") + eFloatToStr(val.y) + eString(",") + eFloatToStr(val.z))
#define eVector4ToStr(val)  (eString("") + eFloatToStr(val.x) + eString(",") + eFloatToStr(val.y) + eString(",") + eFloatToStr(val.z) + eString(",") + eFloatToStr(val.w))

// inlineable functions

eFORCEINLINE eF32 eAbs(eF32 x)
{
    __asm
    {
        fld     dword ptr [x]
        fabs
        fstp    dword ptr [x]
    }

    return x;
}

eFORCEINLINE eF64 eAbs(eF64 x)
{
	__asm
	{
		fld     qword ptr[x]
		fabs
		fstp    qword ptr[x]
	}

	return x;
}

eFORCEINLINE eU32 eAbs(eInt x)
{
    return ((x^(x>>31))-(x>>31));
}

// faster float to long conversion than c-lib's
// default version. must be called explicitly.
eFORCEINLINE eInt eFtoL(eF32 x)
{
    __asm
    {
        fld     dword ptr [x]
        push    eax
        fistp   dword ptr [esp]
        pop     eax
    }
}

eFORCEINLINE eU64 eDtoULL(eF64 x)
{
    __asm
    {
        fld     dword ptr [x]
        push    eax
        fistp   dword ptr [esp]
        pop     eax
    }
}

eINLINE eU32 eSignBit(eF32 x)
{
    return (eU32 &)x&0x80000000;
}

eINLINE eF32 eSign(eF32 x)
{
    // test exponent and mantissa bits: is input zero?
    if (((eInt &)x&0x7fffffff) == 0)
        return 0.0f;

    // mask sign bit in x, set it in r if necessary
    eF32 r = 1.0f;
    (eInt &)r |= eSignBit(x);
    return r;
}

eINLINE void eUndenormalise(eF32 &sample)
{
    if (((*(eU32 *)&sample)&0x7f800000) == 0)
        sample = 0.0f;
}

eINLINE eInt eSign(eInt x)
{
    return (x != 0)|(x>>(sizeof(eInt)*8-1));
}

template<class T> eU32 eHashPtr(const T * const &ptr)
{
    return eHashInt((eInt)ptr);
}

template<class T> void eSetBit(T &t, eU32 index)
{
    eASSERT(index < sizeof(T)*8);
    t |= (1<<index);
}

template<class T> void eSetBit(T &t, eU32 index, eBool set)
{
    eASSERT(set == 0 || set == 1);

    eASSERT(index <sizeof(T)*8);
    t |= (set<<index);
}

template<class T> void eClearBit(T &t, eU32 index)
{
    eASSERT(index < sizeof(T)*8);
    t &= ~(1<<index);
}

template<class T> eBool eGetBit(T t, eU32 index)
{
    eASSERT(index < sizeof(T)*8);
    return ((t&(1<<index)) != 0);
}

template<class T> void eToggleBit(T &t, eU32 index)
{
    eASSERT(index < sizeof(T)*8);
    t ^= (1<<index);
}

template<class T> void eDelete(T &ptr)
{
    delete ptr;
    ptr = nullptr;
}

template<class T> void eDeleteArray(T &ptr)
{
    delete [] ptr;
    ptr = nullptr;
}

template<class T> void eReleaseCom(T &ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}

template<class T> void eSwap(T &a, T &b)
{
    T c = a;
    a = b;
    b = c;
}

template<class T> T eMin(const T &a, const T &b)
{
    return (a < b) ? a : b;
}

template<class T> T eMax(const T &a, const T &b)
{
    return (a > b) ? a : b;
}

template<class T> T eClamp(const T &min, const T &x, const T &max)
{
    if (x < min)
        return min;
    else if (x > max)
        return max;
    else
        return x;
}

template<class T> T eLerp(const T &a, const T &b, eF32 t)
{
    return a+(b-a)*t;
}

template<class T> T eSqr(const T &x)
{
    return x*x;
}

template<class T> T eAlign(const T &val, eU32 alignment)
{
    static_assert(sizeof(val) <= sizeof(alignment), "doesn't work correctly if sizeof(alignment) < sizeof(val)");
    return (T)((((eU32)val)+alignment-1)&(~(alignment-1)));
}

template<class T> eINLINE eBool eInRange(const T &x, const T &min, const T &max)
{
    return (x >= min && x <= max);
}
