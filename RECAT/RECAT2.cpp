#include "pch.h"
#include "RECAT2.h"
#include <string>
#include <ctime>
#include <algorithm>
#include <regex>
#include "resource.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "loguru.cpp"
#include <unordered_map>
#include <utility>
#include <random>

#define MY_PLUGIN_NAME      "RECAT UAEvACC"
#define MY_PLUGIN_VERSION   "1.0"
#define MY_PLUGIN_DEVELOPER "Nils Dornbusch"
#define MY_PLUGIN_COPYRIGHT "Free to be distributed as source code"
#define MY_PLUGIN_VIEW      ""
const int TAG_ITEM_RECAT = 155;
const int TAG_ITEM_RECAT_NOSLASH = 12341;
std::unordered_map<std::string, std::string> recatdict;
CRECAT::CRECAT(void)
	: CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
		MY_PLUGIN_NAME,
		MY_PLUGIN_VERSION,
		MY_PLUGIN_DEVELOPER,
		MY_PLUGIN_COPYRIGHT)
{
	char path[MAX_PATH];
	HMODULE hm = NULL;

	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&CRECAT::test, &hm) == 0)
	{
		int ret = GetLastError();
		fprintf(stderr, "GetModuleHandle failed, error = %d\n", ret);
		// Return or however you want to handle an error.
	}
	if (GetModuleFileName(hm, path, sizeof(path)) == 0)
	{
		int ret = GetLastError();
		fprintf(stderr, "GetModuleFileName failed, error = %d\n", ret);
		// Return or however you want to handle an error.
	}
	std::string dir(path);
	std::string filename("RECAT.dll");
	size_t pos = dir.find(filename);
	dir.replace(pos, filename.length(), "");
	loguru::add_file("RECAT.log", loguru::Append, loguru::Verbosity_INFO);
	char logpath[1024];
	loguru::set_thread_name("RECAT");
	loguru::suggest_log_path(dir.c_str(), logpath, sizeof(logpath));
	loguru::add_file(logpath, loguru::FileMode::Truncate, loguru::Verbosity_INFO);
	LOG_F(INFO, "We successfully started RECAT. Opa!");

	RegisterTagItemType("RECAT", TAG_ITEM_RECAT);
	RegisterTagItemType("RECATnoSlash", TAG_ITEM_RECAT_NOSLASH);


	LOG_F(INFO, "Everything registered. Ready to go!");
	dir += "OMDB.csv";
	io::CSVReader<2, io::trim_chars<' '>, io::no_quote_escape<','>> in(dir);
	in.read_header(io::ignore_extra_column, "Aircraft","RECAT");
	std::string aircraft,recat;
	while (in.read_row(aircraft,recat))
	{
		std::pair<std::string, std::string> temp(aircraft, recat);
		recatdict.insert(temp);
	}
	LOG_F(INFO, "RECAT dictionary read without issues!");
	


}
void CRECAT::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
	EuroScopePlugIn::CRadarTarget RadarTarget,
	int ItemCode,
	int TagData,
	char sItemString[16],
	int * pColorCode,
	COLORREF * pRGB,
	double * pFontSize)
{
	if (ItemCode != TAG_ITEM_RECAT && ItemCode != TAG_ITEM_RECAT_NOSLASH) return;
	std::string type = FlightPlan.GetFlightPlanData().GetAircraftInfo();
	if (type.find("/") != std::string::npos)
	{
		std::regex re = std::regex(R"(\/(.{4})\/)");
		std::smatch match;
		if (std::regex_search(type, match, re))
		{
			type = match.str(1);
		}
	}
	auto found = recatdict.find(type);
	if (found != recatdict.end())
	{
		std::string out =  found->second;
		if (ItemCode == TAG_ITEM_RECAT)
			out = "/" + found->second;
		strcpy(sItemString,out.c_str());
		return;
	}
	else
	{
		char wtc = FlightPlan.GetFlightPlanData().GetAircraftWtc();
		std::string temp;
		switch (wtc)
		{
		case 'J':
		{
			temp = "A";
			break;
		}
		case 'H':
		{
			temp = "B";
			break;
		}
		case 'M':
		{
			temp = "D";
			break;
		}
		case 'L':
		{
			temp = "G";
			break;
		}
		}
		std::string out;
		if (ItemCode == TAG_ITEM_RECAT)
			out = "/" + temp;
		else
			out = temp;
		strcpy(sItemString, out.c_str());
	}
}
CRECAT::~CRECAT(void)
{
}