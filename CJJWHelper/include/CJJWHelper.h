#pragma once
#ifndef _CYAN_CJJWHelper_CJJWHelper_h__
#define _CYAN_CJJWHelper_CJJWHelper_h__

#include <CURLWrapper.h>
#include <HTMLReader.h>
#include <HamOCR.h>
#include <CyImg/Pixels.h>
#include <CyImg/algorithm/Binarize.hpp>
#include <CyImg/algorithm/Split.hpp>
#include <exception>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "Base64.hpp"
#include "encoding.hpp"
#include <fstream>
using namespace macaron;
using std::stringstream;
using std::string;
using std::vector;
using std::runtime_error;

namespace Cyan
{
	class Student
	{
	public:
		struct CourseCell
		{
			bool Valid = false;
			string Name;
			string Instructor;
			string Schedule;
			string Classroom;
			string Group;
			string Time;
		};
		Student()
		{
			hamOCR.LoadDataFile("./jw.hdat");
			//hamOCR.LoadDataFile("D:/jw.hdat");
		}
		~Student() = default;
		void Login(const string& cookie)
		{
			cc.parse(cookie);
		}
		void Login(const string& sid, const string& pwd)
		{
			HTTP http;
			// 下载验证码
			auto captcha_re = http.Get("http://jwgl.cqjtu.edu.cn/jsxsd/verifycode.servlet");
			if (captcha_re.Ready)
			{
				size_t len = captcha_re.Content.size();
				// 去掉 const
				char* t = const_cast<char*>(captcha_re.Content.data());
				// 转换为 uchar*
				unsigned char* tt = reinterpret_cast<unsigned char*>(t);
				Pixels img;
				img.LoadFromJpegBytes(tt, len);
				string captcha_str = OCR_CAPTCHA(img);
				str2lwr(captcha_str);

				stringstream pData;
				// a=insert&encoded=NjMxODA1MDEwNDA5%25%25%25MTIzMTIz&RANDOMCODE=c1c2
				pData
					<< "a=insert&encoded="
					<< Base64::Encode(sid)
					<< "%25%25%25"
					<< Base64::Encode(pwd)
					<< "&RANDOMCODE="
					<< captcha_str;
				auto result = http.Post("http://jwgl.cqjtu.edu.cn/jsxsd/xk/LoginToXk", pData.str());
				if (result.Ready)
				{
					HTMLDoc html;
					html.Parse(result.Content);
					string title;
					try
					{
						title = html["html"]["head"]["title"].GetInner();
					}
					catch (const std::exception&)
					{
						throw runtime_error("获取登录响应失败");
					}
					if (title == "学生个人中心")
					{
						//  /html/body/div[6]/div/div[1]
						try
						{
							this->name = html["html"]["body"]["div"][5]["div"]["div"][0].GetContent();
							trim(this->name);
							// 取出姓名
							this->name = this->name.substr(
								sizeof("姓名：") - 1,
								this->name.size() - sizeof("姓名：") + 1);
						}
						catch (const exception&)
						{
							throw runtime_error("获取学生信息时失败");
						}
						this->sid = sid;
					}
					else
					{
						string excpt;
						try
						{
							// /html/body/form/div/div/div[2]/div[1]/div[2]/font
							excpt = html["html"]["body"]["form"]["div"]["div"]["div"][1]["div"][0]["div"][1]["font"].GetInner();
							excpt = GB2312_to_UTF8(excpt);
						}
						catch (const std::exception&)
						{
							excpt = "获取错误信息时发生错误";
						}
						throw runtime_error(excpt.data());
					}
				}
				else
				{
					throw runtime_error(result.ErrorMsg.data());
				}
			}
			else
			{
				throw runtime_error(captcha_re.ErrorMsg.data());
			}
			cc = http.GetCookieContainer();
		}
		string GetSID() const
		{
			return sid;
		}
		string GetName() const
		{
			return name;
		}
		string GetCookie() const
		{
			return cc.toString();
		}
		vector<vector<CourseCell>> GetCourses(const string& semester, const string& week)
		{
			const static string time[] = {
				"8:20 - 10:00",
				"10:20 - 12:00",
				"14:00 - 15:40",
				"16:00 - 17:40",
				"19:00 - 20:40",
				"21:00 - 22:40" };
			vector<vector<CourseCell>> courses;
			courses.resize(7);
			for (size_t i = 0; i < 7; i++)
			{
				courses[i].resize(6);
			}
			HTTP http;
			http.SetCookieContainer(cc);
			string url = "http://jwgl.cqjtu.edu.cn/jsxsd/xskb/xskb_list.do";
			string data = "cj0701id=&zc=" + week + "&demo=&xnxq01id=" + semester + "&sfFD=1";
			auto re = http.Post(url, data);
			if (re.Ready)
			{
				string htmlStr = re.Content;
				HTMLDoc html;
				html.Parse(htmlStr);
				auto res = html.Search(
					[](const string& tagName, const Attributes& attrs)
					{
						return (tagName == "td" && attrs.Exist("valign", "top"));
					});
				for (size_t k = 7; k < res.size(); ++k)
				{
					CourseCell cell;
					int n = k - 7, y = n % 7, x = ((n + 1) - y) / 7;
					HTMLDoc d = res[k];
					string CName = d["div"].GetContent();
					if (CName != "&nbsp;")
					{
						cell.Valid = true;
						cell.Name = CName;
						HTMLDoc font = d["div"][1]["font"];
						cell.Instructor = font[0].GetInner();
						cell.Schedule = font[1].GetInner();
						try
						{
							// 可能有空教室
							cell.Classroom = font[2].GetInner();
							FotmatClassroom(cell);
						}
						catch (const std::exception&) {}


						try
						{
							cell.Group = font[3].GetInner();
						}
						catch (const std::exception&) {}

						cell.Time = time[x];

					}
					else
					{
						cell.Valid = false;
					}
					courses[y][x] = cell;
				}
			}
			else
			{
				throw runtime_error(re.ErrorMsg.data());
			}

			return courses;
		}
	private:
		string name;
		string sid;
		CookieContainer cc;
		HamOCR hamOCR;
		string OCR_CAPTCHA(Pixels& img)
		{
			RGB_Binarize(img, 170);
			auto a = SubPixels(img, Point(3, 4), Point(13, 20));
			auto b = SubPixels(img, Point(13, 4), Point(23, 20));
			auto c = SubPixels(img, Point(23, 4), Point(33, 20));
			auto d = SubPixels(img, Point(34, 4), Point(48, 20));
			stringstream ss;
			ss << hamOCR.OCR(a) << hamOCR.OCR(b) << hamOCR.OCR(c) << hamOCR.OCR(d);
			return ss.str();

		}

		inline void str2lwr(string& m)
		{
			for (size_t i = 0; i < m.size(); i++)
			{
				m[i] = (char)tolower(m[i]);
			}
		}

		void trim(string& s)
		{
			if (s.empty())
			{
				return;
			}
			s.erase(0, s.find_first_not_of(" "));
			s.erase(s.find_last_not_of(" ") + 1);
		}

		void FotmatClassroom(CourseCell& cell)
		{
			if (cell.Classroom.size() == 5)
			{
				cell.Classroom.insert(1, " - ");
			}
			else if (cell.Classroom.size() == 6)
			{
				cell.Classroom.insert(3, " - ");
			}
		}

	};


} // namespace Cyan

#endif // _CYAN_CJJWHelper_CJJWHelper_h__