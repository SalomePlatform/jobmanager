#  Copyright (C) 2009-2010  CEA/DEN, EDF R&D
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# --
# Author : André RIBES (EDF)
# --
# -----------------------------------------------------------------------------
#
AC_INIT([JOBMANAGER project], [0.1], [andre.ribes@edf.fr], [JobManager-Module])
AM_INIT_AUTOMAKE([tar-pax])
AC_CONFIG_HEADER(jobmanager_config.h)

# set up MODULE_NAME variable for dynamic construction of directories (resources, etc.)
MODULE_NAME=jobmanager
AC_SUBST(MODULE_NAME)

XVERSION=0x001
AC_SUBST(XVERSION)

RELEASE=$VERSION
AC_SUBST(RELEASE)

# This function return on stdout the absolute path of the filename in
# argument. Exemple:
# $ filename="../KERNEL_SRC/configure
# $ absfilename=`absolute_path $filename`
function absolute_path {
    filename=$1
    here=`pwd`
    apath=`dirname $filename`
    cd $apath
    apath=`pwd`
    cd $here
    echo $apath
}

# Build directory, where the configure script is executed.
ROOT_BUILDDIR=`pwd`
# Source directory, where the configure script is located.
ROOT_SRCDIR=`absolute_path $0`

AC_SUBST(ROOT_SRCDIR)
AC_SUBST(ROOT_BUILDDIR)

echo
echo Source root directory : $ROOT_SRCDIR
echo Build  root directory : $ROOT_BUILDDIR

# -----------------------------------------------------------------------------

AC_ENABLE_DEBUG(yes)
AC_DISABLE_PRODUCTION

#Linker options
AC_CANONICAL_BUILD
AC_LINKER_OPTIONS

#Mandatory products
AC_PROG_CXX
AM_PROG_LIBTOOL
AC_CHECK_LIB(dl,dlopen)
I2_CHECK_QT4
AC_CHECK_OMNIORB

# Optional products
CHECK_SPHINX

AC_CHECK_KERNEL
CHECK_GUI(SALOME_Session_Server,SALOME_Session_Server)

AM_CONDITIONAL([SALOME_KERNEL], [test "x$KERNEL_ROOT_DIR" != "x"])
AM_CONDITIONAL([HAS_GUI], [test "x$GUI_ROOT_DIR" != "x"])

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options Summary:"
echo
echo "Mandatory products:"
echo "  Qt4 (graphic interface)  : $qt_ok"
echo "  OmniOrb (CORBA) ........ : $omniORB_ok"
echo "  SALOME KERNEL .......... : $Kernel_ok"
echo "  SALOME GUI ............. : $SalomeGUI_ok"
echo
echo "Optional products:"
echo "  Sphinx (user doc) ...... : $sphinx_ok"
echo
echo "------------------------------------------------------------------------"
echo

if test "x$omniORB_ok" = "xno"; then
  AC_MSG_ERROR([OmniOrb is required],1)
fi
if test "x$qt_ok" = "xno"; then
  AC_MSG_ERROR([Qt4 is required],1)
fi
if test "x$Kernel_ok" = "xno"; then
  AC_MSG_ERROR([SALOME KERNEL is required],1)
fi
if test "x$SalomeGui_ok" = "xno"; then
  AC_MSG_ERROR([SALOME GUI is required],1)
fi

AC_OUTPUT([ \
  Makefile \
  src/Makefile \
  src/bases/Makefile \
  src/engine/Makefile \
  src/genericgui/Makefile \
  src/wrappers/Makefile \
  src/standalone/Makefile \
  src/standalone/start_jobmanager.sh \
  src/salomegui/Makefile \
  doc/Makefile \
])
