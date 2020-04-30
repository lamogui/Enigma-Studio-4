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

#include "extern/Enigma/eshared/system/string.hpp"
#include "extern/Enigma/eshared/system/runtime.hpp"

#include "system/sys_assert.hpp"

void eStrClear(eChar *str)
{
	str[0] = '\0';
}

void eStrCopy(eChar *dst, const eChar *src)
{
	while (*dst++ = *src++);
}

eInt eStrPtrNCopy( eChar * _buffer, int _bufferSize, const eChar * _start, const eChar * _end) {
	passert(_end > _start, "Invalid start & end pointers");
	passert( _bufferSize > _end-_start+1, "Not enought buffer size !"  );
	int i = 0;
	while (_start != _end && i+1<_bufferSize) {
		_buffer[ i++ ] = *_start;
		++_start;
	}
	_buffer[ i ] = '\0';
	return i;
}

void eStrNCopy(eChar *dst, const eChar *src, eU32 count)
{
	// copy string characters
	while (count && (*dst++ = *src++))
		count--;

	// pad out with zeros
	if (count)
		eMemSet(dst, '\0', count - 1);
}

eChar * eStrClone(const eChar *str)
{
	eChar *clone = new eChar[eStrLength(str) + 1];
	eStrCopy(clone, str);
	return clone;
}

eU32 eStrLength(const eChar *str)
{
	const eChar *eos = str;
	while (*eos++);
	return (eU32)(eos - str - 1);
}

eChar * eStrAppend(eChar *dst, const eChar *src)
{
	// find end of source string
	eChar *pd = dst;

	while (*pd)
		pd++;

	eStrCopy(pd, src);
	return dst;
}

// compares two strings and returns an integer to
// indicate whether the first is less than the second,
// the two are equal, or whether the first is greater
// than the second. comparison is done byte by byte on
// an unsigned basis which is to say that null (0) is
// less than any other character (1-255).
//
// returns -1 if first string < second string.
// returns  0 if first string = second string.
// returns +1 if first string > second string.
eInt eStrCompare(const eChar *str0, const eChar *str1)
{
	eInt res = 0;

	while (!(res = *(eU8 *)str0 - *(eU8 *)str1) && *str1)
	{
		str0++;
		str1++;
	}

	if (res < 0)
		res = -1;
	else if (res > 0)
		res = 1;

	return res;
}

eInt eStrICompare(const eChar * _str0, const eChar * _str1) {
	eInt res = 0;

	while ( 1 )
	{
		if ( eIsAlpha( *_str0 ) && eIsAlpha( *_str1 ) ) {
			res = (*((eU8 *)_str0) & 0xDF ) - (*((eU8 *)_str1) & 0xDF);
		} else {
			res = *(eU8 *)_str0 - *(eU8 *)_str1;
		}
		if ( res || !*_str1 ) {
			break;
		}
		_str0++;
		_str1++;
	}

	if (res < 0)
		res = -1;
	else if (res > 0)
		res = 1;

	return res;
}

eChar * eStrUpper(eChar *str)
{
	const eU32 strLen = eStrLength(str);
	for (eU32 i = 0; i < strLen; i++)
	{
		eChar &c = str[i];
		if (c >= 'a' && c <= 'z')
			c -= 32;
	}

	return str;
}

eChar * eIntToStr(eInt val)
{
	// remember if integer is negative and
	// if it is, make it positive
	const eBool negative = (val < 0);

	if (negative)
		val = -val;

	// 12 spaces are enough for 32-bit decimal
	// (10 digits + 1 null terminator byte +
	// eventually a sign character)
	static eChar str[12];

	eChar *cp = str + sizeof(str) - 1;
	*cp = '\0';

	do
	{
		*(--cp) = val % 10 + '0';
		val /= 10;
	} while (val > 0);

	// prepend negative sign character
	if (negative)
		*(--cp) = '-';

	return cp;
}

eInt eStrToInt( const eChar * _str, const char ** _end )
{
	eASSERT(eStrLength(_str) > 0);

	const eBool neg = (_str[0] == '-');
	if (neg) {
		_str++;
	}
	eChar c;
	eInt val = 0;

	while ((c = *_str++) != '\0' && eIsDigit(c))
		val = val * 10 + (c - '0');

	if (_end != nullptr) {
		*_end = _str;
	}

	return (neg ? -val : val);
}

eU64 eStrToHex(const eChar * _str, const char ** _end ) {
	eASSERT(eStrLength(_str) > 0);

	eChar c;
	eU64 val = 0;

	while ((c = *_str++) != '\0' && eIsDigit(c) || ((c & 0xDF) >= 'A' && (c & 0xDF) <= 'F')) {
		if (eIsDigit(c)) {
			val = val * 16 + (c - '0');
		}
		else {
			val = val * 16 + ((c & 0xDF) - 'A');
		}
	}
	if (_end != nullptr) {
		*_end = _str;
	}

	return val;

}

eF64 eStrToFloat(const eChar *str, const char ** _end)
{
	eASSERT(eStrLength(str) > 0);

	eF64 val = 0.0f;
	eInt sign = 1;

	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}

	while (eIsDigit(*str))
		val = val * 10 + (*str++) - '0';

	if (*str == '.')
	{
		str++;
		eF64 dec = 1.0f;

		while (eIsDigit(*str))
		{
			dec = dec / 10.0f;
			val = val + ((*str++) - '0')*dec;
		}
	}

	if (_end != nullptr) {
		*_end = str;
	}

	return val * sign;
}

eBool eIsAlphaNumeric(eChar c)
{
	return eIsAlpha(c) || eIsDigit(c);
}

eBool eIsAlpha(eChar c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' );
}

eBool eIsDigit(eChar c)
{
	return (c >= '0' && c <= '9');
}

#ifdef PROUT_IMGUI
eChar eToUpper(eChar _c)
{
	if (_c >= 'a' && _c <= 'z') {
		_c -= 32;
	}
	return _c;
}

static eChar * eStrChr(const eChar * _s, eInt _c)
{
	while (_s[0] != _c && _s[0] != '\0') {
		_s++;
	}
	if (_s[0] == '\0') {
		return nullptr;
	}
	else {
		return (eChar *)_s;
	}
}

int eStrNCompare(const eChar * _s1, const eChar * _s2, register size_t _n)
{
	register unsigned char u1, u2;

	while (_n-- > 0)
	{
		u1 = (unsigned char)*_s1++;
		u2 = (unsigned char)*_s2++;
		if (u1 != u2)
			return u1 - u2;
		if (u1 == '\0')
			return 0;
	}
	return 0;
}

eChar * eStrStr(const eChar * _s1, const eChar * _s2)
{
	const char *p = _s1;
	const size_t len = eStrLength(_s2);

	for (; (p = eStrChr(p, *_s2)) != 0; p++)
	{
		if (eStrNCompare(p, _s2, len) == 0) {
			return (char *)p;
		}
	}
	return (0);
}

eBool eIsSpace(eChar _c) {
	switch (_c) {
	case '\t':
	case '\n':
	case '\v':
	case '\f':
	case '\r':
	case ' ':
		return true;
	default:
		return false;
	}

}

#endif // PROUT_IMGUI

eStr::eStr()
{
    m_data.Append('\0');
}

eStr::eStr(eChar chr)
{
    m_data.Append(chr);
    m_data.Append('\0');
}

eStr::eStr(const eChar *str)
{
    *this = str;
}

eStr::eStr(const eChar *str, eU32 length)
{
    passert(length > 0, "Invalid String initialisation");
		passert(eStrLength(str) >= length, "Invalid String initialisation");

    m_data.Resize(length+1);
    eMemCopy(&m_data[0], str, length);
}

eStr::eStr(const eStr &str)
{
    *this = str;
}

eU32 eStr::Length() const
{
    return m_data.Num()-1;
}

eBool eStr::Equals(const eStr &str, eU32 count) const
{
		passert(count <= str.Length(), "You should check size before...");

    for (eU32 i=0; i<eMin(count, Length()); i++)
        if (str[i] != m_data[i])
            return eFALSE;

    return eTRUE;
}

void eStr::PadLeft(eU32 totalLen, eChar chr)
{
    while (Length() < totalLen) {
        m_data.Insert(0, chr);
		}
}

void eStr::MakeUpper()
{
    eStrUpper(&m_data[0]);
}

// Returns wether or not the given token
// was found inside the string or not.
eBool eStr::Split(eChar token, eStr &left, eStr &right) const
{
    left = "";
    right = "";

    eBool found = eFALSE;
    eStr *dst = &left;

    for (eU32 i=0; i< Length(); i++)
    {
        const eChar c = At(i);

        if (c == token)
        {
            dst = &right;
            found = eTRUE;
        }
        else
            *dst += c;
    }

    return found;
}

eStr eStr::SubStr(eU32 startIndex, eU32 endIndex) const
{
    passert((eInt)startIndex < (eInt)m_data.Num()-1,"Invalid startIndex");
		passert((eInt)endIndex < (eInt)m_data.Num()-1, "Invalid endIndex");
		passert(startIndex <= endIndex, "Invalid indexes");

    const eChar *str = c_str();

    return eStr(str+startIndex, endIndex-startIndex);
}

// Returns a string with no spaces in the beginning
// and end and all white space sequences in the
// inside of the string are replaced by just one
// single space.
eStr eStr::Simplified() const
{
    // Remove spaces in the beginning.
    eStr ts = *this;

    while (ts.Length() > 0 && ts[0] == ' ')
        ts.RemoveAt(0);

    // Remove spaces in the end.
    while (ts.Length() > 0 && ts[ts.Length()-1] == ' ')
        ts.RemoveAt(ts.Length()-1);

    // Remove inner spaces.
    for (eInt i=ts.Length()-1; i>0; i--)
        if (ts[i] == ' ' && ts[i-1] == ' ')
            ts.RemoveAt(i);

    return ts;
}

// Removes a sub-string start and ending
// at the given indices.
void eStr::Remove(eU32 startIndex, eU32 endIndex)
{
		passert((eInt)startIndex < (eInt)m_data.Num() - 1, "Invalid startIndex");
		passert((eInt)endIndex < (eInt)m_data.Num() - 1, "Invalid endIndex");
		passert(startIndex <= endIndex, "Invalid indexes");

    for (eU32 i=endIndex; i>=startIndex; i--) {
        m_data.RemoveAt(i);
		}
}

void eStr::RemoveAt(eU32 index)
{
		passert((eInt)index < (eInt)m_data.Num()-1, "Invalid index");
    Remove(index, index);
}

eStr eStr::operator + (const eStr &str) const
{
    eStr res = *this;

    for (eU32 i=0; i<str.Length(); i++)
        res += str[i];

    return res;
}

eStr & eStr::operator += (eChar c)
{
    m_data.Insert(m_data.Num()-1, c);
    return *this;
}

eStr & eStr::operator += (const eStr &str)
{
    for (eU32 i=0; i<str.Length(); i++)
        (*this) += str[i];

    return *this;
}

eStr & eStr::operator = (const eChar *str)
{
    if (str)
    {
        m_data.Resize(eStrLength(str)+1);
        eMemCopy(&m_data[0], str, m_data.Num());
    }
    else
    {
        m_data.Resize(1);
        m_data[0] = '\0';
    }

    return *this;
}

const eChar & eStr::At(eU32 index) const
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

eChar & eStr::At(eU32 index)
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

const eChar & eStr::operator [] (eInt index) const
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

eChar & eStr::operator [] (eInt index)
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

eBool eStr::operator == (const eStr &str) const
{
    return (eStrCompare(c_str(), str.c_str()) == 0);
}

eBool eStr::operator == (const eChar *str) const
{
    return (*this == eStr(str));
}

eBool eStr::operator != (const eStr &str) const
{
    return !(*this == str);
}

eBool eStr::operator != (const eChar *str) const
{
    return !(*this == str);
}

const eChar * eStr::c_str() const
{
	return &m_data[ 0 ];
}