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
#include <stdio.h>
#include "http_request.h"
#include "http_request_private.h"

HttpRequest*
http_request_new(GIOChannel* io_channel, GError** error)
{

  GError* local_error = NULL;
  gchar* request = NULL;

  HttpRequest* this = g_malloc(sizeof(HttpRequest));

  request = _http_request_read_request(io_channel, &local_error);
  if (request == NULL) {
    g_propagate_error(error, local_error);
    g_error_free(local_error);
    g_free(this);
    return NULL;
  }

  this->_raw_request = request;

  if (!_http_request_parse_raw_request(this, &local_error))
    {
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      g_free(this);
      return NULL;
    }

  return this;
}

void
http_request_destroy(HttpRequest* this)
{
  g_free(this->_raw_request);
  g_free(this);
}

gchar*
_http_request_read_request(GIOChannel* io_channel, GError** error)
{

  gchar* request = NULL;
  GError* local_error = NULL;
  GIOStatus status;

  /* get the old delimiter */
   gint old_delimiter_length;
   const gchar* old_delimiter =
     g_io_channel_get_line_term(io_channel, &old_delimiter_length);

   /* make the io channel search for the end of the whole http request */
   g_io_channel_set_line_term(io_channel, "\r\n\r\n", 4);

   /* read the whole request as one "line" */
   status = g_io_channel_read_line(io_channel, &request,
                                   NULL, NULL, &local_error);
   if (status == G_IO_STATUS_ERROR)
     {
       g_propagate_error(error, local_error);
       g_error_free(local_error);
       return NULL;
     }
   else if (status == G_IO_STATUS_EOF)
     { /* EOF file should not be possible */
       g_set_error(error, HTTP_MACHINE_ERROR,
                   HTTP_MACHINE_ERROR_INIT,
                   "EOF reached to early when parsing the HttpRequest");
       g_error_free(local_error);
       return NULL;
     }

   /* reset the old delimiter */
   g_io_channel_set_line_term(io_channel,old_delimiter,old_delimiter_length);


   return request;
}

RequestMethod
_http_request_get_request_method(gchar* method) {

  if(g_strcmp0(method,"GET") == 0) {
    return REQUEST_METHOD_GET;
  } else if(g_strcmp0(method,"POST") == 0){
    return REQUEST_METHOD_POST;
  } else {
    /* FIXME: ADD SUPPORT FOR MORE METHODS */
    return REQUEST_METHOD_UNKNOWN;
  }

}

gboolean
_http_request_parse_raw_request(HttpRequest* this, GError** error)
{
  GError* local_error = NULL;
  gchar** request_lines = NULL;
  gchar** first_line_items = NULL;

  request_lines = g_strsplit_set(this->_raw_request, "\r\n", 0);
  first_line_items = g_strsplit_set(*request_lines, " ", 0);

  this->method = _http_request_get_request_method( *first_line_items );
  this->uri = g_strdup( *(first_line_items + 1) );
  this->protocol_version = g_strdup( *(first_line_items + 2) );

  g_strfreev(first_line_items);
  g_strfreev(request_lines);

  return TRUE;
}
