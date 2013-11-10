#pragma once

#include"type_c.h"
#include"../Exceptions/Exceptions.h"

#include<ctime>
#include<vector>

namespace pro{

class PRO_EXPORTS Timer
{
public:

	// �^�C�}��ԗ�
	typedef enum{
		RESET,START,STOP
	}t_state;

private:
	clock_t start_time;	// �J�n����
	clock_t stop_time;	// ��~����
	clock_t sum_stop;	// ��~���Ԃ̍��v

	// ���b�v���C���z��
	std::vector<clock_t> lap_time;

	// �^�C�}���
	t_state state;

	void setState(t_state state);

public:

	Timer(void);
	~Timer(void);

	// �b���萔
	static clock_t const PER_SEC = CLOCKS_PER_SEC;

	// �b���萔 * sec
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

	// �ŏIlap�Ƃ̍����ԏo��
	clock_t getDiff(void); 

	// ���݃^�C�}���ԏo��
	clock_t getNow(void);


};

}