/* --
 * Copyright (C) 2009 Per Arneng
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Boston, MA  02110-1301, USA.
 * --
 */

#include "http_request.h"

#ifndef HTTP_REQUEST_PRIVATE_H_
#define HTTP_REQUEST_PRIVATE_H_

gchar*
_http_request_read_request(GIOChannel* io_channel, GError** error);

gboolean
_http_request_parse_raw_request(HttpRequest* this, GError** error);

RequestMethod
_http_request_get_request_method(gchar* method);

#endif /* HTTP_REQUEST_PRIVATE_H_ */
