#pragma once

#include <wx/wx.h>
#include <wx/listbook.h>

#include <random>

#include "MyMacros.h"
#include "DrawingCnvs.h"
#include "ChartCtrl.h"
#include "Pulsar.h"

/* IDs for the controls and the menu commands/**/
enum CANCAT2(FRAME_CLASS_NAME, _IDs)
{
    /*BUTTON*/
    ID_BTN1 = 1
        , ID_BTN2
        , ID_TEXTCTRL1
        , ID_TEXTCTRL2

        /*IDes for menubar*/
        , idMenuQuit
        , idMenuAbout
};

class FRAME_CLASS_NAME
    : public wxFrame
{
public:
    CLASS_CONSTRUCT(FRAME_CLASS_NAME)(const wxString& title, const wxArrayString& argv = {}, const wxPoint& pos = wxPoint(50, 70), const wxSize& size = wxSize(580, 400));
    CLASS_DESTRUCT(FRAME_CLASS_NAME)() {}

private:
    /*generate a random rectangle*/
    void OnAddBtn(wxCommandEvent& event);
    void OnRemoveBtn(wxCommandEvent& event);
    void OnGameOneBtn(wxCommandEvent& event);

    void OnRectAdded(wxCommandEvent& event);
    void OnRectRemoved(wxCommandEvent& event);
    void OnTopRectRemoved(wxCommandEvent& event);

    void OnAbout(wxCommandEvent& event);

    void OnGameOneStart(void);
    void OnGameOneStop(void);
    
    wxPanel* CreateBtnPanel(wxWindow* parent);

    DrawingCanvas* m_p(canvas);
    ChartControl* m_p(chart);
    const wxArrayString& m(cmdLineArgs);
    std::mt19937 m(randGen);

	struct GameOne {
		Pulsar m(round);
        std::pair<size_t, size_t> m(goals);
        wxButton* m_p(gameOnOffBtn);
	}m(gameOne);


    wxDECLARE_EVENT_TABLE();    /* any class wishing to process wxWidgets evnts must use this macro*/
};