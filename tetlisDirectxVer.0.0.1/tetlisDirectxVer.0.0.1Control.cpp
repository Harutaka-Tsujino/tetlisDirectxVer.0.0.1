/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control
// 
// DATE 2018.07.12
// VER.0.0.5
//
// Author Harutaka-Tsujino
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1Common.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"
#include "tetlisDirectxVer.0.0.1Control.h"
#include "tetlisDirectxVer.0.0.1Render.h"
#include <dinput.h>



BOOL g_durableBlockBeared[TETLIS_HEIGHT][TETLIS_WIDTH];
BOOL g_reduceBlockDurPosition[TETLIS_HEIGHT][TETLIS_WIDTH];

BOOL g_deletedLine = false;
INT g_deletedLineOverall = 0;
INT g_additionalLand = 0;
INT g_swellingUpHolder = 0;
INT g_swellingUpSaver = 0;
//
INT  scene = g_titleScene;

INT inputCount = 0;
BOOL changeFont, deleteFont = false, arrowMoveUp, arrowMoveDown, arrowMoveRight, arrowMoveLeft, pause;

//

////////////////////////////////
//タイトル制御処理関数
void titleControl()
{
	SoundManager& soundManager = SoundManager::GetInstance();
	soundManager.Stop(g_pD3DSound[g_bgmAudio]);
	inputCount++;

	HRESULT hr = g_pDKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		BYTE diks[256];
		g_pDKeyDevice->GetDeviceState(sizeof(diks), &diks);

		if (diks[DIK_RETURN] & 0x80)
		{
			if (inputCount >= 10)
			{
				deleteFont = true;
			}

			if (!changeFont)
			{
				if (inputCount >= 10)
				{
					changeFont = true;
					inputCount = 0;
				}
			}
			else if ((changeFont) && (arrowIconState.y == 425.f))
			{
				if (inputCount >= 10)
				{
					soundManager.Play(g_pD3DSound[g_bgmAudio], true);
					scene = g_gameScene;
					inputCount = 0;
				}
			}
			else if ((changeFont) && (arrowIconState.y == 525.f))
			{
				if (inputCount >= 10)
				{
					PostQuitMessage(0);
				}
			}
		}

		if ((diks[DIK_DOWN] & 0x80) || (diks[DIK_S] & 0x80))
		{
			arrowMoveDown = true;

			if ((arrowMoveDown) && (arrowIconState.y == 525.f))
			{
				if (inputCount >= 10)
				{
					arrowIconState.y = 425.f;
					arrowMoveDown = false;
					inputCount = 0;
				}
			}
			else if (arrowMoveDown)
			{
				if (inputCount >= 10)
				{
					arrowIconState.y = 525.f;
					arrowMoveDown = false;
					inputCount = 0;
				}
			}
		}

		if ((diks[DIK_UP] & 0x80) || (diks[DIK_W] & 0x80))
		{
			arrowMoveUp = true;

			if ((arrowMoveUp) && (arrowIconState.y == 425.f))
			{
				if (inputCount >= 10)
				{
					arrowIconState.y = 525.f;
					arrowMoveUp = false;
					inputCount = 0;
				}
			}
			else if (arrowMoveUp)
			{
				if (inputCount >= 10)
				{
					arrowIconState.y = 425.f;
					arrowMoveUp = false;
					inputCount = 0;
				}
			}
		}

		if (diks[DIK_ESCAPE] & 0x80)
		{
			PostQuitMessage(0);
		}
	}
}

////////////////////////////////
//テトリスなどの操作に関する関数
VOID Control(VOID)
{
	SoundManager& soundManager = SoundManager::GetInstance();
	inputCount++;
	//キーデバイスに入っている情報を読み取る準備
	HRESULT hr = g_pDKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		//キー入力情報のバッファー
		BYTE diks[256];
		static BYTE prevDiks[256];

		//入力された情報を読み取る
		g_pDKeyDevice->GetDeviceState(sizeof(diks), &diks);

		static BOOL canInputLA = true, canInputDA = true, canInputRA = true, canInputUA = true, canInputR = true, canInputSpace = true,canInputQ = true,canInputE = true, isGameover = false, isNewGame = true, canHold = true, wasHold = false, canCreate = true;

		//生成されるのテトリミノ種類を決める
		static INT rACount = 0, lACount = 0,uACount = 0, dACount = 0,qCount = 0,eCount = 0, stopCount = 0, downCount = 0, scoreBuf = 0, minoIRoatationCount = 0,prevUpKeyState = 0,prevCKeyState = 0, prevRKeyState = 0, prevSpaceKeyState = 0, currentTetmino = rand() % 7, deletedLineCount = 0;
		
		static INT lineCount = 0, additionalDeletableLine = 0, prevDeletedLineCount = 0, additionalLandCount = 0;
		
		INT A[2] = { 0,diks[DIK_A] & 0x80 }, W[2] = { 0,diks[DIK_W] & 0x80 },S[2] = { 0,diks[DIK_S] & 0x80 }, D[2] = { 0,diks[DIK_D] & 0x80 }, Q[2] = { 0,diks[DIK_Q] & 0x80 }, E[2] = { 0,diks[DIK_E] & 0x80 };

		//テトリス配列にテトリス配列バッファーの要素全てをコピーしている
		memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
		
		if (!isGameover)
		{
			if (diks[DIK_ESCAPE] & 0x80)
			{
				if ((inputCount >= 10) && (pause))
				{
					pause = false;
					inputCount = 0;
				}
				else if (inputCount >= 10)
				{
					pause = true;
					inputCount = 0;
				}
			}
		}

		if (pause)
		{
			if (diks[DIK_RETURN] & 0x80)
			{
				if (pauseIconState.y == 305.f)
				{
					if (inputCount >= 10)
					{
						pause = false;
					}
				}
				else if (pauseIconState.y == 385.f)
				{
					///////////////////////////////////////////////////////////////////////////
					//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
					ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
						&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
					pauseIconState.y = 305.f;
					pause = false;
					additionalLandCount = 0;
				}
				else if (pauseIconState.y == 465.f)
				{
					///////////////////////////////////////////////////////////////////////////
					//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
					ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
						&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
					pauseIconState.y = 305.f;
					pause = false;
					deleteFont = false;
					changeFont = false;
					inputCount = 0;
					additionalLandCount = 0;
					scene = g_titleScene;
				}
			}

			if ((diks[DIK_UP] & 0x80) || (diks[DIK_W] & 0x80))
			{
				arrowMoveUp = true;

				if ((arrowMoveUp) && (pauseIconState.y == 385.f))
				{
					if (inputCount >= 10)
					{
						pauseIconState.y = 305.f;
						inputCount = 0;
					}
				}
				else if ((arrowMoveUp) && (pauseIconState.y == 465.f))
				{
					if (inputCount >= 10)
					{
						pauseIconState.y = 385.f;
						inputCount = 0;
					}
				}
				else if (arrowMoveUp)
				{
					if (inputCount >= 10)
					{
						pauseIconState.y = 465.f;
						inputCount = 0;
					}
				}
			}

			if ((diks[DIK_DOWN] & 0x80) || (diks[DIK_S] & 0x80))
			{
				arrowMoveDown = true;

				if ((arrowMoveDown) && (pauseIconState.y == 305.f))
				{
					if (inputCount >= 10)
					{
						pauseIconState.y = 385.f;
						inputCount = 0;
					}
				}
				else if ((arrowMoveDown) && (pauseIconState.y == 385.f))
				{
					if (inputCount >= 10)
					{
						pauseIconState.y = 465.f;
						inputCount = 0;
					}
				}
				else if (arrowMoveDown)
				{
					if (inputCount >= 10)
					{
						pauseIconState.y = 305.f;
						inputCount = 0;
					}
				}
			}
		}
		
		if (diks[DIK_J] & 0x80)
		{
			for (INT item = 0; item < g_itemMax; item++)
			{
				g_itemData.haveItem[item] = true;
			}
		}
		static BOOL playMusic = true;

		if (pause)
		{
			//リセットボタン、初期状態に戻す
			if (diks[DIK_BACK] & 0x80)
			{
				if (pauseIconState.y == 305.f)
				{
					if (inputCount >= 10)
					{
						pause = false;
					}
				}
				else if (pauseIconState.y == 385.f)
				{
					///////////////////////////////////////////////////////////////////////////
					//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
					ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
						&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
					soundManager.Play(g_pD3DSound[g_bgmAudio], true);
					additionalLandCount = 0;
					playMusic = true;
				}
				else if (pauseIconState.y == 465.f)
				{
					///////////////////////////////////////////////////////////////////////////
					//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
					ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
						&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
					pauseIconState.y = 305.f;
					additionalLandCount = 0;
					pause = false;
					deleteFont = false;
					changeFont = false;
					inputCount = 0;
					scene = g_titleScene;
				}
			}
		}

		if (isGameover)
		{
			if (playMusic)
			{
				soundManager.Stop(g_pD3DSound[g_bgmAudio]);
				soundManager.Play(g_pD3DSound[g_gameoverAudio], false);
				playMusic = false;
			}
			//ゲームオーバー時に文字列を表示させる
			g_showGameoverStr = true;

		}

		if (isGameover)
		{

			////リセットボタン、初期状態に戻す
			//if (diks[DIK_BACK] & 0x80)
			//{
			//	///////////////////////////////////////////////////////////////////////////
			//	//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
			//	ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
			//		&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &prevDeletedLineCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
			//}

			if (isGameover)
			{
				//ゲームオーバー時に文字列を表示させる
				g_showGameoverStr = true;

				if ((diks[DIK_RIGHT] & 0x80) || (diks[DIK_D] & 0x80))
				{
					arrowMoveRight = true;

					if ((arrowMoveRight) & (resultIconState.x == 760.f))
					{
						if (inputCount >= 10)
						{
							resultIconState.x = 300.f;
							inputCount = 0;
						}
					}
					if (arrowMoveRight)
					{
						if (inputCount >= 10)
						{
							resultIconState.x = 760.f;
							inputCount = 0;
						}
					}
				}

				if ((diks[DIK_LEFT] & 0x80) || (diks[DIK_A] & 0x80))
				{
					arrowMoveLeft = true;

					if ((arrowMoveLeft) & (resultIconState.x == 300.f))
					{
						if (inputCount >= 10)
						{
							resultIconState.x = 760.f;
							inputCount = 0;
						}
					}
					else if (arrowMoveLeft)
					{
						if (inputCount >= 10)
						{
							resultIconState.x = 300.f;
							inputCount = 0;
						}
					}
				}

				if (diks[DIK_RETURN] & 0x80)
				{
					if (resultIconState.x == 300.f)
					{
						if (inputCount >= 10)
						{
							///////////////////////////////////////////////////////////////////////////
							//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
							ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
								&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
							inputCount = 0;
							additionalLandCount = 0;
							soundManager.Play(g_pD3DSound[g_bgmAudio], true);
						}
					}
					else if (resultIconState.x == 760.f)
					{
						if (inputCount >= 10)
						{
							///////////////////////////////////////////////////////////////////////////
							//フラグ、カウント、配列を初期状態に戻しUpdateHoldNextNextNextBoardを用いる
							ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
								&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
							resultIconState.x = 300.f;
							additionalLandCount = 0;
							isGameover = false;
							deleteFont = false;
							changeFont = false;
							scene = g_titleScene;
							inputCount = 0;
						}
					}
				}
			}
		}

		else
		{
			if (!pause)
			{
				if (isNewGame)
				{
					soundManager.Play(g_pD3DSound[g_bgmAudio], true);

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//ニューゲーム(一番最初のフレーム)時にはg_nextとg_nextNextは初期化されていないので初期化し、isNewGameをfalseにする
					InitNextAndNextNext(&isNewGame);
					InitTetlisBoard();
					ChooseAndCpyTetlisBoardSourceToBoard();
					memcpy(g_tetlisBoardBuf, g_tetlisBoard, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
					memset(prevDiks, 0, sizeof(BYTE) * 256);
					isNewGame = false;
				}
			
				if (!(g_deletedLine))
				{
					for (g_tetminoNum = 0; g_tetminoNum < 7; g_tetminoNum++)
					{
						if (g_tetmino[g_tetminoNum].number == currentTetmino)
						{
							//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//g_canCreateを確認しtrueならばg_movMinoNumOfArBufにcurrentTetminoと同じ種類のテトリミノ初期座標を代入し、キー入力のフラグ、
							//I型のテトリミノが何回転したかを確認するカウント、ホールドが行われたか否かのフラグを初期状態に戻し、g_canCreateをfalseにする
							CreateTetlimino(currentTetmino, &canInputLA, &canInputDA, &canInputRA,
								&canInputR, &canInputSpace, &wasHold, &stopCount, &downCount, &minoIRoatationCount, &canCreate);

							break;
						}
					}

					//////////////////////////////////////////////////////////////////////////////////////////////
					//フラグがfalseならばカウントを1増やし、一定値ならばフラグはtrueに変えカウントを初期状態にする
					CountToMakeFlagTrue(&canInputLA, &lACount);
					CountToMakeFlagTrue(&canInputRA, &rACount);
					CountToMakeFlagTrue(&canInputDA, &dACount);
					CountToMakeFlagTrue(&canInputUA, &uACount);
					CountToMakeFlagTrue(&canInputQ, &qCount);
					CountToMakeFlagTrue(&canInputE, &eCount);

					if (g_itemData.useItem == false)
					{
						if (diks[DIK_V] & 0x80)
						{
							if (Q[canInputQ])
							{
								if (0 < g_itemData.currentItemNum)
								{
									g_itemData.currentItemNum -= 1;
								}

								canInputQ = false;
							}

							if (E[canInputE])
							{
								if (g_itemData.currentItemNum < g_itemMax - 1)
								{
									g_itemData.currentItemNum += 1;
								}

								canInputE = false;
							}
						}
					}

					if (!(prevCKeyState))
					{
						if (diks[DIK_C] & 0x80)
						{
							if (g_itemData.haveItem[g_itemData.currentItemNum])
							{
								if (!(g_itemData.decideItemPos))
								{
									memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);

									if (g_itemData.useItem == false)
									{
										g_itemData.useItem = true;

										switch (g_itemData.currentItemNum)
										{
										case g_ultraDrillItem:

											g_itemData.posYX[0] = 4 + g_deletedLineCount;
											g_itemData.posYX[1] = 5;

											break;
										case g_laserCannonItem:

											g_itemData.posYX[0] = 4 + g_deletedLineCount + 1;
											g_itemData.posYX[1] = 1;

											break;
										case 2:
										case 3:
											break;
										}

										g_itemData.posYX[0] = 4 + g_deletedLineCount;
										g_itemData.posYX[1] = 5;
									}

									else
									{
										g_itemData.useItem = false;
										g_itemData.posYX[0] = 0;
										g_itemData.posYX[1] = 0;
									}
								}
							}
						}
					}

					if (g_itemData.useItem)
					{
						switch (g_itemData.currentItemNum)
						{
						case g_ultraDrillItem:

							if (g_itemData.decideItemPos)
							{
								if (g_itemData.count[g_ultraDrillItem] == 420)
								{
									for (INT column = 4 + g_deletedLineCount + g_additionalLand; column < 4 + g_deletedLineCount + g_additionalLand + 20; column++)
									{
										for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
										{
											if (g_tetlisBoardBuf[column][row] != -1)
											{
												scoreBuf += 30;
											}

											g_tetlisBoard[column][row] = -1;
										}
									}

									SynchroTetlisBoardBufToTetlisBoard();
									memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
									g_itemData.swellingUpCount[g_ultraDrillItem] = 5;
								}

								if (g_itemData.count[g_ultraDrillItem] == 480)
								{

									ShiftTetlisLine(&lineCount, &additionalDeletableLine);

									memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
									SynchroTetlisBoardBufToTetlisBoard();

									CountDeletedLine();
									g_deletedLineOverall += 5 + g_additionalLand;
									g_itemData.haveItem[g_ultraDrillItem] = false;
									g_additionalLand = 0;
									g_itemData.useItem = false;
									g_itemData.decideItemPos = false;
								}

								g_itemData.count[g_ultraDrillItem]++;
							}

							break;
						case g_laserCannonItem:
						{
							INT deletedAdditionalLine = 0;///////////////////////////////
							static INT deleteLandNum = 0;
							static INT swellingUpCount = 0;
							if (g_itemData.decideItemPos)
							{
								if (g_itemData.count[g_laserCannonItem] == 30)
								{
									for (INT column = g_itemData.posYX[0] + g_additionalLand - 1; column <= g_itemData.posYX[0] + g_additionalLand + 1; column++)
									{
										INT numOfDirt = 0;

										for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
										{
											if (g_tetlisBoardBuf[column][row] == 110)
											{
												numOfDirt++;
											}

											g_tetlisBoard[column][row] = -1;

											SynchroTetlisBoardBufToTetlisBoard();
										}

										if (numOfDirt > 3)
										{
											swellingUpCount++;
										}

										deleteLandNum = swellingUpCount;
									}

									deletedAdditionalLine = g_additionalLand - swellingUpCount;

									g_itemData.swellingUpCount[g_laserCannonItem] -= deletedAdditionalLine;

									scoreBuf += 200;
								}

								if (g_itemData.count[g_laserCannonItem] == 111)
								{
									ShiftTetlisLine(&lineCount, &additionalDeletableLine);

									SynchroTetlisBoardBufToTetlisBoard();

									CountDeletedLine();

									if (deletedAdditionalLine < 0)
									{
										g_additionalLand = 0;
									}

									else
									{
										g_additionalLand = deletedAdditionalLine;
									}

									g_deletedLineOverall += deleteLandNum;


									swellingUpCount = 0;
									deleteLandNum = 0;
									deletedAdditionalLine = 0;

									g_itemData.useItem = false;
									g_itemData.haveItem[g_laserCannonItem] = false;
									g_itemData.decideItemPos = false;
									g_itemData.posYX[0] = 0;
									g_itemData.posYX[1] = 0;
								}

								g_itemData.count[g_laserCannonItem]++;
							}

							else
							{
								if (S[canInputDA])
								{
									ShiftItemY(1, &canInputDA, 3 + g_deletedLineCount, g_deletedLineCount + 23);
								}

								if (W[canInputUA])
								{
									ShiftItemY(-1, &canInputUA, 4 + g_deletedLineCount, g_deletedLineCount + 24);
								}
							}
						}
						break;
						case g_bulletItem:
							if (g_itemData.decideItemPos)
							{
								switch (g_tetlisBoard[g_itemData.posYX[0] + g_additionalLand][g_itemData.posYX[1]])
								{
								case 140:
									g_itemData.haveItem[g_laserCannonItem] = true;

									break;
								case 160:
									g_itemData.haveItem[g_ultraDrillItem] = true;

									break;
								case 150:
									g_itemData.haveItem[g_bulletItem] = true;

									break;
								case 170:
									g_itemData.haveItem[g_excaliberItem] = true;

									break;
								}

								g_tetlisBoard[g_itemData.posYX[0] + g_additionalLand][g_itemData.posYX[1]] = 151;

								g_itemData.count[g_bulletItem]++;

								SynchroTetlisBoardBufToTetlisBoard();

								if (g_itemData.count[g_bulletItem] == 170)
								{
									/*if ((g_scopeShakeCount/16) == 1 && (g_scopeShakeCount/16) == 5)
									{
										scoreBuf += 30;
									}*/

									g_itemData.useItem = false;
									g_itemData.haveItem[g_bulletItem] = false;
									g_itemData.decideItemPos = false;
									g_itemData.posYX[0] = 0;
									g_itemData.posYX[1] = 0;
									g_itemData.count[g_bulletItem] = 0;
									soundManager.Play(g_pD3DSound[g_bgmAudio], true);

								}
							}

							else
							{

								if (S[canInputDA])
								{
									ShiftItemY(1, &canInputDA, 3 + g_deletedLineCount, g_deletedLineCount + 23);
								}

								if (W[canInputUA])
								{
									ShiftItemY(-1, &canInputUA, 4 + g_deletedLineCount, g_deletedLineCount + 24);
								}

								if (D[canInputRA])
								{
									ShiftItemX(1, &canInputRA, 0, TETLIS_WIDTH - 2);
								}

								if (A[canInputLA])
								{
									ShiftItemX(-1, &canInputLA, 1, TETLIS_WIDTH - 1);
								}
							}

							break;
						case g_excaliberItem:

							if (g_itemData.decideItemPos)
							{
								static INT deleteLandNum = 0;
								static INT swellingUpCount = 0;

								INT deletedAdditionalLine = 0;

								if (g_itemData.count[g_excaliberItem] == 398)
								{
									for (INT column = 4 + g_deletedLineCount + g_additionalLand; column < 4 + g_deletedLineCount + g_additionalLand + 20; column++)
									{
										INT numOfDirt = 0;

										BOOL isOver3Dirts = 0;

										for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
										{
											if (g_tetlisBoardBuf[column][row] == 110)
											{
												numOfDirt++;

												if (numOfDirt > 3)
												{
													isOver3Dirts = true;
												}
											}
										}

										for (INT row = g_itemData.posYX[1] - 1; row < g_itemData.posYX[1] + 2; row++)
										{
											/*if ((column == (4 + g_deletedLineCount + g_additionalLand + 19)) && (row == (g_itemData.posYX[1] y 1)))
											{
												break;
											}

											if ((column ==( 4 + g_deletedLineCount + g_additionalLand + 19)) && (row == (g_itemData.posYX[1] + 1)))
											{
												break;
											}*/

											if (g_tetlisBoardBuf[column][row] == 110)
											{
												numOfDirt--;
											}

											if (g_tetlisBoardBuf[column][row] != 9)
											{
												g_tetlisBoard[column][row] = -1;
											}

											SynchroTetlisBoardBufToTetlisBoard();

											if (isOver3Dirts)
											{
												if (numOfDirt < 4)
												{
													isOver3Dirts = false;
													swellingUpCount++;
												}
											}

											deleteLandNum = swellingUpCount;
										}
									}

									deletedAdditionalLine = g_additionalLand - swellingUpCount;

									g_itemData.swellingUpCount[g_excaliberItem] -= deletedAdditionalLine;
								}

								if (g_itemData.count[g_excaliberItem] == 398 + 30 + 30)
								{
									ShiftTetlisLine(&lineCount, &additionalDeletableLine);

									SynchroTetlisBoardBufToTetlisBoard();

									CountDeletedLine();

									if (deletedAdditionalLine < 0)
									{
										g_additionalLand = 0;
									}

									else
									{
										g_additionalLand = deletedAdditionalLine;
									}

									deletedAdditionalLine = 0;

									g_deletedLineOverall += deleteLandNum;

									deleteLandNum = 0;
									swellingUpCount = 0;

									scoreBuf += 150;
									g_itemData.useItem = false;
									g_itemData.haveItem[g_excaliberItem] = false;
									g_itemData.decideItemPos = false;
									g_itemData.posYX[0] = 0;
									g_itemData.posYX[1] = 0;
								}

								g_itemData.count[g_excaliberItem]++;
							}

							else
							{
								if (D[canInputRA])
								{
									ShiftItemX(1, &canInputRA, 0, TETLIS_WIDTH - 2);
								}

								if (A[canInputLA])
								{
									ShiftItemX(-1, &canInputLA, 1, TETLIS_WIDTH - 1);
								}
							}

							break;
						}

						if (g_itemData.count[g_excaliberItem] == 399 + 30 + 30)
						{
							g_itemData.count[g_excaliberItem] = 0;
							soundManager.Play(g_pD3DSound[g_bgmAudio], true);
						}

						if (g_itemData.count[g_laserCannonItem] == 112)
						{
							g_itemData.count[g_laserCannonItem] = 0;
							soundManager.Play(g_pD3DSound[g_bgmAudio], true);
						}

						if (g_itemData.count[g_ultraDrillItem] == 481)
						{
							g_itemData.count[g_ultraDrillItem] = 0;
							soundManager.Play(g_pD3DSound[g_bgmAudio], true);
						}

						if (!(g_itemData.decideItemPos))
						{
							if (!(prevDiks[DIK_RETURN]))
							{
								if (diks[DIK_RETURN] & 0x80)
								{
									g_itemData.decideItemPos = true;
								}
							}
						}
					}

					else
					{
						AddLand(&scoreBuf, &additionalLandCount);

						//前フレーム時にスペースキーが押されていた場合この処理を通さない、とすることによって連続したフレーム毎にこの処理がされるのを防いでいる
						if (!(prevSpaceKeyState))
						{
							if (diks[DIK_SPACE] & 0x80)
							{
								memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
								SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
								SynchroTetlisBoardBufToTetlisBoard();

								/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//canHoldがtrueならばg_holdに現在のテトリミノの値を代入し、すでに-1(空欄)以外が入っていた場合currentTetminoにg_holdの値を入れる、
								//その後g_canCreateをtrueにしUpdateHoldNextNextNextBoardを呼び出す
								HoldTetlimino(&canHold, &currentTetmino, &canCreate, &wasHold);
							}
						}

						//ホールドが行われていた場合新しくテトリミノを生成するので、この処理を通さない
						if (!(wasHold))
						{
							if (D[canInputRA])
							{
								memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
								SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
								SynchroTetlisBoardBufToTetlisBoard();

								//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//g_movMinoNumOfArBufに引数を足した配列番号をg_tetlisBoardBufで参照し、空欄だった場合g_movMinoNumOfArBufに引数を足した値を代入する
								ShiftTetliminoX(1, &canInputRA);
							}

							if (A[canInputLA])
							{
								memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
								SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
								SynchroTetlisBoardBufToTetlisBoard();

								ShiftTetliminoX(-1, &canInputLA);
							}

							memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
							SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
							SynchroTetlisBoardBufToTetlisBoard();

							//前フレーム時にRが押されていたならば、この処理を通さない、とすることによって連続で同じ処理がされることを防いでいる
							if (!(prevRKeyState))
							{
								if (diks[DIK_R] & 0x80)
								{
									memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
									SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
									SynchroTetlisBoardBufToTetlisBoard();

									///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//g_minoIRoatationCountが３を超えていた場合0代入する、I型のテトリミノの場合g_minoIRoatationCountの値によって軸をずらす、
									//X2、Y2を中心とするためにX2、Y2でそれぞれ4ブロックを引き中心を0,0に移動させる回転させる処理を行う前に
									//g_tetlisBoardBufを参照して回転ができる場合に処理を行う、回転できない場合I型のテトリミノをずらした分だけ元に戻す
									RotateTetlimino(&minoIRoatationCount, currentTetmino);
								}
							}

							if (S[canInputDA])
							{
								memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
								SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
								SynchroTetlisBoardBufToTetlisBoard();

								/////////////////////////////////////////////////////////////////////////////////////////////////
								//g_movMinoNumOfArBufのY方向に１足した配列を用いてg_tetlisBoardBufを参照し、空欄の場合移動を行う
								DownTetlimino(&canInputDA);
							}

							//ハードドロップ　テトリミノのブロックを一個づつ下方を確認し、そこが空欄ではない場合、そこから1つ上にワープさせる
							if (!(prevUpKeyState))
							{
								if (diks[DIK_W] & 0x80)
								{
									memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
									SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
									SynchroTetlisBoardBufToTetlisBoard();

									///////////////////////////////////////////////////////////////////////////////////
									//ループカウンタを用いg_tetlisBoardBufを参照し、空欄ではなかった場合g_movMinoNumOfArBufを
									//ループカウンタ-1の場所にテトリミノを移動させる
									HardDropTetlimino();
								}
							}

							memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
							SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
							SynchroTetlisBoardBufToTetlisBoard();

							//テトリミノの下方を確認し、空欄だった場合カウントをとり、FLAME_PER_DOWNに達した場合一つ下に移動させる
							/////////////////////////////////////////////////////////////////////////////////////////////////////
							//g_movMinoNumOfArBuf+1した配列番号を用いてg_tetlisBoardBufを参照し、空欄だった場合カウントを1増やし、
							//一定値ならばg_movMinoNumOfArBufに１を足し、カウントを初期状態にする
							CountToDawnTetlimino(&downCount);

							memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
							SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
							SynchroTetlisBoardBufToTetlisBoard();

							/*テトリミノ下方を確認し、そこが空欄ではない場合カウントをとり、FLAME_PER_STOPに達した場合
							可動テトリミノのナンバーに100を足し(簡単に可動か非可動を判別するため)、新しい可動テトリミノを生成する*/
							/////////////////////////////////////////////////////////////////////////////////////////////////
							//g_movMinoNumOfArBuf+1をした配列番号を用いg_tetlisBoardBufを参照し、空欄(-1)以外が入っていた場合
							//カウントを１増やし、一定値ならばcurrentTetmino + 10をg_tetlisBoardに代入し、
							//テトリミノを生成できるか否か、ホールドができるか否かのフラグをtrue、
							//ホールドが行われたかのフラグをfalseにし、currentTetminoにg_next、g_nextにg_nextNextを代入して、
							//g_nextNextにテトリミノ番号の範囲の乱数を代入する
							CountToStopTetlimino(&stopCount, &currentTetmino, &canCreate, &canHold, &wasHold);
						}
					}
				}

				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
				SynchroTetlisBoardBufToTetlisBoard();

				if (!(g_itemData.useItem))
				{
					if (deletedLineCount == 0)
					{
						INT diggingCount = 0;

						for (INT column = 19; column < TETLIS_HEIGHT - 1; column++)
						{
							INT nothingInRowCount = 0;

							for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
							{
								if (g_tetlisBoardBuf[column][row] == -1)
								{
									nothingInRowCount++;
								}

								if (nothingInRowCount == 10)
								{
									diggingCount++;
								}
							}
						}

						if (diggingCount > 60)
						{
							INT swapTetlisBoardBuf[60][TETLIS_WIDTH];

							for (INT column = 0; column < 60; column++)
							{
								for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
								{
									swapTetlisBoardBuf[column][row] = g_tetlisBoard[column + 19 + 60][row];
								}
							}

							ChooseAndCpyTetlisBoardSourceToBoard();

							for (INT column = 0; column < 60; column++)
							{
								for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
								{
									g_tetlisBoard[column + 19][row] = swapTetlisBoardBuf[column][row];
								}
							}

							SynchroTetlisBoardBufToTetlisBoard();

							INT shiftUpY[4];

							for (INT block = 0; block < 4; block++)
							{
								shiftUpY[block] = g_movMinoNumOfArBuf.YX[block][0] - (g_deletedLineCount + 4);
							}

							/*for (INT block = 0; block < 4; block++)
							{
								if (shiftUpY[block] < -3)
								{
									shiftUpY[block] = -3;
								}
							}*/

							CountDeletedLine();

							for (INT block = 0; block < 4; block++)
							{
								g_movMinoNumOfArBuf.YX[block][0] = g_deletedLineCount + 4 + shiftUpY[block];
							}
						}

						//そろっている列があるか確認しカウントをとる
						//////////////////////////////////////////////////////////////////////////////
						//g_tetlisBoardBuf中身を確認し一列が空欄(-1)以外の場合空欄にする
						DeleteAndCountFilledLine(&lineCount, &additionalDeletableLine);

						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						/////////////////////////////////////////////////////////////////////////////////
						//lineCountの値によってscoreBufの増やす値を変え、文字列にしg_scoreArrayに代入する
						GetScoreByLineCount(lineCount, &scoreBuf);

						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();
					}

					///////////////////////////////////////////////////////
					//フラグがtrueならば、カウントをとりフラグをfalseにする
					CountToMakeFlagFalse(&deletedLineCount);

					if (deletedLineCount == 60)
					{
						g_swellingUpSaver = 0;
						g_swellingUpHolder = 0;

						//そろった列を空欄にして下にずらしている
						/////////////////////////////////////////////////////////////////////////////////////////////
						//g_tetlisBoardBufを参照し、空欄(-1)以外の場合ループカウンタ+1した配列番号を用い、再度参照し、
						//一列全て空欄の場合ループカウンタ+1の配列番号にコピーし、コピー元を空欄に書き換える
						ShiftTetlisLine(&lineCount, &additionalDeletableLine);

						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						//////////////////////////////////////////////////
						//いくつ初期状態からラインを消されたか確認する関数
						CountDeletedLine();
						g_deletedLineOverall += g_deletedLineCount - prevDeletedLineCount;
					}

					if (deletedLineCount == 0)
					{
						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						////////////////////////////////////////
						//ハードドロップと同じ原理を利用している
						SetTetliminoTarget();

						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						////////////////////////////////////////////////////////////////////////////////////////
						//プレイ時の見える範囲内で一番上の部分に非可動テトリミノがある場合isGameoverをtrueにする
						CheckGameover(&isGameover);
					}
				}

				//キー入力状態を保存
				prevRKeyState = diks[DIK_R] & 0x80;
				prevSpaceKeyState = diks[DIK_SPACE] & 0x80;
				prevUpKeyState = diks[DIK_W] & 0x80;
				prevCKeyState = diks[DIK_C] & 0x80;
				memcpy(prevDiks, diks, sizeof(BYTE) * 256);

				//ホールド時にブロックが表示されているので消す
				if (wasHold || g_itemData.useItem)
				{
					memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				}
			}
		}

		prevDeletedLineCount = g_deletedLineCount;
	}

	return;
}

///////////////////////////////////
//アイテムのX方向のポジションを変更
VOID ShiftItemX(INT shift, BOOL *canInputRA, INT rangeMin, INT rangeMax)
{
	if (rangeMin < g_itemData.posYX[1] && g_itemData.posYX[1] < rangeMax)
	{
		g_itemData.posYX[1] += shift;
		*canInputRA = false;
	}

	return;
}

///////////////////////////////////
//アイテムのY方向のポジションを変更
VOID ShiftItemY(INT shift, BOOL *canInputRA, INT rangeMin, INT rangeMax)
{
	if (rangeMin < g_itemData.posYX[0] && g_itemData.posYX[0] < rangeMax)
	{
		g_itemData.posYX[0] += shift;
		*canInputRA = false;
	}

	return;
}

VOID DeleteBlockWithItem(BOOL *g_useItem)
{
	for (INT column = 4 + g_deletedLineCount; column < 4 + g_deletedLineCount + 20; column++)
	{
		for (INT row = 1; row < TETLIS_WIDTH-1; row++)
		{
			g_tetlisBoard[column][row] = -1;
		}
	}

	SynchroTetlisBoardBufToTetlisBoard();

	return;
}

////////////////////////////////////////////
//テトリス配列に選ばれたパターンをコピーする
VOID ChooseAndCpyTetlisBoardSourceToBoard(VOID)
{
	for (INT line = 19; line < 137; line += 3)
	{
		INT ran = 3 * (rand() % 4);

		for (INT column = ran; column < (ran+3); column++)
		{
			for (INT row = 0; row < 12; row++)
			{
				g_tetlisBoard[line + column-ran][row] = g_tetlisBoardSource[column][row];
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//－1は空欄を表し、先に全ての配列の中身を空欄にすることにより、壁(9)を上から塗りつぶすだけで表が完成する
VOID InitTetlisBoard(VOID)
{
	for (INT column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (INT row = 0; row < TETLIS_WIDTH; row++)
		{
			
			g_tetlisBoard[column][row] = -1;
			if (row == TETLIS_WIDTH - 12 || row == TETLIS_WIDTH - 1)
			{
				g_tetlisBoard[column][row] = 9;
			}
			if (column == TETLIS_HEIGHT - 1)
			{
				g_tetlisBoard[column][row] = 9;
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//可動テトリミノ(動いてるやつ)をテトリスボードバッファーに入れないようにすることにより、回転、移動などの際、可動テトリミノを考慮せずとも良い
VOID InitTetlisBoardBuf(VOID)
{
	
	for (INT column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (INT row = 0; row < TETLIS_WIDTH; row++)
		{
			g_tetlisBoardBuf[column][row] = -1;

			if (row == TETLIS_WIDTH - 12 || row == TETLIS_WIDTH - 1)
			{
				g_tetlisBoardBuf[column][row] = 9;
			}

			if (column == TETLIS_HEIGHT - 1)
			{
				g_tetlisBoardBuf[column][row] = 9;
			}
		}
	}

	return;
}

//////////////////////////////////
//g_durableBlockBearedを初期化する
VOID InitDurableBlockBeared(VOID)
{
	for (INT column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (INT row = 0; row < TETLIS_WIDTH; row++)
		{
			g_durableBlockBeared[column][row] = false;
		}
	}

	return;
}

///////////////////////////////////
//g_reduceBlockDurPositionを初期化する
VOID InitReduceBlockPosition(VOID)
{
	for (INT column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (INT row = 0; row < TETLIS_WIDTH; row++)
		{
			g_reduceBlockDurPosition[column][row] = false;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//テトリミノの初期座標(生成されるときの座標)を用いるため、x座標を-4ずらすことでネクスト等の表との整合性をとり、
//ホールドに入っている値によって、入れるテトリミノを変える
VOID UpdateHoldNextNextNextBoard(VOID)
{
	for (INT column = 0; column < 4; column++)
	{
		for (INT row = 0; row < 4; row++)
		{
			g_nextNextBoard[column][row] = -1;
			g_holdBoard[column][row] = -1;
			g_nextBoard[column][row] = -1;

			if (0 <= g_hold && g_hold <= 6)
			{
				if (((column == g_tetmino[g_hold].yx[0][0]) && (row == g_tetmino[g_hold].yx[0][1] - 4)) ||
					((column == g_tetmino[g_hold].yx[1][0]) && (row == g_tetmino[g_hold].yx[1][1] - 4)) ||
					((column == g_tetmino[g_hold].yx[2][0]) && (row == g_tetmino[g_hold].yx[2][1] - 4)) ||
					((column == g_tetmino[g_hold].yx[3][0]) && (row == g_tetmino[g_hold].yx[3][1] - 4)))
				{
					g_holdBoard[column][row] = g_hold;
				}
			}

			if (0 <= g_next && g_next <= 6)
			{
				if (((column == g_tetmino[g_next].yx[0][0]) && (row == g_tetmino[g_next].yx[0][1] - 4)) ||
					((column == g_tetmino[g_next].yx[1][0]) && (row == g_tetmino[g_next].yx[1][1] - 4)) ||
					((column == g_tetmino[g_next].yx[2][0]) && (row == g_tetmino[g_next].yx[2][1] - 4)) ||
					((column == g_tetmino[g_next].yx[3][0]) && (row == g_tetmino[g_next].yx[3][1] - 4)))
				{
					g_nextBoard[column][row] = g_next;
				}
			}

			if (0 <= g_nextNext && g_nextNext <= 6)
			{
				if (((column == g_tetmino[g_nextNext].yx[0][0]) && (row == g_tetmino[g_nextNext].yx[0][1] - 4)) ||
					((column == g_tetmino[g_nextNext].yx[1][0]) && (row == g_tetmino[g_nextNext].yx[1][1] - 4)) ||
					((column == g_tetmino[g_nextNext].yx[2][0]) && (row == g_tetmino[g_nextNext].yx[2][1] - 4)) ||
					((column == g_tetmino[g_nextNext].yx[3][0]) && (row == g_tetmino[g_nextNext].yx[3][1] - 4)))
				{
					g_nextNextBoard[column][row] = g_nextNext;
				}
			}
		}
	}

	return;
}

//////////////////////////////////////////////////////
//可動テトリミノ以外をg_tetlisBoardBufに同期させている 
VOID SynchroTetlisBoardBufToTetlisBoard(VOID)
{
	for (INT column = 0; column < TETLIS_HEIGHT; column++)
	{
		for (INT row = 0; row < TETLIS_WIDTH; row++) 
		{
			if ((0 <= g_tetlisBoard[column][row]) && (g_tetlisBoard[column][row] <= 6))
			{
				g_tetlisBoardBuf[column][row] = -1;
			}

			else
			{
 				g_tetlisBoardBuf[column][row] = g_tetlisBoard[column][row];
			}
 		}
	}

	return;
}

////////////////////////////////////////////////////
//g_tetlisBoardにg_movMinoNumOfArBufを同期させている
VOID SynchroTetlisBoardToMovMinoNumOfArBuf(INT currentTetmino)
{
	for (INT block = 0; block < 4; block++)
	{
		g_tetlisBoard[g_movMinoNumOfArBuf.YX[block][0]][g_movMinoNumOfArBuf.YX[block][1]] = currentTetmino;
	}

	return;
}

VOID ReturnToInitialStateWithTetlis(BOOL *isGameover, BOOL *canCreate, BOOL *canInputRA, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputR, BOOL *canInputSpace,
	BOOL *canHold, BOOL *wasHold, INT *rACount, INT *lACount, INT *dACount, INT *stopCount, INT *downCount, INT *scoreBuf, INT *currentTetmino, INT *minoIRoatationCount, INT *deletedLineCount, INT *lineCount, INT *additionalDeletableLine)
{
	memset(g_itemData.haveItem, 0, sizeof(INT)*g_itemMax);
	g_showGameoverStr = false;
	*isGameover = false;
	*canCreate = true;
	*canInputRA = true;
	*canInputLA = true;
	*canInputDA = true;
	*canInputR = true;
	*canInputSpace = true;
	*canHold = true;
	*wasHold = false;
	*rACount = 0;
	*lACount = 0;
	*dACount = 0;
	*stopCount = 0;
	*downCount = 0;
	*scoreBuf = 0;
	g_deletedLineCount = 0;
	g_additionalLand = 0;
	g_hold = -1;
	g_next = rand() % 7;
	g_nextNext = rand() % 7;
	*currentTetmino = rand() % 7;
	*minoIRoatationCount = 0;
	g_deletedLine = 0;
	*deletedLineCount = 0;
	*lineCount = 0;
	*additionalDeletableLine = 0;

	InitTetlisBoard();
	ChooseAndCpyTetlisBoardSourceToBoard();
	UpdateHoldNextNextNextBoard();
	memcpy(g_tetlisBoardBuf, g_tetlisBoard, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);

	return;
}

VOID CreateTetlimino(INT currentTetmino, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputRA,
	BOOL *canInputR, BOOL *canInputSpace, BOOL *wasHold, INT *stopCount, INT *downCount,INT *minoIRoatationCount, BOOL *canCreate)
{
	INT shiftY = 0;

	if (*canCreate)
	{
		for (INT block = 0; block < 4; block++)
		{
			for (INT YX = 0; YX < 2; YX++)
			{
				if (YX == 0)
				{
					shiftY = g_deletedLineCount;
				}
				else
				{
					shiftY = 0;
				}

				g_movMinoNumOfArBuf.YX[block][YX] = g_tetmino[currentTetmino].yx[block][YX] + shiftY;
			}
		}

		UpdateHoldNextNextNextBoard();
		*canInputLA = true;
		*canInputDA = true;
		*canInputRA = true;
		*canInputR = true;
		*canInputSpace = true;
		*wasHold = false;
		*stopCount = 0;
		*downCount = 0;
		*minoIRoatationCount = 0;
		*canCreate = false;
	}

	return;
}

VOID CountToMakeFlagTrue(BOOL *canInputLA,INT *lACount)
{
	if (*canInputLA == 0)
	{
		*lACount += 1;

		if (*lACount == SHIFT_FRAME_COUNTER+1)
		{
			*canInputLA = true;
			*lACount = 0;
		}
	}

	return;
}

VOID CountToMakeFlagFalse(INT *lACount)
{
	if (g_deletedLine == 1)
	{
		*lACount += 1;

		if (*lACount == 60+1)
		{
			g_deletedLine = false;
			*lACount = 0;
		}
	}

	return;
}

VOID InitNextAndNextNext(BOOL *isNewGame)
{
	g_next = rand() % 7;
	g_nextNext = rand() % 7;

	return;
}

VOID HoldTetlimino(BOOL *canHold, INT *currentTetmino, BOOL *canCreate, BOOL *wasHold)
{
	if (*canHold)
	{
		if (g_hold == -1)
		{
			g_hold = *currentTetmino;
			*currentTetmino = g_next;
			g_next = g_nextNext;
			g_nextNext = rand() % 7;
		}

		else
		{
			*currentTetmino = g_hold;
			g_hold = g_tetmino[g_tetminoNum].number;
			*canHold = false;
		}

		*wasHold = true;
		*canCreate = true;

		UpdateHoldNextNextNextBoard();
	}

	return;
}

VOID ShiftTetliminoX(INT shiftX,BOOL *canInputRA)
{
	if ((g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[0][0]][g_movMinoNumOfArBuf.YX[0][1] + shiftX] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[1][0]][g_movMinoNumOfArBuf.YX[1][1] + shiftX] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[2][0]][g_movMinoNumOfArBuf.YX[2][1] + shiftX] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[3][0]][g_movMinoNumOfArBuf.YX[3][1] + shiftX] == -1))
	{
		for (INT block = 0; block < 4; block++)
		{
			g_movMinoNumOfArBuf.YX[block][1] += shiftX;
		}

		*canInputRA = false;
	}

	return;
}

VOID RotateTetlimino(INT *minoIRoatationCount, INT currentTetmino)
{
	memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
	SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
	SynchroTetlisBoardBufToTetlisBoard();

	INT swap[4];
	INT backupY2X2[2] = { g_movMinoNumOfArBuf.YX[2][0], g_movMinoNumOfArBuf.YX[2][1] };

	*minoIRoatationCount = (*minoIRoatationCount > 3) ? 0 : *minoIRoatationCount;

	if (currentTetmino == 0)
	{
		for (INT block = 0; block < 4; block++)
		{
			switch (*minoIRoatationCount)
			{
			case 0:
				g_movMinoNumOfArBuf.YX[block][0] += 1;
				break;
			case 1:
				g_movMinoNumOfArBuf.YX[block][1] -= 1;
				break;
			case 2:
				g_movMinoNumOfArBuf.YX[block][0] -= 1;
				break;
			case 3:
				g_movMinoNumOfArBuf.YX[block][1] += 1;
				break;
			}

		}

		backupY2X2[0] = g_movMinoNumOfArBuf.YX[2][0];
		backupY2X2[1] = g_movMinoNumOfArBuf.YX[2][1];
	}

	if (currentTetmino != 6)
	{
		if (g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[0][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[0][0] - backupY2X2[0])) + backupY2X2[1]] == -1 &&
			g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[1][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[1][0] - backupY2X2[0])) + backupY2X2[1]] == -1 &&
			g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[2][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[2][0] - backupY2X2[0])) + backupY2X2[1]] == -1 &&
			g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[3][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[3][0] - backupY2X2[0])) + backupY2X2[1]] == -1)
		{
			for (INT block = 0; block < 4; block++)
			{
				swap[block] = g_movMinoNumOfArBuf.YX[block][1] - backupY2X2[1] + backupY2X2[0];
				g_movMinoNumOfArBuf.YX[block][1] = (-(g_movMinoNumOfArBuf.YX[block][0] - backupY2X2[0])) + backupY2X2[1];
				g_movMinoNumOfArBuf.YX[block][0] = swap[block];
			}

			*minoIRoatationCount += 1;
		}

		else
		{
			if (g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[0][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[0][0] - backupY2X2[0])) + backupY2X2[1] + 1] == -1 &&
				g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[1][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[1][0] - backupY2X2[0])) + backupY2X2[1] + 1] == -1 &&
				g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[2][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[2][0] - backupY2X2[0])) + backupY2X2[1] + 1] == -1 &&
				g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[3][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[3][0] - backupY2X2[0])) + backupY2X2[1] + 1] == -1)
			{
				for (INT block = 0; block < 4; block++)
				{
					swap[block] = g_movMinoNumOfArBuf.YX[block][1] - backupY2X2[1] + backupY2X2[0];
					g_movMinoNumOfArBuf.YX[block][1] = (-(g_movMinoNumOfArBuf.YX[block][0] - backupY2X2[0])) + backupY2X2[1] + 1;
					g_movMinoNumOfArBuf.YX[block][0] = swap[block];
				}

				*minoIRoatationCount += 1;
			}

			else
			{
				if (g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[0][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[0][0] - backupY2X2[0])) + backupY2X2[1] - 1] == -1 &&
					g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[1][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[1][0] - backupY2X2[0])) + backupY2X2[1] - 1] == -1 &&
					g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[2][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[2][0] - backupY2X2[0])) + backupY2X2[1] - 1] == -1 &&
					g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[3][1] - backupY2X2[1]) + backupY2X2[0]][(-(g_movMinoNumOfArBuf.YX[3][0] - backupY2X2[0])) + backupY2X2[1] - 1] == -1)
				{
					for (INT block = 0; block < 4; block++)
					{
						swap[block] = g_movMinoNumOfArBuf.YX[block][1] - backupY2X2[1] + backupY2X2[0];
						g_movMinoNumOfArBuf.YX[block][1] = (-(g_movMinoNumOfArBuf.YX[block][0] - backupY2X2[0])) + backupY2X2[1] - 1;
						g_movMinoNumOfArBuf.YX[block][0] = swap[block];
					}

					*minoIRoatationCount += 1;
				}

				else
				{
					if (g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[0][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[0][0] - backupY2X2[0])) + backupY2X2[1]] == -1 &&
						g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[1][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[1][0] - backupY2X2[0])) + backupY2X2[1]] == -1 &&
						g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[2][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[2][0] - backupY2X2[0])) + backupY2X2[1]] == -1 &&
						g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[3][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[3][0] - backupY2X2[0])) + backupY2X2[1]] == -1)
					{
						for (INT block = 0; block < 4; block++)
						{
							swap[block] = g_movMinoNumOfArBuf.YX[block][1] - backupY2X2[1] + backupY2X2[0] + 1;
							g_movMinoNumOfArBuf.YX[block][1] = (-(g_movMinoNumOfArBuf.YX[block][0] - backupY2X2[0])) + backupY2X2[1];
							g_movMinoNumOfArBuf.YX[block][0] = swap[block];
						}

						*minoIRoatationCount += 1;
					}

					else
					{
						if (currentTetmino == 0)
						{
							if (g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[0][1] - backupY2X2[1]) + backupY2X2[0] - 1][(-(g_movMinoNumOfArBuf.YX[0][0] - backupY2X2[0])) + backupY2X2[1] + 2] == -1 &&
								g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[1][1] - backupY2X2[1]) + backupY2X2[0] - 1][(-(g_movMinoNumOfArBuf.YX[1][0] - backupY2X2[0])) + backupY2X2[1] + 2] == -1 &&
								g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[2][1] - backupY2X2[1]) + backupY2X2[0] - 1][(-(g_movMinoNumOfArBuf.YX[2][0] - backupY2X2[0])) + backupY2X2[1] + 2] == -1 &&
								g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[3][1] - backupY2X2[1]) + backupY2X2[0] - 1][(-(g_movMinoNumOfArBuf.YX[3][0] - backupY2X2[0])) + backupY2X2[1] + 2] == -1)
							{
								for (INT block = 0; block < 4; block++)
								{
									swap[block] = (g_movMinoNumOfArBuf.YX[block][1] - backupY2X2[1]) + backupY2X2[0] - 1;
									g_movMinoNumOfArBuf.YX[block][1] = (-(g_movMinoNumOfArBuf.YX[block][0] - backupY2X2[0])) + backupY2X2[1] + 2;
									g_movMinoNumOfArBuf.YX[block][0] = swap[block];
								}

								*minoIRoatationCount += 1;
							}

							else
							{
								if (g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[0][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[0][0] - backupY2X2[0])) + backupY2X2[1] - 2] == -1 &&
									g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[1][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[1][0] - backupY2X2[0])) + backupY2X2[1] - 2] == -1 &&
									g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[2][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[2][0] - backupY2X2[0])) + backupY2X2[1] - 2] == -1 &&
									g_tetlisBoardBuf[(g_movMinoNumOfArBuf.YX[3][1] - backupY2X2[1]) + backupY2X2[0] + 1][(-(g_movMinoNumOfArBuf.YX[3][0] - backupY2X2[0])) + backupY2X2[1] - 2] == -1)
								{
									for (INT block = 0; block < 4; block++)
									{
										swap[block] = g_movMinoNumOfArBuf.YX[block][1] - backupY2X2[1] + backupY2X2[0] + 1;
										g_movMinoNumOfArBuf.YX[block][1] = (-(g_movMinoNumOfArBuf.YX[block][0] - backupY2X2[0])) + backupY2X2[1] - 2;
										g_movMinoNumOfArBuf.YX[block][0] = swap[block];
									}

									*minoIRoatationCount += 1;
								}

								else
								{
									for (INT block = 0; block < 4; block++)
									{
										switch (*minoIRoatationCount)
										{
										case 0:
											g_movMinoNumOfArBuf.YX[block][0] -= 1;
											break;
										case 1:
											g_movMinoNumOfArBuf.YX[block][1] += 1;
											break;
										case 2:
											g_movMinoNumOfArBuf.YX[block][0] += 1;
											break;
										case 3:
											g_movMinoNumOfArBuf.YX[block][1] -= 1;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return;
}

VOID DownTetlimino(BOOL *canInputDA)
{
	if ((g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[0][0] + 1][g_movMinoNumOfArBuf.YX[0][1]] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[1][0] + 1][g_movMinoNumOfArBuf.YX[1][1]] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[2][0] + 1][g_movMinoNumOfArBuf.YX[2][1]] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[3][0] + 1][g_movMinoNumOfArBuf.YX[3][1]] == -1))
	{
		for (int block = 0; block < 4; block++)
		{
			g_movMinoNumOfArBuf.YX[block][0] += 1;
		}
		
		*canInputDA = false;
	}
	
	return;
}

VOID HardDropTetlimino(VOID)
{
	for (INT column = 1; column < TETLIS_HEIGHT; column++)
	{
		if ((g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[0][0] + column][g_movMinoNumOfArBuf.YX[0][1]] != -1) ||
			(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[1][0] + column][g_movMinoNumOfArBuf.YX[1][1]] != -1) ||
			(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[2][0] + column][g_movMinoNumOfArBuf.YX[2][1]] != -1) ||
			(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[3][0] + column][g_movMinoNumOfArBuf.YX[3][1]] != -1))
		{
			for (int block = 0; block < 4; block++)
			{
				g_movMinoNumOfArBuf.YX[block][0] += column - 1;
				g_hardDropedTetmino = true;
			}

			break;
		}
	}

	return;
}

VOID SetTetliminoTarget(VOID)
{
	for (INT column = 1; column < TETLIS_HEIGHT; column++)
	{
		if ((g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[0][0] + column][(g_movMinoNumOfArBuf.YX[0][1])] != -1) ||
			(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[1][0] + column][(g_movMinoNumOfArBuf.YX[1][1])] != -1) ||
			(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[2][0] + column][(g_movMinoNumOfArBuf.YX[2][1])] != -1) ||
			(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[3][0] + column][(g_movMinoNumOfArBuf.YX[3][1])] != -1))
		{
			for (INT block = 0; block < 4; block++)
			{
				g_targetMinoNumOfArBuf.YX[block][0] = g_movMinoNumOfArBuf.YX[block][0];
				g_targetMinoNumOfArBuf.YX[block][1] = g_movMinoNumOfArBuf.YX[block][1];
				g_targetMinoNumOfArBuf.YX[block][0] += column - 1;
			}

			break;
		}
	}

	return;

}

VOID CountToDawnTetlimino(INT *downCount)
{
	if ((g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[0][0] + 1][g_movMinoNumOfArBuf.YX[0][1]] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[1][0] + 1][g_movMinoNumOfArBuf.YX[1][1]] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[2][0] + 1][g_movMinoNumOfArBuf.YX[2][1]] == -1) &&
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[3][0] + 1][g_movMinoNumOfArBuf.YX[3][1]] == -1))
	{
		*downCount += 1;

		if (*downCount == FLAME_PER_DOWN+1)
		{
			for (int block = 0; block < 4; block++)
			{
				g_movMinoNumOfArBuf.YX[block][0] += 1;
			}

			*downCount = 0;
		}
	}

	return;
}

VOID CountToStopTetlimino(INT *stopCount, INT *currentTetmino,BOOL *canCreate, BOOL *canHold, BOOL *wasHold)
{
	if ((g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[0][0] + 1][g_movMinoNumOfArBuf.YX[0][1]] != -1) ||
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[1][0] + 1][g_movMinoNumOfArBuf.YX[1][1]] != -1) ||
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[2][0] + 1][g_movMinoNumOfArBuf.YX[2][1]] != -1) ||
		(g_tetlisBoardBuf[g_movMinoNumOfArBuf.YX[3][0] + 1][g_movMinoNumOfArBuf.YX[3][1]] != -1))
	{
		*stopCount += 1;
		if (*stopCount == FLAME_PER_STOP+1)
		{
			for (INT column = 0; column < TETLIS_HEIGHT; column++)
			{
				for (INT row = 0; row < TETLIS_WIDTH; row++)
				{
					if (g_tetlisBoard[column][row] == *currentTetmino)
					{
						g_tetlisBoard[column][row] = *currentTetmino + 100;
					}
				}
			}

			for (INT block = 0; block < 4; block++)
			{
				for (INT YX = 0; YX < 2; YX++)
				{
					g_movMinoNumOfArBuf.YX[block][YX] = g_tetmino[*currentTetmino].yx[block][YX];
				}
			}

			SynchroTetlisBoardBufToTetlisBoard();

			*canCreate = true;
			*canHold = true;
			*wasHold = false;
			*stopCount = 0;
			*currentTetmino = g_next;
			g_next = g_nextNext;
			g_nextNext = rand() % 7;
		}
	}

	return;
}

VOID DeleteAndCountFilledLine(INT *lineCount, INT *additionalDeletableLine)
{
	InitDurableBlockBeared();
	InitReduceBlockPosition();

	INT firstDeletedColumn = 0;
	BOOL isFirstDeletedLine = true;
	INT prevAdditionalLand = g_additionalLand;

	for (INT column = TETLIS_HEIGHT - 2; column > 3; column--)
	{
		if (g_tetlisBoardBuf[column][1] % 100 != -1 &&
			g_tetlisBoardBuf[column][2] % 100 != -1 &&
			g_tetlisBoardBuf[column][3] % 100 != -1 &&
			g_tetlisBoardBuf[column][4] % 100 != -1 &&
			g_tetlisBoardBuf[column][5] % 100 != -1 &&
			g_tetlisBoardBuf[column][6] % 100 != -1 &&
			g_tetlisBoardBuf[column][7] % 100 != -1 &&
			g_tetlisBoardBuf[column][8] % 100 != -1 &&
			g_tetlisBoardBuf[column][9] % 100 != -1 &&
			g_tetlisBoardBuf[column][10] % 100 != -1)
		{
			*lineCount += 1;
		}
	}

	for (INT column = TETLIS_HEIGHT - 2; column > 3; column--)
	{
		INT additionalLandCount = 0;

		if (g_tetlisBoardBuf[column][1] % 100 != -1 &&
			g_tetlisBoardBuf[column][2] % 100 != -1 &&
			g_tetlisBoardBuf[column][3] % 100 != -1 &&
			g_tetlisBoardBuf[column][4] % 100 != -1 &&
			g_tetlisBoardBuf[column][5] % 100 != -1 &&
			g_tetlisBoardBuf[column][6] % 100 != -1 &&
			g_tetlisBoardBuf[column][7] % 100 != -1 &&
			g_tetlisBoardBuf[column][8] % 100 != -1 &&
			g_tetlisBoardBuf[column][9] % 100 != -1 &&
			g_tetlisBoardBuf[column][10] % 100 != -1)
		{
			for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
			{
				if (g_tetlisBoardBuf[column][row] == 110)
				{
					additionalLandCount++;
				}
			}

			if (g_additionalLand&&additionalLandCount > 3)
			{
				g_additionalLand--;
			}

			for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
			{
				switch (g_tetlisBoardBuf[column][row])
				{
				case 110:

					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 120:

					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 121:

					g_tetlisBoard[column][row] -= 1;
					g_durableBlockBeared[column][row] = 1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 130:

					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 131:

					g_tetlisBoard[column][row] -= 1;
					g_durableBlockBeared[column][row] = 1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 132:

					g_tetlisBoard[column][row] -= 1;
					g_durableBlockBeared[column][row] = 1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 140:

					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;
					g_itemData.haveItem[g_laserCannonItem] = true;

					break;

				case 150:

					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;
					g_itemData.haveItem[g_bulletItem] = true;

					break;

				case 160:
					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;
					g_itemData.haveItem[g_ultraDrillItem] = true;
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				case 170:

					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;
					g_itemData.haveItem[g_excaliberItem] = true;
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;

				default:
					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				}
			}

			g_deletedLine = true;
		}
	}

	switch (*lineCount)
	{
	case 1:

		*additionalDeletableLine = 0;

		break;

	case 2:

		*additionalDeletableLine = 2;
		
		break;

	case 3:

		*additionalDeletableLine = 4;
		
		break;

	case 4:

		*additionalDeletableLine = 7;
		
		break;
	}

	INT loopLimiter = ((firstDeletedColumn + 1 + *additionalDeletableLine)>TETLIS_HEIGHT - 1) ?
		TETLIS_HEIGHT - 1 : firstDeletedColumn + 1 + *additionalDeletableLine;

	for (INT column = firstDeletedColumn + 1; column < loopLimiter ; column++)
	{
		INT additionalLandCount = 0;

		for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
		{
			if (g_tetlisBoardBuf[column][row] == 110)
			{
				additionalLandCount++;
			}
		}

		if (g_additionalLand&&additionalLandCount > 3)
		{
			g_additionalLand--;
		}

		for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
		{
			switch (*lineCount)
			{
			case 2:
				switch (g_tetlisBoardBuf[column][row])
				{
				case 121:
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				case 131:
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				case 132:
					g_tetlisBoard[column][row] -= 2;
					g_durableBlockBeared[column][row] = 1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				}
			case 3:
			case 4:
				switch (g_tetlisBoardBuf[column][row])
				{
				case 121:
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				case 131:
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				case 132:
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;

					break;
				}
			}

			switch (g_tetlisBoardBuf[column][row])
			{
			case 110:
				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;

				break;
			case 120:
				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;

				break;
			case 130:
				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;

				break;
			case 140:

				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;
				g_itemData.haveItem[g_laserCannonItem] = true;
				break;

			case 150:

				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;
				g_itemData.haveItem[g_bulletItem] = true;

				break;

			case 160:

				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;
				g_itemData.haveItem[g_ultraDrillItem] = true;
				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;

				break;

			case 170:

				if (isFirstDeletedLine)
				{
					firstDeletedColumn = column;
					isFirstDeletedLine = false;
				}

				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;
				g_itemData.haveItem[g_excaliberItem] = true;
				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;

				break;

			default:

				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;

				break;
			}
		}

		g_deletedLine = true;
	}

	if (prevAdditionalLand - g_additionalLand)
	{
		g_swellingUpSaver = -(prevAdditionalLand - g_additionalLand);
	}

	SynchroTetlisBoardBufToTetlisBoard();

	return;
}

VOID ShiftTetlisLine(INT *lineCount, INT *additionalDeletableLine)
{
	for (INT shiftLoop = 0; shiftLoop < 11; shiftLoop++)
	{
		for (INT column = TETLIS_HEIGHT - 2; column > 3; column--)
		{
			if (g_tetlisBoardBuf[column][1] == -1 &&
				g_tetlisBoardBuf[column][2] == -1 &&
				g_tetlisBoardBuf[column][3] == -1 &&
				g_tetlisBoardBuf[column][4] == -1 &&
				g_tetlisBoardBuf[column][5] == -1 &&
				g_tetlisBoardBuf[column][6] == -1 &&
				g_tetlisBoardBuf[column][7] == -1 &&
				g_tetlisBoardBuf[column][8] == -1 &&
				g_tetlisBoardBuf[column][9] == -1 &&
				g_tetlisBoardBuf[column][10] == -1)
			{
				for (INT coordinateY = column; coordinateY > 3; coordinateY--)
				{
					for (INT coordinateX = 1; coordinateX < TETLIS_WIDTH - 1; coordinateX++)
					{
						g_tetlisBoard[coordinateY][coordinateX] = g_tetlisBoard[coordinateY - 1][coordinateX];
						g_tetlisBoard[coordinateY - 1][coordinateX] = -1;
						g_durableBlockBeared[coordinateY][coordinateX] = g_durableBlockBeared[coordinateY - 1][coordinateX];
						g_durableBlockBeared[coordinateY-1][coordinateX] = false;
					}
				}

				SynchroTetlisBoardBufToTetlisBoard();
			}
		}
	}

	for (INT column = TETLIS_HEIGHT - 2; column > 3; column--)
	{
		for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
		{
			////////////////////////////
			//ハードドロップの処理を利用
			ShiftTetlisBlockInvolvedInDurableBlock(column, row);
		}
	}

	*lineCount = 0;
	*additionalDeletableLine = 0;

	return;
}

VOID ShiftTetlisBlockInvolvedInDurableBlock(INT column, INT row)
{
	if (!(g_durableBlockBeared[column][row] == 1))
	{
		return;
	}

	for (INT coordinateX = 1; coordinateX < TETLIS_WIDTH - 1; coordinateX++)
	{
		if (g_tetlisBoardBuf[column][coordinateX] == -1)
		{
			for (INT coordinateY = column; coordinateY > 3; coordinateY--)
			{
				if (g_tetlisBoardBuf[coordinateY][coordinateX] != -1)
				{
					for (INT numOfY = 0; numOfY < 12; numOfY++)
					{
						g_tetlisBoard[column - numOfY][coordinateX] = g_tetlisBoard[coordinateY - numOfY][coordinateX];
						g_tetlisBoard[coordinateY - numOfY][coordinateX] = -1;
						SynchroTetlisBoardBufToTetlisBoard();
					}
					break;
				}
			}
		}
	}

	return;
}

//////////////////////////////////////////////////
//いくつ初期状態からラインを消されたか確認する関数
VOID CountDeletedLine(VOID)
{
	g_deletedLineCount = 0;

	for (INT column = 19; column < TETLIS_HEIGHT - 2; column++)
	{
		INT numOfDirt = 0;
		for (INT row = 1; row < TETLIS_WIDTH - 2; row++)
		{
			if (g_tetlisBoardBuf[column][row] % 100 == 10)
			{
				numOfDirt++;
			}
		}

		if (numOfDirt < 4)
		{
			g_deletedLineCount++;
		}
	}

	return;
}

VOID GetScoreByLineCount(INT lineCount, INT *scoreBuf)
{
	switch (lineCount)
	{
	case 1:
		*scoreBuf += 40;
		break;
	case 2:
		*scoreBuf += 100;
		break;
	case 3:
		*scoreBuf += 300;
		break;
	case 4:
		*scoreBuf += 1200;
		break;
	}

	sprintf(g_scoreArray, "%d", *scoreBuf);

	return;
}

VOID CheckGameover(BOOL *isGameover)
{
	for (INT coordinateX = 1; coordinateX < 11; coordinateX++)
	{
		if (g_tetlisBoardBuf[4 + g_deletedLineCount+ g_additionalLand][coordinateX] != -1)
		{
			SoundManager& soundManager = SoundManager::GetInstance();
			soundManager.Stop(g_pD3DSound[g_bgmAudio]);
			soundManager.Play(g_pD3DSound[g_gameoverAudio], 0);
			*isGameover = true;
		}
	}

	return;
}

//時間で地面をせり上げる
VOID AddLand(INT *scoreBuf,INT *additionalLandCount)
{
	switch ((*scoreBuf) / 2000)
	{
	case 0:

		if (*additionalLandCount > 1200)
		{
			g_additionalLand++;
			*additionalLandCount = 0;
		}

		break;

	case 1:

		if (*additionalLandCount > 1020)
		{
			g_additionalLand++;
			*additionalLandCount = 0;
		}

		break;

	case 2:

		if (*additionalLandCount > 840)
		{
			g_additionalLand++;
			*additionalLandCount = 0;
		}

		break;

	case 3:

		if (*additionalLandCount > 660)
		{
			g_additionalLand++;
			*additionalLandCount = 0;
		}

		break;

	case 4:

		if (*additionalLandCount > 560)
		{
			g_additionalLand++;
			*additionalLandCount = 0;
		}

		break;

	default:

		if (*additionalLandCount > 360)
		{
			g_additionalLand++;
			*additionalLandCount = 0;
		}

		break;

	}

	(*additionalLandCount) += 1;
	
	return;
}
