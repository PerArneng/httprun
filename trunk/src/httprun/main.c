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
#include <signal.h>
#include <http_machine.h>
#include <http_service.h>


HttpMachine* http_machine;

void
interrupt_handler(int signal_number) {

  GError* local_error;

  if (http_machine != NULL)
    {
      if(!http_machine_destroy(http_machine, &local_error))
        {
          g_warning("could not destroy the HttpMachine on interrupt!");
        }
    }

  /* restore the default signal handler*/
  signal(signal_number, SIG_DFL);

  /* call the default signal handler */
  raise(signal_number);
}

void
setup_interrupt_handler()
{
  if (signal (SIGINT, interrupt_handler) == SIG_IGN)
    signal (SIGINT, SIG_IGN);
  if (signal (SIGHUP, interrupt_handler) == SIG_IGN)
    signal (SIGHUP, SIG_IGN);
  if (signal (SIGTERM, interrupt_handler) == SIG_IGN)
    signal (SIGTERM, SIG_IGN);
}

int
main(int argc, char** argv)
{

  GError* error = NULL;

  setup_interrupt_handler();
  g_thread_init(NULL);

  http_machine = http_machine_new(8080, &error);
  if (http_machine == NULL)
    {
      fprintf(stderr,"could not create the http engine: %s\n", error->message);
      g_error_free(error);
      return (EXIT_FAILURE);
    }

  if (!http_service_new("/", "Makefile.am", TARGET_TYPE_PATH, "text/plain",
                        &error) )
    {
      fprintf(stderr,"could not add the http service: %s\n", error->message);
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
      fprintf(stderr,"could not destroy the http engine: %s\n",
              error->message);
      g_error_free(error);
      return (EXIT_FAILURE);
    }

  return (EXIT_SUCCESS);
}

