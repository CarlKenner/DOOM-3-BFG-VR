/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).  

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include "../../idlib/precompiled.h"
#pragma hdrstop

#define IDD_NEWNAME                              1000
#define IDD_RENDERBUMPFLAT                       1001
#define IDD_TOOLS_OPEN                           1002
#define IDD_DIALOG_GOTOLINE                      1003
#define IDB_TOOLS_OPEN                           1004
#define IDB_TOOLS_BACK                           1005

#define IDC_TOOLS_EDITNAME                       1200
#define IDC_TOOLS_BACK                           1201
#define IDC_TOOLS_LOOKIN                         1202
#define IDC_TOOLS_FILENAME                       1203
#define IDC_TOOLS_FILELIST                       1204
#define IDC_RBF_WIDTH                            1205
#define IDC_RBF_HEIGHT                           1206
#define IDC_RBF_FILENAME                         1207
#define IDC_GOTOLINE_STATIC                      1208
#define IDC_GOTOLINE_EDIT                        1209

idCVar rbfg_DefaultWidth( "rbfg_DefaultWidth", "0", 0, "" );
idCVar rbfg_DefaultHeight( "rbfg_DefaultHeight", "0", 0, "" );

static idStr RBFName;

static bool CheckPow2(int Num)
{
	while(Num)
	{
		if ((Num & 1) && (Num != 1))
		{
			return false;
		}

		Num >>= 1;
	}

	return true;
}

extern void Com_WriteConfigToFile( const char *filename );

static BOOL CALLBACK RBFProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{  
    switch (message) 
    { 
        case WM_INITDIALOG: 
			SetDlgItemInt(hwndDlg, IDC_RBF_WIDTH, rbfg_DefaultWidth.GetInteger(), FALSE);
			SetDlgItemInt(hwndDlg, IDC_RBF_HEIGHT, rbfg_DefaultHeight.GetInteger(), FALSE);
			SetDlgItemText(hwndDlg, IDC_RBF_FILENAME, RBFName);
            return TRUE; 
 
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
					{
						int		width, height;

						width = GetDlgItemInt(hwndDlg, IDC_RBF_WIDTH, 0, FALSE);
						height = GetDlgItemInt(hwndDlg, IDC_RBF_HEIGHT, 0, FALSE);

						rbfg_DefaultWidth.SetInteger( width );
						rbfg_DefaultHeight.SetInteger( height );

						//ANON:Linking errors plus code isnt included
						//Com_WriteConfigToFile( CONFIG_FILE );

						if (!CheckPow2(width) || !CheckPow2(height))
						{
							return TRUE;
						}

						DestroyWindow(hwndDlg); 

						cmdSystem->BufferCommandText( CMD_EXEC_APPEND, va("renderbumpflat -size %d %d %s\n", width, height, RBFName.c_str() ) );
	                    return TRUE; 
					}
 
                case IDCANCEL: 
                    DestroyWindow(hwndDlg); 
                    return TRUE; 
            } 
    }

    return FALSE; 
} 

void DoRBFDialog(const char *FileName)
{
	RBFName = FileName;

	Sys_GrabMouseCursor( false );

	DialogBox(0, MAKEINTRESOURCE(IDD_RENDERBUMPFLAT), 0, (DLGPROC)RBFProc); 

	Sys_GrabMouseCursor( true );
}
