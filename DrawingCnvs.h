#pragma once

#include <wx/wx.h>
#include <list>

#include "MyMacros.h"

/*declarate own event macros for .Conect() or .Bind()*/
wxDECLARE_EVENT(CANVAS_RECT_ADDED, wxCommandEvent);
wxDECLARE_EVENT(CANVAS_RECT_REMOVED, wxCommandEvent);
wxDECLARE_EVENT(CANVAS_TOP_RECT_REMOVED, wxCommandEvent);

struct GraphicObject
{
    wxRect2DDouble rect;
    wxColor color;
    wxString text;
    wxAffineMatrix2D transform;
};

class DrawingCanvas : public wxWindow
{
public:
    CLASS_CONSTRUCT(DrawingCanvas)(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    CLASS_DESTRUCT(DrawingCanvas)() {}

    void AddRect(int width, int height, int centerX, int centerY, double angle, wxColor color, const std::string& text);
    void RemoveTopRect();
    int GetObjectCount() { return m_objList.size(); }
    
private:
    void OnPaint(wxPaintEvent& evt);
    /*return (this->m_objList.rend()) if not find*/
    auto GetClickedObj(wxMouseEvent& evt)const;
    
    /*which object is selected, should be rotated(alt-kay) or moved*/
    void OnMouseLDown(wxMouseEvent& evt);
    /*rotate or move*/
    void OnMouseMove(wxMouseEvent& evt);
    void OnMouseUp(wxMouseEvent& evt);
    void OnMouseLeave(wxMouseEvent& evt);/*cursore leave control aria*/
    void OnMouseLDClicked(wxMouseEvent& evt);

    void FinishDrag();
    void FinishRotation();

    void SendRectAddedEvent(const wxString& rectTitle);
    void SendRectRemovedEvent(const wxString& rectTitle);
    void SendTopRectRemovedEvent(const wxString& rectTitle);

    std::list<GraphicObject> m(objList);

    GraphicObject* m(draggedObj);
    bool m(shouldRotate);

    wxPoint2DDouble m(lastDragOrigin);
};