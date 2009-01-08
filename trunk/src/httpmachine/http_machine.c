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

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "http_machine.h"
#include "http_client_connection.h"
#include "http_request.h"

gboolean
http_machine_start(HttpMachine* this, GError** error)
{

  struct sockaddr_in client_addr;
  size_t client_addr_size = -1;
  int client_socket = -1;
  GError* local_error = NULL;
  HttpClientConnection* client = NULL;

  int retval = listen(this->socket, 100);
  if (retval < 0)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
                  HTTP_MACHINE_ERROR_INIT, "could not listen on the socket: %s",
                   g_strerror(errno));
      return FALSE;
    }

  client_addr_size = sizeof(client_addr);

  while (1)
    {

      client_socket = accept(this->socket, (struct sockaddr*) &client_addr,
                             (socklen_t*) &client_addr_size);
      if (client_socket < 0)
        {
          g_set_error(error, HTTP_MACHINE_ERROR,
          HTTP_MACHINE_ERROR_IO, "accept failed: %s", g_strerror(errno));
          return FALSE;
        }

      local_error = NULL;
      client = http_client_connection_new(client_socket,
                                          (struct sockaddr_in*)&client_addr,
                                           &local_error);
      if(client == NULL) {
        g_propagate_error(error, local_error);
        g_error_free(local_error);
        return FALSE;
      }

      g_thread_pool_push(this->handler_thread_pool, (gpointer) client,
          NULL);
    }
  return TRUE;
}

void
_handler_thread_function(gpointer data, gpointer user_data)
{

  GError* local_error = NULL;
  HttpClientConnection* client = (HttpClientConnection*)data;
  char* temp_message = NULL;
  gsize written = -1;
  GIOStatus status;

  HttpRequest* http_request = http_request_new(client->io_channel,
                                               &local_error);
  if(http_request == NULL)
    {
      fprintf(stderr, "failed to read request: %s\n", local_error->message);
      g_error_free(local_error);
      return;
    }

  /*
  printf("request-> %d, %s, %s\n", http_request->method,
         http_request->uri,
         http_request->protocol_version); */

  temp_message = "HTTP/1.0 200 OK\r\nConnection: close\r\n\r\n"
    "<html><body style='background-color: blue;'><h1 style='color: white;'>"
    "Hello, World!</h1></body></html>";

  status = g_io_channel_write_chars(client->io_channel, temp_message,
                                    strlen(temp_message),
                                    &written, &local_error);
  if(status != G_IO_STATUS_NORMAL)
    {
      fprintf(stderr,"failed to write: %s\n",local_error->message);
      g_error_free(local_error);
    }


  status = g_io_channel_flush(client->io_channel, &local_error);
  if(status != G_IO_STATUS_NORMAL)
    {
      fprintf(stderr,"failed to flush: %s\n",local_error->message);
      g_error_free(local_error);
      return;
    }


  http_request_destroy(http_request);

  if(!http_client_connection_destroy(client, &local_error)) {
    fprintf(stderr,"could not shutdown client socket: %s\n",
            local_error->message);
    g_error_free(local_error);
    return;
  }

}

gboolean
http_machine_add_service(HttpMachine* this, gchar* url, gchar* target,
                         TargetType target_type, gchar* mimetype,
                         GError** error)
{
  GError* local_error = NULL;
  HttpService* http_service = http_service_new(url, target, target_type,
                                               mimetype,
                                               &local_error);
  if (http_service == NULL)
    {
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      return FALSE;
    }

  /* FIXME: IMPLEMENT ME */

  return TRUE;
}

int
_create_socket(uint16_t port, GError** error)
{

  int sock = -1;
  struct sockaddr_in name;

  /* create the socket */
  sock = socket(PF_INET,SOCK_STREAM, 0);
  if (sock < 0)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
      HTTP_MACHINE_ERROR_INIT, "could not create the socket: %s", g_strerror(
          errno));
      return -1;
    }

  /* Give the socket a name */
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
      HTTP_MACHINE_ERROR_INIT, "could not bind the socket: %s",
          g_strerror(errno));
      return -1;
    }

  return sock;
}

HttpMachine*
http_machine_new(uint16_t port, GError** error)
{

  GError* local_error = NULL;

  HttpMachine* engine = (HttpMachine*) malloc(sizeof(HttpMachine));
  if (engine == NULL)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
      HTTP_MACHINE_ERROR_INIT, "could not allocate the engine: %s",
      g_strerror(errno));
      return NULL;
    }

  engine->port = port;
  engine->socket = _create_socket(port, &local_error);
  if (engine->socket < 0)
    {
      free(engine);
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      return NULL;
    }

  engine->handler_thread_pool = g_thread_pool_new(&_handler_thread_function,
      NULL, 10, TRUE, &local_error);
  if (engine->handler_thread_pool == NULL)
    {
      free(engine);
      g_propagate_error(error, local_error);
      g_error_free(local_error);
      return NULL;
    }

  return engine;
}

gboolean
http_machine_destroy(HttpMachine* this, GError** error)
{

  int status = shutdown(this->socket, 2);
  if (status < 0)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
      HTTP_MACHINE_ERROR_CLOSE, "could not close the socket: %s", g_strerror(
          errno));
      return FALSE;
    }

  free(this);

  return TRUE;
}

GQuark
http_machine_error_quark()
{
  return g_quark_from_static_string("http-engine-error-quark");
}
