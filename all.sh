#!/usr/bin/env bash
#
# Este script se encarga de invocar los tres programas que permiten la 
# conversion de un PNG a secuencia de pixeles, se procesan esos pixeles y
# posteriormente se convierte esa secuencia de pixeles a un archivo en formato
# PNG
#
# Autor: John Sanabria - john.sanabria@correounivalle.edu.co
# Fecha: 2024-08-22
#
# Set directory
IMAGES_DIR="./Images"

# Process all images in the folder
for img in "$IMAGES_DIR"/*; do
    if [ -f "$img" ]; then
        # Extract the file extension
        extension="${img##*.}"

        # If it's not a PNG, convert it to PNG (requires ImageMagick or similar tool)
        if [ "$extension" != "png" ]; then
            echo "Converting $img to PNG format..."
            convert "$img" "${img%.*}.png"
            img="${img%.*}.png"  # Update to the newly created PNG image
        fi

        # Define temp file for each image
        TEMP_FILE="${img%.*}.bin"

        # Call your existing commands
        echo "Processing $img..."
        python3 fromPNG2Bin.py "$img"
        ./main "$TEMP_FILE"
        python3 fromBin2PNG.py "${TEMP_FILE}.new"
    fi
done
