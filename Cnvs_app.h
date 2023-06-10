#pragma once

#include "Cnvs_frm.h"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------


// Define a new application type, each program should derive a class from wxApp
class APP_CLASS_NAME : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit() wxOVERRIDE;
};
/*
    class cApp doesn't do anything and need for getting start application
*/