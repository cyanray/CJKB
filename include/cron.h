#pragma once
#ifndef CJKB__cron_h_H_
#define CJKB__cron_h_H_
#include "utils.h"
#include "commands.h"
#include <CJJWHelper.h>
using namespace std;

inline void cron_job_morning()
{
	const static string good_morning_words[] =
	{ "😘早安~","起床学习啦!","起床！起床！","你尚未起床，赖床指数666，打败了99%的人!" };
	while (true)
	{

		int hour = GetHour24();

		// 如果到了6点 就给发早安，然后延迟61分钟
		// 如果没到6点 就延迟10分钟再次检测
		int next_sleep_time = 10;

		if (hour == 6)
		{
			vector<QQ_t> users = userDb.GetAllUser();
			for (auto user : users)
			{
				try
				{
					bot.SendMessage(user, MessageChain().Plain(good_morning_words[rand() % 4]));
					json courses;
					if ((courses = userDb.GetCourses(user)).empty()) continue;
					// GetWeek()范围为[1,20]，修改为索引值，即[0,19]
					int week = GetWeekOfSemester() - 1;		// 获取本学期第几周
					int weekday = GetWeekNum();				// 获取今天是星期几
					json today_courses = courses["data"][week][weekday];

					MessageChain mc;
					int count = today_courses.size();
					if (count > 0)
					{
						mc.Plain("你今天共有 ").Plain(count).Plain(" 节课\n");
						mc = mc + CoursesFormat(today_courses);
					}
					else
					{
						mc.Plain("你今天没有课了！好好休息吧！");
					}

					bot.SendMessage(user, mc);

				}
				catch (const exception& ex)
				{
					cout << ex.what() << endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
			next_sleep_time = 61;
		}
		std::this_thread::sleep_for(std::chrono::minutes(next_sleep_time));
	}
}

inline void corn_job_news()
{
	using namespace Cyan;
	while (true)
	{
		try
		{
			HTTP http;
			auto resp = http.Get("http://jw.cqjtu.edu.cn/jxxx/tzgg1.htm");
			if (resp.Ready && resp.StatusCode == 200)
			{
				HTMLDoc html;
				html.Parse(resp.Content);
				// /html/body/div[3]/div/div/div[2]/div/div[2]/div/div/div/div[1]/ul
				auto list =
					html["html"]["body"]
					["div"][2]
					["div"]["div"]["div"][1]
					["div"]["div"][1]
					["div"]["div"]["div"]["div"][0]["ul"];
				auto tables = list.SearchByTagName("dt");
				for (auto& t : tables)
				{
					HTMLDoc a = t["a"];
					string url = "http://jw.cqjtu.edu.cn" + a.GetAttribute("href");
					string title = a["span"][0].GetInner();
					string date = a["span"][1].GetInner();
					if (!newsDb.Exist(title))
					{
						newsDb.Add(title, date, url);
						vector<QQ_t> users = userDb.GetAllUser();
						for (auto user : users)
						{
							try
							{
								MessageChain news;
								news.Plain("标题: " + title + "\n");
								news.Plain("网址: " + url + "\n");
								news.Plain("时间: " + date);
								bot.SendMessage(user, news);
							}
							catch (const exception& ex)
							{
								cout << ex.what() << endl;
							}
							MiraiBot::SleepMilliseconds(200);
						}

					}

				}
			}

		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		std::this_thread::sleep_for(std::chrono::minutes(20));
	}
}


#endif // !CJKB__cron_h_H_