#include "pch.h"
#include "TimerRadar.h"


std::wstring thisdirectory;
const int TIMER = 354864;
CRect Timer(100, 100, 300, 200);
std::wstring timeremaining(L"0:00");
TimerRadar::TimerRadar(std::wstring dir)
{
	thisdirectory = dir;
}

TimerRadar::~TimerRadar()
{
}

void TimerRadar::OnAsrContentLoaded(bool Loaded)
{
}

void TimerRadar::OnAsrContentToBeSaved()
{
}

void TimerRadar::OnRefresh(HDC hDC, int Phase)
{
	

	if (Phase == EuroScopePlugIn::REFRESH_PHASE_AFTER_LISTS) 
	{
		
			// Creating the gdi+ graphics
			Gdiplus::Graphics graphics(hDC);
			graphics.SetPageUnit(Gdiplus::UnitPixel);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			std::string temp = "timer.png";
			std::wstring timer = to_wstring(temp);
			std::wstring path = thisdirectory + timer;
			//Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(path.c_str());
			Gdiplus::SolidBrush timerbrush(Gdiplus::Color::Gray);
			Gdiplus::SolidBrush blackbrush(Gdiplus::Color::Black);
			Gdiplus::Pen mypen(Gdiplus::Color::Black);
			graphics.FillRectangle(&timerbrush,CopyRect(Timer));
			auto topleft = Timer.TopLeft();
			auto height = Timer.Height();
			auto width = Timer.Width();
			auto bottomright = Timer.BottomRight();
			//draw line seperating timer from time options
			graphics.DrawLine(&mypen, topleft.x, topleft.y + height / 3, bottomright.x, topleft.y + height / 3);
			//draw option seperating lines
			for (int i = 1; i <= 5; i++)
			{
				graphics.DrawLine(&mypen, topleft.x+i*width/5, int(topleft.y + height / 3), topleft.x + i * width / 5, bottomright.y);
			}
			graphics.DrawLine(&mypen, topleft.x, topleft.y + 2* height / 3, bottomright.x, topleft.y + 2* height / 3);
			AddScreenObject(TIMER, "timer", Timer, true, "");
			Gdiplus::FontFamily   fontFamily(L"Arial");
			Gdiplus::Font         fontoptions(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
			Gdiplus::Font         fontrem(&fontFamily, 20, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
			Gdiplus::RectF        rectFrem(topleft.x,topleft.y,width,height/3);
			std::vector<Gdiplus::RectF> optionrects;
			std::vector<std::wstring> times;
			times.push_back(L"1:20");
			times.push_back(L"1:40");
			times.push_back(L"2:00");
			times.push_back(L"2:20");
			times.push_back(L"2:40");
			times.push_back(L"3:00");
			times.push_back(L"3:20");
			times.push_back(L"3:40");
			times.push_back(L"4:00");
			times.push_back(L"C");
			
			for (int i = 1; i <= 5; i++)
			{
				for (int j = 1; j <= 2; j++)
				{
					optionrects.push_back(Gdiplus::RectF(topleft.x + (i - 1)*width / 5, topleft.y + j * height / 3, width/5, height / 3));
				}
			}
			Gdiplus::StringFormat stringFormat = new Gdiplus::StringFormat();
			stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
			stringFormat.SetLineAlignment( Gdiplus::StringAlignmentCenter);

			graphics.DrawString(timeremaining.c_str(), -1, &fontrem,rectFrem,&stringFormat , &blackbrush);
			for (int ij = 0 ; ij<10; ij++)
			{
				graphics.DrawString(times.at(ij).c_str(), -1, &fontoptions, optionrects.at(ij), &stringFormat, &blackbrush);
			}
			
			graphics.ReleaseHDC(hDC);
		
			return;
	}
}

void TimerRadar::OnClickScreenObject(int ObjectType, const char * sObjectId, POINT Pt, RECT Area, int Button)
{
}

void TimerRadar::OnMoveScreenObject(int ObjectType, const char * sObjectId, POINT Pt, RECT Area, bool Released)
{
	if (ObjectType != TIMER)
		return;
	Timer.MoveToXY(Pt.x, Pt.y);
	
	RequestRefresh();
}

void TimerRadar::OnOverScreenObject(int ObjectType, const char * sObjectId, POINT Pt, RECT Area)
{
}

bool TimerRadar::OnCompileCommand(const char * sCommandLine)
{
	return false;
}

void TimerRadar::OnRadarTargetPositionUpdate(EuroScopePlugIn::CRadarTarget RadarTarget)
{
}

void TimerRadar::OnFlightPlanDisconnect(EuroScopePlugIn::CFlightPlan FlightPlan)
{
}

std::string TimerRadar::GetBottomLine(const char * Callsign)
{
	return std::string();
}

void TimerRadar::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
}

inline void TimerRadar::OnAsrContentToBeClosed(void)
{
}
std::wstring TimerRadar::to_wstring(const std::string& str) {
	return std::filesystem::path(str).wstring();
}