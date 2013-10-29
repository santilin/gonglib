#ifndef _GONG_HTTP_CLIENT_H
#define _GONG_HTTP_CLIENT_H

//
// sync_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/** @file gonghttpclient.h Reads an http file over the network
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongxtring.h>
using gong::Xtring;
using gong::XtringList;

namespace gong {

int read_http_file(XtringList &header, char **reply,
                   const Xtring &server, const Xtring &filename,
                   const Xtring &extraHeader = Xtring::null,
                   const Xtring &method = "GET", const Xtring &query = Xtring::null);
Xtring uriEncode(const Xtring &sSrc);

}

#endif
