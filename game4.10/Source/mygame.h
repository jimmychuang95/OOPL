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

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// ﹚竡贺絪腹
		AUDIO_BGM,				// 0
		AUDIO_CLICK,			// 1
		AUDIO_BOX,				// 2
		AUDIO_CLEAR,			// 3
	};

	/////////////////////////////////////////////////////////////////////////////
	// 硂class笴栏笴栏秨繷礶ン
	// –Member functionImplementation常璶来
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 笴栏の瓜砞﹚
		void OnBeginState();							// 砞﹚–Ω┮惠跑计
		void OnKeyUp(UINT, UINT, UINT); 				// 矪瞶龄絃Up笆
		void OnLButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
	protected:
		void OnShow();									// 陪ボ硂篈笴栏礶
	private:
		CMovingBitmap logo;								// csielogo
		CMovingBitmap line;
		int		leaveInitCount = 0;
		bool	mute = false;
	};


	class CGameStateSelect : public CGameState {
	public:
		CGameStateSelect(CGame *g);
		void OnInit();  								// 笴栏の瓜砞﹚
		void OnBeginState();							// 砞﹚–Ω┮惠跑计
		void OnLButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
	protected:
		void OnShow();									// 陪ボ硂篈笴栏礶
	private:
		CMovingBitmap	stageOneBg;
		CMovingBitmap	stageTwoBg;
		CMovingBitmap	stageThreeBg;
		CMovingBitmap	stageFourBg;
		CMovingBitmap	stageFiveBg;
		CMovingBitmap	stageSixBg;
		CMovingBitmap	stageSevenBg;
		CMovingBitmap	stageEightBg;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 硂class笴栏笴栏磅︽ン璶笴栏祘Α常硂柑
	// –Member functionImplementation常璶来
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 砞﹚–Ω┮惠跑计
		void OnInit();  								// 笴栏の瓜砞﹚
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
		void OnLButtonUp(UINT nFlags, CPoint point);	// 矪瞶菲公笆
		void OnMouseMove(UINT nFlags, CPoint point);	// 矪瞶菲公笆 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 矪瞶菲公笆
		void OnRButtonUp(UINT nFlags, CPoint point);	// 矪瞶菲公笆
	protected:
		void OnMove();									// 簿笆笴栏じ
		void OnShow();									// 陪ボ硂篈笴栏礶
	private:
		const int		MOVE_COUNTER;
		int				moveCounter;
		int				finishCounter;
		CMovingBitmap	stageOneBg;	// 璉春瓜
		CMovingBitmap	stageTwoBg;
		CMovingBitmap	stageThreeBg;
		CMovingBitmap	stageFourBg;
		CMovingBitmap	stageFiveBg;
		CMovingBitmap	stageSixBg;
		CMovingBitmap	stageSevenBg;
		CLeader			leader;		// ╃
		CGameMap        gamemap;
		CBox			box1;
		CBox			box2;
		CBox			box3;
	};



	/////////////////////////////////////////////////////////////////////////////
	// 硂class笴栏挡篈(Game Over)
	// –Member functionImplementation常璶来
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 砞﹚–Ω┮惠跑计
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);
	protected:
		void OnMove();									// 簿笆笴栏じ
		void OnShow();									// 陪ボ硂篈笴栏礶
	private:
		int counter;	// 计ぇ璸计竟
		CMovingBitmap	stageOneCp;
		CMovingBitmap	stageTwoCp;
		CMovingBitmap	stageThreeCp;
		CMovingBitmap	stageFourCp;
		CMovingBitmap	stageFiveCp;
		CMovingBitmap	stageSixCp;
		CMovingBitmap	stageSevenCp;
	};

}



