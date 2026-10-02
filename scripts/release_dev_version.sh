#!/bin/sh

set -e

[ $# -ge 1 ] || { echo "Usage: $0 new_version" >&2; exit 1; }
version="$1"

BASE="$(dirname "$(dirname "$(readlink -f "$0")")")"

sh $BASE/scripts/set_version.sh "$version"

changelog_version="Version $version"
dashes="$(echo "$changelog_version" | sed 's/./-/g')"
sed "1i $changelog_version\n$dashes\n\n" -i $BASE/CHANGES.md

git -C "$BASE" commit -aevm "Bump version to $version."
