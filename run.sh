#!/bin/env bash

set -e

meson compile -Cbuild
Hyprland -c hyprlandd.conf
