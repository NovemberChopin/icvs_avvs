#!/bin/bash
cd $1
./generatermap_EX --input_path $2 --output_path $3 --leftlane_width=$4 --rightlane_width=$5 --isleft_doublelane=$6  --isright_doublelane=$7
cd $3
sync
mkdir map_file
cp -r map/* map_file
cp -r map/* $1/../../map_file/
sync
tar -zcvf map.tar.gz map_file
rm -rf map_file