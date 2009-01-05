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

#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <glib-2.0/glib.h>

#include "http_machine.h"

typedef struct {

  char* method;
  char* uri;
  char* protocol_version;

  /* private */

  gchar* _raw_request;

} HttpRequest;

HttpRequest*
http_request_new(GIOChannel* io_channel, GError** error);

gboolean
http_request_destroy(HttpRequest* this, GError** error);


#endif /* HTTP_REQUEST_H_ */
