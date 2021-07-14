#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <ctime>
#include "MyTimer.h"
#include <conio.h>
#include <thread>
using namespace std;
#pragma comment(lib,"winmm.lib")

int playtime=0;	// 到现在为止已播放秒数

#define FRAMENUM 6573				// 总帧数
#define FRAMEWIDTH 112				// 帧宽度
#define FRAMEHEIGHT 49				// 帧高度
#define VIDEOLENGTH 218946			// 视频长度(毫秒)
#define VIDEOLENGTH2 215000
#define OFFSET  0
#define OFFSET2 -1500					//位移
#define VIDEOPATH "BadApple.txt"	// 储存视频txt的路径
#ifdef _MSC_VER
		#define AUDIOPATH L"Bad Apple.mp3"	// 储存音频的路径	//VS
		#define AUDIOPATH2 L"1.mid"
#else
		#define AUDIOPATH "Bad Apple.mp3"	//g++
		#define AUDIOPATH2 "1.mid"
#endif

float VideoLen=0,VideoOffset=0;
int audioversion=0;				//音频版本
#define CONSOLETITLE "Bad Apple"	//控制台标题
clock_t starttime, endtime;	// 储存开始时间
MyTimer timer;				// 高精度计时器，用于延时（使音画同步）


void ThreadPlaySound()	// 音频播放进程（函数）
{
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;
	try
	{
#ifdef _MSC_VER	//MSVC
		if(audioversion==1)mciOpen.lpstrDeviceType = reinterpret_cast<LPCWSTR>(MCI_DEVTYPE_WAVEFORM_AUDIO);	//mp3,wav
		if(audioversion==2)mciOpen.lpstrDeviceType = reinterpret_cast<LPCWSTR>(MCI_DEVTYPE_SEQUENCER);		//mid
#else	//g++
		if(audioversion==1)mciOpen.lpstrDeviceType = reinterpret_cast<LPCSTR>(MCI_DEVTYPE_WAVEFORM_AUDIO);
		if(audioversion==2)mciOpen.lpstrDeviceType = reinterpret_cast<LPCSTR>(MCI_DEVTYPE_SEQUENCER);
#endif
		if(audioversion==1)mciOpen.lpstrElementName = AUDIOPATH;
		if(audioversion==2)mciOpen.lpstrElementName = AUDIOPATH2;
		mciOpen.wDeviceID = 1;
		if (mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpen) != 0)
		{
			std::cout << "Failed to open audio" << std::endl;
			playtime = -99;
			return;
		}
		mciPlay.dwFrom = 0;
		mciPlay.dwTo = 100;
		mciSendCommand(1, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD_PTR)(&mciPlay));
		int audiolength=0;
		MCI_STATUS_PARMS status;
		status.dwItem = MCI_STATUS_LENGTH;
		status.dwCallback = 0;
		mciSendCommand(1,MCI_STATUS,MCI_STATUS_ITEM|MCI_WAIT,(DWORD_PTR)&status);
		audiolength =(int)status.dwReturn;
		while (true)
		{

			status.dwItem = MCI_STATUS_POSITION;
			status.dwCallback = 0;

			mciSendCommand(1,MCI_STATUS,MCI_STATUS_ITEM|MCI_WAIT,(DWORD_PTR)&status);
			playtime =(int)status.dwReturn;
			if(playtime==audiolength)	//播放结束
				break;

		}
		mciSendCommand(1,MCI_CLOSE,NULL,NULL);	//关闭设备
	}
	catch (...)
	{
		playtime = -99;
	}

}

int SelectMenu(string msg1,string msg2,string msg3)
{
	string confirmation;
	cout << msg1 <<endl<<endl;
    cout << "[1] " << msg2 <<endl<<endl;
	cout << "[2] " << msg3 <<endl<<endl;
    cout << "[0] " << "退出程序"<<endl<<endl;
	do
    {
        //以下为用户确认
		cout<< "请输入选项[0-2]:";
        cin >> confirmation;
        if (confirmation == "0")
            return 0;
		if (confirmation == "1")
		{
			VideoLen=VIDEOLENGTH;
			VideoOffset=OFFSET;
			audioversion=1;
			return 1;
		}
		if (confirmation == "2")
		{
			VideoLen=VIDEOLENGTH2;
			VideoOffset=OFFSET2;
			audioversion=2;
			return 1;
		}

    } while (confirmation != "2" &&confirmation != "1" && confirmation != "0");

	return 0;
}

int PlayVideo(){
	system("cls");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); // 获取控制台句柄
	SetConsoleTitleA(CONSOLETITLE); // 设置console标题
	SMALL_RECT rc = { 0, 0, FRAMEWIDTH, FRAMEHEIGHT }; // 设置默认窗口大小位置
	SetConsoleWindowInfo(handle, true, &rc);
	SetConsoleTextAttribute(handle, (0<<4)|7);	//设置背景白色(7)，字符黑色(0)
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //使控制台光标不可见

	COORD coord;	// 初始化坐标
	coord.X = 0;
	coord.Y = 0;
	std::ifstream openfile;
	openfile.open(VIDEOPATH);
	if (!openfile.is_open())
	{
		std::cout << "Failed to open video file" << std::endl;
		return 1;
	}
	std::string frame_output;	// 一帧内容字符串
	int cnt = 0;
	playtime=0;
	std::thread th1(ThreadPlaySound);	// 需要异步播放音乐，开启一个线程

	while (!openfile.eof())
	{
		if (!cnt)
		{
			while (true)
			{
				if (playtime > 0)break;	// 消除音乐缓冲时间
				if (playtime == -99) {
					break;
				}
			}
			starttime = clock(); // 计时开始
		}

		unsigned int col = 0;
		while (col < FRAMEHEIGHT)	//对于每个画面我们需要读取FRAMEHEIGHT行
		{
			std::string outdata;
			getline(openfile, outdata, '\n');	// 获取一整行
			frame_output += outdata + '\n';	// 加入frame_output
			++col;
		}
		std::cout << frame_output.c_str();			//输出一帧
		SetConsoleCursorPosition(handle, coord);	//定位指针
		SetConsoleCursorInfo(handle, &CursorInfo);	//设置控制台光标状态
		endtime = clock();
		const float standardtime = VideoLen * static_cast<float>(cnt) / FRAMENUM + VideoOffset;	//计算出一帧应该被播放的时间点
		while (endtime - starttime < standardtime)	//校准因性能等问题造成的音画不同步
		{
			timer.Sleep(1);	//以1ms为单位进行校准
			endtime = clock();	//更新时间
		}
		cnt++;
		frame_output = "";
	}
	th1.detach();	//关闭音频播放进程
	system("cls");
	CursorInfo.bVisible = true; //使控制台光标可见，让用户输入选项
	SetConsoleCursorInfo(handle, &CursorInfo);	//设置控制台光标状态
	return 0;
}

int main()
{
	if(SelectMenu("音乐字符动画：Bad Apple","播放mp3原版","播放mid版")==0)
		return 0;	//用户选择退出
	do
	{
		//播放视频
		if(PlayVideo()==1)	//异常
			return 1;
	}while(SelectMenu("播放已结束","重播mp3原版","重播mid版")==1);
	//用户选择退出
	return 0;
}
