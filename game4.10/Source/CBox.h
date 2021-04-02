namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CBox
	/////////////////////////////////////////////////////////////////////////////

	class CBox
	{
	public:
		CBox();
		int  GetX1();					// 揽オà x 畒夹
		int  GetY1();					// 揽オà y 畒夹
		int  GetX2();					// 揽à x 畒夹
		int  GetY2();					// 揽à y 畒夹
		bool GetStatus();
		void PushedDown(bool flag);
		void PushedUp(bool flag);
		void PushedRight(bool flag);
		void PushedLeft(bool flag);
		bool CanPushDown();
		bool CanPushUp();
		bool CanPushRight();
		bool CanPushLeft();
		void Initialize(int level);				// 砞﹚揽﹍
		void LoadBitmap();				// 更瓜
		void OnMove();					// 簿笆揽
		void OnShow();					// 盢揽瓜禟礶
		void OnShowLeft();
		void SetMovingDown(bool flag);	// 砞﹚琌タ┕簿笆
		void SetMovingLeft(bool flag);	// 砞﹚琌タ┕オ簿笆
		void SetMovingRight(bool flag); // 砞﹚琌タ┕簿笆
		void SetMovingUp(bool flag);	// 砞﹚琌タ┕簿笆
		void SetXY(int nx, int ny);		// 砞﹚揽オà畒夹
		void SetStanding(bool flag);
	protected:
		CMovingBitmap bmp_box;		// 揽笆礶
		int x, y;					// 揽オà畒夹
		bool isMovingDown;			// 琌タ┕簿笆
		bool isMovingLeft;			// 琌タ┕オ簿笆
		bool isMovingRight;			// 琌タ┕簿笆
		bool isMovingUp;
		bool isStanding;
		bool canPushDown;
		bool canPushLeft;
		bool canPushRight;
		bool canPushUp;
	};
}
