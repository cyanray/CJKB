#pragma once
#ifndef CJKB__course_database_h_H_
#define CJKB__course_database_h_H_
#include <string>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>

using std::string;
using std::vector;
using namespace nlohmann;

const static string DB_FILE_NAME = "cjkb.db";

namespace Cyan
{
	class UserDb
	{
	public:
		UserDb() :db_(DB_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
		{

		}

		void Add(QQ_t qq)
		{
			SQLite::Statement sql(db_, "INSERT INTO CJKB (qq) VALUES (?)");
			sql.bind(1, (int64_t)qq);
			sql.exec();
		}

		void Add(QQ_t qq, const string& sid)
		{
			SQLite::Statement sql(db_, "INSERT INTO CJKB (qq,sid) VALUES (?,?)");
			sql.bind(1, (int64_t)qq);
			sql.bind(2, sid);
			sql.exec();
		}

		void Add(QQ_t qq, const string& sid, const json& courses)
		{
			SQLite::Statement sql(db_, "INSERT INTO CJKB (qq,sid,courses) VALUES (?,?,?)");
			sql.bind(1, (int64_t)qq);
			sql.bind(2, sid);
			sql.bind(3, courses.dump());
			sql.exec();
		}

		bool Exist(QQ_t qq)
		{
			SQLite::Statement sql(db_, "SELECT sid FROM CJKB WHERE qq = ?");
			sql.bind(1, (int64_t)qq);
			if (sql.executeStep()) return true;
			else return false;
		}

		string GetSid(QQ_t qq)
		{
			SQLite::Statement sql(db_, "SELECT sid FROM CJKB WHERE qq = ?");
			sql.bind(1, (int64_t)qq);
			if (sql.executeStep())
			{
				return sql.getColumn(0);
			}
			else throw std::runtime_error("未查到数据");
		}

		json GetCourses(QQ_t qq)
		{
			SQLite::Statement sql(db_, "SELECT courses FROM CJKB WHERE qq = ?");
			sql.bind(1, (int64_t)qq);
			if (sql.executeStep())
			{
				string c = sql.getColumn(0);
				if (c.empty()) return json();
				else return json::parse(c);
			}
			else throw std::runtime_error("未查到数据");
		}

		void UpdateSid(QQ_t qq, const string& sid)
		{
			SQLite::Statement sql(db_, "UPDATE CJKB SET sid = ? WHERE qq = ?");
			sql.bind(1, sid);
			sql.bind(2, (int64_t)qq);
			sql.exec();
		}

		void UpdateCourses(QQ_t qq, const json& courses)
		{
			SQLite::Statement sql(db_, "UPDATE CJKB SET courses = ? WHERE qq = ?");
			sql.bind(1, courses.dump());
			sql.bind(2, (int64_t)qq);
			sql.exec();
		}

		vector<QQ_t> GetAllUser()
		{
			vector<QQ_t> res;
			SQLite::Statement sql(db_, "SELECT qq FROM CJKB");
			while (sql.executeStep())
			{
				QQ_t c = (QQ_t)((int64_t)(sql.getColumn(0)));
				res.push_back(c);
			}
			return res;
		}

		~UserDb() = default;

	private:
		SQLite::Database db_;
	};

	struct News
	{
		string title;
		string url;
		string time;
	};

	class NewsDb
	{
	public:
		NewsDb() :db_(DB_FILE_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
		{

		}

		void Add(const string& title,const string& date,const string& url)
		{
			SQLite::Statement sql(db_, "INSERT INTO CJNEWS (title, time, url) VALUES (?,?,?)");
			sql.bind(1, title);
			sql.bind(2, date);
			sql.bind(3, url);
			sql.exec();
		}


		bool Exist(const string& title)
		{
			SQLite::Statement sql(db_, "SELECT title FROM CJNEWS WHERE title = ?");
			sql.bind(1, title);
			if (sql.executeStep()) return true;
			else return false;
		}

		vector<News> GetLatestNews()
		{
			vector<News> res;
			SQLite::Statement sql(db_, "SELECT title, time, url FROM CJNEWS ORDER BY time DESC limit 5");
			while (sql.executeStep())
			{
				News n;
				string title = sql.getColumn(0);
				string time = sql.getColumn(1);
				string url = sql.getColumn(2);
				n.title = title;
				n.time = time;
				n.url = url;
				res.emplace_back(n);
			}
			return res;
		}

		~NewsDb() = default;

	private:
		SQLite::Database db_;
	};



} // namespace Cyan

#endif // !CJKB__keywords_parser_h_H_