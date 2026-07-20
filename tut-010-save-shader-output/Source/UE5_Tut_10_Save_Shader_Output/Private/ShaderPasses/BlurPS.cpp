#include "ShaderPasses/BlurPS.h"

IMPLEMENT_GLOBAL_SHADER(FBlurPS, "/Plugin/BlurShader/Private/Blur.usf", "MainPS", SF_Pixel);
