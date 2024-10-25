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
# IMAGES_DIR="./Images"

# # Process all images in the folder
# for img in "$IMAGES_DIR"/*; do
#     if [ -f "$img" ]; then
#         # Extract the file extension
#         extension="${img##*.}"

#         # If it's not a PNG, convert it to PNG (requires ImageMagick or similar tool)
#         if [ "$extension" != "png" ]; then
#             echo "Converting $img to PNG format..."
#             convert "$img" "${img%.*}.png"
#             img="${img%.*}.png"  # Update to the newly created PNG image
#         fi

#         # Define temp file for each image
#         TEMP_FILE="${img%.*}.bin"

#         # Call your existing commands
#         echo "Processing $img..."
#         python3 fromPNG2Bin.py "$img"
#         ./main "$TEMP_FILE"
#         python3 fromBin2PNG.py "${TEMP_FILE}.new"
#     fi
# done


image_dir="./images"

# Find the first three .png or .jpg files in the directory
# and store them in an array
files=($(find "$image_dir" -maxdepth 1 -type f \( -iname "*.png" -o -iname "*.jpg" \)))

# count=${#files[@]}
# Print the count
# echo "Number of image files: $count"
# Check if any files were found
# if [ ${#files[@]} -eq 0 ]; then
#    echo "No image files found in the directory."
#    exit 1
#fi

# Process or print the first three image files
 for file in "${files[@]}"; do
     echo "Processing: $file"
 	 INPUT_PNG="$file"
	 dimensions=$(identify -format "%wx%h" "$file")
     echo "Dimensions: $dimensions"
	 # Remove the file extension from the original file name
     BASE_NAME="${file%.*}"
     TEMP_FILE="${BASE_NAME}.bin"
     python3 fromPNG2Bin.py ${INPUT_PNG}
     ./main ${TEMP_FILE}
     python3 fromBin2PNG.py ${TEMP_FILE}.new
     # You can add further commands here to process the files
 done


# Convert .jpg files to .png
# for file in "$image_dir"/*.jpg; do
#     # Extract the filename without the extension
#     filename=$(basename "$file" .jpg)
    
#     # Convert the file to PNG using ImageMagick
#     convert "$file" "$image_dir/${filename}.png"
    
#     echo "Converted $file to ${filename}.png"
# done

# # Now you can pass the converted .png files to main.c or another process
# for png_file in "$image_dir"/*.png; do
#     echo "Passing $png_file to main.c"
#     # Here you would include the logic to pass it to your C program
#     # e.g., ./main "$png_file"
# don
