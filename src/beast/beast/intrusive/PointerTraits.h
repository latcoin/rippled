//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_INTRUSIVE_POINTERTRAITS_H_INCLUDED
#define BEAST_INTRUSIVE_POINTERTRAITS_H_INCLUDED

#include <cstddef>

namespace beast {
namespace intrusive {

// an unspecialized PointerTraits is ill-defined
template <typename P>
struct PointerTraits;

// specializations to remove cv-qualifiers
template <typename P>
struct PointerTraits <P const> : PointerTraits <P> { };
template <typename P>
struct PointerTraits <P volatile> : PointerTraits <P> { };
template <typename P>
struct PointerTraits <P const volatile> : PointerTraits <P> { };
// specialization to remove a reference attribute
template <typename P>
struct PointerTraits <P&> : PointerTraits <P> { };

// specialization for raw pointers
template <typename T>
struct PointerTraits <T*>
{
    typedef T               element_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef std::ptrdiff_t  difference_type;

    template <class U>
    struct rebind_pointer
    {
        typedef U* type;
    };

    static pointer pointer_to (reference r)
    {
        return static_cast <pointer> (
            static_cast <void*> (
                const_cast <char*> (
                    &reinterpret_cast <const char&> (
                        r))));
    }

    template <class U>
    static pointer static_cast_from (U* u)
    {
        return static_cast <pointer> (u);
    }

    template <class U>
    static pointer const_cast_from (U* u)
    {
        return const_cast <pointer> (u);
    }

    template <class U>
    static pointer dynamic_cast_from (U* u)
    {
        return dynamic_cast <pointer> (u);
    }
};

}
}

#endif