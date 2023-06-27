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
	/*======== create a menu bar ========/**/

	wxMenuBar* p_menuBar1 = new wxMenuBar();

	wxMenu* p_menuHelp = new wxMenu();
	p_menuHelp->Append(new wxMenuItem(p_menuHelp, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL));
	Connect(idMenuAbout, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&FRAME_CLASS_NAME::OnAbout);
	p_menuBar1->Append(p_menuHelp, _("&Help"));

	SetMenuBar(p_menuBar1);

	/*======== create a drawingcanvas obj and bind the coustem events ========/**/
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_p_canvas = new DrawingCanvas(this, wxID_ANY, wxDefaultPosition, this->FromDIP(size));
	m_p_canvas->Bind(CANVAS_RECT_ADDED, &FRAME_CLASS_NAME::OnRectAdded, this);
	m_p_canvas->Bind(CANVAS_RECT_REMOVED, &FRAME_CLASS_NAME::OnRectRemoved, this);
	sizer->Add(m_p_canvas, 1, wxEXPAND | wxALL, 0);

	/*======== create a button panel ========/**/
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
	for (size_t i = 0; i < 10; i++) FRAME_CLASS_NAME::OnAddBtn(evt);
}

void FRAME_CLASS_NAME::OnAddBtn(
	wxCommandEvent& evt
) {
	wxButton* presedBtn = dynamic_cast<wxButton*>(evt.GetClientObject());

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
		, wxColor(colorDistrib(m_randGen))
		, "Num. " + std::to_string(m_p_canvas->GetObjectCount() + 1));
}

void FRAME_CLASS_NAME::OnRemoveBtn(
	wxCommandEvent& evt
) {
	if (m_p_canvas->GetObjectCount()) { m_p_canvas->RemoveTopRect(); }
}

void FRAME_CLASS_NAME::OnGameOneBtn(
	wxCommandEvent& evt
) {
	wxButton* gameOneBtn = dynamic_cast<wxButton*>(evt.GetEventObject());
	if (gameOneBtn == nullptr) throw std::runtime_error("Unknown batton");

	if (std::strcmp(gameOneBtn->GetLabel().ToStdString().c_str(), "Game1 - Start") == NULL)	{
		gameOneBtn->SetLabel("Game1 - Stop");
		m_gameOne.m_goals.first = m_gameOne.m_goals.second = NULL;
		m_gameOne.m_p_gameOnOffBtn = gameOneBtn;
		this->OnGameOneStart();
	}
	else{
		gameOneBtn->SetLabel("Game1 - Start");
		this->OnGameOneStop();
		/**/SetStatusText("...Corrects->" + std::to_string(m_gameOne.m_goals.first)
			+ "...Errors->" + std::to_string(m_gameOne.m_goals.second), 2);/**/
	}
		
}

void
FRAME_CLASS_NAME::OnAbout(
	wxCommandEvent& evnt
) {
	wxMessageBox(_("Developer:\n\nBakhtiiarAsadov n4k\nNikopol UKRAINE."
	"\nCtrl+LeftClick - push on top"
		"\nAlt+LeftClick - rotate"
		"\nDoubleLeftClick - delete"), _("wx_DrawingCanvas"));
	evnt.Skip();
}

void FRAME_CLASS_NAME::OnRectAdded(
	wxCommandEvent& evt
) {
	SetStatusText("Rect. named " + evt.GetString() + " added", 2);
}

void FRAME_CLASS_NAME::OnRectRemoved(
	wxCommandEvent& evt
) {
	SetStatusText("Rect. named " + evt.GetString() + " removed", 2);
}

void FRAME_CLASS_NAME::OnTopRectRemoved(
	wxCommandEvent& evt
) {/*counter for game*/

	int evtType = evt.GetEventType();

	if (evtType == CANVAS_TOP_RECT_REMOVED) {
		OnGameOneStop();
		OnGameOneStart();
		++m_gameOne.m_goals.first;
		--m_gameOne.m_goals.second;
	}
	else {
		++m_gameOne.m_goals.second;
	}
}

void FRAME_CLASS_NAME::OnGameOneStart(void)
{
	size_t durationSecOfRemove = 10;

	m_gameOne.m_round.SetOnIteration([this, durationSecOfRemove, countDown = durationSecOfRemove](
		)mutable ->void {
			if (--countDown) { SetStatusText("..." + std::to_string(countDown), 2); }
			else { countDown = durationSecOfRemove, m_gameOne.m_round.Stop(); }
		});

	/**SetStatusText("...Game->Over...", 2);/**/
	m_gameOne.m_round.SetOnStop([this](void
		)->void {
			if (m_p_canvas->GetObjectCount()) {
				wxCommandEvent evt(wxEVT_NULL);
				OnRemoveBtn(evt);
				m_gameOne.m_goals.second++;
			}

			if (m_p_canvas->GetObjectCount()) {
				m_gameOne.m_round.Start();
			}
			else {
				wxCommandEvent evt(wxEVT_BUTTON, (m_gameOne.m_p_gameOnOffBtn)->GetId());
				evt.SetEventObject(m_gameOne.m_p_gameOnOffBtn);
				OnGameOneBtn(evt);
			}
		});

	m_p_canvas->Bind(CANVAS_RECT_REMOVED, &FRAME_CLASS_NAME::OnTopRectRemoved, this);
	m_p_canvas->Bind(CANVAS_TOP_RECT_REMOVED, &FRAME_CLASS_NAME::OnTopRectRemoved, this);

	m_gameOne.m_round.Start();
}

void FRAME_CLASS_NAME::OnGameOneStop(void)
{
	m_gameOne.m_round.SetOnStop([](void)->void {});
	m_gameOne.m_round.Stop();
	m_p_canvas->Unbind(CANVAS_RECT_REMOVED, &FRAME_CLASS_NAME::OnTopRectRemoved, this);
	m_p_canvas->Unbind(CANVAS_TOP_RECT_REMOVED, &FRAME_CLASS_NAME::OnTopRectRemoved, this);
}

wxPanel* FRAME_CLASS_NAME::CreateBtnPanel(
	wxWindow* parent
) {
	wxPanel* panel = new wxPanel(parent);
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* addRectBtn = new wxButton(panel, wxID_ANY, "Add rectangle");
	addRectBtn->Bind(wxEVT_BUTTON, &FRAME_CLASS_NAME::OnAddBtn, this);
	sizer->Add(addRectBtn, 0, wxEXPAND | wxALL, 3);

	wxButton* removeRectBtn = new wxButton(panel, wxID_ANY, "Remove top rectangle");
	removeRectBtn->Bind(wxEVT_BUTTON, &FRAME_CLASS_NAME::OnRemoveBtn, this);
	sizer->Add(removeRectBtn, 0, wxEXPAND | wxALL, 3);

	wxButton* gameOneBtn = new wxButton(panel, wxID_ANY, "Game1 - Start");
	gameOneBtn->Bind(wxEVT_BUTTON, &FRAME_CLASS_NAME::OnGameOneBtn, this);
	sizer->Add(gameOneBtn, 0, wxEXPAND | wxALL, 3);

	panel->SetSizer(sizer);

	return panel;
}