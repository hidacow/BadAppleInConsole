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

int playtime=0;	// ������Ϊֹ�Ѳ�������

#define FRAMENUM 6573				// ��֡��
#define FRAMEWIDTH 112				// ֡���
#define FRAMEHEIGHT 49				// ֡�߶�
#define VIDEOLENGTH 218946			// ��Ƶ����(����)
#define VIDEOLENGTH2 215000
#define OFFSET  0
#define OFFSET2 -1500					//λ��
#define VIDEOPATH "BadApple.txt"	// ������Ƶtxt��·��
#ifdef _MSC_VER
		#define AUDIOPATH L"Bad Apple.mp3"	// ������Ƶ��·��	//VS
		#define AUDIOPATH2 L"1.mid"
#else
		#define AUDIOPATH "Bad Apple.mp3"	//g++
		#define AUDIOPATH2 "1.mid"
#endif

float VideoLen=0,VideoOffset=0;
int audioversion=0;				//��Ƶ�汾
#define CONSOLETITLE "Bad Apple"	//����̨����
clock_t starttime, endtime;	// ���濪ʼʱ��
MyTimer timer;				// �߾��ȼ�ʱ����������ʱ��ʹ����ͬ����


void ThreadPlaySound()	// ��Ƶ���Ž��̣�������
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
			if(playtime==audiolength)	//���Ž���
				break;

		}
		mciSendCommand(1,MCI_CLOSE,NULL,NULL);	//�ر��豸
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
    cout << "[0] " << "�˳�����"<<endl<<endl;
	do
    {
        //����Ϊ�û�ȷ��
		cout<< "������ѡ��[0-2]:";
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
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); // ��ȡ����̨���
	SetConsoleTitleA(CONSOLETITLE); // ����console����
	SMALL_RECT rc = { 0, 0, FRAMEWIDTH, FRAMEHEIGHT }; // ����Ĭ�ϴ��ڴ�Сλ��
	SetConsoleWindowInfo(handle, true, &rc);
	SetConsoleTextAttribute(handle, (0<<4)|7);	//���ñ�����ɫ(7)���ַ���ɫ(0)
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false; //ʹ����̨��겻�ɼ�

	COORD coord;	// ��ʼ������
	coord.X = 0;
	coord.Y = 0;
	std::ifstream openfile;
	openfile.open(VIDEOPATH);
	if (!openfile.is_open())
	{
		std::cout << "Failed to open video file" << std::endl;
		return 1;
	}
	std::string frame_output;	// һ֡�����ַ���
	int cnt = 0;
	playtime=0;
	std::thread th1(ThreadPlaySound);	// ��Ҫ�첽�������֣�����һ���߳�

	while (!openfile.eof())
	{
		if (!cnt)
		{
			while (true)
			{
				if (playtime > 0)break;	// �������ֻ���ʱ��
				if (playtime == -99) {
					break;
				}
			}
			starttime = clock(); // ��ʱ��ʼ
		}

		unsigned int col = 0;
		while (col < FRAMEHEIGHT)	//����ÿ������������Ҫ��ȡFRAMEHEIGHT��
		{
			std::string outdata;
			getline(openfile, outdata, '\n');	// ��ȡһ����
			frame_output += outdata + '\n';	// ����frame_output
			++col;
		}
		std::cout << frame_output.c_str();			//���һ֡
		SetConsoleCursorPosition(handle, coord);	//��λָ��
		SetConsoleCursorInfo(handle, &CursorInfo);	//���ÿ���̨���״̬
		endtime = clock();
		const float standardtime = VideoLen * static_cast<float>(cnt) / FRAMENUM + VideoOffset;	//�����һ֡Ӧ�ñ����ŵ�ʱ���
		while (endtime - starttime < standardtime)	//У׼�����ܵ�������ɵ�������ͬ��
		{
			timer.Sleep(1);	//��1msΪ��λ����У׼
			endtime = clock();	//����ʱ��
		}
		cnt++;
		frame_output = "";
	}
	th1.detach();	//�ر���Ƶ���Ž���
	system("cls");
	CursorInfo.bVisible = true; //ʹ����̨���ɼ������û�����ѡ��
	SetConsoleCursorInfo(handle, &CursorInfo);	//���ÿ���̨���״̬
	return 0;
}

int main()
{
	if(SelectMenu("�����ַ�������Bad Apple","����mp3ԭ��","����mid��")==0)
		return 0;	//�û�ѡ���˳�
	do
	{
		//������Ƶ
		if(PlayVideo()==1)	//�쳣
			return 1;
	}while(SelectMenu("�����ѽ���","�ز�mp3ԭ��","�ز�mid��")==1);
	//�û�ѡ���˳�
	return 0;
}
