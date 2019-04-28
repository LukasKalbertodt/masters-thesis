#!/bin/sh

for file in "$@"
do
    echo "Converting $file"
    inkscape -z -D --file=$file --export-pdf=svg2pdf/$file.pdf --export-latex
done
