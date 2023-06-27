#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include "DrawingCnvs.h"

wxDEFINE_EVENT(CANVAS_RECT_ADDED, wxCommandEvent);
wxDEFINE_EVENT(CANVAS_RECT_REMOVED, wxCommandEvent);
wxDEFINE_EVENT(CANVAS_TOP_RECT_REMOVED, wxCommandEvent);


DrawingCanvas::DrawingCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size)
{
	this->m_draggedObj = nullptr;
	this->m_shouldRotate = false;

	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->SetBackgroundColour(wxColor(0, 0, 0, 255));
	
	this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
	this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseLDown, this);
	this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
	this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);/**/
	this->Bind(wxEVT_LEFT_DCLICK, &DrawingCanvas::OnMouseLDClicked, this);

	this->AddRect(this->FromDIP(200), this->FromDIP(160), this->FromDIP(150), this->FromDIP(140), -M_PI/3.0, *wxBLUE, "Sofiia");
	this->AddRect(this->FromDIP(50), this->FromDIP(30), this->FromDIP(180), this->FromDIP(210), 0.0,/*semitransperenty*/wxColor(255, 0, 255, 128), "&");
	this->AddRect(this->FromDIP(200), this->FromDIP(160), this->FromDIP(300), this->FromDIP(210), M_PI / 2.0, *wxYELLOW, "Eldar");

}

void DrawingCanvas::OnPaint(wxPaintEvent& evt
) {
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

		#if NULL
	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
		#else
	std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
		#endif
	
#if NULL
	if (gc)
    {
        // make a path that contains a circle and some lines
        gc->SetPen( *wxRED_PEN );
        wxGraphicsPath path = gc->CreatePath();
        path.AddCircle( 50.0, 50.0, 50.0 );
        path.MoveToPoint(0.0, 50.0);
        path.AddLineToPoint(100.0, 50.0);
        path.MoveToPoint(50.0, 0.0);
        path.AddLineToPoint(50.0, 100.0 );
        path.CloseSubpath();
        path.AddRectangle(25.0, 25.0, 50.0, 50.0);
        gc->StrokePath(path);
    }

#else

	if (gc) {
		for(const auto &obj : this->m_objList){
		gc->SetTransform(gc->CreateMatrix(obj.transform));

		gc->SetBrush(wxBrush(obj.color));
		gc->DrawRectangle(obj.rect.m_x, obj.rect.m_y, obj.rect.m_width, obj.rect.m_height);
		/*Some text*/
		gc->SetFont(*wxNORMAL_FONT, *wxWHITE);
		/*wxString text = "Sofiia & Eldar";*/
		double textWidth, textHeight;
		gc->GetTextExtent(obj.text, &textWidth, &textHeight);
		gc->DrawText(obj.text
			, obj.rect.m_x + obj.rect.m_width / 2.0 - textWidth / 2.0
			, obj.rect.m_y + obj.rect.m_height / 2.0 - textHeight / 2.0);
		}
	}
#endif/*if(gc){...*/
}

void DrawingCanvas::AddRect(
	int width
	, int height
	, int centerX
	, int centerY
	, double angle
	, wxColor color
	, const std::string& text
) {
	GraphicObject obj{
		{ -width / 2.0
		,-height / 2.0
		,static_cast<double>(width)
		,static_cast<double>(height) }/*.rect*/
		,color/*.color*/
		,text/*.text*/
		,{}/*.transform*/
	};

	obj.transform.Translate(static_cast<double>(centerX)
		, static_cast<double>(centerY));
	obj.transform.Rotate(angle);

	m_objList.push_back(obj);
	
	SendRectAddedEvent(text);
	Refresh();
}

void DrawingCanvas::RemoveTopRect()
{
	if (!m_objList.empty() && m_draggedObj == nullptr) {
		wxString text = m_objList.back().text;
		m_objList.pop_back();

		SendTopRectRemovedEvent(text);
		Refresh();
	}
}

auto DrawingCanvas::GetClickedObj(
	wxMouseEvent& evt
) const {
	return std::find_if(
		this->m_objList.rbegin()
		, this->m_objList.rend()
		, [evt](const GraphicObject& grphObj)->bool { /*[scale vision](args)->return{body}*/
			auto inv = grphObj.transform;
			inv.Invert();
			wxPoint2DDouble clickPos = inv.TransformPoint(evt.GetPosition());
			return grphObj.rect.Contains(clickPos); }
	);
}

void DrawingCanvas::OnMouseLDClicked(
	wxMouseEvent& evt
) {
	auto clickedObjIter = GetClickedObj(evt);

	if (clickedObjIter /*if exist*/ != this->m_objList.rend()
		) {
		bool isTop = (&(*clickedObjIter) == &m_objList.back());
		wxString text = (*clickedObjIter).text;
		this->m_objList.erase(/* ".erase()" accept only forward iter */std::prev(clickedObjIter.base()));
		SendRectRemovedEvent(text); 
		if (isTop) SendTopRectRemovedEvent(text);
		Refresh();
	}
}

void DrawingCanvas::OnMouseLDown(
	wxMouseEvent& evt
) {
	auto clickedObjIter = GetClickedObj(evt);

	/*start to drag or rotation process*/

	/*lClick+Ctrl push object on the top of stack, that mean last in the objList*/
	if (clickedObjIter /*if exist*/ != this->m_objList.rend()) {
		if (wxGetKeyState(WXK_CONTROL)){
			auto forwardIt = std::prev(clickedObjIter.base());/* ".erase()" accept only forward iter */
			this->m_objList.push_back(*forwardIt);
			this->m_objList.erase(forwardIt);
			clickedObjIter = m_objList.rbegin();
		}


		this->m_draggedObj = (GraphicObject*)&(*clickedObjIter/**std::prev(this->m_objList.end())/**/);
		this->m_lastDragOrigin = evt.GetPosition();
		this->m_shouldRotate = wxGetKeyState(WXK_ALT);/*if preset 'alt' key*/

		Refresh();
	}
}

void DrawingCanvas::OnMouseMove(
	wxMouseEvent& evt
){
	if (this->m_draggedObj != nullptr) {
		if (this->m_shouldRotate) {
			double absDiffY = evt.GetPosition().y - m_lastDragOrigin.m_y;
			m_draggedObj->transform.Rotate(absDiffY / 100.0 * M_PI);
		}
		else {
			auto dragVctr = evt.GetPosition() - m_lastDragOrigin;
			/*need to translate user dragVctr in corect altVctr for TRS matrix*/
			auto inv = m_draggedObj->transform;
			inv.Invert();
			dragVctr = inv.TransformDistance(dragVctr);
			m_draggedObj->transform.Translate(dragVctr.m_x, dragVctr.m_y);
		}
		m_lastDragOrigin = evt.GetPosition();
		Refresh();
	}
}

void DrawingCanvas::OnMouseUp(wxMouseEvent& evt
) {
	this->FinishDrag(); this->FinishRotation();
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent& evt
) {
	this->FinishDrag(); this->FinishRotation();
}

void DrawingCanvas::FinishDrag(
) {
	m_draggedObj = nullptr;
}

void DrawingCanvas::FinishRotation(
) {
	m_shouldRotate = false;
}

void DrawingCanvas::SendRectAddedEvent(const wxString& rectTitle
){
	wxCommandEvent evt(CANVAS_RECT_ADDED, GetId());
	evt.SetEventObject(this);
	evt.SetString(rectTitle);
	ProcessWindowEvent(evt);
}

void DrawingCanvas::SendRectRemovedEvent(const wxString& rectTitle
) {
	wxCommandEvent evt(CANVAS_RECT_REMOVED, GetId());
	evt.SetEventObject(this);
	evt.SetString(rectTitle);
	ProcessWindowEvent(evt);
}

void DrawingCanvas::SendTopRectRemovedEvent(const wxString& rectTitle
) {
	wxCommandEvent evt(CANVAS_TOP_RECT_REMOVED, GetId());
	evt.SetEventObject(this);
	evt.SetString(rectTitle);
	ProcessWindowEvent(evt);
}
