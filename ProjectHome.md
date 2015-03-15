# httprun #
httprun is a commandline utillity that is used to expose a process input and output streams over the http protocol. The arguments of the process are given in the URL that the process is associated with.

The parameters of the URL are not just limited to the query part. The parameters can be anywhere in the URL in a RESTFul fashion. The application httprun is built on top of a small http engine called HttpMachine wich is also developed in this project. HttpMachine is built in a general way and is intended to be reused for other projects.

Screenshot mockup:
```
$ httprun -p 8080 -u "/thumbs/{1}.jpg" -m "image/jpeg" -s "convert -scale 160x100 {1}.jpg -"
```
The mockup above makes small thumbnails of images in the current folder. A thumbnail will the be available like this `http://localhost:8080/thumbs/examplephoto.jpg`.

## Road Map ##

**0.0.1** (Current)
  * Make building possible with autotools
  * Find a hosting site

**0.0.2**
  * HTTP Requst parsing complete

**0.0.3**
  * Custom http handler callback support for the HttpMachine

**0.5.0**
This is expected to be the first version that can be used for anything.
  * Commandline parsing URL, Target, Mimetype
  * Simple mapping of URL's to files

**0.9.0**
  * Able to start executables and controls stdin and stdout

**1.0.0**
  * Mapping of URL string parameters to process/file arguments
  * Documentation Manpage, User guide etc..

