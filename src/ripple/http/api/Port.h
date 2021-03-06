//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

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

#ifndef RIPPLE_HTTP_PORT_H_INCLUDED
#define RIPPLE_HTTP_PORT_H_INCLUDED

namespace ripple {
using namespace beast;

namespace HTTP {

/** Configuration information for a server listening port. */
struct Port
{
    enum Security
    {
        no_ssl,
        allow_ssl,
        require_ssl
    };

    Port ();
    Port (Port const& other);
    Port& operator= (Port const& other);
    Port (uint16 port_, IPAddress const& addr_,
            Security security_, SSLContext* context_);

    uint16 port;
    IPAddress addr;
    Security security;
    SSLContext* context;
};

bool operator== (Port const& lhs, Port const& rhs);
bool operator!= (Port const& lhs, Port const& rhs);
bool operator<  (Port const& lhs, Port const& rhs);
bool operator<= (Port const& lhs, Port const& rhs);
bool operator>  (Port const& lhs, Port const& rhs);
bool operator>= (Port const& lhs, Port const& rhs);

/** A set of listening ports settings. */
typedef std::vector <Port> Ports;

}
}

#endif
