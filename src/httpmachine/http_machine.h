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

#ifndef _HTTPENGINE_H
#define	_HTTPENGINE_H
#include <glib-2.0/glib.h>
#include <stdint.h>
#include <errno.h>

#define HTTP_MACHINE_ERROR http_machine_error_quark()

GQuark
http_machine_error_quark();

typedef enum
{
  HTTP_MACHINE_ERROR_IO, /* an io error occurred */
  HTTP_MACHINE_ERROR_INIT, /* an error during intialisation */
  HTTP_MACHINE_ERROR_CLOSE /* an error during closing of the socket */
} HttpMachineError;

/**
 * Represents a HTTP engine that can accept HTTP requests on a port.
 * The HttpMachine is an active object which has its own thread pool
 * that handles the individual requests. The HttpMachine is created
 * with the method http_machine_new and then started with the method
 * http_machine_start and when finished working it is destructed with
 * the method http_machine_destroy.
 */
typedef struct
{

  /** The port that the http engine listens on. */
  uint16_t port;

  /** The server socket that the clients connect to. */
  int socket;

  /** The thread pool that holds all the client handlers. */
  GThreadPool* handler_thread_pool;

} HttpMachine;

/**
 * Creates a newly allocated HttpMachine object. The object must
 * be destroyed with the method http_machine_destroy. If any errors
 * occurrs when creating the engine a NULL pointer is returned.
 * When NULL is return the GError pointer is populated with data.
 *
 * @param port The port to listen on.
 * @param error Holding any error information if anything went wrong.
 * @return A newly allocated HttpMachine or NULL if anything went wrong.
 */
HttpMachine*
http_machine_new(uint16_t port, GError** error);

gboolean
http_machine_start(HttpMachine* this, GError** error);

gboolean
http_machine_destroy(HttpMachine* this, GError** error);

/* private */

int
_create_socket(uint16_t port, GError** error);

void
_handler_thread_function(gpointer data, gpointer user_data);

#endif	/* _HTTPENGINE_H */

