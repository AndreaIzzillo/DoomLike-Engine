#!/usr/bin/env bash

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <map_name> [options]"
  exit 1
fi

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
map_name="$1"
options="${@:2}"

cd "$repo_root" || exit 1

if ! "$script_dir/build.sh"; then
  echo "build.sh failed."
  if ! "$script_dir/rebuild.sh"; then
    echo "rebuild.sh failed."
    exit 1
  fi
fi

exec ./build/isim "${map_name}/map" ${options}
