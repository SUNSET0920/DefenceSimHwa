#pragma once

//#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.

//------ 추가 헤더 파일은 여기에서 참조합니다.
#include <objidl.h>
#include <gdiplus.h>  // gdiplus  앞에 objidl.h 있어야지됨
// 파일형태로 lib 가지고 오는 방법
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
