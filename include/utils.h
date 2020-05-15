#pragma once
#ifndef CJKB__utils_h_H_
#define CJKB__utils_h_H_

#include <boost/thread/mutex.hpp>
#include <mirai.h>
#include <database.h>
#include <cstdlib>
#include <ctime>
#include <functional>
using namespace std;
using namespace Cyan;

extern const string INIT_KEY;
extern const QQ_t BOT_QQ;
extern const QQ_t ADMIN_QQ;
extern MiraiBot bot;
extern UserDb userDb;
extern NewsDb newsDb;
const static string Help_Msg =
R"([重交课表机器人使用帮助]
1. 回复 "课表"、"今天/今日课表" 查看今天的课表
2. 回复 "明天/明日课表" 查看明天的课表
3. 回复 "本周/下周课表" 查看一周的课表
4. 回复 "绑定学号 学号 密码" 下载课表
5. 回复 "教务新闻" 查看最新的教务网新闻
6. 其他指令: "第几周"、"帮助")";


typedef function<void(const MessageChain&)> Sender;


inline int GetHour24()
{
	char buffer[4];
	time_t timep;
	struct tm* p;
	time(&timep);
	p = localtime(&timep);
	strftime(buffer, 4, "%H", p);
	return atoi(buffer);
}

inline int GetWeekNum()
{
	time_t timep;
	struct tm* p;
	time(&timep);
	p = localtime(&timep);
	int result = p->tm_wday;
	if (result == 0) return 6;
	else return (result - 1);
}

inline int GetWeekOfSemester()
{
	// 2020/2/23 0:0:0
	// 实际上开学日期是 2020/2/24 0:0:0
	// 把开学日期提前一天可以做到在周日查本周课表的时候，查的是下一周的课表
	const static time_t first_day_this_semester = 1582387200ll;
	time_t now;
	time(&now);
	long long diff_sec = difftime(now, first_day_this_semester);
	int week = ((diff_sec) / (7 * 24 * 60 * 60)) + 1;
	if (week > 20) week = 20;
	return week;
}

inline MessageChain CoursesFormat(const json& courses)
{
	int idx = 1;
	MessageChain mc;
	for (const auto& ele : courses)
	{
		mc
			.Plain(to_string(idx++))
			.Plain(". ")
			.Plain(ele["Name"].get<string>())
			.Plain("，")
			.Plain(ele["Classroom"].get<string>())
			.Plain("，")
			.Plain(ele["Time"].get<string>())
			.Plain("\n");
	}
	return mc;
}

void FriendMsgProcessor(FriendMessage);

void GroupMsgProcessor(GroupMessage);

void TempMsgProcessor(TempMessage);

#endif // !CJKB__utils_h_H_