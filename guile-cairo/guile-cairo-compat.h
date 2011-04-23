/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-compat.h: Guile 1.8 compatibility
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *                                                                  
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *                                                                  
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __GUILE_CAIRO_COMPAT_H__
#define __GUILE_CAIRO_COMPAT_H__

#include <libguile.h>



#define SCM_NEEDS_COMPAT(maj,min,mic)                                   \
  (maj > SCM_MAJOR_VERSION                                              \
   || (maj == SCM_MAJOR_VERSION                                         \
       && (min > SCM_MINOR_VERSION                                      \
           || (min == SCM_MINOR_VERSION && mic > SCM_MINOR_VERSION))))



#if SCM_NEEDS_COMPAT (2, 0, 0)
SCM scm_from_utf8_stringn (const char *str, size_t len);
char* scm_to_utf8_stringn (SCM str, size_t *len);

SCM scm_from_utf8_string (const char *str);
char* scm_to_utf8_string (SCM str);

SCM scm_from_utf8_symbol (const char *str);
char* scm_to_utf8_symbol (SCM str);

#endif /* NEEDS_COMPAT (2.0) */



#endif
