#!/bin/sh
libtoolize && aclocal && autoheader && autoconf && automake && echo "Now run './configure' and 'make'"

