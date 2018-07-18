﻿/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render
//
// DATE 2018.07.12
// VER.0.0.5
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TETLIS_DEIRECT_VER_0_0_1_RENDER_H
#define TETLIS_DEIRECT_VER_0_0_1_RENDER_H

#include <windows.h>

typedef struct
{
	FLOAT x, y, scale;
}ICON;

typedef struct
{
	FLOAT x, y, xScale, yScale;
}ImageState;

extern ImageState g_tetminoState;
extern ImageState g_gameoverStrState;
extern ImageState g_scopeStrState;
extern ImageState g_undergroundStrState;
extern INT g_scopeShakeCount;

extern ICON arrowIconState, pauseIconState, resultIconState;

typedef struct
{
	FLOAT x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
}CustomVertex;

//タイトル画面の描画関数
VOID titleRender(VOID);

//描画に関する関数
VOID Render(VOID);

//総合的な視点の設定をする関数
VOID SetViewPointOverall(VOID);

//総合的な視野角の設定をする関数
VOID SetFocusOfViewOverall(VOID);

//背景を描画する関数
VOID RenderBackground(VOID);

//ハードドロップが行われた時のエフェクトを描画する関数
VOID SetHardDropEffectTextureAndRender(VOID);

//ラインが消された時のエフェクトを描画する関数
VOID SetDeletedLineEffectTextureAndRender(VOID);

//カウントによりエフェクトのアニメーション用の設定を行う関数
VOID CountToAnimation(INT *additionalDeletedEffectScale, INT *deletedLineEffectCaunt, CustomVertex *cusV4DeletedLineEffect);

//額縁を描画する関数
VOID RnderFrame(VOID);

//アイテムの描画に関する関数
VOID SetItemVerticiesAndRender(VOID);

//ゲームオーバー時の文字列を描画する関数
VOID RenderGameoverStr(VOID);

//スコアの文字列を描画する関数
VOID RenderScoreStr(VOID);

//どのくらい地面を掘り進んだかの文字列を描画する関数
VOID RenderUndergroundStr(VOID);

//テトリスのブロックの４頂点を設定し、描画する関数
VOID SetBlockVerticesAndRender(VOID);

//カスタムバーテックスのtu,tvをデフォルトに戻す関数
VOID ResetCustomVertexTuTv(VOID);

//ホールド、ネクスト、ネクストネクストの４頂点を設定し、描画する関数
VOID SetHoldNextNextNextVerticesAndRender(VOID);

//テトリミノのターゲットの４頂点を設定し、描画する関数
VOID SetTetliminoTargetTextureAndRender(VOID);

//ターゲットブロックのアルファ値を点滅させるために変換する関数
VOID UnderGoChangeTarAlpha(CustomVertex *cusV4TarTetmino);

//度数法によるテクスチャの回転関数
VOID RotateTexDeg(INT axis, CustomVertex *dest, CustomVertex *src, FLOAT degree, FLOAT shiftX, FLOAT shiftY);

VOID pauseRender(VOID);

#endif
