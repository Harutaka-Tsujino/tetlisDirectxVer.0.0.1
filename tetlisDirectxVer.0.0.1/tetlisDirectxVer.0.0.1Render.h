#ifndef TETLIS_DEIRECT_VER_0_0_1_RENDER_H
#define TETLIS_DEIRECT_VER_0_0_1_RENDER_H

#include <windows.h>
#include <d3dx9.h>

typedef struct
{
	FLOAT x, y, xScale, yScale;
}ImageState;

extern ImageState g_tetminoState;
extern ImageState g_GameoverStrState;
extern ImageState g_scoreStrState;

typedef struct
{
	FLOAT x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
}CustomVertex;

//描画に関する関数
VOID Render(VOID);

//総合的な視点の設定をする関数
VOID SetViewPointOverall(VOID);

//総合的な視野角の設定をする関数
VOID SetFocusOfViewOverall(VOID);

//画面を表示させる関数
VOID DisplayScreen(CustomVertex * cusV4Background, CustomVertex *cusV4Tetmino, CustomVertex *cusV4Frame, RECT *rectGameoverStr, RECT *rectScoreStr);

//テトリスのブロックの４頂点を設定し、描画する関数
VOID SetBlockVerticesAndRender(CustomVertex *cusV4Tetmino);

//ホールド、ネクスト、ネクストネクストの４頂点を設定し、描画する関数
VOID SetHoldNextNextNextVerticesAndRender(CustomVertex *cusV4Tetmino);

//テトリミノのターゲットの４頂点を設定し、描画する関数
VOID SetTetliminoTargetTextureAndRender(CustomVertex *cusV4Tetmino);

#endif
