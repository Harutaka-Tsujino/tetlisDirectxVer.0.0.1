#ifndef TETLIS_DIRECTX_VER_0_0_1_WINMAIN_H
#define TETLIS_DIRECTX_VER_0_0_1_WINMAIN_H

#include <windows.h>

//プロシージャの生成関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

//ウィンドウの設定、メイン関数
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow);

//メモリ開放をする関数
VOID FreeDx(VOID);

#endif
