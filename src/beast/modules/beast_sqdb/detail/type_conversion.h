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

    Portions based on SOCI - The C++ Database Access Library: 

    SOCI: http://soci.sourceforge.net/

    This file incorporates work covered by the following copyright
    and permission notice:

    Copyright (C) 2004 Maciej Sobczak, Stephen Hutton, Mateusz Loskot,
    Pawel Aleksander Fedorynski, David Courtney, Rafal Bobrowski,
    Julian Taylor, Henning Basold, Ilia Barahovski, Denis Arnaud,
    Daniel Lidstr�m, Matthieu Kermagoret, Artyom Beilis, Cory Bennett,
    Chris Weed, Michael Davidsaver, Jakub Stachowski, Alex Ott, Rainer Bauer,
    Martin Muenstermann, Philip Pemberton, Eli Green, Frederic Chateau,
    Artyom Tonkikh, Roger Orr, Robert Massaioli, Sergey Nikulov,
    Shridhar Daithankar, S�ren Meyer-Eppler, Mario Valesco.

    Boost Software License - Version 1.0, August 17th, 2003

    Permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "Software") to use, reproduce, display, distribute,
    execute, and transmit the Software, and to prepare derivative works of the
    Software, and to permit third-parties to whom the Software is furnished to
    do so, all subject to the following:

    The copyright notices in the Software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the Software, in whole or in part, and
    all derivative works of the Software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_SQDB_DETAIL_TYPE_CONVERSION_H_INCLUDED
#define BEAST_SQDB_DETAIL_TYPE_CONVERSION_H_INCLUDED

namespace sqdb
{

namespace detail
{

// used to order into and uses that need conversions
template<typename T>
struct base_value_holder
{
    typename type_conversion<T>::base_type m_value;
};

// create into_type from type_conversion
template <typename T>
class conversion_into_type
    : private base_value_holder<T>
    , public into_type <typename type_conversion<T>::base_type>
    , public Uncopyable
{
public:
    typedef typename type_conversion<T>::base_type BASE_TYPE;

    conversion_into_type(T& value)
        : into_type<BASE_TYPE>(detail::base_value_holder<T>::m_value, m_own_ind)
        , m_value(value), m_ind(m_own_ind) {}

    conversion_into_type(T& value, indicator& ind)
        : into_type<BASE_TYPE>(detail::base_value_holder<T>::m_value, ind)
        , m_value(value), m_ind(ind) {}

private:
    void convert_from_base()
    {
        type_conversion<T>::from_base(
            detail::base_value_holder<T>::m_value, m_ind, m_value);
    }

    T& m_value;

    indicator m_own_ind;
    indicator& m_ind;
};

// used to create a use_type from a type_conversion
template<typename T>
class conversion_use_type
    : private base_value_holder<T>
    , public use_type<typename type_conversion<T>::base_type>
    , public Uncopyable
{
public:
    typedef typename type_conversion<T>::base_type BASE_TYPE;

    conversion_use_type(T& value)
        : use_type<BASE_TYPE>(detail::base_value_holder<T>::m_value, m_own_ind)
        , m_value(value), m_ind(m_own_ind), m_bReadOnly(false) {}

    conversion_use_type(T const& value)
        : use_type<BASE_TYPE>(detail::base_value_holder<T>::m_value, m_own_ind)
        , m_value(const_cast<T&>(value)), m_ind(m_own_ind), m_bReadOnly(true) {}

    conversion_use_type(T& value, indicator& ind)
        : use_type<BASE_TYPE>(detail::base_value_holder<T>::m_value, ind)
        , m_value(value), m_ind(ind), m_bReadOnly(false) {}

    conversion_use_type(T const& value, indicator& ind)
        : use_type<BASE_TYPE>(detail::base_value_holder<T>::m_value, ind)
        , m_value(const_cast<T&>(value)), m_ind(ind), m_bReadOnly(true) {}

    void convert_from_base()
    {
        if (!m_bReadOnly)
        {
            type_conversion<T>::from_base(
                detail::base_value_holder<T>::m_value, m_ind, m_value);
        }
    }

    void convert_to_base()
    {
        type_conversion<T>::to_base(m_value,
                                    detail::base_value_holder<T>::m_value, m_ind);
    }

private:
    T& m_value;
    indicator m_own_ind;
    indicator& m_ind;
    bool m_bReadOnly;
};

template <typename T>
into_type_ptr do_into(T& t, user_type_tag)
{
    return into_type_ptr(new conversion_into_type<T>(t));
}

template <typename T>
into_type_ptr do_into(T& t, indicator& ind, user_type_tag)
{
    return into_type_ptr(new conversion_into_type<T>(t, ind));
}

template <typename T>
use_type_ptr do_use(T& t, user_type_tag)
{
    return use_type_ptr(new conversion_use_type<T>(t));
}

template <typename T>
use_type_ptr do_use(T const& t, user_type_tag)
{
    return use_type_ptr(new conversion_use_type<T>(t));
}

template <typename T>
use_type_ptr do_use(T& t, indicator& ind, user_type_tag)
{
    return use_type_ptr(new conversion_use_type<T>(t, ind));
}

template <typename T>
use_type_ptr do_use(T const& t, indicator& ind, user_type_tag)
{
    return use_type_ptr(new conversion_use_type<T>(t, ind));
}

}

}

#endif
