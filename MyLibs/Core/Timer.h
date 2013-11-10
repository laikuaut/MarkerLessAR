#pragma once

#include"type_c.h"
#include"../Exceptions/Exceptions.h"

#include<ctime>
#include<vector>

namespace pro{

class PRO_EXPORTS Timer
{
public:

	// タイマ状態列挙
	typedef enum{
		RESET,START,STOP
	}t_state;

private:
	clock_t start_time;	// 開始時間
	clock_t stop_time;	// 停止時間
	clock_t sum_stop;	// 停止時間の合計

	// ラップライム配列
	std::vector<clock_t> lap_time;

	// タイマ状態
	t_state state;

	void setState(t_state state);

public:

	Timer(void);
	~Timer(void);

	// 秒毎定数
	static clock_t const PER_SEC = CLOCKS_PER_SEC;

	// 秒毎定数 * sec
	static clock_t m_sec(long sec);

	t_state getState();

	clock_t getStartTime() const;
	clock_t getStopTime() const;
	clock_t getSumStop() const;
	const std::vector<clock_t> getLapTime() const;

	void start(void);
	clock_t restart(void);
	clock_t stop(void);
	void reset(void);
	clock_t lap(void);
	clock_t lap(long m_sec);

	// 最終lapとの差時間出力
	clock_t getDiff(void); 

	// 現在タイマ時間出力
	clock_t getNow(void);


};

}