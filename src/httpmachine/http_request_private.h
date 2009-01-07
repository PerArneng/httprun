#include "http_request.h"

#ifndef HTTP_REQUEST_PRIVATE_H_
#define HTTP_REQUEST_PRIVATE_H_

gchar*
_http_request_read_request(GIOChannel* io_channel, GError** error);

gboolean
_http_request_parse_raw_request(HttpRequest* this, GError** error);

RequestMethod
_http_request_get_request_method(gchar* method);

#endif /* HTTP_REQUEST_PRIVATE_H_ */
