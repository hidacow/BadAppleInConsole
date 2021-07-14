// 程序名称：精确到微秒的延时类（基于多媒体定时器）
// 原作：yangw80 <yw80@qq.com>

#pragma once
#include <Windows.h>

class MyTimer
{
	LARGE_INTEGER m_clk{};		// 保存时钟信息
	LONGLONG m_oldclk;			// 保存开始时钟和结束时钟
	int m_freq;					// 时钟频率(时钟时间换算率)，时间差

public:
	MyTimer();
	void Sleep(int ms);
	void Reset();
};

// 构造函数
inline MyTimer::MyTimer()
{
	QueryPerformanceFrequency(&m_clk);
	m_freq = (int)m_clk.QuadPart / 1000;		// 获得计数器的时钟频率
	m_oldclk = 0;
}

// 延时
inline void MyTimer::Sleep(int ms)
{
	unsigned int c = ms * m_freq;

	if (m_oldclk == 0)
	{
		// 开始计时
		QueryPerformanceCounter(&m_clk);
		m_oldclk = m_clk.QuadPart;				// 获得开始时钟
	}

	m_oldclk += c;

	QueryPerformanceCounter(&m_clk);
	if (m_clk.QuadPart > m_oldclk)
		m_oldclk = m_clk.QuadPart;
	else
		do
		{
			::Sleep(1);
			QueryPerformanceCounter(&m_clk);	// 获得终止时钟
		}
		while(m_clk.QuadPart < m_oldclk);
}

// 重置延时器
inline void MyTimer::Reset()
{
	m_oldclk = 0;
}