/* Copyright 2017 The serverX Authors. All Rights Reserved.
 * Maintainer: Bytes Club (https://bytesclub.github.io)<bytes-club@googlegroups.com>

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
==============================================================================*/

// Library Header File
// serverXL.h

#ifndef __serverXL_h
#define __serverXL_h

// feature test macro requirements

#define _GNU_SOURCE
#define _USE_BSD
#define _XOPEN_SOURCE 700
#define _XOPEN_SOURCE_EXTENDED

// limits on an HTTP request's size, based on Apache's
// http://httpd.apache.org/docs/2.2/mod/core.html

#define LimitRequestFields 50
#define LimitRequestFieldSize 4094
#define LimitRequestLine 8190

// number of bytes for buffers
#define BYTES 512

// header files

#ifndef __getopt_h
#include <getopt.h>
#endif
#ifndef __dirnet_h
#include <dirent.h>
#endif
#ifndef __errno_h
#include <errno.h>
#endif
#ifndef __limits_h
#include <limits.h>
#endif
#ifndef __math_h
#include <math.h>
#endif
#ifndef __signal_h
#include <signal.h>
#endif
#ifndef __stdbool_h
#include <stdbool.h>
#endif
#ifndef __stdio_h
#include <stdio.h>
#endif
#ifndef __stdlib_h
#include <stdlib.h>
#endif
#ifndef __string_h
#include <string.h>
#endif
#ifndef __strings_h
#include <strings.h>
#endif
#ifndef __sys_stat_h
#include <sys/stat.h>
#endif
#ifndef __sys_types_h
#include <sys/types.h>
#endif

#if defined(_WIN32) || defined(__WIN32__)
#ifndef __winsock2_h
#include <winsock2.h>
#endif
#ifndef __ws2tcpip_h
#include <ws2tcpip.h>
#endif
#ifndef __windows_h
#include <windows.h>
#endif
#define SIGNAL(SIG, HANDLER) \
    signal((SIG), (HANDLER));
#if defined(_MSC_VER)
#define popen  _popen
#define pclose _pclose
#endif

#else
#ifndef __arpa_inet_h
#include <arpa/inet.h>
#endif
#ifndef __sys_socket_h
#include <sys/socket.h>
#endif
#ifndef __unistd_h
#include <unistd.h>
#endif
#define SIGNAL(SIG, HANDLER)     \
    struct sigaction act;        \
    act.sa_handler = (HANDLER);  \
    act.sa_flags = 0;            \
    sigemptyset(&act.sa_mask);   \
    sigaction((SIG), &act, NULL);
#endif

// types
typedef unsigned char BYTE;

// global variables
extern char* root;
extern int cfd, sfd;
extern bool signaled;

// prototypes

/**
 * Checks (without blocking) whether a client has connected to server. 
 * Returns true iff so.
 */
bool connected(void);

/**
 * Responds to client with specified status code.
 */
void error(unsigned short code);

/**
 * Frees memory allocated by scandir.
 */
void freedir(struct dirent** namelist, int n);

/**
 * Handles signals.
 */
void handler(int signal);

/**
 * Escapes string for HTML. Returns dynamically allocated memory for escaped
 * string that must be deallocated by caller.
 */
char* htmlspecialchars(const char* s);

/**
 * Checks, in order, whether index.php or index.html exists inside of path.
 * Returns path to first match if so, else NULL.
 */
char* indexes(const char* path);

/**
 * Interprets PHP file at path using query string.
 */
void interpret(const char* path, const char* query);

/**
 * Responds to client with directory listing of path.
 */
void list(const char* path);

/**
 * Loads a file into memory dynamically allocated on heap.
 * Stores address thereof in *content and length thereof in *length.
 */
bool load(FILE* file, BYTE** content, size_t* length);

/**
 * Returns MIME type for supported extensions, else NULL.
 */
const char* lookup(const char* path);

/**
 * Parses a request-line, storing its absolute-path at abs_path 
 * and its query string at query, both of which are assumed
 * to be at least of length LimitRequestLine + 1.
 */
bool parse(const char* line, char* path, char* query);

/**
 * Returns status code's reason phrase.
 *
 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6
 * https://tools.ietf.org/html/rfc2324
 */
const char* reason(unsigned short code);

/**
 * Redirects client to uri.
 */
void redirect(const char* uri);

/**
 * Reads (without blocking) an HTTP request's headers into memory dynamically allocated on heap.
 * Stores address thereof in *message and length thereof in *length.
 */
bool request(char** message, size_t* length);

/**
 * Responds to a client with status code, headers, and body of specified length.
 */
void respond(int code, const char* headers, const char* body, size_t length);

/**
 * Starts server on specified port rooted at path.
 */
void start(short port, const char* path);

/**
 * Stop server, deallocating any resources.
 */
void stop(void);

/**
 * Transfers file at path with specified type to client.
 */
void transfer(const char* path, const char* type);

/**
 * URL-decodes string, returning dynamically allocated memory for decoded string
 * that must be deallocated by caller.
 */
char* urldecode(const char* s);

#endif