#include "Cnvs_app.h"

wxIMPLEMENT_APP(APP_CLASS_NAME);
/*
    Error	LNK2019	unresolved external symbol _WinMain@16 referenced in function "int __cdecl invoke_main(void)"
(?invoke_main@@YAHXZ)	wxWid1	D:\2\MyProjects\wxWid1\MSVCRTD.lib(exe_winmain.obj)	1
    This macros(wxIMPLEMENT_APP) is need for creating a entrypoint for the app
in the system what is using now. EXAMPLE: for a Windows it is "main"
*/

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. APP_CLASS_NAME and
// not wxApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool APP_CLASS_NAME::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if (!wxApp::OnInit())
        return false;
    
	// create the main application window
    FRAME_CLASS_NAME* frame
        = new FRAME_CLASS_NAME("Create the main application window in \"bool APP_CLASS_NAME::OnInit()\""
            , wxApp::argv.GetArguments());
    
    /**wxApp::argc; wxApp::argv;/**/

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}