//
// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
//// BlockView.cpp : Initialization functions
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
//#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "StdArx.h"
#include "resource.h"
#include <afxdllx.h>



// This command registers an ARX command.
void AddCommand(const TCHAR* cmdGroup, const TCHAR* cmdInt, const TCHAR* cmdLoc,
                const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal = -1);

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_MSG
void InitApplication();
void UnloadApplication();
//}}AFX_ARX_MSG

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_ADDIN_FUNCS
//}}AFX_ARX_ADDIN_FUNCS

////////////////////////////////////////////////////////////////////////////
//
// Define the sole extension module object.
AC_IMPLEMENT_EXTENSION_MODULE(BlockViewDLL);

// Now you can use the CAcModuleRecourceOverride class in
// your application to switch to the correct resource instance.
// Please see the ObjectARX Documentation for more details

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // Extension DLL one time initialization
        BlockViewDLL.AttachInstance(hInstance);
    } else if (dwReason == DLL_PROCESS_DETACH) {
        // Terminate the library before destructors are called
        BlockViewDLL.DetachInstance();
    }
    return TRUE;    // ok
}


/////////////////////////////////////////////////////////////////////////////
// ObjectARX EntryPoint
extern "C" AcRx::AppRetCode 
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
    switch (msg) {
    case AcRx::kInitAppMsg:
        // Comment out the following line if your
        // application should be locked into memory
        acrxDynamicLinker->unlockApplication(pkt);
        acrxDynamicLinker->registerAppMDIAware(pkt);
        InitApplication();
        break;
    case AcRx::kUnloadAppMsg:
        UnloadApplication();
        break;
    }
    return AcRx::kRetOK;
}

// Init this application. Register your
// commands, reactors...
void InitApplication()
{
    // NOTE: DO NOT edit the following lines.
    //{{AFX_ARX_INIT
    AddCommand(_T("ASDK"), _T("BLOCKVIEW"), _T("BVIEW"), ACRX_CMD_MODAL, AsdkBlockView);
    AddCommand(_T("ASDK"), _T("RenderOffScreen"), _T("ROS"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, AsdkRenderOffScreen);
    AddCommand(_T("ASDK"), _T("CONFIGGS"), _T("CONFIGGS"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, AsdkConfigGS);
    //}}AFX_ARX_INIT

  acutPrintf(_T("\nBlockView Sample Loaded"));
  acutPrintf(_T("\n\nCommands are:"));
  acutPrintf(_T("\n  Bview - display the BlockView dialog"));
  acutPrintf(_T("\n  ROS - Render off screen image"));
  acutPrintf(_T("\n  Configgs - Programatically display the Graphics settings\n"));
}

// Unload this application. Unregister all objects
// registered in InitApplication.
void UnloadApplication()
{
    // NOTE: DO NOT edit the following lines.
    //{{AFX_ARX_EXIT
    acedRegCmds->removeGroup(_T("ASDK"));
    //}}AFX_ARX_EXIT
}

// This functions registers an ARX command.
// It can be used to read the localized command name
// from a string table stored in the resources.
void AddCommand(const TCHAR* cmdGroup, const TCHAR* cmdInt, const TCHAR* cmdLoc,
                const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal)
{
    TCHAR cmdLocRes[65];

    // If idLocal is not -1, it's treated as an ID for
    // a string stored in the resources.
    if (idLocal != -1) {
        HMODULE hModule = GetModuleHandle(_T("AsdkBlockView.arx"));

        // Load strings from the string table and register the command.
        ::LoadString(hModule, idLocal, cmdLocRes, 64);
        acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLocRes, cmdFlags, cmdProc);

    } else
        // idLocal is -1, so the 'hard coded'
        // localized function name is used.
        acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLoc, cmdFlags, cmdProc);
}

