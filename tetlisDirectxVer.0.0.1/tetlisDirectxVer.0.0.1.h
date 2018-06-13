#ifndef TETLIS_DIRECTX_VER_0_0_1_H
#define TETLIS_DIRECTX_VER_0_0_1_H

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <dinput.h>

#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#define DISPLAY_WIDTH 1600
#define DISPLAY_HEIGHT 900
#define TETLIS_WIDTH 12
#define TETLIS_HEIGHT 140
#define SHIFT_FRAME_COUNTER 6
#define DOWN_FRAME_COUNTER 2
#define FLAME_PER_DOWN 30
#define FLAME_PER_STOP 30
#define GAMEOVER ("GAME OVER")
#define GAMEOVER_COLOR (0xFF000000)
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

enum TEX
{
	g_backgroundTex,
	g_frameTex,
	g_titleLogoTex,
	g_tetminoITex,
	g_tetminoJTex,
	g_tetminoLTex,
	g_tetminoSTex,
	g_tetminoZTex,
	g_tetminoTTex,
	g_tetminoOTex,
	g_tetminoDirtTex,
	g_tetminoStoneTex,
	g_tetminoIronTex,
	g_tetminoTarTex,
	g_integratedBlockTex,
	g_texMax
};

enum FONT
{
	g_gameoverFont,
	g_scoreFont,
	g_fontMax
};

typedef struct
{
	INT number;
	CHAR name;
	INT yx[4][2];
} Tetmino;

extern Tetmino g_tetmino[7];

typedef struct
{
	INT YX[4][2];
}MovMinoNumoOfArBuf;

extern MovMinoNumoOfArBuf g_movMinoNumOfArBuf;
extern MovMinoNumoOfArBuf g_targetMinoNumOfArBuf;

extern INT g_tetminoNum;
extern INT g_deletedLineCount;
extern INT g_hold, g_next, g_nextNext;
extern INT g_tetlisBoardSource[3][TETLIS_WIDTH];
extern INT g_tetlisBoard[TETLIS_HEIGHT][TETLIS_WIDTH];
extern INT g_tetlisBoardBuf[TETLIS_HEIGHT][TETLIS_WIDTH];
extern INT g_holdBoard[4][4];
extern INT g_nextBoard[4][4];
extern INT g_nextNextBoard[4][4];

extern BOOL g_showGameoverStr;

extern CHAR g_scoreArray[8];

#endif
