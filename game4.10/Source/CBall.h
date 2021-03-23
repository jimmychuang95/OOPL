namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 硂class矗ㄑ露伴伴瞴
	// 来碞э糶Θ祘Α
	/////////////////////////////////////////////////////////////////////////////

	class CBall
	{
	public:
		CBall();
		bool HitEraser(CLeader *eraser);						// 琌窱揽
		bool IsAlive();											// 琌帝
		void LoadBitmap();										// 更瓜
		void OnMove();											// 簿笆
		void OnShow();											// 盢瓜禟礶
		void SetXY(int nx, int ny);								// 砞﹚蛾み畒夹
		void SetIsAlive(bool alive);							// 砞﹚琌帝
		void SetDelay(int d);									// 砞﹚臂锣硉
	protected:
		CMovingBitmap bmp;			// 瞴瓜
		CMovingBitmap bmp_center;	// 蛾み瓜			
		int x, y;					// 蛾み畒夹
		int dx, dy;					// 瞴禯瞒蛾み簿秖
		int index;					// 瞴à0-17ボ0-360
		int delay_counter;			// 秸俱臂锣硉璸计竟
		int delay;					// 臂锣硉
		bool is_alive;				// 琌帝
	private:
		bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 琌窱把计絛瞅痻
	};
}