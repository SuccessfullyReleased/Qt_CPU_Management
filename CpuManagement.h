
#ifndef CPUMANAGEMENT_H
#define CPUMANAGEMENT_H

#include<string>
#include<iostream>
#include<deque>
#include<algorithm>
#include <iomanip>
using namespace std;

class Job
{
public:
	string name;
	int inTime;
	int startTime;
	int finishTime;
	int needTime;
	double responseRatio;
	int RemainTime;
	int turnaroundTime;
	double weight;
	Job() :inTime(0), startTime(0), finishTime(0), needTime(0), responseRatio(1), RemainTime(needTime),turnaroundTime(0),weight(0){}
	void setJob(string _name, int _inTime, int _startTime, int _finishTime, int _needTime)
	{
		name = _name;
		inTime = _inTime;
		startTime = _startTime;
		finishTime = _finishTime;
		needTime = _needTime;
		RemainTime = needTime;
	}
	void display()
	{
		cout << setw(14) << name << setw(14) << timeConversion(inTime) << setw(14) << timeConversion(startTime) << setw(14) << timeConversion(finishTime) << setw(14) << needTime << setw(14) << responseRatio << setw(14) << turnaroundTime << setw(14) << weight << endl;
	}
	void _display()
	{
		cout << setw(14) << name << setw(14) << inTime << setw(14) << startTime << setw(14) << finishTime << setw(14) << needTime << setw(14) << responseRatio << setw(14) << turnaroundTime << setw(14) << weight << endl;
	}
	string static timeConversion(int time)
	{
		string val;
		val += to_string(time / 60);
		val += ":";
		if (to_string(time % 60).size() < 2)
			val += "0";
		val += to_string(time % 60);
		return val;
	}
	bool static compareByName(Job*& a, Job*& b)
	{
		return a->name < b->name;
	}
	bool static compareByInTime(Job*& a, Job*& b)
	{
		if (a->inTime == b->inTime)
			return a->needTime < b->needTime;
		else
			return a->inTime < b->inTime;
	}
	bool static compareByNeedTime(Job*& a, Job*& b)
	{
		return a->needTime < b->needTime;
	}
	bool static compareByNow(int time, Job* val)
	{
		return time < val->inTime;
	}
	bool static compareByRespon(Job*& a, Job*& b)
	{
		return a->responseRatio > b->responseRatio;
	}
	bool static compareByRemainTime(Job*& a, Job*& b)
	{
		return a->RemainTime < b->RemainTime;
	}
};



class CpuManagement
{
	deque<Job*> waitQueue;
	deque<Job*> finishQueue;
	int algorithm;
	int now;
public:
	CpuManagement(int _algorithm);
	void setWaitJob(deque<Job*> val);
	deque<Job*>& getFinishJob();
	double getAVGTurn();
	double getAVGWeight();
	void clear();
	void execution();
	bool FCFS();
	bool SJF();
	bool SRTF();
	bool HRRF();
	bool RR(const int timeSlice);
	void calculate();
	void displayWaitList();
	void displayFinishList();
};

inline CpuManagement::CpuManagement(int _algorithm):algorithm(_algorithm),now(0)
{}

inline void CpuManagement::setWaitJob(deque<Job*> val)
{
	for (deque<Job*>::iterator it = val.begin();it != val.end();++it)
		waitQueue.push_back((*it));
}

inline deque<Job*>& CpuManagement::getFinishJob()
{
	return finishQueue;
}

inline double CpuManagement::getAVGTurn()
{
	double sum(0.0);
	for (deque<Job*>::iterator it = finishQueue.begin();it != finishQueue.end();++it)
		sum += (*it)->turnaroundTime;
	return sum / finishQueue.size();
}

inline double CpuManagement::getAVGWeight()
{
	double sum(0.0);
	for (deque<Job*>::iterator it = finishQueue.begin();it != finishQueue.end();++it)
		sum += (*it)->weight;
	return sum / finishQueue.size();
}

inline void CpuManagement::clear()
{
	waitQueue.clear();
	finishQueue.clear();
	now = 0;
}

void CpuManagement::execution()
{
	sort(waitQueue.begin(), waitQueue.end(), Job::compareByInTime);
	now = (*waitQueue.begin())->inTime;
	if (algorithm == 0)
		while (FCFS());
	else if (algorithm == 1)
		while (SJF());
	else if (algorithm == 2)
		while (SRTF());
	else if (algorithm == 3)
		while (HRRF());
	else if (algorithm == 4)
		while (RR(2));
}

inline bool CpuManagement::FCFS()
{
	if (waitQueue.empty())
		return false;
	else
	{
		Job* val = *waitQueue.begin();
		waitQueue.pop_front();
		if (val->inTime > now)
			now = val->inTime;
		val->startTime = now;
		now += val->needTime;
		val->finishTime = now;
		finishQueue.push_back(val);
		return true;
	}
}

inline bool CpuManagement::SJF()
{
	if (waitQueue.empty())
		return false;
	else
	{
		sort(waitQueue.begin(), waitQueue.end(), Job::compareByInTime);
		deque<Job*>::iterator last = upper_bound(waitQueue.begin(), waitQueue.end(), now,Job::compareByNow);
		sort(waitQueue.begin(), last, Job::compareByNeedTime);
		Job* val = *waitQueue.begin();
		waitQueue.pop_front();
		if (val->inTime > now)
			now = val->inTime;
		val->startTime = now;
		now += val->needTime;
		val->finishTime = now;
		finishQueue.push_back(val);
		return true;
	}
}

inline bool CpuManagement::SRTF()
{
	if (waitQueue.empty())
		return false;
	else
	{
		sort(waitQueue.begin(), waitQueue.end(), Job::compareByInTime);
		deque<Job*>::iterator last = upper_bound(waitQueue.begin(), waitQueue.end(), now, Job::compareByNow);
		sort(waitQueue.begin(), last, Job::compareByRemainTime);
		Job* val = *waitQueue.begin();

		if (last == waitQueue.begin())
			now = (*last)->inTime;
		else if (last == waitQueue.end())
		{
			waitQueue.pop_front();
			if (val->startTime == -1)
				val->startTime = now;
			now += val->RemainTime;
			val->RemainTime = 0;
			val->finishTime = now;
			finishQueue.push_back(val);
		}
		else
		{
			int timeSlice = (*last)->inTime - now;
			if (val->startTime == -1)
				val->startTime = now;
			if (val->RemainTime <= timeSlice)
			{
				now += val->RemainTime;
				val->RemainTime = 0;
				val->finishTime = now;
				waitQueue.pop_front();
				finishQueue.push_back(val);
			}
			else
			{
				now += timeSlice;
				val->RemainTime -= timeSlice;
			}
		}
		return true;
	}
}

inline bool CpuManagement::HRRF()
{
	if (waitQueue.empty())
		return false;
	else
	{
		sort(waitQueue.begin(), waitQueue.end(), Job::compareByInTime);
		deque<Job*>::iterator last = upper_bound(waitQueue.begin(), waitQueue.end(), now, Job::compareByNow);
		for (deque<Job*>::iterator it = waitQueue.begin();it != last;++it)
			(*it)->responseRatio = 1 + (now - (*it)->inTime) / double((*it)->needTime);
		sort(waitQueue.begin(), last, Job::compareByRespon);
		Job* val = *waitQueue.begin();
		waitQueue.pop_front();
		if (val->inTime > now)
			cout << "mistake!!" << endl;
		val->startTime = now;
		now += val->needTime;
		val->finishTime = now;
		finishQueue.push_back(val);
		return true;
	}
}

inline bool CpuManagement::RR(const int timeSlice)
{
	if (waitQueue.empty())
		return false;
	else
	{
		sort(waitQueue.begin(), waitQueue.end(), Job::compareByInTime);
		deque<Job*>::iterator last = upper_bound(waitQueue.begin(), waitQueue.end(), now, Job::compareByNow);
		sort(waitQueue.begin(), last, Job::compareByRemainTime);
		for (deque<Job*>::iterator it = waitQueue.begin();it != last;++it)
		{
			if ((*it)->startTime == -1)
				(*it)->startTime = now;
			if ((*it)->RemainTime - timeSlice >= 0)
			{
				now += timeSlice;
				(*it)->RemainTime -= timeSlice;
				if((*it)->RemainTime==0)
					(*it)->finishTime = now;
			}
			else
			{
				now += (*it)->RemainTime;
				(*it)->RemainTime = 0;
				(*it)->finishTime = now;
			}
		}
		int intervalSize = last - waitQueue.begin();
		for (int i = 0;i < intervalSize;++i)
		{
			Job* val = waitQueue.front();
			if (val->RemainTime)
				break;
			else
			{
				waitQueue.pop_front();
				finishQueue.push_back(val);
			}
		}
		return true;
	}
}

inline void CpuManagement::calculate()
{
	for (deque<Job*>::iterator it = finishQueue.begin();it != finishQueue.end();++it)
	{
		(*it)->turnaroundTime = (*it)->finishTime - (*it)->inTime;
		(*it)->weight = (*it)->turnaroundTime / (double)(*it)->needTime;
	}
	sort(finishQueue.begin(), finishQueue.end(), Job::compareByName);
}

inline void CpuManagement::displayWaitList()
{
	if (!waitQueue.empty())
	{
		cout << setw(14) << "name" << setw(14) << "inTime" << setw(14) << "startTime" << setw(14) << "finishTime" << setw(14) << "needTime" << setw(14) << "responseRatio" << setw(14) << "turnaroundTime" << setw(14) << "weight" << endl;
		for (deque<Job*>::iterator it = waitQueue.begin();it != waitQueue.end();++it)
			(*it)->display();
	}
}

inline void CpuManagement::displayFinishList()
{
	if (!finishQueue.empty())
	{
		cout << setw(14) << "name" << setw(14) << "inTime" << setw(14) << "startTime" << setw(14) << "finishTime" << setw(14) << "needTime" << setw(14) << "responseRatio" << "turnaroundTime" << setw(14) << "weight" << endl;
		for (deque<Job*>::iterator it = finishQueue.begin();it != finishQueue.end();++it)
			(*it)->_display();
	}
}

#endif // !CPUMANAGEMENT_H
