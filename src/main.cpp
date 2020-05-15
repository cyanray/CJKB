#include <iostream>
#include <string>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "utils.h"
#include "cron.h"

//#define DEBUG 1



#if defined(_DEBUG) || defined(DEBUG)
const QQ_t BOT_QQ = 1589588851_qq;
const string INIT_KEY = "INITKEY7A3O1a9v";
MiraiBot bot("127.0.0.1", 539);
#else
const QQ_t BOT_QQ = 211795583_qq;
const string INIT_KEY = "InitKeyVl0CEUzZ";
MiraiBot bot("127.0.0.1", 539);
#endif
const QQ_t ADMIN_QQ = 484599279_qq;
UserDb userDb;
NewsDb newsDb;

int main()
{
	system("chcp 65001");
	srand(time(0));
	while (true)
	{
		try
		{
			bot.Auth(INIT_KEY, BOT_QQ);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;

	bot.On<FriendMessage>(FriendMsgProcessor);

	bot.On<GroupMessage>(GroupMsgProcessor);

	bot.On<TempMessage>(TempMsgProcessor);

	bot.On<NewFriendRequestEvent>([&](NewFriendRequestEvent e)
		{
			try
			{
				e.Accept();
				MiraiBot::SleepSeconds(5);
				bot.SendMessage(e.FromId, MessageChain().Plain(Help_Msg));
			}
			catch (const exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	auto f1 = std::async(std::launch::async, []() { cron_job_morning(); });
	auto f2 = std::async(std::launch::async, []() { corn_job_news(); });

	bot.EventLoop();

	return 0;
}