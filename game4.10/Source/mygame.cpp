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
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
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
 *      6. Add help bitmap to CGameStateRun.
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
		CAudio::Instance()->Load(AUDIO_CLICK, "sounds\\click.mp3");				//Load click sound
		CAudio::Instance()->Load(AUDIO_BGM, "sounds\\bgm.mp3");					//Load BGM
	}
	CAudio::Instance()->Play(AUDIO_BGM, true);									//Play BGM
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
	if (point.x < 230 && point.x > 110 && point.y > 260 && point.y < 315) {		//Play button
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
	stageThreeBg.LoadBitmap(STAGE_THREE_SL);
	stageFourBg.LoadBitmap(STAGE_FOUR_SL);
}

void CGameStateSelect::OnBeginState()
{
}

void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x < 68 && point.x > 24 && point.y > 137 && point.y < 180) {			//level one
		stageNow = 1;
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_RUN);
	}

	if (stageOpened >= 2)
		if (point.x < 130 && point.x > 84 && point.y > 137 && point.y < 180) {		//level two
			stageNow = 2;
			CAudio::Instance()->Play(AUDIO_CLICK, false);
			GotoGameState(GAME_STATE_RUN);
		}

	if (stageOpened >= 3)
		if (point.x < 189 && point.x > 145 && point.y > 137 && point.y < 180) {		//level three
			stageNow = 3;
			CAudio::Instance()->Play(AUDIO_CLICK, false);
			GotoGameState(GAME_STATE_RUN);
		}
	if (stageOpened >= 4)
		if (point.x < 250 && point.x > 208 && point.y > 137 && point.y < 180) {		//level four
			stageNow = 4;
			CAudio::Instance()->Play(AUDIO_CLICK, false);
			GotoGameState(GAME_STATE_RUN);
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
	else if (stageOpened == 3) {
		stageThreeBg.ShowBitmap();
	}
	else if (stageOpened == 4) {
		stageFourBg.ShowBitmap();
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

	stageOneCp.LoadBitmap(STAGE_ONE_CP);
	stageTwoCp.LoadBitmap(STAGE_TWO_CP);
	stageThreeCp.LoadBitmap(STAGE_THREE_CP);
	stageFourCp.LoadBitmap(STAGE_FOUR_CP);

	CAudio::Instance()->Load(AUDIO_CLEAR, "sounds\\clear.mp3");					// Load clear sound
}

void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pow(pow(point.x - 168, 2) + pow(point.y - 369, 2), 0.5) < 34) {			//to next level
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		stageNow++;
		GotoGameState(GAME_STATE_RUN);											
	}
	if (pow(pow(point.x - 239, 2) + pow(point.y - 369, 2), 0.5) < 24) {			//replay this level
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_RUN);			
	}
	if (pow(pow(point.x - 97, 2) + pow(point.y - 369, 2), 0.5) < 24) {			//select level
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_SELECT);		
	}

}



void CGameStateOver::OnShow()
{
	if(stageNow == 1)
		stageOneCp.ShowBitmap();
	if (stageNow == 2)
		stageTwoCp.ShowBitmap();
	if (stageNow == 3)
		stageThreeCp.ShowBitmap();
	if (stageNow == 4)
		stageFourCp.ShowBitmap();
}





/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏笴栏磅︽ン璶笴栏祘Α常硂柑
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), MOVE_COUNTER(20)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
	leader.Initialize(stageNow);
	box1.Initialize(stageNow, 1);	//box 1
	box2.Initialize(stageNow, 2);	//box 2
	box3.Initialize(stageNow, 3);	//box 3
	gamemap.Initialize(stageNow);

	//stageOneBg.SetTopLeft(0, 0);
	//stageTwoBg.SetTopLeft(0, 0);

	moveCounter = MOVE_COUNTER;
	finishCounter = 40;

}

void CGameStateRun::OnMove()
{
	moveCounter--;
	if (moveCounter <= 0) {
		leader.SetMovingLeft(false);		//leader stop moving
		leader.SetMovingRight(false);
		leader.SetMovingUp(false);
		leader.SetMovingDown(false);

		leader.SetStanding(true);

		box1.SetMovingLeft(false);			//boxes stop moving
		box1.SetMovingRight(false);
		box1.SetMovingUp(false);
		box1.SetMovingDown(false);

		box2.SetMovingLeft(false);
		box2.SetMovingRight(false);
		box2.SetMovingUp(false);
		box2.SetMovingDown(false);

		box3.SetMovingLeft(false);
		box3.SetMovingRight(false);
		box3.SetMovingUp(false);
		box3.SetMovingDown(false);
	}

	leader.OnMove();
	
	////////////////////////////////////////
	//box movement
	////////////////////////////////////////

	if (leader.GetX1() - box1.GetX2() < 5 && leader.GetX1() - box1.GetX2() > -5 && leader.GetY1() - box1.GetY1() < 5 && leader.GetY1() - box1.GetY1() > -5) {	//box1 push left
		box1.PushedLeft(true);
	}
	else {
		box1.PushedLeft(false);
	}

	if (leader.GetX2() - box1.GetX1() < 5 && leader.GetX2() - box1.GetX1() > -5 && leader.GetY1() - box1.GetY1() < 5 && leader.GetY1() - box1.GetY1() > -5) {	//box1 push right
		box1.PushedRight(true);
	}
	else {
		box1.PushedRight(false);
	}

	if (leader.GetY2() - box1.GetY1() < 5 && leader.GetY2() - box1.GetY1() > -5 && leader.GetX1() - box1.GetX1() < 5 && leader.GetX1() - box1.GetX1() > -5) {	//box1 push down
		box1.PushedDown(true);
	}
	else {
		box1.PushedDown(false);
	}

	if (leader.GetY1() - box1.GetY2() < 5 && leader.GetY1() - box1.GetY2() > -5 && leader.GetX1() - box1.GetX1() < 5 && leader.GetX1() - box1.GetX1() > -5) {	//box1 push up
		box1.PushedUp(true);
	}
	else {
		box1.PushedUp(false);
	}
	box1.OnMove();

	if (leader.GetX1() - box2.GetX2() < 5 && leader.GetX1() - box2.GetX2() > -5 && leader.GetY1() - box2.GetY1() < 5 && leader.GetY1() - box2.GetY1() > -5) {	//box2 push left
		box2.PushedLeft(true);
	}
	else {
		box2.PushedLeft(false);
	}

	if (leader.GetX2() - box2.GetX1() < 5 && leader.GetX2() - box2.GetX1() > -5 && leader.GetY1() - box2.GetY1() < 5 && leader.GetY1() - box2.GetY1() > -5) {	//box2 push right
		box2.PushedRight(true);
	}
	else {
		box2.PushedRight(false);
	}

	if (leader.GetY2() - box2.GetY1() < 5 && leader.GetY2() - box2.GetY1() > -5 && leader.GetX1() - box2.GetX1() < 5 && leader.GetX1() - box2.GetX1() > -5) {	//box2 push down
		box2.PushedDown(true);
	}
	else {
		box2.PushedDown(false);
	}

	if (leader.GetY1() - box2.GetY2() < 5 && leader.GetY1() - box2.GetY2() > -5 && leader.GetX1() - box2.GetX1() < 5 && leader.GetX1() - box2.GetX1() > -5) {	//box2 push up
		box2.PushedUp(true);
	}
	else {
		box2.PushedUp(false);
	}
	box2.OnMove();

	if (leader.GetX1() - box3.GetX2() < 5 && leader.GetX1() - box3.GetX2() > -5 && leader.GetY1() - box3.GetY1() < 5 && leader.GetY1() - box3.GetY1() > -5) {	//box3 push left
		box3.PushedLeft(true);
	}
	else {
		box3.PushedLeft(false);
	}

	if (leader.GetX2() - box3.GetX1() < 5 && leader.GetX2() - box3.GetX1() > -5 && leader.GetY1() - box3.GetY1() < 5 && leader.GetY1() - box3.GetY1() > -5) {	//box3 push right
		box3.PushedRight(true);
	}
	else {
		box3.PushedRight(false);
	}

	if (leader.GetY2() - box3.GetY1() < 5 && leader.GetY2() - box3.GetY1() > -5 && leader.GetX1() - box3.GetX1() < 5 && leader.GetX1() - box3.GetX1() > -5) {	//box3 push down
		box3.PushedDown(true);
	}
	else {
		box3.PushedDown(false);
	}

	if (leader.GetY1() - box3.GetY2() < 5 && leader.GetY1() - box3.GetY2() > -5 && leader.GetX1() - box3.GetX1() < 5 && leader.GetX1() - box3.GetX1() > -5) {	//box3 push up
		box3.PushedUp(true);
	}
	else {
		box3.PushedUp(false);
	}
	box3.OnMove();

	if (gamemap.IsFinish()) {					//if complete
		finishCounter--;						//不要馬上進到stateOver, 讓箱子跟leader動畫跑完
		if (finishCounter <= 0) {
			if (stageOpened <= stageNow) {
				stageOpened++;					//StageOpened 加一, 讓選擇關卡圖片切換成下一張
			}
			GotoGameState(GAME_STATE_OVER);
		}
	}

}

void CGameStateRun::OnInit()
{
	ShowInitProgress(33);

		leader.LoadBitmap();
		box1.LoadBitmap();
		box2.LoadBitmap();
		box3.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_BOX, "sounds\\box_long.mp3");		// Load箱子移動聲音
	
	stageOneBg.LoadBitmap(MY_STAGE_ONE);
	stageTwoBg.LoadBitmap(MY_STAGE_TWO);
	stageThreeBg.LoadBitmap(MY_STAGE_THREE);
	stageFourBg.LoadBitmap(MY_STAGE_FOUR);

	ShowInitProgress(50);
	Sleep(300);
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
				if (box1.CanPushLeft()) {
					box1.SetMovingLeft(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box2.CanPushLeft()) {
					box2.SetMovingLeft(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box3.CanPushLeft()) {
					box3.SetMovingLeft(true);
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
				if (box1.CanPushRight()) {
					box1.SetMovingRight(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box2.CanPushRight()) {
					box2.SetMovingRight(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box3.CanPushRight()) {
					box3.SetMovingRight(true);
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
				if (box1.CanPushUp()) {
					box1.SetMovingUp(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box2.CanPushUp()) {
					box2.SetMovingUp(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box3.CanPushUp()) {
					box3.SetMovingUp(true);
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
				if (box1.CanPushDown()) {
					box1.SetMovingDown(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box2.CanPushDown()) {
					box2.SetMovingDown(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				if (box3.CanPushDown()) {
					box3.SetMovingDown(true);
					CAudio::Instance()->Play(AUDIO_BOX, false);
				}
			}
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
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

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (pow(pow(point.x - 168, 2) + pow(point.y - 545, 2), 0.5) < 25) {		//go to game state select
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_SELECT);
	}
	if (pow(pow(point.x - 289, 2) + pow(point.y - 546, 2), 0.5) < 23) {		//restart level
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_RUN);
	}

}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CGameStateRun::OnShow()
{
	if (stageNow == 1) {
		stageOneBg.SetTopLeft(0, 0);
		stageOneBg.ShowBitmap();
	}
	if (stageNow == 2) {
		stageTwoBg.SetTopLeft(0, 0);
		stageTwoBg.ShowBitmap();
	}
	if (stageNow == 3) {
		stageThreeBg.SetTopLeft(0, 0);
		stageThreeBg.ShowBitmap();
	}
	if (stageNow == 4) {
		stageFourBg.SetTopLeft(0, 0);
		stageFourBg.ShowBitmap();
	}
	leader.OnShow();
	box1.OnShow();
	box2.OnShow();
	box3.OnShow();

}

}


