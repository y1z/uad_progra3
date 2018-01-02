#pragma once

#ifndef _GLOBALS_H
#define _GLOBALS_H

#define CAPPPARCIAL1_WINDOW_TITLE "OpenGL + GLFW Library test"
#define CAPPPARCIAL2_WINDOW_TITLE "Load 3D Object Test"
#define CAPPPARCIAL3_WINDOW_TITLE "Parcial 3"

// Quick workaround because the file paths on my dev pc are not the same as the paths in the UAD lab pc
// So I comment/uncomment the following line depending on where I'm compiling

//#define IN_CLASS 1

#ifdef  IN_CLASS

#define VERTEX_SHADER_TEST_OBJECT      "C:\\PROGRA2\\MEDIA\\SHADERS\\glsl_testobject.vert"
#define FRAGMENT_SHADER_TEST_OBJECT    "C:\\PROGRA2\\MEDIA\\SHADERS\\glsl_testobject.frag"
#define VERTEX_SHADER_3D_OBJECTS       "C:\\PROGRA2\\MEDIA\\SHADERS\\glsl_3dobject.vert"
#define FRAGMENT_SHADER_3D_OBJECTS     "C:\\PROGRA2\\MEDIA\\SHADERS\\glsl_3dobject.frag"
#define VERTEX_SHADER_MENU             "C:\\PROGRA2\\MEDIA\\SHADERS\\glsl_menu.vert"
#define FRAGMENT_SHADER_MENU           "C:\\PROGRA2\\MEDIA\\SHADERS\\glsl_menu.frag"
#define MENU_TEXTURE_FILE              "C:\\PROGRA2\\MEDIA\\TEXTURES\\Menu.tga"
#define MENU_TEXTURE_FILE_EXAMEN_FINAL "C:\\PROGRA2\\MEDIA\\TEXTURES\\MenuExamenFinal.tga"

#else

#define VERTEX_SHADER_TEST_OBJECT      "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\SHADERS\\glsl_testobject.vert"
#define FRAGMENT_SHADER_TEST_OBJECT    "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\SHADERS\\glsl_testobject.frag"
#define VERTEX_SHADER_3D_OBJECTS       "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\SHADERS\\glsl_3dobject.vert"
#define FRAGMENT_SHADER_3D_OBJECTS     "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\SHADERS\\glsl_3dobject.frag"
#define VERTEX_SHADER_MENU             "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\SHADERS\\glsl_menu.vert"
#define FRAGMENT_SHADER_MENU           "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\SHADERS\\glsl_menu.frag"
#define MENU_TEXTURE_FILE              "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\TEXTURES\\Menu.tga"
#define MENU_TEXTURE_FILE_EXAMEN_FINAL "C:\\Users\\Joel\\Git\\uad_progra3\\uad_progra3\\Resources\\MEDIA\\TEXTURES\\MenuExamenFinal.tga"

#endif //  IN_CLASS

#endif // !_GLOBALS_H
