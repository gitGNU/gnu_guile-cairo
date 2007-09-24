# guile-gnome
# Copyright (C) 2007 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or    
# modify it under the terms of the GNU General Public License as   
# published by the Free Software Foundation; either version 2 of   
# the License, or (at your option) any later version.              

# This program is distributed in the hope that it will be useful,  
# but WITHOUT ANY WARRANTY; without even the implied warranty of   
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    
# GNU General Public License for more details.                     

# You should have received a copy of the GNU General Public License
# along with this program; if not, contact:

# Free Software Foundation           Voice:  +1-617-542-5942
# 59 Temple Place - Suite 330        Fax:    +1-617-542-2652
# Boston, MA  02111-1307,  USA       gnu@gnu.org


#
# Makefile snippet
#

wrapset_module = "(gnome gw $(wrapset_stem)-spec)"
wrapset_name = "gnome-$(wrapset_stem)"
GTK_DOC_TO_TEXI_STUBS = '((@ (gnome gw support gtk-doc) gtk-doc->texi-stubs) (cdr (program-arguments)))'
GTK_DOC_DEFUN_HEURISTICS_METHOD = heuristics
GTK_DOC_DEFUN_HEURISTICS_ARGS = (your-module-here)
GTK_DOC_DEFUN_GWRAP_METHOD = g-wrap
GTK_DOC_DEFUN_GWRAP_ARGS = $(wrapset_module) $(wrapset_name)
GTK_DOC_DEFUN_METHOD = $(GTK_DOC_DEFUN_HEURISTICS_METHOD)
GTK_DOC_DEFUN_ARGS = $(GTK_DOC_DEFUN_HEURISTICS_ARGS)
GTK_DOC_TO_TEXI_DEFUNS = "(apply (@ (gnome gw support gtk-doc) gtk-doc->texi-defuns) (cadr (program-arguments)) '$(GTK_DOC_DEFUN_METHOD) '($(GTK_DOC_DEFUN_ARGS)) (cddr (program-arguments)))"
GUILE = $(top_builddir)/dev-environ guile

generate-stubs:
	$(GUILE) $(GUILE_FLAGS) -c $(GTK_DOC_TO_TEXI_STUBS) $(docbook_xml_files)

generate-defuns:
	$(GUILE) $(GUILE_FLAGS) -c $(GTK_DOC_TO_TEXI_DEFUNS) ./overrides.texi $(docbook_xml_files)
