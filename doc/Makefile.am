# Copyright (C) 2009-2012  CEA/DEN, EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

include $(top_srcdir)/src/bases/make_begin.am

.PHONY : latex

if SPHINX_IS_OK

jobmanagerdoc_DATA=html/index.html
html/index.html:$(RSTFILES)
	make htm
endif

#EXTRA_DIST= html htmldev _static
EXTRA_DIST= html html/index.html _static images

jobmanagerdocdir=$(docdir)/gui/JOBMANAGER

SPHINXOPTS      =
SOURCEDIR       = $(srcdir)
SPHINXBUILD     = sphinx-build
PAPEROPT_a4     = -D latex_paper_size=a4
ALLSPHINXOPTS   = -d doctrees $(PAPEROPT_a4) $(SPHINXOPTS) $(SOURCEDIR)

htm:
	mkdir -p html doctrees
	$(SPHINXBUILD) -c $(top_builddir)/doc -b html $(ALLSPHINXOPTS) html
	@echo
	@echo "Build finished. The HTML pages are in html."

latex:
	mkdir -p latex doctrees
	$(SPHINXBUILD) -c $(top_builddir)/doc -b latex $(ALLSPHINXOPTS) latex
	@echo
	@echo "Build finished; the LaTeX files are in latex."
	@echo "Run \`make all-pdf' or \`make all-ps' in that directory to" \
	      "run these through (pdf)latex."

html:
		 mkdir -p $@

#htmldev:
#		 mkdir -p $@

RSTFILES=          \
index.rst          \
intro.rst          \
resource.rst       \
job.rst            \
jobmanager_gui.rst \
advanced.rst


EXTRA_DIST+= $(RSTFILES)

install-data-local:
	$(INSTALL) -d $(DESTDIR)$(jobmanagerdocdir)
	if test -d "html"; then b=; else b="$(srcdir)/"; fi; \
	cp -rf $$b"html"/* $(DESTDIR)$(jobmanagerdocdir) ; \
	if test -f $$b"latex"/jobmanager.pdf; then cp -f $$b"latex"/jobmanager.pdf $(DESTDIR)$(jobmanagerdocdir) ; fi;

uninstall-local:
	chmod -R +w $(DESTDIR)$(jobmanagerdocdir)
	rm -rf $(DESTDIR)$(jobmanagerdocdir)/*

clean-local:
	-rm -rf html latex doctrees
	if test -d "html"; then rm -rf html ; fi
