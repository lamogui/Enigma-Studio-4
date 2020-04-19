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

eString::eString()
{
    m_data.Append('\0');
}

eString::eString(eChar chr)
{
    m_data.Append(chr);
    m_data.Append('\0');
}

eString::eString(const eChar *str)
{
    *this = str;
}

eString::eString(const eChar *str, eU32 length)
{
    passert(length > 0, "Invalid String initialisation");
		passert(eStrLength(str) >= length, "Invalid String initialisation");

    m_data.Resize(length+1);
    eMemCopy(&m_data[0], str, length);
}

eString::eString(const eString &str)
{
    *this = str;
}

eU32 eString::Length() const
{
    return m_data.Num()-1;
}

eBool eString::Equals(const eString &str, eU32 count) const
{
		passert(count <= str.Length(), "You should check size before...");

    for (eU32 i=0; i<eMin(count, Length()); i++)
        if (str[i] != m_data[i])
            return eFALSE;

    return eTRUE;
}

void eString::PadLeft(eU32 totalLen, eChar chr)
{
    while (Length() < totalLen) {
        m_data.Insert(0, chr);
		}
}

void eString::MakeUpper()
{
    eStrUpper(&m_data[0]);
}

// Returns wether or not the given token
// was found inside the string or not.
eBool eString::Split(eChar token, eString &left, eString &right) const
{
    left = "";
    right = "";

    eBool found = eFALSE;
    eString *dst = &left;

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

eString eString::SubStr(eU32 startIndex, eU32 endIndex) const
{
    passert((eInt)startIndex < (eInt)m_data.Num()-1,"Invalid startIndex");
		passert((eInt)endIndex < (eInt)m_data.Num()-1, "Invalid endIndex");
		passert(startIndex <= endIndex, "Invalid indexes");

    const eChar *str = c_str();

    return eString(str+startIndex, endIndex-startIndex);
}

// Returns a string with no spaces in the beginning
// and end and all white space sequences in the
// inside of the string are replaced by just one
// single space.
eString eString::Simplified() const
{
    // Remove spaces in the beginning.
    eString ts = *this;

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
void eString::Remove(eU32 startIndex, eU32 endIndex)
{
		passert((eInt)startIndex < (eInt)m_data.Num() - 1, "Invalid startIndex");
		passert((eInt)endIndex < (eInt)m_data.Num() - 1, "Invalid endIndex");
		passert(startIndex <= endIndex, "Invalid indexes");

    for (eU32 i=endIndex; i>=startIndex; i--) {
        m_data.RemoveAt(i);
		}
}

void eString::RemoveAt(eU32 index)
{
		passert((eInt)index < (eInt)m_data.Num()-1, "Invalid index");
    Remove(index, index);
}

eString eString::operator + (const eString &str) const
{
    eString res = *this;

    for (eU32 i=0; i<str.Length(); i++)
        res += str[i];

    return res;
}

eString & eString::operator += (eChar c)
{
    m_data.Insert(m_data.Num()-1, c);
    return *this;
}

eString & eString::operator += (const eString &str)
{
    for (eU32 i=0; i<str.Length(); i++)
        (*this) += str[i];

    return *this;
}

eString & eString::operator = (const eChar *str)
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

const eChar & eString::At(eU32 index) const
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

eChar & eString::At(eU32 index)
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

const eChar & eString::operator [] (eInt index) const
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

eChar & eString::operator [] (eInt index)
{
		passert((eInt)index < (eInt)m_data.Num() - 1, "Invalid index");
    return m_data[index];
}

eBool eString::operator == (const eString &str) const
{
    return (eStrCompare(c_str(), str.c_str()) == 0);
}

eBool eString::operator == (const eChar *str) const
{
    return (*this == eString(str));
}

eBool eString::operator != (const eString &str) const
{
    return !(*this == str);
}

eBool eString::operator != (const eChar *str) const
{
    return !(*this == str);
}

const eChar * eString::c_str() const
{
	return &m_data[ 0 ];
}