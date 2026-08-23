#!/bin/sh
# Render the interconnect harness diagram from ffb-harness.yml.
#
# Requires: pip install wireviz, and Graphviz's `dot` on PATH.
# Outputs .svg / .png / .html / .bom.tsv next to the YAML.
set -eu

cd "$(dirname "$0")"

if ! command -v dot >/dev/null 2>&1; then
    echo "error: graphviz 'dot' not found on PATH." >&2
    echo "  macOS:   brew install graphviz" >&2
    echo "  Debian:  apt install graphviz" >&2
    echo "  Windows: winget install Graphviz.Graphviz" >&2
    exit 1
fi

if command -v wireviz >/dev/null 2>&1; then
    wireviz ffb-harness.yml
else
    python -m wireviz.wv_cli ffb-harness.yml
fi

rm -f ffb-harness.tmp ffb-harness.tmp.*
echo "rendered: $(pwd)/ffb-harness.svg"
