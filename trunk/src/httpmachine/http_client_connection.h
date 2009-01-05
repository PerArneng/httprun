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

#ifndef _HTTPCLIENTCONNECTION_H
#define	_HTTPCLIENTCONNECTION_H

#include <sys/socket.h>
#include <glib-2.0/glib.h>
#include "http_machine.h"

#define IP_ADDRESS_LENGTH 17

typedef struct
{
  int socket;
  char* ip_address;
  char* hostname;
  GIOChannel* io_channel;
} HttpClientConnection;

HttpClientConnection*
http_client_connection_new(int socket, struct sockaddr_in* addr,
                           GError** error);

gboolean
http_client_connection_destroy(HttpClientConnection* this, GError** error);

#endif	/* _HTTPCLIENTCONNECTION_H */
