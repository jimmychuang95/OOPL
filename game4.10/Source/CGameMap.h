namespace game_framework {

	class CGameMap
	{
	public:
		CGameMap();
		int CGameMap::GetComponent(int x, int y);
		bool CGameMap::MoveRight();
		bool CGameMap::MoveLeft();
		bool CGameMap::MoveUp();
		bool CGameMap::MoveDown();
		bool CGameMap::IsFinish();
		void CGameMap::SetLeader();
		void CGameMap::Initialize(int level);
		void CGameMap::PreviousStep(int dir, bool boxPush);
		void CGameMap::HackModeOn();
		int CGameMap::return_finish_dis(int i);
	protected:
		int map[9][9];
		const int x, y;
		const int mx, my;
		int init_leadersite[2] = { 0 };
		int leadersite[2] = { 0 };
		int finish_pos[6] = { 0 };
		int finish_dis[6] = { 0 };
	};
}