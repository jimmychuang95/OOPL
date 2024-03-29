﻿/*
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

	int		CGameState::stageOpened = 1;
	int		CGameState::stageNow = 1;
	bool	CGameState::hackEnable = false;
	bool	CGameState::muteSound = false;

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
	line.LoadBitmap(RED_LINE, RGB(255, 255, 255));
	info.LoadBitmap(INFO, RGB(255, 0, 0));
}

void CGameStateInit::OnBeginState()
{
	showInfo = false;
	if (leaveInitCount == 0) {
		CAudio::Instance()->Load(AUDIO_CLICK, "sounds\\click.mp3");				//Load click sound
		CAudio::Instance()->Load(AUDIO_BGM, "sounds\\bgm.mp3");					//Load BGM
	}
	if(!muteBGM)
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
	if (!showInfo) {
		if (pow(pow(point.x - 221, 2) + pow(point.y - 396, 2), 0.5) < 23) {			//hack enable (open all level) show info
			showInfo = true;
			hackEnable = true;
			stageOpened = 99999;
			if (!muteSound)
				CAudio::Instance()->Play(AUDIO_CLICK, false);
		}

		if (point.x < 230 && point.x > 110 && point.y > 260 && point.y < 315) {		//Play button
			GotoGameState(GAME_STATE_SELECT);
			if (!muteSound)
				CAudio::Instance()->Play(AUDIO_CLICK, false);

			CAudio::Instance()->Stop(AUDIO_BGM);
			leaveInitCount++;
		}

		if (!muteBGM) {
			if (pow(pow(point.x - 116, 2) + pow(point.y - 396, 2), 0.5) < 23) {			//mute BGM
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);

				muteBGM = true;
				CAudio::Instance()->Stop(AUDIO_BGM);
			}
		}
		else {
			if (pow(pow(point.x - 116, 2) + pow(point.y - 396, 2), 0.5) < 23) {			//unmute BGM
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);

				muteBGM = false;
				CAudio::Instance()->Play(AUDIO_BGM, true);
			}
		}

		if (!muteSound) {
			if (pow(pow(point.x - 168, 2) + pow(point.y - 396, 2), 0.5) < 23) {			//mute sound effects
				CAudio::Instance()->Play(AUDIO_CLICK, false);
				muteSound = true;
				//muteBGM = true;
				//CAudio::Instance()->Stop(AUDIO_BGM);
			}
		}
		else {
			if (pow(pow(point.x - 168, 2) + pow(point.y - 396, 2), 0.5) < 23) {			//unmute sound effects
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);

				muteSound = false;
				//muteBGM = false;
				//CAudio::Instance()->Play(AUDIO_BGM, true);
			}
		}
	}
	if (showInfo) {
		if (point.x < 294 && point.x > 264 && point.y > 160 && point.y < 190) {		//Close info
			showInfo = false;
			if (!muteSound)
				CAudio::Instance()->Play(AUDIO_CLICK, false);
		}
	}
}

void CGameStateInit::OnShow()
{
	logo.SetTopLeft(0, 0);
	logo.ShowBitmap();

	if (muteBGM) {				//如果靜音顯示紅色斜線
		line.SetTopLeft(95, 374);
		line.ShowBitmap();
	}
	if (muteSound) {
		line.SetTopLeft(147, 374);
		line.ShowBitmap();
	}
	if (showInfo) {
		info.SetTopLeft(50, 149);
		info.ShowBitmap();
	}
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
	selectBg.LoadBitmap(SELECT_BG);
	selectAllLock.LoadBitmap(SELECT_ALL_LOCK, RGB(255, 255, 255));
	stageOneBg.LoadBitmap(STAGE_ONE_SL, RGB(255, 255, 255));
	stageTwoBg.LoadBitmap(STAGE_TWO_SL, RGB(255, 255, 255));
	stageThreeBg.LoadBitmap(STAGE_THREE_SL, RGB(255, 255, 255));
	stageFourBg.LoadBitmap(STAGE_FOUR_SL, RGB(255, 255, 255));
	stageFiveBg.LoadBitmap(STAGE_FIVE_SL, RGB(255, 255, 255));
	stageSixBg.LoadBitmap(STAGE_SIX_SL, RGB(255, 255, 255));
	stageSevenBg.LoadBitmap(STAGE_SEVEN_SL, RGB(255, 255, 255));
	stageEightBg.LoadBitmap(STAGE_EIGHT_SL, RGB(255, 255, 255));
	stageNineBg.LoadBitmap(STAGE_NINE_SL, RGB(255, 255, 255));
	stageTenBg.LoadBitmap(STAGE_TEN_SL, RGB(255, 255, 255));
	stageElevenBg.LoadBitmap(STAGE_ELEVEN_SL, RGB(255, 255, 255));
	stageTwelveBg.LoadBitmap(STAGE_TWELVE_SL, RGB(255, 255, 255));
	stageThirteenBg.LoadBitmap(STAGE_THIRTEEN_SL, RGB(255, 255, 255));
	stageFourteenBg.LoadBitmap(STAGE_FOURTEEN_SL, RGB(255, 255, 255));
	stageFifteenBg.LoadBitmap(STAGE_FIFTEEN_SL, RGB(255, 255, 255));
	stageSixteenBg.LoadBitmap(STAGE_SIXTEEN_SL, RGB(255, 255, 255));
	stageSeventeenBg.LoadBitmap(STAGE_SEVENTEEN_SL, RGB(255, 255, 255));
	stageEighteenBg.LoadBitmap(STAGE_EIGHTEEN_SL, RGB(255, 255, 255));
	stageNineteenBg.LoadBitmap(STAGE_NINETEEN_SL, RGB(255, 255, 255));
	stageTwentyBg.LoadBitmap(STAGE_TWENTY_SL, RGB(255, 255, 255));
	stageTwentyOneBg.LoadBitmap(STAGE_TWENTYONE_SL, RGB(255, 255, 255));
	stageTwentyTwoBg.LoadBitmap(STAGE_TWENTYTWO_SL, RGB(255, 255, 255));
	stageTwentyThreeBg.LoadBitmap(STAGE_TWENTYTHREE_SL, RGB(255, 255, 255));
	stageTwentyFourBg.LoadBitmap(STAGE_TWENTYFOUR_SL, RGB(255, 255, 255));
	stageTwentyFiveBg.LoadBitmap(STAGE_TWENTYFIVE_SL, RGB(255, 255, 255));
	stageTwentySixBg.LoadBitmap(STAGE_TWENTYSIX_SL, RGB(255, 255, 255));
	stageTwentySevenBg.LoadBitmap(STAGE_TWENTYSEVEN_SL, RGB(255, 255, 255));
	stageTwentyEightBg.LoadBitmap(STAGE_TWENTYEIGHT_SL, RGB(255, 255, 255));
	stageTwentyNineBg.LoadBitmap(STAGE_TWENTYNINE_SL, RGB(255, 255, 255));
	stageThirtyBg.LoadBitmap(STAGE_THIRTY_SL, RGB(255, 255, 255));

}

void CGameStateSelect::OnBeginState()
{
}

void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (page == 1) {
		if (point.x < 68 && point.x > 24 && point.y > 137 && point.y < 180) {			//level one
			stageNow = 1;
			if (!muteSound)
				CAudio::Instance()->Play(AUDIO_CLICK, false);
			GotoGameState(GAME_STATE_RUN);
		}

		if (stageOpened >= 2 || hackEnable)
			if (point.x < 130 && point.x > 84 && point.y > 137 && point.y < 180) {		//level two
				stageNow = 2;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}

		if (stageOpened >= 3 || hackEnable)
			if (point.x < 189 && point.x > 145 && point.y > 137 && point.y < 180) {		//level three
				stageNow = 3;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 4 || hackEnable)
			if (point.x < 250 && point.x > 208 && point.y > 137 && point.y < 180) {		//level four
				stageNow = 4;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 5 || hackEnable)
			if (point.x < 313 && point.x > 267 && point.y > 137 && point.y < 180) {		//level five
				stageNow = 5;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 6 || hackEnable)
			if (point.x < 68 && point.x > 24 && point.y > 199 && point.y < 242) {		//level six
				stageNow = 6;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 7 || hackEnable)
			if (point.x < 130 && point.x > 84 && point.y > 199 && point.y < 242) {		//level seven
				stageNow = 7;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 8 || hackEnable)
			if (point.x < 189 && point.x > 145 && point.y > 199 && point.y < 242) {		//level eight
				stageNow = 8;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 9 || hackEnable)
			if (point.x < 250 && point.x > 207 && point.y > 199 && point.y < 242) {		//level nine
				stageNow = 9;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 10 || hackEnable)
			if (point.x < 313 && point.x > 267 && point.y > 199 && point.y < 242) {		//level ten
				stageNow = 10;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 11 || hackEnable)
			if (point.x < 68 && point.x > 24 && point.y > 258 && point.y < 303) {		//level 11
				stageNow = 11;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 12 || hackEnable)
			if (point.x < 130 && point.x > 84 && point.y > 258 && point.y < 303) {		//level 12
				stageNow = 12;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 13 || hackEnable)
			if (point.x < 189 && point.x > 145 && point.y > 258 && point.y < 303) {		//level 13
				stageNow = 13;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 14 || hackEnable)
			if (point.x < 250 && point.x > 207 && point.y > 258 && point.y < 303) {		//level 14
				stageNow = 14;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 15 || hackEnable)
			if (point.x < 313 && point.x > 267 && point.y > 258 && point.y < 303) {		//level 15
				stageNow = 15;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 16 || hackEnable)
			if (point.x < 68 && point.x > 24 && point.y > 318 && point.y < 363) {		//level 16
				stageNow = 16;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 17 || hackEnable)
			if (point.x < 129 && point.x > 85 && point.y > 318 && point.y < 363) {		//level 17
				stageNow = 17;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 18 || hackEnable)
			if (point.x < 190 && point.x > 146 && point.y > 318 && point.y < 363) {		//level 18
				stageNow = 18;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 19 || hackEnable)
			if (point.x < 249 && point.x > 207 && point.y > 320 && point.y < 363) {		//level 19
				stageNow = 19;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 20 || hackEnable)
			if (point.x < 311 && point.x > 268 && point.y > 320 && point.y < 363) {		//level 20
				stageNow = 20;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 21 || hackEnable)
			if (point.x < 67 && point.x > 25 && point.y > 382 && point.y < 424) {		//level 21
				stageNow = 21;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 22 || hackEnable)
			if (point.x < 128 && point.x > 86 && point.y > 382 && point.y < 424) {		//level 22
				stageNow = 22;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 23 || hackEnable)
			if (point.x < 189 && point.x > 147 && point.y > 382 && point.y < 424) {		//level 23
				stageNow = 23;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 24 || hackEnable)
			if (point.x < 249 && point.x > 207 && point.y > 382 && point.y < 424) {		//level 24
				stageNow = 24;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 25 || hackEnable)
			if (point.x < 311 && point.x > 268 && point.y > 382 && point.y < 424) {		//level 25
				stageNow = 25;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 26 || hackEnable)
			if (point.x < 67 && point.x > 25 && point.y > 442 && point.y < 484) {		//level 26
				stageNow = 26;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 27 || hackEnable)
			if (point.x < 128 && point.x > 86 && point.y > 442 && point.y < 484) {		//level 27
				stageNow = 27;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 28 || hackEnable)
			if (point.x < 190 && point.x > 147 && point.y > 442 && point.y < 484) {		//level 28
				stageNow = 28;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
		if (stageOpened >= 29 || hackEnable)
			if (point.x < 249 && point.x > 207 && point.y > 442 && point.y < 484) {		//level 29
				stageNow = 29;
				if (!muteSound)
					CAudio::Instance()->Play(AUDIO_CLICK, false);
				GotoGameState(GAME_STATE_RUN);
			}
	}



	if (pow(pow(point.x - 52, 2) + pow(point.y - 526, 2), 0.5) < 24) {				//return to homepage
		if (!muteSound)
			CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_INIT);
	}
}

void CGameStateSelect::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25;
	const char KEY_RIGHT = 0x27;

	if (NotMoving) {
		if (page != 1) {
			if (nChar == KEY_LEFT) {
				page--;
				moveCounter = 56;
				isMovingRight = true;
				NotMoving = false;
			}
		}
		if (page != 3) {
			if (nChar == KEY_RIGHT) {
				page++;
				moveCounter = 56;
				isMovingLeft = true;
				NotMoving = false;
			}
		}
	}
}

void CGameStateSelect::OnMove()
{
	moveCounter--;
	if (moveCounter < 0) {
		NotMoving = true;
		isMovingLeft = false;
		isMovingRight = false;
	}

	if (isMovingLeft == true)
		x -= 6;

	if (isMovingRight == true)
		x += 6;
}

void CGameStateSelect::OnShow()	
{
	selectBg.SetTopLeft(0, 0);
	selectBg.ShowBitmap();

	if (stageOpened <= 30) {
		selectAllLock.SetTopLeft(x + 338, 0);
		selectAllLock.ShowBitmap();
	}

	if (stageOpened == 99999) {
		stageTwentyNineBg.SetTopLeft(x, 0);
		stageTwentyNineBg.ShowBitmap();
	}
	else if (stageOpened == 1) {
		stageOneBg.SetTopLeft(x, 0);
		stageOneBg.ShowBitmap();
	}
	else if (stageOpened == 2) {
		stageTwoBg.SetTopLeft(x, 0);
		stageTwoBg.ShowBitmap();
	}
	else if (stageOpened == 3) {
		stageThreeBg.SetTopLeft(x, 0);
		stageThreeBg.ShowBitmap();
	}
	else if (stageOpened == 4) {
		stageFourBg.SetTopLeft(x, 0);
		stageFourBg.ShowBitmap();
	}
	else if (stageOpened == 5) {
		stageFiveBg.SetTopLeft(x, 0);
		stageFiveBg.ShowBitmap();
	}
	else if (stageOpened == 6) {
		stageSixBg.SetTopLeft(x, 0);
		stageSixBg.ShowBitmap();
	}
	else if (stageOpened == 7) {
		stageSevenBg.SetTopLeft(x, 0);
		stageSevenBg.ShowBitmap();
	}
	else if (stageOpened == 8) {
		stageEightBg.SetTopLeft(x, 0);
		stageEightBg.ShowBitmap();
	}
	else if (stageOpened == 9) {
		stageNineBg.SetTopLeft(x, 0);
		stageNineBg.ShowBitmap();
	}
	else if (stageOpened == 10) {
		stageTenBg.SetTopLeft(x, 0);
		stageTenBg.ShowBitmap();
	}
	else if (stageOpened == 11) {
		stageElevenBg.SetTopLeft(x, 0);
		stageElevenBg.ShowBitmap();
	}
	else if (stageOpened == 12) {
		stageTwelveBg.SetTopLeft(x, 0);
		stageTwelveBg.ShowBitmap();
	}
	else if (stageOpened == 13) {
		stageThirteenBg.SetTopLeft(x, 0);
		stageThirteenBg.ShowBitmap();
	}
	else if (stageOpened == 14) {
		stageFourteenBg.SetTopLeft(x, 0);
		stageFourteenBg.ShowBitmap();
	}
	else if (stageOpened == 15) {
		stageFifteenBg.SetTopLeft(x, 0);
		stageFifteenBg.ShowBitmap();
	}
	else if (stageOpened == 16) {
		stageSixteenBg.SetTopLeft(x, 0);
		stageSixteenBg.ShowBitmap();
	}
	else if (stageOpened == 17) {
		stageSeventeenBg.SetTopLeft(x, 0);
		stageSeventeenBg.ShowBitmap();
	}
	else if (stageOpened == 18) {
		stageEighteenBg.SetTopLeft(x, 0);
		stageEighteenBg.ShowBitmap();
	}
	else if (stageOpened == 19) {
		stageNineteenBg.SetTopLeft(x, 0);
		stageNineteenBg.ShowBitmap();
	}
	else if (stageOpened == 20) {
		stageTwentyBg.SetTopLeft(x, 0);
		stageTwentyBg.ShowBitmap();
	}
	else if (stageOpened == 21) {
		stageTwentyOneBg.SetTopLeft(x, 0);
		stageTwentyOneBg.ShowBitmap();
	}
	else if (stageOpened == 22) {
		stageTwentyTwoBg.SetTopLeft(x, 0);
		stageTwentyTwoBg.ShowBitmap();
	}
	else if (stageOpened == 23) {
		stageTwentyThreeBg.SetTopLeft(x, 0);
		stageTwentyThreeBg.ShowBitmap();
	}
	else if (stageOpened == 24) {
		stageTwentyFourBg.SetTopLeft(x, 0);
		stageTwentyFourBg.ShowBitmap();
	}
	else if (stageOpened == 25) {
		stageTwentyFiveBg.SetTopLeft(x, 0);
		stageTwentyFiveBg.ShowBitmap();
	}
	else if (stageOpened == 26) {
		stageTwentySixBg.SetTopLeft(x, 0);
		stageTwentySixBg.ShowBitmap();
	}
	else if (stageOpened == 27) {
		stageTwentySevenBg.SetTopLeft(x, 0);
		stageTwentySevenBg.ShowBitmap();
	}
	else if (stageOpened == 28) {
		stageTwentyEightBg.SetTopLeft(x, 0);
		stageTwentyEightBg.ShowBitmap();
	}
	else if (stageOpened == 29) {
		stageTwentyNineBg.SetTopLeft(x, 0);
		stageTwentyNineBg.ShowBitmap();
	}
	else if (stageOpened == 30) {
		stageThirtyBg.SetTopLeft(x, 0);
		stageThirtyBg.ShowBitmap();
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
	if (!muteSound)
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
	stageFiveCp.LoadBitmap(STAGE_FIVE_CP);
	stageSixCp.LoadBitmap(STAGE_SIX_CP);
	stageSevenCp.LoadBitmap(STAGE_SEVEN_CP);
	stageEightCp.LoadBitmap(STAGE_EIGHT_CP);
	stageNineCp.LoadBitmap(STAGE_NINE_CP);
	stageTenCp.LoadBitmap(STAGE_TEN_CP);
	stageElevenCp.LoadBitmap(STAGE_ELEVEN_CP);
	stageTwelveCp.LoadBitmap(STAGE_TWELVE_CP);
	stageThirteenCp.LoadBitmap(STAGE_THIRTEEN_CP);
	stageFourteenCp.LoadBitmap(STAGE_FOURTEEN_CP);
	stageFifteenCp.LoadBitmap(STAGE_FIFTEEN_CP);
	stageSixteenCp.LoadBitmap(STAGE_SIXTEEN_CP);
	stageSeventeenCp.LoadBitmap(STAGE_SEVENTEEN_CP);
	stageEighteenCp.LoadBitmap(STAGE_EIGHTEEN_CP);
	stageNineteenCp.LoadBitmap(STAGE_NINETEEN_CP);
	stageTwentyCp.LoadBitmap(STAGE_TWENTY_CP);
	stageTwentyOneCp.LoadBitmap(STAGE_TWENTYONE_CP);
	stageTwentyTwoCp.LoadBitmap(STAGE_TWENTYTWO_CP);
	stageTwentyThreeCp.LoadBitmap(STAGE_TWENTYTHREE_CP);
	stageTwentyFourCp.LoadBitmap(STAGE_TWENTYFOUR_CP);
	stageTwentyFiveCp.LoadBitmap(STAGE_TWENTYFIVE_CP);
	stageTwentySixCp.LoadBitmap(STAGE_TWENTYSIX_CP);
	stageTwentySevenCp.LoadBitmap(STAGE_TWENTYSEVEN_CP);
	stageTwentyEightCp.LoadBitmap(STAGE_TWENTYEIGHT_CP);
	stageTwentyNineCp.LoadBitmap(STAGE_TWENTYNINE_CP);

	CAudio::Instance()->Load(AUDIO_CLEAR, "sounds\\clear.mp3");					// Load clear sound
}

void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (stageNow < 29) {
		if (pow(pow(point.x - 168, 2) + pow(point.y - 369, 2), 0.5) < 34) {			//to next level
			if (!muteSound)
				CAudio::Instance()->Play(AUDIO_CLICK, false);
			stageNow++;
			GotoGameState(GAME_STATE_RUN);
		}
	}
	if (pow(pow(point.x - 239, 2) + pow(point.y - 369, 2), 0.5) < 24) {			//replay this level
		if (!muteSound)
			CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_RUN);			
	}
	if (pow(pow(point.x - 97, 2) + pow(point.y - 369, 2), 0.5) < 24) {			//select level
		if (!muteSound)
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
	if (stageNow == 5)
		stageFiveCp.ShowBitmap();
	if (stageNow == 6)
		stageSixCp.ShowBitmap();
	if (stageNow == 7)
		stageSevenCp.ShowBitmap();
	if (stageNow == 8)
		stageEightCp.ShowBitmap();
	if (stageNow == 9)
		stageNineCp.ShowBitmap();
	if (stageNow == 10)
		stageTenCp.ShowBitmap();
	if (stageNow == 11)
		stageElevenCp.ShowBitmap();
	if (stageNow == 12)
		stageTwelveCp.ShowBitmap();
	if (stageNow == 13)
		stageThirteenCp.ShowBitmap();
	if (stageNow == 14)
		stageFourteenCp.ShowBitmap();
	if (stageNow == 15)
		stageFifteenCp.ShowBitmap();
	if (stageNow == 16)
		stageSixteenCp.ShowBitmap();
	if (stageNow == 17)
		stageSeventeenCp.ShowBitmap();
	if (stageNow == 18)
		stageEighteenCp.ShowBitmap();
	if (stageNow == 19)
		stageNineteenCp.ShowBitmap();
	if (stageNow == 20)
		stageTwentyCp.ShowBitmap();
	if (stageNow == 21)
		stageTwentyOneCp.ShowBitmap();
	if (stageNow == 22)
		stageTwentyTwoCp.ShowBitmap();
	if (stageNow == 23)
		stageTwentyThreeCp.ShowBitmap();
	if (stageNow == 24)
		stageTwentyFourCp.ShowBitmap();
	if (stageNow == 25)
		stageTwentyFiveCp.ShowBitmap();
	if (stageNow == 26)
		stageTwentySixCp.ShowBitmap();
	if (stageNow == 27)
		stageTwentySevenCp.ShowBitmap();
	if (stageNow == 28)
		stageTwentyEightCp.ShowBitmap();
	if (stageNow == 29)
		stageTwentyNineCp.ShowBitmap();

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
	for (int i = 0; i < 6; i += 2) {
		finish_pos[i] = leader.GetX1() + gamemap.return_finish_dis(i+1) * 38;
		finish_pos[i + 1] = leader.GetY1() + gamemap.return_finish_dis(i) * 38;
	}
	


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
			if (stageOpened == 99999) {			//外掛開啟時不動作
				;
			}
			else if (stageOpened <= stageNow) {
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
		line.LoadBitmap(RED_LINE);
		box_check.LoadBitmap(MY_BOX_CHECK);
		CAudio::Instance()->Load(AUDIO_BOX, "sounds\\box_long.mp3");		// Load箱子移動聲音
	
	stageOneBg.LoadBitmap(MY_STAGE_ONE);
	stageTwoBg.LoadBitmap(MY_STAGE_TWO);
	stageThreeBg.LoadBitmap(MY_STAGE_THREE);
	stageFourBg.LoadBitmap(MY_STAGE_FOUR);
	stageFiveBg.LoadBitmap(MY_STAGE_FIVE);
	stageSixBg.LoadBitmap(MY_STAGE_SIX);
	stageSevenBg.LoadBitmap(MY_STAGE_SEVEN);
	stageEightBg.LoadBitmap(MY_STAGE_EIGHT);
	stageNineBg.LoadBitmap(MY_STAGE_NINE);
	stageTenBg.LoadBitmap(MY_STAGE_TEN);
	stageElevenBg.LoadBitmap(MY_STAGE_ELEVEN);
	stageTwelveBg.LoadBitmap(MY_STAGE_TWELVE);
	stageThirteenBg.LoadBitmap(MY_STAGE_THIRTEEN);
	stageFourteenBg.LoadBitmap(MY_STAGE_FOURTEEN);
	stageFifteenBg.LoadBitmap(MY_STAGE_FIFTEEN);
	stageSixteenBg.LoadBitmap(MY_STAGE_SIXTEEN);
	stageSeventeenBg.LoadBitmap(MY_STAGE_SEVENTEEN);
	stageEighteenBg.LoadBitmap(MY_STAGE_EIGHTEEN);
	stageNineteenBg.LoadBitmap(MY_STAGE_NINETEEN);
	stageTwentyBg.LoadBitmap(MY_STAGE_TWENTY);
	stageTwentyOneBg.LoadBitmap(MY_STAGE_TWENTYONE);
	stageTwentyTwoBg.LoadBitmap(MY_STAGE_TWENTYTWO);
	stageTwentyThreeBg.LoadBitmap(MY_STAGE_TWENTYTHREE);
	stageTwentyFourBg.LoadBitmap(MY_STAGE_TWENTYFOUR);
	stageTwentyFiveBg.LoadBitmap(MY_STAGE_TWENTYFIVE);
	stageTwentySixBg.LoadBitmap(MY_STAGE_TWENTYSIX);
	stageTwentySevenBg.LoadBitmap(MY_STAGE_TWENTYSEVEN);
	stageTwentyEightBg.LoadBitmap(MY_STAGE_TWENTYEIGHT);
	stageTwentyNineBg.LoadBitmap(MY_STAGE_TWENTYNINE);


	console.LoadBitmap(MY_DIRECTION, RGB(255, 255, 255));
	rightCircle.LoadBitmap(RIGHT_CIRCLE);
	handCircle.LoadBitmap(HAND_CIRCLE);


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
				dirStack.push(2);
				if (box1.CanPushLeft()) {
					boxPushStack.push(true);
					box1.SetMovingLeft(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box2.CanPushLeft()) {
					boxPushStack.push(true);
					box2.SetMovingLeft(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box3.CanPushLeft()) {
					boxPushStack.push(true);
					box3.SetMovingLeft(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else {
					boxPushStack.push(false);
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
				dirStack.push(3);
				if (box1.CanPushRight()) {
					boxPushStack.push(true);
					box1.SetMovingRight(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box2.CanPushRight()) {
					boxPushStack.push(true);
					box2.SetMovingRight(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box3.CanPushRight()) {
					boxPushStack.push(true);
					box3.SetMovingRight(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else {
					boxPushStack.push(false);
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
				dirStack.push(0);
				if (box1.CanPushUp()) {
					boxPushStack.push(true);
					box1.SetMovingUp(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box2.CanPushUp()) {
					boxPushStack.push(true);
					box2.SetMovingUp(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box3.CanPushUp()) {
					boxPushStack.push(true);
					box3.SetMovingUp(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else {
					boxPushStack.push(false);
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
				dirStack.push(1);
				if (box1.CanPushDown()) {
					boxPushStack.push(true);
					box1.SetMovingDown(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box2.CanPushDown()) {
					boxPushStack.push(true);
					box2.SetMovingDown(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else if (box3.CanPushDown()) {
					boxPushStack.push(true);
					box3.SetMovingDown(true);
					if (!muteSound)
						CAudio::Instance()->Play(AUDIO_BOX, false);
				}
				else {
					boxPushStack.push(false);
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
		if (!muteSound)
			CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_SELECT);
	}

	if (pow(pow(point.x - 289, 2) + pow(point.y - 546, 2), 0.5) < 23) {		//restart level
		if (!muteSound)
			CAudio::Instance()->Play(AUDIO_CLICK, false);
		GotoGameState(GAME_STATE_RUN);
	}

	if (pow(pow(point.x - 104, 2) + pow(point.y - 546, 2), 0.5) < 23) {		//mute sound
		if (!muteSound) {
			muteSound = true;
			CAudio::Instance()->Play(AUDIO_CLICK, false);
		}
		else {
			muteSound = false;
		}
	}

	if (pow(pow(point.x - 48, 2) + pow(point.y - 546, 2), 0.5) < 23) {		//switch console
		if (!muteSound)
			CAudio::Instance()->Play(AUDIO_CLICK, false);

		switchmodecount++;
		hackCounter++;
		if (hackCounter >= 5) {
			gamemap.HackModeOn();
		}
	}

	if (switchmodecount % 3 == 1) {     //按鈕在左側的向上走
		if (point.x < 69 && point.x > 45 && point.y > 420 && point.y < 445) {
			if (leader.GetStatus()) {
				if (gamemap.MoveUp()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingUp(true);
					leader.SetStanding(false);
					dirStack.push(0);
					if (box1.CanPushUp()) {
						boxPushStack.push(true);
						box1.SetMovingUp(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushUp()) {
						boxPushStack.push(true);
						box2.SetMovingUp(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushUp()) {
						boxPushStack.push(true);
						box3.SetMovingUp(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}

	if (switchmodecount % 3 == 1) {     //按鈕在左側的向下走
		if (point.x < 69 && point.x > 45 && point.y > 470 && point.y < 494) {
			if (leader.GetStatus()) {
				if (gamemap.MoveDown()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingDown(true);
					leader.SetStanding(false);
					dirStack.push(1);
					if (box1.CanPushDown()) {
						boxPushStack.push(true);
						box1.SetMovingDown(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushDown()) {
						boxPushStack.push(true);
						box2.SetMovingDown(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushDown()) {
						boxPushStack.push(true);
						box3.SetMovingDown(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}

	if (switchmodecount % 3 == 1) {			//按鈕在左側的向左走
		if (point.x < 46 && point.x > 20 && point.y > 445 && point.y < 470) {
			if (leader.GetStatus()) {
				if (gamemap.MoveLeft()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingLeft(true);
					leader.SetStanding(false);
					dirStack.push(2);
					if (box1.CanPushLeft()) {
						boxPushStack.push(true);
						box1.SetMovingLeft(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushLeft()) {
						boxPushStack.push(true);
						box2.SetMovingLeft(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushLeft()) {
						boxPushStack.push(true);
						box3.SetMovingLeft(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}

	if (switchmodecount % 3 == 1) {			//按鈕在左側的向右走
		if (point.x < 94 && point.x > 69 && point.y > 445 && point.y < 470) {
			if (leader.GetStatus()) {
				if (gamemap.MoveRight()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingRight(true);
					leader.SetStanding(false);
					dirStack.push(3);
					if (box1.CanPushRight()) {
						boxPushStack.push(true);
						box1.SetMovingRight(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushRight()) {
						boxPushStack.push(true);
						box2.SetMovingRight(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushRight()) {
						boxPushStack.push(true);
						box3.SetMovingRight(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}


	if (switchmodecount % 3 == 2) {     //按鈕在右側的向上走
		if (point.x < 293  && point.x > 268 && point.y > 420 && point.y < 445) {
			if (leader.GetStatus()) {
				if (gamemap.MoveUp()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingUp(true);
					leader.SetStanding(false);
					dirStack.push(0);
					if (box1.CanPushUp()) {
						boxPushStack.push(true);
						box1.SetMovingUp(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushUp()) {
						boxPushStack.push(true);
						box2.SetMovingUp(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushUp()) {
						boxPushStack.push(true);
						box3.SetMovingUp(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}

	if (switchmodecount % 3 == 2) {     //按鈕在右側的向下走
		if (point.x < 293 && point.x > 268 && point.y > 470 && point.y < 494) {
			if (leader.GetStatus()) {
				if (gamemap.MoveDown()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingDown(true);
					leader.SetStanding(false);
					dirStack.push(1);
					if (box1.CanPushDown()) {
						boxPushStack.push(true);
						box1.SetMovingDown(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushDown()) {
						boxPushStack.push(true);
						box2.SetMovingDown(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushDown()) {
						boxPushStack.push(true);
						box3.SetMovingDown(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}

	if (switchmodecount % 3 == 2) {			//按鈕在右側的向左走
		if (point.x < 269 && point.x > 244 && point.y > 445 && point.y < 470) {
			if (leader.GetStatus()) {
				if (gamemap.MoveLeft()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingLeft(true);
					leader.SetStanding(false);
					dirStack.push(2);
					if (box1.CanPushLeft()) {
						boxPushStack.push(true);
						box1.SetMovingLeft(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushLeft()) {
						boxPushStack.push(true);
						box2.SetMovingLeft(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushLeft()) {
						boxPushStack.push(true);
						box3.SetMovingLeft(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}

	if (switchmodecount % 3 == 2) {			//按鈕在右側的向右走
		if (point.x < 369 && point.x > 244 && point.y > 445 && point.y < 470) {
			if (leader.GetStatus()) {
				if (gamemap.MoveRight()) {
					moveCounter = MOVE_COUNTER;
					leader.SetMovingRight(true);
					leader.SetStanding(false);
					dirStack.push(3);
					if (box1.CanPushRight()) {
						boxPushStack.push(true);
						box1.SetMovingRight(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box2.CanPushRight()) {
						boxPushStack.push(true);
						box2.SetMovingRight(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else if (box3.CanPushRight()) {
						boxPushStack.push(true);
						box3.SetMovingRight(true);
						if (!muteSound)
							CAudio::Instance()->Play(AUDIO_BOX, false);
					}
					else {
						boxPushStack.push(false);
					}
				}
			}
		}
	}



	if (pow(pow(point.x - 232, 2) + pow(point.y - 546, 2), 0.5) < 23) {		//previous step
		if (!muteSound)
			CAudio::Instance()->Play(AUDIO_CLICK, false);
		if ((!dirStack.empty()) && (!boxPushStack.empty())) {
			gamemap.PreviousStep(dirStack.top(), boxPushStack.top());			//change map data

			if (!boxPushStack.top()) {
				if (dirStack.top() == 0) {
					leader.SetXY(leader.GetX1(), leader.GetY1() + 38);
				}
				if (dirStack.top() == 1) {
					leader.SetXY(leader.GetX1(), leader.GetY1() - 38);
				}
				if (dirStack.top() == 2) {
					leader.SetXY(leader.GetX1() + 38, leader.GetY1());
				}
				if (dirStack.top() == 3) {
					leader.SetXY(leader.GetX1() - 38, leader.GetY1());
				}
			}
			else if (boxPushStack.top()) {
				if (dirStack.top() == 0) {
					leader.SetXY(leader.GetX1(), leader.GetY1() + 38);
					if (box1.CanPushUp()) {
						box1.SetXY(box1.GetX1(), box1.GetY1() + 38);
					}
					else if (box2.CanPushUp()) {
						box2.SetXY(box2.GetX1(), box2.GetY1() + 38);
					}
					else if (box3.CanPushUp()) {
						box3.SetXY(box3.GetX1(), box3.GetY1() + 38);
					}
				}
				if (dirStack.top() == 1) {
					leader.SetXY(leader.GetX1(), leader.GetY1() - 38);
					if (box1.CanPushDown()) {
						box1.SetXY(box1.GetX1(), box1.GetY1() - 38);
					}
					else if (box2.CanPushDown()) {
						box2.SetXY(box2.GetX1(), box2.GetY1() - 38);
					}
					else if (box3.CanPushDown()) {
						box3.SetXY(box3.GetX1(), box3.GetY1() - 38);
					}
				}
				if (dirStack.top() == 2) {
					leader.SetXY(leader.GetX1() + 38, leader.GetY1());
					if (box1.CanPushLeft()) {
						box1.SetXY(box1.GetX1() + 38, box1.GetY1());
					}
					else if (box2.CanPushLeft()) {
						box2.SetXY(box2.GetX1() + 38, box2.GetY1());
					}
					else if (box3.CanPushLeft()) {
						box3.SetXY(box3.GetX1() + 38, box3.GetY1());
					}
				}
				if (dirStack.top() == 3) {
					leader.SetXY(leader.GetX1() - 38, leader.GetY1());
					if (box1.CanPushRight()) {
						box1.SetXY(box1.GetX1() - 38, box1.GetY1());
					}
					else if (box2.CanPushRight()) {
						box2.SetXY(box2.GetX1() - 38, box2.GetY1());
					}
					else if (box3.CanPushRight()) {
						box3.SetXY(box3.GetX1() - 38, box3.GetY1());
					}
				}
			}
			dirStack.pop();
			boxPushStack.pop();
		}
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
	if (stageNow == 5) {
		stageFiveBg.SetTopLeft(0, 0);
		stageFiveBg.ShowBitmap();
	}
	if (stageNow == 6) {
		stageSixBg.SetTopLeft(0, 0);
		stageSixBg.ShowBitmap();
	}
	if (stageNow == 7) {
		stageSevenBg.SetTopLeft(0, 0);
		stageSevenBg.ShowBitmap();
	}
	if (stageNow == 8) {
		stageEightBg.SetTopLeft(0, 0);
		stageEightBg.ShowBitmap();
	}
	if (stageNow == 9) {
		stageNineBg.SetTopLeft(0, 0);
		stageNineBg.ShowBitmap();
	}
	if (stageNow == 10) {
		stageTenBg.SetTopLeft(0, 0);
		stageTenBg.ShowBitmap();
	}
	if (stageNow == 11) {
		stageElevenBg.SetTopLeft(0, 0);
		stageElevenBg.ShowBitmap();
	}
	if (stageNow == 12) {
		stageTwelveBg.SetTopLeft(0, 0);
		stageTwelveBg.ShowBitmap();
	}
	if (stageNow == 13) {
		stageThirteenBg.SetTopLeft(0, 0);
		stageThirteenBg.ShowBitmap();
	}
	if (stageNow == 14) {
		stageFourteenBg.SetTopLeft(0, 0);
		stageFourteenBg.ShowBitmap();
	}
	if (stageNow == 15) {
		stageFifteenBg.SetTopLeft(0, 0);
		stageFifteenBg.ShowBitmap();
	}
	if (stageNow == 16) {
		stageSixteenBg.SetTopLeft(0, 0);
		stageSixteenBg.ShowBitmap();
	}
	if (stageNow == 17) {
		stageSeventeenBg.SetTopLeft(0, 0);
		stageSeventeenBg.ShowBitmap();
	}
	if (stageNow == 18) {
		stageEighteenBg.SetTopLeft(0, 0);
		stageEighteenBg.ShowBitmap();
	}
	if (stageNow == 19) {
		stageNineteenBg.SetTopLeft(0, 0);
		stageNineteenBg.ShowBitmap();
	}
	if (stageNow == 20) {
		stageTwentyBg.SetTopLeft(0, 0);
		stageTwentyBg.ShowBitmap();
	}
	if (stageNow == 21) {
		stageTwentyOneBg.SetTopLeft(0, 0);
		stageTwentyOneBg.ShowBitmap();
	}
	if (stageNow == 22) {
		stageTwentyTwoBg.SetTopLeft(0, 0);
		stageTwentyTwoBg.ShowBitmap();
	}
	if (stageNow == 23) {
		stageTwentyThreeBg.SetTopLeft(0, 0);
		stageTwentyThreeBg.ShowBitmap();
	}
	if (stageNow == 24) {
		stageTwentyFourBg.SetTopLeft(0, 0);
		stageTwentyFourBg.ShowBitmap();
	}
	if (stageNow == 25) {
		stageTwentyFiveBg.SetTopLeft(0, 0);
		stageTwentyFiveBg.ShowBitmap();
	}
	if (stageNow == 26) {
		stageTwentySixBg.SetTopLeft(0, 0);
		stageTwentySixBg.ShowBitmap();
	}
	if (stageNow == 27) {
		stageTwentySevenBg.SetTopLeft(0, 0);
		stageTwentySevenBg.ShowBitmap();
	}
	if (stageNow == 28) {
		stageTwentyEightBg.SetTopLeft(0, 0);
		stageTwentyEightBg.ShowBitmap();
	}
	if (stageNow == 29) {
		stageTwentyNineBg.SetTopLeft(0, 0);
		stageTwentyNineBg.ShowBitmap();
	}

	leader.OnShow();
	box1.OnShow();
	box2.OnShow();
	box3.OnShow();
	
	for (int i = 0; i < 6; i += 2) {
		if (box1.GetX1() < finish_pos[i] + 10 && box1.GetX1() > finish_pos[i] - 10 && box1.GetY1() < finish_pos[i+1] + 10 && box1.GetY1() > finish_pos[i+1] - 10) {
			box_check.SetTopLeft(box1.GetX1(), box1.GetY1());
			box_check.ShowBitmap();
		}
	}
	for (int i = 0; i < 6; i += 2) {
		if (box2.GetX1() < finish_pos[i] + 10 && box2.GetX1() > finish_pos[i] - 10 && box2.GetY1() < finish_pos[i + 1] + 10 && box2.GetY1() > finish_pos[i + 1] - 10) {
			box_check.SetTopLeft(box2.GetX1(), box2.GetY1());
			box_check.ShowBitmap();
		}
	}
	for (int i = 0; i < 6; i += 2) {
		if (box3.GetX1() < finish_pos[i] + 10 && box3.GetX1() > finish_pos[i] - 10 && box3.GetY1() < finish_pos[i + 1] + 10 && box3.GetY1() > finish_pos[i + 1] - 10) {
			box_check.SetTopLeft(box3.GetX1(), box3.GetY1());
			box_check.ShowBitmap();
		}
	}
	


	if (switchmodecount % 3 == 1) {
		console.SetTopLeft(20, 420);
		console.ShowBitmap();
		rightCircle.SetTopLeft(23, 521);
		rightCircle.ShowBitmap();
	}
	if (switchmodecount % 3 == 2) {
		console.SetTopLeft(243, 420);
		console.ShowBitmap();
		handCircle.SetTopLeft(23, 521);
		handCircle.ShowBitmap();
	}


	if (muteSound) {
		line.SetTopLeft(84, 526);
		line.ShowBitmap();
	}

}

}


