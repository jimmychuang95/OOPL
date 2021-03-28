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
/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏笴栏秨繷礶ン
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	
	//
	// 讽瓜OnInit更┮Τ瓜璶丁磷笴栏
	//     单ぃ瑻沸笴栏穦瞷Loading ...陪ボLoading秈
	//
	ShowInitProgress(0);	// 秨﹍loading秈0%
	//
	// 秨﹍更戈
	//
	logo.LoadBitmap(MY_HOMEPAGE);
	
	//Sleep(300);				// 篊獽睲贰秈龟悔笴栏叫埃Sleep

			// 初始畫面bgm

	//
	// OnInit笆穦钡CGameStaterRun::OnInit()┮秈临⊿100%
	//
}

void CGameStateInit::OnBeginState()
{
	CAudio::Instance()->Load(AUDIO_CLICK, "sounds\\click.mp3");
	CAudio::Instance()->Load(AUDIO_BGM, "sounds\\bgm.mp3");
	CAudio::Instance()->Play(AUDIO_BGM, true);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	//if (nChar == KEY_SPACE)
		//GotoGameState(GAME_STATE_RUN);						// ち传GAME_STATE_RUN
	if (nChar == KEY_ESC)								// Demo 闽超笴栏よ猭
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 闽超笴栏
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x < 230 && point.x > 110 && point.y > 260 && point.y < 315) {
		GotoGameState(GAME_STATE_RUN);
		CAudio::Instance()->Play(AUDIO_CLICK, false);
		CAudio::Instance()->Stop(AUDIO_BGM);		// ち传GAME_STATE_RUN
	}
}

void CGameStateInit::OnShow()
{
	//
	// 禟logo

	logo.SetTopLeft(0, 0);
	logo.ShowBitmap();
	
	 //Demo棵辊ㄏノぃ筁秨祇叫荷秖磷钡ㄏノэノCMovingBitmapゑ耕
	
	//CDC *pDC = CDDraw::GetBackCDC();			// 眔 Back Plain  CDC 
	//CFont f,*fp;
	//f.CreatePointFont(160,"Times New Roman");	// 玻ネ font f; 160ボ16 point
	//fp=pDC->SelectObject(&f);					// 匡ノ font f
	//pDC->SetBkColor(RGB(0,0,0));
	//pDC->SetTextColor(RGB(255,255,0));
	//pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	//pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	//if (ENABLE_GAME_PAUSE)
		//pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	//pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");
	//pDC->SelectObject(fp);						// 奔 font f (窾ぃ璶簗奔)
	//CDDraw::ReleaseBackCDC();					// 奔 Back Plain  CDC
}								

/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏挡篈(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	//if (counter < 0)
		//GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	CAudio::Instance()->Play(AUDIO_CLEAR, false);
	counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit()
{
	//
	// 讽瓜OnInit更┮Τ瓜璶丁磷笴栏
	//     单ぃ瑻沸笴栏穦瞷Loading ...陪ボLoading秈
	//
	ShowInitProgress(66);	// 钡玡篈秈矪秈跌66%
	//
	// 秨﹍更戈
	//
	Sleep(300);				// 篊獽睲贰秈龟悔笴栏叫埃Sleep
	//
	// 程沧秈100%
	//
	ShowInitProgress(100);
	background.LoadBitmap(STAGE_ONE_CP);
	CAudio::Instance()->Load(AUDIO_CLEAR, "sounds\\clear.mp3");	// 破關聲音
}



void CGameStateOver::OnShow()
{
	//CDC *pDC = CDDraw::GetBackCDC();					// 眔 Back Plain  CDC 
	//CFont f,*fp;
	//f.CreatePointFont(160,"Times New Roman");			// 玻ネ font f; 160ボ16 point
	//fp=pDC->SelectObject(&f);							// 匡ノ font f
	///pDC->SetBkColor(RGB(0,0,0));
	//pDC->SetTextColor(RGB(255,255,0));
	//char str[80];										// Demo 计癸﹃锣传
	//sprintf(str, "Game Over ! (%d)", counter / 30);
	//pDC->TextOut(240,210,str);
	//pDC->SelectObject(fp);							// 奔 font f (窾ぃ璶簗奔)
	//CDDraw::ReleaseBackCDC();
	background.ShowBitmap();							// 奔 Back Plain  CDC
}





/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏笴栏磅︽ン璶笴栏祘Α常硂柑
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), NUMBALLS(28), MOVE_COUNTER(20)
{
	//ball = new CBall [NUMBALLS];
}

CGameStateRun::~CGameStateRun()
{
	//delete [] ball;
}

void CGameStateRun::OnBeginState()
{
	//const int BALL_GAP = 90;
	//const int BALL_XY_OFFSET = 45;
	//const int BALL_PER_ROW = 7;
	//const int HITS_LEFT = 10;
	//const int HITS_LEFT_X = 590;
	//const int HITS_LEFT_Y = 0;
	//const int BACKGROUND_X = 60;
	//const int ANIMATION_SPEED = 15;
	//for (int i = 0; i < NUMBALLS; i++) {				// 砞﹚瞴癬﹍畒夹
		//int x_pos = i % BALL_PER_ROW;
		//int y_pos = i / BALL_PER_ROW;
		//ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		//ball[i].SetDelay(x_pos);
		//ball[i].SetIsAlive(true);
	//}
	leader.Initialize();
	background.SetTopLeft(0,0);				// 砞﹚璉春癬﹍畒夹
	moveCounter = MOVE_COUNTER;
	finishCounter = 40;
	//help.SetTopLeft(0, SIZE_Y - help.Height());			// 砞﹚弧瓜癬﹍畒夹
	//hits_left.SetInteger(HITS_LEFT);					// ﹚逞疾阑计
	//hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// ﹚逞疾阑计畒夹
	//CAudio::Instance()->Play(AUDIO_LAKE, true);			// 挤 WAVE
	//CAudio::Instance()->Play(AUDIO_DING, false);		// 挤 WAVE
	//CAudio::Instance()->Play(AUDIO_NTUT, true);			// 挤 MIDI
}

void CGameStateRun::OnMove()							// 簿笆笴栏じ
{
	//
	// 狦辨эcursor妓Α玥盢祘Αcommment
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	// 簿笆璉春瓜畒夹
	//
	//if (background.Top() > SIZE_Y)
		//background.SetTopLeft(60 ,-background.Height());
	//background.SetTopLeft(background.Left(),background.Top()+1);
	//
	// 簿笆瞴
	//
	//int i;
	//for (i=0; i < NUMBALLS; i++)
		//ball[i].OnMove();
	//
	// 簿笆揽
	//
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
		CAudio::Instance()->Stop(AUDIO_BOX);
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
		if(finishCounter <= 0)
			GotoGameState(GAME_STATE_OVER);
	}
	//
	// 耞揽琌窱瞴
	//
	//for (i=0; i < NUMBALLS; i++)
		//if (ball[i].IsAlive() && ball[i].HitEraser(&leader)) {
		//	ball[i].SetIsAlive(false);
		//	CAudio::Instance()->Play(AUDIO_DING);
		//	hits_left.Add(-1);
		//	//
			// 璝逞緇窱疾Ω计0玥铬Game Over篈
			//
		//	if (hits_left.GetInteger() <= 0) {
		//		CAudio::Instance()->Stop(AUDIO_LAKE);	// 氨ゎ WAVE
		//		CAudio::Instance()->Stop(AUDIO_NTUT);	// 氨ゎ MIDI
		//		GotoGameState(GAME_STATE_OVER);
			//}
		//}
	//
	// 簿笆紆铬瞴
	//
	//bball.OnMove();
}

void CGameStateRun::OnInit()  								// 笴栏の瓜砞﹚
{
	//
	// 讽瓜OnInit更┮Τ瓜璶丁磷笴栏
	//     单ぃ瑻沸笴栏穦瞷Loading ...陪ボLoading秈
	//
	ShowInitProgress(33);	// 钡玡篈秈矪秈跌33%
	//
	// 秨﹍更戈
	//
	//int i;
	//for (i = 0; i < NUMBALLS; i++)	
		//ball[i].LoadBitmap();								// 更材i瞴瓜
	leader.LoadBitmap();
	box.LoadBitmap();
	background.LoadBitmap(MY_STAGE_ONE);                  // 更璉春瓜
	
	// ЧΘ场だLoading笆矗蔼秈
	//
	ShowInitProgress(50);
	Sleep(300); // 篊獽睲贰秈龟悔笴栏叫埃Sleep
	//
	// 膥尿更ㄤ戈
	//
	//help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// 更弧瓜
	//corner.LoadBitmap(IDB_CORNER);							// 更à辅瓜
	//corner.ShowBitmap(background);							// 盢corner禟background
	//bball.LoadBitmap();										// 更瓜
	//hits_left.LoadBitmap();									
	
		// 點擊鼠標聲音
	CAudio::Instance()->Load(AUDIO_BOX,    "sounds\\box_long.mp3");		// 箱子移動聲音
	
	//
	// OnInit笆穦钡CGameStaterOver::OnInit()┮秈临⊿100%
	//
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboardオ絙繷
	const char KEY_UP    = 0x26; // keyboard絙繷
	const char KEY_RIGHT = 0x27; // keyboard絙繷
	const char KEY_DOWN  = 0x28; // keyboard絙繷
	if (nChar == KEY_LEFT) {
		if (leader.GetStatus()) {
			if (gamemap.MoveLeft()) {
				moveCounter = MOVE_COUNTER;
				leader.SetMovingLeft(true);
				leader.SetStanding(false);
				//gamemap.SetLeader();
				if (box.CanPushLeft()) {
					box.SetMovingLeft(true);
					CAudio::Instance()->Play(AUDIO_BOX, true);
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
				//gamemap.SetLeader();
				if (box.CanPushRight()) {
					box.SetMovingRight(true);
					CAudio::Instance()->Play(AUDIO_BOX, true);
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
				//gamemap.SetLeader();
				if (box.CanPushUp()) {
					box.SetMovingUp(true);
					CAudio::Instance()->Play(AUDIO_BOX, true);
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
				//gamemap.SetLeader();
				if (box.CanPushDown()) {
					box.SetMovingDown(true);
					CAudio::Instance()->Play(AUDIO_BOX, true);
				}
			}
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboardオ絙繷
	const char KEY_UP    = 0x26; // keyboard絙繷
	const char KEY_RIGHT = 0x27; // keyboard絙繷
	const char KEY_DOWN  = 0x28; // keyboard絙繷
	if (nChar == KEY_LEFT) {
		
	}
	if (nChar == KEY_RIGHT) {

	}
	if (nChar == KEY_UP) {

	}
	if (nChar == KEY_DOWN) {

	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 矪瞶菲公笆
{
	CAudio::Instance()->Play(AUDIO_CLICK, false);

	//CAudio::Instance()->Stop(AUDIO_BOX);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 矪瞶菲公笆
{

}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 矪瞶菲公笆
{
	// ⊿ㄆ狦惠璶矪瞶菲公簿笆杠糶code硂柑
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 矪瞶菲公笆
{
	//leader.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 矪瞶菲公笆
{
	//leader.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
	//
	//  猔種Show柑窾ぃ璶簿笆ヴン畒夹簿笆畒夹莱パMove暗癸
	//        玥讽跌怠穝酶瓜(OnDraw)ン碞穦簿笆癬ㄓ穦┣传砃粂
	//        弧Move璽砫MVCいModelShow璽砫ViewτViewぃ莱笆Model
	//
	//
	//  禟璉春瓜疾阑计瞴揽紆铬瞴
	//
	background.SetTopLeft(0, 0);
	background.ShowBitmap();			// 禟璉春瓜
	//help.ShowBitmap();					// 禟弧瓜
	//hits_left.ShowBitmap();
	//for (int i=0; i < NUMBALLS; i++)
		//ball[i].OnShow();				// 禟材i腹瞴
	//bball.OnShow();						// 禟紆铬瞴
	leader.OnShow();					// 禟揽
	box.OnShow();
	//
	//  禟オのà辅瓜
	//
	//corner.SetTopLeft(0,0);
	//corner.ShowBitmap();
	//corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
	//corner.ShowBitmap();
}
}