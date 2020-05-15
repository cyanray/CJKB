#include "utils.h"
#include <vector>
#include <iostream>
#include <string>
#include "keywords_parser.h"
#include "commands.h"
using namespace std;
using namespace Cyan;

void TempMsgProcessor(TempMessage m)
{
	try
	{

		//m.Reply(MessageChain().Plain(R"(加我为好友可以收到"每日课表"和"教务新闻"哦!)"));

		auto senderQQ = m.Sender.QQ;
		string tmp = m.MessageChain.GetPlainText();
		if (tmp.empty()) return;
		vector<string> cmds;
		stringstream ss(tmp);
		while (getline(ss, tmp, ' ')) cmds.push_back(tmp);

		// 今天的课表
		bool condition1a = KeywordParser(cmds[0])
			.AndWith(AlterKeywords({ "今天","今日" }))
			.AndWith("课")
			.GetResult();
		bool condition1b = KeywordParser(cmds[0])
			.Equals("课表")
			.GetResult();
		if (condition1a || condition1b)
		{
			today_courses(cmds, senderQQ,
				[&](const MessageChain& mc)
				{
					bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, mc);
				});
			return;
		}
		// 明天的课表
		bool condition2a = KeywordParser(cmds[0])
			.AndWith(AlterKeywords({ "明天","明日" }))
			.AndWith("课")
			.GetResult();
		bool condition2b = KeywordParser(cmds[0])
			.AndWith("明天的")
			.GetResult();
		if (condition2a || condition2b)
		{
			tomorrow_courses(cmds, senderQQ,
				[&](const MessageChain& mc)
				{
					bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, mc);
				});
			return;
		}
		// 本周课表
		bool condition3 = KeywordParser(cmds[0])
			.AndWith(AlterKeywords({ "本周","这周" }))
			.AndWith("课")
			.GetResult();
		if (condition3)
		{
			week_courses(cmds, senderQQ,
				[&](const MessageChain& mc)
				{
					bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, mc);
				});
			return;

		}
		// 下周课表
		bool condition4 = KeywordParser(cmds[0])
			.AndWith(AlterKeywords({ "下周","下一周" }))
			.AndWith("课")
			.GetResult();
		if (condition4)
		{
			nextweek_courses(cmds, senderQQ,
				[&](const MessageChain& mc)
				{
					bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, mc);
				});
			return;

		}
		// 这是第几周
		bool condition5 = KeywordParser(cmds[0])
			.AndWith("第几周")
			.GetResult();
		if (condition5)
		{
			what_week_is(cmds, senderQQ,
				[&](const MessageChain& mc)
				{
					bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, mc);
				});
			return;

		}

		// 教务新闻
		if (cmds[0] == "教务新闻")
		{
			try
			{
				for (const auto& n : newsDb.GetLatestNews())
				{
					MessageChain news;
					news.Plain("标题: " + n.title + "\n");
					news.Plain("网址: " + n.url + "\n");
					news.Plain("时间: " + n.time);
					m.Reply(news);
					MiraiBot::SleepMilliseconds(200);
				}
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
			return;
		}

		// 帮助信息
		if (cmds[0] == "帮助")
		{
			m.Reply(MessageChain().Plain(Help_Msg));
			return;
		}

		// 绑定学号&下载课表
		if (cmds[0] == "绑定学号")
		{
			bind_sid(cmds, senderQQ,
				[&](const MessageChain& mc)
				{
					bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, mc);
				});
			return;
		}


	}
	catch (const std::exception& ex)
	{
		cout << "处理临时消息时出现错误: " << ex.what() << endl;
		cout << "  | " << m.MessageChain.ToString() << endl;
	}

}
