#!/bin/sh

set -e
autoreconf -vif
./configure "$@"
echo "Now type 'make' to compile $package."
