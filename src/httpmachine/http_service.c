
#include <stdlib.h>
#include "http_service.h"
#include "http_machine.h"

HttpService*
http_service_new(gchar* url, gchar* target, TargetType type,
                  gchar* mimetype, GError** error)
{

  HttpService* this = malloc(sizeof(HttpService));
  if(this == NULL)
    {
      g_set_error(error, HTTP_MACHINE_ERROR,
                  HTTP_MACHINE_ERROR_INIT,
                  "could not allocate a HttpServicet: %s",
                   g_strerror(errno));
      return NULL;
    }

  this->url = g_strdup(url);
  this->target = g_strdup(target);
  this->target_type = type;
  this->mimetype = g_strdup(mimetype);

  return this;

}

gboolean
http_service_destroy(HttpService* this)
{
  free(this->url);
  free(this->target);
  free(this->mimetype);
  free(this);

  return TRUE;
}

