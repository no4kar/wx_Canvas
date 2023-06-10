#include "Cnvs_frm.h"

#if 1
wxBEGIN_EVENT_TABLE(FRAME_CLASS_NAME, wxFrame)
/*EVT_BUTTON*/
/*EVT_BUTTON(BTN1, FRAME_CLASS_NAME::OnBtn1Clicked)/*for one-time binding*/
/*EVT_MENU*/
wxEND_EVENT_TABLE()
#endif

/*size_t FRAME_CLASS_NAME::m_rows = 0;/*static member initializing*/
/*
	class FRAME_CLASS_NAME is going to be a graphical interface component.
	Derived (or inherited) by ::wxFrame class.
	(is going to be a FORM in a "windows" language)
*/



FRAME_CLASS_NAME::FRAME_CLASS_NAME(const wxString& title, const wxArrayString& argv, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
	, m_cmdLineArgs(argv)
{

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	m_p_canvas = new DrawingCanvas(this, wxID_ANY, wxDefaultPosition, this->FromDIP(size));
	m_p_canvas->Bind(CANVAS_RECT_ADDED, &FRAME_CLASS_NAME::OnRectAdded, this);
	m_p_canvas->Bind(CANVAS_RECT_REMOVED, &FRAME_CLASS_NAME::OnRectRemoved, this);
	sizer->Add(m_p_canvas, 1, wxEXPAND | wxALL, 0);

	auto btnPanel = CreateBtnPanel(this);
	sizer->Add(btnPanel, 0, wxEXPAND | wxALL, 0);

	this->SetSizerAndFit(sizer);

	/*======== create a status bar just for fun (by default with 1 pane only) ========/**/
#if wxUSE_STATUSBAR
	CreateStatusBar(3);/*below of the window*/
	SetStatusText("\"FRAME_CLASS_NAME::FRAME_CLASS_NAME(const wxString& title)\"", 0);
	SetStatusText("One is glad to be of service", 1);
#endif

	wxCommandEvent evt(wxEVT_NULL);
	for (size_t i = 0; i < 10; i++) FRAME_CLASS_NAME::OnAddButtonClick(evt);
}

void FRAME_CLASS_NAME::OnAddButtonClick(
	wxCommandEvent& evt
) {
	std::uniform_int_distribution<>
		sizeDistrib(FromDIP(50), FromDIP(200))/*random size*/
		, xDistrib(0, m_p_canvas->GetSize().GetWidth())/*random x*/
		, yDistrib(0, m_p_canvas->GetSize().GetHeight())/*random y*/
		, colorDistrib(0, 0xFFFFFF);/*random color*/
	
	std::uniform_real_distribution<>
		angleDistrib(0.0, M_PI * 2.0);/*random rotate angle from 0 to 360*/
	
	m_p_canvas->AddRect(sizeDistrib(m_randGen)
		, sizeDistrib(m_randGen)
		, xDistrib(m_randGen)
		, yDistrib(m_randGen)
		, angleDistrib(m_randGen)
		, colorDistrib(m_randGen)
		, "Num. " + std::to_string(m_p_canvas->GetObjectCount() + 1));
}

void FRAME_CLASS_NAME::OnRemoveButtonClick(
	wxCommandEvent& evt
){
	if(m_p_canvas->GetObjectCount()){ m_p_canvas->RemoveTopRect(); }
}

void FRAME_CLASS_NAME::OnRectAdded(
	wxCommandEvent& evt
){
	SetStatusText("Rect named "+ evt.GetString()+" added", 2);
}

void FRAME_CLASS_NAME::OnRectRemoved(
	wxCommandEvent& evt
){
	SetStatusText("Rect named " + evt.GetString() + " removed", 2);
}

wxPanel* FRAME_CLASS_NAME::CreateBtnPanel(
	wxWindow* parent
){
	wxPanel* panel = new wxPanel(parent);

	wxButton* addRectBtn = new wxButton(panel, wxID_ANY, "Add rectangle");
	addRectBtn->Bind(wxEVT_BUTTON, &FRAME_CLASS_NAME::OnAddButtonClick, this);

	wxButton* removeRectBtn = new wxButton(panel, wxID_ANY, "Remove top rectangle");
	removeRectBtn->Bind(wxEVT_BUTTON, &FRAME_CLASS_NAME::OnRemoveButtonClick, this);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(addRectBtn, 0, wxEXPAND | wxALL, 3);
	sizer->Add(removeRectBtn, 0, wxEXPAND | wxALL, 3);

	panel->SetSizer(sizer);

	return panel;
}