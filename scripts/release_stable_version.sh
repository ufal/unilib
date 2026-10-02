#!/bin/sh

set -e

[ $# -ge 1 ] || { echo "Usage: $0 new_version" >&2; exit 1; }
version="$1"

BASE="$(dirname "$(dirname "$(readlink -f "$0")")")"

sh $BASE/scripts/set_version.sh "$version"

changelog_version="Version $version [$(date +"%d %b %Y")]"
dashes="$(echo "$changelog_version" | sed 's/./-/g')"
sed "1s/.*/$changelog_version/; 2s/.*/$dashes/" -i $BASE/CHANGES.md

git -C "$BASE" commit -aevm "Release version $version."
git tag v$version
