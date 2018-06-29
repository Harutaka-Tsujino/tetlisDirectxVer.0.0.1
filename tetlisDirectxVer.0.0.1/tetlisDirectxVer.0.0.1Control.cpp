/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control
// 
// DATE 2018.06.13
// VER.0.0.5
//
// Author Harutaka-Tsujino
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetlisDirectxVer.0.0.1Common.h"
#include "tetlisDirectxVer.0.0.1Initialize.h"
#include "tetlisDirectxVer.0.0.1Control.h"
#include <dinput.h>

BOOL g_durableBlockBeared[TETLIS_HEIGHT][TETLIS_WIDTH];
BOOL g_reduceBlockDurPosition[TETLIS_HEIGHT][TETLIS_WIDTH];

INT g_deletedLine = false;
INT g_deletedLineNum = 0;

////////////////////////////////
//�e�g���X�Ȃǂ̑���Ɋւ���֐�
VOID Control(VOID)
{
	//�L�[�f�o�C�X�ɓ����Ă������ǂݎ�鏀��
	HRESULT hr = g_pDKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		//�L�[���͏��̃o�b�t�@�[
		BYTE diks[256];

		//���͂��ꂽ����ǂݎ��
		g_pDKeyDevice->GetDeviceState(sizeof(diks), &diks);

		static BOOL canInputLA = true, canInputDA = true, canInputRA = true, canInputR = true, canInputSpace = true, isGameover = false, isNewGame = true, canHold = true, wasHold = false, canCreate = true;

		//���������̃e�g���~�m��ނ����߂�
		static INT rACount = 0, lACount = 0, dACount = 0, stopCount = 0, downCount = 0, scoreBuf = 0, minoIRoatationCount = 0,prevUpKeyState = 0, prevRKeyState = 0, prevSpaceKeyState = 0, currentTetmino = rand() % 7, prevDeletedLineCount = 0, deletedLineCount = 0;
		
		static INT lineCount = 0, additionalDeletableLine = 0;

		INT LEFT[2] = { 0,diks[DIK_LEFT] & 0x80 }, DOWN[2] = { 0,diks[DIK_DOWN] & 0x80 }, RIGHT[2] = { 0,diks[DIK_RIGHT] & 0x80 };

		//�e�g���X�z��Ƀe�g���X�z��o�b�t�@�[�̗v�f�S�Ă��R�s�[���Ă���
		memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);

		//���Z�b�g�{�^���A������Ԃɖ߂�
		if (diks[DIK_BACK] & 0x80)
		{
			///////////////////////////////////////////////////////////////////////////
			//�t���O�A�J�E���g�A�z���������Ԃɖ߂�UpdateHoldNextNextNextBoard��p����
			ReturnToInitialStateWithTetlis(&isGameover, &canCreate, &canInputRA, &canInputLA, &canInputDA, &canInputR, &canInputSpace,
				&canHold, &wasHold, &rACount, &lACount, &dACount, &stopCount, &downCount, &scoreBuf, &currentTetmino, &minoIRoatationCount, &prevDeletedLineCount, &deletedLineCount, &lineCount, &additionalDeletableLine);
		}

		if (isGameover)
		{
			//�Q�[���I�[�o�[���ɕ������\��������
			g_showGameoverStr = true;
		}
		else
		{
			if (isNewGame)
			{
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//�j���[�Q�[��(��ԍŏ��̃t���[��)���ɂ�g_next��g_nextNext�͏���������Ă��Ȃ��̂ŏ��������AisNewGame��false�ɂ���
				InitNextAndNextNext(&isNewGame);
				InitTetlisBoard();
				ChooseAndCpyTetlisBoardSourceToBoard();
				memcpy(g_tetlisBoardBuf, g_tetlisBoard, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				isNewGame = false;
			}

			if (!(g_deletedLine))
			{
				for (g_tetminoNum = 0; g_tetminoNum < 7; g_tetminoNum++)
				{
					if (g_tetmino[g_tetminoNum].number == currentTetmino)
					{
						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//g_canCreate���m�F��true�Ȃ��g_movMinoNumOfArBuf��currentTetmino�Ɠ�����ނ̃e�g���~�m�������W�������A�L�[���͂̃t���O�A
						//I�^�̃e�g���~�m������]���������m�F����J�E���g�A�z�[���h���s��ꂽ���ۂ��̃t���O��������Ԃɖ߂��Ag_canCreate��false�ɂ���
						CreateTetlimino(currentTetmino, &canInputLA, &canInputDA, &canInputRA,
							&canInputR, &canInputSpace, &wasHold, &stopCount, &downCount, &minoIRoatationCount, &canCreate);

						break;
					}
				}

				//////////////////////////////////////////////////////////////////////////////////////////////
				//�t���O��false�Ȃ�΃J�E���g��1���₵�A���l�Ȃ�΃t���O��true�ɕς��J�E���g��������Ԃɂ���
				CountToMakeFlagTrue(&canInputLA, &lACount);
				CountToMakeFlagTrue(&canInputRA, &rACount);
				CountToMakeFlagTrue(&canInputDA, &dACount);

				//�O�t���[�����ɃX�y�[�X�L�[��������Ă����ꍇ���̏�����ʂ��Ȃ��A�Ƃ��邱�Ƃɂ���ĘA�������t���[�����ɂ��̏����������̂�h���ł���
				if (!(prevSpaceKeyState))
				{
					if (diks[DIK_SPACE] & 0x80)
					{
						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//canHold��true�Ȃ��g_hold�Ɍ��݂̃e�g���~�m�̒l�������A���ł�-1(��)�ȊO�������Ă����ꍇcurrentTetmino��g_hold�̒l������A
						//���̌�g_canCreate��true�ɂ�UpdateHoldNextNextNextBoard���Ăяo��
						HoldTetlimino(&canHold, &currentTetmino, &canCreate, &wasHold);
					}
				}

				//�z�[���h���s���Ă����ꍇ�V�����e�g���~�m�𐶐�����̂ŁA���̏�����ʂ��Ȃ�
				if (!(wasHold))
				{
					if (RIGHT[canInputRA])
					{
						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//g_movMinoNumOfArBuf�Ɉ����𑫂����z��ԍ���g_tetlisBoardBuf�ŎQ�Ƃ��A�󗓂������ꍇg_movMinoNumOfArBuf�Ɉ����𑫂����l��������
						ShiftTetliminoX(1, &canInputRA);
					}

					if (LEFT[canInputLA])
					{
						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						ShiftTetliminoX(-1, &canInputLA);
					}

					memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
					SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
					SynchroTetlisBoardBufToTetlisBoard();

					//�O�t���[������R��������Ă����Ȃ�΁A���̏�����ʂ��Ȃ��A�Ƃ��邱�Ƃɂ���ĘA���œ�������������邱�Ƃ�h���ł���
					if (!(prevRKeyState))
					{
						if (diks[DIK_R] & 0x80)
						{
							memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
							SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
							SynchroTetlisBoardBufToTetlisBoard();

							///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//g_minoIRoatationCount���R�𒴂��Ă����ꍇ0�������AI�^�̃e�g���~�m�̏ꍇg_minoIRoatationCount�̒l�ɂ���Ď������炷�A
							//X2�AY2�𒆐S�Ƃ��邽�߂�X2�AY2�ł��ꂼ��4�u���b�N���������S��0,0�Ɉړ��������]�����鏈�����s���O��
							//g_tetlisBoardBuf���Q�Ƃ��ĉ�]���ł���ꍇ�ɏ������s���A��]�ł��Ȃ��ꍇI�^�̃e�g���~�m�����炵�����������ɖ߂�
							RotateTetlimino(&minoIRoatationCount, currentTetmino);
						}
					}

					if (DOWN[canInputDA])
					{
						memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
						SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
						SynchroTetlisBoardBufToTetlisBoard();

						/////////////////////////////////////////////////////////////////////////////////////////////////
						//g_movMinoNumOfArBuf��Y�����ɂP�������z���p����g_tetlisBoardBuf���Q�Ƃ��A�󗓂̏ꍇ�ړ����s��
						DownTetlimino(&canInputDA);
					}

					//�n�[�h�h���b�v�@�e�g���~�m�̃u���b�N����Â������m�F���A�������󗓂ł͂Ȃ��ꍇ�A��������1��Ƀ��[�v������
					if (!(prevUpKeyState))
					{
						if (diks[DIK_UP] & 0x80)
						{
							memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
							SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
							SynchroTetlisBoardBufToTetlisBoard();

							///////////////////////////////////////////////////////////////////////////////////
							//���[�v�J�E���^��p��g_tetlisBoardBuf���Q�Ƃ��A�󗓂ł͂Ȃ������ꍇg_movMinoNumOfArBuf��
							//���[�v�J�E���^-1�̏ꏊ�Ƀe�g���~�m���ړ�������
							HardDropTetlimino();
						}
					}

					memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
					SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
					SynchroTetlisBoardBufToTetlisBoard();

					//�e�g���~�m�̉������m�F���A�󗓂������ꍇ�J�E���g���Ƃ�AFLAME_PER_DOWN�ɒB�����ꍇ����Ɉړ�������
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					//g_movMinoNumOfArBuf+1�����z��ԍ���p����g_tetlisBoardBuf���Q�Ƃ��A�󗓂������ꍇ�J�E���g��1���₵�A
					//���l�Ȃ��g_movMinoNumOfArBuf�ɂP�𑫂��A�J�E���g��������Ԃɂ���
					CountToDawnTetlimino(&downCount);

					memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
					SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
					SynchroTetlisBoardBufToTetlisBoard();

					/*�e�g���~�m�������m�F���A�������󗓂ł͂Ȃ��ꍇ�J�E���g���Ƃ�AFLAME_PER_STOP�ɒB�����ꍇ
					���e�g���~�m�̃i���o�[��100�𑫂�(�ȒP�ɉ�������𔻕ʂ��邽��)�A�V�������e�g���~�m�𐶐�����*/
					/////////////////////////////////////////////////////////////////////////////////////////////////
					//g_movMinoNumOfArBuf+1�������z��ԍ���p��g_tetlisBoardBuf���Q�Ƃ��A��(-1)�ȊO�������Ă����ꍇ
					//�J�E���g���P���₵�A���l�Ȃ��currentTetmino + 10��g_tetlisBoard�ɑ�����A
					//�e�g���~�m�𐶐��ł��邩�ۂ��A�z�[���h���ł��邩�ۂ��̃t���O��true�A
					//�z�[���h���s��ꂽ���̃t���O��false�ɂ��AcurrentTetmino��g_next�Ag_next��g_nextNext�������āA
					//g_nextNext�Ƀe�g���~�m�ԍ��͈̗̔͂�����������
					CountToStopTetlimino(&stopCount, &currentTetmino, &canCreate, &canHold, &wasHold);
				}
			}

			memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
			SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
			SynchroTetlisBoardBufToTetlisBoard();

			if (deletedLineCount == 0)
			{
				//������Ă���񂪂��邩�m�F���J�E���g���Ƃ�
				//////////////////////////////////////////////////////////////////////////////
				//g_tetlisBoardBuf���g���m�F����񂪋�(-1)�ȊO�̏ꍇ�󗓂ɂ���
				DeleteAndCountFilledLine(&lineCount, &additionalDeletableLine);

				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
				SynchroTetlisBoardBufToTetlisBoard();

				/////////////////////////////////////////////////////////////////////////////////
				//lineCount�̒l�ɂ����scoreBuf�̑��₷�l��ς��A������ɂ�g_scoreArray�ɑ������
				GetScoreByLineCount(lineCount, &scoreBuf);

				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
				SynchroTetlisBoardBufToTetlisBoard();
			}

			///////////////////////////////////////////////////////
			//�t���O��true�Ȃ�΁A�J�E���g���Ƃ�t���O��false�ɂ���
			CountToMakeFlagFalse(&deletedLineCount);

			if (deletedLineCount == 60)
			{
				//�����������󗓂ɂ��ĉ��ɂ��炵�Ă���
				/////////////////////////////////////////////////////////////////////////////////////////////
				//g_tetlisBoardBuf���Q�Ƃ��A��(-1)�ȊO�̏ꍇ���[�v�J�E���^+1�����z��ԍ���p���A�ēx�Q�Ƃ��A
				//���S�ċ󗓂̏ꍇ���[�v�J�E���^+1�̔z��ԍ��ɃR�s�[���A�R�s�[�����󗓂ɏ���������
				ShiftTetlisLine(&lineCount, &prevDeletedLineCount, &additionalDeletableLine);

				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
				SynchroTetlisBoardBufToTetlisBoard();

				//////////////////////////////////////////////////
				//����������Ԃ��烉�C���������ꂽ���m�F����֐�
				CountDeletedLine();
			}

			if (deletedLineCount == 0)
			{
				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
				SynchroTetlisBoardBufToTetlisBoard();

				////////////////////////////////////////
				//�n�[�h�h���b�v�Ɠ��������𗘗p���Ă���
				SetTetliminoTarget();

				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
				SynchroTetlisBoardToMovMinoNumOfArBuf(currentTetmino);
				SynchroTetlisBoardBufToTetlisBoard();

				////////////////////////////////////////////////////////////////////////////////////////
				//�v���C���̌�����͈͓��ň�ԏ�̕����ɔ���e�g���~�m������ꍇisGameover��true�ɂ���
				CheckGameover(&isGameover);
			}

			//�L�[���͏�Ԃ�ۑ�
			prevRKeyState = diks[DIK_R] & 0x80;
			prevSpaceKeyState = diks[DIK_SPACE] & 0x80;
			prevUpKeyState = diks[DIK_UP] & 0x80;

			//�z�[���h���Ƀu���b�N���\������Ă���̂ŏ���
			if (wasHold)
			{
				memcpy(g_tetlisBoard, g_tetlisBoardBuf, sizeof(INT)*TETLIS_HEIGHT*TETLIS_WIDTH);
			}
		}
	}

	return;
}

////////////////////////////////////////////
//�e�g���X�z��ɑI�΂ꂽ�p�^�[�����R�s�[����
VOID ChooseAndCpyTetlisBoardSourceToBoard(VOID)
{
	for (INT line = 19; line < 137; line += 3)
	{
		for (INT column = 0; column < 3; column++)
		{
			for (INT row = 0; row < 12; row++)
			{
				g_tetlisBoard[line + column][row] = g_tetlisBoardSource[column][row];
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//�|1�͋󗓂�\���A��ɑS�Ă̔z��̒��g���󗓂ɂ��邱�Ƃɂ��A��(9)���ォ��h��Ԃ������ŕ\����������
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
//���e�g���~�m(�����Ă���)���e�g���X�{�[�h�o�b�t�@�[�ɓ���Ȃ��悤�ɂ��邱�Ƃɂ��A��]�A�ړ��Ȃǂ̍ہA���e�g���~�m���l�������Ƃ��ǂ�
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
//g_durableBlockBeared������������
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
//g_reduceBlockDurPosition������������
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
//�e�g���~�m�̏������W(���������Ƃ��̍��W)��p���邽�߁Ax���W��-4���炷���ƂŃl�N�X�g���̕\�Ƃ̐��������Ƃ�A
//�z�[���h�ɓ����Ă���l�ɂ���āA�����e�g���~�m��ς���
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
//���e�g���~�m�ȊO��g_tetlisBoardBuf�ɓ��������Ă��� 
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
//g_tetlisBoard��g_movMinoNumOfArBuf�𓯊������Ă���
VOID SynchroTetlisBoardToMovMinoNumOfArBuf(INT currentTetmino)
{
	for (INT block = 0; block < 4; block++)
	{
		g_tetlisBoard[g_movMinoNumOfArBuf.YX[block][0]][g_movMinoNumOfArBuf.YX[block][1]] = currentTetmino;
	}

	return;
}

VOID ReturnToInitialStateWithTetlis(BOOL *isGameover, BOOL *canCreate, BOOL *canInputRA, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputR, BOOL *canInputSpace,
	BOOL *canHold, BOOL *wasHold, INT *rACount, INT *lACount, INT *dACount, INT *stopCount, INT *downCount, INT *scoreBuf, INT *currentTetmino, INT *minoIRoatationCount, INT *prevDeletedLineCount, INT *deletedLineCount, INT *lineCount, INT *additionalDeletableLine)
{
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
	g_hold = -1;
	g_next = rand() % 7;
	g_nextNext = rand() % 7;
	*currentTetmino = rand() % 7;
	*minoIRoatationCount = 0;
	*prevDeletedLineCount = 0;
	g_deletedLine = false;
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

				if (g_tetlisBoardBuf[column][row] % 100 >= 21)
				{
					if (g_tetlisBoardBuf[column][row] % 100 == 30)
					{
						if (isFirstDeletedLine)
						{
							firstDeletedColumn = column;
							isFirstDeletedLine = false;
						}

						g_tetlisBoard[column][row] = -1;
						g_reduceBlockDurPosition[column][row] = 1;
					}

					else
					{
						switch (*lineCount)
						{
						case 1:
							switch (g_tetlisBoard[column][row] % 100)
							{
							case 21:
								g_tetlisBoard[column][row] -= 1;
								g_durableBlockBeared[column][row] = 1;
								g_reduceBlockDurPosition[column][row] = 1;
								break;
							case 32:
								g_tetlisBoard[column][row] -= 1;
								g_durableBlockBeared[column][row] = 1;
								g_reduceBlockDurPosition[column][row] = 1;
								break;
							case 31:
								g_tetlisBoard[column][row] -= 1;
								g_durableBlockBeared[column][row] = 1;
								g_reduceBlockDurPosition[column][row] = 1;
								break;
							}

							break;
						case 2:
							switch (g_tetlisBoard[column][row] % 100)
							{
							case 21:
								g_tetlisBoard[column][row] = -1;
								g_reduceBlockDurPosition[column][row] = 1;
								break;
							case 32:
								g_tetlisBoard[column][row] -= 2;
								g_durableBlockBeared[column][row] = 1;
								g_reduceBlockDurPosition[column][row] = 1;
								break;
							case 31:
								g_tetlisBoard[column][row] = -1;
								g_reduceBlockDurPosition[column][row] = 1;
								break;
							}

							break;
						case 3:
							g_tetlisBoard[column][row] = -1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						case 4:
							g_tetlisBoard[column][row] = -1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						}
					}
				}

				else
				{
					if (isFirstDeletedLine)
					{
						firstDeletedColumn = column;
						isFirstDeletedLine = false;
					}

					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;
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
		for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
		{
			if (g_tetlisBoardBuf[column][row] % 100 >= 21)
			{
				if (g_tetlisBoardBuf[column][row] % 100 == 30)
				{
					g_tetlisBoard[column][row] = -1;
					g_reduceBlockDurPosition[column][row] = 1;
				}

				else
				{
					switch (*lineCount)
					{
					case 1:
						switch (g_tetlisBoard[column][row] % 100)
						{
						case 21:
							g_tetlisBoard[column][row] -= 1;
							g_durableBlockBeared[column][row] = 1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						case 32:
							g_tetlisBoard[column][row] -= 1;
							g_durableBlockBeared[column][row] = 1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						case 31:
							g_tetlisBoard[column][row] -= 1;
							g_durableBlockBeared[column][row] = 1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						}

						break;
					case 2:
						switch (g_tetlisBoard[column][row] % 100)
						{
						case 21:
							g_tetlisBoard[column][row] = -1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						case 32:
							g_tetlisBoard[column][row] -= 2;
							g_durableBlockBeared[column][row] = 1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						case 31:
							g_tetlisBoard[column][row]  = -1;
							g_reduceBlockDurPosition[column][row] = 1;
							break;
						}

						break;
					case 3:
						g_tetlisBoard[column][row] = -1;
						g_reduceBlockDurPosition[column][row] = 1;
						break;
					case 4:
						g_tetlisBoard[column][row] = -1;
						g_reduceBlockDurPosition[column][row] = 1;
						break;
					}
				}
			}

			else
			{
				g_tetlisBoard[column][row] = -1;
				g_reduceBlockDurPosition[column][row] = 1;
			}
		}
	}

	SynchroTetlisBoardBufToTetlisBoard();

	return;
}

VOID ShiftTetlisLine(INT *lineCount, INT *prevDeletedLineCount, INT *additionalDeletableLine)
{
	for (*prevDeletedLineCount = g_deletedLineCount; (*prevDeletedLineCount) < g_deletedLineCount + (*lineCount) + (*additionalDeletableLine); *prevDeletedLineCount += 1)
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

	prevDeletedLineCount -= (*lineCount) + (*additionalDeletableLine);

	for (INT column = TETLIS_HEIGHT - 2; column > 3; column--)
	{
		for (INT row = 1; row < TETLIS_WIDTH - 1; row++)
		{
			////////////////////////////
			//�n�[�h�h���b�v�̏����𗘗p
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
		if (g_tetlisBoardBuf[4 + g_deletedLineCount][coordinateX] != -1)
		{
			*isGameover = true;
		}
	}

	return;
}
