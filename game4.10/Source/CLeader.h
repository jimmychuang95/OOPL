namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 硂class矗ㄑノ龄絃┪菲公北揽
	// 来碞э糶Θ祘Α
	/////////////////////////////////////////////////////////////////////////////

	class CLeader
	{
	public:
		CLeader();
		int  GetX1();					// 揽オà x 畒夹
		int  GetY1();					// 揽オà y 畒夹
		int  GetX2();					// 揽à x 畒夹
		int  GetY2();					// 揽à y 畒夹
		bool GetStatus();
		void Initialize(int level);				// 砞﹚揽﹍
		void LoadBitmap();				// 更瓜
		void OnMove();					// 簿笆揽
		void OnShow();					// 盢揽瓜禟礶
		void SetMovingDown(bool flag);	// 砞﹚琌タ┕簿笆
		void SetMovingLeft(bool flag);	// 砞﹚琌タ┕オ簿笆
		void SetMovingRight(bool flag); // 砞﹚琌タ┕簿笆
		void SetMovingUp(bool flag);	// 砞﹚琌タ┕簿笆
		void SetXY(int nx, int ny);		// 砞﹚揽オà畒夹
		void SetStanding(bool flag);
	protected:
		CMovingBitmap animation_up;		// 揽笆礶
		CMovingBitmap animation_down;
		CMovingBitmap animation_left;
		CMovingBitmap animation_right;
		int x, y,current;					// 揽オà畒夹
		bool isMovingDown;			// 琌タ┕簿笆
		bool isMovingLeft;			// 琌タ┕オ簿笆
		bool isMovingRight;			// 琌タ┕簿笆
		bool isMovingUp;
		bool isStanding;
	};
}