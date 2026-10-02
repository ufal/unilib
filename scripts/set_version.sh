#!/bin/sh

[ $# -ge 1 ] || { echo "Usage: $0 new_version" >&2; exit 1; }
version="$1"

BASE="$(dirname "$(dirname "$(readlink -f "$0")")")"

sed 's/^UNILIB=.*$/UNILIB='"$version"'/' -i $BASE/gen/Makefile
(cd $BASE/gen && make)
(cd $BASE/doc && make)
