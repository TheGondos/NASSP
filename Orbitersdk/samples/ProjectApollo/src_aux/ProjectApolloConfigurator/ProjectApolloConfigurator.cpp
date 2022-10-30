/***************************************************************************
  This file is part of Project Apollo - NASSP
  Copyright 2003-2006 

  Project Apollo Configurator

  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************************************************************/

#pragma warning(disable : 4996 ) 

#define STRICT 1
#define ORBITER_MODULE
// To force Orbitersdk.h to use <fstream> in any compiler version
#pragma include_alias( <fstream.h>, <fstream> )
#include "Orbitersdk.h"
#include <stdio.h>
#include <cstring>
#include <imgui.h>
//#include "vesim.h"

// ==============================================================
// Some global parameters

#define MAX_TABNUM 3

// file name for storing custom parameters
static const char *cfgfile = "ProjectApollo/Saturn5.launchpad.cfg";

class ProjectApolloConfigurator;

///
/// \ingroup DLLsupport
///
static struct {
	ProjectApolloConfigurator *item;

	int Saturn_MainPanelSplit;
	int Saturn_GNSplit;
	int Saturn_LowRes;
	int Saturn_FDAIDisabled;
	int Saturn_FDAISmooth;
	int Saturn_RHC;
	int Saturn_THC;
	int Saturn_RSL;
	int Saturn_TSL;
	int Saturn_RHCTHCToggle;
	int Saturn_RHCTHCToggleId;
	int Saturn_VESIM;
	int Saturn_MaxTimeAcceleration;
	int Saturn_MultiThread;
	int Saturn_VAGCChecklistAutoSlow;
	int Saturn_VAGCChecklistAutoEnabled;
	int Saturn_VcInfoEnabled;
} gParams;


// ==============================================================
// A class defining the new launchpad parameter item
// This opens a dialog box for a user-defined item, and writes
// the value to a file to be read next time.
// ==============================================================

///
/// \ingroup Config
///
class ProjectApolloConfigurator: public LaunchpadItem {
public:
	ProjectApolloConfigurator();
	const char *Name() override { return "Project Apollo Configuration"; }
	const char *Description() override { return "Global configuration for Project Apollo - NASSP."; }
	int clbkWriteConfig () override;
	void Draw() override;

    void DrawVisuals();
	void DrawControls();
	void DrawMisc();

protected:
	void WriteConfig(FILEHANDLE hFile);
};

ProjectApolloConfigurator::ProjectApolloConfigurator (): LaunchpadItem ()
{
	char *line;
	// Read the current parameter values from file
	FILEHANDLE hFile = oapiOpenFile(cfgfile, FILE_IN, CONFIG);
	while (oapiReadScenario_nextline(hFile, line)) {
		if (!strnicmp (line, "MAINPANELSPLIT", 14)) {
			sscanf (line + 14, "%i", &gParams.Saturn_MainPanelSplit);
		} else if (!strnicmp (line, "LOWRES", 6)) {
			sscanf (line + 6, "%i", &gParams.Saturn_LowRes);
		} else if (!strnicmp (line, "FDAIDISABLED", 12)) {
			sscanf (line + 12, "%i", &gParams.Saturn_FDAIDisabled);
		} else if (!strnicmp (line, "FDAISMOOTH", 10)) {
			sscanf (line + 10, "%i", &gParams.Saturn_FDAISmooth);
		} else if (!strnicmp (line, "GNSPLIT", 7)) {
			sscanf (line + 7, "%i", &gParams.Saturn_GNSplit);
		} else if (!strnicmp (line, "MAXTIMEACC", 10)) {
			sscanf (line + 10, "%i", &gParams.Saturn_MaxTimeAcceleration);
		} else if (!strnicmp (line, "MULTITHREAD", 11)) {
			sscanf (line + 11, "%i", &gParams.Saturn_MultiThread);
		} else if (!strnicmp (line, "VAGCCHECKLISTAUTOSLOW", 21)) {
			sscanf (line + 21, "%i", &gParams.Saturn_VAGCChecklistAutoSlow);
		} else if (!strnicmp (line, "VAGCCHECKLISTAUTOENABLED", 24)) {
			sscanf (line + 24, "%i", &gParams.Saturn_VAGCChecklistAutoEnabled);
		} else if (!strnicmp (line, "JOYSTICK_RHC", 12)) {
			sscanf (line + 12, "%i", &gParams.Saturn_RHC);
		} else if (!strnicmp (line, "JOYSTICK_THC", 12)) {
			sscanf (line + 12, "%i", &gParams.Saturn_THC);
		} else if (!strnicmp(line, "JOYSTICK_RSL", 12)) {
			sscanf(line + 12, "%i", &gParams.Saturn_RSL);
		} else if (!strnicmp(line, "JOYSTICK_TSL", 12)) {
			sscanf(line + 12, "%i", &gParams.Saturn_TSL);
		} else if (!strnicmp (line, "JOYSTICK_RTTID", 14)) {
			sscanf (line + 14, "%i", &gParams.Saturn_RHCTHCToggleId);
		} else if (!strnicmp (line, "JOYSTICK_RTT", 12)) {
			sscanf (line + 12, "%i", &gParams.Saturn_RHCTHCToggle);			
		} else if (!strnicmp (line, "JOYSTICK_VESIM", 14)) {
			sscanf(line + 14, "%i", &gParams.Saturn_VESIM);
		} else if (!strnicmp(line, "VCINFOENABLED", 13)) {
			sscanf(line + 13, "%i", &gParams.Saturn_VcInfoEnabled);
		}
	}	
	oapiCloseFile (hFile, FILE_IN);
}

int ProjectApolloConfigurator::clbkWriteConfig ()
{
	// called when orbiter needs to write its configuration to disk
	WriteConfig(oapiOpenFile(cfgfile, FILE_OUT, CONFIG));
	WriteConfig(oapiOpenFile("ProjectApollo/Saturn1b.launchpad.cfg", FILE_OUT, CONFIG));
	WriteConfig(oapiOpenFile("ProjectApollo/LEM.launchpad.cfg", FILE_OUT, CONFIG));

	return 0;
}

void ProjectApolloConfigurator::WriteConfig(FILEHANDLE hFile)
{
	char cbuf[1000];

	sprintf(cbuf, "LOWRES %d", gParams.Saturn_LowRes);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "MAINPANELSPLIT %d", gParams.Saturn_MainPanelSplit);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "GNSPLIT %d", gParams.Saturn_GNSplit);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "FDAIDISABLED %d", gParams.Saturn_FDAIDisabled);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "FDAISMOOTH %d", gParams.Saturn_FDAISmooth);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "MAXTIMEACC %d", gParams.Saturn_MaxTimeAcceleration);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "MULTITHREAD %d", gParams.Saturn_MultiThread);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "VAGCCHECKLISTAUTOSLOW %d", gParams.Saturn_VAGCChecklistAutoSlow);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "VAGCCHECKLISTAUTOENABLED %d", gParams.Saturn_VAGCChecklistAutoEnabled);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "JOYSTICK_RHC %d", gParams.Saturn_RHC);
	oapiWriteLine(hFile, cbuf);

	oapiWriteLine(hFile, "JOYSTICK_RAUTO");	// Not configurable currently

	sprintf(cbuf, "JOYSTICK_RTT %d", gParams.Saturn_RHCTHCToggle);
	oapiWriteLine(hFile, cbuf);
	
	sprintf(cbuf, "JOYSTICK_RTTID %d", gParams.Saturn_RHCTHCToggleId);
	oapiWriteLine(hFile, cbuf);
	
	sprintf(cbuf, "JOYSTICK_THC %d", gParams.Saturn_THC);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "JOYSTICK_RSL %d", gParams.Saturn_RSL);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "JOYSTICK_TSL %d", gParams.Saturn_TSL);
	oapiWriteLine(hFile, cbuf);

	sprintf(cbuf, "JOYSTICK_VESIM %d", gParams.Saturn_VESIM);
	oapiWriteLine(hFile, cbuf);

	oapiWriteLine(hFile, "JOYSTICK_TAUTO");	// Not configurable currently

	sprintf(cbuf, "VCINFOENABLED %d", gParams.Saturn_VcInfoEnabled);
	oapiWriteLine(hFile, cbuf);

	oapiCloseFile (hFile, FILE_OUT);
}

void ProjectApolloConfigurator::DrawVisuals()
{
	ImGui::BeginGroupPanel("Mesh resolution");
    ImGui::RadioButton("High resolution", &gParams.Saturn_LowRes, 1); ImGui::SameLine();
    ImGui::RadioButton("Low resolution",  &gParams.Saturn_LowRes, 0);
	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("CSM main panel");
    ImGui::RadioButton("Single, continuous panel", &gParams.Saturn_MainPanelSplit, 0); ImGui::SameLine();
    ImGui::RadioButton("Multiple, split panels",  &gParams.Saturn_MainPanelSplit, 1);
	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("CSM G&N Lower Equipment Bay panel");
    ImGui::RadioButton("Single, continuous panel##GN", &gParams.Saturn_GNSplit, 0); ImGui::SameLine();
    ImGui::RadioButton("Multiple, split panels##GN",  &gParams.Saturn_GNSplit, 1);
	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("FDAI");
	ImGui::Checkbox("Disabled", (bool *)&gParams.Saturn_FDAIDisabled);
	ImGui::Checkbox("Smooth scrolling", (bool *)&gParams.Saturn_FDAISmooth);
	ImGui::EndGroupPanel();
}
void ProjectApolloConfigurator::DrawControls()
{
	
}
void ProjectApolloConfigurator::DrawMisc()
{
	ImGui::BeginGroupPanel("Time Acceleration");
	if(ImGui::InputInt("Max. time acceleration (0=unlimited)", &gParams.Saturn_MaxTimeAcceleration)) {
		gParams.Saturn_MaxTimeAcceleration = std::clamp(gParams.Saturn_MaxTimeAcceleration, 0, 100);
	}
	ImGui::Checkbox("Multi-Threading in time acceleration", (bool *)&gParams.Saturn_MultiThread);
	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("Checklists");
	ImGui::Checkbox("Slow automatic checklist execution (e.g. for demonstrations)", (bool *)&gParams.Saturn_VAGCChecklistAutoSlow);
	ImGui::Checkbox("Enable automatic checklist execution", (bool *)&gParams.Saturn_VAGCChecklistAutoEnabled);
	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("Other");
	ImGui::Checkbox("Enable VC flight instruments in debug string (e.g. during PDI)", (bool *)&gParams.Saturn_VcInfoEnabled);
	ImGui::EndGroupPanel();
}

void ProjectApolloConfigurator::Draw ()
{
        const char *tabs[] = {
            "Visuals", "Controls", "Miscellaneous",
        };

        void (ProjectApolloConfigurator::* func[])() = {
            &ProjectApolloConfigurator::DrawVisuals,
			&ProjectApolloConfigurator::DrawControls,
			&ProjectApolloConfigurator::DrawMisc
        };

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            for(size_t i = 0; i<sizeof(tabs)/sizeof(tabs[0]);i++) {
                if(ImGui::BeginTabItem(tabs[i])) {
                    (this->*func[i])();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
}

// ==============================================================
// The DLL entry point
// ==============================================================

DLLCLBK void opcDLLInit (MODULEHANDLE hDLL)
{
	// create the new config item
	gParams.Saturn_LowRes = 0;
	gParams.Saturn_MainPanelSplit = 0;
	gParams.Saturn_GNSplit = 1;
	gParams.Saturn_FDAIDisabled = 0;
	gParams.Saturn_FDAISmooth = 0;
	gParams.Saturn_RHC = -1;
	gParams.Saturn_THC = -1;
	gParams.Saturn_RSL = -1;
	gParams.Saturn_TSL = -1;
	gParams.Saturn_RHCTHCToggle = 0;
	gParams.Saturn_RHCTHCToggleId = -1;
	gParams.Saturn_VESIM = 0;
	gParams.Saturn_MaxTimeAcceleration = 0;
	gParams.Saturn_MultiThread = 0;
	gParams.Saturn_VAGCChecklistAutoSlow = 1;
	gParams.Saturn_VAGCChecklistAutoEnabled = 0;
	gParams.Saturn_VcInfoEnabled = 0;

	gParams.item = new ProjectApolloConfigurator;

	// register the config entry
	oapiRegisterLaunchpadItem (gParams.item, "Vessel configuration");
}

// ==============================================================
// The DLL exit point
// ==============================================================

DLLCLBK void opcDLLExit (MODULEHANDLE hDLL)
{
	// Unregister the launchpad items
	oapiUnregisterLaunchpadItem (gParams.item);
	delete gParams.item;
}
