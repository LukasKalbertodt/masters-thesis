#!/bin/bash

set -e

if [ -f "/mnt/ramdisk/README.md" ]; then
    echo "ramdisk already exists"
    exit 1
fi

mkdir -p /mnt/ramdisk
mount -t tmpfs -o size=500M none /mnt/ramdisk
cp $(dirname $0)/data/* /mnt/ramdisk
chmod -R 777 /mnt/ramdisk

echo "Created 500MB ramdisk at /mnt/ramdisk"
