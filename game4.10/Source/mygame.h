/*
 * mygame.h: セ郎纗笴栏セōclassinterface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CLeader.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "CLeader.h"
#include "CBall.h"
#include "CBouncingBall.h"
#include "CGameMap.h"
#include "CBox.h"
#include <stack>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 音效
		AUDIO_BGM,				// 0
		AUDIO_CLICK,			// 1
		AUDIO_BOX,				// 2
		AUDIO_CLEAR,			// 3
	};

	/////////////////////////////////////////////////////////////////////////////
	// State Init
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								
		void OnBeginState();							
		void OnKeyUp(UINT, UINT, UINT); 				
		void OnLButtonDown(UINT nFlags, CPoint point);  
	protected:
		void OnShow();									
	private:
		CMovingBitmap logo;								//Background
		CMovingBitmap line;
		int		leaveInitCount = 0;
		bool	muteBGM = false;
	};


	class CGameStateSelect : public CGameState {
	public:
		CGameStateSelect(CGame *g);
		void OnInit();  
		void OnBeginState();							
		void OnLButtonDown(UINT nFlags, CPoint point);  
	protected:
		void OnShow();									
	private:
		CMovingBitmap	stageOneBg;
		CMovingBitmap	stageTwoBg;
		CMovingBitmap	stageThreeBg;
		CMovingBitmap	stageFourBg;
		CMovingBitmap	stageFiveBg;
		CMovingBitmap	stageSixBg;
		CMovingBitmap	stageSevenBg;
		CMovingBitmap	stageEightBg;
		CMovingBitmap	stageNineBg;
		CMovingBitmap	stageTenBg;
		CMovingBitmap	stageElevenBg;
		CMovingBitmap	stageTwelveBg;
		CMovingBitmap	stageThirteenBg;
		CMovingBitmap	stageFourteenBg;
		CMovingBitmap	stageFifteenBg;
	};

	/////////////////////////////////////////////////////////////////////////////
	// State Run
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();				
		void OnInit();  							
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  
		void OnLButtonUp(UINT nFlags, CPoint point);	
		void OnMouseMove(UINT nFlags, CPoint point);	
		void OnRButtonDown(UINT nFlags, CPoint point);  
		void OnRButtonUp(UINT nFlags, CPoint point);	
	protected:
		void OnMove();									
		void OnShow();									
	private:
		const int		MOVE_COUNTER;
		int				moveCounter;
		int				finishCounter;
		int				hackCounter = 0;
		int				switchmodecount = 0;
		int             finish_pos[6] = { 0 };

		CMovingBitmap	stageOneBg;
		CMovingBitmap	stageTwoBg;
		CMovingBitmap	stageThreeBg;
		CMovingBitmap	stageFourBg;
		CMovingBitmap	stageFiveBg;
		CMovingBitmap	stageSixBg;
		CMovingBitmap	stageSevenBg;
		CMovingBitmap	stageEightBg;
		CMovingBitmap	stageNineBg;
		CMovingBitmap	stageTenBg;
		CMovingBitmap	stageElevenBg;
		CMovingBitmap	stageTwelveBg;
		CMovingBitmap	stageThirteenBg;
		CMovingBitmap	stageFourteenBg;


		CMovingBitmap	line;
		CMovingBitmap	console;
		CMovingBitmap	rightCircle;
		CMovingBitmap	handCircle;
		CMovingBitmap   box_check;
		

		CLeader			leader;	
		CGameMap        gamemap;
		CBox			box1;
		CBox			box2;
		CBox			box3;
		

		std::stack<int>		dirStack;
		std::stack<bool>	boxPushStack;
	};



	/////////////////////////////////////////////////////////////////////////////
	// State Over
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();	
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);
	protected:
		void OnMove();	
		void OnShow();									
	private:
		int counter;	
		CMovingBitmap	stageOneCp;
		CMovingBitmap	stageTwoCp;
		CMovingBitmap	stageThreeCp;
		CMovingBitmap	stageFourCp;
		CMovingBitmap	stageFiveCp;
		CMovingBitmap	stageSixCp;
		CMovingBitmap	stageSevenCp;
		CMovingBitmap	stageEightCp;
		CMovingBitmap	stageNineCp;
		CMovingBitmap	stageTenCp;
		CMovingBitmap	stageElevenCp;
		CMovingBitmap	stageTwelveCp;
		CMovingBitmap	stageThirteenCp;
		CMovingBitmap	stageFourteenCp;
	};

}



