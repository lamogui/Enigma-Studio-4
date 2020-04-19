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
#include "extern/Enigma/eshared/system/types.hpp"
#include "system/sys_assert.hpp"

template<class T> class eList;
typedef eList<ePtr> ePtrList;
typedef eList<eU8> eByteArray;

// non-templated functions used to avoid code
// bloat, caused by heavy template instantiation
void ePtrListInit(ePtrList *a, eU32 typeSize, eU32 size);
void ePtrListCopy(ePtrList *a, const ePtrList *ta);
void ePtrListClear(ePtrList *a);
void ePtrListFree(ePtrList *a);
void ePtrListReserve(ePtrList *a, eU32 capacity);
void ePtrListResize(ePtrList *a, eU32 size);
ePtr ePtrListAppend(ePtrList *a);
void ePtrListInsert(ePtrList *a, eU32 index, const ePtr data);
void ePtrListRemoveAt(ePtrList *a, eU32 index);
void ePtrListRemoveSwap(ePtrList *a, eU32 index);
eInt ePtrListFind(const ePtrList *a, const ePtr data);
eBool ePtrListEqual(const ePtrList *a0, const ePtrList *a1);

// performs insertion sort. not really fast,
// but already a lot faster than bubble sort
// and quite small, easy to implement and easy
// to get correct.
template<class T> void eSort(T *data, eU32 count, eBool (*predicate)(const T &a, const T &b))
{
    for (eU32 j=1; j<count; j++)
    {
        const T key = data[j]; // don't use a reference here!
        eInt i = (eInt)j-1;

        while (i >= 0 && predicate(data[i], key))
        {
            data[i+1] = data[i];
            i--;
        }

        data[i+1] = key;
    }
}

// templated dynamic array. this class is intro-safe,
// because all array functions which are duplicated
// during template instantiation are inlined, using
// non-templated functions. works for all kind of
// data types, not only pointers.
// one drawback is that object's (T) constructors
// aren't called when copying or instantiating array,
// same goes for destructors.
// all memory is for SIMD usage 16-byte aligned.
template<class T> class eList
{
public:
    eFORCEINLINE eList(eU32 size=0)
    {
        ePtrListInit((ePtrList *)this, sizeof(T), size);
    }

    eFORCEINLINE eList(const eList &a)
    {
        ePtrListInit((ePtrList *)this, sizeof(T), 0);
        ePtrListCopy((ePtrList *)this, (const ePtrList *)&a);
    }

	eFORCEINLINE eList(const eU8 *data, eU32 len)
	{
		ePtrListInit((ePtrList *)this, sizeof(eU8), len);
		eMemCopy(m_data, data, len);
	}

    eFORCEINLINE ~eList()
    {
        free();
    }

    eFORCEINLINE eBool IsEmpty() const
    {
        return (m_num == 0);
    }

    eFORCEINLINE void Resize(eU32 size)
    {
        ePtrListResize((ePtrList *)this, size);
    }

    eFORCEINLINE void Reserve(eU32 capacity)
    {
        ePtrListReserve((ePtrList *)this, capacity);
    }

    eFORCEINLINE void Clear()
    {
        ePtrListClear((ePtrList *)this);
    }

    eFORCEINLINE void free()
    {
        ePtrListFree((ePtrList *)this);
    }

    eFORCEINLINE T & Append(const T &data)
    {
        *(T *)ePtrListAppend((ePtrList *)this) = data;
        return Last();
    }

    // only appends item if no element with
    // the same value exists in array
    eFORCEINLINE T & AppendUnique(const T &data)
    {
        const eInt index = Find(data);
        return (index != -1 ? m_data[index] : Append(data));
    }

    eFORCEINLINE T & Alloc()
    {
        Append(T());
        return Last();
    }

    eFORCEINLINE void Append(const eList &a)
    {
        for (eU32 i=0; i<a.Num(); i++) {
            Append(a[i]);
				}
    }

    eFORCEINLINE T & Push(const T &data)
    {
        return Append(data);
    }

    eFORCEINLINE T & Push()
    {
        return Append();
    }

    eFORCEINLINE T Pop()
    {
        T data = Last();
        RemoveLast();
        return data;
    }

    eFORCEINLINE void Insert(eU32 index, const T &data)
    {
        ePtrListInsert((ePtrList *)this, index, (ePtr *)&data);
    }

    eFORCEINLINE void RemoveAt(eU32 index)
    {
        ePtrListRemoveAt((ePtrList *)this, index);
    }

    eFORCEINLINE void RemoveSwap(eU32 index)
    {
        ePtrListRemoveSwap((ePtrList *)this, index);
    }

    eFORCEINLINE void RemoveLast()
    {
        m_num--;
    }

    eFORCEINLINE eInt Find(const T &data) const
    {
        return ePtrListFind((ePtrList *)this, (ePtr *)&data);
    }

    eFORCEINLINE eBool Contains(const T &data) const
    {
        return (Find(data) != -1);
    }

    eFORCEINLINE eU32 Num() const
    {
        return m_num;
    }

    eFORCEINLINE eU32 Capacity() const
    {
        return m_capacity;
    }

    eFORCEINLINE T & Last()
    {
        passert( m_num > 0, "Can't call Last() on empty list" );
        return m_data[ m_num-1 ];
    }

    eFORCEINLINE const T & Last() const
    {
			passert(m_num > 0, "Can't call Last() on empty list");
        return m_data[m_num-1];
    }

    eFORCEINLINE T & First()
    {
				passert(m_num > 0, "Can't call Fist() on empty list");
        return m_data[0];
    }

    eFORCEINLINE const T & First() const
    {
				passert(m_num > 0, "Can't call Fist() on empty list");
        return m_data[0];
    }

    eFORCEINLINE eList & operator = (const eList &a)
    {
        if (this != &a)
            ePtrListCopy((ePtrList *)this, (const ePtrList *)&a);

        return *this;
    }

    eFORCEINLINE T & operator [] (eInt index)
    {
				passert(index >= 0 && (eU32)index < m_num, "Invalid index" );
        return m_data[index];
    }

    eFORCEINLINE const T & operator [] (eInt index) const
    {
				passert(index >= 0 && (eU32)index < m_num, "Invalid index");
        return m_data[index];
    }

    eFORCEINLINE eBool operator == (const eList &a) const
    {
        return ePtrListEqual((const ePtrList *)this, (const ePtrList *)&a);
    }

    eFORCEINLINE eBool operator != (const eList &a) const
    {
        return !(*this == a);
    }

public:
    T *     m_data;
    eU32    m_num;
    eU32    m_capacity;
    eU32    m_typeSize;
};
