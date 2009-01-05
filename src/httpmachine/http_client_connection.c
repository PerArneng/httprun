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

#include <stdlib.h>
#include <arpa/inet.h>

#include "http_client_connection.h"

HttpClientConnection*
http_client_connection_new(int socket, struct sockaddr_in* addr, GError** error)
{
  GError* local_error = NULL;

  HttpClientConnection* this = malloc(sizeof(HttpClientConnection));
  if (this == NULL)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
                  HTTP_MACHINE_ERROR_INIT,
                  "failed to allocate an HttpClientConnection object: %s",
                  g_strerror(errno));
      return NULL;
    }

  this->socket = socket;

  this->ip_address = malloc(sizeof(char) * IP_ADDRESS_LENGTH);
  if(this->ip_address == NULL)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
                  HTTP_MACHINE_ERROR_INIT,
                  "failed to allocate an ip address object: %s",
                  g_strerror(errno));
      free(this);
      return NULL;
    }

  inet_ntop(AF_INET, (void*) &addr->sin_addr, this->ip_address,
            IP_ADDRESS_LENGTH);

  this->io_channel = g_io_channel_unix_new(this->socket);

  GIOStatus status = g_io_channel_set_encoding(this->io_channel, NULL, NULL);
  if (status != G_IO_STATUS_NORMAL)
    {
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      free(this->ip_address);
      free(this);
      return NULL;
    }

  g_io_channel_set_buffered(this->io_channel, TRUE);

  return this;
}

gboolean
http_client_connection_destroy(HttpClientConnection* this, GError** error)
{
  GError* local_error = NULL;

  GIOStatus status = g_io_channel_shutdown(this->io_channel, TRUE,
                                           &local_error);
  if (status != G_IO_STATUS_NORMAL)
    {
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      return FALSE;
    }

  g_io_channel_unref(this->io_channel);

  free(this->ip_address);
  free(this);

  return TRUE;
}
