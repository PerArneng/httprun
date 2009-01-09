
#include <stdlib.h>
#include "http_service.h"
#include "http_machine.h"

HttpService*
http_service_new(gchar* url, gchar* target, TargetType type,
                  gchar* mimetype, GError** error)
{

  HttpService* this = NULL;

  this = g_malloc(sizeof(HttpService));
  this->url = g_strdup(url);
  this->target = g_strdup(target);
  this->target_type = type;
  this->mimetype = g_strdup(mimetype);

  return this;

}

gboolean
http_service_destroy(HttpService* this)
{
  g_free(this->url);
  g_free(this->target);
  g_free(this->mimetype);
  g_free(this);

  return TRUE;
}

