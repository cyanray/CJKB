#include "commands.h"
#include "utils.h"
#include <CJJWHelper.h>
#include <vector>

namespace Cyan
{

	void today_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg)
	{
		try
		{
			json courses;
			if (!userDb.Exist(target) || (courses = userDb.GetCourses(target)).empty())
			{
				sendMsg(MessageChain().Plain("没有查到你的课表，回复 \"绑定学号\" 启动本功能"));
				return;
			}

			// GetWeek()范围为[1,20]，修改为索引值，即[0,19]
			int week = GetWeekOfSemester() - 1;		// 获取本学期第几周
			int weekday = GetWeekNum();				// 获取今天是星期几
			json today_courses = courses["data"][week][weekday];

			cout << today_courses.dump() << endl;

			MessageChain mc;
			int count = today_courses.size();
			if (count > 0)
			{
				mc.Plain("你今天共有 ").Plain(count).Plain(" 节课\n");
				mc = mc + CoursesFormat(today_courses);
			}
			else
			{
				mc.Plain("你今天没有课了!");
			}

			sendMsg(mc);

		}
		catch (const std::exception & ex)
		{
			cout << ex.what() << endl;
		}

	}
	void tomorrow_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg)
	{
		try
		{
			json courses;
			if (!userDb.Exist(target) || (courses = userDb.GetCourses(target)).empty())
			{
				sendMsg(MessageChain().Plain("没有查到你的课表，回复 \"绑定学号\" 启动本功能"));
				return;
			}

			// GetWeek()范围为[1,20]，修改为索引值，即[0,19]
			int week = GetWeekOfSemester() - 1;		// 获取本学期第几周
			int weekday = (GetWeekNum() + 1) % 7;				// 获取今天是星期几
			json today_courses = courses["data"][week][weekday];

			MessageChain mc;
			int count = today_courses.size();
			if (count > 0)
			{
				mc.Plain("你明天共有 ").Plain(count).Plain(" 节课\n");
				mc = mc + CoursesFormat(today_courses);
			}
			else
			{
				mc.Plain("你明天没有课了!");
			}

			sendMsg(mc);

		}
		catch (const std::exception & ex)
		{
			cout << ex.what() << endl;
		}
	}
	void week_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg)
	{
		const static string weekdayStr[7] = { "星期一","星期二","星期三","星期四","星期五","星期六","星期天" };
		try
		{
			json courses;
			if (!userDb.Exist(target) || (courses = userDb.GetCourses(target)).empty())
			{
				sendMsg(MessageChain().Plain("没有查到你的课表，回复 \"绑定学号\" 启动本功能"));
				return;
			}

			// GetWeek()范围为[1,20]，修改为索引值，即[0,19]
			int week = GetWeekOfSemester() - 1;		// 获取本学期第几周
			sendMsg(MessageChain().Plain("本周是第 ").Plain(week + 1).Plain(" 周"));
			for (int w = 0; w < 7; ++w)
			{
				json today_courses = courses["data"][week][w];

				MessageChain mc;
				int count = today_courses.size();
				if (count > 0)
				{
					mc.Plain("本周 ").Plain(weekdayStr[w]).Plain(" 你共有 ").Plain(count).Plain(" 节课\n");
					mc = mc + CoursesFormat(today_courses);
				}
				else
				{
					mc.Plain("本周 ").Plain(weekdayStr[w]).Plain(" 你没有课");
				}
				sendMsg(mc);
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

		}
		catch (const std::exception & ex)
		{
			cout << ex.what() << endl;
		}

	}
	void bind_sid(const vector<string>& cmds, QQ_t target, Sender sendMsg)
	{
		const static auto bdxh_format_help = MessageChain()
			.Plain("正确格式： ")
			.Plain("绑定学号<空格><学号><空格><教务网密码>\n")
			.Plain("例如：绑定学号 631805010000 mima123456\n\n")
			.Plain("机器人不会存储你的密码\n记录在服务器的只有你的QQ、学号和课表。");
		if (cmds.size() < 3)
		{
			sendMsg(bdxh_format_help);
			return;
		}

		try
		{
			json all_courses;
			all_courses["data"] = json::array();
			sendMsg(MessageChain().Plain("登录教务网中...\n(如果提示【验证码错误】或长时间未响应请重试)"));
			Student stu;
			stu.Login(cmds[1], cmds[2]);
			for (int i = 1; i <= 20; ++i)
			{
				vector<vector<Student::CourseCell>> courses;
				// 重试三次，第四次循环会抛出异常不继续获取
				for (size_t u = 1; u <= 4; u++)
				{
					if (u == 4) throw runtime_error("获取课表失败：超出最大重试次数，请过一段时间后重试!");
					try
					{
						sendMsg(MessageChain().Plain("正在获取第 ").Plain(i).Plain(" 周的课表..."));
						courses = stu.GetCourses("2019-2020-2", to_string(i));
						break;
					}
					catch (const std::exception & ex)
					{
						sendMsg(
							MessageChain()
							.Plain("获取第 ").Plain(i).Plain(" 周的课表失败：")
							.Plain(ex.what()).Plain("\n正在重试..."));
					}
				}
				json week_courses = json::array();
				for (int x = 0; x < courses.size(); ++x)
				{
					json day_courses = json::array();
					for (int y = 0; y < courses[x].size(); ++y)
					{
						if (courses[x][y].Valid)
						{
							json cell = json::object();
							cell["Name"] = courses[x][y].Name;
							cell["Instructor"] = courses[x][y].Instructor;
							cell["Classroom"] = courses[x][y].Classroom;
							cell["Time"] = courses[x][y].Time;
							day_courses.push_back(cell);
						}
					}
					week_courses.push_back(day_courses);
				}
				all_courses["data"].push_back(week_courses);
			}
			if (!userDb.Exist(target)) userDb.Add(target);
			userDb.UpdateSid(target, cmds[1]);
			userDb.UpdateCourses(target, all_courses);
			sendMsg(MessageChain().Plain("绑定成功！"));
		}
		catch (const std::exception & ex)
		{
			try
			{
				sendMsg(MessageChain().Plain("出现错误: ").Plain(ex.what()));
			}
			catch (const std::exception&ex2)
			{
				cout << ex2.what() << endl;
			}
		}

	}
	void nextweek_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg)
	{
		const static string weekdayStr[7] = { "星期一","星期二","星期三","星期四","星期五","星期六","星期天" };
		try
		{
			json courses;
			if (!userDb.Exist(target) || (courses = userDb.GetCourses(target)).empty())
			{
				sendMsg(MessageChain().Plain("没有查到你的课表，回复 \"绑定学号\" 启动本功能"));
				return;
			}

			// GetWeek()范围为[1,20]，修改为索引值，即[0,19]
			int week = GetWeekOfSemester() % 20;		// 获取本学期第几周
			sendMsg(MessageChain().Plain("下周是第 ").Plain(week + 1).Plain(" 周"));
			for (int w = 0; w < 7; ++w)
			{
				json today_courses = courses["data"][week][w];

				MessageChain mc;
				int count = today_courses.size();
				if (count > 0)
				{
					mc.Plain("下周 ").Plain(weekdayStr[w]).Plain(" 你共有 ").Plain(count).Plain(" 节课\n");
					mc = mc + CoursesFormat(today_courses);
				}
				else
				{
					mc.Plain("下周 ").Plain(weekdayStr[w]).Plain(" 你没有课");
				}
				sendMsg(mc);
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

		}
		catch (const std::exception & ex)
		{
			cout << ex.what() << endl;
		}

	}
	void what_week_is(const vector<string>& cmds, QQ_t target, Sender sendMsg)
	{
		int week = GetWeekOfSemester();
		sendMsg(MessageChain().Plain("这周是第 ").Plain(week).Plain(" 周"));
	}
} // namespace Cyan