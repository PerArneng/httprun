/* --
 * Copyright (C) 2009 Per Arneng <per dot arneng (a) anyplanet dot com>
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

#include <stdio.h>
#include <stdlib.h>
#include <http_machine.h>

int
main(int argc, char** argv)
{

  GError* error = NULL;

  g_thread_init(NULL);

  HttpMachine* http_machine = http_machine_new(8080, &error);
  if (http_machine == NULL)
    {
      fprintf(stderr,"could not create the http engine: %s\n", error->message);
      g_error_free(error);
      return (EXIT_FAILURE);
    }

  if (!http_machine_start(http_machine, &error))
    {
      fprintf(stderr,"could not start the http engine: %s\n", error->message);
      g_error_free(error);
      return (EXIT_FAILURE);
    }

  if (!http_machine_destroy(http_machine, &error))
    {
      fprintf(stderr,"could not start destroy http engine: %s\n",
              error->message);
      g_error_free(error);
      return (EXIT_FAILURE);
    }

  return (EXIT_SUCCESS);
}

