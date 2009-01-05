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
#include "http_request.h"

HttpRequest*
http_request_new(GIOChannel* io_channel, GError** error)
{

  GError* local_error = NULL;

  HttpRequest* this = malloc(sizeof(HttpRequest));
  if(this == NULL)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
                  HTTP_MACHINE_ERROR_INIT,
                  "could not allocate the HttpRequest object: %s",
                  g_strerror(errno));
      return NULL;
    }


  /* get the old delimiter */
  gint old_delimiter_length;
  const gchar* old_delimiter =
    g_io_channel_get_line_term(io_channel, &old_delimiter_length);

  /* make the io channel search for the end of the whole http request */
  g_io_channel_set_line_term(io_channel, "\r\n\r\n", 4);

  /* read the whole request as one "line" */
  GIOStatus status = g_io_channel_read_line(io_channel, &this->_raw_request,
                                            NULL, NULL, &local_error);
  if (status == G_IO_STATUS_ERROR)
    {
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      free(this);
      return NULL;
    }
  else if (status == G_IO_STATUS_EOF)
    { /* EOF file should not be possible */
      g_set_error(error, HTTP_MACHINE_ERROR,
                  HTTP_MACHINE_ERROR_INIT,
                  "EOF reached to early when parsing the HttpRequest");
      g_error_free(local_error);
      free(this);
      return NULL;
    }

  // reset the old delimiter
  g_io_channel_set_line_term(io_channel,old_delimiter,old_delimiter_length);

  return this;
}

gboolean
http_request_destroy(HttpRequest* this, GError** error)
{
  free(this->_raw_request);
  free(this);

  return TRUE;
}

