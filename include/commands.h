#pragma once
#ifndef CJKB__command_h_H_
#define CJKB__command_h_H_
#include <string>
#include <vector>
#include <mirai.h>
#include "utils.h"
using std::string;
using std::vector;

namespace Cyan
{

	void today_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg);
	void tomorrow_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg);
	void week_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg);
	void bind_sid(const vector<string>& cmds, QQ_t target, Sender sendMsg);
	void nextweek_courses(const vector<string>& cmds, QQ_t target, Sender sendMsg);
	void what_week_is(const vector<string>& cmds, QQ_t target, Sender sendMsg);
} // namespace Cyan

#endif // !CJKB__keywords_parser_h_H_