/*
 * mygame.cpp: セ郎纗笴栏セōclassimplementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStageOne, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStageOne.
 *   2005-09-13
 *      Rewrite the codes for CBall and CLeader.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStageOne.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {

	int CGameState::stageOpened = 1;
	int CGameState::stageNow = 1;

/////////////////////////////////////////////////////////////////////////////
// CGameStateInit
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	ShowInitProgress(0);
	logo.LoadBitmap(MY_HOMEPAGE);
}

void CGameStateInit::OnBeginState()
{
	if (leaveInitCount == 0) {
		CAudio::Instance()->Load(AUDIO_CLICK, "sounds\\click.mp3");
		CAudio::Instance()->Load(AUDIO_BGM, "sounds\\bgm.mp3");
	}
	CAudio::Instance()->Play(AUDIO_BGM, true);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';

	if (nChar == KEY_ESC)
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x < 230 && point.x > 110 && point.y > 260 && point.y < 315) {
		GotoGameState(GAME_STATE_SELECT);
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		CAudio::Instance()->Stop(AUDIO_BGM);
		leaveInitCount++;
	}
}

void CGameStateInit::OnShow()
{
	logo.SetTopLeft(0, 0);
	logo.ShowBitmap();
}								

/////////////////////////////////////////////////////////////////////////////
//class Game Select
/////////////////////////////////////////////////////////////////////////////

CGameStateSelect::CGameStateSelect(CGame *g)
	: CGameState(g)
{
}

void CGameStateSelect::OnInit()
{
	ShowInitProgress(0);
	stageOneBg.LoadBitmap(STAGE_ONE_SL);
	stageTwoBg.LoadBitmap(STAGE_TWO_SL);
	
}

void CGameStateSelect::OnBeginState()
{
}

void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x < 68 && point.x > 24 && point.y > 137 && point.y < 180) {			//level one
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STAGE_ONE);
	}
	if (stageOpened == 2) {
		if (point.x < 130 && point.x > 84 && point.y > 137 && point.y < 180) {		//level two
			CAudio::Instance()->Play(AUDIO_CLICK, false);
			GotoGameState(GAME_STAGE_TWO);
		}
	}


	if (pow(pow(point.x - 52, 2) + pow(point.y - 526, 2), 0.5) < 24) {				//return to homepage
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_INIT);
	}
}

void CGameStateSelect::OnShow()	
{
	if (stageOpened == 1) {
		stageOneBg.ShowBitmap();
	}
	else if (stageOpened == 2) {
		stageTwoBg.ShowBitmap();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Game Over
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{

}

void CGameStateOver::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_CLEAR, false);
}

void CGameStateOver::OnInit()
{
	ShowInitProgress(66);
	ShowInitProgress(100);
	background.LoadBitmap(STAGE_ONE_CP);
	if (leaveOverCount == 0) {
		CAudio::Instance()->Load(AUDIO_CLEAR, "sounds\\clear.mp3");	// 破關聲音
	}
}

void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pow(pow(point.x - 168, 2) + pow(point.y - 369, 2), 0.5) < 34) {
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(stageNow + 2);			//to next level
		leaveOverCount++;
	}
	if (pow(pow(point.x - 239, 2) + pow(point.y - 369, 2), 0.5) < 24) {
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(stageNow + 1);			//replay this level
		leaveOverCount++;
	}
	if (pow(pow(point.x - 97, 2) + pow(point.y - 369, 2), 0.5) < 24) {
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_SELECT);
		leaveOverCount++;
	}

}



void CGameStateOver::OnShow()
{
	background.ShowBitmap();
}





/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏笴栏磅︽ン璶笴栏祘Α常硂柑
/////////////////////////////////////////////////////////////////////////////

CGameStageOne::CGameStageOne(CGame *g)
: CGameState(g), NUMBALLS(28), MOVE_COUNTER(20)
{
}

CGameStageOne::~CGameStageOne()
{
}

void CGameStageOne::OnBeginState()
{
	stageNow = 1;
	leader.Initialize(1);
	box.Initialize(1);
	gamemap.Initialize();
	background.SetTopLeft(0,0);
	moveCounter = MOVE_COUNTER;
	finishCounter = 40;

}

void CGameStageOne::OnMove()
{
	moveCounter--;
	if (moveCounter <= 0) {
		leader.SetMovingLeft(false);
		leader.SetMovingRight(false);
		leader.SetMovingUp(false);
		leader.SetMovingDown(false);
		box.SetMovingLeft(false);
		box.SetMovingRight(false);
		box.SetMovingUp(false);
		box.SetMovingDown(false);
		leader.SetStanding(true);
		//CAudio::Instance()->Stop(AUDIO_BOX);
	}
	leader.OnMove();
	
	if (leader.GetX1() - box.GetX2() < 5 && leader.GetX1() - box.GetX2() > -5 && leader.GetY1() - box.GetY1() < 5 && leader.GetY1() - box.GetY1() > -5) {	//box push left
		box.PushedLeft(true);
	}
	else {
		box.PushedLeft(false);
	}

	if (leader.GetX2() - box.GetX1() < 5 && leader.GetX2() - box.GetX1() > -5 && leader.GetY1() - box.GetY1() < 5 && leader.GetY1() - box.GetY1() > -5) {	//box push right
		box.PushedRight(true);
	}
	else {
		box.PushedRight(false);
	}

	if (leader.GetY2() - box.GetY1() < 5 && leader.GetY2() - box.GetY1() > -5 && leader.GetX1() - box.GetX1() < 5 && leader.GetX1() - box.GetX1() > -5) {	//box push down
		box.PushedDown(true);
	}
	else {
		box.PushedDown(false);
	}

	if (leader.GetY1() - box.GetY2() < 5 && leader.GetY1() - box.GetY2() > -5 && leader.GetX1() - box.GetX1() < 5 && leader.GetX1() - box.GetX1() > -5) {	//box push up
		box.PushedUp(true);
	}
	else {
		box.PushedUp(false);
	}
	box.OnMove();

	if (gamemap.IsFinish()) {
		finishCounter--;
		if (finishCounter <= 0) {
			if (stageOpened <= 2) {	//待修改，需要全域變數!!!!
				stageOpened = 2;
			}
			GotoGameState(GAME_STATE_OVER);
		}
	}

}

void CGameStageOne::OnInit()
{
	ShowInitProgress(33);


	leader.LoadBitmap();
	box.LoadBitmap();
	background.LoadBitmap(MY_STAGE_ONE);

	ShowInitProgress(50);
	Sleep(300);


	CAudio::Instance()->Load(AUDIO_BOX,    "sounds\\box_long.mp3");		// 箱子移動聲音
	
}

void CGameStageOne::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; 
	const char KEY_UP    = 0x26; 
	const char KEY_RIGHT = 0x27; 
	const char KEY_DOWN  = 0x28; 
	if (nChar == KEY_LEFT) {
		if (leader.GetStatus()) {
			if (gamemap.MoveLeft()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingLeft(true);
				leader.SetStanding(false);
				if (box.CanPushLeft()) {
					box.SetMovingLeft(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
	if (nChar == KEY_RIGHT) {
		if (leader.GetStatus()) {
			if (gamemap.MoveRight()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingRight(true);
				leader.SetStanding(false);
				if (box.CanPushRight()) {
					box.SetMovingRight(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
	if (nChar == KEY_UP) {
		if (leader.GetStatus()) {
			if (gamemap.MoveUp()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingUp(true);
				leader.SetStanding(false);
				if (box.CanPushUp()) {
					box.SetMovingUp(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
	if (nChar == KEY_DOWN) {
		if (leader.GetStatus()) {
			if (gamemap.MoveDown()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingDown(true);
				leader.SetStanding(false);
				if (box.CanPushDown()) {
					box.SetMovingDown(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
}

void CGameStageOne::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25;
	const char KEY_UP    = 0x26; 
	const char KEY_RIGHT = 0x27;
	const char KEY_DOWN  = 0x28;
	if (nChar == KEY_LEFT) {
		
	}
	if (nChar == KEY_RIGHT) {

	}
	if (nChar == KEY_UP) {

	}
	if (nChar == KEY_DOWN) {

	}
}

void CGameStageOne::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (pow(pow(point.x - 168, 2) + pow(point.y - 545, 2), 0.5) < 25) {
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_SELECT);
	}

}

void CGameStageOne::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CGameStageOne::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CGameStageOne::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStageOne::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CGameStageOne::OnShow()
{
	background.SetTopLeft(0, 0);
	background.ShowBitmap();
	leader.OnShow();
	box.OnShow();

}

/////////////////////////////////////////////////
//Game Stage Two
/////////////////////////////////////////////////

CGameStageTwo::CGameStageTwo(CGame *g)
	: CGameState(g), NUMBALLS(28), MOVE_COUNTER(20)
{
}

CGameStageTwo::~CGameStageTwo()
{
}

void CGameStageTwo::OnBeginState()
{
	leader.Initialize(2);
	box.Initialize(2);
	gamemap.Initialize();
	background.SetTopLeft(0, 0);
	moveCounter = MOVE_COUNTER;
	finishCounter = 40;

}

void CGameStageTwo::OnMove()
{
	moveCounter--;
	if (moveCounter <= 0) {
		leader.SetMovingLeft(false);
		leader.SetMovingRight(false);
		leader.SetMovingUp(false);
		leader.SetMovingDown(false);
		box.SetMovingLeft(false);
		box.SetMovingRight(false);
		box.SetMovingUp(false);
		box.SetMovingDown(false);
		leader.SetStanding(true);
		//CAudio::Instance()->Stop(AUDIO_BOX);
	}
	leader.OnMove();

	if (leader.GetX1() - box.GetX2() < 5 && leader.GetX1() - box.GetX2() > -5 && leader.GetY1() - box.GetY1() < 5 && leader.GetY1() - box.GetY1() > -5) {	//box push left
		box.PushedLeft(true);
	}
	else {
		box.PushedLeft(false);
	}

	if (leader.GetX2() - box.GetX1() < 5 && leader.GetX2() - box.GetX1() > -5 && leader.GetY1() - box.GetY1() < 5 && leader.GetY1() - box.GetY1() > -5) {	//box push right
		box.PushedRight(true);
	}
	else {
		box.PushedRight(false);
	}

	if (leader.GetY2() - box.GetY1() < 5 && leader.GetY2() - box.GetY1() > -5 && leader.GetX1() - box.GetX1() < 5 && leader.GetX1() - box.GetX1() > -5) {	//box push down
		box.PushedDown(true);
	}
	else {
		box.PushedDown(false);
	}

	if (leader.GetY1() - box.GetY2() < 5 && leader.GetY1() - box.GetY2() > -5 && leader.GetX1() - box.GetX1() < 5 && leader.GetX1() - box.GetX1() > -5) {	//box push up
		box.PushedUp(true);
	}
	else {
		box.PushedUp(false);
	}
	box.OnMove();

	if (gamemap.IsFinish()) {
		finishCounter--;
		if (finishCounter <= 0) {
			//if (stageOpened <= 3) {	//待修改，需要全域變數!!!!
			//	stageOpened = 3;
			//}
			GotoGameState(GAME_STATE_OVER);
		}
	}

}

void CGameStageTwo::OnInit()
{
	ShowInitProgress(33);


	leader.LoadBitmap();
	box.LoadBitmap();
	background.LoadBitmap(MY_STAGE_TWO);

	ShowInitProgress(50);
	Sleep(300);


	//CAudio::Instance()->Load(AUDIO_BOX, "sounds\\box_long.mp3");		// 箱子移動聲音

}

void CGameStageTwo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25;
	const char KEY_UP = 0x26;
	const char KEY_RIGHT = 0x27;
	const char KEY_DOWN = 0x28;
	if (nChar == KEY_LEFT) {
		if (leader.GetStatus()) {
			if (gamemap.MoveLeft()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingLeft(true);
				leader.SetStanding(false);
				if (box.CanPushLeft()) {
					box.SetMovingLeft(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
	if (nChar == KEY_RIGHT) {
		if (leader.GetStatus()) {
			if (gamemap.MoveRight()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingRight(true);
				leader.SetStanding(false);
				if (box.CanPushRight()) {
					box.SetMovingRight(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
	if (nChar == KEY_UP) {
		if (leader.GetStatus()) {
			if (gamemap.MoveUp()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingUp(true);
				leader.SetStanding(false);
				if (box.CanPushUp()) {
					box.SetMovingUp(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
	if (nChar == KEY_DOWN) {
		if (leader.GetStatus()) {
			if (gamemap.MoveDown()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingDown(true);
				leader.SetStanding(false);
				if (box.CanPushDown()) {
					box.SetMovingDown(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
}

void CGameStageTwo::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25;
	const char KEY_UP = 0x26;
	const char KEY_RIGHT = 0x27;
	const char KEY_DOWN = 0x28;
	if (nChar == KEY_LEFT) {

	}
	if (nChar == KEY_RIGHT) {

	}
	if (nChar == KEY_UP) {

	}
	if (nChar == KEY_DOWN) {

	}
}

void CGameStageTwo::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pow(pow(point.x - 168, 2) + pow(point.y - 545, 2), 0.5) < 25) {
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_SELECT);
	}

}

void CGameStageTwo::OnShow()
{
	background.SetTopLeft(0, 0);
	background.ShowBitmap();
	leader.OnShow();
	box.OnShow();

}

}


