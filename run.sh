#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${BUILD_DIR:-build-native}"
MESON_BIN="${MESON_BIN:-/usr/bin/meson}"
PLUGIN_NAME="wigglewobble"
LAST_PLUGIN_FILE="$ROOT_DIR/.last_plugin"

usage() {
	cat <<'EOF'
Usage:
  ./run.sh dev      # Compile and hot-reload plugin in current Hyprland session
  ./run.sh nested   # Compile and start Hyprland with hyprlandd.conf
  ./run.sh build    # Compile only

Optional environment variables:
  BUILD_DIR=<dir>   Meson build directory (default: build-native)
  MESON_BIN=<path>  Meson binary (default: /usr/bin/meson)
EOF
}

ensure_build_dir() {
	if [[ ! -d "$ROOT_DIR/$BUILD_DIR" ]]; then
		"$MESON_BIN" setup "$ROOT_DIR/$BUILD_DIR"
	fi
}

compile_plugin() {
	ensure_build_dir
	"$MESON_BIN" compile -C "$ROOT_DIR/$BUILD_DIR"
}

unload_known_plugins() {
	if [[ -f "$LAST_PLUGIN_FILE" ]]; then
		local last_path
		last_path="$(cat "$LAST_PLUGIN_FILE")"
		if [[ -n "$last_path" ]]; then
			hyprctl plugin unload "$last_path" >/dev/null 2>&1 || true
		fi
	fi

	# Also unload any mapped wigglewobble instances to avoid duplicates.
	local hypr_pid
	hypr_pid="$(pidof Hyprland | awk '{print $1}')"
	if [[ -n "${hypr_pid:-}" ]]; then
		rg -n "$PLUGIN_NAME" "/proc/$hypr_pid/maps" 2>/dev/null \
			| awk '{print $6}' \
			| sed 's/ (deleted)//' \
			| sort -u \
			| while read -r path; do
				[[ -n "$path" ]] || continue
				hyprctl plugin unload "$path" >/dev/null 2>&1 || true
			done
	fi
}

load_fresh_plugin() {
	local built_so
	built_so="$ROOT_DIR/$BUILD_DIR/src/libwigglewobble.so"
	if [[ ! -f "$built_so" ]]; then
		echo "Built plugin not found: $built_so" >&2
		exit 1
	fi

	local unique_so
	unique_so="$ROOT_DIR/$BUILD_DIR/libwigglewobble_$(date +%s).so"
	cp "$built_so" "$unique_so"

	unload_known_plugins
	hyprctl plugin load "$unique_so"
	echo "$unique_so" > "$LAST_PLUGIN_FILE"

	hyprctl plugin list -j | python3 -c 'import json,sys; data=json.load(sys.stdin); print(f"{len(data)} plugin(s) loaded:"); [print(" -", p["name"]) for p in data]'
}

cmd="${1:-dev}"

case "$cmd" in
	dev)
		compile_plugin
		load_fresh_plugin
		;;
	nested)
		compile_plugin
		Hyprland -c "$ROOT_DIR/hyprlandd.conf"
		;;
	build)
		compile_plugin
		;;
	-h|--help|help)
		usage
		;;
	*)
		echo "Unknown command: $cmd" >&2
		usage
		exit 1
		;;
esac
