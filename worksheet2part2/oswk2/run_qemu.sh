#!/usr/bin/env bash
set -euo pipefail
ISO="os.iso"
if [ ! -f "$ISO" ]; then
  echo "os.iso not found — building..."
  make iso
fi
if ! command -v qemu-system-i386 >/dev/null 2>&1 ; then
  echo "Error: qemu-system-i386 not installed. Please install qemu."
  exit 1
fi
echo "Starting QEMU (nographic). To exit press Ctrl-A X or Ctrl-C if necessary."
qemu-system-i386 -cdrom "$ISO" -m 64M -nographic
