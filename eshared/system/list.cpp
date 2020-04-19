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

#include "extern/Enigma/eshared/system/list.hpp"
#include "extern/Enigma/eshared/system/runtime.hpp"

#include "system/sys_assert.hpp"

void ePtrListInit(ePtrList *a, eU32 typeSize, eU32 size)
{
    passert(typeSize > 0, "Invalid typeSize");

    a->m_data = nullptr;
    a->m_num = 0;
    a->m_capacity = 0;
    a->m_typeSize = typeSize;

    if (size > 0)
        ePtrListResize(a, size);
}

void ePtrListCopy(ePtrList *a, const ePtrList *ta)
{
    a->m_typeSize = ta->m_typeSize;
    ePtrListReserve(a, ta->m_capacity);
    a->m_num = ta->m_num;

    if (ta->m_num)
        eMemCopy(a->m_data, ta->m_data, ta->m_num*ta->m_typeSize);
}

void ePtrListClear(ePtrList *a)
{
    a->m_num = 0;
}

void ePtrListFree(ePtrList *a)
{
    eFreeAligned(a->m_data);
    a->m_data = nullptr;
    a->m_num = 0;
    a->m_capacity = 0;
}

void ePtrListReserve(ePtrList *a, eU32 capacity)
{
    if (!capacity)
        ePtrListClear(a);
    else if (a->m_capacity < capacity)
    {
        eU8 *temp = (eU8 *)eAllocAlignedAndZero(capacity*a->m_typeSize, 16);
        passert(temp != nullptr, "Failed to allocate memory for eList");
        eU32 newSize = 0;

        if (a->m_data)
        {
            newSize = eMin(a->m_num, capacity);
            eMemCopy(temp, a->m_data, newSize*a->m_typeSize);
            eFreeAligned(a->m_data);
            a->m_data = nullptr;
        }

        a->m_data = (ePtr *)temp;
        a->m_num = newSize;
        a->m_capacity = capacity;
    }
}

void ePtrListResize(ePtrList *a, eU32 size)
{
    if (size > a->m_capacity)
        ePtrListReserve(a, size);

    a->m_num = size;
}

ePtr ePtrListAppend(ePtrList *a)
{
    if (a->m_num >= a->m_capacity)
    {
        const eU32 newCapacity = (a->m_capacity > 0 ? a->m_capacity*2 : 32);
        ePtrListReserve(a, newCapacity);
    }

    ePtr res = ((eU8 *)a->m_data)+a->m_num*a->m_typeSize;  
    a->m_num++;
    return res;
}

void ePtrListInsert(ePtrList *a, eU32 index, const ePtr data)
{
    passert(index <= a->m_num, "Invalid index");

    if (a->m_num >= a->m_capacity)
    {
        const eU32 newCapacity = (a->m_capacity > 0 ? a->m_capacity*2 : 32);
        ePtrListReserve(a, newCapacity);
    }

    eMemMove(((eU8 *)a->m_data)+(index+1)*a->m_typeSize,
             ((eU8 *)a->m_data)+index*a->m_typeSize,
             (a->m_num-index)*a->m_typeSize);

    eMemCopy(((eU8 *)a->m_data)+index*a->m_typeSize, data, a->m_typeSize);
    a->m_num++;
}

void ePtrListRemoveAt(ePtrList *a, eU32 index)
{
    passert(index < a->m_num, "Invalid index");

    eMemMove(((eU8 *)a->m_data)+index*a->m_typeSize,
             ((eU8 *)a->m_data)+(index+1)*a->m_typeSize,
             (a->m_num-index-1)*a->m_typeSize);

    a->m_num--;
}

void ePtrListRemoveSwap(ePtrList *a, eU32 index)
{
    passert(index < a->m_num, "Invalid index" );

    if (index < a->m_num-1)
    {
        eMemCopy(((eU8 *)a->m_data)+index*a->m_typeSize,
                 ((eU8 *)a->m_data)+(a->m_num-1)*a->m_typeSize,
                 a->m_typeSize);
    }
   
    a->m_num--;
}

eInt ePtrListFind(const ePtrList *a, const ePtr data)
{
    for (eU32 i=0, index=0; i<a->Num(); i++, index+=a->m_typeSize)
        if (eMemEqual(((eU8 *)a->m_data)+index, data, a->m_typeSize))
            return i;

    return -1;
}

eBool ePtrListEqual(const ePtrList *a0, const ePtrList *a1)
{
    if (a0->Num() != a1->Num())
        return eFALSE;
    else if (a0->m_num == 0) // => both empty because of first condition
        return eTRUE;
    else
        return eMemEqual(a0->m_data, a1->m_data, a0->Num()*a0->m_typeSize);
}