#!/bin/sh

# If you've non-standard directories, set these
#ACLOCAL_DIR=
#GETTEXT_DIR=

if test "$ACLOCAL_DIR" != ""; then
  ACLOCAL="aclocal -I $ACLOCAL_DIR"
  export ACLOCAL
fi

if test ! -f ChangeLog; then
  # automake dies unless this exists. It's generated in Makefile
  touch -t `date +%m%d`0000 ChangeLog
fi

autoreconf -i
