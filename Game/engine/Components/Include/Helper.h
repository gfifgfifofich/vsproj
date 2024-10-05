#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif


#include <stdio.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <strstream>
#include <vector>
#include <map>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>


#include "Mathematics.h"

extern glm::vec2 CameraPosition;
extern glm::vec2 CameraScale;

extern GLuint WIDTH, HEIGHT ;
extern std::string TextFromKeyboard;
extern bool TextEditMode;// disables keyboard key inputs, only TextFromKeyboard, enter, escape, backspace

extern GLFWwindow* window;

extern float bloomLevels[8];

extern GLfloat lastX;
extern GLfloat lastY;


extern glm::vec4 BackgroundColor;

extern glm::vec2 MousePosition;
extern glm::vec2 ScreenMousePosition;
extern glm::vec2 scrpos;

extern bool keys[1024];
extern bool JustPressedkey[1024];
extern bool JustReleasedkey[1024];
//Same as keys[], but affected by window activity
extern bool Holdingkey[1024];

extern bool buttons[64];
extern bool JustPressedbutton[64];
extern bool JustReleasedbutton[64];
//Same as buttons[], but affected by window activity
extern bool Holdingbutton[64];

extern int scrollmovement;

extern bool ReleasedLMB;
extern bool HoldingLMB;
extern bool JustPressedLMB;

// saved controls for window stuff
extern bool bJustPressedkey[1024];
extern bool bJustReleasedkey[1024];
extern bool bHoldingkey[1024];
extern bool bJustPressedbutton[64];
extern bool bJustReleasedbutton[64];
extern bool bHoldingbutton[64];
extern int bscrollmovement;


extern bool bReleasedLMB;
extern bool bHoldingLMB;
extern bool bJustPressedLMB;

extern glm::vec2 LastJustPressedLMBScrMousePos;
extern glm::vec2 LastJustPressedMMBScrMousePos;
extern glm::vec2 LastJustPressedRMBScrMousePos;
extern glm::vec2 LastJustPressedLMBMousePos;
extern glm::vec2 LastJustPressedRMBMousePos;
extern glm::vec2 LastJustPressedMMBMousePos;


extern bool SettingsWindow ;
extern int threadcount;

extern bool VSync;
extern int ifpsHardLock;
extern bool bfpsHardLock;

extern int framecount ;
extern float delta;
extern float timer, lt;

extern float SceneExposure;


enum SHAPE { CIRCLE, QUAD, LINE, TRIANGLE };
extern bool DrawCubicColiders ;

//
//extern float ScaleMultiplyer ;
//
//extern float ScreenAspectRatio ;
//
//extern float ScreenDivisorX ;
//extern float ScreenDivisorY ;


extern bool HDR ;
extern bool bloom ;
extern bool Lighting ;

//DrameBufferShowing
extern bool NormalMap ;
extern bool RegularMap ;
extern bool LightingMap;


//Lighting
extern float AmbientLight ;
extern float DirectionalLight ;
//Bloom
extern float bloomIntensity;

extern bool BloomLevelsWindow ;
//Chromatic abberation
extern float ChromaticStrength ;
extern glm::vec2 ChromaticPoint ;
extern glm::vec4 AmbientColor ;

