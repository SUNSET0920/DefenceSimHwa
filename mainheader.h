#pragma once

//#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.

//------ �߰� ��� ������ ���⿡�� �����մϴ�.
#include <objidl.h>
#include <gdiplus.h>  // gdiplus  �տ� objidl.h �־������
// �������·� lib ������ ���� ���
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
