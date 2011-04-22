/* guile-cairo
 * Copyright (C) 2007, 2011 Andy Wingo <wingo at pobox dot com>
 *
 * guile-cairo-compat.c: Guile 1.8 support
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

#include <libguile.h>

#include "guile-cairo-compat.h"


#if SCM_NEEDS_COMPAT (2, 0, 0)

SCM
scm_from_utf8_string (const char *str)
{
  /* Assume the user is in a UTF-8 locale. */
  return scm_from_locale_string (str);
}

char*
scm_to_utf8_string (SCM str)
{
  /* Assume the user is in a UTF-8 locale. */
  return scm_to_locale_string (str);
}

#endif /* NEEDS_COMPAT (2.0) */
