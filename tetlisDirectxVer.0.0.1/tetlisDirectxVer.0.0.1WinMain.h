#ifndef TETLIS_DIRECTX_VER_0_0_1_WINMAIN_H
#define TETLIS_DIRECTX_VER_0_0_1_WINMAIN_H

#include <windows.h>

//�v���V�[�W���̐����֐�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

//�E�B���h�E�̐ݒ�A���C���֐�
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow);

//�������J��������֐�
VOID FreeDx(VOID);

#endif
