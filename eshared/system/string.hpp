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
#include "extern/Enigma/eshared/system/list.hpp"

// helper template structs for compile time string hashes
// (taken from "Quasi compile-time string hashing" article)

template<eU32 N, eU32 I> struct eFnvHash
{
    eINLINE static eU32 hash(const eChar (&str)[N])
    {
        return (eFnvHash<N, I-1>::hash(str)^str[I-1])*16777619;
    }
};

template<eU32 N> struct eFnvHash<N, 1>
{
    eINLINE static eU32 hash(const eChar (&str)[N])
    {
        return (2166136261^str[0])*16777619;
    }
};

// use this function to calculate a
// string hash at compile time
template<eU32 N> eU32 eHashStr(const eChar (&str)[N])
{
    return eFnvHash<N, N>::hash(str);
}

void    eStrClear(eChar *str);
void    eStrCopy(eChar *dst, const eChar *src);
eInt		eStrPtrNCopy( eChar * _buffer, int _bufferSize, const eChar * _start, const eChar * _end );
void    eStrNCopy(eChar *dst, const eChar *src, eU32 count);
eChar * eStrClone(const eChar *str);
eU32    eStrLength(const eChar *str);
eChar * eStrAppend(eChar *dst, const eChar *src);
eInt    eStrCompare(const eChar *str0, const eChar *str1);
eInt    eStrICompare(const eChar *str0, const eChar *str1);
eChar * eStrUpper(eChar *str);
eChar * eIntToStr(eInt val);
eInt    eStrToInt(const eChar *_str, const char ** _end = nullptr );
eU64		eStrToHex( const eChar * _str, const char ** _end = nullptr );
eF64    eStrToFloat(const eChar *str, const char ** _end = nullptr);
eBool   eIsAlphaNumeric(eChar c);
eBool   eIsDigit(eChar c);
eBool   eIsAlpha(eChar c);

#ifdef PROUT_IMGUI
eChar	eToUpper(eChar _c);
int eStrNCompare(const eChar * _s1, const eChar * _s2, register size_t _n);
eChar * eStrStr(const eChar * s1, const eChar * s2);
eBool	eIsSpace(eChar c);

#endif // PROUT_IMGUI

// dynamic string class
class eStr
{
public:
    eStr();
    eStr(eChar chr);
    eStr(const eChar *str);
    eStr(const eChar *str, eU32 length);
    eStr(const eStr &str);

		eFORCEINLINE void	PreAllocate( eU32 _size ) { m_data.PreAllocate( _size ); }
    eU32            Length() const;
    eBool           Equals(const eStr &str, eU32 count) const;

    void            PadLeft(eU32 totalLen, eChar chr);
    void            MakeUpper();
    eBool           Split(eChar token, eStr &left, eStr &right) const;
    eStr         SubStr(eU32 startIndex, eU32 endIndex) const;
    eStr         Simplified() const;
    void            Remove(eU32 startIndex, eU32 endIndex);
    void            RemoveAt(eU32 index);

    eStr         operator + (const eStr &str) const;
    eStr &       operator += (eChar c);
    eStr &       operator += (const eStr &str);
    eStr &       operator = (const eChar *str);

    const eChar &   At(eU32 index) const;
    eChar &         At(eU32 index);
    const eChar &   operator [] (eInt index) const;
    eChar &         operator [] (eInt index);

    eBool           operator == (const eStr &str) const;
    eBool           operator == (const eChar *str) const;
    eBool           operator != (const eStr &str) const;
    eBool           operator != (const eChar *str) const;

		const eChar *	c_str() const;

		inline static eInt Cmp( const char * _s1, const char * _s2 ) { return eStrCompare( _s1, _s2 ); }
		inline static eInt Icmp( const char * _s1, const char * _s2 ) { return eStrICompare( _s1, _s2 ); }
		inline eInt Icmp( const char * _other ) const { return eStrICompare( m_data.Ptr(), _other ); }

private:
    eList<eChar>   m_data;
};
