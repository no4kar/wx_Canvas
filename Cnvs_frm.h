#pragma once

#include <wx/wx.h>
#include <wx/listbook.h>

#include <random>

#include "MyMacros.h"
#include "DrawingCnvs.h"
#include "ChartCtrl.h"



class FRAME_CLASS_NAME
    : public wxFrame
{
public:
    CLASS_CONSTRUCT(FRAME_CLASS_NAME)(const wxString& title, const wxArrayString& argv = {}, const wxPoint& pos = wxPoint(50, 70), const wxSize& size = wxSize(580, 400));
    CLASS_DESTRUCT(FRAME_CLASS_NAME)() {}

private:
    /*generate a random rectangle*/
    void OnAddButtonClick(wxCommandEvent& event);
    void OnRemoveButtonClick(wxCommandEvent& event);

    void OnRectAdded(wxCommandEvent& event);
    void OnRectRemoved(wxCommandEvent& event);

    wxPanel* CreateBtnPanel(wxWindow* parent);

    DrawingCanvas* m_p(canvas);
    ChartControl* m_p(chart);
    const wxArrayString& m(cmdLineArgs);

    std::mt19937 m(randGen);

    wxDECLARE_EVENT_TABLE();    /* any class wishing to process wxWidgets evnts must use this macro*/
};