cmake_minimum_required( VERSION 2.8 )

project( SOIL )
add_library(SOIL SOIL.c image_DXT.c image_helper.c stb_image_aug.c)