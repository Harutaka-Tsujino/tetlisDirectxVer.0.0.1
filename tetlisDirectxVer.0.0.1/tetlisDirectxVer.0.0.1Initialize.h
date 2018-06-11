#ifndef TETLIS_DIRECTX_VER_0_0_1_INITIALIZE_H
#define TETLIS_DIRECTX_VER_0_0_1_INITIALIZE_H

#include<windows.h>
#include <d3dx9.h>
#include <dinput.h>

extern LPDIRECT3D9 g_pDirect3D;
extern LPDIRECT3DDEVICE9 g_pD3dDevice;
extern LPDIRECTINPUT8 g_pDinput;
extern LPDIRECT3DTEXTURE9 g_pTexture[g_texMax];
extern LPDIRECTINPUTDEVICE8 g_pDKeyDevice;
extern LPD3DXFONT g_pFont[g_fontMax];
extern D3DPRESENT_PARAMETERS g_D3dPresentParameters;

//ダイレクト3Dに関する初期化の関数
HRESULT InitDirect3D(HWND hWnd);

//総合的なバッファの設定をする関数
VOID SetBuckBufferOverall(VOID);

//総合的な描画の設定をする関数
VOID SetRenderStateOverall(VOID);

//総合的なテクスチャ階層の設定をする関数
VOID SetTextureStageStateOverall(VOID);

//テクスチャを作る関数
VOID CreateTextureFromFile(VOID);

//ダイレクトインプットに関する初期化の関数
HRESULT InitDinput(HWND hWnd);

//ダイレクトフォントに関する初期化の関数
HRESULT InitDfont(HWND hWnd);

//フォントを作る関数
VOID CreateFont(VOID);

#endif
