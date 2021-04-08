/*
 * gamelib.h: セ郎纗笴栏闽classinterface
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
 *		1. Add ENABLE_AUDIO flag.
 *   2004-03-02 V4.0
 *		1. Add CInteger class.
 *      2. Add CGameState abstract class and adjust CGame to apply
 *         state pattern for switching game states.
 *   2004-03-08 V4.1
 *      1. Add OPEN_AS_FULLSCREEN flag.
 *      2. The Game Engine now becomes a framework.
 *   2005-07-28
 *      1. Add GAME_ASSERT macro to display run time errors gracefully.
 *      2. Remove CDDarw::CheckSystem(). It is useless.
 *   2005-09-08
 *      1. Eliminate CSpecialEffect::Abort. It is useless now.
 *      2. Add SHOW_GAME_CYCLE_TIME flag.
 *      3. Add DEFAULT_BG_COLOR flag;
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add MOUSEMOVE Handler for CGame and CGameState.
 *      2. Add _TRACE preprocessor flag for VC++.net.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Remove constantness of CAnimation::DELAY_COUNT.
 *      3. Enhance CAnimation to support SetDelayCount(), Reset(), and IsFinalBitmap().
 *      4. Remove CAnimation::GetLocation() and CMovingBitmap::GetLocation().
 *      5. Bitmap coordinate can no longer be set by CMovingBitmap::LoadBitmap().
 *         Defauts to (0,0).
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *      2. Fix bug: audio is now correctly recovered after a sleep or suspension of windows.
 *      3. Support ENABLE_GAME_PAUSE.
 *   2008-02-15 V4.4
 *      1. Add setup project for Visual studio 2005.
 *      2. Support bitmap scaling when ShowBitmap(scale) is called.
 *      3. Add namespace game_framework.
 *      4. Make the class CGame a singleton so that MFC can access it easily.
 *      5. Support loading of bitmap from bmp file.
 *      6. Support ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Rewrite CAudio with MCI commands to eliminate dependency with DirectMusic.
*/

/////////////////////////////////////////////////////////////////////////////
// ﹚竡笴栏砞﹚吏挂籔兵ン
/////////////////////////////////////////////////////////////////////////////

#define SIZE_X				 337		// 砞﹚笴栏礶秆猂640x480
#define SIZE_Y				 599		// 爹璝ぃㄏノ夹非秆猂玥ぃち传棵辊
#define OPEN_AS_FULLSCREEN	 false		// 琌棵辊よΑ秨币笴栏
#define SHOW_LOAD_PROGRESS   true		// 琌陪ボloading(OnInit)秈
#define DEFAULT_BG_COLOR	 RGB(0,0,0)	// 笴栏礶箇砞璉春肅︹(堵︹)
#define GAME_CYCLE_TIME		 33		    // –33ms禲ΩMoveのShow(–30Ω)
#define SHOW_GAME_CYCLE_TIME false		// 琌debug mode陪ボcycle time
#define ENABLE_GAME_PAUSE	 true		// 琌す砛 Ctrl-Q 既氨笴栏
#define ENABLE_AUDIO		 true		// 币笆ざ

/////////////////////////////////////////////////////////////////////////////
// ﹚竡CGameのCGameState┮ㄏノ篈盽计
/////////////////////////////////////////////////////////////////////////////

enum GAME_STATES {
	GAME_STATE_INIT,
	GAME_STATE_SELECT,
	GAME_STATE_RUN,
	GAME_STATE_OVER
};

/////////////////////////////////////////////////////////////////////////////
// Header for STL (Standard Template Library)
/////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// 厨祘Α岿粇ノmacro
// 称爹硂柑ㄏノmacro獽玂痙郎嘿の︽腹debug
/////////////////////////////////////////////////////////////////////////////

#define GAME_ASSERT(boolexp,str)											\
		if (!(boolexp)) {													\
			int id;															\
			char s[300]="";													\
			sprintf(s,"Game fatal error:\n\n%s\n\nFile: %s\n\nLine: %d"		\
				"\n\n(Press Retry to debug the application, "				\
				"if it is executed in debug mode.)"							\
				"\n(Press Cancel otherwise.)",								\
				 str , __FILE__,__LINE__);									\
			id = AfxMessageBox(s, MB_RETRYCANCEL);							\
			if (id == IDCANCEL)												\
				exit(1);													\
			AfxDebugBreak();												\
		}

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// 硂class矗ㄑ丁岿粇单北
// 笴栏ぃ惠钡巨硂ンぃ恨硂classㄏノよ猭
/////////////////////////////////////////////////////////////////////////////

class CSpecialEffect {
public:
	static void  SetCurrentTime();					// 纗ヘ玡丁ctime
	static DWORD GetEllipseTime();					// 弄ヘ玡丁 - ctime
	static int   GetCurrentTimeCount();				// 弄纗ctimeΩ计
	static void  Delay(DWORD ms);					// ┑筐 x ms
	static void  DelayFromSetCurrentTime(DWORD ms);	// ctime癬衡┑筐 x ms
private:
	static DWORD ctime;
	static int	 ctimeCount;
};

/////////////////////////////////////////////////////////////////////////////
// 硂class穦ミDirectDrawン矗ㄑㄤclassㄏノ
// 笴栏ぃ惠钡巨硂ンぃ恨硂classㄏノよ猭
/////////////////////////////////////////////////////////////////////////////

class CDDraw {
	friend class CMovingBitmap;
public:
	~CDDraw();
	static void  BltBackColor(DWORD);		// 盢Back plain场帝﹚肅︹
	static void  BltBackToPrimary();		// 盢Back plain禟Primary plain
	static CDC*  GetBackCDC();				// 眔Back PlainDC (device context)
	static void  GetClientRect(CRect &r);	// 眔砞﹚秆猂
	static void  Init(int, int);			// Initialize direct draw
	static void  ReleaseBackCDC();			// 奔Back PlainDC (device context)
	static bool  SetFullScreen(bool);		// 砞﹚棵辊家Α/跌怠家Α
	static bool  IsFullScreen();			// 氮琌棵辊家Α/跌怠家Α
private:
	CDDraw();								// private constructor
	static void  BltBitmapToBack(unsigned SurfaceID, int x, int y);
	static void  BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor);
	static void  BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y);
	static void	 CheckDDFail(char *s);
	static bool  CreateSurface();
	static bool  CreateSurfaceFullScreen();
	static bool  CreateSurfaceWindowed();
	static void  LoadBitmap(int i, int IDB_BITMAP);
	static void  LoadBitmap(int i, char *filename);
	static DWORD MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color);
	static int   RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey);
	static int   RegisterBitmap(char *filename, COLORREF ColorKey);
	static void  ReleaseSurface();
	static void  RestoreSurface();
	static void  SetColorKey(unsigned SurfaceID, COLORREF color);
    static HDC					hdc;
	static CDC					cdc;
	static CView				*pCView;
    static LPDIRECTDRAW2		lpDD;
	static LPDIRECTDRAWCLIPPER	lpClipperPrimary;   
	static LPDIRECTDRAWCLIPPER	lpClipperBack;   
	static LPDIRECTDRAWSURFACE	lpDDSPrimary;
	static LPDIRECTDRAWSURFACE	lpDDSBack;
	static vector<LPDIRECTDRAWSURFACE>	lpDDS;
    static HRESULT				ddrval;
	static vector<int>			BitmapID;
	static vector<string>		BitmapName;
	static vector<CRect>		BitmapRect;
	static vector<COLORREF>		BitmapColorKey;
	static bool					fullscreen;
	static CDDraw				ddraw;
	static int					size_x, size_y;
};

/////////////////////////////////////////////////////////////////////////////
// 硂class矗ㄑ笆篈(簿笆)瓜
// –Public Interfaceノ猭常璶来Implementationぃ来
/////////////////////////////////////////////////////////////////////////////

class CMovingBitmap {
public:
	CMovingBitmap();
	int   Height();						// 眔瓜蔼
	int   Left();						// 眔瓜オà x 畒夹
	void  LoadBitmap(int,COLORREF=CLR_INVALID);		// 更瓜﹚瓜絪腹(resource)の硓︹
	void  LoadBitmap(char *,COLORREF=CLR_INVALID);	// 更瓜﹚瓜郎の硓︹
	void  SetTopLeft(int,int);			// 盢瓜オà畒夹簿 (x,y)
	void  ShowBitmap();					// 盢瓜禟棵辊
	void  ShowBitmap(double factor);	// 盢瓜禟棵辊 factor < 1罽>1猔種惠璶VGA祑砰や穿玥穦篊
	void  ShowBitmap(CMovingBitmap &);	// 盢瓜禟眎瓜 (度ㄑ疭ノ硚)
	int   Top();						// 眔瓜オà y 畒夹
	int   Width();						// 眔瓜糴
protected:
	CRect    location;			// location of the bitmap
	bool     isBitmapLoaded;	// whether a bitmap has been loaded
	unsigned SurfaceID;			// the surface id of this bitmap
};

/////////////////////////////////////////////////////////////////////////////
// 硂class矗ㄑ簿笆笆礶
// –Public Interfaceノ猭常璶来Implementationぃ来
/////////////////////////////////////////////////////////////////////////////

class CAnimation {
public:
	CAnimation(int=10);				// Constructor (箇砞笆礶冀繵瞯–1/3传眎瓜)
	void  AddBitmap(int,COLORREF=CLR_INVALID);
									// 糤眎瓜笆礶(瓜絪腹の硓︹)
	void  AddBitmap(char *,COLORREF=CLR_INVALID);
									// 糤眎瓜笆礶(瓜絪腹の硓︹)
	int   GetCurrentBitmapNumber();	// 眔タ挤bitmap琌材碭bitmap
	int   Height();					// 眔笆礶蔼
	bool  IsFinalBitmap();			// 肚タ挤bitmap琌程bitmap
	int   Left();					// 眔笆礶オà x 畒夹
	void  OnMove();					// ㄌ繵瞯传bitmap
	void  OnShow();					// 盢笆礶禟棵辊
	void  Reset();					// 砞冀抖材眎瓜
	void  SetDelayCount(int);		// 砞﹚笆礶冀硉盽计(禫禫篊)
	void  SetTopLeft(int,int);		// 盢笆礶オà畒夹簿 (x,y)
	int   Top();					// 眔笆礶オà y 畒夹
	int   Width();					// 眔笆礶糴
private:
	list<CMovingBitmap>				bmp;			// list of CMovingBitmap
	list<CMovingBitmap>::iterator	bmp_iter;		// list iterator
	int								bmp_counter;	// 纗bmp_iter材nbmp
	int								delay_counter;	// ┑絯笆礶冀硉璸计竟
	int								delay_count;	// 笆礶冀硉盽计
	int								x, y;			// 笆礶畒夹
};

/////////////////////////////////////////////////////////////////////////////
// 硂class矗ㄑ陪ボ俱计瓜
// –Public Interfaceノ猭常璶来Implementationぃ来
/////////////////////////////////////////////////////////////////////////////

class CInteger {
public:
	CInteger(int=5);			// default 5 digits
	void Add(int n);			// 糤俱计
	int  GetInteger();			// 肚俱计
	void LoadBitmap();			// 更0..9の璽腹ぇ瓜
	void SetInteger(int);		// 砞﹚俱计
	void SetTopLeft(int,int);	// 盢笆礶オà畒夹簿 (x,y)
	void ShowBitmap();			// 盢笆礶禟棵辊
private:
	const int NUMDIGITS;			// 陪ボNUMDIGITS计
	static CMovingBitmap digit[11]; // 纗0..9の璽腹ぇ瓜(bitmap)
	int x, y;						// 陪ボ畒夹
	int n;							// 俱计
	bool isBmpLoaded;				// 琌竒更瓜
};

/////////////////////////////////////////////////////////////////////////////
// ﹟ゼ﹚竡class
/////////////////////////////////////////////////////////////////////////////

class CGame;
class CGameStateInit;
class CGameStateSelect;
class CGameStateRun;
class CGameStageTwo;
class CGameStateOver;

/////////////////////////////////////////////////////////////////////////////
// 硂class笴栏贺篈ぇBase class(琌abstract class)
// –Public Interfaceノ猭常璶来Implementationぃ来
/////////////////////////////////////////////////////////////////////////////

class CGameState {
public:
	CGameState(CGame *g);
	void OnDraw();			// Template Method
	void OnCycle();			// Template Method
	//
	// virtual functions, パ膥┯矗ㄑimplementation
	//
	virtual ~CGameState() {}								// virtual destructor
	virtual void OnBeginState() {}							// 砞﹚–Ω秈硂篈┮惠
	virtual void OnInit() {}								// 篈の瓜砞﹚
	virtual void OnKeyDown(UINT, UINT, UINT) {}				// 矪瞶龄絃Down笆
	virtual void OnKeyUp(UINT, UINT, UINT) {}				// 矪瞶龄絃Up笆
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {}// 矪瞶菲公笆
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {}	// 矪瞶菲公笆
	virtual void OnMouseMove(UINT nFlags, CPoint point) {}  // 矪瞶菲公笆 
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {}// 矪瞶菲公笆
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {}	// 矪瞶菲公笆
	static int	stageOpened;
	static int	stageNow;
	static bool	hackEnable;
protected:
	void GotoGameState(int state);							// 铬臘﹚state
	void ShowInitProgress(int percent);						// 陪ボ﹍て秈
	//
	// virtual functions, パ膥┯矗ㄑimplementation
	//
	virtual void OnMove() {}								// 簿笆硂篈笴栏じ
	virtual void OnShow() = 0;								// 陪ボ硂篈笴栏礶
	CGame *game;
	
};

/////////////////////////////////////////////////////////////////////////////
// 硂class琌笴栏み北笴栏秈︽
// 笴栏ぃ惠钡巨硂ンぃ恨硂classㄏノよ猭
/////////////////////////////////////////////////////////////////////////////

class CGame {
public:
	CGame();										// Constructor
	~CGame();										// Destructor
	bool IsRunning();								// 弄笴栏琌タ秈︽い
	void OnDraw();									// 癸莱CGameViewOnDraw()
	void OnFilePause();								// 笴栏既氨
	void OnInit();									// 笴栏酶瓜の﹍て
	void OnInitStates();							// 笴栏篈の瓜砞﹚
	bool OnIdle();									// 笴栏癹伴
	void OnKeyDown(UINT, UINT, UINT);				// 矪瞶龄絃Down笆
	void OnKeyUp(UINT, UINT, UINT);					// 矪瞶龄絃Up笆
	void OnKillFocus();								// 笴栏砆既氨
	void OnLButtonDown(UINT nFlags, CPoint point);	// 矪瞶菲公笆
	void OnLButtonUp(UINT nFlags, CPoint point);	// 矪瞶菲公笆
	void OnMouseMove(UINT nFlags, CPoint point);    // 矪瞶菲公笆 
	void OnRButtonDown(UINT nFlags, CPoint point);	// 矪瞶菲公笆
	void OnRButtonUp(UINT nFlags, CPoint point);	// 矪瞶菲公笆
	void OnResume();								// 矪瞶㏑临笆
	void OnSetFocus();								// 矪瞶Focus
	void OnSuspend();								// 矪瞶㏑笆
	void SetGameState(int);
	static CGame *Instance();
private:
	bool			running;			// 笴栏琌タ秈︽い(ゼ砆Pause)
	bool            suspended;			// 笴栏琌砆suspended
	const int		NUM_GAME_STATES;	// 笴栏篈计(3篈)
	CGameState		*gameState;			// pointerヘ玡笴栏篈
	CGameState		*gameStateTable[4];	// 笴栏篈ンpointer
	static CGame	instance;			// 笴栏斑instance
};

}