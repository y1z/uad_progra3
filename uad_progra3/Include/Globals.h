#pragma once

#ifndef _GLOBALS_H
#define _GLOBALS_H
// MATH 
#define PI_OVER_180 3.14129/180
#define PI  3.14159f

// WINDOW TITLES
#define CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE      "Empty app - OpenGL + GLFW Library initialization"
#define CAPP_PROGRA3_CUBES_WINDOW_TITLE         "Cubes app"
#define CAPP_PROGRA3_OBJ_LOADER_WINDOW_TITLE    ".OBJ Loader app"
#define CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE     "Geometric shapes app"

// SHADER FILENAMES
#define VERTEX_SHADER_3D_OBJECT_COLOR           "SHADERS\\glsl_testobject.vert"
#define FRAGMENT_SHADER_3D_OBJECT_COLOR         "SHADERS\\glsl_testobject.frag"

#define VERTEX_SHADER_3D_OBJECT                 "SHADERS\\glsl_3dobject.vert"
#define FRAGMENT_SHADER_3D_OBJECT               "SHADERS\\glsl_3dobject.frag"

#define VERTEX_SHADER_TEXTURED_3D_OBJECT        "SHADERS\\glsl_3dobject_textured.vert"
#define FRAGMENT_SHADER_TEXTURED_3D_OBJECT      "SHADERS\\glsl_3dobject_textured.frag"

#define VERTEX_SHADER_MC_CUBE                   "SHADERS\\glsl_mccube.vert"
#define FRAGMENT_SHADER_MC_CUBE                 "SHADERS\\glsl_mccube.frag"

#define VERTEX_SHADER_MENU                      "SHADERS\\glsl_menu.vert"
#define FRAGMENT_SHADER_MENU                    "SHADERS\\glsl_menu.frag"

#define VERTEX_SHADER_WIREFRAME                 "SHADERS\\glsl_wireframe.vert"
#define FRAGMENT_SHADER_WIREFRAME               "SHADERS\\glsl_wireframe.frag"

// TEXTURE FILENAMES
#define MENU_TEXTURE_FILE                       "TEXTURES\\Menu.tga"
#define MENU_TEXTURE_FILE_EXAMEN_FINAL          "TEXTURES\\MenuExamenFinal.tga"

#define MC_GRASS_DIRT_TEXTURE                   "TEXTURES\\mc_grass_dirt_cube_512x384.tga"
#define MC_DIRT_TEXTURE                         "TEXTURES\\mc_dirt_cube_512x384.tga"
#define MC_DIRT_2_TEXTURE                       "TEXTURES\\mc_dirt2_cube_512x384.tga"
#define MC_GRASS_2_TEXTURE                      "TEXTURES\\mc_grass2_512x384.tga"
#define MC_GRASS_2_DIRT_TEXTURE                 "TEXTURES\\mc_grass2_dirt_512x384.tga"
#define MC_GRASS_3_TEXTURE                      "TEXTURES\\mc_grass4_512x384.tga"
#define MC_LEAVES_TEXTURE                       "TEXTURES\\mc_leaves_cube_512x384.tga"
#define MC_WOOD_TEXTURE                         "TEXTURES\\mc_wood_512x384.tga"

// 
#define DEFAULT_ROTATION_SPEED                  90.0f
#define DEFAULT_CAMERA_MOVE_SPEED               0.015f

#endif // !_GLOBALS_H
