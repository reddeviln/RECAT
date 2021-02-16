#pragma once
#pragma warning(disable : 4996)
#include "EuroScopePlugIn.h"
#include "pch.h"
#include <vector>
#include "csv.h"
#include "loguru.hpp"
class CRECAT :
	//The class that holds all our functions 
	public EuroScopePlugIn::CPlugIn
{
public:
	//the list displayed in euroscope



	CRECAT(void);



	virtual ~CRECAT(void);


	/*
	This function overrides a Euroscope function. If you type ".showtolist" in the euroscope textbox it will show the t/o sequence list
	Input: sCommandLine (the textbox string)
	*/
	static bool test()
	{
		return true;
	}

	bool fileExists(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			return true;
		}
		return false;
	}




	virtual void  OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
		EuroScopePlugIn::CRadarTarget RadarTarget,
		int ItemCode,
		int TagData,
		char sItemString[16],
		int * pColorCode,
		COLORREF * pRGB,
		double * pFontSize);
	
	virtual EuroScopePlugIn::CRadarScreen    *OnRadarScreenCreated(const char * sDisplayName,
		bool NeedRadarContent,
		bool GeoReferenced,
		bool CanBeSaved,
		bool CanBeCreated);

};