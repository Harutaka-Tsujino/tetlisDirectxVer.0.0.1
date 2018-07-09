/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control
//
// DATE 2018.06.13
// VER.0.0.5
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TETLIS_DIRECTX_VER_0_0_1_CONTROL_H
#define TETLIS_DIRECTX_VER_0_0_1_CONTROL_H

#include <windows.h>

extern BOOL g_durableBlockBeared[TETLIS_HEIGHT][TETLIS_WIDTH];
extern BOOL g_reduceBlockDurPosition[TETLIS_HEIGHT][TETLIS_WIDTH];

extern INT g_deletedLine;
extern INT g_deletedLineOverall;

//テトリスなどの操作に関する関数
VOID Control(VOID);

//アイテムの左右移動をする関数
VOID ShiftItemX(INT shift, BOOL *canInputRA, INT rangeMin, INT rangeMax);

////アイテムの上下移動をする関数
VOID ShiftItemY(INT shift, BOOL *canInputRA, INT rangeMin, INT rangeMax);

//アイテムによるブロック破壊の処理をする関数
VOID DeleteBlockWithItem(BOOL *g_useItem);

//テトリス配列に選ばれたパターンをコピーする関数
VOID ChooseAndCpyTetlisBoardSourceToBoard(VOID);

//テトリスボードを初期化する関数
VOID InitTetlisBoard(VOID);

//テトリスボードバッファーを初期化する関数
VOID InitTetlisBoardBuf(VOID);

//ホールド、ネクスト、ネクストネクストを初期化する関数
VOID InitNextAndNextNext(BOOL *isNewGame);

//g_durableBlockBearedを初期化する関数
VOID InitDurableBlockBeared(VOID);

//g_reduceBlockDurPositionを初期化する関数
VOID InitReduceBlockPosition(VOID);

//ホールド、ネクスト、ネクストネクストボードの中身を更新する関数
VOID UpdateHoldNextNextNextBoard(VOID);

//テトリス配列バッファーに可動テトリミノを除いたテトリス配列の要素を同期させる関数
VOID SynchroTetlisBoardBufToTetlisBoard(VOID);

//テトリス配列に可動テトリミノを同期させる関数
VOID SynchroTetlisBoardToMovMinoNumOfArBuf(INT currentTetmino);

//テトリスに関するデータを初期状態に戻す関数
VOID ReturnToInitialStateWithTetlis(BOOL *isGameover, BOOL *canCreate, BOOL *canInputRA, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputR, BOOL *canInputSpace,
	BOOL *canHold, BOOL *wasHold, INT *rACount, INT *lACount, INT *dACount, INT *stopCount, INT *downCount, INT *scoreBuf, INT *currentTetmino, INT *minoIRoatationCount, INT *deletedLineCount, INT *lineCount, INT *additionalDeletableLine);

//テトリミノを生成に関する関数
VOID CreateTetlimino(INT currentTetmino, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputRA, 
	BOOL *canInputR, BOOL *canInputSpace, BOOL *wasHold, INT *stopCount, INT *downCount, INT *minoIRoatationCount, BOOL *canCreate);

//キー入力をカウントをとり可能にする関数
VOID CountToMakeFlagTrue(BOOL *canInputLA, INT *lACount);

//フラグをカウントをとりオフにする関数
VOID CountToMakeFlagFalse(INT *lACount);

//テトリミノをホールドすることに関する関数
VOID HoldTetlimino(BOOL *canHold, INT *currentTetmino, BOOL *canCreate, BOOL *wasHold);

//テトリミノのX軸方向の移動に関する関数
VOID ShiftTetliminoX(INT shiftX, BOOL *canInputRA);

//テトリミノの回転に関する関数
VOID RotateTetlimino(INT *minoIRoatationCount, INT currentTetmino);

//テトリミノのY軸方向の移動に関する関数
VOID DownTetlimino(BOOL *canInputDA);

//テトリミノのハードドロップに関する関数
VOID HardDropTetlimino(VOID);

//テトリミノのターゲットの座標を合わせる関数
VOID SetTetliminoTarget(VOID);

//テトリミノの自動落下に関する関数
VOID CountToDawnTetlimino(INT *downCount);

//テトリミノ停止に関する関数
VOID CountToStopTetlimino(INT *stopCount, INT *currentTetmino,BOOL *canCreate, BOOL *canHold, BOOL *wasHold);

//そろったテトリスのラインを消しカウントをとる関数
VOID DeleteAndCountFilledLine(INT *lineCount, INT *additionalDeletableLine);

//消されたテトリスのライン部分にずらす関数
VOID ShiftTetlisLine(INT *lineCount, INT *additionalDeletableLine);

//耐久値ブロックが関わったブロックずらしをする関数
VOID ShiftTetlisBlockInvolvedInDurableBlock(INT column, INT row);

//消されたテトリスのラインを数える関数
VOID CountDeletedLine(VOID);

//テトリスのスコアに関する関数
VOID GetScoreByLineCount(INT lineCount, INT *scoreBuf);

//ゲームオーバーか、否かを調べる関数
VOID CheckGameover(BOOL *isGameover);

#endif
