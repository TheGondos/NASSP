#include "Orbitersdk.h"
#include "ApolloRTCCMFD.h"
#include "iu.h"
#include "nassputils.h"

using nassp::utils::skpTextFmt;

char Buffer[100];

// Repaint the MFD
bool ApolloRTCCMFD::Update(oapi::Sketchpad *skp)
{
	Title(skp, "Apollo RTCC MFD");
	skp->SetFont(font);

	// Draws the MFD title

	// Add MFD display routines here.
	// Use the device context (hDC) for Windows GDI paint functions.

	//sprintf(Buffer, "%d", G->screen);
	//skp->Text(7.5 * W / 8,(int)(0.5 * H / 14), Buffer, strlen(Buffer));


	if (G->vesseltype < 0)
	{
		skp->Text(3 * W / 8, 7 * H / 14, "Unsupported Vessel", 18);
		return true;
	}

	if (screen == 0)
	{
		if (G->vesseltype == 0)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "CSM", 3);
		}
		else if(G->vesseltype == 1)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "LM", 2);
		}
		else
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "MCC", 3);
		}

		skp->Text(1 * W / 8, 2 * H / 14, "Maneuver Targeting", 18);
		skp->Text(1 * W / 8, 4 * H / 14, "Pre-Advisory Data", 17);
		skp->Text(1 * W / 8, 6 * H / 14, "Utility", 7);
		skp->Text(1 * W / 8, 8 * H / 14, "MCC Displays", 12);
		skp->Text(1 * W / 8, 10 * H / 14, "Mission Plan Table", 18);
		skp->Text(1 * W / 8, 12 * H / 14, "Configuration", 13);

		skp->Text(5 * W / 8, 2 * H / 14, "Uplinks", 7);
	}
	else if (screen == 1)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Two Impulse", 11);

		if (GC->rtcc->med_k30.IVFlag == 0)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Both Fixed", 10);
		}
		else if (GC->rtcc->med_k30.IVFlag == 1)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "First Fixed", 11);
		}
		else
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Second Fixed", 12);
		}

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_k30.Vehicle == 1)
			{
				skp->Text(1 * W / 8, 4 * H / 14, "Chaser: CSM", 11);
				skp->Text(1 * W / 8, 5 * H / 14, "Target: LEM", 11);
			}
			else
			{
				skp->Text(1 * W / 8, 4 * H / 14, "Chaser: LEM", 11);
				skp->Text(1 * W / 8, 5 * H / 14, "Target: CSM", 11);
			}

			skp->Text(1 * W / 8, 6 * H / 14, "CHA:", 4);
			if (GC->rtcc->med_k30.ChaserVectorTime > 0)
			{
				GET_Display(Buffer, GC->rtcc->med_k30.ChaserVectorTime);
			}
			else
			{
				strcpy(Buffer, "Present time");
			}
			skp->Text(2 * W / 8, 6 * H / 14, Buffer, strlen(Buffer));

			skp->Text(1 * W / 8, 8 * H / 14, "TGT:", 4);
			if (GC->rtcc->med_k30.TargetVectorTime > 0)
			{
				GET_Display(Buffer, GC->rtcc->med_k30.TargetVectorTime);
			}
			else
			{
				strcpy(Buffer, "Present time");
			}
			skp->Text(2 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			const char* target;
			if (G->target != NULL)
			{
				target = G->target->GetName();
			}
			else
			{
				target = "No Target!";
			}
			skp->Text(1 * W / 8, 4 * H / 14, target, strlen(target));
		}

		if (GC->rtcc->med_k30.StartTime >= 0)
		{
			GET_Display(Buffer, GC->rtcc->med_k30.StartTime);
		}
		else
		{
			sprintf(Buffer, "E = %.2f°", GC->rtcc->GZGENCSN.TIElevationAngle*DEG);
		}
		skp->Text(1 * W / 8, 10 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_k30.EndTime >= 0)
		{
			GET_Display(Buffer, GC->rtcc->med_k30.EndTime);
		}
		else
		{
			sprintf(Buffer, "WT = %.2f°", GC->rtcc->GZGENCSN.TITravelAngle*DEG);
		}
		skp->Text(1 * W / 8, 12 * H / 14, Buffer, strlen(Buffer));

		skp->Text(9 * W / 16, 8 * H / 21, "PHASE", 5);
		skp->Text(9 * W / 16, 9 * H / 21, "DEL H", 5);
		skp->Text(9 * W / 16, 10 * H / 21, "ELEV", 4);
		skp->Text(9 * W / 16, 11 * H / 21, "WT", 2);
		skpTextFmt(skp, 6 * W / 8, 8 * H / 21, "%.3f°", GC->rtcc->GZGENCSN.TIPhaseAngle* DEG);
		skpTextFmt(skp, 6 * W / 8, 9 * H / 21, "%.3f NM", GC->rtcc->GZGENCSN.TIDeltaH / 1852.0);
		skpTextFmt(skp, 6 * W / 8, 10 * H / 21, "%.3f°", GC->rtcc->GZGENCSN.TIElevationAngle* DEG);
		skpTextFmt(skp, 6 * W / 8, 11 * H / 21, "%.3f°", GC->rtcc->GZGENCSN.TITravelAngle* DEG);

		if (GC->rtcc->med_k30.IVFlag != 0)
		{
			skpTextFmt(skp, 6 * W / 8, 2 * H / 14, "%.0lf s", GC->rtcc->med_k30.TimeStep);
			skpTextFmt(skp, 6 * W / 8, 4 * H / 14, "%.0lf s", GC->rtcc->med_k30.TimeRange);
		}
	}
	else if (screen == 2)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "TWO IMPULSE MULTIPLE SOLUTION (MSK 0063)", 40);

		Text(skp, 32 * W / 64, 30 * H / 32, GC->rtcc->TwoImpMultDispBuffer.ErrorMessage);

		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::RIGHT);
		skp->Text(6 * W / 32, 4 * H / 32, "LM STA ID", 9);
		skp->Text(6 * W / 32, 5 * H / 32, "LM GETTHS", 9);
		skp->Text(6 * W / 32, 6 * H / 32, "MAN VEH", 7);
		skp->Text(6 * W / 32, 7 * H / 32, "WT", 2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skpTextFmt(skp, 1 * W / 32, 8 * H / 32, "GET%s", GC->rtcc->TwoImpMultDispBuffer.GETFRZ.c_str());
		skpTextFmt(skp, 1 * W / 32, 9 * H / 32, "GMT%s", GC->rtcc->TwoImpMultDispBuffer.GMTFRZ.c_str());
		skp->SetTextAlign(oapi::Sketchpad::RIGHT);
		Text(skp, 10 * W / 32, 6 * H / 32, GC->rtcc->TwoImpMultDispBuffer.MAN_VEH);
		skpTextFmt(skp, 10 * W / 32, 7 * H / 32, "%.3lf", GC->rtcc->TwoImpMultDispBuffer.WT);
		GET_Display3(Buffer, GC->rtcc->TwoImpMultDispBuffer.GET1);
		skp->Text(10 * W / 32, 8 * H / 32, Buffer, strlen(Buffer));
		GET_Display3(Buffer, GC->rtcc->TwoImpMultDispBuffer.GMT1);
		skp->Text(10 * W / 32, 9 * H / 32, Buffer, strlen(Buffer));

		skp->Text(24 * W / 32, 4 * H / 32, "CSM STA ID", 10);
		skp->Text(24 * W / 32, 5 * H / 32, "CSM GETTHS", 10);
		skp->Text(24 * W / 32, 6 * H / 32, "PHASE", 10);
		skp->Text(24 * W / 32, 7 * H / 32, "DEL H", 5);
		skp->Text(24 * W / 32, 8 * H / 32, "OPTION", 6);

		skpTextFmt(skp, 31 * W / 32, 6 * H / 32, "%.4lf", GC->rtcc->TwoImpMultDispBuffer.PHASE);
		skpTextFmt(skp, 31 * W / 32, 7 * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.DH);
		Text(skp, 31 * W / 32, 8 * H / 32, GC->rtcc->TwoImpMultDispBuffer.OPTION);

		skp->Text(5 * W / 32, 11 * H / 32, "DEL V1", 6);
		skp->Text(8 * W / 32, 11 * H / 32, "YAW", 3);
		skp->Text(23 * W / 64, 11 * H / 32, "PITCH", 5);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skpTextFmt(skp, 27 * W / 64, 11 * H / 32, "GET%s", GC->rtcc->TwoImpMultDispBuffer.GETVAR.c_str());
		skp->SetTextAlign(oapi::Sketchpad::RIGHT);
		skp->Text(43 * W / 64, 11 * H / 32, "DEL V2", 6);
		if (GC->rtcc->TwoImpMultDispBuffer.showTPI)
		{
			skp->Text(52 * W / 64, 11 * H / 32, "TTPI", 4);
		}
		else
		{
			skp->Text(49 * W / 64, 11 * H / 32, "YAW", 3);
			skp->Text(28 * W / 32, 11 * H / 32, "PITCH", 5);
		}
		
		skp->Text(60 * W / 64, 11 * H / 32, "L", 1);
		skp->Text(63 * W / 64, 11 * H / 32, "C", 1);
		
		for (int i = 0;i < GC->rtcc->TwoImpMultDispBuffer.Solutions;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (12 + i) * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.data[i].DELV1);
			skpTextFmt(skp, 8 * W / 32, (12 + i) * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.data[i].YAW1);
			skpTextFmt(skp, 23 * W / 64, (12 + i) * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.data[i].PITCH1);
			GET_Display3(Buffer, GC->rtcc->TwoImpMultDispBuffer.data[i].Time2);
			skp->Text(35 * W / 64, (12 + i) * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 43 * W / 64, (12 + i) * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.data[i].DELV2);
			if (GC->rtcc->TwoImpMultDispBuffer.showTPI)
			{
				GET_Display(Buffer, GC->rtcc->TwoImpMultDispBuffer.data[i].T_TPI, false);
				skp->Text(55 * W / 64, (12 + i) * H / 32, Buffer, strlen(Buffer));
			}
			else
			{
				skpTextFmt(skp, 50 * W / 64, (12 + i) * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.data[i].YAW2);
				skpTextFmt(skp, 57 * W / 64, (12 + i) * H / 32, "%.2lf", GC->rtcc->TwoImpMultDispBuffer.data[i].PITCH2);
			}
			
			skpTextFmt(skp, 60 * W / 64, (12 + i) * H / 32, "%c", GC->rtcc->TwoImpMultDispBuffer.data[i].L);
			skpTextFmt(skp, 63 * W / 64, (12 + i) * H / 32, "%d", GC->rtcc->TwoImpMultDispBuffer.data[i].C);
		}
	}
	else if (screen == 3)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Coelliptic", 10);

		skp->Text(1 * W / 16, 2 * H / 14, "SPQ Initialization", 18);

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_k01.ChaserVehicle == 1)
			{
				skp->Text(1 * W / 16, 4 * H / 14, "Chaser: CSM", 11);
				skp->Text(1 * W / 16, 5 * H / 14, "Target: LEM", 11);
			}
			else
			{
				skp->Text(1 * W / 16, 4 * H / 14, "Chaser: LEM", 11);
				skp->Text(1 * W / 16, 5 * H / 14, "Target: CSM", 11);
			}

			if (GC->rtcc->med_k01.ChaserThresholdGET < 0)
			{
				strcpy(Buffer, "Present Time");
			}
			else
			{
				GET_Display(Buffer, GC->rtcc->med_k01.ChaserThresholdGET);
			}
			skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));
			if (GC->rtcc->med_k01.TargetThresholdGET < 0)
			{
				strcpy(Buffer, "Present Time");
			}
			else
			{
				GET_Display(Buffer, GC->rtcc->med_k01.TargetThresholdGET);
			}
			skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "Chaser: %s", G->vessel->GetName());
			if (G->target)
			{
				sprintf_s(Buffer, "Target: %s", G->target->GetName());
			}
			else
			{
				strcpy(Buffer, "Target: Not set!");
			}
			skp->Text(1 * W / 16, 5 * H / 14, Buffer, strlen(Buffer));
		}

		if (G->SPQMode != 1)
		{
			if (G->SPQMode == 2)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "Optimum CSI", 11);
			}
			else
			{
				skp->Text(1 * W / 16, 10 * H / 14, "CSI", 3);

				if (G->CDHtimemode == 0)
				{
					skp->Text(1 * W / 16, 12 * H / 14, "Fixed TPI time", 14);
				}
				else if (G->CDHtimemode == 1)
				{
					skp->Text(1 * W / 16, 12 * H / 14, "Fixed DH", 8);
				}
			}

			GET_Display(Buffer, G->CSItime);
			skp->Text(10 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(1 * W / 16, 10 * H / 14, "CDH", 3);

			if (G->CDHtimemode == 0)
			{
				skp->Text(1 * W / 16, 12 * H / 14, "Fixed", 5);
			}
			else if (G->CDHtimemode == 1)
			{
				skp->Text(1 * W / 16, 12 * H / 14, "Find GETI", 9);
			}

			GET_Display(Buffer, G->CDHtime);
			skp->Text(10 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));
		}

		GET_Display(Buffer, G->SPQTIG);
		skp->Text(5 * W / 8, 15 * H / 21, Buffer, strlen(Buffer));

		skp->Text(5 * W / 8, 16 * H / 21, "DX", 2);
		skp->Text(5 * W / 8, 17 * H / 21, "DY", 2);
		skp->Text(5 * W / 8, 18 * H / 21, "DZ", 2);

		AGC_Display(Buffer, G->SPQDeltaV.x / 0.3048);
		skp->Text(6 * W / 8, 16 * H / 21, Buffer, strlen(Buffer));
		AGC_Display(Buffer, G->SPQDeltaV.y / 0.3048);
		skp->Text(6 * W / 8, 17 * H / 21, Buffer, strlen(Buffer));
		AGC_Display(Buffer, G->SPQDeltaV.z / 0.3048);
		skp->Text(6 * W / 8, 18 * H / 21, Buffer, strlen(Buffer));
	}
	else if (screen == 4)
	{
		skp->Text(4 * W / 8, (int)(0.5 * H / 14), "General Purpose Maneuver", 24);

		skp->Text(1 * W / 22, (marker + 3) * H / 22, "*", 1);

		skp->Text(2 * W / 22, 2 * H / 22, "Code:", 5);
		GMPManeuverCodeName(Buffer, G->GMPManeuverCode);
		skp->Text(5 * W / 22, 2 * H / 22, Buffer, strlen(Buffer));

		if (GC->MissionPlanningActive)
		{
			skp->Text(2 * W / 22, 3 * H / 22, "VEH", 3);
			if (GC->rtcc->med_k20.Vehicle == RTCC_MPT_CSM)
			{
				skp->Text(5 * W / 22, 3 * H / 22, "CSM", 3);
			}
			else
			{
				skp->Text(5 * W / 22, 3 * H / 22, "LM", 2);
			}
		}

		skp->Text(2 * W / 22, 4 * H / 22, "TYP", 3);
		GMPManeuverTypeName(Buffer, G->GMPManeuverType);
		skp->Text(4 * W / 22, 4 * H / 22, Buffer, strlen(Buffer));

		skp->Text(2 * W / 22, 5 * H / 22, "PNT", 3);
		GMPManeuverPointName(Buffer, G->GMPManeuverPoint);
		skp->Text(4 * W / 22, 5 * H / 22, Buffer, strlen(Buffer));

		skp->Text(2 * W / 22, 6 * H / 22, "GET", 3);
		GET_Display(Buffer, G->SPSGET, false);
		skp->Text(4 * W / 22, 6 * H / 22, Buffer, strlen(Buffer));

		skp->Text(2 * W / 22, 7 * H / 22, "REF", 3);

		if (G->OrbAdjAltRef == 0)
		{
			skp->Text(4 * W / 22, 7 * H / 22, "Mean rad", 8);
		}
		else
		{
			skp->Text(4 * W / 22, 7 * H / 22, "Pad/LS", 6);
		}

		//Desired Maneuver Height
		if (G->GMPManeuverCode == RTCC_GMP_CRH || G->GMPManeuverCode == RTCC_GMP_HBH || G->GMPManeuverCode == RTCC_GMP_FCH || G->GMPManeuverCode == RTCC_GMP_CPH ||
			G->GMPManeuverCode == RTCC_GMP_CNH || G->GMPManeuverCode == RTCC_GMP_PCH || G->GMPManeuverCode == RTCC_GMP_NSH || G->GMPManeuverCode == RTCC_GMP_HOH)
		{
			skp->Text(2 * W / 22, 8 * H / 22, "ALT", 3);
			skpTextFmt(skp, 4 * W / 22, 8 * H / 22, "%.2f NM", G->GMPManeuverHeight / 1852.0);
		}
		//Desired Maneuver Longitude
		else if (G->GMPManeuverCode == RTCC_GMP_PCL || G->GMPManeuverCode == RTCC_GMP_CRL || G->GMPManeuverCode == RTCC_GMP_HOL || G->GMPManeuverCode == RTCC_GMP_NSL ||
			G->GMPManeuverCode == RTCC_GMP_FCL || G->GMPManeuverCode == RTCC_GMP_NHL || G->GMPManeuverCode == RTCC_GMP_SAL || G->GMPManeuverCode == RTCC_GMP_PHL ||
			G->GMPManeuverCode == RTCC_GMP_CPL || G->GMPManeuverCode == RTCC_GMP_HBL || G->GMPManeuverCode == RTCC_GMP_CNL || G->GMPManeuverCode == RTCC_GMP_HNL ||
			G->GMPManeuverCode == RTCC_GMP_SAA || G->GMPManeuverCode == RTCC_GMP_HAS)
		{
			skp->Text(2 * W / 22, 8 * H / 22, "LNG", 3);
			skpTextFmt(skp, 4 * W / 22, 8 * H / 22, "%.2f°", G->GMPManeuverLongitude* DEG);
		}

		//Height Change
		if (G->GMPManeuverCode == RTCC_GMP_HOL || G->GMPManeuverCode == RTCC_GMP_HOT || G->GMPManeuverCode == RTCC_GMP_HAO || G->GMPManeuverCode == RTCC_GMP_HPO ||
			G->GMPManeuverCode == RTCC_GMP_HNL || G->GMPManeuverCode == RTCC_GMP_HNT || G->GMPManeuverCode == RTCC_GMP_HNA || G->GMPManeuverCode == RTCC_GMP_HNP ||
			G->GMPManeuverCode == RTCC_GMP_PHL || G->GMPManeuverCode == RTCC_GMP_PHT || G->GMPManeuverCode == RTCC_GMP_PHA || G->GMPManeuverCode == RTCC_GMP_PHP || G->GMPManeuverCode == RTCC_GMP_HOH)
		{
			skp->Text(2 * W / 22, 9 * H / 22, "DH", 2);
			skpTextFmt(skp, 4 * W / 22, 9 * H / 22, "%.2f NM", G->GMPHeightChange / 1852.0);
		}
		//Apoapsis Height
		else if (G->GMPManeuverCode == RTCC_GMP_HBT || G->GMPManeuverCode == RTCC_GMP_HBH || G->GMPManeuverCode == RTCC_GMP_HBO || G->GMPManeuverCode == RTCC_GMP_HBL ||
			G->GMPManeuverCode == RTCC_GMP_NHT || G->GMPManeuverCode == RTCC_GMP_NHL || G->GMPManeuverCode == RTCC_GMP_HAS)
		{
			skp->Text(2 * W / 22, 9 * H / 22, "ApA", 3);
			skpTextFmt(skp, 4 * W / 22, 9 * H / 22, "%.2f NM", G->GMPApogeeHeight / 1852.0);
		}
		//Delta V
		else if (G->GMPManeuverCode == RTCC_GMP_FCT || G->GMPManeuverCode == RTCC_GMP_FCA || G->GMPManeuverCode == RTCC_GMP_FCP || G->GMPManeuverCode == RTCC_GMP_FCE ||
			G->GMPManeuverCode == RTCC_GMP_FCL || G->GMPManeuverCode == RTCC_GMP_FCH)
		{
			skp->Text(2 * W / 22, 9 * H / 22, "DV", 2);
			skpTextFmt(skp, 4 * W / 22, 9 * H / 22, "%.2f ft/s", G->GMPDeltaVInput / 0.3048);
		}
		//Apse line rotation
		else if (G->GMPManeuverCode == RTCC_GMP_SAT || G->GMPManeuverCode == RTCC_GMP_SAO || G->GMPManeuverCode == RTCC_GMP_SAL)
		{
			skp->Text(2 * W / 22, 9 * H / 22, "ROT", 4);
			skpTextFmt(skp, 4 * W / 22, 9 * H / 22, "%.2f°", G->GMPApseLineRotAngle* DEG);
		}

		//Wedge Angle
		if (G->GMPManeuverCode == RTCC_GMP_PCE || G->GMPManeuverCode == RTCC_GMP_PCL || G->GMPManeuverCode == RTCC_GMP_PCT || G->GMPManeuverCode == RTCC_GMP_PHL ||
			G->GMPManeuverCode == RTCC_GMP_PHT || G->GMPManeuverCode == RTCC_GMP_PHA || G->GMPManeuverCode == RTCC_GMP_PHP || G->GMPManeuverCode == RTCC_GMP_CPL ||
			G->GMPManeuverCode == RTCC_GMP_CPH || G->GMPManeuverCode == RTCC_GMP_CPT || G->GMPManeuverCode == RTCC_GMP_CPA || G->GMPManeuverCode == RTCC_GMP_CPP ||
			G->GMPManeuverCode == RTCC_GMP_PCH)
		{
			skp->Text(2 * W / 22, 10 * H / 22, "DW", 2);
			skpTextFmt(skp, 4 * W / 22, 10 * H / 22, "%.2f°", G->GMPWedgeAngle * DEG);
		}
		//Node Shift
		else if (G->GMPManeuverCode == RTCC_GMP_NST || G->GMPManeuverCode == RTCC_GMP_NSO || G->GMPManeuverCode == RTCC_GMP_NSH || G->GMPManeuverCode == RTCC_GMP_NSL ||
			G->GMPManeuverCode == RTCC_GMP_CNL || G->GMPManeuverCode == RTCC_GMP_CNH || G->GMPManeuverCode == RTCC_GMP_CNT || G->GMPManeuverCode == RTCC_GMP_CNA || G->GMPManeuverCode == RTCC_GMP_CNP || 
			G->GMPManeuverCode == RTCC_GMP_HNL || G->GMPManeuverCode == RTCC_GMP_HNT || G->GMPManeuverCode == RTCC_GMP_HNA || G->GMPManeuverCode == RTCC_GMP_HNP)
		{
			skp->Text(2 * W / 22, 10 * H / 22, "DLN", 3);
			skpTextFmt(skp, 4 * W / 22, 10 * H / 22, "%.2f°", G->GMPNodeShiftAngle * DEG);
		}
		//Periapsis Height
		else if (G->GMPManeuverCode == RTCC_GMP_HBT || G->GMPManeuverCode == RTCC_GMP_HBH || G->GMPManeuverCode == RTCC_GMP_HBO || G->GMPManeuverCode == RTCC_GMP_HBL ||
			G->GMPManeuverCode == RTCC_GMP_NHT || G->GMPManeuverCode == RTCC_GMP_NHL || G->GMPManeuverCode == RTCC_GMP_HAS)
		{
			skp->Text(2 * W / 22, 10 * H / 22, "PeA", 3);
			skpTextFmt(skp, 4 * W / 22, 10 * H / 22, "%.2f NM", G->GMPPerigeeHeight / 1852.0);
		}
		//Pitch
		else if (G->GMPManeuverCode == RTCC_GMP_FCT || G->GMPManeuverCode == RTCC_GMP_FCA || G->GMPManeuverCode == RTCC_GMP_FCP || G->GMPManeuverCode == RTCC_GMP_FCE ||
			G->GMPManeuverCode == RTCC_GMP_FCL || G->GMPManeuverCode == RTCC_GMP_FCH)
		{
			skp->Text(2 * W / 22, 10 * H / 22, "P", 1);
			skpTextFmt(skp, 4 * W / 22, 10 * H / 22, "%.2f°", G->GMPPitch * DEG);
		}

		//Yaw
		if (G->GMPManeuverCode == RTCC_GMP_FCT || G->GMPManeuverCode == RTCC_GMP_FCA || G->GMPManeuverCode == RTCC_GMP_FCP || G->GMPManeuverCode == RTCC_GMP_FCE ||
			G->GMPManeuverCode == RTCC_GMP_FCL || G->GMPManeuverCode == RTCC_GMP_FCH)
		{
			skp->Text(2 * W / 22, 11 * H / 22, "Y", 1);
			skpTextFmt(skp, 4 * W / 22, 11 * H / 22, "%.2f°", G->GMPYaw * DEG);
		}
		//Node Shift
		else if (G->GMPManeuverCode == RTCC_GMP_NHT || G->GMPManeuverCode == RTCC_GMP_NHL)
		{
			skp->Text(2 * W / 22, 11 * H / 22, "DLN", 3);
			skpTextFmt(skp, 4 * W / 22, 11 * H / 22, "%.2f°", G->GMPNodeShiftAngle * DEG);
		}
		//Rev counter
		else if (G->GMPManeuverCode == RTCC_GMP_HAS)
		{
			skp->Text(2 * W / 22, 11 * H / 22, "N", 1);
			skpTextFmt(skp, 4 * W / 22, 11 * H / 22, "%d", G->GMPRevs);
		}

		if (GC->rtcc->PZGPMDIS.Err)
		{
			skpTextFmt(skp, 3 * W / 8, 21 * H / 22, "Error: %d", GC->rtcc->PZGPMDIS.Err);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(4 * W / 22, 13 * H / 22, "GET A", 5);
		skp->Text(4 * W / 22, 14 * H / 22, "HA", 2);
		skp->Text(4 * W / 22, 15 * H / 22, "LONG A", 6);
		skp->Text(4 * W / 22, 16 * H / 22, "LAT A", 5);
		skp->Text(4 * W / 22, 17 * H / 22, "GET P", 5);
		skp->Text(4 * W / 22, 18 * H / 22, "HP", 2);
		skp->Text(4 * W / 22, 19 * H / 22, "LONG P", 6);
		skp->Text(4 * W / 22, 20 * H / 22, "LAT P", 5);

		GET_Display(Buffer, GC->rtcc->PZGPMDIS.GET_A, false);
		skp->Text(10 * W / 22, 13 * H / 22, Buffer, strlen(Buffer));
		skpTextFmt(skp, 10 * W / 22, 14 * H / 22, "%.1f", GC->rtcc->PZGPMDIS.HA / 1852.0);
		FormatLongitude(Buffer, GC->rtcc->PZGPMDIS.long_A*DEG);
		skp->Text(10 * W / 22, 15 * H / 22, Buffer, strlen(Buffer));
		FormatLatitude(Buffer, GC->rtcc->PZGPMDIS.lat_A*DEG);
		skp->Text(10 * W / 22, 16 * H / 22, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->PZGPMDIS.GET_P, false);
		skp->Text(10 * W / 22, 17 * H / 22, Buffer, strlen(Buffer));
		skpTextFmt(skp, 10 * W / 22, 18 * H / 22, "%.1f", GC->rtcc->PZGPMDIS.HP / 1852.0);
		FormatLongitude(Buffer, GC->rtcc->PZGPMDIS.long_P*DEG);
		skp->Text(10 * W / 22, 19 * H / 22, Buffer, strlen(Buffer));
		FormatLatitude(Buffer, GC->rtcc->PZGPMDIS.lat_P*DEG);
		skp->Text(10 * W / 22, 20 * H / 22, Buffer, strlen(Buffer));

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(12 * W / 22, 5 * H / 22, "GETI", 4);
		skp->Text(12 * W / 22, 6 * H / 22, "DEL V MAN", 9);
		skp->Text(12 * W / 22, 7 * H / 22, "PIT MAN", 7);
		skp->Text(12 * W / 22, 8 * H / 22, "YAW MAN", 7);
		skp->Text(12 * W / 22, 9 * H / 22, "H MAN", 5);
		skp->Text(12 * W / 22, 10 * H / 22, "LONG MAN", 8);
		skp->Text(12 * W / 22, 11 * H / 22, "LAT MAN", 7);

		skp->Text(12 * W / 22, 13 * H / 22, "A", 1);
		skp->Text(12 * W / 22, 14 * H / 22, "E", 1);
		skp->Text(12 * W / 22, 15 * H / 22, "I", 1);
		skp->Text(12 * W / 22, 16 * H / 22, "NODE AN", 7);
		skp->Text(12 * W / 22, 17 * H / 22, "DEL G", 5);

		skp->Text(12 * W / 22, 19 * H / 22, "VX", 3);
		skp->Text(12 * W / 22, 20 * H / 22, "VY", 3);
		skp->Text(12 * W / 22, 21 * H / 22, "VZ", 3);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		GET_Display(Buffer, GC->rtcc->PZGPMDIS.GET_TIG, false);
		skp->Text(21 * W / 22, 5 * H / 22, Buffer, strlen(Buffer));
		AGC_Display(Buffer, length(GC->rtcc->PZGPMDIS.DV) / 0.3048);
		skp->Text(21 * W / 22, 6 * H / 22, Buffer, strlen(Buffer));
		skpTextFmt(skp, 21 * W / 22, 7 * H / 22, "%+.2f", GC->rtcc->PZGPMDIS.Pitch_Man * DEG);
		skpTextFmt(skp, 21 * W / 22, 8 * H / 22, "%+.2f", GC->rtcc->PZGPMDIS.Yaw_Man * DEG);
		skpTextFmt(skp, 21 * W / 22, 9 * H / 22, "%.1f", GC->rtcc->PZGPMDIS.H_Man / 1852.0);
		FormatLongitude(Buffer, GC->rtcc->PZGPMDIS.long_Man*DEG);
		skp->Text(21 * W / 22, 10 * H / 22, Buffer, strlen(Buffer));
		FormatLatitude(Buffer, GC->rtcc->PZGPMDIS.lat_Man*DEG);
		skp->Text(21 * W / 22, 11 * H / 22, Buffer, strlen(Buffer));

		skpTextFmt(skp, 21 * W / 22, 13 * H / 22, "%.1f", GC->rtcc->PZGPMDIS.A / 1852.0);
		skpTextFmt(skp, 21 * W / 22, 14 * H / 22, "%.6f", GC->rtcc->PZGPMDIS.E);
		skpTextFmt(skp, 21 * W / 22, 15 * H / 22, "%.3f°", GC->rtcc->PZGPMDIS.I* DEG);
		FormatLongitude(Buffer, GC->rtcc->PZGPMDIS.Node_Ang*DEG);
		skp->Text(21 * W / 22, 16 * H / 22, Buffer, strlen(Buffer));
		FormatLongitude(Buffer, GC->rtcc->PZGPMDIS.Del_G*DEG);
		skp->Text(21 * W / 22, 17 * H / 22, Buffer, strlen(Buffer));

		AGC_Display(Buffer, GC->rtcc->PZGPMDIS.DV.x / 0.3048);
		skp->Text(21 * W / 22, 19 * H / 22, Buffer, strlen(Buffer));
		AGC_Display(Buffer, GC->rtcc->PZGPMDIS.DV.y / 0.3048);
		skp->Text(21 * W / 22, 20 * H / 22, Buffer, strlen(Buffer));
		AGC_Display(Buffer, GC->rtcc->PZGPMDIS.DV.z / 0.3048);
		skp->Text(21 * W / 22, 21 * H / 22, Buffer, strlen(Buffer));
	}
	else if (screen == 5)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "REFSMMAT", 8);

		if (G->REFSMMATopt == 0) //P30 Maneuver
		{
			if (G->REFSMMATHeadsUp)
			{
				skp->Text(5 * W / 8, 2 * H / 14, "P30 (Heads up)", 14);
			}
			else
			{
				skp->Text(5 * W / 8, 2 * H / 14, "P30 (Heads down)", 16);
			}

			GET_Display(Buffer, G->P30TIG);
			skp->Text((int)(0.5 * W / 8), 2 * H / 14, Buffer, strlen(Buffer));

			skp->Text(6 * W / 8, 4 * H / 14, "DV Vector", 9);
			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(6 * W / 8, 5 * H / 14, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(6 * W / 8, 6 * H / 14, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(6 * W / 8, 7 * H / 14, Buffer, strlen(Buffer));
		}
		else if (G->REFSMMATopt == 1)//Retrofire
		{
			skp->Text(5 * W / 8, 2 * H / 14, "P30 Retro", 9);

			GET_Display(Buffer, G->P30TIG);
			skp->Text((int)(0.5 * W / 8), 2 * H / 14, Buffer, strlen(Buffer));

			skp->Text(6 * W / 8, 4 * H / 14, "DV Vector", 9);
			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(6 * W / 8, 5 * H / 14, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(6 * W / 8, 6 * H / 14, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(6 * W / 8, 7 * H / 14, Buffer, strlen(Buffer));

		}
		else if (G->REFSMMATopt == 2)
		{
			skp->Text(5 * W / 8, 2 * H / 14, "LVLH", 4);

			GET_Display(Buffer, G->REFSMMAT_LVLH_Time);
			skp->Text((int)(0.5 * W / 8), 2 * H / 14, Buffer, strlen(Buffer));
		}
		else if (G->REFSMMATopt == 3)
		{
			skp->Text(5 * W / 8, 2 * H / 14, "Lunar Entry", 11);
		}
		else if (G->REFSMMATopt == 4)
		{
			skp->Text(5 * W / 8, 2 * H / 14, "Launch", 6);

			if (GC->mission == 0)
			{
				skp->Text((int)(0.5 * W / 8), 2 * H / 14, "Manual", 6);
			}
			else if (GC->mission >= 7)
			{
				skpTextFmt(skp, (int)(0.5 * W / 8), 2 * H / 14, "Apollo %i", GC->mission);
			}
		}
		else if (G->REFSMMATopt == 5 || G->REFSMMATopt == 8)
		{
			GET_Display(Buffer, GC->rtcc->CZTDTGTU.GETTD);
			skp->Text((int)(0.5 * W / 8), 2 * H / 14, Buffer, strlen(Buffer));

			skpTextFmt(skp, (int)(5.5 * W / 8), 8 * H / 14, "%f°", GC->rtcc->BZLAND.lat[RTCC_LMPOS_BEST] * DEG);
			skpTextFmt(skp, (int)(5.5 * W / 8), 10 * H / 14, "%f°", GC->rtcc->BZLAND.lng[RTCC_LMPOS_BEST] * DEG);

			if (G->REFSMMATopt == 8)
			{
				skp->Text(5 * W / 8, 2 * H / 14, "LS during TLC", 13);

				skp->Text((int)(5.5 * W / 8), 11 * H / 14, "Azimuth:", 8);
				skpTextFmt(skp, (int)(5.5 * W / 8), 12 * H / 14, "%f°", GC->rtcc->med_k18.psi_DS);
			}
			else
			{
				skp->Text(5 * W / 8, 2 * H / 14, "Landing Site", 12);

				if (G->vesseltype != 0 && GC->MissionPlanningActive == false)
				{
					if (G->target != NULL)
					{
						sprintf_s(Buffer, "CSM: %s", G->target->GetName());
					}
					else
					{
						strcpy(Buffer, "No Target!");
					}
					skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
				}
			}

		}
		else if (G->REFSMMATopt == 6)
		{
			skp->Text(5 * W / 8, 2 * H / 14, "PTC", 3);

			skp->Text(1 * W / 16, 2 * H / 14, "Average time of TEI:", 20);
			skpTextFmt(skp, 1 * W / 16, 3 * H / 14, "MJD %lf", G->REFSMMAT_PTC_MJD);
		}
		else if (G->REFSMMATopt == 7)
		{
			skp->Text(5 * W / 8, 2 * H / 14, "REFS from Attitude", 18);

			skp->Text((int)(0.5 * W / 8), 9 * H / 21, "Current REFSMMAT:", 17);
			REFSMMATName(Buffer, G->REFSMMATcur);
			skp->Text((int)(0.5 * W / 8), 10 * H / 21, Buffer, strlen(Buffer));

			skp->Text((int)(0.5 * W / 8), 12 * H / 21, "Attitude:", 9);
			skpTextFmt(skp, (int)(0.5 * W / 8), 13 * H / 21, "%+07.2f R", G->VECangles.x* DEG);
			skpTextFmt(skp, (int)(0.5 * W / 8), 14 * H / 21, "%+07.2f P", G->VECangles.y* DEG);
			skpTextFmt(skp, (int)(0.5 * W / 8), 15 * H / 21, "%+07.2f Y", G->VECangles.z* DEG);
		}

		REFSMMATData *refsdata;
		if (G->vesseltype == 0)
		{
			refsdata = &GC->rtcc->EZJGMTX1.data[0];
		}
		else
		{
			refsdata = &GC->rtcc->EZJGMTX3.data[0];
		}

		std::string REFSMMATCode;
		GC->rtcc->FormatREFSMMATCode(RTCC_REFSMMAT_TYPE_CUR, refsdata->ID, REFSMMATCode);
		Text(skp, 7 * W / 16, 3 * H / 14, REFSMMATCode);

		MATRIX3 ref;
		ref = mul(refsdata->REFSMMAT, OrbMech::tmat(GC->rtcc->SystemParameters.MAT_J2000_BRCS));

		for (int i = 0; i < 9; i++)
		{
			skpTextFmt(skp, 7 * W / 16, (4 + i) * H / 14, "%f", ref.data[i]);
		}
	}
	else if (screen == 6)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Entry Options", 13);

		skp->Text(1 * W / 8, 2 * H / 14, "Tradeoff", 15);
		skp->Text(1 * W / 8, 4 * H / 14, "Abort Scan Table", 16);
		skp->Text(1 * W / 8, 6 * H / 14, "Return to Earth Digitals", 24);
		skp->Text(1 * W / 8, 8 * H / 14, "Splashdown Update", 17);
		skp->Text(1 * W / 8, 10 * H / 14, "RTE Constraints", 15);

		skp->Text(5 * W / 8, 2 * H / 14, "RTED Manual Input", 17);
		skp->Text(5 * W / 8, 4 * H / 14, "RTED Entry Profile", 18);
	}
	else if (screen == 7)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "AGS NAVIGATION UPDATES (277)", 28);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (G->SVSlot)
		{
			skp->Text((int)(0.5 * W / 8), 8 * H / 14, "CSM", 3);
		}
		else
		{
			skp->Text((int)(0.5 * W / 8), 8 * H / 14, "LM", 2);
		}

		if (G->svtarget != NULL)
		{
			sprintf(Buffer, G->svtarget->GetName());
			skp->Text((int)(0.5 * W / 8), 6 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text((int)(0.5 * W / 8), 6 * H / 14, "No Target!", 10);
		}

		skp->Text((int)(0.5 * W / 8), 3 * H / 14, "REFSMMAT:", 9);
		REFSMMATName(Buffer, G->REFSMMATcur);
		skp->Text((int)(0.5 * W / 8), 4 * H / 14, Buffer, strlen(Buffer));

		int hh, mm;
		double secs;

		SStoHHMMSS(GC->rtcc->GETfromGMT(GC->rtcc->GetAGSClockZero()), hh, mm, secs); //Should be relative to LGC clock zero instead of liftoff time
		skpTextFmt(skp, (int)(0.5 * W / 8), 10 * H / 14, "%d:%02d:%05.2f GET", hh, mm, secs);

		if (IsBusy(G->subThreadStatus))
		{
			skp->Text(1 * W / 16, 12 * H / 14, "Calculating...", 14);
		}
		else
		{
			skp->Text(1 * W / 16, 12 * H / 14, "Calculate K-Factor", 18);
		}

		skpTextFmt(skp, 4 * W / 8, 4 * H / 21, "%+06.0f", G->agssvpad.DEDA240);
		skp->Text(6 * W / 8, 4 * H / 21, "240", 3);
		skpTextFmt(skp, 4 * W / 8, 5 * H / 21, "%+06.0f", G->agssvpad.DEDA241);
		skp->Text(6 * W / 8, 5 * H / 21, "241", 3);
		skpTextFmt(skp, 4 * W / 8, 6 * H / 21, "%+06.0f", G->agssvpad.DEDA242);
		skp->Text(6 * W / 8, 6 * H / 21, "242", 3);

		skpTextFmt(skp, 4 * W / 8, 7 * H / 21, "%+06.0f", G->agssvpad.DEDA260);
		skp->Text(6 * W / 8, 7 * H / 21, "260", 3);
		skpTextFmt(skp, 4 * W / 8, 8 * H / 21, "%+06.0f", G->agssvpad.DEDA261);
		skp->Text(6 * W / 8, 8 * H / 21, "261", 3);
		skpTextFmt(skp, 4 * W / 8, 9 * H / 21, "%+06.0f", G->agssvpad.DEDA262);
		skp->Text(6 * W / 8, 9 * H / 21, "262", 3);

		skpTextFmt(skp, 4 * W / 8, 10 * H / 21, "%+07.1f", G->agssvpad.DEDA254);
		skp->Text(6 * W / 8, 10 * H / 21, "254", 3);

		skpTextFmt(skp, 4 * W / 8, 11 * H / 21, "%+06.0f", G->agssvpad.DEDA244);
		skp->Text(6 * W / 8, 11 * H / 21, "244", 3);
		skpTextFmt(skp, 4 * W / 8, 12 * H / 21, "%+06.0f", G->agssvpad.DEDA245);
		skp->Text(6 * W / 8, 12 * H / 21, "245", 3);
		skpTextFmt(skp, 4 * W / 8, 13 * H / 21, "%+06.0f", G->agssvpad.DEDA246);
		skp->Text(6 * W / 8, 13 * H / 21, "246", 3);

		skpTextFmt(skp, 4 * W / 8, 14 * H / 21, "%+06.0f", G->agssvpad.DEDA264);
		skp->Text(6 * W / 8, 14 * H / 21, "264", 3);
		skpTextFmt(skp, 4 * W / 8, 15 * H / 21, "%+06.0f", G->agssvpad.DEDA265);
		skp->Text(6 * W / 8, 15 * H / 21, "265", 3);
		skpTextFmt(skp, 4 * W / 8, 16 * H / 21, "%+06.0f", G->agssvpad.DEDA266);
		skp->Text(6 * W / 8, 16 * H / 21, "266", 3);

		skpTextFmt(skp, 4 * W / 8, 17 * H / 21, "%+07.1f", G->agssvpad.DEDA272);
		skp->Text(6 * W / 8, 17 * H / 21, "272", 3);
	}
	else if (screen == 8)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Config", 6);

		if (GC->mission == 0)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Manual", 8);
		}
		else if (GC->mission >= 7)
		{
			skpTextFmt(skp, 1 * W / 8, 2 * H / 14, "Apollo %i", GC->mission);
		}

		skpTextFmt(skp, 4 * W / 8, 2 * H / 14, "%02d:%02d:%04d", GC->rtcc->GZGENCSN.DayofLiftoff, GC->rtcc->GZGENCSN.MonthofLiftoff, GC->rtcc->GZGENCSN.Year);

		GET_Display2(Buffer, GC->rtcc->GetGMTLO()*3600.0);
		skp->Text(4 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 4 * W / 8, 6 * H / 14, "AGC Epoch: %d", GC->rtcc->SystemParameters.AGCEpoch);

		skp->Text(4 * W / 8, 8 * H / 14, "Update Liftoff Time", 19);



		if (G->vesseltype == 0)
		{
			if (G->vesselisdocked)
			{
				skp->Text(1 * W / 8, 4 * H / 14, "CSM/LM docked", 13);
			}
			else
			{
				skp->Text(1 * W / 8, 4 * H / 14, "CSM", 3);
			}
		}
		else if (G->vesseltype == 1)
		{
			if (G->vesselisdocked)
			{
				skp->Text(1 * W / 8, 4 * H / 14, "LM/CSM docked", 13);
			}
			else
			{
				skp->Text(1 * W / 8, 4 * H / 14, "LM", 3);
			}
		}
		else if (G->vesseltype == 2)
		{
			skp->Text(1 * W / 8, 4 * H / 14, "MCC", 3);
		}
		else
		{
			skp->Text(1 * W / 8, 4 * H / 14, "Unsupported", 11);
		}

		if (G->vesseltype == 1)
		{
			if (G->lemdescentstage)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "Descent Stage", 13);
			}
			else
			{
				skp->Text(1 * W / 8, 6 * H / 14, "Ascent Stage", 12);
			}
		}

		skp->Text(1 * W / 8, 10 * H / 14, "Sxt/Star Check:", 15);
		skpTextFmt(skp, 4 * W / 8, 10 * H / 14, "%.0f min", -G->sxtstardtime / 60.0);

		skp->Text(1 * W / 8, 12 * H / 14, "Uplink in LOS:", 14);

		if (G->inhibUplLOS)
		{
			skp->Text(4 * W / 8, 12 * H / 14, "Inhibit", 7);
		}
		else
		{
			skp->Text(4 * W / 8, 12 * H / 14, "Enabled", 7);
		}

		//skp->Text(1 * W / 8, 12 * H / 14, "DV Format:", 9);
		//skp->Text(5 * W / 8, 12 * H / 14, "AGC DSKY", 8);
	}
	else if (screen == 9)
	{
		if (G->manpadopt == 0)
		{
			if (G->HeadsUp)
			{
				skp->Text((int)(0.5 * W / 8), 6 * H / 14, "Heads Up", 8);
			}
			else
			{
				skp->Text((int)(0.5 * W / 8), 6 * H / 14, "Heads Down", 10);
			}

			if (G->manpad_ullage_opt)
			{
				sprintf_s(Buffer, "4 quads, %.1f s", G->manpad_ullage_dt);
			}
			else
			{
				sprintf_s(Buffer, "2 quads, %.1f s", G->manpad_ullage_dt);
			}
			skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

			//TBD: Find a new place for this?
			//skp->Text((int)(0.5 * W / 8), 8 * H / 14, "REFSMMAT:", 9);
			//REFSMMATName(Buffer, G->REFSMMATcur);
			//skp->Text((int)(0.5 * W / 8), 9 * H / 14, Buffer, strlen(Buffer));

			if (G->vesseltype == 0)
			{
				skp->Text(5 * W / 8, (int)(0.5 * H / 14), "P30 Maneuver", 12);

				if (G->vesselisdocked == false)
				{
					skp->Text((int)(0.5 * W / 8), 2 * H / 14, "CSM", 3);
				}
				else
				{
					skp->Text((int)(0.5 * W / 8), 2 * H / 14, "CSM/LM", 6);
				}

				ThrusterName(Buffer, G->manpadenginetype);
				skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

				if (G->vesselisdocked)
				{
					skpTextFmt(skp, (int)(0.5 * W / 8), 10 * H / 14, "LM Weight: %5.0f", G->manpad.LMWeight);
				}

				skp->Text((int)(0.5 * W / 8), 18 * H / 23, "Set Stars:", 10);
				skp->Text((int)(0.5 * W / 8), 19 * H / 23, G->manpad.SetStars, strlen(G->manpad.SetStars));

				/*if (length(G->manpad.GDCangles) == 0.0)
				{
					skp->Text((int)(0.5 * W / 8), 19 * H / 23, "N/A", 3);
				}
				else
				{
					if (G->GDCset == 0)
					{
						skp->Text((int)(0.5 * W / 8), 19 * H / 23, "Vega, Deneb", 11);
					}
					else if (G->GDCset == 1)
					{
						skp->Text((int)(0.5 * W / 8), 19 * H / 23, "Navi, Polaris", 13);
					}
					else
					{
						skp->Text((int)(0.5 * W / 8), 19 * H / 23, "Acrux, Atria", 12);
					}
				}*/

				skpTextFmt(skp, (int)(0.5 * W / 8), 20 * H / 23, "R %03.0f", OrbMech::round(G->manpad.GDCangles.x));
				skpTextFmt(skp, (int)(0.5 * W / 8), 21 * H / 23, "P %03.0f", OrbMech::round(G->manpad.GDCangles.y));
				skpTextFmt(skp, (int)(0.5 * W / 8), 22 * H / 23, "Y %03.0f", OrbMech::round(G->manpad.GDCangles.z));

				int hh, mm;
				double secs;

				SStoHHMMSS(G->P30TIG, hh, mm, secs);

				skp->Text(7 * W / 8, 3 * H / 26, "N47", 3);
				skp->Text(7 * W / 8, 4 * H / 26, "N48", 3);
				skp->Text(7 * W / 8, 6 * H / 26, "N33", 3);
				skp->Text(7 * W / 8, 9 * H / 26, "N81", 3);
				skp->Text(7 * W / 8, 15 * H / 26, "N44", 3);

				skpTextFmt(skp, (int)(3.5 * W / 8), 3 * H / 26, "%+06.0f WGT", G->manpad.Weight);

				if (G->manpadenginetype == RTCC_ENGINETYPE_CSMSPS)
				{
					skpTextFmt(skp, (int)(3.5 * W / 8), 4 * H / 26, "%+07.2f PTRIM", G->manpad.pTrim);
					skpTextFmt(skp, (int)(3.5 * W / 8), 5 * H / 26, "%+07.2f YTRIM", G->manpad.yTrim);
				}
				else
				{
					skp->Text((int)(3.5 * W / 8), 4 * H / 26, "N/A      PTRIM", 14);
					skp->Text((int)(3.5 * W / 8), 5 * H / 26, "N/A      YTRIM", 14);
				}

				skpTextFmt(skp, (int)(3.5 * W / 8), 6 * H / 26, "%+06d HRS GETI", hh);
				skpTextFmt(skp, (int)(3.5 * W / 8), 7 * H / 26, "%+06d MIN", mm);
				skpTextFmt(skp, (int)(3.5 * W / 8), 8 * H / 26, "%+06.0f SEC", secs * 100.0);

				skpTextFmt(skp, (int)(3.5 * W / 8), 9 * H / 26, "%+07.1f DVX", G->dV_LVLH.x / 0.3048);
				skpTextFmt(skp, (int)(3.5 * W / 8), 10 * H / 26, "%+07.1f DVY", G->dV_LVLH.y / 0.3048);
				skpTextFmt(skp, (int)(3.5 * W / 8), 11 * H / 26, "%+07.1f DVZ", G->dV_LVLH.z / 0.3048);

				skpTextFmt(skp, (int)(3.5 * W / 8), 12 * H / 26, "XXX%03.0f R", G->manpad.Att.x);
				skpTextFmt(skp, (int)(3.5 * W / 8), 13 * H / 26, "XXX%03.0f P", G->manpad.Att.y);
				skpTextFmt(skp, (int)(3.5 * W / 8), 14 * H / 26, "XXX%03.0f Y", G->manpad.Att.z);

				skpTextFmt(skp, (int)(3.5 * W / 8), 15 * H / 26, "%+07.1f HA", min(9999.9, G->manpad.HA));
				skpTextFmt(skp, (int)(3.5 * W / 8), 16 * H / 26, "%+07.1f HP", G->manpad.HP);

				skpTextFmt(skp, (int)(3.5 * W / 8), 17 * H / 26, "%+07.1f VT", G->manpad.Vt);// length(G->dV_LVLH) / 0.3048);

				SStoHHMMSS(G->manpad.burntime, hh, mm, secs);

				skpTextFmt(skp, (int)(3.5 * W / 8), 18 * H / 26, "XXX%d:%02.0f BT (MIN:SEC)", mm, secs);
				skpTextFmt(skp, (int)(3.5 * W / 8), 19 * H / 26, "%+07.1f VC", G->manpad.Vc);

				//skp->Text(4 * W / 8, 13 * H / 20, "SXT star check", 14);

				if (G->manpad.Star == 0)
				{
					sprintf(Buffer, "N/A     SXTS");
					skp->Text((int)(3.5 * W / 8), 20 * H / 26, Buffer, strlen(Buffer));
					sprintf(Buffer, "N/A     SFT");
					skp->Text((int)(3.5 * W / 8), 21 * H / 26, Buffer, strlen(Buffer));
					sprintf(Buffer, "N/A     TRN");
					skp->Text((int)(3.5 * W / 8), 22 * H / 26, Buffer, strlen(Buffer));
				}
				else
				{
					skpTextFmt(skp, (int)(3.5 * W / 8), 20 * H / 26, "XXXX%02d SXTS", G->manpad.Star);
					skpTextFmt(skp, (int)(3.5 * W / 8), 21 * H / 26, "%+07.2f SFT", G->manpad.Shaft);
					skpTextFmt(skp, (int)(3.5 * W / 8), 22 * H / 26, "%+07.3f TRN", G->manpad.Trun);
				}
				if (G->manpad.BSSStar == 0)
				{
					sprintf(Buffer, "N/A     BSS");
					skp->Text((int)(3.5 * W / 8), 23 * H / 26, Buffer, strlen(Buffer));
					sprintf(Buffer, "N/A     SPA");
					skp->Text((int)(3.5 * W / 8), 24 * H / 26, Buffer, strlen(Buffer));
					sprintf(Buffer, "N/A     SXP");
					skp->Text((int)(3.5 * W / 8), 25 * H / 26, Buffer, strlen(Buffer));
				}
				else
				{
					skpTextFmt(skp, (int)(3.5 * W / 8), 23 * H / 26, "XXXX%02d BSS", G->manpad.BSSStar);
					skpTextFmt(skp, (int)(3.5 * W / 8), 24 * H / 26, "%+07.2f SPA", G->manpad.SPA);
					skpTextFmt(skp, (int)(3.5 * W / 8), 25 * H / 26, "%+07.3f SXP", G->manpad.SXP);
				}
			}
			else
			{
				skp->Text(5 * W / 8, (int)(0.5 * H / 14), "P30 LM Maneuver", 15);

				ThrusterName(Buffer, G->manpadenginetype);
				skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

				if (G->vesselisdocked == false)
				{
					skp->Text((int)(0.5 * W / 8), 2 * H / 14, "LM", 3);
				}
				else
				{
					skp->Text((int)(0.5 * W / 8), 2 * H / 14, "LM/CSM", 6);
				}

				skpTextFmt(skp, (int)(0.5 * W / 8), 10 * H / 14, "LM Weight: %5.0f", G->lmmanpad.LMWeight);

				if (G->vesselisdocked)
				{
					skpTextFmt(skp, (int)(0.5 * W / 8), 11 * H / 14, "CSM Weight: %5.0f", G->lmmanpad.CSMWeight);
				}

				int hh, mm;
				double secs;

				SStoHHMMSS(G->P30TIG, hh, mm, secs);

				skpTextFmt(skp, (int)(3.5 * W / 8), 5 * H / 26, "%+06d HRS GETI", hh);
				skpTextFmt(skp, (int)(3.5 * W / 8), 6 * H / 26, "%+06d MIN", mm);
				skpTextFmt(skp, (int)(3.5 * W / 8), 7 * H / 26, "%+06.0f SEC", secs * 100.0);

				skpTextFmt(skp, (int)(3.5 * W / 8), 8 * H / 26, "%+07.1f DVX", G->dV_LVLH.x / 0.3048);
				skpTextFmt(skp, (int)(3.5 * W / 8), 9 * H / 26, "%+07.1f DVY", G->dV_LVLH.y / 0.3048);
				skpTextFmt(skp, (int)(3.5 * W / 8), 10 * H / 26, "%+07.1f DVZ", G->dV_LVLH.z / 0.3048);

				skpTextFmt(skp, (int)(3.5 * W / 8), 11 * H / 26, "%+07.1f HA", min(9999.9, G->lmmanpad.HA));
				skpTextFmt(skp, (int)(3.5 * W / 8), 12 * H / 26, "%+07.1f HP", G->lmmanpad.HP);
				skpTextFmt(skp, (int)(3.5 * W / 8), 13 * H / 26, "%+07.1f DVR", length(G->dV_LVLH) / 0.3048);

				SStoHHMMSS(G->lmmanpad.burntime, hh, mm, secs);

				skpTextFmt(skp, (int)(3.5 * W / 8), 14 * H / 26, "XXX%d:%02.0f BT", mm, secs);
				skpTextFmt(skp, (int)(3.5 * W / 8), 15 * H / 26, "XXX%03.0f R", G->lmmanpad.Att.x);
				skpTextFmt(skp, (int)(3.5 * W / 8), 16 * H / 26, "XXX%03.0f P", G->lmmanpad.Att.y);
				skpTextFmt(skp, (int)(3.5 * W / 8), 17 * H / 26, "%+07.1f DVX AGS N86", G->lmmanpad.dV_AGS.x);
				skpTextFmt(skp, (int)(3.5 * W / 8), 18 * H / 26, "%+07.1f DVY AGS", G->lmmanpad.dV_AGS.y);
				skpTextFmt(skp, (int)(3.5 * W / 8), 19 * H / 26, "%+07.1f DVZ AGS", G->lmmanpad.dV_AGS.z);

				if (G->lmmanpad.BSSStar == 0)
				{
					sprintf(Buffer, "N/A     BSS");
					skp->Text((int)(3.5 * W / 8), 20 * H / 26, Buffer, strlen(Buffer));
					sprintf(Buffer, "N/A     SPA");
					skp->Text((int)(3.5 * W / 8), 21 * H / 26, Buffer, strlen(Buffer));
					sprintf(Buffer, "N/A     SXP");
					skp->Text((int)(3.5 * W / 8), 22 * H / 26, Buffer, strlen(Buffer));
				}
				else
				{
					skpTextFmt(skp, (int)(3.5 * W / 8), 20 * H / 26, "XXXX%02d BSS", G->lmmanpad.BSSStar);
					skpTextFmt(skp, (int)(3.5 * W / 8), 21 * H / 26, "%+07.2f SPA", G->lmmanpad.SPA);
					skpTextFmt(skp, (int)(3.5 * W / 8), 22 * H / 26, "%+07.3f SXP", G->lmmanpad.SXP);
				}

				skp->Text((int)(0.5 * W / 8), 24 * H / 26, G->lmmanpad.remarks, strlen(G->lmmanpad.remarks));
			}
		}
		else if (G->manpadopt == 1)
		{
			skp->Text(4 * W / 8, (int)(0.5 * H / 14), "Terminal Phase Initiate", 23);

			int hh, mm; // ss;
			double secs;

			SStoHHMMSS(G->P30TIG, hh, mm, secs);

			skp->Text(7 * W / 8, 3 * H / 20, "N37", 3);

			skpTextFmt(skp, 3 * W / 8, 3 * H / 20, "%+06d HRS GETI", hh);
			skpTextFmt(skp, 3 * W / 8, 4 * H / 20, "%+06d MIN", mm);
			skpTextFmt(skp, 3 * W / 8, 5 * H / 20, "%+06.0f SEC", secs * 100.0);

			skpTextFmt(skp, 3 * W / 8, 6 * H / 20, "%+07.1f DVX", G->dV_LVLH.x / 0.3048);
			skpTextFmt(skp, 3 * W / 8, 7 * H / 20, "%+07.1f DVY", G->dV_LVLH.y / 0.3048);
			skpTextFmt(skp, 3 * W / 8, 8 * H / 20, "%+07.1f DVZ", G->dV_LVLH.z / 0.3048);

			if (G->TPIPAD_dV_LOS.x > 0)
			{
				sprintf(Buffer, "F%04.1f/%02.0f DVX LOS/BT", abs(G->TPIPAD_dV_LOS.x), G->TPIPAD_BT.x);
			}
			else
			{
				sprintf(Buffer, "A%04.1f/%02.0f DVX LOS/BT", abs(G->TPIPAD_dV_LOS.x), G->TPIPAD_BT.x);
			}
			skp->Text(3 * W / 8, 9 * H / 20, Buffer, strlen(Buffer));
			if (G->TPIPAD_dV_LOS.y > 0)
			{
				sprintf(Buffer, "R%04.1f/%02.0f DVY LOS/BT", abs(G->TPIPAD_dV_LOS.y), G->TPIPAD_BT.y);
			}
			else
			{
				sprintf(Buffer, "L%04.1f/%02.0f DVY LOS/BT", abs(G->TPIPAD_dV_LOS.y), G->TPIPAD_BT.y);
			}
			skp->Text(3 * W / 8, 10 * H / 20, Buffer, strlen(Buffer));
			if (G->TPIPAD_dV_LOS.z > 0)
			{
				sprintf(Buffer, "D%04.1f/%02.0f DVZ LOS/BT", abs(G->TPIPAD_dV_LOS.z), G->TPIPAD_BT.z);
			}
			else
			{
				sprintf(Buffer, "U%04.1f/%02.0f DVZ LOS/BT", abs(G->TPIPAD_dV_LOS.z), G->TPIPAD_BT.z);
			}
			skp->Text(3 * W / 8, 11 * H / 20, Buffer, strlen(Buffer));

			skpTextFmt(skp, 3 * W / 8, 12 * H / 20, "X%04.1f/%02.1f dH TPI/ddH", G->TPIPAD_dH, G->TPIPAD_ddH);
			skpTextFmt(skp, 3 * W / 8, 13 * H / 20, "X%06.2f R", G->TPIPAD_R);
			skpTextFmt(skp, 3 * W / 8, 14 * H / 20, "%+07.1f RDOT at TPI", G->TPIPAD_Rdot);
			skpTextFmt(skp, 3 * W / 8, 15 * H / 20, "X%06.2f EL minus 5 min", G->TPIPAD_ELmin5);
			skpTextFmt(skp, 3 * W / 8, 16 * H / 20, "X%06.2f AZ", G->TPIPAD_AZ);

		}
		else
		{
			if (G->vesseltype == 0)
			{
				skp->Text(4 * W / 8, (int)(0.5 * H / 14), "TLI PAD", 7);

				GET_Display(Buffer, G->tlipad.TB6P);
				skpTextFmt(skp, 3 * W / 8, 3 * H / 20, "%s TB6p", Buffer);

				skpTextFmt(skp, 3 * W / 8, 4 * H / 20, "XXX%03.0f R", G->tlipad.IgnATT.x);
				skpTextFmt(skp, 3 * W / 8, 5 * H / 20, "XXX%03.0f P", G->tlipad.IgnATT.y);
				skpTextFmt(skp, 3 * W / 8, 6 * H / 20, "XXX%03.0f Y", G->tlipad.IgnATT.z);

				double secs;
				int mm, hh;
				SStoHHMMSS(G->tlipad.BurnTime, hh, mm, secs);

				skpTextFmt(skp, 3 * W / 8, 7 * H / 20, "XXX%d:%02.0f BT", mm, secs);

				skpTextFmt(skp, 3 * W / 8, 8 * H / 20, "%07.1f DVC", G->tlipad.dVC);
				skpTextFmt(skp, 3 * W / 8, 9 * H / 20, "%+06.0f VI", G->tlipad.VI);

				skpTextFmt(skp, 3 * W / 8, 10 * H / 20, "XXX%03.0f R", G->tlipad.SepATT.x);
				skpTextFmt(skp, 3 * W / 8, 11 * H / 20, "XXX%03.0f P SEP", G->tlipad.SepATT.y);
				skpTextFmt(skp, 3 * W / 8, 12 * H / 20, "XXX%03.0f Y", G->tlipad.SepATT.z);

				skpTextFmt(skp, 3 * W / 8, 13 * H / 20, "XXX%03.0f R", G->tlipad.ExtATT.x);
				skpTextFmt(skp, 3 * W / 8, 14 * H / 20, "XXX%03.0f P EXTRACTION", G->tlipad.ExtATT.y);
				skpTextFmt(skp, 3 * W / 8, 15 * H / 20, "XXX%03.0f Y", G->tlipad.ExtATT.z);
			}
			else
			{
				skp->Text(5 * W / 8, (int)(0.5 * H / 14), "PDI PAD", 7);

				if (G->HeadsUp)
				{
					skp->Text((int)(0.5 * W / 8), 6 * H / 14, "Heads Up", 8);
				}
				else
				{
					skp->Text((int)(0.5 * W / 8), 6 * H / 14, "Heads Down", 10);
				}

				skp->Text(4 * W / 8, 15 * H / 20, "T_L:", 4);
				GET_Display(Buffer, GC->rtcc->CZTDTGTU.GETTD);
				skp->Text(5 * W / 8, 15 * H / 20, Buffer, strlen(Buffer));

				skp->Text(4 * W / 8, 16 * H / 20, "Lat:", 4);
				skpTextFmt(skp, 5 * W / 8, 16 * H / 20, "%.3f°", GC->rtcc->BZLAND.lat[RTCC_LMPOS_BEST] * DEG);

				skp->Text(4 * W / 8, 17 * H / 20, "Lng:", 4);
				skpTextFmt(skp, 5 * W / 8, 17 * H / 20, "%.3f°", GC->rtcc->BZLAND.lng[RTCC_LMPOS_BEST] * DEG);

				skp->Text(4 * W / 8, 18 * H / 20, "Rad:", 4);
				skpTextFmt(skp, 5 * W / 8, 18 * H / 20, "%.2f NM", GC->rtcc->BZLAND.rad[RTCC_LMPOS_BEST] / 1852.0);

				if (!G->PADSolGood)
				{
					skp->Text(5 * W / 8, 2 * H / 14, "Calculation failed!", 19);
				}

				int hh, mm; // ss;
				double secs;

				SStoHHMMSS(G->pdipad.GETI, hh, mm, secs);

				skp->Text(3 * W / 8, 5 * H / 20, "HRS", 3);
				skp->Text((int)(4.5 * W / 8), 5 * H / 20, "TIG", 3);
				skpTextFmt(skp, 6 * W / 8, 5 * H / 20, "%+06d", hh);

				skp->Text(3 * W / 8, 6 * H / 20, "MIN", 3);
				skp->Text((int)(4.5 * W / 8), 6 * H / 20, "PDI", 3);
				skpTextFmt(skp, 6 * W / 8, 6 * H / 20, "%+06d", mm);

				skp->Text(3 * W / 8, 7 * H / 20, "SEC", 3);
				skpTextFmt(skp, 6 * W / 8, 7 * H / 20, "%+06.0f", secs * 100.0);

				SStoHHMMSS(G->pdipad.t_go, hh, mm, secs);
				skp->Text(3 * W / 8, 8 * H / 20, "TGO", 3);
				skp->Text((int)(4.5 * W / 8), 8 * H / 20, "N61", 3);
				skpTextFmt(skp, 6 * W / 8, 8 * H / 20, "XX%02d:%02.0f", mm, secs);

				skp->Text(3 * W / 8, 9 * H / 20, "CROSSRANGE", 10);
				skpTextFmt(skp, 6 * W / 8, 9 * H / 20, "%07.1f", G->pdipad.CR);

				skp->Text(3 * W / 8, 10 * H / 20, "R", 1);
				skp->Text((int)(4.5 * W / 8), 10 * H / 20, "FDAI", 4);
				skpTextFmt(skp, 6 * W / 8, 10 * H / 20, "XXX%03.0f", G->pdipad.Att.x);

				skp->Text(3 * W / 8, 11 * H / 20, "P", 1);
				skp->Text((int)(4.5 * W / 8), 11 * H / 20, "AT TIG", 6);
				skpTextFmt(skp, 6 * W / 8, 11 * H / 20, "XXX%03.0f", G->pdipad.Att.y);

				skp->Text(3 * W / 8, 12 * H / 20, "Y", 1);
				skpTextFmt(skp, 6 * W / 8, 12 * H / 20, "XXX%03.0f", G->pdipad.Att.z);

				skp->Text(3 * W / 8, 13 * H / 20, "DEDA 231 IF RQD", 15);
				skpTextFmt(skp, 6 * W / 8, 13 * H / 20, "%+06.0f", G->pdipad.DEDA231);
			}
		}
	}
	else if (screen == 10)
	{

		if (G->entrypadopt == 0)
		{
			skp->Text(5 * W / 8, 1 * H / 28, "Earth Entry PAD", 15);

			skp->SetFont(font2);

			skp->Text(7 * W / 16, 3 * H / 32, "PREBURN", 7);

			skpTextFmt(skp, 3 * W / 8, 4 * H / 32, "XX%+05.1f dV TO", G->earthentrypad.dVTO[0]);

			skpTextFmt(skp, 3 * W / 8, 5 * H / 32, "XXX%03.0f R 0.05G", G->earthentrypad.Att400K[0].x);
			skpTextFmt(skp, 3 * W / 8, 6 * H / 32, "XXX%03.0f P 0.05G", G->earthentrypad.Att400K[0].y);
			skpTextFmt(skp, 3 * W / 8, 7 * H / 32, "XXX%03.0f Y 0.05G", G->earthentrypad.Att400K[0].z);

			skpTextFmt(skp, 3 * W / 8, 8 * H / 32, "%+07.1f RTGO .05G", G->earthentrypad.RTGO[0]);
			skpTextFmt(skp, 3 * W / 8, 9 * H / 32, "%+06.0f VIO  .05G", G->earthentrypad.VIO[0]);

			double secs;
			int mm, hh;

			SStoHHMMSS(G->earthentrypad.Ret05[0], hh, mm, secs);

			skpTextFmt(skp, 3 * W / 8, 10 * H / 32, "XX%02d:%02.0f RET  .05G", mm, secs);
			skpTextFmt(skp, 3 * W / 8, 11 * H / 32, "%+07.2f LAT", G->earthentrypad.Lat[0]);
			skpTextFmt(skp, 3 * W / 8, 12 * H / 32, "%+07.2f LONG", G->earthentrypad.Lng[0]);

			SStoHHMMSS(G->earthentrypad.Ret2[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 13 * H / 32, "XX%02d:%02.0f RET  .2G", mm, secs);
			skpTextFmt(skp, 3 * W / 8, 14 * H / 32, "%+07.1lf DRE (55°)  N66", G->earthentrypad.DRE[0]);

			sprintf(Buffer, "RR55/55 BANK AN");
			skp->Text(3 * W / 8, 15 * H / 32, Buffer, strlen(Buffer));

			SStoHHMMSS(G->earthentrypad.RetRB[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 16 * H / 32, "XX%02d:%02.0f RET RB", mm, secs);

			SStoHHMMSS(G->earthentrypad.RetBBO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 17 * H / 32, "XX%02d:%02.0f RETBBO", mm, secs);

			SStoHHMMSS(G->earthentrypad.RetEBO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 18 * H / 32, "XX%02d:%02.0f RETEBO", mm, secs);

			SStoHHMMSS(G->earthentrypad.RetDrog[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 19 * H / 32, "XX%02d:%02.0f RETDROG", mm, secs);

			skp->Text(7 * W / 16, 20 * H / 32, "POSTBURN", 8);

			skpTextFmt(skp, 3 * W / 8, 21 * H / 32, "XXX%03.0f R 0.05G", G->earthentrypad.PB_R400K[0]);
			skpTextFmt(skp, 3 * W / 8, 22 * H / 32, "%+07.1f RTGO .05G", G->earthentrypad.PB_RTGO[0]);
			skpTextFmt(skp, 3 * W / 8, 23 * H / 32, "%+06.0f VIO  .05G", G->earthentrypad.PB_VIO[0]);

			SStoHHMMSS(G->earthentrypad.PB_Ret05[0], hh, mm, secs);

			skpTextFmt(skp, 3 * W / 8, 24 * H / 32, "XX%02d:%02.0f RET  .05G", mm, secs);

			SStoHHMMSS(G->earthentrypad.PB_Ret2[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 25 * H / 32, "XX%02d:%02.0f RET  .2G", mm, secs);

			skpTextFmt(skp, 3 * W / 8, 26 * H / 32, "%+07.1lf DRE +/- 100nm  N66", G->earthentrypad.PB_DRE[0]);

			sprintf(Buffer, "RR55/55 BANK AN");
			skp->Text(3 * W / 8, 27 * H / 32, Buffer, strlen(Buffer));

			SStoHHMMSS(G->earthentrypad.PB_RetRB[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 28 * H / 32, "XX%02d:%02.0f RET RB", mm, secs);

			SStoHHMMSS(G->earthentrypad.PB_RetBBO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 29 * H / 32, "XX%02d:%02.0f RETBBO", mm, secs);

			SStoHHMMSS(G->earthentrypad.PB_RetEBO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 30 * H / 32, "XX%02d:%02.0f RETEBO", mm, secs);

			SStoHHMMSS(G->earthentrypad.PB_RetDrog[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 31 * H / 32, "XX%02d:%02.0f RETDROG", mm, secs);

			skp->Text(1 * W / 16, 6 * H / 21, "Deorbit:", 8);

			ThrusterName(Buffer, G->manpadenginetype);
			skp->Text(1 * W / 16, 7 * H / 21, Buffer, strlen(Buffer));

			GET_Display(Buffer, G->P30TIG);
			skp->Text(1 * W / 16, 8 * H / 21, Buffer, strlen(Buffer));

			skp->Text(1 * W / 16, 9 * H / 21, "DVX", 3);
			skp->Text(1 * W / 16, 10 * H / 21, "DVY", 3);
			skp->Text(1 * W / 16, 11 * H / 21, "DVZ", 3);

			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(3 * W / 16, 9 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(3 * W / 16, 10 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(3 * W / 16, 11 * H / 21, Buffer, strlen(Buffer));

			skp->Text(1 * W / 16, 12 * H / 21, "Initial Bank:", 13);
			skpTextFmt(skp, 1 * W / 16, 13 * H / 21, "%+.2lf°", GC->rtcc->RZC1RCNS.entry.GNInitialBank* DEG);

			skp->Text(1 * W / 16, 14 * H / 21, "G-Level:", 8);
			skpTextFmt(skp, 1 * W / 16, 15 * H / 21, "%+.2lf", GC->rtcc->RZC1RCNS.entry.GLevel);

			skp->Text(1 * W / 16, 16 * H / 21, "Splashdown:", 11);
			skpTextFmt(skp, 1 * W / 16, 17 * H / 21, "Lat:  %+.2f°", G->EntryLatcor* DEG);
			skpTextFmt(skp, 1 * W / 16, 18 * H / 21, "Long: %+.2f°", G->EntryLngcor* DEG);
		}
		else
		{
			skp->Text(5 * W / 8, 1 * H / 28, "Lunar Entry PAD", 15);

			skp->SetFont(font2);

			if (G->entryrange != 0)
			{
				skp->Text((int)(0.5 * W / 8), 6 * H / 14, "Desired Range:", 14);
				skpTextFmt(skp, (int)(0.5 * W / 8), 7 * H / 14, "%.1f NM", G->entryrange);
			}

			skpTextFmt(skp, 3 * W / 8, 5 * H / 32, "XXX%03.0f R 0.05G", G->lunarentrypad.Att05[0].x);
			skpTextFmt(skp, 3 * W / 8, 6 * H / 32, "XXX%03.0f P 0.05G", G->lunarentrypad.Att05[0].y);
			skpTextFmt(skp, 3 * W / 8, 7 * H / 32, "XXX%03.0f Y 0.05G", G->lunarentrypad.Att05[0].z);

			GET_Display(Buffer, G->lunarentrypad.GETHorCheck[0]);
			skp->Text(3 * W / 8, 8 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 3 * W / 8, 9 * H / 32, "XXX%03.0f P HOR CK", G->lunarentrypad.PitchHorCheck[0]);

			skpTextFmt(skp, 3 * W / 8, 10 * H / 32, "%+07.2f LAT", G->lunarentrypad.Lat[0]);
			skpTextFmt(skp, 3 * W / 8, 11 * H / 32, "%+07.2f LONG", G->lunarentrypad.Lng[0]);

			skpTextFmt(skp, 3 * W / 8, 12 * H / 32, "XXX%04.1f MAX G", G->lunarentrypad.MaxG[0]);

			skpTextFmt(skp, 3 * W / 8, 13 * H / 32, "%+06.0f V400k", G->lunarentrypad.V400K[0]);
			skpTextFmt(skp, 3 * W / 8, 14 * H / 32, "%+07.2f y400k", G->lunarentrypad.Gamma400K[0]);

			skpTextFmt(skp, 3 * W / 8, 15 * H / 32, "%+07.1f RTGO .05G", G->lunarentrypad.RTGO[0]);
			skpTextFmt(skp, 3 * W / 8, 16 * H / 32, "%+06.0f VIO  .05G", G->lunarentrypad.VIO[0]);

			GET_Display(Buffer, G->lunarentrypad.RRT[0]);
			skpTextFmt(skp, 3 * W / 8, 17 * H / 32, "%s RRT", Buffer);

			double secs;
			int mm, hh;

			SStoHHMMSS(G->lunarentrypad.RET05[0], hh, mm, secs);

			skpTextFmt(skp, 3 * W / 8, 18 * H / 32, "XX%02d:%02.0f RET  .05G", mm, secs);
			skpTextFmt(skp, 3 * W / 8, 19 * H / 32, "%+07.2lf DL MAX", G->lunarentrypad.DLMax[0]);
			skpTextFmt(skp, 3 * W / 8, 20 * H / 32, "%+07.2lf DL MIN", G->lunarentrypad.DLMin[0]);
			skpTextFmt(skp, 3 * W / 8, 21 * H / 32, "%+06.0lf VL MAX", G->lunarentrypad.VLMax[0]);
			skpTextFmt(skp, 3 * W / 8, 22 * H / 32, "%+06.0lf VL MIN", G->lunarentrypad.VLMin[0]);
			skpTextFmt(skp, 3 * W / 8, 23 * H / 32, "XXX%04.2f DO", G->lunarentrypad.DO[0]);

			SStoHHMMSS(G->lunarentrypad.RETVCirc[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 24 * H / 32, "XX%02d:%02.0f RET V CIRC", mm, secs);

			SStoHHMMSS(G->lunarentrypad.RETBBO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 25 * H / 32, "XX%02d:%02.0f RETBBO", mm, secs);

			SStoHHMMSS(G->lunarentrypad.RETEBO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 26 * H / 32, "XX%02d:%02.0f RETEBO", mm, secs);

			SStoHHMMSS(G->lunarentrypad.RETDRO[0], hh, mm, secs);
			skpTextFmt(skp, 3 * W / 8, 27 * H / 32, "XX%02d:%02.0f RETDRO", mm, secs);

			if (G->lunarentrypad.SXTS[0] == 0)
			{
				sprintf(Buffer, "N/A     SXTS");
				skp->Text(3 * W / 8, 28 * H / 32, Buffer, strlen(Buffer));
				sprintf(Buffer, "N/A     SFT");
				skp->Text(3 * W / 8, 29 * H / 32, Buffer, strlen(Buffer));
				sprintf(Buffer, "N/A     TRN");
				skp->Text(3 * W / 8, 30 * H / 32, Buffer, strlen(Buffer));
			}
			else
			{
				skpTextFmt(skp, 3 * W / 8, 28 * H / 32, "XXXX%02d SXTS", G->lunarentrypad.SXTS[0]);
				skpTextFmt(skp, 3 * W / 8, 29 * H / 32, "%+07.2f SFT", G->lunarentrypad.SFT[0]);
				skpTextFmt(skp, 3 * W / 8, 30 * H / 32, "%+07.3f TRN", G->lunarentrypad.TRN[0]);
			}

			skpTextFmt(skp, 3 * W / 8, 31 * H / 32, "XXXX%s LIFT VECTOR", G->lunarentrypad.LiftVector[0]);

			skp->Text(1 * W / 16, 12 * H / 21, "Splashdown:", 11);
			skpTextFmt(skp, 1 * W / 16, 13 * H / 21, "Lat:  %+.2f°", G->EntryLatcor* DEG);
			skpTextFmt(skp, 1 * W / 16, 14 * H / 21, "Long: %+.2f°", G->EntryLngcor* DEG);
		}
	}
	else if (screen == 11)
	{
		char Buffer2[100];

		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Map Update", 10);

		GET_Display(Buffer, G->mapUpdateGET);
		skp->Text(1 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));

		if (G->mappage == 0)
		{
			skp->Text(6 * W / 8, 4 * H / 14, "Earth", 5);

			sprintf(Buffer, gsnames[G->mapgs]);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

			GET_Display(Buffer2, G->GSAOSGET);
			skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "AOS %s", Buffer2);

			GET_Display(Buffer2, G->GSLOSGET);
			skpTextFmt(skp, 1 * W / 8, 7 * H / 14, "LOS %s", Buffer2);
		}
		else if (G->mappage == 1)
		{
			skp->Text(6 * W / 8, 4 * H / 14, "Moon", 4);

			GET_Display(Buffer2, G->mapupdate.LOSGET);
			skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "LOS %s", Buffer2);

			GET_Display(Buffer2, G->mapupdate.SRGET);
			skpTextFmt(skp, 1 * W / 8, 5 * H / 14, "SR  %s", Buffer2);

			GET_Display(Buffer2, G->mapupdate.PMGET);
			skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "PM  %s", Buffer2);

			GET_Display(Buffer2, G->mapupdate.AOSGET);
			skpTextFmt(skp, 1 * W / 8, 7 * H / 14, "AOS %s", Buffer2);

			GET_Display(Buffer2, G->mapupdate.SSGET);
			skpTextFmt(skp, 1 * W / 8, 8 * H / 14, "SS  %s", Buffer2);
		}
	}
	else if (screen == 12)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "LOI Computation (MED K18)", 25);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 8, 2 * H / 14, "LOI Initialization", 18);

		if (GC->MissionPlanningActive)
		{
			GET_Display(Buffer, GC->rtcc->med_k18.VectorTime);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
		}

		skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "%.1lf NM", GC->rtcc->med_k18.HALOI1);
		skpTextFmt(skp, 1 * W / 8, 8 * H / 14, "%.1lf NM", GC->rtcc->med_k18.HPLOI1);
		skpTextFmt(skp, 1 * W / 8, 10 * H / 14, "%.0lf ft/s", GC->rtcc->med_k18.DVMAXp);
		skpTextFmt(skp, 1 * W / 8, 12 * H / 14, "%.0lf ft/s", GC->rtcc->med_k18.DVMAXm);
		skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%.1f°", GC->rtcc->med_k18.psi_MN);
		skpTextFmt(skp, 5 * W / 8, 6 * H / 14, "%.1f°", GC->rtcc->med_k18.psi_DS);
		skpTextFmt(skp, 5 * W / 8, 8 * H / 14, "%.1f°", GC->rtcc->med_k18.psi_MX);
	}
	else if (screen == 13)
	{
		char Buffer2[100];
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "Landmark Tracking", 17);

		GET_Display(Buffer, G->LmkTime);
		skp->Text(1 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
		skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "%.3f°", G->LmkLat * DEG);
		skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "%.3f°", G->LmkLng * DEG);

		GET_Display(Buffer2, G->landmarkpad.T1[0]);
		skpTextFmt(skp, 4 * W / 8, 6 * H / 14, "T1: %s (HOR)", Buffer2);
		GET_Display(Buffer2, G->landmarkpad.T2[0]);
		skpTextFmt(skp, 4 * W / 8, 7 * H / 14, "T2: %s (35°)", Buffer2);

		if (G->landmarkpad.CRDist[0] > 0)
		{
			sprintf(Buffer, "%.1f NM North", G->landmarkpad.CRDist[0]);
		}
		else
		{
			sprintf(Buffer, "%.1f NM South", abs(G->landmarkpad.CRDist[0]));
		}

		skp->Text(4 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));
		skp->Text(4 * W / 8, 9 * H / 14, "N89", 3);
		skpTextFmt(skp, 4 * W / 8, 10 * H / 14, "Lat %+07.3f°", G->landmarkpad.Lat[0]);
		skpTextFmt(skp, 4 * W / 8, 11 * H / 14, "Long/2 %+07.3f°", G->landmarkpad.Lng05[0]);
		skpTextFmt(skp, 4 * W / 8, 12 * H / 14, "Alt %+07.2f NM", G->landmarkpad.Alt[0]);
	}
	else if (screen == 14)
	{
		if (G->vesseltype == 0)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "CSM", 3);
		}
		else if (G->vesseltype == 1)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "LM", 2);
		}
		else
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "MCC", 3);
		}

		skp->Text(1 * W / 16, 2 * H / 14, "Rendezvous", 10);
		skp->Text(1 * W / 16, 4 * H / 14, "General Purpose Maneuver", 24);
		skp->Text(1 * W / 16, 6 * H / 14, "Midcourse", 9);
		skp->Text(1 * W / 16, 8 * H / 14, "Lunar Insertion", 15);
		skp->Text(1 * W / 16, 10 * H / 14, "Return to Earth", 15);
		skp->Text(1 * W / 16, 12 * H / 14, "Deorbit", 7);

		skp->Text(5 * W / 8, 2 * H / 14, "Descent Planning", 16);
		skp->Text(5 * W / 8, 4 * H / 14, "LLWP", 4);
		skp->Text(5 * W / 8, 6 * H / 14, "LLTP", 4);
		skp->Text(5 * W / 8, 8 * H / 14, "Lunar Ascent", 12);
		skp->Text(5 * W / 8, 10 * H / 14, "Perigee Adjust", 14);
		skp->Text(5 * W / 8, 12 * H / 14, "Previous Page", 13);
	}
	else if (screen == 15)
	{
		if (G->VECoption == 0)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Point SC at body", 16);
		}
		else
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Open hatch thermal control", 26);
		}

		if (G->VECoption == 0)
		{
			if (G->VECbody != NULL)
			{
				oapiGetObjectName(G->VECbody, Buffer, 20);
				skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
			}

			if (G->VECdirection == 0)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "+X", 2);
			}
			else if (G->VECdirection == 1)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "-X", 2);
			}
			else if (G->VECdirection == 2)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "+Y", 2);
			}
			else if (G->VECdirection == 3)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "-Y", 2);
			}
			else if (G->VECdirection == 4)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "+Z", 2);
			}
			else if (G->VECdirection == 5)
			{
				skp->Text(1 * W / 8, 6 * H / 14, "-Z", 2);
			}
		}

		skpTextFmt(skp, 6 * W / 8, 10 * H / 14, "%+07.2f R", G->VECangles.x * DEG);
		skpTextFmt(skp, 6 * W / 8, 11 * H / 14, "%+07.2f P", G->VECangles.y * DEG);
		skpTextFmt(skp, 6 * W / 8, 12 * H / 14, "%+07.2f Y", G->VECangles.z * DEG);
	}
	else if (screen == 16)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "Computation for Lunar Descent Planning (K16)", 44);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 16, 2 * H / 14, "Init", 4);

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_k16.Vehicle == RTCC_MPT_LM)
			{
				skp->Text(1 * W / 16, 4 * H / 14, "LEM", 3);
			}
			else
			{
				skp->Text(1 * W / 16, 4 * H / 14, "CSM", 3);
			}

			GET_Display(Buffer, GC->rtcc->med_k16.VectorTime, false);
			skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));
		}

		if (GC->rtcc->med_k16.Mode == 1)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "CSM Phase Change", 16);

			if (GC->rtcc->med_k16.Sequence == 1)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "1: PC, DOI", 10);
			}
			else if (GC->rtcc->med_k16.Sequence == 2)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "2: PCC, DOI", 11);
			}
			else if (GC->rtcc->med_k16.Sequence == 3)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "3: ASP, CIA, DOI", 16);
			}
			else if (GC->rtcc->med_k16.Sequence == 4)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "4: PCCH, DOI", 12);
			}
			else if (GC->rtcc->med_k16.Sequence == 5)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "5: PCCT, DOI", 12);
			}
		}
		else if (GC->rtcc->med_k16.Mode == 2)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "Single CSM Maneuver", 19);

			if (GC->rtcc->med_k16.Sequence == 2)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "2: ASH, DOI", 11);
			}
			else if (GC->rtcc->med_k16.Sequence == 3)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "3: CIR, DOI", 11);
			}
			else
			{
				sprintf(Buffer, "%d: Not Used", GC->rtcc->med_k16.Sequence);
				skp->Text(1 * W / 16, 10 * H / 14, Buffer, strlen(Buffer));
			}
		}
		else if (GC->rtcc->med_k16.Mode == 3)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "Double CSM Maneuver", 19);

			if (GC->rtcc->med_k16.Sequence == 1)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "1: ASH at time, CIA, DOI", 24);
			}
			else if (GC->rtcc->med_k16.Sequence == 3)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "3: ASH at apsis, CIA, DOI", 25);
			}
			else
			{
				skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%d: Not Used", GC->rtcc->med_k16.Sequence);
			}
		}
		else if (GC->rtcc->med_k16.Mode == 4)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "LM Maneuver Sequence", 20);
			skp->Text(1 * W / 16, 10 * H / 14, "DOI", 3);
		}
		else if (GC->rtcc->med_k16.Mode == 5)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "Double Hohmann, PC", 28);

			if (GC->rtcc->med_k16.Sequence == 1)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "1: PC, HO1, HO2, DOI", 20);
			}
			else if (GC->rtcc->med_k16.Sequence == 2)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "2: HO1, PC, HO2, DOI", 20);
			}
			else if (GC->rtcc->med_k16.Sequence == 3)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "3: HO1, HO2, PC, DOI", 20);
			}
			else
			{
				skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%d: Not Used", GC->rtcc->med_k16.Sequence);
			}
		}
		else if (GC->rtcc->med_k16.Mode == 6)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "LM Powered Descent (N/A)", 24);
		}
		else if (GC->rtcc->med_k16.Mode == 7)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "CSM Prelaunch Plane Change", 26);
			skp->Text(1 * W / 16, 10 * H / 14, "PPC", 3);
		}

		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.3f NM", GC->rtcc->med_k16.DesiredHeight / 1852.0);

		GET_Display(Buffer, GC->rtcc->med_k16.GETTH1, false);
		skp->Text(5 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->med_k16.GETTH2, false);
		skp->Text(5 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->med_k16.GETTH3, false);
		skp->Text(5 * W / 8, 6 * H / 14, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->med_k16.GETTH4, false);
		skp->Text(5 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));
	}
	else if (screen == 17)
	{
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "Skylab Rendezvous", 17);

		if (G->target != NULL)
		{
			sprintf(Buffer, G->target->GetName());
			skp->Text(5 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
		}

		if (G->Skylabmaneuver != 0)
		{
			skp->Text(4 * W / 8, 16 * H / 21, "TIG", 3);
			GET_Display(Buffer, G->P30TIG);
			skp->Text(5 * W / 8, 16 * H / 21, Buffer, strlen(Buffer));

			skpTextFmt(skp, 5 * W / 8, 17 * H / 21, "%+07.1f", G->dV_LVLH.x / 0.3048);
			skpTextFmt(skp, 5 * W / 8, 18 * H / 21, "%+07.1f", G->dV_LVLH.y / 0.3048);
			skpTextFmt(skp, 5 * W / 8, 19 * H / 21, "%+07.1f", G->dV_LVLH.z / 0.3048);
		}

		if (G->Skylabmaneuver < 7)
		{
			skp->Text(4 * W / 8, 5 * H / 21, "TPI", 3);
			GET_Display(Buffer, G->t_TPI);
			skp->Text(5 * W / 8, 5 * H / 21, Buffer, strlen(Buffer));
		}

		if (!G->SkylabSolGood)
		{
			skp->Text(3 * W / 8, 7 * H / 14, "Calculation failed!", 19);
		}

		if (G->Skylabmaneuver == 0)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "TPI Search", 10);

			GET_Display(Buffer, G->SkylabTPIGuess);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
		}
		if (G->Skylabmaneuver == 1 || G->Skylabmaneuver == 2)
		{
			skpTextFmt(skp, 1 * W / 8, 10 * H / 14, "%.1f NM", G->SkylabDH2 / 1852.0);

			skpTextFmt(skp, 1 * W / 8, 12 * H / 14, "%.2f°", G->Skylab_E_L* DEG);

			skp->Text(4 * W / 8, 10 * H / 21, "NCC", 3);
			GET_Display(Buffer, G->Skylab_t_NCC);
			skp->Text(5 * W / 8, 10 * H / 21, Buffer, strlen(Buffer));
			skpTextFmt(skp, 5 * W / 8, 11 * H / 21, "%+07.1f ft/s", G->Skylab_dv_NCC / 0.3048);

			skp->Text(4 * W / 8, 12 * H / 21, "NSR", 3);
			GET_Display(Buffer, G->Skylab_t_NSR);
			skp->Text(5 * W / 8, 12 * H / 21, Buffer, strlen(Buffer));
			skpTextFmt(skp, 5 * W / 8, 13 * H / 21, "%+07.1f ft/s", length(G->Skylab_dV_NSR) / 0.3048);
		}
		if (G->Skylabmaneuver == 1)
		{
			if (G->Skylab_NPCOption)
			{
				skp->Text(1 * W / 8, 2 * H / 14, "NC1 with Plane Change", 21);
			}
			else
			{
				skp->Text(1 * W / 8, 2 * H / 14, "NC1", 3);
			}

			GET_Display(Buffer, G->Skylab_t_NC1);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

			skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "%.1f", G->Skylab_n_C);
			skpTextFmt(skp, 1 * W / 8, 8 * H / 14, "%.1f NM", G->SkylabDH1 / 1852.0);

			skp->Text(4 * W / 8, 7 * H / 21, "NC2", 3);
			GET_Display(Buffer, G->Skylab_t_NC2);
			skp->Text(5 * W / 8, 7 * H / 21, Buffer, strlen(Buffer));
			skpTextFmt(skp, 5 * W / 8, 8 * H / 21, "%+07.1f ft/s", G->Skylab_dv_NC2 / 0.3048);
			skpTextFmt(skp, 5 * W / 8, 9 * H / 21, "DH: %.1f NM", G->Skylab_dH_NC2 / 1852.0);

			skp->Text(4 * W / 8, 14 * H / 21, "DVT", 3);
			skpTextFmt(skp, 5 * W / 8, 14 * H / 21, "%+07.1f ft/s", (length(G->dV_LVLH) + G->Skylab_dv_NC2 + G->Skylab_dv_NCC + length(G->Skylab_dV_NSR)) / 0.3048);
		}
		if (G->Skylabmaneuver == 2)
		{
			if (G->Skylab_NPCOption)
			{
				skp->Text(1 * W / 8, 2 * H / 14, "NC2 with Plane Change", 21);
			}
			else
			{
				skp->Text(1 * W / 8, 2 * H / 14, "NC2", 3);
			}

			GET_Display(Buffer, G->Skylab_t_NC2);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

			skp->Text(4 * W / 8, 14 * H / 21, "DVT", 3);
			skpTextFmt(skp, 5 * W / 8, 14 * H / 21, "%+07.1f ft/s", (length(G->dV_LVLH) + G->Skylab_dv_NCC + length(G->Skylab_dV_NSR)) / 0.3048);
		}
		if (G->Skylabmaneuver == 3)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "NCC", 3);

			GET_Display(Buffer, G->Skylab_t_NCC);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

			skpTextFmt(skp, 1 * W / 8, 10 * H / 14, "%.1f NM", G->SkylabDH2 / 1852.0);

			skpTextFmt(skp, 1 * W / 8, 12 * H / 14, "%.2f°", G->Skylab_E_L * DEG);

			skp->Text(4 * W / 8, 10 * H / 21, "NSR", 3);
			GET_Display(Buffer, G->Skylab_t_NSR);
			skp->Text(5 * W / 8, 10 * H / 21, Buffer, strlen(Buffer));
			skpTextFmt(skp, 5 * W / 8, 11 * H / 21, "%+07.1f", G->Skylab_dV_NSR.x / 0.3048);
			skpTextFmt(skp, 5 * W / 8, 12 * H / 21, "%+07.1f", G->Skylab_dV_NSR.y / 0.3048);
			skpTextFmt(skp, 5 * W / 8, 13 * H / 21, "%+07.1f", G->Skylab_dV_NSR.z / 0.3048);
		}
		if (G->Skylabmaneuver == 4)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "NSR", 3);

			GET_Display(Buffer, G->Skylab_t_NSR);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

			skpTextFmt(skp, 1 * W / 8, 12 * H / 14, "%.2f°", G->Skylab_E_L* DEG);
		}
		if (G->Skylabmaneuver == 5)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "TPI", 3);

			skp->Text(1 * W / 8, 4 * H / 14, "Calculate TPI TIG", 17);

			skpTextFmt(skp, 1 * W / 8, 12 * H / 14, "%.2f°", G->Skylab_E_L * DEG);
		}
		if (G->Skylabmaneuver == 6)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "TPM", 3);

			skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "DT = %.1f mins", G->Skylab_dt_TPM / 60.0);
		}
		if (G->Skylabmaneuver == 7)
		{
			if (G->Skylab_PCManeuver == 0)
			{
				skp->Text(1 * W / 8, 2 * H / 14, "NPC after NC1", 13);
			}
			else
			{
				skp->Text(1 * W / 8, 2 * H / 14, "NPC after NC2", 13);
			}
		}
	}
	else if (screen == 18)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "Initialization for Lunar Descent Planning (K17)", 47);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->rtcc->GZGENCSN.LDPPAzimuth != 0.0)
		{
			skpTextFmt(skp, 1 * W / 8, 2 * H / 14, "%.3f°", GC->rtcc->GZGENCSN.LDPPAzimuth* DEG);
		}
		else
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Optimum Azimuth", 15);
		}

		skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "%.0f ft", GC->rtcc->GZGENCSN.LDPPHeightofPDI / 0.3048);

		if (GC->rtcc->GZGENCSN.LDPPPoweredDescentSimFlag)
		{
			skp->Text(1 * W / 8, 6 * H / 14, "Simulate powered descent (N/A)", 30);
		}
		else
		{
			skp->Text(1 * W / 8, 6 * H / 14, "Do not simulate powered descent", 31);
		}

		GET_Display(Buffer, GC->rtcc->GZGENCSN.LDPPTimeofPDI);
		skp->Text(1 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 8, 10 * H / 14, "%d", GC->rtcc->GZGENCSN.LDPPDwellOrbits);
		skpTextFmt(skp, 5 * W / 8, 2 * H / 14, "%.2f min", GC->rtcc->GZGENCSN.LDPPDescentFlightTime / 60.0);
		skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%.2f°", GC->rtcc->GZGENCSN.LDPPDescentFlightArc * DEG);
	}
	else if (screen == 19)
	{
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "Terrain Model", 13);

		skpTextFmt(skp, 1 * W / 8, 2 * H / 14, "%.3f°", G->TMLat* DEG);
		skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "%.3f°", G->TMLng* DEG);
		skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "%.3f°", G->TMAzi* DEG);
		skpTextFmt(skp, 1 * W / 8, 8 * H / 14, "%.1f ft", G->TMDistance / 0.3048);
		skpTextFmt(skp, 1 * W / 8, 10 * H / 14, "%.1f ft", G->TMStepSize / 0.3048);

		skp->Text(5 * W / 8, 9 * H / 14, "LS Height:", 10);
		skpTextFmt(skp, 5 * W / 8, 10 * H / 14, "%.2f NM", G->TMAlt / 1852.0);
	}
	else if (screen == 20)
	{
		if (G->vesseltype == 0)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "CSM", 3);
		}
		else if (G->vesseltype == 1)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "LM", 2);
		}
		else
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "MCC", 3);
		}

		skp->Text(1 * W / 8, 2 * H / 14, "Maneuver PAD", 12);
		skp->Text(1 * W / 8, 4 * H / 14, "Entry PAD", 9);
		skp->Text(1 * W / 8, 6 * H / 14, "Landmark Tracking", 17);
		skp->Text(1 * W / 8, 8 * H / 14, "Map Update", 10);
		skp->Text(1 * W / 8, 10 * H / 14, "Nav Check PAD", 13);

		skp->Text(5 * W / 8, 2 * H / 14, "DAP PAD", 7);
		skp->Text(5 * W / 8, 4 * H / 14, "LM Ascent PAD", 13);
		skp->Text(5 * W / 8, 6 * H / 14, "AGS SV PAD", 10);
		skp->Text(5 * W / 8, 12 * H / 14, "Previous Page", 13);
	}
	else if (screen == 21)
	{
		if (G->vesseltype == 0)
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "CSM", 3);
		}
		else
		{
			skp->Text(7 * W / 8, (int)(0.5 * H / 14), "LM", 2);
		}

		skp->Text(1 * W / 8, 2 * H / 14, "Landing Site", 12);
		skp->Text(1 * W / 8, 4 * H / 14, "REFSMMAT", 8);
		skp->Text(1 * W / 8, 6 * H / 14, "VECPOINT", 8);
		skp->Text(1 * W / 8, 8 * H / 14, "EMP", 3);
		skp->Text(1 * W / 8, 10 * H / 14, "Nodal Target Conversion", 23);
		skp->Text(1 * W / 8, 12 * H / 14, "Descent Abort", 13);

		skp->Text(5 * W / 8, 2 * H / 14, "LVDC", 4);
		skp->Text(5 * W / 8, 4 * H / 14, "Terrain Model", 13);
		skp->Text(5 * W / 8, 6 * H / 14, "AGC Ephemeris", 13);
		skp->Text(5 * W / 8, 8 * H / 14, "Lunar Impact", 12);
		skp->Text(5 * W / 8, 12 * H / 14, "Previous Page", 13);
	}
	else if (screen == 22)
	{
		const char* option = "";
		switch (GC->rtcc->PZMCCPLN.Mode)
		{
		case 1:
			option = "Option 1: Nodal Targeting";
			break;
		case 2:
			option = "Option 2: FR BAP, Fixed LPO, LS";
			break;
		case 3:
			option = "Option 3: FR BAP, Free LPO, LS";
			break;
		case 4:
			option = "Option 4: Non-FR BAP, Fixed LPO, LS";
			break;
		case 5:
			option = "Option 5: Non-FR BAP, Free LPO, LS";
			break;
		case 6:
			option = "Option 6: Circumlunar flyby, nominal";
			break;
		case 7:
			option = "Option 7: Circumlunar flyby, specified H_pc";
			break;
		case 8:
			option = "Option 8: SPS flyby to spec. FR inclination";
			break;
		default:
			option = "Option 9: Fuel critical lunar flyby";
			break;
		}

		skp->Text(1 * W / 8, 2 * H / 14, option, strlen(option));

		if (GC->MissionPlanningActive)
		{
			GET_Display(Buffer, GC->rtcc->PZMCCPLN.VectorGET);
			skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
		}

		GET_Display(Buffer, GC->rtcc->PZMCCPLN.MidcourseGET);
		skp->Text(1 * W / 8, 6 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 8, 8 * H / 14, "%d", GC->rtcc->PZMCCPLN.Column);

		if (GC->rtcc->PZMCCPLN.Config)
		{
			skp->Text(1 * W / 8, 10 * H / 14, "Docked", 6);
		}
		else
		{
			skp->Text(1 * W / 8, 10 * H / 14, "Undocked", 8);
		}

		if (GC->rtcc->PZMCCPLN.SFPBlockNum == 1)
		{
			sprintf(Buffer, "1 (Preflight)");
		}
		else
		{
			sprintf(Buffer, "2 (Nominal Targets)");
		}
		skp->Text(1 * W / 8, 12 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->PZMCCPLN.Mode == 7)
		{
			skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%.2f NM", GC->rtcc->PZMCCPLN.h_PC / 1852.0);
		}
		else if (GC->rtcc->PZMCCPLN.Mode >= 8)
		{
			if (GC->rtcc->PZMCCPLN.h_PC <= 0)
			{
				sprintf(Buffer, "Height from SFP");
			}
			else
			{
				sprintf(Buffer, "%.2f NM", GC->rtcc->PZMCCPLN.h_PC / 1852.0);
			}
			skp->Text(5 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
		}

		if (GC->rtcc->PZMCCPLN.Mode >= 8)
		{
			skpTextFmt(skp, 5 * W / 8, 6 * H / 14, "%.2f°", GC->rtcc->PZMCCPLN.incl_fr* DEG);
		}

		if (GC->rtcc->PZMCCPLN.Mode == 5)
		{
			if (GC->rtcc->PZMCCPLN.h_PC_mode5 < 0)
			{
				sprintf(Buffer, "Height from SFP");
			}
			else
			{
				sprintf(Buffer, "%.2f NM", GC->rtcc->PZMCCPLN.h_PC_mode5 / 1852.0);
			}
			skp->Text(5 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));
		}

		skp->Text(5 * W / 8, 10 * H / 14, "Constraints", 11);
	}
	else if (screen == 23)
	{
		skp->Text(4 * W / 8, 1 * H / 28, "Lunar Launch Window", 19);

		skp->Text(1 * W / 16, 2 * H / 14, "Initialization", 14);

		if (GC->rtcc->med_k15.Chaser == 1)
		{
			skp->Text(1 * W / 16, 4 * H / 14, "Chaser: CSM", 11);
		}
		else
		{
			skp->Text(1 * W / 16, 4 * H / 14, "Chaser: LM", 10);
		}

		if (GC->MissionPlanningActive)
		{
			GET_Display2(Buffer, GC->rtcc->med_k15.CSMVectorTime);
			skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));
		}

		if (GC->rtcc->med_k15.TPIDefinition == 1)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "TLO:", 4);
		}
		else
		{
			skp->Text(1 * W / 16, 8 * H / 14, "TPI:", 4);
		}
		GET_Display2(Buffer, GC->rtcc->med_k15.ThresholdTime);
		skp->Text(3 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_k15.CSI_Flag == 0.0)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "CSI at 90 degrees from insertion", 32);
		}
		else if (GC->rtcc->med_k15.CSI_Flag < 0)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "CSI at LM apolune", 17);
		}
		else
		{
			skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.1lf min", GC->rtcc->med_k15.CSI_Flag / 60.0);
		}

		if (GC->rtcc->med_k15.CDH_Flag == 0)
		{
			skp->Text(1 * W / 16, 12 * H / 14, "CDH at apsis after CSI", 22);
		}
		else
		{
			skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "CDH %d half revs after CSI", GC->rtcc->med_k15.CDH_Flag);
		}

		if (GC->MissionPlanningActive == false)
		{
			const char* target;
			if (G->target != NULL)
			{
				target = G->target->GetName();
			}
			else
			{
				target = "No Target!";
			}
			skp->Text(10 * W / 16, 2 * H / 14, target, strlen(target));
		}

		if (GC->rtcc->med_k15.TPIDefinition == 1)
		{
			skpTextFmt(skp, 8 * W / 16, 4 * H / 14, "TPI longitude: %.4lf°", GC->rtcc->med_k15.TPIValue* DEG);
		}
		else
		{
			skp->Text(8 * W / 16, 4 * H / 14, "TPI at threshold time", 22);
		}

		if (GC->rtcc->med_k15.DeltaHTFlag > 0)
		{
			skpTextFmt(skp, 8 * W / 16, 6 * H / 14, "Launch window with %d heights", GC->rtcc->med_k15.DeltaHTFlag);
		}
		else
		{
			skp->Text(8 * W / 16, 6 * H / 14, "Calc using input heights", 24);
			skpTextFmt(skp, 8 * W / 16, 8 * H / 14, "%.2lf %.2lf %.2lf NM", GC->rtcc->med_k15.DH1 / 1852.0, GC->rtcc->med_k15.DH2 / 1852.0, GC->rtcc->med_k15.DH3 / 1852.0);
		}

		/*if (G->LunarLiftoffTimeOption == 0)
		{
			if (G->LunarLiftoffInsVelInput)
			{
				skp->Text((int)(0.5 * W / 8), 4 * H / 14, "Input Ins. Velocity", 19);
			}
			else
			{
				skp->Text((int)(0.5 * W / 8), 4 * H / 14, "Calc. Ins. Velocity", 19);
			}
		}

		skp->Text((int)(0.5 * W / 8), 8 * H / 21, "Rendezvous Schedule:", 20);

		skp->Text((int)(0.5 * W / 8), 9 * H / 21, "Launch:", 7);
		GET_Display(Buffer, G->LunarLiftoffRes.t_L);
		skp->Text(2 * W / 8, 9 * H / 21, Buffer, strlen(Buffer));

		skp->Text((int)(0.5 * W / 8), 10 * H / 21, "Insertion:", 10);
		GET_Display(Buffer, G->LunarLiftoffRes.t_Ins);
		skp->Text(2 * W / 8, 10 * H / 21, Buffer, strlen(Buffer));

		if (G->LunarLiftoffTimeOption == 0)
		{
			skp->Text((int)(0.5 * W / 8), 11 * H / 21, "T CSI:", 6);
			GET_Display(Buffer, G->LunarLiftoffRes.t_CSI);
			skp->Text(2 * W / 8, 11 * H / 21, Buffer, strlen(Buffer));

			skp->Text((int)(0.5 * W / 8), 12 * H / 21, "DV CSI:", 7);
			sprintf(Buffer, "%.1f ft/s", G->LunarLiftoffRes.DV_CSI / 0.3048);
			skp->Text(2 * W / 8, 12 * H / 21, Buffer, strlen(Buffer));

			skp->Text((int)(0.5 * W / 8), 13 * H / 21, "T CDH:", 6);
			GET_Display(Buffer, G->LunarLiftoffRes.t_CDH);
			skp->Text(2 * W / 8, 13 * H / 21, Buffer, strlen(Buffer));

			skp->Text((int)(0.5 * W / 8), 14 * H / 21, "DV CDH:", 7);
			sprintf(Buffer, "%.1f ft/s", G->LunarLiftoffRes.DV_CDH / 0.3048);
			skp->Text(2 * W / 8, 14 * H / 21, Buffer, strlen(Buffer));
		}

		if (G->LunarLiftoffTimeOption == 0 || G->LunarLiftoffTimeOption == 1)
		{
			skp->Text((int)(0.5 * W / 8), 15 * H / 21, "T TPI:", 6);
			GET_Display(Buffer, G->LunarLiftoffRes.t_TPI);
			skp->Text(2 * W / 8, 15 * H / 21, Buffer, strlen(Buffer));

			skp->Text((int)(0.5 * W / 8), 16 * H / 21, "DV TPI:", 7);
			sprintf(Buffer, "%.1f ft/s", G->LunarLiftoffRes.DV_TPI / 0.3048);
			skp->Text(2 * W / 8, 16 * H / 21, Buffer, strlen(Buffer));
		}

		skp->Text((int)(0.5 * W / 8), 17 * H / 21, "T TPF:", 6);
		GET_Display(Buffer, G->LunarLiftoffRes.t_TPF);
		skp->Text(2 * W / 8, 17 * H / 21, Buffer, strlen(Buffer));

		skp->Text((int)(0.5 * W / 8), 18 * H / 21, "DV TPF:", 7);
		sprintf(Buffer, "%.1f ft/s", G->LunarLiftoffRes.DV_TPF / 0.3048);
		skp->Text(2 * W / 8, 18 * H / 21, Buffer, strlen(Buffer));

		skp->Text((int)(0.5 * W / 8), 19 * H / 21, "DVT:", 4);
		sprintf(Buffer, "%.1f ft/s", G->LunarLiftoffRes.DV_T / 0.3048);
		skp->Text(2 * W / 8, 19 * H / 21, Buffer, strlen(Buffer));

		if (G->LunarLiftoffTimeOption == 0)
		{
			skp->Text(4 * W / 8, 2 * H / 14, "Concentric Profile", 18);
		}
		else if (G->LunarLiftoffTimeOption == 1)
		{
			skp->Text(4 * W / 8, 2 * H / 14, "Direct Profile", 14);
		}
		else
		{
			skp->Text(4 * W / 8, 2 * H / 14, "Time Critical Profile", 21);
		}

		if (GC->MissionPlanningActive)
		{

		}
		else
		{
			if (G->target != NULL)
			{
				sprintf(Buffer, G->target->GetName());
				skp->Text(5 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
			}
		}
		if (G->LunarLiftoffTimeOption == 1)
		{
			skp->Text(5 * W / 8, 6 * H / 14, "DT Insertion-TPI:", 17);
			sprintf(Buffer, "%.1f min", GC->DT_Ins_TPI / 60.0);
			skp->Text(5 * W / 8, 7 * H / 14, Buffer, strlen(Buffer));
		}

		skp->Text(5 * W / 8, 9 * H / 14, "Horizontal Velocity:", 20);
		sprintf(Buffer, "%+.1f ft/s", G->LunarLiftoffRes.v_LH / 0.3048);
		skp->Text(5 * W / 8, 10 * H / 14, Buffer, strlen(Buffer));

		skp->Text(5 * W / 8, 11 * H / 14, "Vertical Velocity:", 18);
		sprintf(Buffer, "%+.1f ft/s", G->LunarLiftoffRes.v_LV / 0.3048);
		skp->Text(5 * W / 8, 12 * H / 14, Buffer, strlen(Buffer));*/
	}
	else if (screen == 24)
	{
		skp->Text(4 * W / 8, (int)(0.5 * H / 14), "Erasable Memory Programs", 24);

		skp->Text(1 * W / 8, 2 * H / 14, "Program 99", 10);

		skpTextFmt(skp, 5 * W / 8, 2 * H / 14, "Uplink No. %d", G->EMPUplinkNumber);

	}
	else if (screen == 25)
	{
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "Nav Check PAD", 13);

		GET_Display(Buffer, G->navcheckpad.NavChk[0]);
		skp->Text(1 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 4 * W / 8, 6 * H / 14, "%+07.2f LAT", G->navcheckpad.lat[0]);
		skpTextFmt(skp, 4 * W / 8, 7 * H / 14, "%+07.2f LNG", G->navcheckpad.lng[0]);
		skpTextFmt(skp, 4 * W / 8, 8 * H / 14, "%+07.1f ALT", G->navcheckpad.alt[0]);
	}
	else if (screen == 26)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Deorbit", 7);

		if (GC->rtcc->RZJCTTC.R32_Code == 1)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Type 1 (No Sep/Shaping)", 23);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Type 2 (With Sep/Shaping)", 25);
		}

		GET_Display(Buffer, GC->rtcc->RZJCTTC.R32_GETI);
		skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->RZJCTTC.R32_lat_T <= -720.0*RAD)
		{
			sprintf(Buffer, "No latitude iteration");
		}
		else
		{
			sprintf(Buffer, "%f °", GC->rtcc->RZJCTTC.R32_lat_T*DEG);
		}
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%f °", GC->rtcc->RZJCTTC.R32_lng_T* DEG);

		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.2lf NM", GC->rtcc->RZJCTTC.R32_MD);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(15 * W / 16, 2 * H / 14, "Retrofire Digitals", 18);
		skp->Text(15 * W / 16, 4 * H / 14, "Retrofire External DV", 21);
		skp->Text(15 * W / 16, 6 * H / 14, "Retrofire Separation", 20);
	}
	else if (screen == 27)
	{
		skp->Text(3 * W / 8, 1 * H / 14, "Return to Earth Digitals Inputs", 31);

		if (GC->rtcc->med_f80.Column == 1)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Primary", 7);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Manual", 6);
		}

		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%d", GC->rtcc->med_f80.ASTCode);

		Text(skp, 1 * W / 16, 6 * H / 14, GC->rtcc->med_f80.REFSMMAT);

		Text(skp, 1 * W / 16, 8 * H / 14, GC->rtcc->med_f80.ManeuverCode);

		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%d quads %.1lf seconds", GC->rtcc->med_f80.NumQuads, GC->rtcc->med_f80.UllageDT);

		if (GC->rtcc->med_f80.TrimAngleInd == -1)
		{
			skp->Text(1 * W / 16, 12 * H / 14, "Compute trim gimbals", 20);
		}
		else
		{
			skp->Text(1 * W / 16, 12 * H / 14, "Use system parameter", 20);
		}

		skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%.2lf°", GC->rtcc->med_f80.DockingAngle* DEG);

		if (GC->rtcc->med_f80.HeadsUp)
		{
			skp->Text(10 * W / 16, 6 * H / 14, "Heads Up", 8);
		}
		else
		{
			skp->Text(10 * W / 16, 6 * H / 14, "Heads Down", 10);
		}

		if (GC->rtcc->med_f80.Iterate)
		{
			skp->Text(10 * W / 16, 8 * H / 14, "Iterate", 7);
		}
		else
		{
			skp->Text(10 * W / 16, 8 * H / 14, "Don't iterate", 13);
		}
	}
	else if (screen == 28)
	{
		skp->Text(2 * W / 8, 2 * H / 32, "RETURN TO EARTH DIGITALS (MSK 363)", 34);

		skp->SetFont(font2);
		skp->SetPen(pen2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);

		skp->Text(1 * W / 32, 5 * H / 32, "GETR", 4);
		skp->Text(12 * W / 32, 5 * H / 32, "CM WT", 5);
		skp->Text(20 * W / 32, 5 * H / 32, "K FAC", 5);
		skp->Text(26 * W / 32, 5 * H / 32, "STAID", 5);

		skp->Text(9 * W / 32, 6 * H / 32, "PRIMARY", 7);
		skp->Text(21 * W / 32, 6 * H / 32, "MANUAL", 6);
		skp->Text(15 * W / 32, 6 * H / 32, "CODE", 4);
		skp->Text(27 * W / 32, 6 * H / 32, "CODE", 4);

		skp->Line(8 * W / 32, 6 * H / 32, 8 * W / 32, H);
		skp->Line(20 * W / 32, 6 * H / 32, 20 * W / 32, H);

		skp->Text(1 * W / 32, 7 * H / 32, "STA ID  AM", 10);
		skp->Text(2 * W / 32, 8 * H / 32, "GETV", 4);
		skp->Text(2 * W / 32, 9 * H / 32, "AREA THR", 8);
		skp->Text(2 * W / 32, 10 * H / 32, "MATRIX WT", 9);
		skp->Text(2 * W / 32, 11 * H / 32, "TAA EP", 6);
		skp->Text(1 * W / 32, 12 * H / 32, "RLH PLH YLH", 11);
		skp->Text(1 * W / 32, 13 * H / 32, "RO PI YM", 8);
		skp->Text(2 * W / 32, 14 * H / 32, "VC BT", 5);
		skp->Text(2 * W / 32, 15 * H / 32, "VT U DT", 7);
		skp->Text(2 * W / 32, 16 * H / 32, "PETI", 4);
		skp->Text(2 * W / 32, 17 * H / 32, "GETI", 4);
		skp->Text(2 * W / 32, 18 * H / 32, "GMTI", 4);
		skp->Text(1 * W / 32, 19 * H / 32, "BU PETIR LV", 11);
		skp->Text(2 * W / 32, 21 * H / 32, "GIR/GCON", 8);
		skp->Text(2 * W / 32, 22 * H / 32, "GMAX", 4);
		skp->Text(2 * W / 32, 23 * H / 32, "PETEI", 5);
		skp->Text(1 * W / 32, 24 * H / 32, "VEI GEI", 7);
		skp->Text(1 * W / 32, 25 * H / 32, "LAT LNG EI", 10);
		skp->Text(1 * W / 32, 26 * H / 32, "LAT LNG ML2", 11);
		skp->Text(1 * W / 32, 27 * H / 32, "LAT LNG T", 9);
		skp->Text(1 * W / 32, 28 * H / 32, "LAT LNG ZL2", 11);
		skp->Text(1 * W / 32, 29 * H / 32, "LAT LNG IPB", 11);
		skp->Text(1 * W / 32, 30 * H / 32, "GETL", 4);
		skp->Text(5 * W / 32, 30 * H / 32, "MD", 2);

		GET_Display(Buffer, GC->rtcc->SystemParameters.MCGREF*3600.0, false);
		skp->Text(4 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));

		RTEDigitalSolutionTable *tab;
		int hh, mm;
		double secs;
		for (int i = 0;i < 2;i++)
		{
			tab = &GC->rtcc->PZREAP.RTEDTable[i];

			if (tab->Error)
			{
				skpTextFmt(skp, (11 + 12 * i) * W / 32, 31 * H / 32, "Error: %d", tab->Error);
			}

			if (tab->RTEDCode == "") continue;

			Text(skp, (18 + 12 * i) * W / 32, 6 * H / 32, tab->RTEDCode);
			Text(skp, (11 + 12 * i) * W / 32, 7 * H / 32, tab->StationID);
			Text(skp, (16 + 12 * i) * W / 32, 7 * H / 32, tab->ASTSolutionCode);

			GET_Display(Buffer, tab->VectorGET, false);
			skp->Text((12 + 12 * i) * W / 32, 8 * H / 32, Buffer, strlen(Buffer));

			Text(skp, (12 + 12 * i) * W / 32, 9 * H / 32, tab->LandingSiteID);
			Text(skp, (15 + 12 * i) * W / 32, 9 * H / 32, tab->ManeuverCode);

			Text(skp, (11 + 12 * i) * W / 32, 10 * H / 32, tab->SpecifiedREFSMMAT);
			skpTextFmt(skp, (15 + 12 * i) * W / 32, 10 * H / 32, "%.0lf", tab->VehicleWeight * LBS * 1000.0);

			skpTextFmt(skp, (13 + 12 * i) * W / 32, 11 * H / 32, "%.0lf", tab->TrueAnomaly * DEG);
			Text(skp, (15 + 12 * i) * W / 32, 11 * H / 32, tab->PrimaryReentryMode);

			skp->SetTextAlign(oapi::Sketchpad::RIGHT, oapi::Sketchpad::BASELINE);

			skpTextFmt(skp, (12 + 12 * i) * W / 32, 12 * H / 32, "%.1lf", tab->LVLHAtt.x * DEG);
			skpTextFmt(skp, (15 + 12 * i) * W / 32, 12 * H / 32, "%.1lf", tab->LVLHAtt.y * DEG);
			skpTextFmt(skp, (18 + 12 * i) * W / 32, 12 * H / 32, "%.1lf", tab->LVLHAtt.z * DEG);

			FormatIMUAngle1(Buffer, tab->IMUAtt.x);
			skp->Text((12 + 12 * i) * W / 32, 13 * H / 32, Buffer, strlen(Buffer));
			FormatIMUAngle1(Buffer, tab->IMUAtt.y);
			skp->Text((15 + 12 * i) * W / 32, 13 * H / 32, Buffer, strlen(Buffer));
			FormatIMUAngle1(Buffer, tab->IMUAtt.z);
			skp->Text((18 + 12 * i) * W / 32, 13 * H / 32, Buffer, strlen(Buffer));

			skpTextFmt(skp, (13 + 12 * i) * W / 32, 14 * H / 32, "%.1lf", tab->DVC / 0.3048);
			SStoHHMMSS(tab->dt, hh, mm, secs);
			skpTextFmt(skp, (18 + 12 * i) * W / 32, 14 * H / 32, "%02d:%02.1lf", mm, secs);

			skpTextFmt(skp, (13 + 12 * i) * W / 32, 15 * H / 32, "%.1lf", tab->dv / 0.3048);
			SStoHHMMSS(tab->dt_ullage, hh, mm, secs);
			skpTextFmt(skp, (18 + 12 * i) * W / 32, 15 * H / 32, "%+d %02d:%02.1lf", tab->NumQuads, mm, secs);

			skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);

			GET_Display(Buffer, tab->PETI, false);
			skp->Text((12 + 12 * i) * W / 32, 16 * H / 32, Buffer, strlen(Buffer));
			GET_Display2(Buffer, tab->GETI);
			skp->Text((12 + 12 * i) * W / 32, 17 * H / 32, Buffer, strlen(Buffer));
			GET_Display2(Buffer, tab->GMTI);
			skp->Text((12 + 12 * i) * W / 32, 18 * H / 32, Buffer, strlen(Buffer));

			Text(skp, (9 + 12 * i) * W / 32, 19 * H / 32, tab->BackupReentryMode);
			GET_Display(Buffer, tab->RollPET, false);
			skp->Text((12 + 12 * i) * W / 32, 19 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, (17 + 12 * i) * W / 32, 19 * H / 32, "%.1lf", tab->LiftVectorOrientation * DEG);

			skpTextFmt(skp, (13 + 12 * i) * W / 32, 21 * H / 32, "%.2lf", tab->GLevelRoll);
			skpTextFmt(skp, (13 + 12 * i) * W / 32, 22 * H / 32, "%.2lf", tab->MaxGLevelPrimary);
			GET_Display(Buffer, tab->ReentryPET, false);
			skp->Text((12 + 12 * i) * W / 32, 23 * H / 32, Buffer, strlen(Buffer));

			skpTextFmt(skp, (12 + 12 * i) * W / 32, 24 * H / 32, "%.0lf", tab->v_EI / 0.3048);
			skpTextFmt(skp, (16 + 12 * i) * W / 32, 24 * H / 32, "%.2lf", tab->gamma_EI * DEG);

			FormatLatitude(Buffer, tab->lat_EI*DEG);
			skp->Text((10 + 12 * i) * W / 32, 25 * H / 32, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_EI*DEG);
			skp->Text((15 + 12 * i) * W / 32, 25 * H / 32, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_imp_2nd_max*DEG);
			skp->Text((10 + 12 * i) * W / 32, 26 * H / 32, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_imp_2nd_max*DEG);
			skp->Text((15 + 12 * i) * W / 32, 26 * H / 32, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_imp_tgt*DEG);
			skp->Text((10 + 12 * i) * W / 32, 27 * H / 32, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_imp_tgt*DEG);
			skp->Text((15 + 12 * i) * W / 32, 27 * H / 32, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_imp_2nd_min*DEG);
			skp->Text((10 + 12 * i) * W / 32, 28 * H / 32, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_imp_2nd_min*DEG);
			skp->Text((15 + 12 * i) * W / 32, 28 * H / 32, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_imp_bu*DEG);
			skp->Text((10 + 12 * i) * W / 32, 29 * H / 32, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_imp_bu*DEG);
			skp->Text((15 + 12 * i) * W / 32, 29 * H / 32, Buffer, strlen(Buffer));

			GET_Display(Buffer, tab->ImpactGET_prim, false);
			skp->Text((9 + 12 * i) * W / 32, 30 * H / 32, Buffer, strlen(Buffer));
			if (tab->md_lat > 0)
			{
				sprintf_s(Buffer, "%.0lfN", tab->md_lat / 1852.0);
			}
			else
			{
				sprintf_s(Buffer, "%.0lfS", abs(tab->md_lat / 1852.0));
			}
			skp->Text((15 + 12 * i) * W / 32, 30 * H / 32, Buffer, strlen(Buffer));
			if (tab->md_lng > 0)
			{
				sprintf_s(Buffer, "%.0lfE", tab->md_lng / 1852.0);
			}
			else
			{
				sprintf_s(Buffer, "%.0lfW", abs(tab->md_lng / 1852.0));
			}
			skp->Text((17 + 12 * i) * W / 32, 30 * H / 32, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 29)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "RETURN TO EARTH TARGET (MSK 366)", 32);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->SetFont(font2);

		skp->Text(2 * W / 32, 4 * H / 28, "CONSTRAINTS", 11);
		skp->Text(2 * W / 32, 6 * H / 28, "DVMAX", 5);
		skp->Text(2 * W / 32, 8 * H / 28, "TZMIN", 5);
		skp->Text(2 * W / 32, 10 * H / 28, "TZMAX", 5);
		skp->Text(2 * W / 32, 12 * H / 28, "GMAX", 4);
		skp->Text(2 * W / 32, 14 * H / 28, "HMINMC", 6);
		skp->Text(2 * W / 32, 16 * H / 28, "IRMAX", 5);
		skp->Text(2 * W / 32, 18 * H / 28, "RRBIAS", 6);
		skp->Text(2 * W / 32, 20 * H / 28, "VRMAX", 5);
		skp->Text(2 * W / 32, 22 * H / 28, "MOTION", 6);
		skp->Text(2 * W / 32, 24 * H / 28, "TGTLN", 5);
		skp->Text(2 * W / 32, 26 * H / 28, "VECID", 5);

		skp->Text(20 * W / 32, 4 * H / 28, "ATP", 5);
		skp->Text(20 * W / 32, 18 * H / 28, "PTP", 5);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skpTextFmt(skp, 10 * W / 32, 6 * H / 28, "%.0f", GC->rtcc->PZREAP.DVMAX);
		GET_Display(Buffer, GC->rtcc->PZREAP.TZMIN*3600.0, false);
		skp->Text(10 * W / 32, 8 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->PZREAP.TZMAX*3600.0, false);
		skp->Text(10 * W / 32, 10 * H / 28, Buffer, strlen(Buffer));
		
		skpTextFmt(skp, 10 * W / 32, 12 * H / 28, "%.1f", GC->rtcc->PZREAP.GMAX);
		skpTextFmt(skp, 10 * W / 32, 14 * H / 28, "%.1f", GC->rtcc->PZREAP.HMINMC);
		skpTextFmt(skp, 10 * W / 32, 16 * H / 28, "%.2f", GC->rtcc->PZREAP.IRMAX);
		skpTextFmt(skp, 10 * W / 32, 18 * H / 28, "%.0f", GC->rtcc->PZREAP.RRBIAS);
		skpTextFmt(skp, 10 * W / 32, 20 * H / 28, "%.0f", GC->rtcc->PZREAP.VRMAX);

		if (GC->rtcc->PZREAP.MOTION == 0)
		{
			skp->Text(10 * W / 32, 22 * H / 28, "EITHER", 6);
		}
		else if (GC->rtcc->PZREAP.MOTION == 1)
		{
			skp->Text(10 * W / 32, 22 * H / 28, "DIRECT", 6);
		}
		else
		{
			skp->Text(10 * W / 32, 22 * H / 28, "CIRCUM", 6);
		}

		if (GC->rtcc->PZREAP.TGTLN == 0)
		{
			skp->Text(10 * W / 32, 24 * H / 28, "SHALLOW", 7);
		}
		else
		{
			skp->Text(10 * W / 32, 24 * H / 28, "STEEP", 5);
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		for (unsigned i = 0;i < 5;i++)
		{
			//If first element is not valid, skip this ATP
			if (GC->rtcc->PZREAP.ATPCoordinates[i][0] >= 1e9)
			{
				continue;
			}

			Text(skp, (11 + i * 4) * W / 32, 6 * H / 28, GC->rtcc->PZREAP.ATPSite[i]);
			for (unsigned j = 0;j < 5;j++)
			{
				//If current element isn't valid, skip the rest
				if (GC->rtcc->PZREAP.ATPCoordinates[i][2 * j] >= 1e9)
				{
					break;
				}

				skpTextFmt(skp, (11 + i * 4) * W / 32, (7 + j * 2) * H / 28, "%.2f", GC->rtcc->PZREAP.ATPCoordinates[i][2 * j] * DEG);
				skpTextFmt(skp, (11 + i * 4) * W / 32, (8 + j * 2) * H / 28, "%.2f", GC->rtcc->PZREAP.ATPCoordinates[i][2 * j + 1] * DEG);
			}
		}

		for (unsigned i = 0;i < 5;i++)
		{
			//If name is not valid, skip this PTP
			if (GC->rtcc->PZREAP.PTPSite[i] == "")
			{
				continue;
			}

			Text(skp, (11 + i * 4) * W / 32, 20 * H / 28, GC->rtcc->PZREAP.PTPSite[i]);
			skpTextFmt(skp, (11 + i * 4) * W / 32, 21 * H / 28, "%.2f", GC->rtcc->PZREAP.PTPLatitude[i] * DEG);
			skpTextFmt(skp, (11 + i * 4) * W / 32, 22 * H / 28, "%.2f", GC->rtcc->PZREAP.PTPLongitude[i] * DEG);
		}
	}
	else if (screen == 30)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "Entry Update", 12);

		skpTextFmt(skp, 1 * W / 8, 2 * H / 14, "Desired Range: %.1f NM", G->entryrange);
		skpTextFmt(skp, 5 * W / 8, 5 * H / 14, "Lat:  %.2f °", G->EntryLatcor * DEG);
		skpTextFmt(skp, 5 * W / 8, 6 * H / 14, "Long: %.2f °", G->EntryLngcor * DEG);
	}
	else if (screen == 31)
	{
		//Spare
	}
	else if (screen == 32)
	{
		skp->Text(1 * W / 8, 2 * H / 14, "Two Impulse Processor", 21);
		skp->Text(1 * W / 8, 4 * H / 14, "Coelliptic Sequence Processor", 29);
		skp->Text(1 * W / 8, 6 * H / 14, "Docking Initiation Processor", 28);
		skp->Text(1 * W / 8, 8 * H / 14, "Skylab Rendezvous", 17);
		skp->Text(1 * W / 8, 10 * H / 14, "TPI Times", 9);
	}
	else if (screen == 33)
	{
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "Docking Initiate", 16);

		skp->Text(1 * W / 16, 2 * H / 14, "TIG and TPI Definition", 22);
		skp->Text(1 * W / 16, 4 * H / 14, "Init Parameters", 15);

		if (GC->rtcc->med_k00.NC1 > 0)
		{
			sprintf(Buffer, "%.2f", GC->rtcc->med_k00.NC1);
		}
		else
		{
			sprintf(Buffer, "No NC1 maneuver");
		}
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2f", GC->rtcc->med_k00.NH);

		if (GC->rtcc->med_k00.I4)
		{
			sprintf(Buffer, "NCC: %.2f", GC->rtcc->med_k00.NCC);
		}
		else
		{
			sprintf(Buffer, "%.2f", GC->rtcc->med_k00.NSR);
		}		
		skp->Text(1 * W / 16, 10 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.0f", GC->rtcc->med_k00.MI);

		if (G->target != NULL)
		{
			sprintf(Buffer, G->target->GetName());
			skp->Text(10 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(10 * W / 16, 4 * H / 14, "No Target!", 10);
		}

		if (GC->rtcc->med_k00.NPC > 0)
		{
			skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%.2f", GC->rtcc->med_k00.NPC);
		}
		else
		{
			skp->Text(10 * W / 16, 6 * H / 14, "No NPC maneuver", 15);
		}

	}
	else if (screen == 34)
	{
		skp->Text(3 * W / 8, (int)(0.5 * H / 14), "DKI TIG and TPI Definition", 26);

		if (GC->rtcc->med_k00.ChaserVehicle == RTCC_MPT_CSM)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "CSM", 3);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "LM", 2);
		}

		if (GC->rtcc->med_k00.I4)
		{
			skp->Text(1 * W / 16, 4 * H / 14, "Skylab Rendezvous", 17);
		}
		else
		{
			skp->Text(1 * W / 16, 4 * H / 14, "Regular DKI", 11);
		}

		if (GC->rtcc->med_k10.MLDOption == 1)
		{
			skp->Text(1 * W / 16, 6 * H / 14, "ML at input time", 16);

			GET_Display(Buffer, GC->rtcc->med_k10.MLDTime, false);
			skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			if (GC->rtcc->med_k10.MLDOption == 2)
			{
				skp->Text(1 * W / 16, 6 * H / 14, "ML at chaser apoapsis", 21);
			}
			else
			{
				skp->Text(1 * W / 16, 6 * H / 14, "ML at target apogee", 19);
			}
			skp->Text(1 * W / 16, 7 * H / 14, "Threshold time:", 15);
			if (GC->rtcc->med_k10.MLDTime == 0.0)
			{
				sprintf(Buffer, "Current Time");
			}
			else
			{
				GET_Display(Buffer, GC->rtcc->med_k10.MLDTime, false);
			}
			skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));
		}

		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.1lf", GC->rtcc->med_k10.MLDValue);
		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%d", GC->rtcc->GZGENCSN.DKIPhaseAngleSetting);

		if (GC->rtcc->GZGENCSN.DKI_TP_Definition == 0)
		{
			sprintf(Buffer, "Input TPI phase angle");
			skp->Text(10 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

			skpTextFmt(skp, 10 * W / 16, 2 * H / 14, "%.1lf", GC->rtcc->GZGENCSN.DKI_TPDefinitionValue);
		}
		else
		{
			switch (GC->rtcc->GZGENCSN.DKI_TP_Definition)
			{
			case 1:
				sprintf(Buffer, "Input TPI time");
				break;
			case 2:
				sprintf(Buffer, "Input TPF time");
				break;
			case 3:
				sprintf(Buffer, "TPI at X minutes into night");
				break;
			case 4:
				sprintf(Buffer, "TPI at X minutes into day");
				break;
			case 5:
				sprintf(Buffer, "TPF at X minutes into night");
				break;
			case 6:
				sprintf(Buffer, "TPF at X minutes into day");
				break;
			default:
				sprintf(Buffer, "");
				break;
			}
			skp->Text(8 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

			if (GC->rtcc->GZGENCSN.DKI_TP_Definition == 1 || GC->rtcc->GZGENCSN.DKI_TP_Definition == 2)
			{
				GET_Display(Buffer, GC->rtcc->GZGENCSN.DKI_TPDefinitionValue, false);
				skp->Text(10 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
			}
			else
			{
				skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%.0lf min", GC->rtcc->GZGENCSN.DKI_TPDefinitionValue);
			}
		}
	}
	else if (screen == 35)
	{
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "DAP PAD", 7);

		skpTextFmt(skp, (int)(3.5 * W / 8), 5 * H / 21, "%+06.0f WT N47", G->DAP_PAD.ThisVehicleWeight);
		skpTextFmt(skp, (int)(3.5 * W / 8), 6 * H / 21, "%+06.0f", G->DAP_PAD.OtherVehicleWeight);

		skpTextFmt(skp, (int)(3.5 * W / 8), 7 * H / 21, "%+07.2f GMBL N48", G->DAP_PAD.PitchTrim);
		skpTextFmt(skp, (int)(3.5 * W / 8), 8 * H / 21, "%+07.2f", G->DAP_PAD.YawTrim);
	}
	else if (screen == 36)
	{
		skp->Text(6 * W / 8, (int)(0.5 * H / 14), "LVDC", 4);

		skpTextFmt(skp, 1 * W / 8, 2 * H / 14, "Launch Azimuth: %.4f°", G->LVDCLaunchAzimuth* DEG);
	}
	else if (screen == 37)
	{
		if (G->AGCEphemOption == 0)
		{
			skp->Text(4 * W / 8, (int)(0.5 * H / 14), "AGC Ephemeris Generator", 23);

			skp->Text(1 * W / 8, 2 * H / 14, "Epoch of BRCS:", 14);
			skp->Text(1 * W / 8, 4 * H / 14, "TEphemZero:", 11);
			skp->Text(1 * W / 8, 8 * H / 14, "TIMEM0:", 7);

			skpTextFmt(skp, 4 * W / 8, 2 * H / 14, "%d", G->AGCEphemBRCSEpoch);
			skpTextFmt(skp, 4 * W / 8, 4 * H / 14, "%f", G->AGCEphemTEphemZero);
			skpTextFmt(skp, 4 * W / 8, 8 * H / 14, "%f", G->AGCEphemTIMEM0);
		}
		else
		{
			skp->Text(4 * W / 8, (int)(0.5 * H / 14), "AGC Correction Vectors", 23);

			skp->Text(1 * W / 8, 6 * H / 14, "TEPHEM:", 7);
			skpTextFmt(skp, 4 * W / 8, 6 * H / 14, "%.4f", G->AGCEphemTEPHEM);

			skp->Text(1 * W / 8, 10 * H / 14, "TLAND:", 6);
			skpTextFmt(skp, 4 * W / 8, 10 * H / 14, "%+.2f Days", G->AGCEphemTLAND);

			skp->Text(1 * W / 8, 12 * H / 14, "Mission:", 8);
			skpTextFmt(skp, 4 * W / 8, 12 * H / 14, "%d", G->AGCEphemMission);

			if (G->AGCEphemIsCMC)
			{
				skp->Text(7 * W / 8, 6 * H / 14, "CMC", 3);
			}
			else
			{
				skp->Text(7 * W / 8, 6 * H / 14, "LGC", 3);
			}
		}
	}
	else if (screen == 38)
	{
		skp->Text(4 * W / 8, (int)(0.5 * H / 14), "Lunar Ascent Processor", 22);

		GET_Display2(Buffer, G->t_LunarLiftoff);
		skp->Text(1 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "%+07.1f ft/s", GC->rtcc->PZLTRT.InsertionHorizontalVelocity / 0.3048);
		skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "%+07.1f ft/s", GC->rtcc->PZLTRT.InsertionRadialVelocity / 0.3048);

		skp->Text(1 * W / 8, 8 * H / 14, "Powered Flight Arc:", 19);
		skpTextFmt(skp, 1 * W / 8, 9 * H / 14, "%.3f°", GC->rtcc->PZLTRT.PoweredFlightArc* DEG);
		skp->Text(1 * W / 8, 10 * H / 14, "Powered Flight Time:", 20);
		skpTextFmt(skp, 1 * W / 8, 11 * H / 14, "%.1f s", GC->rtcc->PZLTRT.PoweredFlightTime);
		skp->Text(1 * W / 8, 12 * H / 14, "Insertion GET:", 14);
		double get = GC->rtcc->GETfromGMT(GC->rtcc->JZLAI.sv_Insertion.GMT);
		if (get < 0)
		{
			get = 0.0;
		}
		GET_Display(Buffer, get, false);
		skp->Text(1 * W / 8, 13 * H / 14, Buffer, strlen(Buffer));

		if (!GC->MissionPlanningActive)
		{
			if (G->target != NULL)
			{
				sprintf(Buffer, G->target->GetName());
				skp->Text(5 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
			}
			else
			{
				skp->Text(5 * W / 8, 2 * H / 14, "No Target!", 10);
			}
		}

		skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.R.x);
		skpTextFmt(skp, 5 * W / 8, 5 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.R.y);
		skpTextFmt(skp, 5 * W / 8, 6 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.R.z);

		skpTextFmt(skp, 5 * W / 8, 8 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.V.x);
		skpTextFmt(skp, 5 * W / 8, 9 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.V.y);
		skpTextFmt(skp, 5 * W / 8, 10 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.V.z);

		skpTextFmt(skp, 5 * W / 8, 12 * H / 14, "%f", GC->rtcc->JZLAI.sv_Insertion.GMT);
	}
	else if (screen == 39)
	{
		skp->Text(4 * W / 8, (int)(0.5 * H / 14), "LM Ascent PAD", 13);

		if (G->target != NULL)
		{
			sprintf(Buffer, G->target->GetName());
		}
		else
		{
			sprintf(Buffer, "No Target!");
		}
		skp->Text((int)(5.5 * W / 8), 4 * H / 14, Buffer, strlen(Buffer));

		int hh, mm;
		double secs;

		SStoHHMMSS(G->t_LunarLiftoff, hh, mm, secs);
		skpTextFmt(skp, 2 * W / 8, 6 * H / 21, "%+06d HRS", hh);
		skpTextFmt(skp, 2 * W / 8, 7 * H / 21, "%+06d MIN TIG", mm);
		skpTextFmt(skp, 2 * W / 8, 8 * H / 21, "%+06.0f SEC", secs * 100.0);

		skpTextFmt(skp, 2 * W / 8, 9 * H / 21, "%+07.1f V (HOR)", GC->rtcc->PZLTRT.InsertionHorizontalVelocity / 0.3048);
		skpTextFmt(skp, 2 * W / 8, 10 * H / 21, "%+07.1f V (VERT) N76", GC->rtcc->PZLTRT.InsertionRadialVelocity / 0.3048);
		skpTextFmt(skp, 2 * W / 8, 11 * H / 21, "%+07.1f CROSSRANGE", G->lmascentpad.CR);
		skpTextFmt(skp, 2 * W / 8, 12 * H / 21, "%+06d DEDA 047", G->lmascentpad.DEDA047);
		skpTextFmt(skp, 2 * W / 8, 13 * H / 21, "%+06d DEDA 053", G->lmascentpad.DEDA053);
		if (GC->mission >= 14)
		{
			sprintf(Buffer, "%+06.0f DEDA 224/226", G->lmascentpad.DEDA225_226);
		}
		else
		{
			sprintf(Buffer, "%+06.0f DEDA 225/226", G->lmascentpad.DEDA225_226);
		}
		skp->Text(2 * W / 8, 14 * H / 21, Buffer, strlen(Buffer));
		skpTextFmt(skp, 2 * W / 8, 15 * H / 21, "%+06.0f DEDA 231", G->lmascentpad.DEDA231);

	}
	else if (screen == 40)
	{
		skp->Text(4 * W / 8, (int)(0.5 * H / 14), "Descent Abort", 13);

		if (IsBusy(G->subThreadStatus))
		{
			skp->Text(5 * W / 8, 3 * H / 14, "Calculating...", 14);
		}
		else if (!G->PADSolGood)
		{
			skp->Text(5 * W / 8, 3 * H / 14, "Calculation failed!", 19);
		}

		if (G->PDAPEngine == 0)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "DPS/APS", 7);
		}
		else
		{
			skp->Text(1 * W / 8, 2 * H / 14, "APS", 7);
		}

		skp->Text(4 * W / 8, 3 * H / 21, "TPI:", 4);
		GET_Display(Buffer, G->t_TPI);
		skp->Text(5 * W / 8, 3 * H / 21, Buffer, strlen(Buffer));

		skp->Text(1 * W / 8, 5 * H / 21, "PGNS Coefficients:", 18);
		if (GC->mission <= 11)
		{
			skpTextFmt(skp, 1 * W / 8, 6 * H / 21, "%e", G->PDAPABTCOF[0] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 7 * H / 21, "%e", G->PDAPABTCOF[1] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 8 * H / 21, "%e", G->PDAPABTCOF[2] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 9 * H / 21, "%f", G->PDAPABTCOF[3] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 10 * H / 21, "%e", G->PDAPABTCOF[4] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 11 * H / 21, "%e", G->PDAPABTCOF[5] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 12 * H / 21, "%e", G->PDAPABTCOF[6] / 0.3048);
			skpTextFmt(skp, 1 * W / 8, 13 * H / 21, "%f", G->PDAPABTCOF[7] / 0.3048);
		}
		else
		{
			skp->Text(1 * W / 8, 6 * H / 21, "J1", 2);
			skpTextFmt(skp, 2 * W / 8, 6 * H / 21, "%.4f NM", G->PDAP_J1 / 1852.0);
			skp->Text(1 * W / 8, 7 * H / 21, "K1", 2);
			skpTextFmt(skp, 2 * W / 8, 7 * H / 21, "%.4f NM/DEG", G->PDAP_K1 / 1852.0 / DEG);
			skp->Text(1 * W / 8, 8 * H / 21, "J2", 2);
			skpTextFmt(skp, 2 * W / 8, 8 * H / 21, "%.4f NM", G->PDAP_J2 / 1852.0);
			skp->Text(1 * W / 8, 9 * H / 21, "K2", 2);
			skpTextFmt(skp, 2 * W / 8, 9 * H / 21, "%.4f NM/DEG", G->PDAP_K2 / 1852.0 / DEG);
			skp->Text(1 * W / 8, 10 * H / 21, "THET", 4);
			skpTextFmt(skp, 2 * W / 8, 10 * H / 21, "%.4f°", G->PDAP_Theta_LIM* DEG);
			skp->Text(1 * W / 8, 11 * H / 21, "RMIN", 4);
			skpTextFmt(skp, 2 * W / 8, 11 * H / 21, "%.4f NM", G->PDAP_R_amin / 1852.0);
		}

		skp->Text(1 * W / 8, 15 * H / 21, "AGS Coefficients:", 18);
		skp->Text(1 * W / 8, 16 * H / 21, "224", 3);
		skpTextFmt(skp, 2 * W / 8, 16 * H / 21, "%+06.0f", G->DEDA224 / 0.3048 / 100.0);
		skp->Text(1 * W / 8, 17 * H / 21, "225", 3);
		skpTextFmt(skp, 2 * W / 8, 17 * H / 21, "%+06.0f", G->DEDA225 / 0.3048 / 100.0);
		skp->Text(1 * W / 8, 18 * H / 21, "226", 3);
		skpTextFmt(skp, 2 * W / 8, 18 * H / 21, "%+06.0f", G->DEDA226 / 0.3048 / 100.0);
		skp->Text(1 * W / 8, 19 * H / 21, "227", 3);
		skpTextFmt(skp, 2 * W / 8, 19 * H / 21, "%+06d", G->DEDA227);

		if (G->target != NULL)
		{
			sprintf(Buffer, G->target->GetName());
			skp->Text((int)(5.5 * W / 8), 4 * H / 14, Buffer, strlen(Buffer));
		}

		skp->Text(5 * W / 8, 15 * H / 21, "Landing Site:", 13);
		skpTextFmt(skp, 5 * W / 8, 16 * H / 21, "%.3f°", GC->rtcc->BZLAND.lat[RTCC_LMPOS_BEST] * DEG);
		skpTextFmt(skp, 5 * W / 8, 17 * H / 21, "%.3f°", GC->rtcc->BZLAND.lng[RTCC_LMPOS_BEST] * DEG);
		skpTextFmt(skp, 5 * W / 8, 18 * H / 21, "%.2f NM", GC->rtcc->BZLAND.rad[RTCC_LMPOS_BEST] / 1852.0);
		skp->Text(5 * W / 8, 19 * H / 21, "TLAND:", 6);
		GET_Display(Buffer, GC->rtcc->CZTDTGTU.GETTD);
		skp->Text(5 * W / 8, 20 * H / 21, Buffer, strlen(Buffer));
	}
	else if (screen == 41 || screen == 71)
	{
		FIDOOrbitDigitals *tab;

		if (screen == 41)
		{
			skp->Text(4 * W / 8, 1 * H / 28, "FDO ORBIT DIGITALS NO 1 (MSK 0046)", 18);
			tab = &GC->rtcc->EZSAVCSM;
			G->CycleFIDOOrbitDigitals1();
		}
		else
		{
			skp->Text(4 * W / 8, 1 * H / 28, "FDO ORBIT DIGITALS NO 2 (MSK 0045)", 18);
			tab = &GC->rtcc->EZSAVLEM;
			G->CycleFIDOOrbitDigitals2();
		}

		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(5 * W / 32, 3 * H / 28, "GET", 3);
		skp->Text(5 * W / 32, 4 * H / 28, "VEHICLE", 7);
		skp->Text(5 * W / 32, 5 * H / 28, "REV", 3);
		skp->Text(5 * W / 32, 6 * H / 28, "REF", 3);
		skp->Text(5 * W / 32, 7 * H / 28, "GMT ID", 6);
		skp->Text(5 * W / 32, 8 * H / 28, "GET ID", 6);
		skp->Text(5 * W / 32, 10 * H / 28, "H", 1);
		skp->Text(5 * W / 32, 11 * H / 28, "V", 1);
		skp->Text(5 * W / 32, 12 * H / 28, "GAM", 3);
		skp->Text(5 * W / 32, 14 * H / 28, "A", 1);
		skp->Text(5 * W / 32, 15 * H / 28, "E", 1);
		skp->Text(5 * W / 32, 16 * H / 28, "I", 1);
		skp->Text(5 * W / 32, 18 * H / 28, "HA", 2);
		skp->Text(5 * W / 32, 19 * H / 28, "PA", 2);
		skp->Text(5 * W / 32, 20 * H / 28, "LA", 2);
		skp->Text(5 * W / 32, 21 * H / 28, "GETA", 4);
		skp->Text(5 * W / 32, 23 * H / 28, "HP", 2);
		skp->Text(5 * W / 32, 24 * H / 28, "PP", 2);
		skp->Text(5 * W / 32, 25 * H / 28, "LP", 2);
		skp->Text(5 * W / 32, 26 * H / 28, "GETP", 4);

		skp->Text(15 * W / 32, 3 * H / 28, "LPP", 3);
		skp->Text(15 * W / 32, 4 * H / 28, "PPP", 3);
		skp->Text(15 * W / 32, 5 * H / 28, "GETCC", 5);
		skp->Text(15 * W / 32, 6 * H / 28, "TAPP", 4);
		skp->Text(15 * W / 32, 7 * H / 28, "LNPP", 4);

		skp->Text(25 * W / 32, 3 * H / 28, "REVL", 4);
		skp->Text(25 * W / 32, 4 * H / 28, "GETL", 4);
		skp->Text(25 * W / 32, 5 * H / 28, "L", 1);
		skp->Text(25 * W / 32, 6 * H / 28, "TO", 2);
		skp->Text(25 * W / 32, 7 * H / 28, "K", 1);
		skp->Text(25 * W / 32, 8 * H / 28, "ORBWT", 5);

		skp->Text(25 * W / 32, 10 * H / 28, "REQUESTED", 9);
		skp->Text(23 * W / 32, 11 * H / 28, "REF", 3);
		skp->Text(23 * W / 32, 12 * H / 28, "GETBV", 5);
		skp->Text(23 * W / 32, 13 * H / 28, "HA", 2);
		skp->Text(23 * W / 32, 14 * H / 28, "PA", 2);
		skp->Text(23 * W / 32, 15 * H / 28, "LA", 2);
		skp->Text(23 * W / 32, 16 * H / 28, "GETA", 4);
		skp->Text(23 * W / 32, 18 * H / 28, "HP", 2);
		skp->Text(23 * W / 32, 19 * H / 28, "PP", 2);
		skp->Text(23 * W / 32, 20 * H / 28, "LP", 2);
		skp->Text(23 * W / 32, 21 * H / 28, "GETP", 4);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		GET_Display(Buffer, tab->GET, false);
		skp->Text(3 * W / 16, 3 * H / 28, Buffer, strlen(Buffer));
		skp->Text(3 * W / 16, 4 * H / 28, tab->VEHID, strlen(tab->VEHID));
		skpTextFmt(skp, 3 * W / 16, 5 * H / 28, "%03d", tab->REV);
		skp->Text(3 * W / 16, 6 * H / 28, tab->REF, strlen(tab->REF));
		GET_Display(Buffer, tab->GMTID, false);
		skp->Text(3 * W / 16, 7 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, tab->GETID, false);
		skp->Text(3 * W / 16, 8 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 3 * W / 16, 10 * H / 28, "%08.1f", tab->H);
		skpTextFmt(skp, 3 * W / 16, 11 * H / 28, "%05.0f", tab->V);
		skpTextFmt(skp, 3 * W / 16, 12 * H / 28, "%+06.2f", tab->GAM);
		skpTextFmt(skp, 3 * W / 16, 14 * H / 28, "%08.1f", tab->A);
		skpTextFmt(skp, 3 * W / 16, 15 * H / 28, "%06.4f", tab->E);
		skpTextFmt(skp, 3 * W / 16, 16 * H / 28, "%05.2f", tab->I);

		if (tab->E < 1.0)
		{
			skpTextFmt(skp, 3 * W / 16, 18 * H / 28, "%08.1f", tab->HA);
			if (tab->E > 0.0001)
			{
				if (tab->PA > 0)
				{
					sprintf(Buffer, "%06.2f N", tab->PA);
				}
				else
				{
					sprintf(Buffer, "%06.2f S", abs(tab->PA));
				}
				skp->Text(3 * W / 16, 19 * H / 28, Buffer, strlen(Buffer));
				if (tab->LA > 0)
				{
					sprintf(Buffer, "%06.2f E", tab->LA);
				}
				else
				{
					sprintf(Buffer, "%06.2f W", abs(tab->LA));
				}
				skp->Text(3 * W / 16, 20 * H / 28, Buffer, strlen(Buffer));
				GET_Display(Buffer, tab->GETA, false);
				skp->Text(3 * W / 16, 21 * H / 28, Buffer, strlen(Buffer));
			}
		}

		skpTextFmt(skp, 3 * W / 16, 23 * H / 28, "%08.1f", tab->HP);
		if (tab->E > 0.0001)
		{
			if (tab->PP > 0)
			{
				sprintf(Buffer, "%06.2f N", tab->PP);
			}
			else
			{
				sprintf(Buffer, "%06.2f S", abs(tab->PP));
			}
			skp->Text(3 * W / 16, 24 * H / 28, Buffer, strlen(Buffer));
			if (tab->LP > 0)
			{
				sprintf(Buffer, "%06.2f E", tab->LP);
			}
			else
			{
				sprintf(Buffer, "%06.2f W", abs(tab->LP));
			}
			skp->Text(3 * W / 16, 25 * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, tab->GETP, false);
			skp->Text(3 * W / 16, 26 * H / 28, Buffer, strlen(Buffer));
		}

		if (tab->LPP > 0)
		{
			sprintf(Buffer, "%06.2f E", tab->LPP);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(tab->LPP));
		}
		skp->Text(8 * W / 16, 3 * H / 28, Buffer, strlen(Buffer));
		if (tab->PPP > 0)
		{
			sprintf(Buffer, "%06.2f N", tab->PPP);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(tab->PPP));
		}
		skp->Text(8 * W / 16, 4 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, tab->GETCC, false);
		skp->Text(8 * W / 16, 5 * H / 28, Buffer, strlen(Buffer));
		if (tab->E > 0.0001)
		{
			sprintf(Buffer, "%05.1f", tab->TAPP);
			skp->Text(8 * W / 16, 6 * H / 28, Buffer, strlen(Buffer));
		}
		if (tab->LNPP > 0)
		{
			sprintf(Buffer, "%06.2f E", tab->LNPP);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(tab->LNPP));
		}
		skp->Text(8 * W / 16, 7 * H / 28, Buffer, strlen(Buffer));

		skpTextFmt(skp, 13 * W / 16, 3 * H / 28, "%04d", tab->REVL);
		GET_Display(Buffer, tab->GETL, false);
		skp->Text(13 * W / 16, 4 * H / 28, Buffer, strlen(Buffer));
		if (tab->L > 0)
		{
			sprintf(Buffer, "%06.2f E", tab->L);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(tab->L));
		}
		skp->Text(13 * W / 16, 5 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, tab->TO, false);
		skp->Text(13 * W / 16, 6 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 13 * W / 16, 7 * H / 28, "%05.1f", tab->K);
		skpTextFmt(skp, 13 * W / 16, 8 * H / 28, "%07.1f", tab->ORBWT);

		skp->Text(12 * W / 16, 11 * H / 28, tab->REFR, strlen(tab->REFR));
		GET_Display(Buffer, tab->GETBV, false);
		skp->Text(12 * W / 16, 12 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 12 * W / 16, 13 * H / 28, "%08.1f", tab->HAR);
		if (tab->PAR > 0)
		{
			sprintf(Buffer, "%06.2f N", tab->PAR);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(tab->PAR));
		}
		skp->Text(12 * W / 16, 14 * H / 28, Buffer, strlen(Buffer));
		if (tab->LAR > 0)
		{
			sprintf(Buffer, "%06.2f E", tab->LAR);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(tab->LAR));
		}
		skp->Text(12 * W / 16, 15 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, tab->GETAR, false);
		skp->Text(12 * W / 16, 16 * H / 28, Buffer, strlen(Buffer));

		skpTextFmt(skp, 12 * W / 16, 18 * H / 28, "%08.1f", tab->HPR);
		if (tab->PPR > 0)
		{
			sprintf(Buffer, "%06.2f N", tab->PPR);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(tab->PPR));
		}
		skp->Text(12 * W / 16, 19 * H / 28, Buffer, strlen(Buffer));
		if (tab->LPR > 0)
		{
			sprintf(Buffer, "%06.2f E", tab->LPR);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(tab->LPR));
		}
		skp->Text(12 * W / 16, 20 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, tab->GETPR, false);
		skp->Text(12 * W / 16, 21 * H / 28, Buffer, strlen(Buffer));

		if (tab->Error > 0)
		{
			sprintf(Buffer, "Error: %d", tab->Error);
			skp->Text(8 * W / 16, 27 * H / 28, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 42)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(5 * W / 8, 1 * H / 64, "TV GUIDE (MSK 0001)", 19);

		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 16, 4 * H / 28, "0040 FDO Launch Analog No 1", 27);
		skp->Text(1 * W / 16, 5 * H / 28, "0041 FDO Launch Analog No 2", 27);
		skp->Text(1 * W / 16, 6 * H / 28, "0045 FDO Orbit Digitals LM", 26);
		skp->Text(1 * W / 16, 7 * H / 28, "0046 FDO Orbit Digitals CSM", 27);
		skp->Text(1 * W / 16, 8 * H / 28, "0047 Mission Plan Table", 23);
		skp->Text(1 * W / 16, 9 * H / 28, "0048 General Purpose Maneuver", 29);
		skp->Text(1 * W / 16, 10 * H / 28, "0054 Detailed Maneuver Table 1", 30);
		skp->Text(1 * W / 16, 11 * H / 28, "0055 CSM PSAT 1", 15);
		skp->Text(1 * W / 16, 12 * H / 28, "0056 LM PSAT 1", 14);
		skp->Text(1 * W / 16, 13 * H / 28, "0058 Rendezvous Eval Display", 28);
		skp->Text(1 * W / 16, 14 * H / 28, "0060 Relative Motion Digitals", 29);
		skp->Text(1 * W / 16, 15 * H / 28, "0069 Detailed Maneuver Table 2", 30);
		skp->Text(1 * W / 16, 16 * H / 28, "0082 Space Digitals", 19);
		skp->Text(1 * W / 16, 17 * H / 28, "0087 CSM PSAT 2", 15);
		skp->Text(1 * W / 16, 18 * H / 28, "0088 LM PSAT 2", 14);
		skp->Text(1 * W / 16, 19 * H / 28, "0229 GOST", 9);

		skp->Text(8 * W / 16, 4 * H / 28, "1501 Moonrise/Moonset Times", 27);
		skp->Text(8 * W / 16, 5 * H / 28, "1502 Sunrise/Sunset Times", 25);
		skp->Text(8 * W / 16, 6 * H / 28, "1503 Next Station Contacts", 26);
		skp->Text(8 * W / 16, 7 * H / 28, "1506 Experimental Site Acquisition", 26);
		skp->Text(8 * W / 16, 8 * H / 28, "1508 Landmark Acquisition Display", 25);
		skp->Text(8 * W / 16, 9 * H / 28, "1590 Vector Compare Display", 27);
		skp->Text(8 * W / 16, 10 * H / 28, "1591 Vector Panel Summary", 25);
		skp->Text(8 * W / 16, 11 * H / 28, "1597 Skeleton Flight Plan Table", 31);
		skp->Text(8 * W / 16, 12 * H / 28, "1619 Checkout Monitor", 21);
		skp->Text(8 * W / 16, 13 * H / 28, "1629 On Line Monitor", 20);
	}
	else if (screen == 43)
	{
		if (GC->MissionPlanningActive)
		{
			G->CycleSpaceDigitals();
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->rtcc->EZETVMED.SpaceDigVehID == 3)
		{
			skp->Text(4 * W / 8, 1 * H / 64, "LEM SPACE DIGITALS", 18);
		}
		else
		{
			skp->Text(4 * W / 8, 1 * H / 64, "CSM SPACE DIGITALS", 18);
		}

		skp->SetFont(font2);

		skp->Text(1 * W / 32, 2 * H / 28, "STA ID", 6);
		skp->Text(1 * W / 32, 3 * H / 28, "GMTV", 4);
		skp->Text(1 * W / 32, 5 * H / 28, "GET", 3);
		skp->Text(1 * W / 32, 6 * H / 28, "REF", 3);
		skp->Text(1 * W / 32, 8 * H / 28, "GET VECTOR 1", 12);
		skp->Text(1 * W / 32, 10 * H / 28, "REF", 3);
		skp->Text(11 * W / 64, 10 * H / 28, "WT", 2);
		skp->Text(1 * W / 32, 11 * H / 28, "AREA", 4);
		skp->Text(1 * W / 32, 12 * H / 28, "GETA", 4);
		skp->Text(1 * W / 32, 13 * H / 28, "HA", 2);
		skp->Text(1 * W / 32, 14 * H / 28, "HP", 2);
		skp->Text(1 * W / 32, 15 * H / 28, "H", 1);
		skp->Text(1 * W / 32, 16 * H / 28, "V", 1);
		skp->Text(1 * W / 32, 17 * H / 28, "GAM", 3);
		skp->Text(1 * W / 32, 18 * H / 28, "PSI", 3);
		skp->Text(1 * W / 32, 19 * H / 28, "PHI", 3);
		skp->Text(1 * W / 32, 20 * H / 28, "LAM", 3);
		skp->Text(1 * W / 32, 21 * H / 28, "HS", 2);
		skp->Text(1 * W / 32, 22 * H / 28, "HO", 2);
		skp->Text(1 * W / 32, 23 * H / 28, "PHIO", 4);
		skp->Text(1 * W / 32, 24 * H / 28, "IEMP", 4);
		skp->Text(1 * W / 32, 25 * H / 28, "W", 1);
		skp->Text(1 * W / 32, 26 * H / 28, "OMG", 3);
		skp->Text(1 * W / 32, 27 * H / 28, "PRA", 3);
		skp->Text(8 * W / 32, 24 * H / 28, "A", 1);
		skp->Text(8 * W / 32, 25 * H / 28, "L", 1);
		skp->Text(8 * W / 32, 26 * H / 28, "E", 1);
		skp->Text(8 * W / 32, 27 * H / 28, "I", 1);

		skp->Text(11 * W / 32, 2 * H / 28, "WEIGHT", 6);
		skp->Text(11 * W / 32, 3 * H / 28, "GETV", 4);
		skp->Text(11 * W / 32, 8 * H / 28, "GET VECTOR 2", 12);
		skp->Text(11 * W / 32, 10 * H / 28, "GETSI", 5);
		skp->Text(11 * W / 32, 11 * H / 28, "GETCA", 5);
		skp->Text(11 * W / 32, 12 * H / 28, "VCA", 3);
		skp->Text(11 * W / 32, 13 * H / 28, "HCA", 3);
		skp->Text(11 * W / 32, 14 * H / 28, "PCA", 3);
		skp->Text(11 * W / 32, 15 * H / 28, "LCA", 3);
		skp->Text(11 * W / 32, 16 * H / 28, "PSICA", 5);
		skp->Text(11 * W / 32, 17 * H / 28, "GETMN", 5);
		skp->Text(11 * W / 32, 18 * H / 28, "HMN", 3);
		skp->Text(11 * W / 32, 19 * H / 28, "PMN", 3);
		skp->Text(11 * W / 32, 20 * H / 28, "LMN", 3);
		skp->Text(11 * W / 32, 21 * H / 28, "DMN", 3);

		skp->Text(21 * W / 32, 3 * H / 28, "GET AXIS", 8);
		skp->Text(21 * W / 32, 8 * H / 28, "GET VECTOR 3", 12);
		skp->Text(21 * W / 32, 10 * H / 28, "GETSE", 5);
		skp->Text(21 * W / 32, 11 * H / 28, "GETEI", 5);
		skp->Text(21 * W / 32, 12 * H / 28, "VEI", 3);
		skp->Text(21 * W / 32, 13 * H / 28, "GEI", 3);
		skp->Text(21 * W / 32, 14 * H / 28, "PEI", 3);
		skp->Text(21 * W / 32, 15 * H / 28, "LEI", 3);
		skp->Text(21 * W / 32, 16 * H / 28, "PSIEI", 5);
		skp->Text(21 * W / 32, 17 * H / 28, "GETVP", 5);
		skp->Text(21 * W / 32, 18 * H / 28, "VVP", 3);
		skp->Text(21 * W / 32, 19 * H / 28, "HVP", 3);
		skp->Text(21 * W / 32, 20 * H / 28, "PVP", 3);
		skp->Text(21 * W / 32, 21 * H / 28, "LVP", 3);
		skp->Text(21 * W / 32, 22 * H / 28, "PSI VP", 6);
		skp->Text(21 * W / 32, 26 * H / 28, "IE", 2);
		skp->Text(21 * W / 32, 27 * H / 28, "LN", 2);

		skp->Text(9 * W / 32, 5 * H / 28, "V", 1);
		skp->Text(13 * W / 32, 5 * H / 28, "PHI", 3);
		skp->Text(20 * W / 32, 5 * H / 28, "H", 1);
		skp->Text(25 * W / 32, 5 * H / 28, "ADA", 3);

		skp->Text(8 * W / 32, 6 * H / 28, "GAM", 3);
		skp->Text(15 * W / 32, 6 * H / 28, "LAM", 3);
		skp->Text(23 * W / 32, 6 * H / 28, "PSI", 3);

		Text(skp, 14 * W / 32, 27 * H / 28, GC->rtcc->EZSPACE.errormessage);

		Text(skp, 5 * W / 32, 2 * H / 28, GC->rtcc->EZSPACE.VecID);

		GET_Display(Buffer, GC->rtcc->EZSPACE.GMTV, false);
		skp->Text(4 * W / 32, 3 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->EZSPACE.GET, false);
		skp->Text(4 * W / 32, 5 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 6 * H / 28, GC->rtcc->EZSPACE.REF, strlen(GC->rtcc->EZSPACE.REF));

		skpTextFmt(skp, 16 * W / 32, 2 * H / 28, "%07.1f", GC->rtcc->EZSPACE.WEIGHT);
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETV, false);
		skp->Text(14 * W / 32, 3 * H / 28, Buffer, strlen(Buffer));

		GET_Display(Buffer, GC->rtcc->EZSPACE.GETR, false);
		skp->Text(26 * W / 32, 2 * H / 28, Buffer, strlen(Buffer));

		skpTextFmt(skp, 10 * W / 32, 5 * H / 28, "%05.0f", GC->rtcc->EZSPACE.V);
		if (GC->rtcc->EZSPACE.PHI > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PHI);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PHI));
		}
		skp->Text(15 * W / 32, 5 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 21 * W / 32, 5 * H / 28, "%06.0f", GC->rtcc->EZSPACE.H);
		skpTextFmt(skp, 28 * W / 32, 5 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.ADA);

		skpTextFmt(skp, 11 * W / 32, 6 * H / 28, "%+06.1f°", GC->rtcc->EZSPACE.GAM);
		if (GC->rtcc->EZSPACE.LAM > 0)
		{
			sprintf(Buffer, "%06.2f E", GC->rtcc->EZSPACE.LAM);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(GC->rtcc->EZSPACE.LAM));
		}
		skp->Text(18 * W / 32, 6 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 25 * W / 32, 6 * H / 28, "%+06.1f°", GC->rtcc->EZSPACE.PSI);

		GET_Display(Buffer, GC->rtcc->EZSPACE.GETVector1, false);
		skp->Text(1 * W / 32, 9 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETVector2, false);
		skp->Text(11 * W / 32, 9 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETVector3, false);
		skp->Text(21 * W / 32, 9 * H / 28, Buffer, strlen(Buffer));

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(25 * W / 32, 2 * H / 28, "GETR", 4);

		skp->Text(10 * W / 64, 10 * H / 28, GC->rtcc->EZSPACE.REF1, strlen(GC->rtcc->EZSPACE.REF1));
		skpTextFmt(skp, 10 * W / 32, 10 * H / 28, "%05.0f", GC->rtcc->EZSPACE.WT);
		skpTextFmt(skp, 10 * W / 32, 13 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HA);
		skpTextFmt(skp, 10 * W / 32, 14 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HP);
		skpTextFmt(skp, 10 * W / 32, 15 * H / 28, "%08.1f", GC->rtcc->EZSPACE.H1);
		skpTextFmt(skp, 10 * W / 32, 16 * H / 28, "%05.0f", GC->rtcc->EZSPACE.V1);
		skpTextFmt(skp, 10 * W / 32, 17 * H / 28, "%+06.1f°", GC->rtcc->EZSPACE.GAM1);
		skpTextFmt(skp, 10 * W / 32, 18 * H / 28, "%+06.1f°", GC->rtcc->EZSPACE.PSI1);
		if (GC->rtcc->EZSPACE.PHI1 > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PHI1);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PHI1));
		}
		skp->Text(10 * W / 32, 19 * H / 28, Buffer, strlen(Buffer));
		if (GC->rtcc->EZSPACE.LAM1 > 0)
		{
			sprintf(Buffer, "%06.2f E", GC->rtcc->EZSPACE.LAM1);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(GC->rtcc->EZSPACE.LAM1));
		}
		skp->Text(10 * W / 32, 20 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 10 * W / 32, 21 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HS);
		skpTextFmt(skp, 10 * W / 32, 22 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HO);
		if (GC->rtcc->EZSPACE.PHIO > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PHIO);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PHIO));
		}
		skp->Text(10 * W / 32, 23 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 7 * W / 32, 24 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.IEMP);
		skpTextFmt(skp, 7 * W / 32, 25 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.W1);
		skpTextFmt(skp, 7 * W / 32, 26 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.OMG);
		skpTextFmt(skp, 7 * W / 32, 27 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.PRA);

		if (GC->rtcc->EZSPACE.A1 > 0)
		{
			skpTextFmt(skp, 13 * W / 32, 24 * H / 28, "%06.0f", GC->rtcc->EZSPACE.A1);
		}
		skpTextFmt(skp, 13 * W / 32, 25 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.L1);
		skpTextFmt(skp, 13 * W / 32, 26 * H / 28, "%.5f", GC->rtcc->EZSPACE.E1);
		skpTextFmt(skp, 13 * W / 32, 27 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.I1);

		GET_Display(Buffer, GC->rtcc->EZSPACE.GETSI, false);
		skp->Text(20 * W / 32, 10 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETCA, false);
		skp->Text(20 * W / 32, 11 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 20 * W / 32, 12 * H / 28, "%05.0f", GC->rtcc->EZSPACE.VCA);
		skpTextFmt(skp, 20 * W / 32, 13 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HCA);
		if (GC->rtcc->EZSPACE.PCA > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PCA);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PCA));
		}
		skp->Text(20 * W / 32, 14 * H / 28, Buffer, strlen(Buffer));
		if (GC->rtcc->EZSPACE.LCA > 0)
		{
			sprintf(Buffer, "%06.2f E", GC->rtcc->EZSPACE.LCA);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(GC->rtcc->EZSPACE.LCA));
		}
		skp->Text(20 * W / 32, 15 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 20 * W / 32, 16 * H / 28, "%+06.1f°", GC->rtcc->EZSPACE.PSICA);
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETMN, false);
		skp->Text(20 * W / 32, 17 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 20 * W / 32, 18 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HMN);
		if (GC->rtcc->EZSPACE.PMN > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PMN);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PMN));
		}
		skp->Text(20 * W / 32, 19 * H / 28, Buffer, strlen(Buffer));
		if (GC->rtcc->EZSPACE.LMN > 0)
		{
			sprintf(Buffer, "%06.2f E", GC->rtcc->EZSPACE.LMN);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(GC->rtcc->EZSPACE.LMN));
		}
		skp->Text(20 * W / 32, 20 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 20 * W / 32, 21 * H / 28, "%05.2f", GC->rtcc->EZSPACE.DMN);


		GET_Display(Buffer, GC->rtcc->EZSPACE.GETSE, false);
		skp->Text(30 * W / 32, 10 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETEI, false);
		skp->Text(30 * W / 32, 11 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 30 * W / 32, 12 * H / 28, "%08.2f", GC->rtcc->EZSPACE.VEI);
		skpTextFmt(skp, 30 * W / 32, 13 * H / 28, "%+07.2f°", GC->rtcc->EZSPACE.GEI);
		if (GC->rtcc->EZSPACE.PEI > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PEI);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PEI));
		}
		skp->Text(30 * W / 32, 14 * H / 28, Buffer, strlen(Buffer));
		if (GC->rtcc->EZSPACE.LEI > 0)
		{
			sprintf(Buffer, "%06.2f E", GC->rtcc->EZSPACE.LEI);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(GC->rtcc->EZSPACE.LEI));
		}
		skp->Text(30 * W / 32, 15 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 30 * W / 32, 16 * H / 28, "%+07.2f°", GC->rtcc->EZSPACE.PSIEI);
		GET_Display(Buffer, GC->rtcc->EZSPACE.GETVP, false);
		skp->Text(30 * W / 32, 17 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 30 * W / 32, 18 * H / 28, "%08.2f", GC->rtcc->EZSPACE.VVP);
		skpTextFmt(skp, 30 * W / 32, 19 * H / 28, "%08.1f", GC->rtcc->EZSPACE.HVP);
		if (GC->rtcc->EZSPACE.PVP > 0)
		{
			sprintf(Buffer, "%06.2f N", GC->rtcc->EZSPACE.PVP);
		}
		else
		{
			sprintf(Buffer, "%06.2f S", abs(GC->rtcc->EZSPACE.PVP));
		}
		skp->Text(30 * W / 32, 20 * H / 28, Buffer, strlen(Buffer));
		if (GC->rtcc->EZSPACE.LVP > 0)
		{
			sprintf(Buffer, "%06.2f E", GC->rtcc->EZSPACE.LVP);
		}
		else
		{
			sprintf(Buffer, "%06.2f W", abs(GC->rtcc->EZSPACE.LVP));
		}
		skp->Text(30 * W / 32, 21 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 30 * W / 32, 22 * H / 28, "%+07.2f°", GC->rtcc->EZSPACE.PSIVP);

		skpTextFmt(skp, 30 * W / 32, 26 * H / 28, "%05.2f°", GC->rtcc->EZSPACE.IE);
		skpTextFmt(skp, 30 * W / 32, 27 * H / 28, "%06.2f°", GC->rtcc->EZSPACE.LN);
	}
	else if (screen == 44)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "MISSION PLAN TABLE (MSK 0047)", 29);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		if (GC->MissionPlanningActive)
		{
			skp->Text(1 * W / 32, 2 * H / 14, "Active", 6);
		}
		else
		{
			skp->Text(1 * W / 32, 2 * H / 14, "Inactive", 8);
		}

		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(3 * W / 32, 8 * H / 28, "GETBI", 5);
		skp->Text(8 * W / 32, 8 * H / 28, "DT", 2);
		skp->Text(12 * W / 32, 8 * H / 28, "DELTAV", 6);
		skp->Text(16 * W / 32, 8 * H / 28, "DVREM", 5);
		skp->Text(20 * W / 32, 8 * H / 28, "HA", 2);
		skp->Text(47 * W / 64, 8 * H / 28, "HP", 2);
		skp->Text(29 * W / 32, 8 * H / 28, "CODE", 4);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(6 * W / 32, 5 * H / 28, "CSM STA ID", 10);
		Text(skp, 12 * W / 32, 5 * H / 28, GC->rtcc->MPTDISPLAY.CSMSTAID);
		skp->Text(6 * W / 32, 6 * H / 28, "GETAV", 5);
		Text(skp, 10 * W / 32, 6 * H / 28, GC->rtcc->MPTDISPLAY.CSMGETAV);
		skp->Text(18 * W / 32, 5 * H / 28, "LEM STA ID", 10);
		Text(skp, 24 * W / 32, 5 * H / 28, GC->rtcc->MPTDISPLAY.LEMSTAID);
		skp->Text(18 * W / 32, 6 * H / 28, "GETAV", 5);
		Text(skp, 22 * W / 32, 6 * H / 28, GC->rtcc->MPTDISPLAY.LEMGETAV);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		for (unsigned i = 0;i < GC->rtcc->MPTDISPLAY.man.size();i++)
		{
			Text(skp, 5 * W / 32, (i * 2 + 9) * H / 28, GC->rtcc->MPTDISPLAY.man[i].GETBI);

			skpTextFmt(skp, 14 * W / 32, (i * 2 + 9) * H / 28, "%07.1f", GC->rtcc->MPTDISPLAY.man[i].DELTAV);
			skpTextFmt(skp, 18 * W / 32, (i * 2 + 9) * H / 28, "%.1f", GC->rtcc->MPTDISPLAY.man[i].DVREM);
			skpTextFmt(skp, 43 * W / 64, (i * 2 + 9) * H / 28, "%06.1f", GC->rtcc->MPTDISPLAY.man[i].HA);
			skpTextFmt(skp, 50 * W / 64, (i * 2 + 9) * H / 28, "%06.1f", GC->rtcc->MPTDISPLAY.man[i].HP);

			Text(skp, 63 * W / 64, (i * 2 + 9) * H / 28, GC->rtcc->MPTDISPLAY.man[i].code);
		}

		for (unsigned i = 1;i < GC->rtcc->MPTDISPLAY.man.size();i++)
		{
			Text(skp, 10 * W / 32, (i * 2 + 8) * H / 28, GC->rtcc->MPTDISPLAY.man[i].DT);
		}

	}
	else if (screen == 45)
	{
		G->CycleNextStationContactsDisplay();

		if (GC->rtcc->SystemParameters.MGRTAG == 0)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Lunar", 5);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "All", 3);
		}

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 3 * H / 28, "NEXT STATION CONTACTS (MSK 1503)", 21);

		skp->Text(8 * W / 32, 5 * H / 28, "CSM", 3);
		skp->Text(24 * W / 32, 5 * H / 28, "LEM", 3);

		skp->SetFont(font2);

		skp->Text(12 * W / 32, 5 * H / 28, "GET", 3);

		skp->Text(2 * W / 32, 17 * H / 56, "STA", 3);
		skp->Text(6 * W / 32, 8 * H / 28, "GETHCA", 6);
		skp->Text(6 * W / 32, 9 * H / 28, "DT KLOS", 7);
		skp->Text(6 * W / 32, 10 * H / 28, "HH MM SS", 8);
		skp->Text(10 * W / 32, 17 * H / 56, "EMAX", 4);
		skp->Text(10 * W / 32, 10 * H / 28, "DEG", 4);
		skp->Text(28 * W / 64, 8 * H / 28, "DTPASS", 6);
		skp->Text(28 * W / 64, 9 * H / 28, "DT KH", 5);
		skp->Text(28 * W / 64, 10 * H / 28, "HH MM SS", 8);

		skp->Text(35 * W / 64, 17 * H / 56, "STA", 3);
		skp->Text(43 * W / 64, 8 * H / 28, "GETHCA", 6);
		skp->Text(43 * W / 64, 9 * H / 28, "DT KLOS", 7);
		skp->Text(43 * W / 64, 10 * H / 28, "HH MM SS", 8);
		skp->Text(51 * W / 64, 17 * H / 56, "EMAX", 4);
		skp->Text(51 * W / 64, 10 * H / 28, "DEG", 3);
		skp->Text(59 * W / 64, 8 * H / 28, "DTPASS", 6);
		skp->Text(59 * W / 64, 9 * H / 28, "DT KH", 5);
		skp->Text(59 * W / 64, 10 * H / 28, "HH MM SS", 8);

		GET_Display(Buffer, GC->rtcc->NextStationContactsBuffer.GET, false);
		skp->Text(16 * W / 32, 5 * H / 28, Buffer, strlen(Buffer));

		unsigned i, j;
		for (j = 0;j < 2;j++)
		{
			for (i = 0;i < 6;i++)
			{
				Text(skp, (4 + j * 31) * W / 64, (2*i + 11) * H / 28, GC->rtcc->NextStationContactsBuffer.STA[j][i]);

				if (GC->rtcc->NextStationContactsBuffer.BestAvailableAOS[j][i])
				{
					skp->Text((7 + j * 31) * W / 64, (2*i + 11) * H / 28, "*", 1);
				}
				GET_Display(Buffer, GC->rtcc->NextStationContactsBuffer.GETHCA[j][i], false);
				skp->Text((12 + j * 31) * W / 64, (2*i + 11) * H / 28, Buffer, strlen(Buffer));

				if (GC->rtcc->NextStationContactsBuffer.BestAvailableEMAX[j][i])
				{
					skp->Text((17 + j * 31) * W / 64, (2*i + 11) * H / 28, "*", 1);
				}
				skpTextFmt(skp, (20 + j * 31) * W / 64, (2*i + 11) * H / 28, "%.1f", GC->rtcc->NextStationContactsBuffer.EMAX[j][i]);

				GET_Display4(Buffer, GC->rtcc->NextStationContactsBuffer.DTPASS[j][i]);
				skp->Text((27 + j * 31) * W / 64, (2*i + 11) * H / 28, Buffer, strlen(Buffer));
			}
		}
	}
	else if (screen == 46 || screen == 72 || screen == 73 || screen == 74)
	{
		PredictedSiteAcquisitionTable *tab;

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		if (screen == 46)
		{
			skp->Text(4 * W / 8, 1 * H / 28, "PREDICTED SITE ACQUISITION TABLE NO 1", 37);
			tab = &GC->rtcc->EZACQ1;
		}
		else if (screen == 72)
		{
			skp->Text(4 * W / 8, 1 * H / 28, "PREDICTED SITE ACQUISITION TABLE NO 1", 37);
			tab = &GC->rtcc->EZACQ3;
		}
		else if (screen == 73)
		{
			skp->Text(4 * W / 8, 1 * H / 28, "PREDICTED SITE ACQUISITION TABLE NO 2", 37);
			tab = &GC->rtcc->EZDPSAD1;
		}
		else
		{
			skp->Text(4 * W / 8, 1 * H / 28, "PREDICTED SITE ACQUISITION TABLE NO 2", 37);
			tab = &GC->rtcc->EZDPSAD3;
		}

		skp->SetFont(font2);

		skp->Text(22 * W / 32, 3 * H / 28, "STA ID", 6);
		skp->Text(4 * W / 32, 11 * H / 56, "REV", 3);
		skp->Text(10 * W / 32, 11 * H / 56, "STA", 3);
		skp->Text(15 * W / 32, 5 * H / 28, "AOS", 3);
		skp->Text(15 * W / 32, 6 * H / 28, "GET", 3);
		skp->Text(21 * W / 32, 5 * H / 28, "LOS", 3);
		skp->Text(21 * W / 32, 6 * H / 28, "GET", 3);
		skp->Text(26 * W / 32, 5 * H / 28, "MAX ELEV", 8);
		skp->Text(26 * W / 32, 6 * H / 28, "DEG", 3);

		skpTextFmt(skp, 6 * W / 32, 3 * H / 28, "PAGE %02d OF %02d", tab->curpage, tab->pages);
		skpTextFmt(skp, 14 * W / 32, 3 * H / 28, "VEHICLE %s", tab->VEHICLE.c_str());

		for (unsigned i = 0;i < tab->numcontacts[tab->curpage - 1];i++)
		{
			skpTextFmt(skp, 4 * W / 32, (i + 7) * H / 28, "%03d", tab->REV[tab->curpage - 1][i]);

			Text(skp, 10 * W / 32, (i + 7) * H / 28, tab->STA[tab->curpage - 1][i]);

			if (tab->BestAvailableAOS[tab->curpage - 1][i])
			{
				skp->Text(25 * W / 64, (i + 7) * H / 28, "*", 1);
			}

			GET_Display(Buffer, tab->GETHCA[tab->curpage - 1][i], false);
			skp->Text(15 * W / 32, (i + 7) * H / 28, Buffer, strlen(Buffer));

			if (tab->BestAvailableLOS[tab->curpage - 1][i])
			{
				skp->Text(37 * W / 64, (i + 7) * H / 28, "*", 1);
			}

			GET_Display(Buffer, tab->GETHCD[tab->curpage - 1][i], false);
			skp->Text(21 * W / 32, (i + 7) * H / 28, Buffer, strlen(Buffer));

			if (tab->BestAvailableEMAX[tab->curpage - 1][i])
			{
				skp->Text(24 * W / 32, (i + 7) * H / 28, "*", 1);
			}
			skpTextFmt(skp, 26 * W / 32, (i + 7) * H / 28, "%04.1f", tab->ELMAX[tab->curpage - 1][i]);
		}
	}
	else if (screen == 47)
	{
		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 16, 4 * H / 28, "00: CMC CSM Navigation Update", 29);
		skp->Text(1 * W / 16, 5 * H / 28, "06: CMC Landing Site Vector", 27);
		skp->Text(1 * W / 16, 6 * H / 28, "07: CMC Time Increment", 22);
		skp->Text(1 * W / 16, 7 * H / 28, "08: CMC Liftoff Time Update", 27);
		skp->Text(1 * W / 16, 8 * H / 28, "09: CMC LM Navigation Update", 28);
		skp->Text(1 * W / 16, 9 * H / 28, "10: CMC External DV", 19);
		skp->Text(1 * W / 16, 10 * H / 28, "12: CMC REFSMMAT Update", 23);
		skp->Text(1 * W / 16, 11 * H / 28, "13: CMC Retrofire External DV", 29);
		skp->Text(1 * W / 16, 12 * H / 28, "14: CMC Entry Update", 20);

		skp->Text(1 * W / 16, 14 * H / 28, "20: LGC LM Navigation Update", 28);
		skp->Text(1 * W / 16, 15 * H / 28, "21: LGC CSM Navigation Update", 29);
		skp->Text(1 * W / 16, 16 * H / 28, "22: LGC External DV", 19);
		skp->Text(1 * W / 16, 17 * H / 28, "23: LGC REFSMMAT Update", 23);
		skp->Text(1 * W / 16, 18 * H / 28, "24: LGC Time Increment", 22);
		skp->Text(1 * W / 16, 19 * H / 28, "25: LGC Liftoff Time Update", 27);
		skp->Text(1 * W / 16, 20 * H / 28, "26: LGC Landing Site Vector", 27);
		skp->Text(1 * W / 16, 21 * H / 28, "28: LGC Descent Update", 22);

		skp->Text(9 * W / 16, 4 * H / 28, "49: LVDC Navigation Update", 26);
	}
	else if (screen == 48 || screen == 99 || screen == 100 || screen == 101)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		RTCC::NavUpdateMakeupBuffer * tab;
		if (screen == 48)
		{
			skp->Text(4 * W / 8, 1 * H / 14, "CSM NAV UPDATE TO CMC (276)", 27);
			tab = &GC->rtcc->CZNAVGEN.CMCCSMUpdate;
		}
		else if (screen == 99)
		{
			skp->Text(4 * W / 8, 1 * H / 14, "LM NAV UPDATE TO CMC (270)", 26);
			tab = &GC->rtcc->CZNAVGEN.CMCLEMUpdate;
		}
		else if (screen == 100)
		{
			skp->Text(4 * W / 8, 1 * H / 14, "LGC CSM NAV UPDATE (278)", 24);
			tab = &GC->rtcc->CZNAVGEN.LGCCSMUpdate;
		}
		else
		{
			skp->Text(4 * W / 8, 1 * H / 14, "LGC LM NAV UPDATE (279)", 23);
			tab = &GC->rtcc->CZNAVGEN.LGCLEMUpdate;
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 8, 4 * H / 28, "GET", 3);
		skp->Text(1 * W / 8, 5 * H / 28, "LOAD NO", 7);
		skp->Text(1 * W / 8, 6 * H / 28, "REF", 3);

		if (G->SVDesiredGET < 0)
		{
			sprintf(Buffer, "Present GET");
		}
		else
		{
			GET_Display(Buffer, G->SVDesiredGET);
		}
		skp->Text(2 * W / 8, 4 * H / 28, Buffer, strlen(Buffer));
		skpTextFmt(skp, 3 * W / 8, 5 * H / 28, "%04d", tab->SequenceNumber);
		if (tab->sv.RBI == BODY_EARTH)
		{
			skp->Text(2 * W / 8, 6 * H / 28, "Earth", 5);
		}
		else if (tab->sv.RBI == BODY_MOON)
		{
			skp->Text(2 * W / 8, 6 * H / 28, "Moon", 4);
		}

		if (GC->MissionPlanningActive == false)
		{
			if (G->svtarget != NULL)
			{
				sprintf(Buffer, G->svtarget->GetName());
			}
			else
			{
				sprintf(Buffer, "No Vehicle");
			}
			skp->Text(5 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
		}

		skp->Text(5 * W / 32, 9 * H / 28, "OID", 3);
		skp->Text(10 * W / 32, 9 * H / 28, "FCT", 3);
		skp->Text(15 * W / 32, 9 * H / 28, "DSKY V71", 8);
		skp->Text(22 * W / 32, 9 * H / 28, "VECTOR", 6);

		for (int i = 1;i <= 021;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (i + 9) * H / 28, "%o", i);
		}

		skp->Text(10 * W / 32, 10 * H / 28, "INDEX", 5);
		skp->Text(10 * W / 32, 11 * H / 28, "ADD", 3);
		skp->Text(10 * W / 32, 12 * H / 28, "VID", 3);
		skp->Text(10 * W / 32, 13 * H / 28, "X", 1);
		skp->Text(10 * W / 32, 14 * H / 28, "X", 1);
		skp->Text(10 * W / 32, 15 * H / 28, "Y", 1);
		skp->Text(10 * W / 32, 16 * H / 28, "Y", 1);
		skp->Text(10 * W / 32, 17 * H / 28, "Z", 1);
		skp->Text(10 * W / 32, 18 * H / 28, "Z", 1);
		skp->Text(10 * W / 32, 19 * H / 28, "X-DOT", 5);
		skp->Text(10 * W / 32, 20 * H / 28, "X-DOT", 5);
		skp->Text(10 * W / 32, 21 * H / 28, "Y-DOT", 5);
		skp->Text(10 * W / 32, 22 * H / 28, "Y-DOT", 5);
		skp->Text(10 * W / 32, 23 * H / 28, "Z-DOT", 5);
		skp->Text(10 * W / 32, 24 * H / 28, "Z-DOT", 5);
		skp->Text(10 * W / 32, 25 * H / 28, "T", 1);
		skp->Text(10 * W / 32, 26 * H / 28, "T", 1);

		for (int i = 0;i < 021;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 10) * H / 28, "%05d", tab->Octals[i]);
		}

		if (tab->sv.RBI == BODY_EARTH)
		{
			skp->Text(22 * W / 32, 11 * H / 28, "EARTH", 5);
		}
		else
		{
			skp->Text(22 * W / 32, 11 * H / 28, "MOON", 4);
		}
		skpTextFmt(skp, 22 * W / 32, 13 * H / 28, "%.1f", tab->sv.R.x);
		skpTextFmt(skp, 22 * W / 32, 15 * H / 28, "%.1f", tab->sv.R.y);
		skpTextFmt(skp, 22 * W / 32, 17 * H / 28, "%.1f", tab->sv.R.z);
		skpTextFmt(skp, 22 * W / 32, 19 * H / 28, "%.1f", tab->sv.V.x);
		skpTextFmt(skp, 22 * W / 32, 21 * H / 28, "%.1f", tab->sv.V.y);
		skpTextFmt(skp, 22 * W / 32, 23 * H / 28, "%.1f", tab->sv.V.z);
		GET_Display(Buffer, tab->sv.GMT, false);
		skp->Text(22 * W / 32, 25 * H / 28, Buffer, strlen(Buffer));
	}
	else if (screen == 49)
	{
		skp->Text(5 * W / 8, (int)(0.5 * H / 14), "Landing Site Update", 19);

		if (G->svtarget != NULL)
		{
			sprintf(Buffer, G->svtarget->GetName());
			skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "No Target!", 10);
		}

		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.3f°", GC->rtcc->BZLAND.lat[RTCC_LMPOS_BEST] * DEG);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.3f°", GC->rtcc->BZLAND.lng[RTCC_LMPOS_BEST] * DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2f NM", GC->rtcc->BZLAND.rad[RTCC_LMPOS_BEST] / 1852.0);
	}
	else if (screen == 50 || screen == 98)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		RTCC::LandingSiteMakupBuffer *tab;

		if (screen == 50)
		{
			skp->Text(4 * W / 8, 2 * H / 14, "LANDING SITE UPDT TO CMC (293)", 30);
			tab = &GC->rtcc->CZLSVECT.CSMLSUpdate;
		}
		else
		{
			skp->Text(4 * W / 8, 2 * H / 14, "LANDING SITE UPDT TO LGC (294)", 30);
			tab = &GC->rtcc->CZLSVECT.LMLSUpdate;
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 8, 4 * H / 14, "LAT", 3);
		skp->Text(1 * W / 8, 5 * H / 14, "LNG", 3);
		skpTextFmt(skp, 2 * W / 8, 4 * H / 14, "%.3f°", tab->lat* DEG);
		skpTextFmt(skp, 2 * W / 8, 5 * H / 14, "%.3f°", tab->lng* DEG);

		skp->Text(4 * W / 8, 4 * H / 14, "RAD", 3);
		skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%.2f NM", tab->rad / 1852.0);

		skp->Text(5 * W / 32, 13 * H / 28, "OID", 3);
		skp->Text(10 * W / 32, 13 * H / 28, "FCT", 3);
		skp->Text(15 * W / 32, 13 * H / 28, "DSKY V71", 8);
		skp->Text(22 * W / 32, 13 * H / 28, "VECTOR", 6);

		for (int i = 1;i <= 010;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (i + 14) * H / 28, "%o", i);
		}

		skp->Text(10 * W / 32, 15 * H / 28, "INDEX", 5);
		skp->Text(10 * W / 32, 16 * H / 28, "ADD", 3);
		skp->Text(10 * W / 32, 17 * H / 28, "X", 1);
		skp->Text(10 * W / 32, 18 * H / 28, "X", 1);
		skp->Text(10 * W / 32, 19 * H / 28, "Y", 1);
		skp->Text(10 * W / 32, 20 * H / 28, "Y", 1);
		skp->Text(10 * W / 32, 21 * H / 28, "Z", 1);
		skp->Text(10 * W / 32, 22 * H / 28, "Z", 1);

		for (int i = 0;i < 010;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 15) * H / 28, "%05d", tab->Octals[i]);
		}

		skpTextFmt(skp, 22 * W / 32, 17 * H / 28, "%.1f", tab->R_LS.x);
		skpTextFmt(skp, 22 * W / 32, 19 * H / 28, "%.1f", tab->R_LS.y);
		skpTextFmt(skp, 22 * W / 32, 21 * H / 28, "%.1f", tab->R_LS.z);
	}
	else if (screen == 51 || screen == 102)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		if (screen == 51)
		{
			skp->Text(4 * W / 8, 2 * H / 14, "CMC EXTERNAL DV UPDATE (264)", 28);
		}
		else
		{
			skp->Text(4 * W / 8, 2 * H / 14, "LGC EXTERNAL DV UPDATE (280)", 28);
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(5 * W / 32, 8 * H / 28, "OID", 3);
		skp->Text(10 * W / 32, 8 * H / 28, "FCT", 3);
		skp->Text(15 * W / 32, 8 * H / 28, "DSKY V71", 8);

		for (int i = 1;i <= 012;i++)
		{
			sprintf(Buffer, "%o", i);
			skp->Text(5 * W / 32, (i + 9) * H / 28, Buffer, strlen(Buffer));
		}

		skp->Text(10 * W / 32, 10 * H / 28, "INDEX", 5);
		skp->Text(10 * W / 32, 11 * H / 28, "ADD", 3);
		skp->Text(10 * W / 32, 12 * H / 28, "VGX", 3);
		skp->Text(10 * W / 32, 13 * H / 28, "VGX", 3);
		skp->Text(10 * W / 32, 14 * H / 28, "VGY", 3);
		skp->Text(10 * W / 32, 15 * H / 28, "VGY", 3);
		skp->Text(10 * W / 32, 16 * H / 28, "VGZ", 3);
		skp->Text(10 * W / 32, 17 * H / 28, "VGZ", 3);
		skp->Text(10 * W / 32, 18 * H / 28, "TIGN", 4);
		skp->Text(10 * W / 32, 19 * H / 28, "TIGN", 4);

		RTCC::ExternalDVMakeupBuffer *tab;
		if (screen == 51)
		{
			tab = &GC->rtcc->CZAXTRDV;
		}
		else
		{
			tab = &GC->rtcc->CZLXTRDV;
		}

		for (int i = 0;i < 012;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 10) * H / 28, "%05d", tab->Octals[i]);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(27 * W / 32, 8 * H / 28, "DECIMAL", 7);
		if (GC->MissionPlanningActive)
		{
			skpTextFmt(skp, 27 * W / 32, 12 * H / 28, "%+07.1f", tab->DV.x);
			skpTextFmt(skp, 27 * W / 32, 14 * H / 28, "%+07.1f", tab->DV.y);
			skpTextFmt(skp, 27 * W / 32, 16 * H / 28, "%+07.1f", tab->DV.z);
			GET_Display2(Buffer, tab->GET);
			skp->Text(27 * W / 32, 18 * H / 28, Buffer, strlen(Buffer));
		}
		else
		{
			skpTextFmt(skp, 27 * W / 32, 12 * H / 28, "%+07.1f", G->dV_LVLH.x / 0.3048);
			skpTextFmt(skp, 27 * W / 32, 14 * H / 28, "%+07.1f", G->dV_LVLH.y / 0.3048);
			skpTextFmt(skp, 27 * W / 32, 16 * H / 28, "%+07.1f", G->dV_LVLH.z / 0.3048);
			GET_Display2(Buffer, G->P30TIG);
			skp->Text(27 * W / 32, 18 * H / 28, Buffer, strlen(Buffer));
		}

	}
	else if (screen == 52)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 2 * H / 14, "CMC RETROFIRE EXTERNAL DV UPDATE (360)", 38);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(5 * W / 32, 8 * H / 28, "OID", 3);
		skp->Text(10 * W / 32, 8 * H / 28, "FCT", 3);
		skp->Text(15 * W / 32, 8 * H / 28, "DSKY V71", 8);

		for (int i = 1;i <= 016;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (i + 9) * H / 28, "%o", i);
		}

		skp->Text(10 * W / 32, 10 * H / 28, "INDEX", 5);
		skp->Text(10 * W / 32, 11 * H / 28, "ADD", 3);
		skp->Text(10 * W / 32, 12 * H / 28, "LAT", 3);
		skp->Text(10 * W / 32, 13 * H / 28, "LAT", 3);
		skp->Text(10 * W / 32, 14 * H / 28, "LONG", 4);
		skp->Text(10 * W / 32, 15 * H / 28, "LONG", 4);
		skp->Text(10 * W / 32, 16 * H / 28, "VGX", 3);
		skp->Text(10 * W / 32, 17 * H / 28, "VGX", 3);
		skp->Text(10 * W / 32, 18 * H / 28, "VGY", 3);
		skp->Text(10 * W / 32, 19 * H / 28, "VGY", 3);
		skp->Text(10 * W / 32, 20 * H / 28, "VGZ", 3);
		skp->Text(10 * W / 32, 21 * H / 28, "VGZ", 3);
		skp->Text(10 * W / 32, 22 * H / 28, "TIGN", 4);
		skp->Text(10 * W / 32, 23 * H / 28, "TIGN", 4);

		for (int i = 0;i < 016;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 10) * H / 28, "%05d", GC->rtcc->CZREXTDV.Octals[i]);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(27 * W / 32, 8 * H / 28, "DECIMAL", 7);
		skpTextFmt(skp, 27 * W / 32, 12 * H / 28, "%+.2f°", GC->rtcc->CZREXTDV.Lat);
		skpTextFmt(skp, 27 * W / 32, 14 * H / 28, "%+.2f°", GC->rtcc->CZREXTDV.Lng);
		skpTextFmt(skp, 27 * W / 32, 16 * H / 28, "%+07.1f", GC->rtcc->CZREXTDV.DV.x);
		skpTextFmt(skp, 27 * W / 32, 18 * H / 28, "%+07.1f", GC->rtcc->CZREXTDV.DV.y);
		skpTextFmt(skp, 27 * W / 32, 20 * H / 28, "%+07.1f", GC->rtcc->CZREXTDV.DV.z);
		GET_Display2(Buffer, GC->rtcc->CZREXTDV.GET_TIG);
		skp->Text(27 * W / 32, 22 * H / 28, Buffer, strlen(Buffer));

	}
	else if (screen == 53 || screen == 94)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->SetFont(font2);

		RTCC::REFSMMATUpdateMakeupTableBlock *block;

		if (screen == 53)
		{
			block = &GC->rtcc->CZREFMAT.Block[0];
		}
		else
		{
			block = &GC->rtcc->CZREFMAT.Block[1];
		}

		if (screen == 53)
		{
			skp->Text(15 * W / 16, 1 * H / 14, "266", 3);
			if (block->MatrixType == 2)
			{
				skp->Text(4 * W / 8, 1 * H / 14, "CMC DESIRED REFSMMAT UPDATE", 27);
			}
			else
			{
				skp->Text(4 * W / 8, 1 * H / 14, "CMC REFSMMAT UPDATE", 19);
			}

		}
		else
		{
			skp->Text(15 * W / 16, 1 * H / 14, "265", 3);
			if (block->MatrixType == 2)
			{
				skp->Text(4 * W / 8, 1 * H / 14, "LGC DESIRED REFSMMAT UPDATE", 27);
			}
			else
			{
				skp->Text(4 * W / 8, 1 * H / 14, "LGC REFSMMAT UPDATE", 19);
			}
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(5 * W / 32, 6 * H / 32, "ID:", 3);
		Text(skp, 7 * W / 32, 6 * H / 32, block->MatrixID);

		skp->Text(12 * W / 32, 6 * H / 32, "LOAD:", 5);
		skpTextFmt(skp, 16 * W / 32, 6 * H / 32, "%d", block->UpdateNo);

		skp->Text(5 * W / 32, 8 * H / 32, "OID", 3);
		skp->Text(10 * W / 32, 8 * H / 32, "FCT", 3);
		skp->Text(15 * W / 32, 8 * H / 32, "DSKY V71", 8);

		for (int i = 1;i <= 024;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (i + 9) * H / 32, "%o", i);
		}

		skp->Text(10 * W / 32, 10 * H / 32, "INDEX", 5);
		skp->Text(10 * W / 32, 11 * H / 32, "ADD", 3);
		skp->Text(10 * W / 32, 12 * H / 32, "XIXE", 4);
		skp->Text(10 * W / 32, 13 * H / 32, "XIXE", 4);
		skp->Text(10 * W / 32, 14 * H / 32, "XIYE", 4);
		skp->Text(10 * W / 32, 15 * H / 32, "XIYE", 4);
		skp->Text(10 * W / 32, 16 * H / 32, "XIZE", 4);
		skp->Text(10 * W / 32, 17 * H / 32, "XIZE", 4);
		skp->Text(10 * W / 32, 18 * H / 32, "YIXE", 4);
		skp->Text(10 * W / 32, 19 * H / 32, "YIXE", 4);
		skp->Text(10 * W / 32, 20 * H / 32, "YIYE", 4);
		skp->Text(10 * W / 32, 21 * H / 32, "YIYE", 4);
		skp->Text(10 * W / 32, 22 * H / 32, "YIZE", 4);
		skp->Text(10 * W / 32, 23 * H / 32, "YIZE", 4);
		skp->Text(10 * W / 32, 24 * H / 32, "ZIXE", 4);
		skp->Text(10 * W / 32, 25 * H / 32, "ZIXE", 4);
		skp->Text(10 * W / 32, 26 * H / 32, "ZIYE", 4);
		skp->Text(10 * W / 32, 27 * H / 32, "ZIYE", 4);
		skp->Text(10 * W / 32, 28 * H / 32, "ZIZE", 4);
		skp->Text(10 * W / 32, 29 * H / 32, "ZIZE", 4);

		for (int i = 0;i < 024;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 10) * H / 32, "%05d", block->Octals[i]);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		for (int i = 0;i < 9;i++)
		{
			skpTextFmt(skp, 27 * W / 32, (i * 2 + 12) * H / 32, "%.8lf", block->REFSMMAT.data[i]);
		}

		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		Text(skp, 16 * W / 32, 31 * H / 32, block->error);
	}
	else if (screen == 54)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "Two Impulse Transfer (MED M72)", 30);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->rtcc->med_m72.Table == 1)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Multiple Solution", 17);
		}
		else
		{
			skp->Text(1 * W / 8, 2 * H / 14, "Corrective Combination", 22);
		}

		skpTextFmt(skp, 1 * W / 8, 4 * H / 14, "%d", GC->rtcc->med_m72.Plan);

		ThrusterName(Buffer, GC->rtcc->med_m72.Thruster);
		skp->Text(1 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_m72.UllageDT < 0)
		{
			strcpy(Buffer, "Nominal ullage");
		}
		else
		{
			if (GC->rtcc->med_m72.UllageQuads)
			{
				sprintf_s(Buffer, "4 quads, %.1f s", GC->rtcc->med_m72.UllageDT);
			}
			else
			{
				sprintf_s(Buffer, "2 quads, %.1f s", GC->rtcc->med_m72.UllageDT);
			}
		}
		skp->Text(1 * W / 8, 12 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_m72.Thruster == RTCC_ENGINETYPE_LMDPS)
		{
			skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%.1f s", GC->rtcc->med_m72.TenPercentDT);
			skpTextFmt(skp, 5 * W / 8, 6 * H / 14, "%.3f", GC->rtcc->med_m72.DPSThrustFactor);
		}

		if (GC->rtcc->med_m72.Iteration)
		{
			skp->Text(5 * W / 8, 2 * H / 14, "Iterate", 7);
		}
		else
		{
			skp->Text(5 * W / 8, 2 * H / 14, "Do not iterate", 14);
		}

		if (GC->rtcc->med_m72.TimeFlag)
		{
			skp->Text(5 * W / 8, 8 * H / 14, "Impulsive TIG", 13);
		}
		else
		{
			skp->Text(5 * W / 8, 8 * H / 14, "Optimum TIG", 11);
		}

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_m72.DeleteGET > 0)
			{
				GET_Display(Buffer, GC->rtcc->med_m72.DeleteGET);
				skp->Text(1 * W / 8, 6 * H / 14, Buffer, strlen(Buffer));
			}
			else
			{
				skp->Text(1 * W / 8, 6 * H / 14, "Do not delete", 13);
			}

			const char *attname = MPTAttitudeName(GC->rtcc->med_m72.Attitude);
			skp->Text(1 * W / 8, 10 * H / 14, attname, strlen(attname));
		}
		else
		{
			GET_Display2(Buffer, G->P30TIG);
			skp->Text(5 * W / 8, 15 * H / 21, Buffer, strlen(Buffer));

			skp->Text(5 * W / 8, 16 * H / 21, "DVX", 3);
			skp->Text(5 * W / 8, 17 * H / 21, "DVY", 3);
			skp->Text(5 * W / 8, 18 * H / 21, "DVZ", 3);

			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(6 * W / 8, 16 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(6 * W / 8, 17 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(6 * W / 8, 18 * H / 21, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 55)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		if (GC->rtcc->med_m70.Plan == 0)
		{
			skp->Text(4 * W / 8, 1 * H / 14, "SPQ Transfer (MED M70)", 22);
		}
		else if (GC->rtcc->med_m70.Plan == 1)
		{
			skp->Text(4 * W / 8, 1 * H / 14, "DKI Transfer (MED M70)", 22);
		}
		else
		{
			skp->Text(4 * W / 8, 1 * H / 14, "Descent Plan Transfer (MED M70)", 31);
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_m70.DeleteGET > 0)
			{
				GET_Display(Buffer, GC->rtcc->med_m70.DeleteGET);
				skp->Text(1 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
			}
			else
			{
				skp->Text(1 * W / 8, 2 * H / 14, "Do not delete", 13);
			}
		}

		ThrusterName(Buffer, GC->rtcc->med_m70.Thruster);
		skp->Text(1 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));

		if (GC->MissionPlanningActive)
		{
			const char* attname = MPTAttitudeName(GC->rtcc->med_m70.Attitude);
			skp->Text(1 * W / 8, 6 * H / 14, attname, strlen(attname));
		}

		if (GC->rtcc->med_m70.UllageDT < 0)
		{
			strcpy(Buffer, "Nominal ullage");
		}
		else
		{
			if (GC->rtcc->med_m70.UllageQuads)
			{
				sprintf_s(Buffer, "4 quads, %.1f s", GC->rtcc->med_m70.UllageDT);
			}
			else
			{
				sprintf_s(Buffer, "2 quads, %.1f s", GC->rtcc->med_m70.UllageDT);
			}
		}
		skp->Text(1 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_m70.Iteration)
		{
			skp->Text(1 * W / 8, 10 * H / 14, "Iterate", 7);
		}
		else
		{
			skp->Text(1 * W / 8, 10 * H / 14, "Do not iterate", 14);
		}

		if (GC->rtcc->med_m70.TimeFlag)
		{
			skp->Text(1 * W / 8, 12 * H / 14, "Impulsive TIG", 13);
		}
		else
		{
			skp->Text(1 * W / 8, 12 * H / 14, "Optimum TIG", 11);
		}

		if (GC->rtcc->med_m70.Thruster == RTCC_ENGINETYPE_LMDPS)
		{
			skpTextFmt(skp, 5 * W / 8, 2 * H / 14, "%.1f s", GC->rtcc->med_m70.TenPercentDT);
			skpTextFmt(skp, 5 * W / 8, 4 * H / 14, "%.3f", GC->rtcc->med_m70.DPSThrustFactor);
		}

		if (GC->MissionPlanningActive == false)
		{
			GET_Display(Buffer, G->P30TIG);
			skp->Text(5 * W / 8, 10 * H / 21, Buffer, strlen(Buffer));

			skp->Text(5 * W / 8, 11 * H / 21, "DVX", 3);
			skp->Text(5 * W / 8, 12 * H / 21, "DVY", 3);
			skp->Text(5 * W / 8, 13 * H / 21, "DVZ", 3);

			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(6 * W / 8, 11 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(6 * W / 8, 12 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(6 * W / 8, 13 * H / 21, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 56)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "DIRECT INPUT TO MPT (MED M66)", 29);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->rtcc->med_m66.Table == RTCC_MPT_CSM)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "CSM", 3);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "LM", 2);
		}

		if (GC->rtcc->med_m66.ReplaceCode == 0)
		{
			skp->Text(1 * W / 16, 4 * H / 14, "Don't replace", 13);
		}
		else
		{
			skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%d", GC->rtcc->med_m66.ReplaceCode);
		}

		GET_Display(Buffer, GC->rtcc->med_m66.GETBI);
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		ThrusterName(Buffer, GC->rtcc->med_m66.Thruster);
		skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

		const char* attname = MPTAttitudeName(GC->rtcc->med_m66.AttitudeOpt);
		skp->Text(1 * W / 16, 10 * H / 14, attname, strlen(attname));

		if (GC->rtcc->med_m66.BurnParamNo == 1)
		{
			char Buffer2[4];
			skp->Text(1 * W / 16, 12 * H / 14, "P1: DV or DT", 16);
			if (GC->rtcc->PZBURN.P1_DVIND == 0)
			{
				sprintf(Buffer2, "MAG");
			}
			else if (GC->rtcc->PZBURN.P1_DVIND == 1)
			{
				sprintf(Buffer2, "DVC");
			}
			else
			{
				sprintf(Buffer2, "XBT");
			}
			skpTextFmt(skp, 1 * W / 16, 13 * H / 14, "%.1f ft/s, %s, %.1f s", GC->rtcc->PZBURN.P1_DV / 0.3048, Buffer2, GC->rtcc->PZBURN.P1_DT);
		}
		else if (GC->rtcc->med_m66.BurnParamNo == 2)
		{
			skp->Text(1 * W / 16, 12 * H / 14, "P2: DV Vector (AGC)", 19);

			skpTextFmt(skp, 1 * W / 16, 13 * H / 14, "%+.1f %+.1f %+.1f", GC->rtcc->PZBURN.P2_DV.x / 0.3048, GC->rtcc->PZBURN.P2_DV.y / 0.3048, GC->rtcc->PZBURN.P2_DV.z / 0.3048);
		}
		else if (GC->rtcc->med_m66.BurnParamNo == 3)
		{
			skp->Text(1 * W / 16, 12 * H / 14, "P3: DV Vector (IMU)", 19);

			skpTextFmt(skp, 1 * W / 16, 13 * H / 14, "%+.1f %+.1f %+.1f", GC->rtcc->PZBURN.P3_DV.x / 0.3048, GC->rtcc->PZBURN.P3_DV.y / 0.3048, GC->rtcc->PZBURN.P3_DV.z / 0.3048);
		}
		else if (GC->rtcc->med_m66.BurnParamNo == 4)
		{
			skp->Text(1 * W / 16, 12 * H / 14, "P4: DV Vector (LVLH)", 20);

			skpTextFmt(skp, 1 * W / 16, 13 * H / 14, "%+.1f %+.1f %+.1f", GC->rtcc->PZBURN.P4_DV.x / 0.3048, GC->rtcc->PZBURN.P4_DV.y / 0.3048, GC->rtcc->PZBURN.P4_DV.z / 0.3048);
		}
		else if (GC->rtcc->med_m66.BurnParamNo == 5)
		{
			skp->Text(1 * W / 16, 12 * H / 14, "P5: Lambert", 11);
		}

		if (GC->rtcc->med_m66.BurnParamNo == 1)
		{
			if (GC->rtcc->med_m66.CoordInd == 0)
			{
				skp->Text(7 * W / 16, 2 * H / 14, "LVLH", 4);
			}
			else if (GC->rtcc->med_m66.CoordInd == 1)
			{
				skp->Text(7 * W / 16, 2 * H / 14, "IMU", 4);
			}
			else
			{
				skp->Text(7 * W / 16, 2 * H / 14, "FDAI", 4);
			}
			skpTextFmt(skp, 7 * W / 16, 3 * H / 14, "%06.2f°P %06.2f°Y %06.2f°R", GC->rtcc->med_m66.Att.x* DEG, GC->rtcc->med_m66.Att.y* DEG, GC->rtcc->med_m66.Att.z* DEG);
		}

		if (GC->rtcc->med_m66.AttitudeOpt >= 3)
		{
			if (GC->rtcc->med_m66.HeadsUp)
			{
				skp->Text(10 * W / 16, 4 * H / 14, "Heads Up", 8);
			}
			else
			{
				skp->Text(10 * W / 16, 4 * H / 14, "Heads Down", 10);
			}
		}

		if (GC->rtcc->med_m66.Thruster == RTCC_ENGINETYPE_LMDPS)
		{
			skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%.1lf s", GC->rtcc->med_m66.TenPercentDT);
			skpTextFmt(skp, 10 * W / 16, 8 * H / 14, "%.3lf", GC->rtcc->med_m66.DPSThrustFactor);
		}

		skp->Text(10 * W / 16, 10 * H / 14, "Page 1/2", 8);
	}
	else if (screen == 57)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "GPM Transfer (MED M65)", 22);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_m65.Table == 1)
			{
				skp->Text(1 * W / 16, 2 * H / 14, "CSM", 3);
			}
			else
			{
				skp->Text(1 * W / 16, 2 * H / 14, "LEM", 3);
			}

			if (GC->rtcc->med_m65.ReplaceCode == 0)
			{
				skp->Text(1 * W / 16, 4 * H / 14, "Don't replace", 13);
			}
			else
			{
				skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%d", GC->rtcc->med_m65.ReplaceCode);
			}
		}

		ThrusterName(Buffer, GC->rtcc->med_m65.Thruster);
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		if (GC->MissionPlanningActive)
		{
			const char* attname = MPTAttitudeName(GC->rtcc->med_m65.Attitude);
			skp->Text(1 * W / 16, 8 * H / 14, attname, strlen(attname));

			if (GC->rtcc->med_m65.UllageDT < 0)
			{
				strcpy(Buffer, "Nominal ullage");
			}
			else
			{
				sprintf_s(Buffer, "%lf s", GC->rtcc->med_m65.UllageDT);
			}
			skp->Text(1 * W / 16, 10 * H / 14, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m65.UllageQuads)
			{
				skp->Text(1 * W / 16, 12 * H / 14, "4 Thrusters", 11);
			}
			else
			{
				skp->Text(1 * W / 16, 12 * H / 14, "2 Thrusters", 11);
			}

			if (GC->rtcc->med_m65.Iteration)
			{
				skp->Text(10 * W / 16, 2 * H / 14, "Iterate", 7);
			}
			else
			{
				skp->Text(10 * W / 16, 2 * H / 14, "Don't iterate", 13);
			}

			if (GC->rtcc->med_m65.Thruster == RTCC_ENGINETYPE_LMDPS)
			{
				skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%lf s", GC->rtcc->med_m65.TenPercentDT);
				skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%lf", GC->rtcc->med_m65.DPSThrustFactor);
			}

			if (GC->rtcc->med_m65.TimeFlag)
			{
				skp->Text(10 * W / 16, 8 * H / 14, "Impulsive TIG", 13);
			}
			else
			{
				skp->Text(10 * W / 16, 8 * H / 14, "Optimum TIG", 11);
			}
		}

		if (GC->MissionPlanningActive == false)
		{
			GET_Display(Buffer, G->P30TIG);
			skp->Text(5 * W / 8, 17 * H / 21, Buffer, strlen(Buffer));

			skp->Text(5 * W / 8, 18 * H / 21, "DVX", 3);
			skp->Text(5 * W / 8, 19 * H / 21, "DVY", 3);
			skp->Text(5 * W / 8, 20 * H / 21, "DVZ", 3);

			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(6 * W / 8, 18 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(6 * W / 8, 19 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(6 * W / 8, 20 * H / 21, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 58)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, (int)(0.5 * H / 14), "CHECKOUT MONITOR", 16);

		skp->SetFont(font2);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 32, 3 * H / 28, "GET", 3);
		GET_Display(Buffer, GC->rtcc->EZCHECKDIS.GET, false);
		skp->Text(4 * W / 32, 3 * H / 28, Buffer, strlen(Buffer));

		skp->Text(1 * W / 32, 4 * H / 28, "GMT", 3);
		GET_Display(Buffer, GC->rtcc->EZCHECKDIS.GMT, false);
		skp->Text(4 * W / 32, 4 * H / 28, Buffer, strlen(Buffer));

		skp->Text(1 * W / 32, 5 * H / 28, "VEH", 3);
		skp->Text(4 * W / 32, 5 * H / 28, GC->rtcc->EZCHECKDIS.VEH, strlen(GC->rtcc->EZCHECKDIS.VEH));

		skp->Text(9 * W / 32, 3 * H / 28, "R-DAY", 5);
		skpTextFmt(skp, 13 * W / 32, 3 * H / 28, "%02d:%02d:%04d", GC->rtcc->EZCHECKDIS.R_Day[0], GC->rtcc->EZCHECKDIS.R_Day[1], GC->rtcc->EZCHECKDIS.R_Day[2]);
		skp->Text(9 * W / 32, 4 * H / 28, "VID", 3);
		Text(skp, 11 * W / 32, 4 * H / 28, GC->rtcc->EZCHECKDIS.VID);
		skp->Text(7 * W / 32, 5 * H / 28, "XT", 2);
		skp->Text(19 * W / 32, 3 * H / 28, "K-FAC", 5);
		skp->Text(16 * W / 32, 4 * H / 28, "RF", 2);
		skp->Text(18 * W / 32, 4 * H / 28, GC->rtcc->EZCHECKDIS.RF, 3);
		skp->Text(15 * W / 32, 5 * H / 28, "YT", 2);
		skp->Text(21 * W / 32, 4 * H / 28, "A", 1);

		skp->Text(26 * W / 32, 4 * H / 28, "CFG", 3);
		skp->Text(23 * W / 32, 5 * H / 28, "ZT", 2);
		skp->Text(24 * W / 32, 6 * H / 28, "OPTION", 6);
		skp->Text(26 * W / 32, 7 * H / 28, "NV", 2);
		if (GC->rtcc->EZCHECKDIS.NV > 0)
		{
			skpTextFmt(skp, 29 * W / 32, 7 * H / 28, "%d", GC->rtcc->EZCHECKDIS.NV);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skpTextFmt(skp, 25 * W / 32, 3 * H / 28, "%.2f", GC->rtcc->EZCHECKDIS.K_Fac);
		skpTextFmt(skp, 25 * W / 32, 4 * H / 28, "%.2f", GC->rtcc->EZCHECKDIS.A);

		if (GC->rtcc->EZCHECKDIS.U_T.x > -1)
		{
			skpTextFmt(skp, 13 * W / 32, 5 * H / 28, "%0.5f", GC->rtcc->EZCHECKDIS.U_T.x);
			skpTextFmt(skp, 21 * W / 32, 5 * H / 28, "%0.5f", GC->rtcc->EZCHECKDIS.U_T.y);
			skpTextFmt(skp, 29 * W / 32, 5 * H / 28, "%0.5f", GC->rtcc->EZCHECKDIS.U_T.z);
		}

		skp->Text(30 * W / 32, 4 * H / 28, GC->rtcc->EZCHECKDIS.CFG, strlen(GC->rtcc->EZCHECKDIS.CFG));
		skp->Text(30 * W / 32, 6 * H / 28, GC->rtcc->EZCHECKDIS.Option, 3);

		skp->Text(2 * W / 32, 7 * H / 28, "X", 1);
		skp->Text(2 * W / 32, 9 * H / 28, "Y", 1);
		skp->Text(2 * W / 32, 11 * H / 28, "Z", 1);
		skp->Text(2 * W / 32, 13 * H / 28, "DX", 2);
		skp->Text(2 * W / 32, 15 * H / 28, "DY", 2);
		skp->Text(2 * W / 32, 17 * H / 28, "DZ", 2);

		if (GC->rtcc->EZCHECKDIS.unit == 0)
		{
			skpTextFmt(skp, 10 * W / 32, 7 * H / 28, "%+013.9f", GC->rtcc->EZCHECKDIS.Pos.x);
			skpTextFmt(skp, 10 * W / 32, 9 * H / 28, "%+013.9f", GC->rtcc->EZCHECKDIS.Pos.y);
			skpTextFmt(skp, 10 * W / 32, 11 * H / 28, "%+013.9f", GC->rtcc->EZCHECKDIS.Pos.z);
			skpTextFmt(skp, 10 * W / 32, 13 * H / 28, "%+013.9f", GC->rtcc->EZCHECKDIS.Vel.x);
			skpTextFmt(skp, 10 * W / 32, 15 * H / 28, "%+013.9f", GC->rtcc->EZCHECKDIS.Vel.y);
			skpTextFmt(skp, 10 * W / 32, 17 * H / 28, "%+013.9f", GC->rtcc->EZCHECKDIS.Vel.z);
		}
		else
		{
			skpTextFmt(skp, 10 * W / 32, 7 * H / 28, "%+012.0f", GC->rtcc->EZCHECKDIS.Pos.x);
			skpTextFmt(skp, 10 * W / 32, 9 * H / 28, "%+012.0f", GC->rtcc->EZCHECKDIS.Pos.y);
			skpTextFmt(skp, 10 * W / 32, 11 * H / 28, "%+012.0f", GC->rtcc->EZCHECKDIS.Pos.z);
			skpTextFmt(skp, 10 * W / 32, 13 * H / 28, "%+013.6f", GC->rtcc->EZCHECKDIS.Vel.x);
			skpTextFmt(skp, 10 * W / 32, 15 * H / 28, "%+013.6f", GC->rtcc->EZCHECKDIS.Vel.y);
			skpTextFmt(skp, 10 * W / 32, 17 * H / 28, "%+013.6f", GC->rtcc->EZCHECKDIS.Vel.z);
		}

		skp->Text(4 * W / 32, 19 * H / 28, "LO/C", 4);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.LOC);
		skp->Text(10 * W / 32, 19 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 20 * H / 28, "GRR/C", 5);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.GRRC);
		skp->Text(10 * W / 32, 20 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 21 * H / 28, "ZS/C", 4);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.ZSC);
		skp->Text(10 * W / 32, 21 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 22 * H / 28, "GRR/S", 5);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.GRRS);
		skp->Text(10 * W / 32, 22 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 23 * H / 28, "ZS/L", 4);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.ZSL);
		skp->Text(10 * W / 32, 23 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 24 * H / 28, "ZS/A", 4);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.ZSA);
		skp->Text(10 * W / 32, 24 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 25 * H / 28, "EPHB", 4);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.EPHB);
		skp->Text(10 * W / 32, 25 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 26 * H / 28, "EPHE", 4);
		GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.EPHE);
		skp->Text(10 * W / 32, 26 * H / 28, Buffer, strlen(Buffer));
		skp->Text(4 * W / 32, 27 * H / 28, "THT", 4);
		if (GC->rtcc->EZCHECKDIS.THT > 0)
		{
			GET_Display2(Buffer, GC->rtcc->EZCHECKDIS.THT);
			skp->Text(10 * W / 32, 27 * H / 28, Buffer, strlen(Buffer));
		}

		skp->Text(13 * W / 32, 7 * H / 28, "V", 1);
		skpTextFmt(skp, 17 * W / 32, 7 * H / 28, "%07.1f", GC->rtcc->EZCHECKDIS.V_i);
		skp->Text(13 * W / 32, 9 * H / 28, "PTH", 3);
		skpTextFmt(skp, 17 * W / 32, 9 * H / 28, "%+07.3f", GC->rtcc->EZCHECKDIS.gamma_i);
		skp->Text(13 * W / 32, 11 * H / 28, "AZ", 2);
		skpTextFmt(skp, 17 * W / 32, 11 * H / 28, "%+07.3f", GC->rtcc->EZCHECKDIS.psi);
		skp->Text(13 * W / 32, 13 * H / 28, "LATC", 4);
		skpTextFmt(skp, 17 * W / 32, 13 * H / 28, "%+07.4f", GC->rtcc->EZCHECKDIS.phi_c);
		skp->Text(13 * W / 32, 15 * H / 28, "LONC", 4);
		skpTextFmt(skp, 17 * W / 32, 15 * H / 28, "%+07.3f", GC->rtcc->EZCHECKDIS.lambda);
		skp->Text(12 * W / 32, 17 * H / 28, "HS", 2);
		skpTextFmt(skp, 17 * W / 32, 17 * H / 28, "%07.2f", GC->rtcc->EZCHECKDIS.h_s);

		skp->Text(19 * W / 32, 7 * H / 28, "A", 1);
		skpTextFmt(skp, 24 * W / 32, 7 * H / 28, "%08.2f", GC->rtcc->EZCHECKDIS.a);
		skp->Text(20 * W / 32, 9 * H / 28, "E", 1);
		skpTextFmt(skp, 24 * W / 32, 9 * H / 28, "%07.5f", GC->rtcc->EZCHECKDIS.e);
		skp->Text(20 * W / 32, 11 * H / 28, "I", 1);
		skpTextFmt(skp, 24 * W / 32, 11 * H / 28, "%07.3f", GC->rtcc->EZCHECKDIS.i);
		skp->Text(20 * W / 32, 13 * H / 28, "AP", 2);
		skpTextFmt(skp, 24 * W / 32, 13 * H / 28, "%07.3f", GC->rtcc->EZCHECKDIS.omega_p);
		skp->Text(20 * W / 32, 15 * H / 28, "RAN", 3);
		skpTextFmt(skp, 24 * W / 32, 15 * H / 28, "%07.3f", GC->rtcc->EZCHECKDIS.Omega);
		skp->Text(20 * W / 32, 17 * H / 28, "TA", 2);
		skp->Text(21 * W / 32, 19 * H / 28, "MA", 2);
		if (GC->rtcc->EZCHECKDIS.TABlank == false)
		{
			skpTextFmt(skp, 24 * W / 32, 17 * H / 28, "%07.3f", GC->rtcc->EZCHECKDIS.nu);
		}
		if (GC->rtcc->EZCHECKDIS.MABlank == false)
		{
			skpTextFmt(skp, 25 * W / 32, 19 * H / 28, "%07.3f", GC->rtcc->EZCHECKDIS.m);
		}
		skp->Text(27 * W / 32, 9 * H / 28, "WT", 2);
		skpTextFmt(skp, 31 * W / 32, 9 * H / 28, "%06.0f", GC->rtcc->EZCHECKDIS.WT);
		skp->Text(27 * W / 32, 10 * H / 28, "WC", 2);
		skpTextFmt(skp, 31 * W / 32, 10 * H / 28, "%05.0f", GC->rtcc->EZCHECKDIS.WC);
		skp->Text(27 * W / 32, 11 * H / 28, "SPS", 3);
		if (GC->rtcc->EZCHECKDIS.SPS >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 11 * H / 28, "%05.0f", GC->rtcc->EZCHECKDIS.SPS);
		}
		skp->Text(27 * W / 32, 12 * H / 28, "RCSC", 4);
		if (GC->rtcc->EZCHECKDIS.RCS_C >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 12 * H / 28, "%04.0f", GC->rtcc->EZCHECKDIS.RCS_C);
		}
		skp->Text(27 * W / 32, 13 * H / 28, "WL", 2);
		if (GC->rtcc->EZCHECKDIS.WL >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 13 * H / 28, "%05.0f", GC->rtcc->EZCHECKDIS.WL);
		}
		skp->Text(27 * W / 32, 14 * H / 28, "APS", 3);
		if (GC->rtcc->EZCHECKDIS.APS >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 14 * H / 28, "%05.0f", GC->rtcc->EZCHECKDIS.APS);
		}
		skp->Text(27 * W / 32, 15 * H / 28, "DPS", 3);
		if (GC->rtcc->EZCHECKDIS.DPS >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 15 * H / 28, "%05.0f", GC->rtcc->EZCHECKDIS.DPS);
		}
		skp->Text(27 * W / 32, 16 * H / 28, "RCSL", 4);
		if (GC->rtcc->EZCHECKDIS.RCS_L >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 16 * H / 28, "%04.0f", GC->rtcc->EZCHECKDIS.RCS_L);
		}
		skp->Text(27 * W / 32, 17 * H / 28, "J2", 2);
		if (GC->rtcc->EZCHECKDIS.J2 >= 0)
		{
			skpTextFmt(skp, 31 * W / 32, 17 * H / 28, "%05.0f", GC->rtcc->EZCHECKDIS.J2);
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(11 * W / 32, 19 * H / 28, "HA", 2);
		skpTextFmt(skp, 15 * W / 32, 19 * H / 28, "%07.2f", GC->rtcc->EZCHECKDIS.h_a);
		skp->Text(11 * W / 32, 20 * H / 28, "HP", 2);
		skpTextFmt(skp, 15 * W / 32, 20 * H / 28, "%07.2f", GC->rtcc->EZCHECKDIS.h_p);
		skp->Text(11 * W / 32, 21 * H / 28, "HO", 2);
		skp->Text(11 * W / 32, 22 * H / 28, "HO", 2);
		if (GC->rtcc->EZCHECKDIS.HOBlank == false)
		{
			skpTextFmt(skp, 15 * W / 32, 21 * H / 28, "%07.2f", GC->rtcc->EZCHECKDIS.h_o_NM);
			skpTextFmt(skp, 15 * W / 32, 22 * H / 28, "%07.0f", GC->rtcc->EZCHECKDIS.h_o_ft);
		}
		skp->Text(11 * W / 32, 23 * H / 28, "LATD", 4);
		skpTextFmt(skp, 15 * W / 32, 23 * H / 28, "%+07.3f", GC->rtcc->EZCHECKDIS.phi_D);
		skp->Text(11 * W / 32, 24 * H / 28, "LOND", 4);
		skpTextFmt(skp, 15 * W / 32, 24 * H / 28, "%07.3f", GC->rtcc->EZCHECKDIS.lambda_D);
		skp->Text(11 * W / 32, 25 * H / 28, "R", 1);
		skpTextFmt(skp, 15 * W / 32, 25 * H / 28, "%+09.2f", GC->rtcc->EZCHECKDIS.R);
		skp->Text(11 * W / 32, 26 * H / 28, "DECL", 4);
		skpTextFmt(skp, 15 * W / 32, 26 * H / 28, "%+07.3f", GC->rtcc->EZCHECKDIS.deltaL);
		skp->Text(11 * W / 32, 27 * H / 28, "LSB", 3);
		skp->Text(20 * W / 32, 27 * H / 28, "LLS", 3);
		if (GC->rtcc->EZCHECKDIS.LSTBlank == false)
		{
			GET_Display(Buffer, GC->rtcc->EZCHECKDIS.LAL, false);
			skp->Text(15 * W / 32, 27 * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->EZCHECKDIS.LOL, false);
			skp->Text(24 * W / 32, 27 * H / 28, Buffer, strlen(Buffer));
		}

		Text(skp, 28 * W / 32, 24 * H / 28, GC->rtcc->EZCHECKDIS.ErrorMessage);
	}
	else if (screen == 59)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(5 * W / 8, 1 * H / 28, "MPT INITIALIZATION", 18);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 16, (marker + 4) * H / 16, "*", 1);

		skp->Text(10 * W / 16, 5 * H / 16, "MPT:", 4);

		if (G->mptinitmode == 0)
		{
			skp->Text(2 * W / 16, 4 * H / 16, "MED M49: Fuel Remaining", 23);

			if (GC->rtcc->med_m49.Table == RTCC_MPT_LM)
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: LM", 9);
			}
			else
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: CSM", 10);
			}

			if (GC->rtcc->med_m49.SPSFuelRemaining >= 0)
			{
				sprintf(Buffer, "SPS: %.0f lbs", GC->rtcc->med_m49.SPSFuelRemaining / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "SPS: No Update");
			}
			skp->Text(2 * W / 16, 6 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m49.CSMRCSFuelRemaining >= 0)
			{
				sprintf(Buffer, "CSM RCS: %.0f lbs", GC->rtcc->med_m49.CSMRCSFuelRemaining / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "CSM RCS: No Update");
			}
			skp->Text(2 * W / 16, 7 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m49.SIVBFuelRemaining >= 0)
			{
				sprintf(Buffer, "S-IVB: %.0f lbs", GC->rtcc->med_m49.SIVBFuelRemaining / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "S-IVB: No Update");
			}
			skp->Text(2 * W / 16, 8 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m49.LMAPSFuelRemaining >= 0)
			{
				sprintf(Buffer, "LM APS: %.0f lbs", GC->rtcc->med_m49.LMAPSFuelRemaining / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "LM APS: No Update");
			}
			skp->Text(2 * W / 16, 9 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m49.LMRCSFuelRemaining >= 0)
			{
				sprintf(Buffer, "LM RCS: %.0f lbs", GC->rtcc->med_m49.LMRCSFuelRemaining / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "LM RCS: No Update");
			}
			skp->Text(2 * W / 16, 10 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m49.LMDPSFuelRemaining >= 0)
			{
				sprintf(Buffer, "LM DPS: %.0f lbs", GC->rtcc->med_m49.LMDPSFuelRemaining / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "LM DPS: No Update");
			}
			skp->Text(2 * W / 16, 11 * H / 16, Buffer, strlen(Buffer));

			MissionPlanTable *mpt = GC->rtcc->GetMPTPointer(GC->rtcc->med_m49.Table);

			skpTextFmt(skp, 10 * W / 16, 6 * H / 16, "%.0f lbs", mpt->CommonBlock.SPSFuelRemaining / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 7 * H / 16, "%.0f lbs", mpt->CommonBlock.CSMRCSFuelRemaining / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 8 * H / 16, "%.0f lbs", mpt->CommonBlock.SIVBFuelRemaining / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 9 * H / 16, "%.0f lbs", mpt->CommonBlock.LMAPSFuelRemaining / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 10 * H / 16, "%.0f lbs", mpt->CommonBlock.LMRCSFuelRemaining / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 11 * H / 16, "%.0f lbs", mpt->CommonBlock.LMDPSFuelRemaining / 0.45359237);
		}
		else if (G->mptinitmode == 1)
		{
			skp->Text(2 * W / 16, 4 * H / 16, "MED M50: Weights", 16);

			if (GC->rtcc->med_m50.Table == RTCC_MPT_LM)
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: LM", 9);
			}
			else
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: CSM", 10);
			}

			if (GC->rtcc->med_m50.CSMWT >= 0)
			{
				sprintf(Buffer, "CSM: %.0f lbs", GC->rtcc->med_m50.CSMWT / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "CSM: No Update");
			}
			skp->Text(2 * W / 16, 6 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m50.SIVBWT >= 0)
			{
				sprintf(Buffer, "S-IVB: %.0f lbs", GC->rtcc->med_m50.SIVBWT / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "S-IVB: No Update");
			}
			skp->Text(2 * W / 16, 7 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m50.LMWT >= 0)
			{
				sprintf(Buffer, "LM Total: %.0f lbs", GC->rtcc->med_m50.LMWT / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "LM Total: No Update");
			}
			skp->Text(2 * W / 16, 8 * H / 16, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m50.LMASCWT >= 0)
			{
				sprintf(Buffer, "LM Ascent: %.0f lbs", GC->rtcc->med_m50.LMASCWT / 0.45359237);
			}
			else
			{
				sprintf(Buffer, "LM Ascent: No Update");
			}
			skp->Text(2 * W / 16, 9 * H / 16, Buffer, strlen(Buffer));

			GET_Display(Buffer, GC->rtcc->med_m50.WeightGET, true);
			skp->Text(2 * W / 16, 10 * H / 16, Buffer, strlen(Buffer));

			MissionPlanTable *mpt = GC->rtcc->GetMPTPointer(GC->rtcc->med_m50.Table);

			skpTextFmt(skp, 10 * W / 16, 6 * H / 16, "CSM %.0f lbs", mpt->CommonBlock.CSMMass / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 7 * H / 16, "S-IVB %.0f lbs", mpt->CommonBlock.SIVBMass / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 8 * H / 16, "LM ASC %.0f lbs", mpt->CommonBlock.LMAscentMass / 0.45359237);
			skpTextFmt(skp, 10 * W / 16, 9 * H / 16, "LM DSC %.0f lbs", mpt->CommonBlock.LMDescentMass / 0.45359237);

			skpTextFmt(skp, 10 * W / 16, 12 * H / 16, "Total %.0f lbs", mpt->TotalInitMass / 0.45359237);
		}
		else if (G->mptinitmode == 2)
		{
			skp->Text(2 * W / 16, 4 * H / 16, "MED M51: Areas", 14);

			if (GC->rtcc->med_m51.Table == RTCC_MPT_LM)
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: LM", 9);
			}
			else
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: CSM", 10);
			}

			skpTextFmt(skp, 2 * W / 16, 6 * H / 16, "CSM %.2f sq ft", GC->rtcc->med_m51.CSMArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 2 * W / 16, 7 * H / 16, "S-IVB %.2f sq ft", GC->rtcc->med_m51.SIVBArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 2 * W / 16, 8 * H / 16, "LM ASC %.2f sq ft", GC->rtcc->med_m51.LMAscentArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 2 * W / 16, 9 * H / 16, "LM DSC %.2f sq ft", GC->rtcc->med_m51.LMDescentArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 2 * W / 16, 10 * H / 16, "K-Factor %.2f", GC->rtcc->med_m51.KFactor);

			MissionPlanTable *mpt = GC->rtcc->GetMPTPointer(GC->rtcc->med_m51.Table);

			skpTextFmt(skp, 10 * W / 16, 6 * H / 16, "%.2f sq ft", mpt->CommonBlock.CSMArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 10 * W / 16, 7 * H / 16, "%.2f sq ft", mpt->CommonBlock.SIVBArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 10 * W / 16, 8 * H / 16, "%.2f sq ft", mpt->CommonBlock.LMAscentArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 10 * W / 16, 9 * H / 16, "%.2f sq ft", mpt->CommonBlock.LMDescentArea / 0.3048 / 0.3048);
			skpTextFmt(skp, 10 * W / 16, 10 * H / 16, "%.2f", mpt->KFactor);

			skpTextFmt(skp, 10 * W / 16, 12 * H / 16, "Total %.2f sq ft", mpt->ConfigurationArea / 0.3048 / 0.3048);
		}
		else
		{
			skp->Text(2 * W / 16, 4 * H / 16, "MED M55: Configuration", 22);

			if (GC->rtcc->med_m55.Table == RTCC_MPT_LM)
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: LM", 9);
			}
			else
			{
				skp->Text(2 * W / 16, 5 * H / 16, "Table: CSM", 10);
			}

			if (GC->rtcc->med_m55.ConfigCode == "")
			{
				sprintf(Buffer, "Config: None");
			}
			else
			{
				sprintf(Buffer, "Config: %s", GC->rtcc->med_m55.ConfigCode.c_str());
			}
			skp->Text(2 * W / 16, 6 * H / 16, Buffer, strlen(Buffer));

			sprintf(Buffer, "T BV:");
			skp->Text(2 * W / 16, 7 * H / 16, Buffer, strlen(Buffer));
			if (GC->rtcc->med_m55.VentingGET >= 0)
			{
				GET_Display(Buffer, GC->rtcc->med_m55.VentingGET);
			}
			else
			{
				sprintf(Buffer, "No Update");
			}
			skp->Text(4 * W / 16, 7 * H / 16, Buffer, strlen(Buffer));

			sprintf(Buffer, "DDA:");
			skp->Text(2 * W / 16, 8 * H / 16, Buffer, strlen(Buffer));
			if (GC->rtcc->med_m55.DeltaDockingAngle >= 0)
			{
				sprintf(Buffer, "%.2f°", GC->rtcc->med_m55.DeltaDockingAngle*DEG);
			}
			else
			{
				sprintf(Buffer, "No Update");
			}
			skp->Text(4 * W / 16, 8 * H / 16, Buffer, strlen(Buffer));

			MissionPlanTable *mpt = GC->rtcc->GetMPTPointer(GC->rtcc->med_m55.Table);

			const char* MPTstr;
			if (mpt->CommonBlock.ConfigCode == 0)
			{
				MPTstr = "None";
			}
			else
			{
				MPTstr = GC->rtcc->MPTGetStringFromConfig(mpt->CommonBlock.ConfigCode);
			}
			skp->Text(10 * W / 16, 6 * H / 16, MPTstr, strlen(MPTstr));

			GET_Display(Buffer, mpt->SIVBVentingBeginGET);
			skp->Text(10 * W / 16, 7 * H / 16, Buffer, strlen(Buffer));

			skpTextFmt(skp, 10 * W / 16, 8 * H / 16, "%.2f°", mpt->DeltaDockingAngle* DEG);
		}

		if (GC->pMPTVessel != NULL)
		{
			sprintf(Buffer, "Vessel: %s", GC->pMPTVessel->GetName());
		}
		else
		{
			sprintf(Buffer, "Vessel: None");
		}
		skp->Text(9 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

		if (GC->mptInitError == 1)
		{
			skp->Text(7 * W / 16, 13 * H / 14, "Update successful!", 18);
		}
		else if (GC->mptInitError == 2)
		{
			skp->Text(7 * W / 16, 13 * H / 14, "Update rejected!", 16);
		}
	}
	else if (screen == 60)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 28, "LM DESCENT PLANNING (MSK 0086)", 30);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->SetFont(font2);

		skp->Text(1 * W / 32, 3 * H / 28, "STA ID", 6);
		skp->Text(1 * W / 32, 4 * H / 28, "LM WT", 5);
		skpTextFmt(skp, 5 * W / 32, 4 * H / 28, "%07.1f", GC->rtcc->PZLDPDIS.LMWT);
		skp->Text(9 * W / 32, 3 * H / 28, "GMTV", 4);
		skp->Text(9 * W / 32, 4 * H / 28, "GETV", 4);
		skp->Text(17 * W / 32, 3 * H / 28, "MODE", 4);
		skpTextFmt(skp, 21 * W / 32, 3 * H / 28, "%d", GC->rtcc->PZLDPDIS.MODE);
		skp->Text(17 * W / 32, 4 * H / 28, "TL", 2);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		GET_Display(Buffer, GC->rtcc->PZLDPDIS.GMTV, false);
		skp->Text(16 * W / 32, 3 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->PZLDPDIS.GETV, false);
		skp->Text(16 * W / 32, 4 * H / 28, Buffer, strlen(Buffer));

		skp->Text(26 * W / 32, 3 * H / 28, "LAT LLS", 7);
		skp->Text(26 * W / 32, 4 * H / 28, "LONG LLS", 8);

		if (GC->rtcc->PZLDPDIS.LAT_LLS > 0)
		{
			sprintf(Buffer, "%05.2f°N", GC->rtcc->PZLDPDIS.LAT_LLS);
		}
		else
		{
			sprintf(Buffer, "%05.2f°S", abs(GC->rtcc->PZLDPDIS.LAT_LLS));
		}
		skp->Text(31 * W / 32, 3 * H / 28, Buffer, strlen(Buffer));
		if (GC->rtcc->PZLDPDIS.LONG_LLS > 0)
		{
			sprintf(Buffer, "%06.2f°E", GC->rtcc->PZLDPDIS.LONG_LLS);
		}
		else
		{
			sprintf(Buffer, "%06.2f°W", abs(GC->rtcc->PZLDPDIS.LONG_LLS));
		}
		skp->Text(31 * W / 32, 4 * H / 28, Buffer, strlen(Buffer));

		skpTextFmt(skp, 30 * W / 32, 20 * H / 28, "%07.3f°", GC->rtcc->PZLDPDIS.DescAsc);

		skp->Text(27 * W / 32, 19 * H / 28, GC->rtcc->PZLDPDIS.DescAzMode, strlen(GC->rtcc->PZLDPDIS.DescAzMode));

		skpTextFmt(skp, 10 * W / 32, 20 * H / 28, "%+06.2f", GC->rtcc->PZLDPDIS.PD_ThetaIgn);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 64, 6 * H / 28, "MVR/REV", 7);
		skp->Text(5 * W / 32, 6 * H / 28, "GETTH/GETIG", 11);
		skp->Text(23 * W / 64, 6 * H / 28, "LIG/DV", 6);
		skp->Text(15 * W / 32, 6 * H / 28, "HAC/HPC", 7);
		skp->Text(20 * W / 32, 6 * H / 28, "DEL/THPC", 8);
		skp->Text(25 * W / 32, 6 * H / 28, "DVX/DVY", 7);
		skp->Text(30 * W / 32, 6 * H / 28, "DVZ", 3);

		for (int i = 0;i < 4;i++)
		{
			Text(skp, 2 * W / 32, (7 + 3 * i) * H / 28, GC->rtcc->PZLDPDIS.MVR[i]);
			GET_Display(Buffer, GC->rtcc->PZLDPDIS.GETTH[i], false);
			skp->Text(6 * W / 32, (7 + 3 * i) * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->PZLDPDIS.GETIG[i], false);
			skp->Text(6 * W / 32, (8 + 3 * i) * H / 28, Buffer, strlen(Buffer));
			if (GC->rtcc->PZLDPDIS.LIG[i] > 0)
			{
				sprintf(Buffer, "%06.2f°E", GC->rtcc->PZLDPDIS.LIG[i]);
			}
			else
			{
				sprintf(Buffer, "%06.2f°W", abs(GC->rtcc->PZLDPDIS.LIG[i]));
			}
			skp->Text(12 * W / 32, (7 + 3 * i) * H / 28, Buffer, strlen(Buffer));
			skpTextFmt(skp, 12 * W / 32, (8 + 3 * i) * H / 28, "%06.2f", GC->rtcc->PZLDPDIS.DV[i]);
			skpTextFmt(skp, 17 * W / 32, (7 + 3 * i) * H / 28, "%+07.2f", GC->rtcc->PZLDPDIS.AC[i]);
			skpTextFmt(skp, 17 * W / 32, (8 + 3 * i) * H / 28, "%+07.2f", GC->rtcc->PZLDPDIS.HPC[i]);
			skpTextFmt(skp, 25 * W / 32, (7 + 3 * i) * H / 28, "%+06.1f", GC->rtcc->PZLDPDIS.DVVector[i].x);
			skpTextFmt(skp, 25 * W / 32, (8 + 3 * i) * H / 28, "%+06.1f", GC->rtcc->PZLDPDIS.DVVector[i].y);
			skpTextFmt(skp, 29 * W / 32, (7 + 3 * i) * H / 28, "%+06.1f", GC->rtcc->PZLDPDIS.DVVector[i].z);
		}

		skp->Text(1 * W / 32, 19 * H / 28, "POWERED DESCENT", 15);
		skp->Text(1 * W / 32, 20 * H / 28, "THETA IGN", 9);
		skp->Text(1 * W / 32, 21 * H / 28, "PROP. REM", 9);

		skp->Text(12 * W / 32, 19 * H / 28, "GETTH", 5);
		GET_Display(Buffer, GC->rtcc->PZLDPDIS.PD_GETTH, false);
		skp->Text(16 * W / 32, 19 * H / 28, Buffer, strlen(Buffer));
		skp->Text(12 * W / 32, 20 * H / 28, "GETIG", 5);
		GET_Display(Buffer, GC->rtcc->PZLDPDIS.PD_GETIG, false);
		skp->Text(16 * W / 32, 20 * H / 28, Buffer, strlen(Buffer));
		skp->Text(12 * W / 32, 21 * H / 28, "GETTD", 5);
		GET_Display(Buffer, GC->rtcc->PZLDPDIS.PD_GETTD, false);
		skp->Text(16 * W / 32, 21 * H / 28, Buffer, strlen(Buffer));

		skp->Text(21 * W / 32, 19 * H / 28, "MODE", 4);
		skp->Text(21 * W / 32, 20 * H / 28, "DESC AZ", 7);
		skp->Text(21 * W / 32, 21 * H / 28, "SN.LK.A", 7);
	}
	else if (screen == 61)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 2 * H / 14, "LGC DESCENT TARGET UPDATE (295)", 31);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 8, 4 * H / 14, "TLAND", 5);
		GET_Display(Buffer, GC->rtcc->CZTDTGTU.GETTD);
		skp->Text(3 * W / 8, 4 * H / 14, Buffer, strlen(Buffer));
		skp->Text(1 * W / 8, 5 * H / 14, "LOAD NO", 7);
		skpTextFmt(skp, 3 * W / 8, 5 * H / 14, "%d", GC->rtcc->CZTDTGTU.SequenceNumber);

		skp->Text(5 * W / 32, 13 * H / 28, "OID", 3);
		skp->Text(10 * W / 32, 13 * H / 28, "FCT", 3);
		skp->Text(15 * W / 32, 13 * H / 28, "DSKY V72", 8);

		for (int i = 1;i <= 5;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (i + 14) * H / 28, "%o", i);
		}

		skp->Text(10 * W / 32, 15 * H / 28, "INDEX", 5);
		skp->Text(10 * W / 32, 16 * H / 28, "ADD1", 4);
		skp->Text(10 * W / 32, 17 * H / 28, "TLAND", 5);
		skp->Text(10 * W / 32, 18 * H / 28, "ADD2", 4);
		skp->Text(10 * W / 32, 19 * H / 28, "TLAND", 5);

		for (int i = 0;i < 5;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 15) * H / 28, "%05d", GC->rtcc->CZTDTGTU.Octals[i]);
		}
	}
	else if (screen == 62)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "SUNRISE/SUNSET TIMES (MSK 1502)", 31);

		skp->SetFont(font2);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(9 * W / 32, 4 * H / 28, "SUNRISE", 7);
		skp->Text(25 * W / 32, 4 * H / 28, "SUNSET", 6);

		skp->Text(1 * W / 32, 5 * H / 28, "REV", 3);
		skp->Text(1 * W / 32, 6 * H / 28, "NO", 2);

		skp->Text(9 * W / 64, 5 * H / 28, "GETTR", 5);
		skp->Text(9 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(19 * W / 64, 5 * H / 28, "GETSR", 5);
		skp->Text(19 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(27 * W / 64, 5 * H / 28, "PIT", 3);
		skp->Text(27 * W / 64, 6 * H / 28, "DEG", 3);

		skp->Text(63 * W / 128, 5 * H / 28, "YAW", 3);
		skp->Text(63 * W / 128, 6 * H / 28, "DEG", 3);

		skp->Text(39 * W / 64, 5 * H / 28, "GETTS", 5);
		skp->Text(39 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(49 * W / 64, 5 * H / 28, "GETSS", 5);
		skp->Text(49 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(56 * W / 64, 5 * H / 28, "PIT", 3);
		skp->Text(56 * W / 64, 6 * H / 28, "DEG", 3);

		skp->Text(61 * W / 64, 5 * H / 28, "YAW", 3);
		skp->Text(61 * W / 64, 6 * H / 28, "DEG", 3);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		for (int i = 0;i < 8;i++)
		{
			skpTextFmt(skp, 1 * W / 32, (i + 8) * H / 28, "%02d", GC->rtcc->EZSSTAB.data[i].REV);

			GET_Display(Buffer, GC->rtcc->EZSSTAB.data[i].GETTR, false);
			skp->Text(3 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->EZSSTAB.data[i].GETSR, false);
			skp->Text(8 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));

			skpTextFmt(skp, 13 * W / 32, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZSSTAB.data[i].theta_SR);
			skpTextFmt(skp, 15 * W / 32, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZSSTAB.data[i].psi_SR);

			GET_Display(Buffer, GC->rtcc->EZSSTAB.data[i].GETTS, false);
			skp->Text(35 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->EZSSTAB.data[i].GETSS, false);
			skp->Text(45 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));

			skpTextFmt(skp, 55 * W / 64, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZSSTAB.data[i].theta_SS);
			skpTextFmt(skp, 59 * W / 64, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZSSTAB.data[i].psi_SS);
		}
	}
	else if (screen == 63)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "MOONRISE/MOONSET TIMES (MSK 1501)", 33);

		skp->SetFont(font2);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(9 * W / 32, 4 * H / 28, "MOONRISE", 8);
		skp->Text(25 * W / 32, 4 * H / 28, "MOONSET", 7);

		skp->Text(1 * W / 32, 5 * H / 28, "REV", 3);
		skp->Text(1 * W / 32, 6 * H / 28, "NO", 2);

		skp->Text(9 * W / 64, 5 * H / 28, "GETTR", 5);
		skp->Text(9 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(19 * W / 64, 5 * H / 28, "GETMR", 5);
		skp->Text(19 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(27 * W / 64, 5 * H / 28, "PIT", 3);
		skp->Text(27 * W / 64, 6 * H / 28, "DEG", 3);

		skp->Text(63 * W / 128, 5 * H / 28, "YAW", 3);
		skp->Text(63 * W / 128, 6 * H / 28, "DEG", 3);

		skp->Text(39 * W / 64, 5 * H / 28, "GETTS", 5);
		skp->Text(39 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(49 * W / 64, 5 * H / 28, "GETMS", 5);
		skp->Text(49 * W / 64, 6 * H / 28, "HH:MM:SS", 8);

		skp->Text(56 * W / 64, 5 * H / 28, "PIT", 3);
		skp->Text(56 * W / 64, 6 * H / 28, "DEG", 3);

		skp->Text(61 * W / 64, 5 * H / 28, "YAW", 3);
		skp->Text(61 * W / 64, 6 * H / 28, "DEG", 3);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		for (int i = 0;i < 8;i++)
		{
			skpTextFmt(skp, 1 * W / 32, (i + 8) * H / 28, "%02d", GC->rtcc->EZMMTAB.data[i].REV);

			GET_Display(Buffer, GC->rtcc->EZMMTAB.data[i].GETTR, false);
			skp->Text(3 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->EZMMTAB.data[i].GETSR, false);
			skp->Text(8 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));

			skpTextFmt(skp, 13 * W / 32, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZMMTAB.data[i].theta_SR);
			skpTextFmt(skp, 15 * W / 32, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZMMTAB.data[i].psi_SR);

			GET_Display(Buffer, GC->rtcc->EZMMTAB.data[i].GETTS, false);
			skp->Text(35 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->EZMMTAB.data[i].GETSS, false);
			skp->Text(45 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));

			skpTextFmt(skp, 55 * W / 64, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZMMTAB.data[i].theta_SS);
			skpTextFmt(skp, 59 * W / 64, (i + 8) * H / 28, "%03.0f", GC->rtcc->EZMMTAB.data[i].psi_SS);
		}
	}
	//FDO Launch Analog No. 1
	else if (screen == 64)
	{
		if (oapiGetSimTime() > GC->rtcc->fdolaunchanalog1tab.LastUpdateTime + 0.5)
		{
			EphemerisData sv = GC->rtcc->StateVectorCalcEphem(G->vessel);
			GC->rtcc->FDOLaunchAnalog1(sv);
		}

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "FDO LAUNCH ANALOG NO 1 (MSK 0040)", 33);

		skp->SetFont(font2);

		skp->SetTextAlign(oapi::Sketchpad::CENTER, oapi::Sketchpad::BASELINE);

		skp->Text(2 * W / 64, 13 * H / 80, "2.0", 3);
		skp->Text(5 * W / 64, 13 * H / 80, "35", 2);
		skp->Text(8 * W / 64, 13 * H / 80, "40", 2);

		skp->Text(2 * W / 64, 20 * H / 80, "1.5", 3);
		skp->Text(5 * W / 64, 20 * H / 80, "30", 2);
		skp->Text(8 * W / 64, 20 * H / 80, "35", 2);

		skp->Text(2 * W / 64, 27 * H / 80, "1.0", 3);
		skp->Text(5 * W / 64, 27 * H / 80, "25", 2);
		skp->Text(8 * W / 64, 27 * H / 80, "30", 2);

		skp->Text(5 * W / 64, 61 * H / 160, "(y)", 3);

		skp->Text(2 * W / 64, 34 * H / 80, ".5", 2);
		skp->Text(5 * W / 64, 34 * H / 80, "20", 2);
		skp->Text(8 * W / 64, 34 * H / 80, "25", 2);

		skp->Text(2 * W / 64, 41 * H / 80, "0", 1);
		skp->Text(5 * W / 64, 41 * H / 80, "15", 2);
		skp->Text(8 * W / 64, 41 * H / 80, "20", 2);

		skp->Text(2 * W / 64, 48 * H / 80, "-.5", 3);
		skp->Text(5 * W / 64, 48 * H / 80, "10", 2);
		skp->Text(8 * W / 64, 48 * H / 80, "15", 2);

		skp->Text(2 * W / 64, 55 * H / 80, "-1.0", 4);
		skp->Text(5 * W / 64, 55 * H / 80, "5", 1);
		skp->Text(8 * W / 64, 55 * H / 80, "10", 2);

		skp->Text(2 * W / 64, 62 * H / 80, "-1.5", 4);
		skp->Text(5 * W / 64, 62 * H / 80, "0", 1);
		skp->Text(8 * W / 64, 62 * H / 80, "5", 1);

		skp->Text(2 * W / 64, 69 * H / 80, "-2.0", 4);
		skp->Text(5 * W / 64, 69 * H / 80, "-5", 2);
		skp->Text(8 * W / 64, 69 * H / 80, "0", 1);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(3 * W / 20, 21 * H / 24, "0", 1);
		skp->Text(3 * W / 20, 22 * H / 24, "5", 1);
		skp->Text(3 * W / 20, 23 * H / 24, "21", 2);

		skp->Text(13 * W / 60, 22 * H / 24, "6.5", 3);

		skp->Text(17 * W / 60, 21 * H / 24, "1", 1);
		skp->Text(17 * W / 60, 22 * H / 24, "8", 1);
		skp->Text(17 * W / 60, 23 * H / 24, "22", 2);

		skp->Text(21 * W / 60, 22 * H / 24, "9.5", 3);

		skp->Text(25 * W / 60, 21 * H / 24, "2", 1);
		skp->Text(25 * W / 60, 22 * H / 24, "11", 2);
		skp->Text(25 * W / 60, 23 * H / 24, "23", 2);

		skp->Text(29 * W / 60, 22 * H / 24, "12.5", 4);
		skp->Text(29 * W / 60, 23 * H / 24, "(V)", 3);

		skp->Text(33 * W / 60, 21 * H / 24, "3", 1);
		skp->Text(33 * W / 60, 22 * H / 24, "14", 2);
		skp->Text(33 * W / 60, 23 * H / 24, "24", 2);

		skp->Text(37 * W / 60, 22 * H / 24, "15.5", 4);

		skp->Text(41 * W / 60, 21 * H / 24, "4", 1);
		skp->Text(41 * W / 60, 22 * H / 24, "17", 2);
		skp->Text(41 * W / 60, 23 * H / 24, "25", 2);

		skp->Text(45 * W / 60, 22 * H / 24, "18.5", 4);

		skp->Text(49 * W / 60, 21 * H / 24, "5", 1);
		skp->Text(49 * W / 60, 22 * H / 24, "20", 2);
		skp->Text(49 * W / 60, 23 * H / 24, "26", 2);

		skp->Text(53 * W / 60, 22 * H / 24, "21.5", 4);

		skp->Text(57 * W / 60, 21 * H / 24, "6", 1);
		skp->Text(57 * W / 60, 22 * H / 24, "23", 2);
		skp->Text(57 * W / 60, 23 * H / 24, "27", 2);

		skp->Line(W * 3 / 20, H * 3 / 20, W * 19 / 20, H * 3 / 20);
		skp->Line(W * 3 / 20, H * 3 / 20, W * 3 / 20, H * 17 / 20);
		skp->Line(W * 3 / 20, H * 17 / 20, W * 19 / 20, H * 17 / 20);
		skp->Line(W * 19 / 20, H * 3 / 20, W * 19 / 20, H * 17 / 20);

		for (unsigned i = 0;i < GC->rtcc->fdolaunchanalog1tab.XVal.size();i++)
		{
			skp->Pixel((int)(GC->rtcc->fdolaunchanalog1tab.XVal[i] * W), (int)(GC->rtcc->fdolaunchanalog1tab.YVal[i] * H), 0x00FFFF);
		}

	}
	//FDO Launch Analog No. 2
	else if (screen == 65)
	{
		if (oapiGetSimTime() > GC->rtcc->fdolaunchanalog2tab.LastUpdateTime + 0.5)
		{
			EphemerisData sv = GC->rtcc->StateVectorCalcEphem(G->vessel);
			GC->rtcc->FDOLaunchAnalog2(sv);
		}

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 1 * H / 14, "FDO LAUNCH ANALOG NO 2 (MSK 0041)", 33);

		skp->SetFont(font2);

		skp->SetTextAlign(oapi::Sketchpad::CENTER, oapi::Sketchpad::BASELINE);

		skp->Text(2 * W / 64, 10 * H / 60, "0", 1);
		skp->Text(5 * W / 64, 10 * H / 60, "0", 1);
		skp->Text(8 * W / 64, 10 * H / 60, "0", 1);

		skp->Text(2 * W / 64, 17 * H / 60, "-2", 2);
		skp->Text(5 * W / 64, 17 * H / 60, "-2", 2);
		skp->Text(8 * W / 64, 17 * H / 60, "-6", 2);

		skp->Text(2 * W / 64, 24 * H / 60, "-4", 2);
		skp->Text(5 * W / 64, 24 * H / 60, "-4", 2);
		skp->Text(8 * W / 64, 24 * H / 60, "-12", 3);

		skp->Text(5 * W / 64, 55 * H / 120, "(yEI)", 5);

		skp->Text(2 * W / 64, 31 * H / 60, "-6", 2);
		skp->Text(5 * W / 64, 31 * H / 60, "-6", 2);
		skp->Text(8 * W / 64, 31 * H / 60, "-18", 3);

		skp->Text(2 * W / 64, 38 * H / 60, "-8", 2);
		skp->Text(5 * W / 64, 38 * H / 60, "-8", 2);
		skp->Text(8 * W / 64, 38 * H / 60, "-24", 3);

		skp->Text(2 * W / 64, 45 * H / 60, "-10", 3);
		skp->Text(5 * W / 64, 45 * H / 60, "-10", 3);
		skp->Text(8 * W / 64, 45 * H / 60, "-30", 3);

		skp->Text(2 * W / 64, 52 * H / 60, "-12", 3);
		skp->Text(5 * W / 64, 52 * H / 60, "-12", 3);
		skp->Text(8 * W / 64, 52 * H / 60, "-36", 3);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(3 * W / 20, 21 * H / 24, "6", 1);
		skp->Text(3 * W / 20, 22 * H / 24, "22.5", 4);
		skp->Text(3 * W / 20, 23 * H / 24, "25", 2);

		skp->Text(83 * W / 340, 21 * H / 24, "8", 1);
		skp->Text(83 * W / 340, 22 * H / 24, "23", 2);
		skp->Text(83 * W / 340, 23 * H / 24, "26", 2);

		skp->Text(115 * W / 340, 21 * H / 24, "10", 2);
		skp->Text(115 * W / 340, 22 * H / 24, "23.5", 4);
		skp->Text(115 * W / 340, 23 * H / 24, "27", 2);

		skp->Text(147 * W / 340, 21 * H / 24, "12", 2);
		skp->Text(147 * W / 340, 22 * H / 24, "24", 2);
		skp->Text(147 * W / 340, 23 * H / 24, "28", 2);

		skp->Text(163 * W / 340, 22 * H / 24, "V", 1);

		skp->Text(179 * W / 340, 21 * H / 24, "14", 2);
		skp->Text(179 * W / 340, 22 * H / 24, "24.5", 4);
		skp->Text(179 * W / 340, 23 * H / 24, "29", 2);

		skp->Text(211 * W / 340, 21 * H / 24, "16", 2);
		skp->Text(211 * W / 340, 22 * H / 24, "25", 2);
		skp->Text(211 * W / 340, 23 * H / 24, "30", 2);

		skp->Text(243 * W / 340, 21 * H / 24, "18", 2);
		skp->Text(243 * W / 340, 22 * H / 24, "25.5", 4);
		skp->Text(243 * W / 340, 23 * H / 24, "31", 2);

		skp->Text(275 * W / 340, 21 * H / 24, "20", 2);
		skp->Text(275 * W / 340, 22 * H / 24, "26", 2);
		skp->Text(275 * W / 340, 23 * H / 24, "32", 2);

		skp->Text(307 * W / 340, 21 * H / 24, "22", 2);
		skp->Text(307 * W / 340, 22 * H / 24, "26.5", 4);
		skp->Text(307 * W / 340, 23 * H / 24, "33", 2);

		skp->Text(323 * W / 340, 21 * H / 24, "23", 2);
		skp->Text(323 * W / 340, 23 * H / 24, "33.5", 4);

		skp->Line(W * 3 / 20, H * 3 / 20, W * 19 / 20, H * 3 / 20);
		skp->Line(W * 3 / 20, H * 3 / 20, W * 3 / 20, H * 17 / 20);
		skp->Line(W * 3 / 20, H * 17 / 20, W * 19 / 20, H * 17 / 20);
		skp->Line(W * 19 / 20, H * 3 / 20, W * 19 / 20, H * 17 / 20);

		for (unsigned i = 0;i < GC->rtcc->fdolaunchanalog2tab.XVal.size();i++)
		{
			skp->Pixel((int)(GC->rtcc->fdolaunchanalog2tab.XVal[i] * W), (int)(GC->rtcc->fdolaunchanalog2tab.YVal[i] * H), 0x00FFFF);
		}

	}
	//Return-to-Earth Tradeoff Display
	else if (screen == 66)
	{
		if (RTETradeoffScreen == 0)
		{
			skp->SetTextAlign(oapi::Sketchpad::CENTER);
			skp->Text(4 * W / 8, 1 * H / 14, "RTE TRADE OFF INPUTS", 32);

			skp->SetTextAlign(oapi::Sketchpad::LEFT);

			if (G->RTETradeoffMode == 0)
			{
				skp->Text(1 * W / 16, 2 * H / 14, "Near-Earth", 11);

				Text(skp, 1 * W / 16, 6 * H / 14, GC->rtcc->med_f70.Site);
				GET_Display(Buffer, GC->rtcc->med_f70.T_V *3600.0, false);
				skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));
				GET_Display(Buffer, GC->rtcc->med_f70.T_omin*3600.0, false);
				skp->Text(1 * W / 16, 10 * H / 14, Buffer, strlen(Buffer));
				GET_Display(Buffer, GC->rtcc->med_f70.T_omax*3600.0, false);
				skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));

				if (GC->rtcc->med_f70.EntryProfile == 1)
				{
					skp->Text(10 * W / 16, 10 * H / 14, "Constant G", 10);
				}
				else
				{
					skp->Text(10 * W / 16, 10 * H / 14, "G&N", 3);
				}
			}
			else
			{
				skp->Text(1 * W / 16, 2 * H / 14, "Remote-Earth", 13);

				skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%d", GC->rtcc->med_f71.Page);
				Text(skp, 1 * W / 16, 6 * H / 14, GC->rtcc->med_f71.Site);
				GET_Display(Buffer, GC->rtcc->med_f71.T_V *3600.0, false);
				skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));
				GET_Display(Buffer, GC->rtcc->med_f71.T_omin*3600.0, false);
				skp->Text(1 * W / 16, 10 * H / 14, Buffer, strlen(Buffer));
				GET_Display(Buffer, GC->rtcc->med_f71.T_omax*3600.0, false);
				skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));

				if (GC->rtcc->med_f71.EntryProfile == 1)
				{
					skp->Text(10 * W / 16, 10 * H / 14, "Constant G", 10);
				}
				else
				{
					skp->Text(10 * W / 16, 10 * H / 14, "G&N", 3);
				}
			}
		}
		else
		{
			skp->SetTextAlign(oapi::Sketchpad::CENTER);
			skp->Text(4 * W / 8, 1 * H / 14, "RTE TRADE OFF DISPLAY (MSK 0364)", 32);

			skp->SetFont(font2);

			skp->SetTextAlign(oapi::Sketchpad::CENTER, oapi::Sketchpad::BASELINE);

			skp->Line(W * 1 / 10, H * 3 / 20, W * 19 / 20, H * 3 / 20);
			skp->Line(W * 1 / 10, H * 3 / 20, W * 1 / 10, H * 9 / 10);
			skp->Line(W * 1 / 10, H * 9 / 10, W * 19 / 20, H * 9 / 10);
			skp->Line(W * 19 / 20, H * 3 / 20, W * 19 / 20, H * 9 / 10);

			unsigned p = RTETradeoffScreen - 1;

			skpTextFmt(skp, 4 * W / 40, 23 * H / 24, "%d", GC->rtcc->RTETradeoffTableBuffer[p].XLabels[0]);
			skpTextFmt(skp, 21 * W / 40, 23 * H / 24, "%d", GC->rtcc->RTETradeoffTableBuffer[p].XLabels[1]);
			skpTextFmt(skp, 38 * W / 40, 23 * H / 24, "%d", GC->rtcc->RTETradeoffTableBuffer[p].XLabels[2]);

			skpTextFmt(skp, 1 * W / 16, 6 * H / 40, "%d", GC->rtcc->RTETradeoffTableBuffer[p].YLabels[2]);
			skpTextFmt(skp, 1 * W / 16, 21 * H / 40, "%d", GC->rtcc->RTETradeoffTableBuffer[p].YLabels[1]);
			skpTextFmt(skp, 1 * W / 16, 36 * H / 40, "%d", GC->rtcc->RTETradeoffTableBuffer[p].YLabels[0]);

			skpTextFmt(skp, 12 * W / 40, 23 * H / 24, "Site: %s", GC->rtcc->RTETradeoffTableBuffer[p].Site.c_str());

			Text(skp, 29 * W / 40, 23 * H / 24, GC->rtcc->RTETradeoffTableBuffer[p].XAxisName);

			skp->SetFont(font2vert);
			Text(skp, 1 * W / 32, 14 * H / 40, GC->rtcc->RTETradeoffTableBuffer[p].YAxisName);
			skp->SetFont(font2);

			skp->SetPen(pen);

			for (unsigned i = 0;i < GC->rtcc->RTETradeoffTableBuffer[p].curves;i++)
			{
				if (GC->rtcc->RTETradeoffTableBuffer[p].TZDisplay[i] >= 0)
				{
					skpTextFmt(skp, (int)(GC->rtcc->RTETradeoffTableBuffer[p].TZxval[i] * W), (int)(GC->rtcc->RTETradeoffTableBuffer[p].TZyval[i] * H), 
								"%d", GC->rtcc->RTETradeoffTableBuffer[p].TZDisplay[i]);
				}

				for (unsigned j = 0;j < GC->rtcc->RTETradeoffTableBuffer[p].NumInCurve[i] - 1;j++)
				{
					skp->Line((int)(GC->rtcc->RTETradeoffTableBuffer[p].xval[i][j] * W), (int)(GC->rtcc->RTETradeoffTableBuffer[p].yval[i][j] * H),
						(int)(GC->rtcc->RTETradeoffTableBuffer[p].xval[i][j + 1] * W), (int)(GC->rtcc->RTETradeoffTableBuffer[p].yval[i][j + 1] * H));
				}
			}
		}
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->SetFont(font2);

		skpTextFmt(skp, 15 * W / 16, 1 * H / 14, "%d/5", RTETradeoffScreen);
	}
	//Detailed Maneuver Table 1 and 2
	else if (screen == 67 || screen == 68)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		DetailedManeuverTable *tab;
		if (screen == 67)
		{
			tab = &GC->rtcc->DMTBuffer[0];
			skp->Text(4 * W / 8, 1 * H / 14, "FDO DETAILED MANEUVER TABLE (MSK 0054)", 38);
		}
		else
		{
			tab = &GC->rtcc->DMTBuffer[1];
			skp->Text(4 * W / 8, 1 * H / 14, "FDO DETAILED MANEUVER TABLE (MSK 0069)", 38);
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->SetFont(font2);

		skp->Text(1 * W / 64, 5 * H / 32, "C STA ID", 8);
		skp->Text(1 * W / 64, 6 * H / 32, "GMTV", 4);
		skp->Text(1 * W / 64, 7 * H / 32, "GETV", 4);
		skp->Text(1 * W / 64, 8 * H / 32, "CODE", 4);

		skp->Text(18 * W / 64, 5 * H / 32, "L STA ID", 8);
		skp->Text(18 * W / 64, 6 * H / 32, "GMTV", 4);
		skp->Text(18 * W / 64, 7 * H / 32, "GETV", 4);
		skp->Text(20 * W / 64, 8 * H / 32, "REF", 3);

		skp->Text(35 * W / 64, 5 * H / 32, "C STA ID", 8);
		skp->Text(35 * W / 64, 6 * H / 32, "GMTV", 4);
		skp->Text(35 * W / 64, 7 * H / 32, "GETV", 4);
		skp->Text(35 * W / 64, 8 * H / 32, "GETR", 4);

		skp->Text(52 * W / 64, 5 * H / 32, "WT", 2);
		skp->Text(52 * W / 64, 6 * H / 32, "WC", 2);
		skp->Text(52 * W / 64, 7 * H / 32, "WL", 2);
		skp->Text(52 * W / 64, 8 * H / 32, "WF", 2);

		skp->Text(1 * W / 64, 10 * H / 32, "GETI", 4);
		skp->Text(1 * W / 64, 11 * H / 32, "PETI", 4);
		skp->Text(1 * W / 64, 12 * H / 32, "DVM", 3);
		skp->Text(1 * W / 64, 13 * H / 32, "DVREM", 5);
		skp->Text(1 * W / 64, 14 * H / 32, "DVC", 3);

		skp->Text(18 * W / 64, 10 * H / 32, "DTB", 3);
		skp->Text(18 * W / 64, 11 * H / 32, "DTU", 3);

		skp->Text(29 * W / 64, 10 * H / 32, "DT TO", 5);
		skp->Text(29 * W / 64, 11 * H / 32, "DV TO", 5);

		skp->Text(41 * W / 64, 10 * H / 32, "REFSMMAT", 8);

		skp->Text(52 * W / 64, 10 * H / 32, "DEL P", 5);
		skp->Text(52 * W / 64, 11 * H / 32, "DEL Y", 5);

		skp->Text(17 * W / 64, 12 * H / 32, "VGX", 3);
		skp->Text(17 * W / 64, 13 * H / 32, "VGY", 3);
		skp->Text(17 * W / 64, 14 * H / 32, "VGZ", 3);

		if (tab->isCSMTV)
		{
			skp->Text(29 * W / 64, 12 * H / 32, "OR", 2);
			skp->Text(29 * W / 64, 13 * H / 32, "IP", 2);
			skp->Text(29 * W / 64, 14 * H / 32, "MY", 2);

			skp->Text(40 * W / 64, 12 * H / 32, "RB", 2);
			skp->Text(40 * W / 64, 13 * H / 32, "PB", 2);
			skp->Text(40 * W / 64, 14 * H / 32, "YB", 2);
		}
		else
		{
			skp->Text(29 * W / 64, 12 * H / 32, "OY", 2);
			skp->Text(29 * W / 64, 13 * H / 32, "IP", 2);
			skp->Text(29 * W / 64, 14 * H / 32, "MR", 2);

			skp->Text(40 * W / 64, 12 * H / 32, "YB", 2);
			skp->Text(40 * W / 64, 13 * H / 32, "PB", 2);
			skp->Text(40 * W / 64, 14 * H / 32, "RB", 2);
		}

		skp->Text(52 * W / 64, 12 * H / 32, "YH", 2);
		skp->Text(52 * W / 64, 13 * H / 32, "PH", 2);
		skp->Text(52 * W / 64, 14 * H / 32, "RH", 2);

		skp->Text(1 * W / 64, 16 * H / 32, "VF", 2);
		skp->Text(1 * W / 64, 17 * H / 32, "VS", 2);
		skp->Text(1 * W / 64, 18 * H / 32, "VD", 2);
		skp->Text(1 * W / 64, 19 * H / 32, "DH", 2);
		skp->Text(1 * W / 64, 20 * H / 32, "PHASE", 5);
		skp->Text(1 * W / 64, 21 * H / 32, "PHASE DOT", 9);
		skp->Text(1 * W / 64, 22 * H / 32, "WEDGE ANG", 9);
		skp->Text(1 * W / 64, 23 * H / 32, "YD", 2);

		skp->Text(17 * W / 64, 16 * H / 32, "H BI", 4);
		skp->Text(17 * W / 64, 17 * H / 32, "P BI", 4);
		skp->Text(17 * W / 64, 18 * H / 32, "L BI", 4);
		skp->Text(17 * W / 64, 19 * H / 32, "F BI", 4);

		skp->Text(29 * W / 64, 16 * H / 32, "HA", 2);
		skp->Text(29 * W / 64, 17 * H / 32, "HP", 2);
		skp->Text(29 * W / 64, 18 * H / 32, "L AN", 4);
		skp->Text(29 * W / 64, 19 * H / 32, "E", 1);
		skp->Text(29 * W / 64, 20 * H / 32, "I", 1);
		skp->Text(29 * W / 64, 21 * H / 32, "WP", 2);

		skp->Text(42 * W / 64, 16 * H / 32, "VP", 2);
		skp->Text(42 * W / 64, 17 * H / 32, "THETA P", 7);
		skp->Text(42 * W / 64, 18 * H / 32, "DELTA P", 7);
		skp->Text(42 * W / 64, 19 * H / 32, "P LLS", 5);
		skp->Text(42 * W / 64, 20 * H / 32, "L LLS", 5);
		skp->Text(42 * W / 64, 21 * H / 32, "R LLS", 5);

		if (tab->Attitude == RTCC_ATTITUDE_SIVB_IGM)
		{
			skp->Text(1 * W / 64, 25 * H / 32, "IU IGM", 6);
			skp->Text(1 * W / 64, 27 * H / 32, "TB6", 3);
		}
		else
		{
			skp->Text(1 * W / 64, 25 * H / 32, "PGNS", 4);
			skp->Text(1 * W / 64, 26 * H / 32, "EXT DV", 6);
			skp->Text(1 * W / 64, 27 * H / 32, "GETI", 4);
			skp->Text(2 * W / 64, 28 * H / 32, "VX", 2);
			skp->Text(2 * W / 64, 29 * H / 32, "VY", 2);
			skp->Text(2 * W / 64, 30 * H / 32, "VZ", 2);

			skp->Text(18 * W / 64, 25 * H / 32, "AGS", 3);
			skp->Text(18 * W / 64, 26 * H / 32, "EXT DV", 6);
			skp->Text(18 * W / 64, 27 * H / 32, "GETI", 4);
			skp->Text(19 * W / 64, 28 * H / 32, "VX", 2);
			skp->Text(19 * W / 64, 29 * H / 32, "VY", 2);
			skp->Text(19 * W / 64, 30 * H / 32, "VZ", 2);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		Text(skp, 17 * W / 64, 5 * H / 32, tab->C_STA_ID);
		GET_Display3(Buffer, tab->C_GMTV);
		skp->Text(17 * W / 64, 6 * H / 32, Buffer, strlen(Buffer));
		GET_Display3(Buffer, tab->C_GETV);
		skp->Text(17 * W / 64, 7 * H / 32, Buffer, strlen(Buffer));
		Text(skp, 19 * W / 64, 8 * H / 32, tab->CODE);

		Text(skp, 34 * W / 64, 5 * H / 32, tab->L_STA_ID);
		GET_Display3(Buffer, tab->L_GMTV);
		skp->Text(34 * W / 64, 6 * H / 32, Buffer, strlen(Buffer));
		GET_Display3(Buffer, tab->L_GETV);
		skp->Text(34 * W / 64, 7 * H / 32, Buffer, strlen(Buffer));
		Text(skp, 34 * W / 64, 8 * H / 32, tab->REF);

		Text(skp, 51 * W / 64, 5 * H / 32, tab->X_STA_ID);
		GET_Display3(Buffer, tab->X_GMTV);
		skp->Text(51 * W / 64, 6 * H / 32, Buffer, strlen(Buffer));
		GET_Display3(Buffer, tab->X_GETV);
		skp->Text(51 * W / 64, 7 * H / 32, Buffer, strlen(Buffer));
		GET_Display3(Buffer, tab->GETR);
		skp->Text(51 * W / 64, 8 * H / 32, Buffer, strlen(Buffer));

		skpTextFmt(skp, 63 * W / 64, 5 * H / 32, "%07.1f", tab->WT);
		skpTextFmt(skp, 63 * W / 64, 6 * H / 32, "%07.1f", tab->WC);
		skpTextFmt(skp, 63 * W / 64, 7 * H / 32, "%07.1f", tab->WL);
		skpTextFmt(skp, 63 * W / 64, 8 * H / 32, "%07.1f", tab->WF);

		GET_Display3(Buffer, tab->GETI);
		skp->Text(17 * W / 64, 10 * H / 32, Buffer, strlen(Buffer));
		GET_Display3(Buffer, tab->PETI);
		skp->Text(17 * W / 64, 11 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 15 * W / 64, 12 * H / 32, "%.1f", tab->DVM);
		skpTextFmt(skp, 15 * W / 64, 13 * H / 32, "%.1f", tab->DVREM);
		skpTextFmt(skp, 15 * W / 64, 14 * H / 32, "%.1f", tab->DVC);

		int hh, mm;
		double ss;
		SStoHHMMSS(tab->DT_B, hh, mm, ss);
		skpTextFmt(skp, 28 * W / 64, 10 * H / 32, "%d:%04.1f", mm, ss);
		skpTextFmt(skp, 28 * W / 64, 11 * H / 32, "%.2f", tab->DT_U);
		skpTextFmt(skp, 28 * W / 64, 12 * H / 32, "%.1f", tab->VG.x);
		skpTextFmt(skp, 28 * W / 64, 13 * H / 32, "%.1f", tab->VG.y);
		skpTextFmt(skp, 28 * W / 64, 14 * H / 32, "%.1f", tab->VG.z);

		skpTextFmt(skp, 40 * W / 64, 10 * H / 32, "%.2f", tab->DT_TO);
		if (tab->DV_TO >= 10.0)
		{
			sprintf_s(Buffer, "%.1f", tab->DV_TO);
		}
		else
		{
			sprintf_s(Buffer, "%.2f", tab->DV_TO);
		}
		skp->Text(40 * W / 64, 11 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 39 * W / 64, 12 * H / 32, "%.1f", tab->IMUAtt.x);
		skpTextFmt(skp, 39 * W / 64, 13 * H / 32, "%.1f", tab->IMUAtt.y);
		skpTextFmt(skp, 39 * W / 64, 14 * H / 32, "%.1f", tab->IMUAtt.z);

		Text(skp, 50 * W / 64, 11 * H / 32, tab->REFSMMAT_Code);
		skpTextFmt(skp, 51 * W / 64, 12 * H / 32, "%.1f", tab->FDAIAtt.x);
		skpTextFmt(skp, 51 * W / 64, 13 * H / 32, "%.1f", tab->FDAIAtt.y);
		skpTextFmt(skp, 51 * W / 64, 14 * H / 32, "%.1f", tab->FDAIAtt.z);

		skpTextFmt(skp, 63 * W / 64, 10 * H / 32, "%5.2f", tab->DEL_P);
		skpTextFmt(skp, 63 * W / 64, 11 * H / 32, "%5.2f", tab->DEL_Y);
		skpTextFmt(skp, 63 * W / 64, 12 * H / 32, "%.1f", tab->LVLHAtt.x);
		skpTextFmt(skp, 63 * W / 64, 13 * H / 32, "%.1f", tab->LVLHAtt.y);
		skpTextFmt(skp, 63 * W / 64, 14 * H / 32, "%.1f", tab->LVLHAtt.z);

		skpTextFmt(skp, 15 * W / 64, 16 * H / 32, "%.2f", tab->VF);
		skpTextFmt(skp, 15 * W / 64, 17 * H / 32, "%.2f", tab->VS);
		skpTextFmt(skp, 15 * W / 64, 18 * H / 32, "%.2f", tab->VD);
		skpTextFmt(skp, 15 * W / 64, 19 * H / 32, "%.2f", tab->DH);
		skpTextFmt(skp, 19 * W / 64, 20 * H / 32, "%.3f", tab->PHASE);
		skpTextFmt(skp, 19 * W / 64, 21 * H / 32, "%.3f", tab->PHASE_DOT);
		skpTextFmt(skp, 19 * W / 64, 22 * H / 32, "%.3f", tab->WEDGE_ANG);
		skpTextFmt(skp, 19 * W / 64, 23 * H / 32, "%.4f", tab->YD);

		skpTextFmt(skp, 28 * W / 64, 16 * H / 32, "%.1f", tab->H_BI);
		if (tab->P_BI > 0)
		{
			sprintf_s(Buffer, "%.2f N", tab->P_BI);
		}
		else
		{
			sprintf_s(Buffer, "%.2f S", abs(tab->P_BI));
		}
		skp->Text(28 * W / 64, 17 * H / 32, Buffer, strlen(Buffer));
		if (tab->L_BI > 0)
		{
			sprintf_s(Buffer, "%.2f E", tab->L_BI);
		}
		else
		{
			sprintf_s(Buffer, "%.2f W", abs(tab->L_BI));
		}
		skp->Text(28 * W / 64, 18 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 28 * W / 64, 19 * H / 32, "%.2f", tab->F_BI);

		skpTextFmt(skp, 41 * W / 64, 16 * H / 32, "%.3f", tab->HA);
		skpTextFmt(skp, 41 * W / 64, 17 * H / 32, "%.3f", tab->HP);
		if (tab->L_AN > 0)
		{
			sprintf_s(Buffer, "%.2f E", tab->L_AN);
		}
		else
		{
			sprintf_s(Buffer, "%.2f W", abs(tab->L_AN));
		}
		skp->Text(41 * W / 64, 18 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 41 * W / 64, 19 * H / 32, "%.8f", tab->E);
		skpTextFmt(skp, 41 * W / 64, 20 * H / 32, "%.4f", tab->I);
		skpTextFmt(skp, 41 * W / 64, 21 * H / 32, "%.4f", tab->WP);

		skpTextFmt(skp, 57 * W / 64, 16 * H / 32, "%.2f", tab->VP);
		skpTextFmt(skp, 57 * W / 64, 17 * H / 32, "%.2f", tab->THETA_P);
		skpTextFmt(skp, 57 * W / 64, 18 * H / 32, "%.2f", tab->DELTA_P);
		if (tab->P_LLS > 0)
		{
			sprintf_s(Buffer, "%.2f N", tab->P_LLS);
		}
		else
		{
			sprintf_s(Buffer, "%.2f S", abs(tab->P_LLS));
		}
		skp->Text(57 * W / 64, 19 * H / 32, Buffer, strlen(Buffer));
		if (tab->L_LLS > 0)
		{
			sprintf_s(Buffer, "%.2f E", tab->L_LLS);
		}
		else
		{
			sprintf_s(Buffer, "%.2f W", abs(tab->L_LLS));
		}
		skp->Text(57 * W / 64, 20 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 57 * W / 64, 21 * H / 32, "%.2f", tab->R_LLS);

		if (tab->Attitude == RTCC_ATTITUDE_SIVB_IGM)
		{
			GET_Display3(Buffer, tab->PGNS_GETI);
			skp->Text(17 * W / 64, 27 * H / 32, Buffer, strlen(Buffer));
		}
		else
		{
			GET_Display3(Buffer, tab->PGNS_GETI);
			skp->Text(17 * W / 64, 27 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 17 * W / 64, 28 * H / 32, "%.4f", tab->PGNS_DV.x);
			skpTextFmt(skp, 17 * W / 64, 29 * H / 32, "%.4f", tab->PGNS_DV.y);
			skpTextFmt(skp, 17 * W / 64, 30 * H / 32, "%.4f", tab->PGNS_DV.z);

			GET_Display3(Buffer, tab->AGS_GETI);
			skp->Text(34 * W / 64, 27 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 34 * W / 64, 28 * H / 32, "%.4f", tab->AGS_DV.x);
			skpTextFmt(skp, 34 * W / 64, 29 * H / 32, "%.4f", tab->AGS_DV.y);
			skpTextFmt(skp, 34 * W / 64, 30 * H / 32, "%.4f", tab->AGS_DV.z);
		}

		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		Text(skp, 32 * W / 64, 31 * H / 32, tab->error);
	}
	else if (screen == 69)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "SPQ Initialization (MED K06)", 28);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skpTextFmt(skp, 1 * W / 16, 2 * H / 14, "%.1f NM", GC->rtcc->GZGENCSN.SPQDeltaH / 1852.0);
		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.2f°", GC->rtcc->GZGENCSN.SPQElevationAngle * DEG);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.2f°", GC->rtcc->GZGENCSN.SPQTerminalPhaseAngle * DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.1f NM", GC->rtcc->GZGENCSN.SPQMinimumPerifocus / 1852.0);
		if (GC->rtcc->GZGENCSN.TPIDefinition == 1)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "Chaser Day/Night Time", 21);
			sprintf_s(Buffer, "%.1f min", GC->rtcc->GZGENCSN.TPIDefinitionValue / 60.0);
		}
		else if (GC->rtcc->GZGENCSN.TPIDefinition == 2)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "Chaser Longitude", 18);
			sprintf_s(Buffer, "%.1f°", GC->rtcc->GZGENCSN.TPIDefinitionValue*DEG);
		}
		else if (GC->rtcc->GZGENCSN.TPIDefinition == 3)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "On time", 7);
			GET_Display(Buffer, GC->rtcc->GZGENCSN.TPIDefinitionValue);
		}
		else if (GC->rtcc->GZGENCSN.TPIDefinition == 4)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "Target Day/Night Time", 21);
			sprintf_s(Buffer, "%.1f min", GC->rtcc->GZGENCSN.TPIDefinitionValue / 60.0);
		}
		else if (GC->rtcc->GZGENCSN.TPIDefinition == 5)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "Target Longitude", 16);
			sprintf_s(Buffer, "%.1f°", GC->rtcc->GZGENCSN.TPIDefinitionValue*DEG);
		}
		skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		if (GC->rtcc->med_k01.I_CDH == 1)
		{
			skp->Text(15 * W / 16, 2 * H / 14, "CDH at upcoming apsis (AEG):", 28);
			sprintf_s(Buffer, "%d", GC->rtcc->med_k01.CDH_Apsis);
		}
		else if (GC->rtcc->med_k01.I_CDH == 2)
		{
			skp->Text(15 * W / 16, 2 * H / 14, "CDH on time:", 12);
			GET_Display3(Buffer, GC->rtcc->med_k01.CDH_Time);
		}
		else if (GC->rtcc->med_k01.I_CDH == 3)
		{
			skp->Text(15 * W / 16, 2 * H / 14, "Angle from CSI to CDH:", 22);
			sprintf_s(Buffer, "%.1lf°", GC->rtcc->med_k01.CDH_Angle*DEG);
		}
		else
		{
			skp->Text(15 * W / 16, 2 * H / 14, "CDH at upcoming apsis (Keplerian):", 34);
			sprintf_s(Buffer, "%d", GC->rtcc->med_k01.CDH_Apsis);
		}

		skp->Text(15 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
	}
	else if (screen == 70)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "DKI Initialization (MED K05)", 28);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		if (GC->rtcc->med_k00.I4)
		{
			skpTextFmt(skp, 1 * W / 16, 2 * H / 14, "NCC DH: %.1f NM", GC->rtcc->GZGENCSN.DKIDeltaH_NCC / 1852.0);
		}
		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "NSR DH: %.1f NM", GC->rtcc->GZGENCSN.DKIDeltaH_NSR / 1852.0);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.2f°", GC->rtcc->GZGENCSN.DKIElevationAngle* DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2f°", GC->rtcc->GZGENCSN.DKITerminalPhaseAngle* DEG);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.1f NM", GC->rtcc->GZGENCSN.DKIMinPerigee / 1852.0);
		if (GC->rtcc->med_k00.I4)
		{
			skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.1f min", GC->rtcc->med_k00.dt_NCC_NSR / 60.0);
		}
	}
	else if (screen == 75)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "ON LINE MONITOR (MSK 1629)", 28);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->SetFont(font2);

		unsigned line = 0;

		for (unsigned i = 0;i < GC->rtcc->RTCCONLINEMON.data.size();i++)
		{
			for (unsigned j = 0;j < GC->rtcc->RTCCONLINEMON.data[i].message.size();j++)
			{
				Text(skp, 1 * W / 32, (4 + line) * H / 32, GC->rtcc->RTCCONLINEMON.data[i].message[j]);
				line++;
				if (line >= 28)
				{
					break;
				}
			}		
			line++;
			if (line >= 28)
			{
				break;
			}
		}
	}
	else if (screen == 76)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		if (GC->rtcc->med_m78.Type)
		{
			skp->Text(4 * W / 8, 1 * H / 14, "LOI Transfer (MED M78)", 22);
		}
		else
		{
			skp->Text(4 * W / 8, 1 * H / 14, "MCC Transfer (MED M78)", 22);
		}

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_m78.Table == 1)
			{
				skp->Text(1 * W / 16, 2 * H / 14, "CSM", 3);
			}
			else
			{
				skp->Text(1 * W / 16, 2 * H / 14, "LEM", 3);
			}

			if (GC->rtcc->med_m78.ReplaceCode == 0)
			{
				skp->Text(1 * W / 16, 4 * H / 14, "Don't replace", 13);
			}
			else
			{
				skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%d", GC->rtcc->med_m78.ReplaceCode);
			}
		}

		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%d", GC->rtcc->med_m78.ManeuverNumber);

		ThrusterName(Buffer, GC->rtcc->med_m78.Thruster);
		skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

		if (GC->MissionPlanningActive)
		{
			const char* attname = MPTAttitudeName(GC->rtcc->med_m78.Attitude);
			skp->Text(1 * W / 16, 10 * H / 14, attname, strlen(attname));

			if (GC->rtcc->med_m78.UllageDT < 0)
			{
				strcpy(Buffer, "Nominal ullage");
			}
			else
			{
				if (GC->rtcc->med_m78.UllageQuads)
				{
					sprintf_s(Buffer, "4 quads, %.1f s", GC->rtcc->med_m78.UllageDT);
				}
				else
				{
					sprintf_s(Buffer, "2 quads, %.1f s", GC->rtcc->med_m78.UllageDT);
				}
			}
			skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));

			if (GC->rtcc->med_m78.Iteration)
			{
				skp->Text(10 * W / 16, 2 * H / 14, "Iterate", 7);
			}
			else
			{
				skp->Text(10 * W / 16, 2 * H / 14, "Don't iterate", 13);
			}

			if (GC->rtcc->med_m78.Thruster == RTCC_ENGINETYPE_LMDPS)
			{
				skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%lf s", GC->rtcc->med_m78.TenPercentDT);
				skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%lf", GC->rtcc->med_m78.DPSThrustFactor);
			}

			if (GC->rtcc->med_m78.TimeFlag)
			{
				skp->Text(10 * W / 16, 8 * H / 14, "Impulsive TIG", 13);
			}
			else
			{
				skp->Text(10 * W / 16, 8 * H / 14, "Optimum TIG", 11);
			}
		}

		if (GC->MissionPlanningActive == false)
		{
			GET_Display(Buffer, G->P30TIG);
			skp->Text(5 * W / 8, 17 * H / 21, Buffer, strlen(Buffer));

			skp->Text(5 * W / 8, 18 * H / 21, "DVX", 3);
			skp->Text(5 * W / 8, 19 * H / 21, "DVY", 3);
			skp->Text(5 * W / 8, 20 * H / 21, "DVZ", 3);

			AGC_Display(Buffer, G->dV_LVLH.x / 0.3048);
			skp->Text(6 * W / 8, 18 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.y / 0.3048);
			skp->Text(6 * W / 8, 19 * H / 21, Buffer, strlen(Buffer));
			AGC_Display(Buffer, G->dV_LVLH.z / 0.3048);
			skp->Text(6 * W / 8, 20 * H / 21, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 77)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "SKELETON FLIGHT PLAN TABLE (MSK 1597)", 37);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->SetFont(font2);

		int i = GC->rtcc->PZSFPTAB.DisplayBlockNum - 1;

		skp->Text(2 * W / 32, 5 * H / 35, "BLOCK NUMBER", 12);
		skp->Text(2 * W / 32, 6 * H / 35, "01-GMT TIME FLAG", 16);
		skp->Text(2 * W / 32, 7 * H / 35, "02-MODE", 7);
		skp->Text(2 * W / 32, 8 * H / 35, "03-GMT OF TLI PERICYN.", 22);
		skp->Text(2 * W / 32, 9 * H / 35, "04-LAT. OF TLI PERICYN.", 23);
		skp->Text(2 * W / 32, 10 * H / 35, "05-LONG. OF TLI PERICYN.", 24);
		skp->Text(2 * W / 32, 11 * H / 35, "06-HEIGHT OF TLI PERICYN.", 25);
		skp->Text(2 * W / 32, 12 * H / 35, "07-GMT OF LOI PERICYN.", 22);
		skp->Text(2 * W / 32, 13 * H / 35, "08-LAT. OF LOI PERICYN.", 23);
		skp->Text(2 * W / 32, 14 * H / 35, "09-LONG. OF LOI PERICYN.", 24);
		skp->Text(2 * W / 32, 15 * H / 35, "10-HEIGHT OF LOI PERICYN.", 25);
		skp->Text(2 * W / 32, 16 * H / 35, "11-GET OF TLI IGNITION", 22);
		skp->Text(2 * W / 32, 17 * H / 35, "12-GMT OF NODE", 14);
		skp->Text(2 * W / 32, 18 * H / 35, "13-LAT. OF NODE", 15);
		skp->Text(2 * W / 32, 19 * H / 35, "14-LONG. OF NODE", 16);
		skp->Text(2 * W / 32, 20 * H / 35, "15-HEIGHT OF NODE", 17);
		skp->Text(2 * W / 32, 21 * H / 35, "16-DELTA AZIMUTH OF LOI", 23);
		skp->Text(2 * W / 32, 22 * H / 35, "17-FLIGHT PATH ANGLE AT LOI", 27);
		skp->Text(2 * W / 32, 23 * H / 35, "18-DELTA TIME OF LPO", 20);
		skp->Text(2 * W / 32, 24 * H / 35, "19-DELTA TIME OF LLS", 20);
		skp->Text(2 * W / 32, 25 * H / 35, "20-AZIMUTH OF LLS", 17);
		skp->Text(2 * W / 32, 26 * H / 35, "21-LAT. OF LLS", 14);
		skp->Text(2 * W / 32, 27 * H / 35, "22-LONG. OF LLS", 15);
		skp->Text(2 * W / 32, 28 * H / 35, "23-RADIUS OF LLS", 16);
		skp->Text(2 * W / 32, 29 * H / 35, "24-DELTA AZIMUTH OF TEI", 23);
		skp->Text(2 * W / 32, 30 * H / 35, "25-DELTA V OF TEI", 17);
		skp->Text(2 * W / 32, 31 * H / 35, "26-DELTA TIME OF TEI", 20);
		//skp->Text(2 * W / 32, 32 * H / 35, "27-INCLINATION OF FREE RET.", 27);
		//skp->Text(2 * W / 32, 33 * H / 35, "28-DELTA T OF UPDATED NOM.", 26);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		if (i == 0)
		{
			skp->Text(25 * W / 32, 5 * H / 35, "1 (PREFLIGHT)", 13);
		}
		else
		{
			skp->Text(25 * W / 32, 5 * H / 35, "2 (NOMINAL)", 11);
		}
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].GMTTimeFlag);
		skp->Text(25 * W / 32, 6 * H / 35, Buffer, strlen(Buffer));
		skpTextFmt(skp, 25 * W / 32, 7 * H / 35, "%d", GC->rtcc->PZSFPTAB.blocks[i].mode);
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].GMT_pc1);
		skp->Text(25 * W / 32, 8 * H / 35, Buffer, strlen(Buffer));
		skpTextFmt(skp, 25 * W / 32, 9 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lat_pc1 * DEG);
		skpTextFmt(skp, 25 * W / 32, 10 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lng_pc1 * DEG);
		skpTextFmt(skp, 25 * W / 32, 11 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].h_pc1 / 1852.0);
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].GMT_pc2);
		skp->Text(25 * W / 32, 12 * H / 35, Buffer, strlen(Buffer));
		skpTextFmt(skp, 25 * W / 32, 13 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lat_pc2 * DEG);
		skpTextFmt(skp, 25 * W / 32, 14 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lng_pc2 * DEG);
		skpTextFmt(skp, 25 * W / 32, 15 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].h_pc2 / 1852.0);
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].GET_TLI);
		skp->Text(25 * W / 32, 16 * H / 35, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].GMT_nd);
		skp->Text(25 * W / 32, 17 * H / 35, Buffer, strlen(Buffer));
		skpTextFmt(skp, 25 * W / 32, 18 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lat_nd * DEG);
		skpTextFmt(skp, 25 * W / 32, 19 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lng_nd * DEG);
		skpTextFmt(skp, 25 * W / 32, 20 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].h_nd / 1852.0);
		skpTextFmt(skp, 25 * W / 32, 21 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].dpsi_loi * DEG);
		skpTextFmt(skp, 25 * W / 32, 22 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].gamma_loi * DEG);
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].T_lo);
		skp->Text(25 * W / 32, 23 * H / 35, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].dt_lls);
		skp->Text(25 * W / 32, 24 * H / 35, Buffer, strlen(Buffer));
		skpTextFmt(skp, 25 * W / 32, 25 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].psi_lls* DEG);
		skpTextFmt(skp, 25 * W / 32, 26 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lat_lls* DEG);
		skpTextFmt(skp, 25 * W / 32, 27 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].lng_lls* DEG);
		skpTextFmt(skp, 25 * W / 32, 28 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].rad_lls / 1852.0);
		skpTextFmt(skp, 25 * W / 32, 29 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].dpsi_tei* DEG);
		skpTextFmt(skp, 25 * W / 32, 30 * H / 35, "%.3lf", GC->rtcc->PZSFPTAB.blocks[i].dv_tei / 0.3048);
		GET_Display2(Buffer, GC->rtcc->PZSFPTAB.blocks[i].T_te);
		skp->Text(25 * W / 32, 31 * H / 35, Buffer, strlen(Buffer));
	}
	else if (screen == 78)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "MIDCOURSE TRADEOFF (MSK 0079)", 29);
		skp->SetFont(font2);
		skp->SetPen(pen2);

		skp->Text(22 * W / 64, 5 * H / 32, "1", 1);
		skp->Text(33 * W / 64, 5 * H / 32, "2", 1);
		skp->Text(44 * W / 64, 5 * H / 32, "3", 1);
		skp->Text(55 * W / 64, 5 * H / 32, "4", 1);
		skp->Line(16 * W / 64, 13 * H / 64, 60 * W / 64, 13 * H / 64);
		skp->Line(16 * W / 64, 13 * H / 64, 16 * W / 64, 31 * H / 32);
		skp->Line(27 * W / 64, 13 * H / 64, 27 * W / 64, 31 * H / 32);
		skp->Line(38 * W / 64, 13 * H / 64, 38 * W / 64, 31 * H / 32);
		skp->Line(49 * W / 64, 13 * H / 64, 49 * W / 64, 31 * H / 32);
		skp->Line(60 * W / 64, 13 * H / 64, 60 * W / 64, 31 * H / 32);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->Text(1 * W / 32, 5 * H / 32, "COLUMN", 6);
		skp->Text(1 * W / 32, 7 * H / 32, "MODE", 4);
		skp->Text(1 * W / 32, 8 * H / 32, "RETURN", 6);
		skp->Text(1 * W / 32, 9 * H / 32, "AZ MIN", 6);
		skp->Text(1 * W / 32, 10 * H / 32, "AZ MAX", 6);
		skp->Text(1 * W / 32, 11 * H / 32, "WEIGHT", 6);
		skp->Text(1 * W / 32, 12 * H / 32, "GETMCC", 6);
		skp->Text(1 * W / 32, 13 * H / 32, "DV MCC", 6);
		skp->Text(1 * W / 32, 14 * H / 32, "YAW MCC", 7);
		skp->Text(1 * W / 32, 15 * H / 32, "H PYCN", 6);
		skp->Text(1 * W / 32, 16 * H / 32, "GET LOI", 7);
		skp->Text(1 * W / 32, 17 * H / 32, "DV LOI", 6);
		skp->Text(1 * W / 32, 18 * H / 32, "AZ ACT", 6);
		skp->Text(1 * W / 32, 19 * H / 32, "I FR", 4);
		skp->Text(1 * W / 32, 20 * H / 32, "I PR", 4);
		skp->Text(1 * W / 32, 21 * H / 32, "V EI", 4);
		skp->Text(1 * W / 32, 22 * H / 32, "G EI", 4);
		skp->Text(1 * W / 32, 23 * H / 32, "GETTEI", 6);
		skp->Text(1 * W / 32, 24 * H / 32, "DV TEI", 6);
		skp->Text(1 * W / 32, 25 * H / 32, "DV REM", 6);
		skp->Text(1 * W / 32, 26 * H / 32, "GET LC", 6);
		skp->Text(1 * W / 32, 27 * H / 32, "LAT IP", 6);
		skp->Text(1 * W / 32, 28 * H / 32, "LNG IP", 6);
		skp->Text(1 * W / 32, 29 * H / 32, "DV PC", 5);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		for (int i = 0;i < 4;i++)
		{
			if (GC->rtcc->PZMCCDIS.data[i].Mode == 0)
			{
				continue;
			}

			skpTextFmt(skp, (26 + 11 * i) * W / 64, 7 * H / 32, "%d", GC->rtcc->PZMCCDIS.data[i].Mode);
			const char* retmode = "";
			if (GC->rtcc->PZMCCDIS.data[i].Return == 0)
			{
				retmode = "N/A";
			}
			else if (GC->rtcc->PZMCCDIS.data[i].Return == 1)
			{
				retmode = "NONFREE";
			}
			else
			{
				retmode = "FREE";
			}
			skp->Text((26 + 11 * i) * W / 64, 8 * H / 32, retmode, strlen(retmode));
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 9 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].AZ_min* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 10 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].AZ_max* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 11 * H / 32, "%.0lf", (GC->rtcc->PZMCCDIS.data[i].CSMWT + GC->rtcc->PZMCCDIS.data[i].LMWT) / 0.45359237);
			GET_Display(Buffer, GC->rtcc->PZMCCDIS.data[i].GET_MCC, false);
			skp->Text((26 + 11 * i) * W / 64, 12 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 13 * H / 32, "%.1lf", length(GC->rtcc->PZMCCDIS.data[i].DV_MCC) / 0.3048);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 14 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].YAW_MCC* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 15 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].h_PC / 1852.0);
			GET_Display(Buffer, GC->rtcc->PZMCCDIS.data[i].GET_LOI, false);
			skp->Text((26 + 11 * i) * W / 64, 16 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 17 * H / 32, "%.1lf", length(GC->rtcc->PZMCCDIS.data[i].DV_LOI) / 0.3048);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 18 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].AZ_act* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 19 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].incl_fr* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 20 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].incl_pr* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 21 * H / 32, "%.1lf", GC->rtcc->PZMCCDIS.data[i].v_EI / 0.3048);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 22 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].gamma_EI* DEG);
			GET_Display(Buffer, GC->rtcc->PZMCCDIS.data[i].GET_TEI, false);
			skp->Text((26 + 11 * i) * W / 64, 23 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 24 * H / 32, "%.1lf", length(GC->rtcc->PZMCCDIS.data[i].DV_TEI) / 0.3048);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 25 * H / 32, "%.1lf", GC->rtcc->PZMCCDIS.data[i].DV_REM / 0.3048);
			GET_Display(Buffer, GC->rtcc->PZMCCDIS.data[i].GET_LC, false);
			skp->Text((26 + 11 * i) * W / 64, 26 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 27 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].lat_IP* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 28 * H / 32, "%.3lf", GC->rtcc->PZMCCDIS.data[i].lng_IP* DEG);
			skpTextFmt(skp, (26 + 11 * i) * W / 64, 29 * H / 32, "%.1lf", length(GC->rtcc->PZMCCDIS.data[i].DV_LOPC / 0.3048));
		}
	}
	else if (screen == 79)
	{
		if (G->TLImaneuver == 0)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "TLI (nodal)", 11);
		}
		else if (G->TLImaneuver == 1)
		{
			skp->Text(1 * W / 8, 2 * H / 14, "TLI (free return)", 17);
		}

		skp->Text(4 * W / 8, 1 * H / 14, "TLI PLANNING DISPLAY (MSK 0080)", 31);

		skp->Text(4 * W / 8, 6 * H / 14, "Under construction!", 19);
	}
	else if (screen == 80)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "MIDCOURSE INPUTS AND CONSTANTS", 30);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->SetFont(font2);

		skp->Text(1 * W / 16, 4 * H / 28, "MED F22", 7);
		skp->Text(1 * W / 16, 5 * H / 28, "Azimuth Constraints (Modes 3/5)", 31);
		skpTextFmt(skp, 2 * W / 16, 6 * H / 28, "%.2lf°", GC->rtcc->PZMCCPLN.AZ_min* DEG);
		skpTextFmt(skp, 2 * W / 16, 7 * H / 28, "%.2lf°", GC->rtcc->PZMCCPLN.AZ_max* DEG);

		skp->Text(1 * W / 16, 9 * H / 28, "MED F23", 7);
		skp->Text(1 * W / 16, 10 * H / 28, "Min/Max GET at Node (Modes 4/5)", 31);
		GET_Display(Buffer, GC->rtcc->PZMCCPLN.TLMIN*3600.0);
		skp->Text(2 * W / 16, 11 * H / 28, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->PZMCCPLN.TLMAX*3600.0);
		skp->Text(2 * W / 16, 12 * H / 28, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 14 * H / 28, "MED F24", 7);
		skp->Text(1 * W / 16, 15 * H / 28, "Gamma and Reentry Range", 23);
		skpTextFmt(skp, 2 * W / 16, 16 * H / 28, "%.2lf°", GC->rtcc->PZMCCPLN.gamma_reentry* DEG);
		skpTextFmt(skp, 2 * W / 16, 17 * H / 28, "%.0lf NM", GC->rtcc->PZMCCPLN.Reentry_range);

		skp->Text(1 * W / 16, 19 * H / 28, "MED F29", 7);
		skp->Text(1 * W / 16, 20 * H / 28, "Pericynthion height limits (Mode 9)", 35);
		skpTextFmt(skp, 2 * W / 16, 21 * H / 28, "Min: %.0lf NM", GC->rtcc->PZMCCPLN.H_PCYN_MIN / 1852.0);
		skpTextFmt(skp, 2 * W / 16, 22 * H / 28, "Max: %.0lf NM", GC->rtcc->PZMCCPLN.H_PCYN_MAX / 1852.0);

		skp->Text(1 * W / 16, 24 * H / 28, "Latitude Bias (Modes 8/9)", 25);
		skpTextFmt(skp, 2 * W / 16, 25 * H / 28, "%.1lf°", GC->rtcc->PZMCCPLN.LATBIAS* DEG);

		skp->Text(8 * W / 16, 4 * H / 28, "LOI/DOI Geometry (Modes 2/4)", 28);
		skpTextFmt(skp, 9 * W / 16, 5 * H / 28, "HALOI1 %.1lf", GC->rtcc->PZMCCPLN.H_A_LPO1 / 1852.0);
		skpTextFmt(skp, 9 * W / 16, 6 * H / 28, "HPLOI1 %.1lf", GC->rtcc->PZMCCPLN.H_P_LPO1 / 1852.0);
		skpTextFmt(skp, 9 * W / 16, 7 * H / 28, "HALOI2 %.1lf", GC->rtcc->PZMCCPLN.H_A_LPO2 / 1852.0);
		skpTextFmt(skp, 9 * W / 16, 8 * H / 28, "HPLOI2 %.2lf", GC->rtcc->PZMCCPLN.H_P_LPO2 / 1852.0);
		skpTextFmt(skp, 9 * W / 16, 9 * H / 28, "REVS1 %.2lf", GC->rtcc->PZMCCPLN.REVS1);
		skpTextFmt(skp, 9 * W / 16, 10 * H / 28, "REVS2 %d", GC->rtcc->PZMCCPLN.REVS2);
		skpTextFmt(skp, 9 * W / 16, 11 * H / 28, "SITEROT %.1lf°", GC->rtcc->PZMCCPLN.SITEROT* DEG);
		skpTextFmt(skp, 9 * W / 16, 12 * H / 28, "ETA1 %.3lf°", GC->rtcc->PZMCCPLN.ETA1* DEG);

		skp->Text(8 * W / 16, 14 * H / 28, "Mission Constants", 17);
		skpTextFmt(skp, 9 * W / 16, 15 * H / 28, "M %d", GC->rtcc->PZMCCPLN.LOPC_M);
		skpTextFmt(skp, 9 * W / 16, 16 * H / 28, "N %d", GC->rtcc->PZMCCPLN.LOPC_N);
		skpTextFmt(skp, 9 * W / 16, 17 * H / 28, "I PR MAX %.3lf°", GC->rtcc->PZMCCPLN.INCL_PR_MAX* DEG);

	}
	else if (screen == 81)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "NODAL TARGET CONVERSION", 30);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (G->NodeConvOpt)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Selenographic to EMP", 20);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "EMP to Selenographic", 20);
		}

		GET_Display3(Buffer, G->NodeConvGET);
		skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.3lf°", G->NodeConvLat* DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.3lf°", G->NodeConvLng* DEG);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.3lf NM", G->NodeConvHeight / 1852.0);

		skp->Text(9 * W / 16, 8 * H / 14, "Result:", 7);
		skpTextFmt(skp, 9 * W / 16, 9 * H / 14, "%.3lf° Lat", G->NodeConvResLat* DEG);
		skpTextFmt(skp, 9 * W / 16, 10 * H / 14, "%.3lf° Lng", G->NodeConvResLng* DEG);
	}
	else if (screen == 82)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "LOI Initialization", 18);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skpTextFmt(skp, 1 * W / 16, 2 * H / 14, "%.1lf NM", GC->rtcc->PZLOIPLN.HA_LLS);
		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.2lf NM", GC->rtcc->PZLOIPLN.HP_LLS);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.1lf°", GC->rtcc->PZLOIPLN.DW);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2lf", GC->rtcc->PZLOIPLN.REVS1);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%d", GC->rtcc->PZLOIPLN.REVS2);
		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.1lf°", GC->rtcc->PZLOIPLN.eta_1);

		skpTextFmt(skp, 5 * W / 8, 2 * H / 14, "%.1lf NM", GC->rtcc->PZLOIPLN.dh_bias);

		if (GC->rtcc->PZLOIPLN.PlaneSolnForInterSoln)
		{
			skp->Text(5 * W / 8, 4 * H / 14, "Plane solution", 18);
		}
		else
		{
			skp->Text(5 * W / 8, 4 * H / 14, "Min theta solution", 18);
		}
	}
	else if (screen == 83)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "LUNAR ORBIT INSERTION PLANNING (MSK 0078)", 41);
		skp->SetFont(font2);
		skp->Text(4 * W / 8, 71 * H / 128, "INTERSECTION", 12);
		skp->Text(4 * W / 8, 85 * H / 128, "COPLANAR", 8);
		skp->Text(4 * W / 8, 99 * H / 128, "MIN THETA", 9);
		skp->Text(4 * W / 8, 113 * H / 128, "PLANE", 5);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(5 * W / 32, 4 * H / 32, "CSM STA", 7);
		skp->Text(6 * W / 32, 5 * H / 32, "GET VECTOR", 10);

		Text(skp, 12 * W / 32, 4 * H / 32, GC->rtcc->PZLRBTI.StaID);
		GET_Display(Buffer, GC->rtcc->PZLRBTI.VectorGET, false);
		skp->Text(12 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));

		skp->Text(7 * W / 32, 7 * H / 32, "LAT LLS", 7);
		skp->Text(7 * W / 32, 8 * H / 32, "LNG LLS", 7);
		skp->Text(7 * W / 32, 9 * H / 32, "R LLS", 5);

		if (GC->rtcc->PZLRBTI.lat_lls >= 0)
		{
			sprintf(Buffer, "%.4lfN", GC->rtcc->PZLRBTI.lat_lls);
		}
		else
		{
			sprintf(Buffer, "%.4lfS", -GC->rtcc->PZLRBTI.lat_lls);
		}
		skp->Text(13 * W / 32, 7 * H / 32, Buffer, strlen(Buffer));
		if (GC->rtcc->PZLRBTI.lng_lls >= 0)
		{
			sprintf(Buffer, "%.4lfE", GC->rtcc->PZLRBTI.lng_lls);
		}
		else
		{
			sprintf(Buffer, "%.4lfW", -GC->rtcc->PZLRBTI.lng_lls);
		}
		skp->Text(13 * W / 32, 8 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 13 * W / 32, 9 * H / 32, "%.4lf", GC->rtcc->PZLRBTI.R_lls);

		skp->Text(18 * W / 32, 7 * H / 32, "REVS1", 5);
		skp->Text(18 * W / 32, 8 * H / 32, "REVS2", 5);
		skp->Text(18 * W / 32, 9 * H / 32, "DHBIAS", 6);
		if (GC->rtcc->PZLRBTI.planesoln)
		{
			skp->Text(18 * W / 32, 10 * H / 32, "PLANE", 5);
		}
		else
		{
			skp->Text(18 * W / 32, 10 * H / 32, "MIN THETA", 9);
		}

		skpTextFmt(skp, 21 * W / 32, 7 * H / 32, "%.2lf", GC->rtcc->PZLRBTI.REVS1);
		skpTextFmt(skp, 21 * W / 32, 8 * H / 32, "%d", GC->rtcc->PZLRBTI.REVS2);
		skpTextFmt(skp, 21 * W / 32, 9 * H / 32, "%+.1lf", GC->rtcc->PZLRBTI.DHBIAS);

		skp->Text(27 * W / 32, 7 * H / 32, "HALOI1", 6);
		skp->Text(27 * W / 32, 8 * H / 32, "HPLOI1", 6);
		skp->Text(27 * W / 32, 9 * H / 32, "HALOI2", 6);
		skp->Text(27 * W / 32, 10 * H / 32, "HPLOI2", 6);

		skpTextFmt(skp, 31 * W / 32, 7 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.HALOI1);
		skpTextFmt(skp, 31 * W / 32, 8 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.HPLOI1);
		skpTextFmt(skp, 31 * W / 32, 9 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.HALOI2);
		skpTextFmt(skp, 31 * W / 32, 10 * H / 32, "%.2lf", GC->rtcc->PZLRBTI.HPLOI2);

		skp->Text(6 * W / 32, 12 * H / 32, "AZMN FND", 8);
		skp->Text(6 * W / 32, 13 * H / 32, "AZMX FND", 8);

		skpTextFmt(skp, 10 * W / 32, 12 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.AZMN_f_ND);
		skpTextFmt(skp, 10 * W / 32, 13 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.AZMX_f_ND);

		skp->Text(17 * W / 32, 12 * H / 32, "AZLLS", 5);
		skp->Text(17 * W / 32, 13 * H / 32, "FLLS", 4);

		skpTextFmt(skp, 21 * W / 32, 12 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.AZ_LLS);
		skpTextFmt(skp, 21 * W / 32, 13 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.f_LLS);

		skp->Text(27 * W / 32, 12 * H / 32, "DVMAX+", 6);
		skp->Text(27 * W / 32, 13 * H / 32, "DVMAX-", 6);
		skp->Text(27 * W / 32, 14 * H / 32, "RA-RP GT", 8);

		skpTextFmt(skp, 31 * W / 32, 12 * H / 32, "%.0lf", GC->rtcc->PZLRBTI.DVMAXp);
		skpTextFmt(skp, 31 * W / 32, 13 * H / 32, "%.0lf", GC->rtcc->PZLRBTI.DVMAXm);
		skpTextFmt(skp, 31 * W / 32, 14 * H / 32, "%.1lf", GC->rtcc->PZLRBTI.RARPGT);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(2 * W / 32, 16 * H / 32, "CODE", 4);
		skp->Text(11 * W / 64, 16 * H / 32, "GETLOI", 6);
		skp->Text(19 * W / 64, 16 * H / 32, "DVLOI1", 6);
		skp->Text(27 * W / 64, 16 * H / 32, "DVLOI2", 6);
		skp->Text(17 * W / 32, 16 * H / 32, "HND", 3);
		skp->Text(20 * W / 32, 16 * H / 32, "FND/H", 5);
		skp->Text(93 * W / 128, 16 * H / 32, "HPC", 3);
		skp->Text(53 * W / 64, 16 * H / 32, "THETA", 5);
		skp->Text(30 * W / 32, 16 * H / 32, "FND/E", 5);
		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->SetPen(pen2);

		for (int i = 0;i < 5;i++)
		{
			//Horizontal lines
			skp->Line(1 * W / 64, (31 + 7 * i) * H / 64, 63 * W / 64, (31 + 7 * i) * H / 64);
			skp->Line(1 * W / 64, (35 + 7 * i) * H / 64, 63 * W / 64, (35 + 7 * i) * H / 64);
			//Vertical lines
			skp->Line(1 * W / 64, (31 + 7 * i) * H / 64, 1 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(7 * W / 64, (31 + 7 * i) * H / 64, 7 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(15 * W / 64, (31 + 7 * i) * H / 64, 15 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(23 * W / 64, (31 + 7 * i) * H / 64, 23 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(31 * W / 64, (31 + 7 * i) * H / 64, 31 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(73 * W / 128, (31 + 7 * i) * H / 64, 73 * W / 128, (35 + 7 * i) * H / 64);
			skp->Line(43 * W / 64, (31 + 7 * i) * H / 64, 43 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(49 * W / 64, (31 + 7 * i) * H / 64, 49 * W / 64, (35 + 7 * i) * H / 64);
			skp->Line(113 * W / 128, (31 + 7 * i) * H / 64, 113 * W / 128, (35 + 7 * i) * H / 64);
			skp->Line(63 * W / 64, (31 + 7 * i) * H / 64, 63 * W / 64, (35 + 7 * i) * H / 64);
		}		

		for (int i = 0;i < 4;i++)
		{
			skp->Text(2 * W / 32, (38 + 7 * i) * H / 64, "%d", 2 * i + 1);
			skp->Text(2 * W / 32, (40 + 7 * i) * H / 64, "%d", 2 * i + 2);

			GET_Display_HHMM(Buffer, GC->rtcc->PZLRBTI.sol[2 * i].GETLOI);
			skp->Text(7 * W / 32, (38 + 7 * i) * H / 64, Buffer, strlen(Buffer));
			GET_Display_HHMM(Buffer, GC->rtcc->PZLRBTI.sol[2 * i + 1].GETLOI);
			skp->Text(7 * W / 32, (40 + 7 * i) * H / 64, Buffer, strlen(Buffer));

			skpTextFmt(skp, 11 * W / 32, (38 + 7 * i) * H / 64, "%.0lf", GC->rtcc->PZLRBTI.sol[2 * i].DVLOI1);
			skpTextFmt(skp, 11 * W / 32, (40 + 7 * i) * H / 64, "%.0lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].DVLOI1);

			skpTextFmt(skp, 15 * W / 32, (38 + 7 * i) * H / 64, "%.0lf", GC->rtcc->PZLRBTI.sol[2 * i].DVLOI2);
			skpTextFmt(skp, 15 * W / 32, (40 + 7 * i) * H / 64, "%.0lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].DVLOI2);

			skpTextFmt(skp, 18 * W / 32, (38 + 7 * i) * H / 64, "%.1lf", GC->rtcc->PZLRBTI.sol[2 * i].H_ND);
			skpTextFmt(skp, 18 * W / 32, (40 + 7 * i) * H / 64, "%.1lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].H_ND);

			skpTextFmt(skp, 21 * W / 32, (38 + 7 * i) * H / 64, "%.1lf", GC->rtcc->PZLRBTI.sol[2 * i].f_ND_H);
			skpTextFmt(skp, 21 * W / 32, (40 + 7 * i) * H / 64, "%.1lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].f_ND_H);

			skpTextFmt(skp, 24 * W / 32, (38 + 7 * i) * H / 64, "%.0lf", GC->rtcc->PZLRBTI.sol[2 * i].H_PC);
			skpTextFmt(skp, 24 * W / 32, (40 + 7 * i) * H / 64, "%.0lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].H_PC);

			skpTextFmt(skp, 28 * W / 32, (38 + 7 * i) * H / 64, "%.2lf", GC->rtcc->PZLRBTI.sol[2 * i].Theta);
			skpTextFmt(skp, 28 * W / 32, (40 + 7 * i) * H / 64, "%.2lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].Theta);

			skpTextFmt(skp, 31 * W / 32, (38 + 7 * i) * H / 64, "%.1lf", GC->rtcc->PZLRBTI.sol[2 * i].f_ND_E);
			skpTextFmt(skp, 31 * W / 32, (40 + 7 * i) * H / 64, "%.1lf", GC->rtcc->PZLRBTI.sol[2 * i + 1].f_ND_E);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);
	}
	else if (screen == 84)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 1 * H / 14, "DIRECT INPUT TO MPT (MED M66)", 29);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		if (GC->rtcc->med_m66.UllageDT < 0)
		{
			strcpy(Buffer, "Default");
		}
		else
		{
			sprintf_s(Buffer, "%.0lfs Ullage DT", GC->rtcc->med_m66.UllageDT);
		}
		skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_m66.UllageQuads)
		{
			skp->Text(1 * W / 16, 4 * H / 14, "4 Thrusters", 11);
		}
		else
		{
			skp->Text(1 * W / 16, 4 * H / 14, "2 Thrusters", 11);
		}
		
		if (GC->rtcc->med_m66.BurnParamNo == 1 && GC->rtcc->med_m66.CoordInd >= 1)
		{
			skp->Text(1 * W / 16, 6 * H / 14, "REFSMMAT: CUR", 13);
		}

		if (GC->rtcc->med_m66.ConfigChangeInd == 0)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "No change", 9);
		}
		else
		{
			if (GC->rtcc->med_m66.ConfigChangeInd == 1)
			{
				skp->Text(1 * W / 16, 8 * H / 14, "Undocking", 9);
			}
			else if (GC->rtcc->med_m66.ConfigChangeInd == 2)
			{
				skp->Text(1 * W / 16, 8 * H / 14, "Docking", 7);
			}

			Text(skp, 1 * W / 16, 10 * H / 14, GC->rtcc->med_m66.FinalConfig);
		}

		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%+.1lf° Delta Docking Angle", GC->rtcc->med_m66.DeltaDA* DEG);

		if (GC->rtcc->med_m66.TrimAngleIndicator == 0)
		{
			skp->Text(10 * W / 16, 2 * H / 14, "Compute Trim", 12);
		}
		else
		{
			skp->Text(10 * W / 16, 2 * H / 14, "System Parameters", 17);
		}

		skp->Text(10 * W / 16, 4 * H / 14, "Transfer to MPT", 15);
		skp->Text(10 * W / 16, 10 * H / 14, "Page 2/2", 8);
	}
	else if (screen == 85)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 28, "EXPERIMENTAL SITE ACQUISITION (MSK 1506)", 40);

		skp->SetFont(font2);

		skp->Text(22 * W / 32, 4 * H / 28, "STA ID", 6);
		skp->Text(2 * W / 32, 6 * H / 28, "REV", 3);
		skp->Text(5 * W / 32, 6 * H / 28, "STA", 3);
		skp->Text(9 * W / 32, 6 * H / 28, "GETAOS", 6);
		skp->Text(55 * W / 128, 6 * H / 28, "GNDRNG", 6);
		skp->Text(17 * W / 32, 6 * H / 28, "ALT", 3);
		skp->Text(20 * W / 32, 6 * H / 28, "EMAX", 4);
		skp->Text(24 * W / 32, 6 * H / 28, "GETCA", 5);
		skp->Text(29 * W / 32, 6 * H / 28, "GETLOS", 8);

		skpTextFmt(skp, 6 * W / 32, 4 * H / 28, "PAGE %02d OF %02d", GC->rtcc->EZDPSAD2.curpage, GC->rtcc->EZDPSAD2.pages);

		for (unsigned i = 0;i < GC->rtcc->EZDPSAD2.numcontacts[GC->rtcc->EZDPSAD2.curpage - 1];i++)
		{
			skpTextFmt(skp, 2 * W / 32, (i + 8) * H / 28, "%03d", GC->rtcc->EZDPSAD2.REV[GC->rtcc->EZDPSAD2.curpage - 1][i]);

			Text(skp, 5 * W / 32, (i + 8) * H / 28, GC->rtcc->EZDPSAD2.STA[GC->rtcc->EZDPSAD2.curpage - 1][i]);

			if (GC->rtcc->EZDPSAD2.BestAvailableAOS[GC->rtcc->EZDPSAD2.curpage - 1][i])
			{
				skp->Text(8 * W / 32, (i + 8) * H / 28, "*", 1);
			}
			GET_Display(Buffer, GC->rtcc->EZDPSAD2.GETAOS[GC->rtcc->EZDPSAD2.curpage - 1][i], false);
			skp->Text(9 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));

			skpTextFmt(skp, 14 * W / 32, (i + 8) * H / 28, "%.1f", GC->rtcc->EZDPSAD2.GNDRNG[GC->rtcc->EZDPSAD2.curpage - 1][i]);

			skpTextFmt(skp, 17 * W / 32, (i + 8) * H / 28, "%04.1f", GC->rtcc->EZDPSAD2.ALT[GC->rtcc->EZDPSAD2.curpage - 1][i]);

			if (GC->rtcc->EZDPSAD2.BestAvailableEMAX[GC->rtcc->EZDPSAD2.curpage - 1][i])
			{
				skp->Text(19 * W / 32, (i + 8) * H / 28, "*", 1);
			}
			skpTextFmt(skp, 20 * W / 32, (i + 8) * H / 28, "%04.1f", GC->rtcc->EZDPSAD2.ELMAX[GC->rtcc->EZDPSAD2.curpage - 1][i]);

			GET_Display(Buffer, GC->rtcc->EZDPSAD2.GETCA[GC->rtcc->EZDPSAD2.curpage - 1][i], false);
			skp->Text(24 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));

			if (GC->rtcc->EZDPSAD2.BestAvailableLOS[GC->rtcc->EZDPSAD2.curpage - 1][i])
			{
				skp->Text(27 * W / 32, (i + 8) * H / 28, "*", 1);
			}
			GET_Display(Buffer, GC->rtcc->EZDPSAD2.GETLOS[GC->rtcc->EZDPSAD2.curpage - 1][i], false);
			skp->Text(29 * W / 32, (i + 8) * H / 28, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 86)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "RELATIVE MOTION DIGITALS (MSK 0060)", 35);
		skp->SetFont(font2);

		Text(skp, 16 * W / 32, 31 * H / 32, GC->rtcc->EZRMDT.error);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->Text(1 * W / 32, 4 * H / 32, "CSM STAID", 9);
		skp->Text(1 * W / 32, 5 * H / 32, "GMTV", 4);
		GET_Display(Buffer, GC->rtcc->EZRMDT.CSMGMTV, false);
		skp->Text(4 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
		skp->Text(1 * W / 32, 6 * H / 32, "GETV", 4);
		GET_Display(Buffer, GC->rtcc->EZRMDT.CSMGETV, false);
		skp->Text(4 * W / 32, 6 * H / 32, Buffer, strlen(Buffer));
		skp->Text(1 * W / 32, 7 * H / 32, "MODE", 4);
		skpTextFmt(skp, 4 * W / 32, 7 * H / 32, "%c", GC->rtcc->EZRMDT.Mode);
		skpTextFmt(skp, 5 * W / 128, 8 * H / 32, "GET%s", GC->rtcc->EZRMDT.PETorSH.c_str());

		skp->Text(10 * W / 32, 4 * H / 32, "LM STAID", 8);
		skp->Text(10 * W / 32, 5 * H / 32, "GMTV", 4);
		GET_Display(Buffer, GC->rtcc->EZRMDT.LMGMTV, false);
		skp->Text(13 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
		skp->Text(10 * W / 32, 6 * H / 32, "GETV", 4);
		GET_Display(Buffer, GC->rtcc->EZRMDT.LMGETV, false);
		skp->Text(13 * W / 32, 6 * H / 32, Buffer, strlen(Buffer));

		skp->Text(19 * W / 32, 4 * H / 32, "GETR", 4);
		GET_Display(Buffer, GC->rtcc->EZRMDT.GETR, false);
		skp->Text(22 * W / 32, 4 * H / 32, Buffer, strlen(Buffer));
		skp->Text(19 * W / 32, 5 * H / 32, "VEH", 3);
		Text(skp, 43 * W / 64, 5 * H / 32, GC->rtcc->EZRMDT.VEH);

		skpTextFmt(skp, 25 * W / 32, 5 * H / 32, "%c", GC->rtcc->EZRMDT.AXIS);
		skp->Text(26 * W / 32, 5 * H / 32, "AXIS AT TGT", 11);
		skp->Text(19 * W / 32, 6 * H / 32, "REFSMMAT NO", 11);
		Text(skp, 53 * W / 64, 6 * H / 32, GC->rtcc->EZRMDT.REFSMMAT);

		skp->SetPen(pen2);
		skp->Line(5 * W / 32, 15 * H / 64, W, 15 * H / 64);
		skp->Line(11 * W / 64, 15 * H / 64, 11 * W / 64, 62 * H / 64);
		skp->Line(18 * W / 64, 15 * H / 64, 18 * W / 64, 62 * H / 64);
		skp->Line(26 * W / 64, 15 * H / 64, 26 * W / 64, 62 * H / 64);
		skp->Line(33 * W / 64, 15 * H / 64, 33 * W / 64, 62 * H / 64);
		skp->Line(40 * W / 64, 15 * H / 64, 40 * W / 64, 62 * H / 64);
		skp->Line(48 * W / 64, 15 * H / 64, 48 * W / 64, 62 * H / 64);
		skp->Line(56 * W / 64, 15 * H / 64, 56 * W / 64, 62 * H / 64);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skpTextFmt(skp, 29 * W / 128, 8 * H / 32, "R%s", GC->rtcc->EZRMDT.YDotorT.c_str());
		if (GC->rtcc->EZRMDT.Mode == '1')
		{
			skp->Text(62 * W / 256, 57 * H / 256, ".", 1);
		}
		skp->Text(81 * W / 256, 57 * H / 256, ".", 1);
		skp->Text(22 * W / 64, 8 * H / 32, "R/PB", 4);
		skp->Text(59 * W / 128, 8 * H / 32, "AZ/YB", 5);
		skp->Text(73 * W / 128, 8 * H / 32, "EL/RB", 5);
		skpTextFmt(skp, 89 * W / 128, 8 * H / 32, "X/P%c", GC->rtcc->EZRMDT.Pitch);
		skpTextFmt(skp, 26 * W / 32, 8 * H / 32, "Z/Y%c", GC->rtcc->EZRMDT.Yaw);
		skpTextFmt(skp, 30 * W / 32, 8 * H / 32, "Y/R%c", GC->rtcc->EZRMDT.Roll);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);
		for (int i = 0;i < GC->rtcc->EZRMDT.solns;i++)
		{
			GET_Display(Buffer, GC->rtcc->EZRMDT.data[i].GET, false);
			skp->Text(5 * W / 32, (9 + i * 2) * H / 32, Buffer, strlen(Buffer));

			if (GC->rtcc->EZRMDT.Mode == '1')
			{
				GET_Display(Buffer, GC->rtcc->EZRMDT.data[i].PETorShaft, false);
			}
			else
			{
				sprintf_s(Buffer, "%.1lf", GC->rtcc->EZRMDT.data[i].PETorShaft);
			}
			skp->Text(5 * W / 32, (10 + i * 2) * H / 32, Buffer, strlen(Buffer));
			if (GC->rtcc->EZRMDT.data[i].R <= 9999.9)
			{
				sprintf_s(Buffer, "%.1lf", GC->rtcc->EZRMDT.data[i].R);
				skp->Text(35 * W / 128, (9 + i * 2) * H / 32, Buffer, strlen(Buffer));
			}
			if (GC->rtcc->EZRMDT.Mode == '1')
			{
				if (abs(GC->rtcc->EZRMDT.data[i].YdotorTrun) <= 999.9)
				{
					skpTextFmt(skp, 35 * W / 128, (10 + i * 2) * H / 32, "%+.1lf", GC->rtcc->EZRMDT.data[i].YdotorTrun);
				}
			}
			else
			{
				skpTextFmt(skp, 35 * W / 128, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].YdotorTrun);
			}
			
			if (abs(GC->rtcc->EZRMDT.data[i].RDOT) <= 999.9)
			{
				skpTextFmt(skp, 25 * W / 64, (9 + i * 2) * H / 32, "%+.1lf", GC->rtcc->EZRMDT.data[i].RDOT);
			}
			skpTextFmt(skp, 25 * W / 64, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].PB);
			skpTextFmt(skp, 32 * W / 64, (9 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].AZH);
			skpTextFmt(skp, 32 * W / 64, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].YB);
			skpTextFmt(skp, 39 * W / 64, (9 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].EL);
			skpTextFmt(skp, 39 * W / 64, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].RB);
			if (GC->rtcc->EZRMDT.Mode == '1' && abs(GC->rtcc->EZRMDT.data[i].X) <= 9999.9)
			{
				skpTextFmt(skp, 95 * W / 128, (9 + i * 2) * H / 32, "%.1lf%c", GC->rtcc->EZRMDT.data[i].X, GC->rtcc->EZRMDT.data[i].XInd);
			}
			skpTextFmt(skp, 95 * W / 128, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].Pitch);
			if (GC->rtcc->EZRMDT.Mode == '1' && abs(GC->rtcc->EZRMDT.data[i].Z) <= 9999.9)
			{
				skpTextFmt(skp, 111 * W / 128, (9 + i * 2) * H / 32, "%.1lf%c", GC->rtcc->EZRMDT.data[i].Z, GC->rtcc->EZRMDT.data[i].ZInd);
			}
			skpTextFmt(skp, 111 * W / 128, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].Yaw);
			if (GC->rtcc->EZRMDT.Mode == '1' && abs(GC->rtcc->EZRMDT.data[i].Y) <= 999.9)
			{
				skpTextFmt(skp, 63 * W / 64, (9 + i * 2) * H / 32, "%.1lf%c", GC->rtcc->EZRMDT.data[i].Y, GC->rtcc->EZRMDT.data[i].YInd);
			}
			skpTextFmt(skp, 63 * W / 64, (10 + i * 2) * H / 32, "%.1lf", GC->rtcc->EZRMDT.data[i].Roll);
		}
	}
	else if (screen == 87)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "RENDEZVOUS EVALUATION DISPLAY (MSK 0058)", 40);

		Text(skp, 32 * W / 64, 30 * H / 32, GC->rtcc->PZREDT.ErrorMessage);
		skp->SetFont(font2);

		skp->Text(6 * W / 32, 4 * H / 32, "ID", 2);
		skp->Text(12 * W / 32, 4 * H / 32, "M", 1);

		skp->Text(4 * W / 64, 6 * H / 32, "GET", 3);
		skp->Text(14 * W / 64, 6 * H / 32, "DT", 2);
		skp->Text(22 * W / 64, 6 * H / 32, "DV", 2);
		skp->Text(27 * W / 64, 6 * H / 32, "VEH", 3);
		skp->Text(33 * W / 64, 6 * H / 32, "PURP", 4);
		//skp->Text(39 * W / 64, 6 * H / 32, "CODE", 4);
		skp->Text(40 * W / 64, 6 * H / 32, "PHASE", 5);
		skp->Text(48 * W / 64, 6 * H / 32, "HEIGHT", 6);
		skp->Text(55 * W / 64, 6 * H / 32, "HA", 2);
		skp->Text(61 * W / 64, 6 * H / 32, "HP", 2);

		skp->Text(6 * W / 64, 19 * H / 32, "PITCH", 5);
		skp->Text(14 * W / 64, 19 * H / 32, "YAW", 3);
		skp->Text(22 * W / 64, 19 * H / 32, "VX", 2);
		skp->Text(30 * W / 64, 19 * H / 32, "VY", 2);
		skp->Text(38 * W / 64, 19 * H / 32, "VZ", 2);

		skpTextFmt(skp, 8 * W / 32, 4 * H / 32, "%d", GC->rtcc->PZREDT.ID);
		if (GC->rtcc->PZREDT.isDKI)
		{
			skpTextFmt(skp, 14 * W / 32, 4 * H / 32, "%d", GC->rtcc->PZREDT.M);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);
		for (int i = 0;i < GC->rtcc->PZREDT.NumMans;i++)
		{
			GET_Display3(Buffer, GC->rtcc->PZREDT.GET[i]);
			skp->Text(11 * W / 64, (7 + i * 2) * H / 32, Buffer, strlen(Buffer));
			if (i > 0)
			{
				GET_Display3(Buffer, GC->rtcc->PZREDT.DT[i]);
				skp->Text(20 * W / 64, (6 + i * 2) * H / 32, Buffer, strlen(Buffer));
			}
			skpTextFmt(skp, 24 * W / 64, (7 + i * 2) * H / 32, "%.2lf", GC->rtcc->PZREDT.DV[i]);
			Text(skp, 29 * W / 64, (7 + i * 2) * H / 32, GC->rtcc->PZREDT.VEH[i]);
			Text(skp, 35 * W / 64, (7 + i * 2) * H / 32, GC->rtcc->PZREDT.PURP[i]);
			if (GC->rtcc->PZREDT.isDKI)
			{
				//skpTextFmt(skp, 40 * W / 64, (7 + i * 2) * H / 32, "%.2lf", GC->rtcc->PZREDT.CODE[i]);
			}
			skpTextFmt(skp, 43 * W / 64, (7 + i * 2) * H / 32, "%.4lf", GC->rtcc->PZREDT.PHASE[i]);
			skpTextFmt(skp, 51 * W / 64, (7 + i * 2) * H / 32, "%.4lf", GC->rtcc->PZREDT.HEIGHT[i]);
			skpTextFmt(skp, 57 * W / 64, (7 + i * 2) * H / 32, "%.2lf", GC->rtcc->PZREDT.HA[i]);
			skpTextFmt(skp, 63 * W / 64, (7 + i * 2) * H / 32, "%.2lf", GC->rtcc->PZREDT.HP[i]);

			skpTextFmt(skp, 8 * W / 64, (20 + i) * H / 32, "%.3lf", GC->rtcc->PZREDT.Pitch[i]);
			skpTextFmt(skp, 16 * W / 64, (20 + i) * H / 32, "%.3lf", GC->rtcc->PZREDT.Yaw[i]);
			skpTextFmt(skp, 24 * W / 64, (20 + i) * H / 32, "%.3lf", GC->rtcc->PZREDT.DVVector[i].x);
			skpTextFmt(skp, 32 * W / 64, (20 + i) * H / 32, "%.3lf", GC->rtcc->PZREDT.DVVector[i].y);
			skpTextFmt(skp, 40 * W / 64, (20 + i) * H / 32, "%.3lf", GC->rtcc->PZREDT.DVVector[i].z);
		}
	}
	else if (screen == 88)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "Init for LM Targeting and Launch Window (K13)", 45);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skpTextFmt(skp, 1 * W / 16, 2 * H / 14, "%.3lf°", GC->rtcc->PZLTRT.PoweredFlightArc* DEG);
		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.1lf s", GC->rtcc->PZLTRT.PoweredFlightTime);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.0lf ft", GC->rtcc->PZLTRT.InsertionHeight / 0.3048);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.1lf ft/s", GC->rtcc->PZLTRT.InsertionHorizontalVelocity / 0.3048);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.1lf ft/s", GC->rtcc->PZLTRT.InsertionRadialVelocity / 0.3048);
		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.2lf°", GC->rtcc->PZLTRT.YawSteerCap* DEG);
		skpTextFmt(skp, 10 * W / 16, 2 * H / 14, "%.2lf hrs", GC->rtcc->PZLTRT.MaxAscLifetime / 3600.0);
		skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%.2lf NM", GC->rtcc->PZLTRT.MinSafeHeight / 1852.0);
		skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%.2lf ft/s", GC->rtcc->PZLTRT.LMMaxDeltaV / 0.3048);
		skpTextFmt(skp, 10 * W / 16, 8 * H / 14, "%.2lf ft/s", GC->rtcc->PZLTRT.CSMMaxDeltaV / 0.3048);
	}
	else if (screen == 89)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "LAUNCH WINDOW INITIALIZATION (K14)", 34);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 16, 2 * H / 14, "Targeting Parameters", 34);
		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.1lf min", GC->rtcc->PZLTRT.dt_bias / 60.0);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.2lf°", GC->rtcc->PZLTRT.ElevationAngle* DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2lf°", GC->rtcc->PZLTRT.TerminalPhaseTravelAngle* DEG);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.2lf NM", GC->rtcc->PZLTRT.TPF_Height_Offset / 1852.0);
		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.2lf°", GC->rtcc->PZLTRT.TPF_Phase_Offset* DEG);
		skpTextFmt(skp, 10 * W / 16, 2 * H / 14, "%.2lf NM", GC->rtcc->PZLTRT.Height_Diff_Begin / 1852.0);
		skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%.2lf NM", GC->rtcc->PZLTRT.Height_Diff_Incr / 1852.0);
	}
	else if (screen == 90)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "LUNAR RENDEZVOUS PLAN TABLE", 27);
		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 32, 4 * H / 32, "CSM STA", 7);
		skp->Text(1 * W / 32, 5 * H / 32, "GMTV", 4);
		skp->Text(1 * W / 32, 6 * H / 32, "GETV", 4);
		skp->Text(1 * W / 32, 7 * H / 32, "MVR VEH", 7);

		skp->Text(9 * W / 32, 4 * H / 32, "LM POSITION", 11);
		skp->TextW(9 * W / 32, 5 * H / 32, L"\u03C6LLS", 4);
		skp->TextW(9 * W / 32, 6 * H / 32, L"\u03BBLLS", 4);
		skp->Text(9 * W / 32, 7 * H / 32, "THT", 3);

		skp->Text(33 * W / 64, 4 * H / 32, "LM STA", 6);
		skp->Text(33 * W / 64, 5 * H / 32, "GMTV", 4);
		skp->Text(33 * W / 64, 6 * H / 32, "GETV", 4);
		skp->Text(33 * W / 64, 7 * H / 32, "DTCSI", 5);

		skp->Text(49 * W / 64, 4 * H / 32, "LM LIFE", 11);
		skp->Text(49 * W / 64, 5 * H / 32, "DV MAX", 6);
		skp->Text(49 * W / 64, 6 * H / 32, "MIN H", 5);
		skp->Text(49 * W / 64, 7 * H / 32, "WT", 2);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(2 * W / 32, 9 * H / 32, "ID", 2);
		skp->Text(4 * W / 32, 9 * H / 32, "M", 1);
		skp->Text(6 * W / 32, 9 * H / 32, "DH", 2);
		skp->Text(10 * W / 32, 9 * H / 32, "GETLO", 5);
		skp->Text(10 * W / 32, 10 * H / 32, "GETINS", 6);
		skp->Text(15 * W / 32, 9 * H / 32, "GETCSI", 6);
		skp->Text(15 * W / 32, 10 * H / 32, "DVCSI", 5);
		skp->Text(20 * W / 32, 9 * H / 32, "GETCDH", 6);
		skp->Text(20 * W / 32, 10 * H / 32, "DVCDH", 5);
		skp->Text(25 * W / 32, 9 * H / 32, "GETTPI", 6);
		skp->Text(25 * W / 32, 10 * H / 32, "DVTPI", 5);
		skp->Text(30 * W / 32, 9 * H / 32, "DVTPF", 5);
		skp->Text(30 * W / 32, 10 * H / 32, "DVT", 3);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		if (GC->MissionPlanningActive)
		{
			GET_Display(Buffer, GC->rtcc->GMTfromGET(GC->rtcc->med_k15.CSMVectorTime), false);
			skp->Text(8 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->med_k15.CSMVectorTime, false);
			skp->Text(8 * W / 32, 6 * H / 32, Buffer, strlen(Buffer));
		}

		if (GC->rtcc->med_k15.Chaser == 1)
		{
			skp->Text(8 * W / 32, 7 * H / 32, "CSM", 3);
		}
		else
		{
			skp->Text(8 * W / 32, 7 * H / 32, "LEM", 3);
		}

		skpTextFmt(skp, 31 * W / 64, 5 * H / 32, "%+.4lf", GC->rtcc->BZLAND.lat[0] * DEG);
		skpTextFmt(skp, 31 * W / 64, 6 * H / 32, "%+.4lf", GC->rtcc->BZLAND.lng[0] * DEG);
		GET_Display(Buffer, GC->rtcc->med_k15.ThresholdTime, false);
		skp->Text(31 * W / 64, 7 * H / 32, Buffer, strlen(Buffer));

		if (GC->rtcc->med_k15.CSI_Flag > 0)
		{
			GET_Display(Buffer, GC->rtcc->med_k15.CSI_Flag, false);
			skp->Text(24 * W / 32, 7 * H / 32, Buffer, strlen(Buffer));
		}

		skpTextFmt(skp, 31 * W / 32, 4 * H / 32, "%.1lf h", GC->rtcc->PZLTRT.MaxAscLifetime / 3600.0);
		if (GC->rtcc->med_k15.Chaser == 1)
		{
			sprintf_s(Buffer, "%.0lf", GC->rtcc->PZLTRT.CSMMaxDeltaV / 0.3048);
		}
		else
		{
			sprintf_s(Buffer, "%.0lf", GC->rtcc->PZLTRT.LMMaxDeltaV / 0.3048);
		}
		skp->Text(31 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 31 * W / 32, 6 * H / 32, "%.1lf", GC->rtcc->PZLTRT.MinSafeHeight / 1852.0);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		
		for (int i = 0;i < GC->rtcc->PZLRPT.plans;i++)
		{
			skpTextFmt(skp, 2 * W / 32, (12 + 3 * i) * H / 32, "%d", GC->rtcc->PZLRPT.data[i].ID);
			skpTextFmt(skp, 4 * W / 32, (12 + 3 * i) * H / 32, "%d", GC->rtcc->PZLRPT.data[i].N);
			skpTextFmt(skp, 6 * W / 32, (12 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZLRPT.data[i].DH);
			GET_Display(Buffer, GC->rtcc->PZLRPT.data[i].GETLO, false);
			skp->Text(10 * W / 32, (12 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->PZLRPT.data[i].T_INS, false);
			skp->Text(10 * W / 32, (13 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->PZLRPT.data[i].T_CSI, false);
			skp->Text(15 * W / 32, (12 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 15 * W / 32, (13 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZLRPT.data[i].DVCSI);
			GET_Display(Buffer, GC->rtcc->PZLRPT.data[i].T_CDH, false);
			skp->Text(20 * W / 32, (12 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 20 * W / 32, (13 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZLRPT.data[i].DVCDH);
			GET_Display(Buffer, GC->rtcc->PZLRPT.data[i].T_TPI, false);
			skp->Text(25 * W / 32, (12 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 25 * W / 32, (13 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZLRPT.data[i].DVTPI);
			skpTextFmt(skp, 30 * W / 32, (12 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZLRPT.data[i].DVTPF);
			skpTextFmt(skp, 30 * W / 32, (13 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZLRPT.data[i].DVT);
		}
	}
	else if (screen == 91)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->SetFont(font2);
		skp->SetPen(pen2);
		skp->Text(4 * W / 8, 3 * H / 32, "LUNAR LAUNCH TARGETING TABLE", 39);
		skp->Text(30 * W / 32, 3 * H / 32, "0077", 4);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->Text(1 * W / 32, 4 * H / 32, "CSM STA ID", 10);
		skp->Text(1 * W / 32, 5 * H / 32, "GETTH", 5);
		skp->Text(1 * W / 32, 6 * H / 32, "GETV", 4);
		skp->Text(17 * W / 32, 4 * H / 32, "LM POSITION", 11);

		skp->TextW(24 * W / 32, 4 * H / 32, L"\u03C6LLS", 4);
		skp->TextW(24 * W / 32, 5 * H / 32, L"\u03BBLLS", 4);
		skp->Text(24 * W / 32, 6 * H / 32, "RLLS", 4);

		skp->Line(1 * W / 64, 15 * H / 64, 63 * W / 64, 15 * H / 64);

		skp->TextW(1 * W / 32, 9 * H / 32, L"PF\u2222", 3);
		skp->TextW(1 * W / 32, 10 * H / 32, L"PF\u0394T", 4);
		skp->Text(1 * W / 32, 11 * H / 32, "HINS", 4);
		skp->TextW(1 * W / 32, 12 * H / 32, L"\u1E58INS", 4);
		skp->Text(1 * W / 32, 13 * H / 32, "YSTEER", 6);

		skp->TextW(11 * W / 32, 9 * H / 32, L"\u0394T(INS-TPI)", 11);
		skp->TextW(11 * W / 32, 10 * H / 32, L"\u0394TNOM", 5);

		skp->TextW(23 * W / 32, 9 * H / 32, L"\u0394HTPI", 5);
		skp->TextW(23 * W / 32, 10 * H / 32, L"\u0394\u03B8TPI", 5);
		skp->TextW(23 * W / 32, 11 * H / 32, L"\u03C9T", 2);
		skp->Text(23 * W / 32, 12 * H / 32, "HPMIN", 5);
		skp->TextW(23 * W / 32, 13 * H / 32, L"EL \u2222", 4);

		skp->Line(1 * W / 64, 16 * H / 32, 63 * W / 64, 16 * H / 32);

		skp->Text(1 * W / 32, 17 * H / 32, "GETLOR", 6);
		skp->Text(16 * W / 32, 17 * H / 32, "VH", 2);

		skp->Line(1 * W / 64, 19 * H / 32, 63 * W / 64, 19 * H / 32);

		skp->Text(1 * W / 32, 19 * H / 32, "CODE", 4);
		skp->Text(5 * W / 32, 19 * H / 32, "GET", 3);
		skp->TextW(11 * W / 32, 19 * H / 32, L"\u0394V", 2);
		skp->TextW(15 * W / 32, 19 * H / 32, L"\u0394VX", 3);
		skp->TextW(20 * W / 32, 19 * H / 32, L"\u0394VY", 3);
		skp->TextW(25 * W / 32, 19 * H / 32, L"\u0394VZ", 3);
		skp->Text(28 * W / 32, 19 * H / 32, "HA/HP", 5);

		skp->Line(1 * W / 64, 41 * H / 64, 63 * W / 64, 41 * H / 64);

		skp->Text(1 * W / 32, 21 * H / 32, "TPI", 3);
		skp->Text(1 * W / 32, 23 * H / 32, "TPF", 3);

		skp->Line(1 * W / 64, 25 * H / 32, 63 * W / 64, 25 * H / 32);

		skp->TextW(25 * W / 32, 27 * H / 32, L"H\u2090T", 3);
		skp->TextW(25 * W / 32, 28 * H / 32, L"H\u209AT", 3);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		if (GC->MissionPlanningActive)
		{
			//TBD: STA ID

			GET_Display(Buffer, GC->rtcc->med_k50.GETV, false);
			skp->Text(8 * W / 32, 6 * H / 32, Buffer, strlen(Buffer));
		}
		else
		{
			if (G->target != NULL)
			{
				sprintf(Buffer, G->target->GetName());
				skp->Text(12 * W / 32, 4 * H / 32, Buffer, strlen(Buffer));
			}
			else
			{
				skp->Text(12 * W / 32, 4 * H / 32, "No Target!", 10);
			}
		}

		GET_Display(Buffer, GC->rtcc->med_k50.GETTH, false);
		skp->Text(9 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));

		skpTextFmt(skp, 31 * W / 32, 4 * H / 32, "%+.4lf", GC->rtcc->BZLAND.lat[RTCC_LMPOS_BEST] * DEG);
		skpTextFmt(skp, 31 * W / 32, 5 * H / 32, "%+.4lf", GC->rtcc->BZLAND.lng[RTCC_LMPOS_BEST] * DEG);
		skpTextFmt(skp, 31 * W / 32, 6 * H / 32, "%.4lf", GC->rtcc->BZLAND.rad[RTCC_LMPOS_BEST] / 1852.0);

		skpTextFmt(skp, 8 * W / 32, 9 * H / 32, "%.3lf", GC->rtcc->PZLTRT.PoweredFlightArc* DEG);
		skpTextFmt(skp, 8 * W / 32, 10 * H / 32, "%.2lf", GC->rtcc->PZLTRT.PoweredFlightTime);
		skpTextFmt(skp, 8 * W / 32, 11 * H / 32, "%.0lf", GC->rtcc->PZLTRT.InsertionHeight / 0.3048);
		skpTextFmt(skp, 8 * W / 32, 12 * H / 32, "%+.1lf", GC->rtcc->PZLTRT.InsertionRadialVelocity / 0.3048);
		skpTextFmt(skp, 8 * W / 32, 13 * H / 32, "%.2lf", GC->rtcc->PZLTRT.YawSteerCap* DEG);

		skpTextFmt(skp, 20 * W / 32, 9 * H / 32, "%.2lf", GC->rtcc->PZLTRT.DT_Ins_TPI / 60.0);
		skpTextFmt(skp, 20 * W / 32, 10 * H / 32, "%.2lf", GC->rtcc->PZLTRT.DT_Ins_TPI_NOM / 60.0);

		skpTextFmt(skp, 31 * W / 32, 9 * H / 32, "%+.2lf", GC->rtcc->PZLTRT.DT_DH / 1852.0);
		skpTextFmt(skp, 31 * W / 32, 10 * H / 32, "%+.2lf", GC->rtcc->PZLTRT.DT_Theta_i * DEG);
		skpTextFmt(skp, 31 * W / 32, 11 * H / 32, "%.1lf", GC->rtcc->PZLTRT.TerminalPhaseTravelAngle * DEG);
		skpTextFmt(skp, 31 * W / 32, 12 * H / 32, "%+.1lf", GC->rtcc->PZLTRT.MinSafeHeight / 1852.0);
		skpTextFmt(skp, 31 * W / 32, 13 * H / 32, "%.1lf", GC->rtcc->PZLTRT.ElevationAngle * DEG);

		GET_Display2(Buffer, GC->rtcc->PZLLTT.GETLOR);
		skp->Text(11 * W / 32, 17 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 21 * W / 32, 17 * H / 32, "%.1lf", GC->rtcc->PZLLTT.VH / 0.3048);

		GET_Display(Buffer, GC->rtcc->PZLLTT.GET_TPI, false);
		skp->Text(8 * W / 32, 21 * H / 32, Buffer, strlen(Buffer));
		GET_Display(Buffer, GC->rtcc->PZLLTT.GET_TPF, false);
		skp->Text(8 * W / 32, 23 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 12 * W / 32, 21 * H / 32, "%.1lf", GC->rtcc->PZLLTT.DV_TPI / 0.3048);
		skpTextFmt(skp, 12 * W / 32, 23 * H / 32, "%.1lf", GC->rtcc->PZLLTT.DV_TPF / 0.3048);
		skpTextFmt(skp, 17 * W / 32, 21 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.DV_TPI_LVLH.x / 0.3048);
		skpTextFmt(skp, 17 * W / 32, 23 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.DV_TPF_LVLH.x / 0.3048);
		skpTextFmt(skp, 22 * W / 32, 21 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.DV_TPI_LVLH.y / 0.3048);
		skpTextFmt(skp, 22 * W / 32, 23 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.DV_TPF_LVLH.y / 0.3048);
		skpTextFmt(skp, 27 * W / 32, 21 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.DV_TPI_LVLH.z / 0.3048);
		skpTextFmt(skp, 27 * W / 32, 23 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.DV_TPF_LVLH.z / 0.3048);

		skpTextFmt(skp, 31 * W / 32, 21 * H / 32, "%.1lf", GC->rtcc->PZLLTT.HA_TPI / 1852.0);
		skpTextFmt(skp, 31 * W / 32, 22 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.HP_TPI / 1852.0);
		skpTextFmt(skp, 31 * W / 32, 23 * H / 32, "%.1lf", GC->rtcc->PZLLTT.HA_TPF / 1852.0);
		skpTextFmt(skp, 31 * W / 32, 24 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.HP_TPF / 1852.0);

		skpTextFmt(skp, 31 * W / 32, 27 * H / 32, "%.1lf", GC->rtcc->PZLLTT.HA_T / 1852.0);
		skpTextFmt(skp, 31 * W / 32, 28 * H / 32, "%+.1lf", GC->rtcc->PZLLTT.HP_T / 1852.0);
	}
	else if (screen == 92)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "TPI TIMES", 9);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		const char* target;
		if (G->target != NULL)
		{
			target = G->target->GetName();
		}
		else
		{
			target = "No Target!";
		}
		skp->Text(1 * W / 8, 2 * H / 14, target, strlen(target));

		if (G->TPI_Mode == 0)
		{
			skp->Text(1 * W / 8, 4 * H / 14, "TPI on time", 11);
		}
		else if (G->TPI_Mode == 1)
		{
			skp->Text(1 * W / 8, 4 * H / 14, "TPI at orbital midnight", 23);
		}
		else
		{
			skp->Text(1 * W / 8, 4 * H / 14, "TPI at X min before sunrise:", 28);
		}

		if (G->TPI_Mode == 2)
		{
			skpTextFmt(skp, 1 * W / 8, 6 * H / 14, "%.1f min", G->dt_TPI_sunrise / 60.0);
		}

		GET_Display(Buffer, G->t_TPIguess, false);
		skp->Text(1 * W / 8, 8 * H / 14, Buffer, strlen(Buffer));

		GET_Display(Buffer, G->t_TPI, false);
		skp->Text(1 * W / 8, 10 * H / 14, Buffer, strlen(Buffer));

	}
	else if (screen == 93)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->SetFont(fonttest);
		skp->SetPen(pen2);
		skp->Text(4 * W / 8, 2 * H / 32, "VECTOR COMPARE DISPLAY", 39);
		skp->Text(30 * W / 32, 2 * H / 32, "1590", 4);

		Text(skp, 16 * W / 32, 31 * H / 32, GC->rtcc->VectorCompareDisplayBuffer.error);

		skp->Text(15 * W / 64, 7 * H / 32, "V1", 2);
		skp->Text(29 * W / 64, 7 * H / 32, "V2", 2);
		skp->Text(43 * W / 64, 7 * H / 32, "V3", 2);
		skp->Text(57 * W / 64, 7 * H / 32, "V4", 2);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->Text(1 * W / 32, 5 * H / 32, "VEH", 3);
		skp->Text(6 * W / 32, 5 * H / 32, "REF", 3);
		skp->Text(21 * W / 32, 4 * H / 32, "PET", 3);
		skp->Text(21 * W / 32, 5 * H / 32, "GMTR", 4);
		skp->TextW(1 * W / 32, 10 * H / 32, L"H\u2090", 2);
		skp->TextW(1 * W / 32, 11 * H / 32, L"H\u209A", 2);
		skp->Text(1 * W / 32, 12 * H / 32, "V", 1);
		skp->TextW(1 * W / 32, 13 * H / 32, L"\u03B3", 1);
		skp->TextW(1 * W / 32, 14 * H / 32, L"\u03C8", 1);
		skp->TextW(1 * W / 32, 15 * H / 32, L"\u03C6", 1);
		skp->TextW(1 * W / 32, 16 * H / 32, L"\u03BB", 1);
		skp->Text(1 * W / 32, 17 * H / 32, "h", 1);
		skp->Text(1 * W / 32, 18 * H / 32, "a", 1);
		skp->Text(1 * W / 32, 19 * H / 32, "e", 1);
		skp->Text(1 * W / 32, 20 * H / 32, "i", 1);
		skp->TextW(1 * W / 32, 21 * H / 32, L"\u0398\u209A", 2);
		skp->TextW(1 * W / 32, 22 * H / 32, L"\u03A9", 1);
		skp->TextW(1 * W / 32, 23 * H / 32, L"\u03BD", 1);
		skp->Text(1 * W / 32, 24 * H / 32, "U", 1);
		skp->Text(1 * W / 32, 25 * H / 32, "V", 1);
		skp->Text(1 * W / 32, 26 * H / 32, "W", 1);
		skp->TextW(1 * W / 32, 27 * H / 32, L"\u0307U", 2);
		skp->TextW(1 * W / 32, 28 * H / 32, L"\u0307V", 2);
		skp->TextW(1 * W / 32, 29 * H / 32, L"\u0307W", 2);

		skp->Line(11 * W / 32, 8 * H / 32, 11 * W / 32, 31 * H / 32);
		skp->Line(18 * W / 32, 8 * H / 32, 18 * W / 32, 31 * H / 32);
		skp->Line(25 * W / 32, 8 * H / 32, 25 * W / 32, 31 * H / 32);

		skp->Line(0, 18 * H / 32, W, 18 * H / 32);
		skp->Line(0, 24 * H / 32, W, 24 * H / 32);

		if (GC->rtcc->med_s80.time > 0.0)
		{
			skp->Text(1 * W / 32, 4 * H / 32, "GMT", 3);
			GET_Display2(Buffer, GC->rtcc->med_s80.time);
			skp->Text(4 * W / 32, 4 * H / 32, Buffer, strlen(Buffer));
		}
		else if (GC->rtcc->med_s80.time < 0.0)
		{
			skp->Text(1 * W / 32, 4 * H / 32, "GET", 3);
			GET_Display2(Buffer, -GC->rtcc->med_s80.time);
			skp->Text(4 * W / 32, 4 * H / 32, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(1 * W / 32, 4 * H / 32, "Time From V1", 12);
		}
		
		if (GC->rtcc->med_s80.VEH == 1)
		{
			skp->Text(3 * W / 32, 5 * H / 32, "CSM", 3);
		}
		else
		{
			skp->Text(3 * W / 32, 5 * H / 32, "LEM", 3);
		}

		if (GC->rtcc->med_s80.REF == BODY_EARTH)
		{
			skp->Text(8 * W / 32, 5 * H / 32, "E", 1);
		}
		else
		{
			skp->Text(8 * W / 32, 5 * H / 32, "M", 1);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		GET_Display2(Buffer, GC->rtcc->VectorCompareDisplayBuffer.PET);
		skp->Text(31 * W / 32, 4 * H / 32, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->VectorCompareDisplayBuffer.GMTR);
		skp->Text(31 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));

		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		for (int i = 0;i < 4;i++)
		{
			Text(skp, (15 + 14 * i) * W / 64, 9 * H / 32, GC->rtcc->med_s80.VID[i]);
		}
		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		for (int i = 0;i < GC->rtcc->VectorCompareDisplayBuffer.NumVec;i++)
		{
			GET_Display2(Buffer, GC->rtcc->VectorCompareDisplayBuffer.data[i].GMT);
			skp->Text((21 + 14 * i) * W / 64, 8 * H / 32, Buffer, strlen(Buffer));
			
			if (GC->rtcc->VectorCompareDisplayBuffer.showHA[i])
			{
				if (i == 0)
				{
					sprintf_s(Buffer, "%.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].HA);
				}
				else
				{
					sprintf_s(Buffer, "%+.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].HA);
				}
				skp->Text((21 + 14 * i) * W / 64, 10 * H / 32, Buffer, strlen(Buffer));
			}
			skpTextFmt(skp, (21 + 14 * i) * W / 64, 11 * H / 32, "%+.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].HP);
			if (i == 0)
			{
				sprintf_s(Buffer, "%.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].v);
			}
			else
			{
				sprintf_s(Buffer, "%+.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].v);
			}
			skp->Text((21 + 14 * i) * W / 64, 12 * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, (21 + 14 * i) * W / 64, 13 * H / 32, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].gamma);
			if (i == 0)
			{
				sprintf_s(Buffer, "%.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].psi);
			}
			else
			{
				sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].psi);
			}
			skp->Text((21 + 14 * i) * W / 64, 14 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				if (GC->rtcc->VectorCompareDisplayBuffer.data[i].phi > 0)
				{
					sprintf_s(Buffer, "%.5lfN", GC->rtcc->VectorCompareDisplayBuffer.data[i].phi);
				}
				else
				{
					sprintf_s(Buffer, "%.5lfS", abs(GC->rtcc->VectorCompareDisplayBuffer.data[i].phi));
				}
			}
			else
			{
				sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].phi);
			}
			skp->Text((21 + 14 * i) * W / 64, 15 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				if (GC->rtcc->VectorCompareDisplayBuffer.data[i].lambda > 0)
				{
					sprintf_s(Buffer, "%.5lfE", GC->rtcc->VectorCompareDisplayBuffer.data[i].lambda);
				}
				else
				{
					sprintf_s(Buffer, "%.5lfW", abs(GC->rtcc->VectorCompareDisplayBuffer.data[i].lambda));
				}
			}
			else
			{
				sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].lambda);
			}
			skp->Text((21 + 14 * i) * W / 64, 16 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].h);
			}
			else
			{
				sprintf_s(Buffer, "%+.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].h);
			}
			skp->Text((21 + 14 * i) * W / 64, 17 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].a);
			}
			else
			{
				sprintf_s(Buffer, "%+.3lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].a);
			}
			skp->Text((21 + 14 * i) * W / 64, 18 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].e);
			}
			else
			{
				sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].e);
			}
			skp->Text((21 + 14 * i) * W / 64, 19 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].i);
			}
			else
			{
				sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].i);
			}
			skp->Text((21 + 14 * i) * W / 64, 20 * H / 32, Buffer, strlen(Buffer));
			if (GC->rtcc->VectorCompareDisplayBuffer.showWPAndTA[i])
			{
				if (i == 0)
				{
					sprintf_s(Buffer, "%.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].theta_p);
				}
				else
				{
					sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].theta_p);
				}
				skp->Text((21 + 14 * i) * W / 64, 21 * H / 32, Buffer, strlen(Buffer));
				if (i == 0)
				{
					sprintf_s(Buffer, "%.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].nu);
				}
				else
				{
					sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].nu);
				}
				skp->Text((21 + 14 * i) * W / 64, 23 * H / 32, Buffer, strlen(Buffer));
			}
			if (i == 0)
			{
				sprintf_s(Buffer, "%.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].Omega);
			}
			else
			{
				sprintf_s(Buffer, "%+.5lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].Omega);
			}
			skp->Text((21 + 14 * i) * W / 64, 22 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].U);
			}
			else
			{
				sprintf_s(Buffer, "%+.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].U);
			}
			skp->Text((21 + 14 * i) * W / 64, 24 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].V);
			}
			else
			{
				sprintf_s(Buffer, "%+.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].V);
			}
			skp->Text((21 + 14 * i) * W / 64, 25 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].W);
			}
			else
			{
				sprintf_s(Buffer, "%+.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].W);
			}
			skp->Text((21 + 14 * i) * W / 64, 26 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].U_dot);
			}
			else
			{
				sprintf_s(Buffer, "%+.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].U_dot);
			}
			skp->Text((21 + 14 * i) * W / 64, 27 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].V_dot);
			}
			else
			{
				sprintf_s(Buffer, "%+.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].V_dot);
			}
			skp->Text((21 + 14 * i) * W / 64, 28 * H / 32, Buffer, strlen(Buffer));
			if (i == 0)
			{
				sprintf_s(Buffer, "%.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].W_dot);
			}
			else
			{
				sprintf_s(Buffer, "%+.2lf", GC->rtcc->VectorCompareDisplayBuffer.data[i].W_dot);
			}
			skp->Text((21 + 14 * i) * W / 64, 29 * H / 32, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 95)
	{
		skp->SetFont(font3);
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 3 * H / 26, "GUIDANCE OPTICS SUPPORT TABLE", 29);
		Text(skp, 1 * W / 2, 25 * H / 26, GC->rtcc->GOSTDisplayBuffer.err);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);
		skp->Text(38 * W / 43, 3 * H / 26, "0229", 4);

		skp->Text(3 * W / 43, 5 * H / 26, "CODE", 4);
		skp->TextW(17 * W / 43, 5 * H / 26, L"\u03B8\u2095", 2);
		skp->TextW(25 * W / 43, 5 * H / 26, L"\u03C8\u2095", 2);
		skp->TextW(33 * W / 43, 5 * H / 26, L"\u03C6\u2095", 2);

		skp->Text(2 * W / 43, 6 * H / 26, "GETAC", 5);
		skp->Text(20 * W / 43, 6 * H / 26, "IGA", 3);
		skp->Text(31 * W / 43, 6 * H / 26, "IRA", 3);

		skp->Text(2 * W / 43, 7 * H / 26, "SXT", 3);
		skp->Text(13 * W / 43, 7 * H / 26, "SFT", 3);
		skp->Text(20 * W / 43, 7 * H / 26, "TRN", 3);
		skp->Text(30 * W / 43, 7 * H / 26, "SFT", 3);
		skp->Text(38 * W / 43, 7 * H / 26, "TRN", 3);

		skp->Text(2 * W / 43, 8 * H / 26, "S1", 2);
		skp->Text(9 * W / 43, 8 * H / 26, "INP", 3);
		skp->Text(25 * W / 43, 8 * H / 26, "RTCC", 4);

		skp->Text(2 * W / 43, 9 * H / 26, "S2", 2);
		skp->Text(9 * W / 43, 9 * H / 26, "INP", 3);
		skp->Text(25 * W / 43, 9 * H / 26, "RTCC", 4);

		skp->Text(2 * W / 43, 10 * H / 26, "SCT", 3);

		skp->Text(1 * W / 43, 11 * H / 26, "S", 1);
		skp->Text(15 * W / 43, 11 * H / 26, "M", 1);
		skp->Text(21 * W / 43, 11 * H / 26, "R", 1);
		skp->Text(26 * W / 43, 11 * H / 26, "RT ASC", 6);
		skp->Text(37 * W / 43, 11 * H / 26, "DEC", 3);

		skp->Text(1 * W / 43, 12 * H / 26, "1", 1);
		skp->Text(6 * W / 43, 12 * H / 26, "SF", 2);

		skp->Text(1 * W / 43, 13 * H / 26, "2", 1);
		skp->Text(6 * W / 43, 13 * H / 26, "TR", 2);

		skp->Text(1 * W / 43, 15 * H / 26, "BORESIGHT", 9);
		skp->Text(11 * W / 43, 15 * H / 26, "SPA", 3);
		skp->Text(18 * W / 43, 15 * H / 26, "SPX", 3);
		skp->Text(24 * W / 43, 15 * H / 26, "RT ASC", 6);
		skp->Text(36 * W / 43, 15 * H / 26, "DEC", 3);

		skp->Text(2 * W / 43, 16 * H / 26, "S1", 2);
		skp->Text(2 * W / 43, 17 * H / 26, "S2", 2);

		skp->Text(1 * W / 43, 18 * H / 26, "LOS", 3);
		skp->Text(6 * W / 43, 18 * H / 26, "IX", 2);
		skp->Text(18 * W / 43, 18 * H / 26, "IY", 2);
		skp->Text(30 * W / 43, 18 * H / 26, "IZ", 2);

		skp->Text(1 * W / 43, 19 * H / 26, "GET", 3);
		skp->Text(14 * W / 43, 19 * H / 26, "SC", 2);
		skp->Text(20 * W / 43, 19 * H / 26, "RA", 2);
		skp->Text(31 * W / 43, 19 * H / 26, "DEC", 3);

		skp->Text(18 * W / 43, 20 * H / 26, "P", 1);
		skp->Text(27 * W / 43, 20 * H / 26, "Y", 1);
		skp->Text(36 * W / 43, 20 * H / 26, "R", 1);
		skp->Text(2 * W / 43, 21 * H / 26, "MAT", 3);
		skp->Text(18 * W / 43, 21 * H / 26, "P", 1);
		skp->Text(27 * W / 43, 21 * H / 26, "Y", 1);
		skp->Text(36 * W / 43, 21 * H / 26, "R", 1);

		skp->Text(1 * W / 43, 22 * H / 26, "X/XE", 4);
		skp->Text(1 * W / 43, 23 * H / 26, "Y/XE", 4);
		skp->Text(1 * W / 43, 24 * H / 26, "Z/XE", 4);

		skp->Text(15 * W / 43, 22 * H / 26, "X/YE", 4);
		skp->Text(15 * W / 43, 23 * H / 26, "Y/YE", 4);
		skp->Text(15 * W / 43, 24 * H / 26, "Z/YE", 4);
		
		skp->Text(29 * W / 43, 22 * H / 26, "X/ZE", 4);
		skp->Text(29 * W / 43, 23 * H / 26, "Y/ZE", 4);
		skp->Text(29 * W / 43, 24 * H / 26, "Z/ZE", 4);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		Text(skp, 15 * W / 43, 5 * H / 26, GC->rtcc->GOSTDisplayBuffer.data.CODE);
		skpTextFmt(skp, 23 * W / 43, 5 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att_H.x);
		skpTextFmt(skp, 31 * W / 43, 5 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att_H.y);
		skpTextFmt(skp, 40 * W / 43, 5 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att_H.z);

		GET_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.GETAC, false);
		skp->Text(16 * W / 43, 6 * H / 26, Buffer, strlen(Buffer));
		skpTextFmt(skp, 28 * W / 43, 6 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.IGA);
		Text(skp, 40 * W / 43, 6 * H / 26, GC->rtcc->GOSTDisplayBuffer.data.IRA);

		skpTextFmt(skp, 7 * W / 43, 8 * H / 26, "%03o", GC->rtcc->GOSTDisplayBuffer.data.SXT_STAR[0]);
		skpTextFmt(skp, 16 * W / 43, 8 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_SFT_INP[0]);
		skpTextFmt(skp, 23 * W / 43, 8 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_TRN_INP[0]);
		skpTextFmt(skp, 33 * W / 43, 8 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_SFT_RTCC[0]);
		skpTextFmt(skp, 40 * W / 43, 8 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_TRN_RTCC[0]);

		skpTextFmt(skp, 7 * W / 43, 9 * H / 26, "%03o", GC->rtcc->GOSTDisplayBuffer.data.SXT_STAR[1]);
		skpTextFmt(skp, 16 * W / 43, 9 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_SFT_INP[1]);
		skpTextFmt(skp, 23 * W / 43, 9 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_TRN_INP[1]);
		skpTextFmt(skp, 33 * W / 43, 9 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_SFT_RTCC[1]);
		skpTextFmt(skp, 40 * W / 43, 9 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SXT_TRN_RTCC[1]);

		skpTextFmt(skp, 5 * W / 43, 12 * H / 26, "%03o", GC->rtcc->GOSTDisplayBuffer.data.SCT_S[0]);
		skpTextFmt(skp, 12 * W / 43, 12 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SCT_SF);
		skpTextFmt(skp, 17 * W / 43, 12 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SCT_M[0]);
		skpTextFmt(skp, 23 * W / 43, 12 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SCT_R[0]);
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.SCT_RTASC[0], false);
		skp->Text(32 * W / 43, 12 * H / 26, Buffer, strlen(Buffer));
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.SCT_DEC[0]);
		skp->Text(42 * W / 43, 12 * H / 26, Buffer, strlen(Buffer));

		skpTextFmt(skp, 5 * W / 43, 13 * H / 26, "%03o", GC->rtcc->GOSTDisplayBuffer.data.SCT_S[1]);
		skpTextFmt(skp, 12 * W / 43, 13 * H / 26, "%04.1lf", GC->rtcc->GOSTDisplayBuffer.data.SCT_TR);
		skpTextFmt(skp, 17 * W / 43, 13 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SCT_M[1]);
		skpTextFmt(skp, 23 * W / 43, 13 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.SCT_R[1]);
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.SCT_RTASC[1], false);
		skp->Text(32 * W / 43, 13 * H / 26, Buffer, strlen(Buffer));
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.SCT_DEC[1]);
		skp->Text(42 * W / 43, 13 * H / 26, Buffer, strlen(Buffer));

		skpTextFmt(skp, 7 * W / 43, 16 * H / 26, "%03o", GC->rtcc->GOSTDisplayBuffer.data.BS_S[0]);
		skpTextFmt(skp, 14 * W / 43, 16 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.BS_SPA[0]);
		skpTextFmt(skp, 21 * W / 43, 16 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.BS_SXP[0]);
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.BS_RTASC[0], false);
		skp->Text(31 * W / 43, 16 * H / 26, Buffer, strlen(Buffer));
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.BS_DEC[0]);
		skp->Text(41 * W / 43, 16 * H / 26, Buffer, strlen(Buffer));

		skpTextFmt(skp, 7 * W / 43, 17 * H / 26, "%03o", GC->rtcc->GOSTDisplayBuffer.data.BS_S[1]);
		skpTextFmt(skp, 14 * W / 43, 17 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.BS_SPA[1]);
		skpTextFmt(skp, 21 * W / 43, 17 * H / 26, "%+05.1lf", GC->rtcc->GOSTDisplayBuffer.data.BS_SXP[1]);
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.BS_RTASC[1], false);
		skp->Text(31 * W / 43, 17 * H / 26, Buffer, strlen(Buffer));
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.BS_DEC[1]);
		skp->Text(41 * W / 43, 17 * H / 26, Buffer, strlen(Buffer));

		skpTextFmt(skp, 15 * W / 43, 18 * H / 26, "%+.5lf", GC->rtcc->GOSTDisplayBuffer.data.Landmark_LOS.x);
		skpTextFmt(skp, 27 * W / 43, 18 * H / 26, "%+.5lf", GC->rtcc->GOSTDisplayBuffer.data.Landmark_LOS.y);
		skpTextFmt(skp, 39 * W / 43, 18 * H / 26, "%+.5lf", GC->rtcc->GOSTDisplayBuffer.data.Landmark_LOS.z);

		GET_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.Landmark_GET, false);
		skp->Text(12 * W / 43, 19 * H / 26, Buffer, strlen(Buffer));
		Text(skp, 19 * W / 43, 19 * H / 26, GC->rtcc->GOSTDisplayBuffer.data.Landmark_SC);
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.Landmark_RA, false);
		skp->Text(29 * W / 43, 19 * H / 26, Buffer, strlen(Buffer));
		Angle_Display(Buffer, GC->rtcc->GOSTDisplayBuffer.data.Landmark_DEC);
		skp->Text(42 * W / 43, 19 * H / 26, Buffer, strlen(Buffer));

		skpTextFmt(skp, 24 * W / 43, 20 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att[0].x);
		skpTextFmt(skp, 33 * W / 43, 20 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att[0].y);
		skpTextFmt(skp, 42 * W / 43, 20 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att[0].z);
		skpTextFmt(skp, 24 * W / 43, 21 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att[1].x);
		skpTextFmt(skp, 33 * W / 43, 21 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att[1].y);
		skpTextFmt(skp, 42 * W / 43, 21 * H / 26, "%05.1lf", GC->rtcc->GOSTDisplayBuffer.data.Att[1].z);

		Text(skp, 12 * W / 43, 21 * H / 26, GC->rtcc->GOSTDisplayBuffer.data.MAT);

		skpTextFmt(skp, 13 * W / 43, 22 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m11);
		skpTextFmt(skp, 13 * W / 43, 23 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m21);
		skpTextFmt(skp, 13 * W / 43, 24 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m31);
		skpTextFmt(skp, 27 * W / 43, 22 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m12);
		skpTextFmt(skp, 27 * W / 43, 23 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m22);
		skpTextFmt(skp, 27 * W / 43, 24 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m32);
		skpTextFmt(skp, 42 * W / 43, 22 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m13);
		skpTextFmt(skp, 42 * W / 43, 23 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m23);
		skpTextFmt(skp, 42 * W / 43, 24 * H / 26, "%+.7lf", GC->rtcc->GOSTDisplayBuffer.data.REFSMMAT.m33);
	}
	else if (screen == 96)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(1 * W / 2, 3 * H / 32, "SLV NAVIGATION UPDATE (MSK 235)", 31);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(3 * W / 32, 5 * H / 32, "LOAD NO", 7);
		skp->Text(11 * W / 32, 5 * H / 32, "GETSV", 5);
		skp->Text(9 * W / 32, 7 * H / 32, "GRR/S", 5);
		skp->Text(23 * W / 32, 7 * H / 32, "AZI", 3);

		skp->Text(4 * W / 32, 10 * H / 32, "FCT", 3);
		skp->Text(14 * W / 32, 10 * H / 32, "ENGLISH", 31);
		skp->Text(24 * W / 32, 10 * H / 32, "METRIC", 31);

		skp->Text(4 * W / 32, 13 * H / 32, "Z DOT", 5);
		skp->Text(4 * W / 32, 15 * H / 32, "X DOT", 5);
		skp->Text(4 * W / 32, 17 * H / 32, "Y DOT", 5);
		skp->Text(4 * W / 32, 19 * H / 32, "Z POS", 5);
		skp->Text(4 * W / 32, 21 * H / 32, "X POS", 5);
		skp->Text(4 * W / 32, 23 * H / 32, "Y POS", 5);
		skp->Text(4 * W / 32, 25 * H / 32, "TIME", 4);

		if (G->svtarget != NULL)
		{
			sprintf(Buffer, G->svtarget->GetName());
			skp->Text(6 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(6 * W / 8, 2 * H / 14, "No Target!", 10);
		}

		if (G->SVDesiredGET < 0)
		{
			sprintf(Buffer, "Present GET");
		}
		else
		{
			GET_Display2(Buffer, G->SVDesiredGET);
		}
		skp->Text(16 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->GetIUClockZero());
		skp->Text(14 * W / 32, 7 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 26 * W / 32, 7 * H / 32, "%+.3lf°", GC->rtcc->GetIULaunchAzimuth()* DEG);

		skpTextFmt(skp, 14 * W / 32, 13 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.DotS.z / 0.3048);
		skpTextFmt(skp, 24 * W / 32, 13 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.DotS.z);
		skpTextFmt(skp, 14 * W / 32, 15 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.DotS.x / 0.3048);
		skpTextFmt(skp, 24 * W / 32, 15 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.DotS.x);
		skpTextFmt(skp, 14 * W / 32, 17 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.DotS.y / 0.3048);
		skpTextFmt(skp, 24 * W / 32, 17 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.DotS.y);
		skpTextFmt(skp, 14 * W / 32, 19 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.PosS.z / 1852.0);
		skpTextFmt(skp, 24 * W / 32, 19 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.PosS.z / 1000.0);
		skpTextFmt(skp, 14 * W / 32, 21 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.PosS.x / 1852.0);
		skpTextFmt(skp, 24 * W / 32, 21 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.PosS.x / 1000.0);
		skpTextFmt(skp, 14 * W / 32, 23 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.PosS.y / 1852.0);
		skpTextFmt(skp, 24 * W / 32, 23 * H / 32, "%+.1lf", GC->rtcc->CZNAVSLV.PosS.y / 1000.0);

		GET_Display2(Buffer, GC->rtcc->CZNAVSLV.NUPTIM);
		skp->Text(14 * W / 32, 25 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 24 * W / 32, 25 * H / 32, "%+.4lf", GC->rtcc->CZNAVSLV.NUPTIM / 3600.0);

		switch (G->iuUplinkResult)
		{
		case 1:
			sprintf(Buffer, "Uplink accepted!");
			break;
		case 2:
			sprintf(Buffer, "No vessel or IU!");
			break;
		case 3:
			sprintf(Buffer, "Uplink rejected!");
			break;
		case 4:
			sprintf(Buffer, "No state vector!");
			break;
		default:
			sprintf(Buffer, "No Uplink");
			break;
		}
		skp->Text(13 * W / 32, 30 * H / 32, Buffer, strlen(Buffer));

	}
	else if (screen == 97)
	{
		G->CycleVectorPanelSummary();

		skp->SetFont(font4);
		skp->SetPen(pen2);
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(5 * W / 8, 1 * H / 32, "VECTOR PANEL SUMMARY", 20);
		skp->Text(38 * W / 43, 1 * H / 32, "1591", 4);
		
		skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);

		skp->Text(2 * W / 43, 3 * H / 32, "CSM ANCHOR", 10);
		Text(skp, 10 * W / 43, 3 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.AnchorVectorID[0]);
		skp->Text(17 * W / 43, 3 * H / 32, "CURRENT GMT", 11);
		skp->Text(29 * W / 43, 3 * H / 32, "LM ANCHOR", 9);
		Text(skp, 37 * W / 43, 3 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.AnchorVectorID[1]);

		skp->Text(3 * W / 43, 4 * H / 32, "GMTAV", 10);
		Text(skp, 8 * W / 43, 4 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.AnchorVectorGMT[0]);
		Text(skp, 19 * W / 43, 4 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CurrentGMT);
		skp->Text(30 * W / 43, 4 * H / 32, "GMTAV", 10);
		Text(skp, 35 * W / 43, 4 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.AnchorVectorGMT[1]);

		skp->Line(0, 5 * H / 32, W, 5 * H / 32);
		skp->Line(0, 7 * H / 32, W, 7 * H / 32);
		skp->Line(W / 2, 5 * H / 32, W / 2, H);
		skp->Line(0, 15 * H / 32, W, 15 * H / 32);
		skp->Line(W / 4, 7 * H / 32, W / 4, H);
		skp->Line(3 * W / 4, 7 * H / 32, 3 * W / 4, H);
		skp->Line(W / 4, 20 * H / 32, W / 2, 20 * H / 32);
		skp->Line(3 * W / 4, 20 * H / 32, W, 20 * H / 32);
		skp->Line(0, 23 * H / 32, W / 4, 23 * H / 32);
		skp->Line(W / 2, 23 * H / 32, 3 * W / 4, 23 * H / 32);
		skp->Line(W / 4, 25 * H / 32, W / 2, 25 * H / 32);
		skp->Line(3 * W / 4, 25 * H / 32, W, 25 * H / 32);

		skp->Text(5 * W / 43, 13 * H / 64, "CSM VECTORS", 11);
		skp->Text(26 * W / 43, 13 * H / 64, "LM VECTORS", 10);

		for (int i = 0;i < 2;i++)
		{
			skp->Text((22 * i + 1) * W / 44, 8 * H / 32, "CMC", 3);
			skp->Text((22 * i + 1) * W / 44, 16 * H / 32, "LGC", 3);
			skp->Text((22 * i + 1) * W / 44, 24 * H / 32, "AGS", 3);

			skp->Text((22 * i + 12) * W / 44, 8 * H / 32, "IU", 2);
			skp->Text((22 * i + 11) * W / 44, 16 * H / 32, "HIGHSPEED RADAR", 15);
			skp->Text((22 * i + 12) * W / 44, 21 * H / 32, "DC VECTOR", 9);
			skp->Text((22 * i + 12) * W / 44, 26 * H / 32, "LAST EXECUTED", 13);
			skp->Text((22 * i + 13) * W / 44, 27 * H / 32, "MANEUVER", 8);

			//CMC, LGC and AGS
			for (int j = 0;j < 3;j++)
			{
				skp->Text((22 * i + 2) * W / 44, (j * 8 + 9) * H / 32, "UV", 2);
				Text(skp, (22 * i + 4) * W / 44, (j * 8 + 9) * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompUsableID[i][j]);
				Text(skp, (22 * i + 4) * W / 44, (j * 8 + 10) * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompUsableGMT[i][j]);
				skp->Text((22 * i + 2) * W / 44, (j * 8 + 11) * H / 32, "EV", 2);
				Text(skp, (22 * i + 4) * W / 44, (j * 8 + 11) * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompEvalID[i][j]);
				Text(skp, (22 * i + 4) * W / 44, (j * 8 + 12) * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompEvalGMT[i][j]);
				skp->Text((22 * i + 2) * W / 44, (j * 8 + 13) * H / 32, "TH", 3);
				Text(skp, (22 * i + 4) * W / 44, (j * 8 + 13) * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompTelemetryHighGMT[i][j]);
				skp->Text((22 * i + 2) * W / 44, (j * 8 + 14) * H / 32, "TL", 3);
				Text(skp, (22 * i + 4) * W / 44, (j * 8 + 14) * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompTelemetryLowGMT[i][j]);
			}

			//IU
			skp->Text((22 * i + 13) * W / 44, 9 * H / 32, "UV", 2);
			Text(skp, (22 * i + 15) * W / 44, 9 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompUsableID[i][3]);
			Text(skp, (22 * i + 15) * W / 44, 10 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompUsableGMT[i][3]);
			skp->Text((22 * i + 13) * W / 44, 11 * H / 32, "EV", 2);
			Text(skp, (22 * i + 15) * W / 44, 11 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompEvalID[i][3]);
			Text(skp, (22 * i + 15) * W / 44, 12 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompEvalGMT[i][3]);
			skp->Text((22 * i + 13) * W / 44, 13 * H / 32, "TH", 3);
			Text(skp, (22 * i + 15) * W / 44, 13 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompTelemetryHighGMT[i][3]);
			skp->Text((22 * i + 13) * W / 44, 14 * H / 32, "TL", 3);
			Text(skp, (22 * i + 15) * W / 44, 14 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.CompTelemetryLowGMT[i][3]);

			//HSR
			skp->Text((22 * i + 13) * W / 44, 18 * H / 32, "UV", 2);
			Text(skp, (22 * i + 15) * W / 44, 18 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.HSRID[i]);
			Text(skp, (22 * i + 15) * W / 44, 19 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.HSRGMT[i]);

			//DC
			skp->Text((22 * i + 13) * W / 44, 23 * H / 32, "UV", 2);
			Text(skp, (22 * i + 15) * W / 44, 23 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.DCID[i]);
			Text(skp, (22 * i + 15) * W / 44, 24 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.DCGMT[i]);

			//Last Executed Maneuver
			skp->Text((22 * i + 12) * W / 44, 28 * H / 32, "GMTUL", 5);
			Text(skp, (22 * i + 17) * W / 44, 28 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.LastManGMTUL[i]);
			skp->Text((22 * i + 12) * W / 44, 29 * H / 32, "GMTBO", 5);
			Text(skp, (22 * i + 17) * W / 44, 29 * H / 32, GC->rtcc->VectorPanelSummaryBuffer.LastManGMTBO[i]);
		}
	}
	else if (screen == 103)
	{
		skp->Text(5 * W / 8, 1 * H / 32, "Retrofire Constraints", 21);

		ThrusterName(Buffer, GC->rtcc->RZJCTTC.R31_Thruster);
		skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->RZJCTTC.R31_BurnMode == 1)
		{
			skp->Text(1 * W / 16, 4 * H / 14, "DV", 2);
			skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "DV = %.1lf", GC->rtcc->RZJCTTC.R31_dv / 0.3048);
		}
		else if (GC->rtcc->RZJCTTC.R31_BurnMode == 2)
		{
			skp->Text(1 * W / 16, 4 * H / 14, "DT", 2);
			skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "DT = %.1lf", GC->rtcc->RZJCTTC.R31_dt);
		}
		else
		{
			skp->Text(1 * W / 16, 4 * H / 14, "V, Gamma", 8);
		}

		if (GC->rtcc->RZJCTTC.R31_AttitudeMode == 1)
		{
			skp->Text(1 * W / 16, 8 * H / 14, "LVLH", 4);
			skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%+.1lf %+.1lf %+.1lf", GC->rtcc->RZJCTTC.R31_LVLHAttitude.x* DEG, GC->rtcc->RZJCTTC.R31_LVLHAttitude.y* DEG, GC->rtcc->RZJCTTC.R31_LVLHAttitude.z* DEG);
		}
		else
		{
			skp->Text(1 * W / 16, 8 * H / 14, "31.7° window line", 17);
		}


		if (GC->rtcc->RZJCTTC.R31_Thruster == RTCC_ENGINETYPE_CSMSPS)
		{
			if (GC->rtcc->RZJCTTC.R31_Use4UllageThrusters)
			{
				sprintf_s(Buffer, "4 quads, %.1lf s ullage", GC->rtcc->RZJCTTC.R31_UllageTime);
			}
			else
			{
				sprintf_s(Buffer, "2 quads, %.1lf s ullage", GC->rtcc->RZJCTTC.R31_UllageTime);
			}
			skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));
		}

		const char *name = GC->rtcc->EMGSTGENName(GC->rtcc->RZJCTTC.R31_REFSMMAT);
		skp->Text(9 * W / 16, 2 * H / 14, name, strlen(name));

		if (GC->rtcc->RZJCTTC.R31_GimbalIndicator == 1)
		{
			skp->Text(9 * W / 16, 4 * H / 14, "Use System Parameters", 21);
		}
		else
		{
			skp->Text(9 * W / 16, 4 * H / 14, "Compute Gimbal Trims", 20);
		}

		skpTextFmt(skp, 9 * W / 16, 6 * H / 14, "%+.1lf°", GC->rtcc->RZJCTTC.R31_InitialBankAngle* DEG);
		skpTextFmt(skp, 9 * W / 16, 8 * H / 14, "%.1lf gs", GC->rtcc->RZJCTTC.R31_GLevel);
		skpTextFmt(skp, 9 * W / 16, 10 * H / 14, "%+.1lf°", GC->rtcc->RZJCTTC.R31_FinalBankAngle* DEG);
	}
	else if (screen == 104)
	{
		int hh, mm;
		double secs;

		skp->Text(2 * W / 8, 2 * H / 26, "Retrofire Digitals (MSK 356)", 28);

		skp->SetFont(font3);
		skp->SetPen(pen2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(2 * W / 44, 5 * H / 26, "AREA", 4);
		skp->Text(2 * W / 44, 6 * H / 26, "MATRIX", 6);
		skp->Text(2 * W / 44, 7 * H / 26, "WT TAA", 6);
		skp->Text(2 * W / 44, 8 * H / 26, "RLH PLH YLH", 11);
		skp->Text(2 * W / 44, 9 * H / 26, "RO PI YM", 8);
		skp->Text(2 * W / 44, 10 * H / 26, "VC BT", 5);
		skp->Text(2 * W / 44, 11 * H / 26, "VT U DT", 7);
		skp->Text(2 * W / 44, 12 * H / 26, "H", 1);
		skp->Text(2 * W / 44, 13 * H / 26, "GETI", 4);
		skp->Text(2 * W / 44, 14 * H / 26, "GMTI", 4);
		skp->Text(2 * W / 44, 15 * H / 26, "RET", 3);
		skp->Text(2 * W / 44, 16 * H / 26, "V EI", 4);
		skp->Text(6 * W / 44, 16 * H / 26, "GAM EI", 6);
		skp->Text(2 * W / 44, 17 * H / 26, "BA", 2);
		skp->Text(2 * W / 44, 18 * H / 26, "RETRB", 5);
		skp->Text(2 * W / 44, 19 * H / 26, "LAT ML", 6);
		skp->Text(8 * W / 44, 19 * H / 26, "LNG ML", 6);
		skp->Text(2 * W / 44, 20 * H / 26, "LAT T", 5);
		skp->Text(8 * W / 44, 20 * H / 26, "LNG T", 5);
		skp->Text(2 * W / 44, 21 * H / 26, "LAT IP", 6);
		skp->Text(8 * W / 44, 21 * H / 26, "LNG IP", 6);
		skp->Text(2 * W / 44, 22 * H / 26, "LAT ZL", 6);
		skp->Text(8 * W / 44, 22 * H / 26, "LNG ZL", 6);
		skp->Text(2 * W / 44, 23 * H / 26, "DLAT", 4);
		skp->Text(8 * W / 44, 23 * H / 26, "DLNG", 4);
		
		skp->Line(14 * W / 44, 6 * H / 26, 14 * W / 44, 24 * H / 26);

		RetrofireDisplayParametersTableData *tab = &GC->rtcc->RZRFDP.data[2];

		if (tab->Indicator == 0)
		{
			Text(skp, 16 * W / 44, 6 * H / 26, tab->RefsID);
			skpTextFmt(skp, 16 * W / 44, 7 * H / 26, "%.1lf", tab->CSMWeightRetro);
			skpTextFmt(skp, 22 * W / 44, 7 * H / 26, "%.0lf", tab->TrueAnomalyRetro);
			skpTextFmt(skp, 16 * W / 44, 8 * H / 26, "%.1lf %.1lf %.1lf", tab->Att_LVLH.x, tab->Att_LVLH.y, tab->Att_LVLH.z);
			skpTextFmt(skp, 16 * W / 44, 9 * H / 26, "%.1lf %.1lf %.1lf", tab->Att_IMU.x, tab->Att_IMU.y, tab->Att_IMU.z);
			skpTextFmt(skp, 16 * W / 44, 10 * H / 26, "%.1lf", tab->DVC);
			SStoHHMMSS(tab->BurnTime, hh, mm, secs);
			skpTextFmt(skp, 22 * W / 44, 10 * H / 26, "%02d:%04.1lf", mm, secs);
			skpTextFmt(skp, 16 * W / 44, 11 * H / 26, "%.1lf", tab->DVT);
			skpTextFmt(skp, 21 * W / 44, 11 * H / 26, "%+d", tab->UllageQuads);
			skpTextFmt(skp, 24 * W / 44, 11 * H / 26, "%.1lf", tab->UllageDT);
			skpTextFmt(skp, 16 * W / 44, 12 * H / 26, "%.1lf", tab->H_Retro);
			GET_Display2(Buffer, tab->GETI);
			skp->Text(16 * W / 44, 13 * H / 26, Buffer, strlen(Buffer));
			GET_Display(Buffer, tab->GMTI, false);
			skp->Text(16 * W / 44, 14 * H / 26, Buffer, strlen(Buffer));
			SStoHHMMSS(tab->RET400k, hh, mm, secs);
			skpTextFmt(skp, 16 * W / 44, 15 * H / 26, "%d:%02.0lf", mm, secs);
			skpTextFmt(skp, 16 * W / 44, 16 * H / 26, "%.0lf", tab->V400k);
			skpTextFmt(skp, 22 * W / 44, 16 * H / 26, "%.2lf", tab->Gamma400k);
			skpTextFmt(skp, 16 * W / 44, 17 * H / 26, "%.1lf°", tab->BankAngle);
			SStoHHMMSS(tab->RETRB, hh, mm, secs);
			skpTextFmt(skp, 16 * W / 44, 18 * H / 26, "%d:%02.0lf", mm, secs);
			FormatLatitude(Buffer, tab->lat_ML);
			skp->Text(16 * W / 44, 19 * H / 26, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_ML);
			skp->Text(24 * W / 44, 19 * H / 26, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_T);
			skp->Text(16 * W / 44, 20 * H / 26, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_T);
			skp->Text(24 * W / 44, 20 * H / 26, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_IP);
			skp->Text(16 * W / 44, 21 * H / 26, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_IP);
			skp->Text(24 * W / 44, 21 * H / 26, Buffer, strlen(Buffer));
			FormatLatitude(Buffer, tab->lat_ZL);
			skp->Text(16 * W / 44, 22 * H / 26, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_ZL);
			skp->Text(24 * W / 44, 22 * H / 26, Buffer, strlen(Buffer));
			skpTextFmt(skp, 16 * W / 44, 23 * H / 26, "%.2lf %.2lf NM", tab->dlat_NM, tab->dlng_NM);
		}
		else if (tab->Indicator == 1)
		{
			skp->Text(16 * W / 44, 13 * H / 26, "NO DATA", 7);
		}
		else
		{
			skp->Text(16 * W / 44, 13 * H / 26, "ERROR - REFER TO ONLINE", 23);
		}
	}
	else if (screen == 105)
	{
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(2 * W / 8, 2 * H / 26, "Retrofire External DV (MSK 329)", 31);

		skp->Text(1 * W / 16, 10 * H / 26, "GETI", 4);
		skp->Text(1 * W / 16, 11 * H / 26, "VGX XDV", 7);
		skp->Text(1 * W / 16, 12 * H / 26, "VGY XDV", 7);
		skp->Text(1 * W / 16, 13 * H / 26, "VGZ XDV", 7);
		skp->Text(1 * W / 16, 14 * H / 26, "DEL P", 5);
		skp->Text(1 * W / 16, 15 * H / 26, "DEL Y", 5);
		skp->Text(1 * W / 16, 16 * H / 26, "DT TO", 5);
		skp->Text(1 * W / 16, 17 * H / 26, "DV TO", 5);
		skp->Text(1 * W / 16, 18 * H / 26, "HA", 2);
		skp->Text(1 * W / 16, 19 * H / 26, "HP", 2);
		skp->Text(1 * W / 16, 20 * H / 26, "LAT IP", 6);
		skp->Text(1 * W / 16, 21 * H / 26, "LNG IP", 6);

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(9 * W / 16, 8 * H / 26, "PRIMARY", 7);
		skp->Text(15 * W / 16, 8 * H / 26, "MANUAL", 6);

		if (GC->rtcc->RetrofireEXDV.data.size() > 0)
		{
			for (unsigned i = 0;i < 2;i++)
			{
				RTCC::RetrofireExternalDVDisplayData *tab = &GC->rtcc->RetrofireEXDV.data[i];

				if (tab->Indicator)
				{
					GET_Display2(Buffer, tab->GETI);
					skp->Text((9 + i * 6) * W / 16, 10 * H / 26, Buffer, strlen(Buffer));

					AGC_Display(Buffer, tab->DV.x);
					skp->Text((9 + i * 6) * W / 16, 11 * H / 26, Buffer, strlen(Buffer));
					AGC_Display(Buffer, tab->DV.y);
					skp->Text((9 + i * 6) * W / 16, 12 * H / 26, Buffer, strlen(Buffer));
					AGC_Display(Buffer, tab->DV.z);
					skp->Text((9 + i * 6) * W / 16, 13 * H / 26, Buffer, strlen(Buffer));

					skpTextFmt(skp, (9 + i * 6) * W / 16, 14 * H / 26, "%+.2lf", tab->P_G);
					skpTextFmt(skp, (9 + i * 6) * W / 16, 15 * H / 26, "%+.2lf", tab->Y_G);
					skpTextFmt(skp, (9 + i * 6) * W / 16, 16 * H / 26, "%.2lf", tab->DT_TO);
					skpTextFmt(skp, (9 + i * 6) * W / 16, 17 * H / 26, "%.2lf", tab->DV_TO);
					if (tab->H_apo < 99999.9)
					{
						skpTextFmt(skp, (9 + i * 6) * W / 16, 18 * H / 26, "%.1lf", tab->H_apo);
					}
					skpTextFmt(skp, (9 + i * 6) * W / 16, 19 * H / 26, "%.1lf", tab->H_peri);
					FormatLatitude(Buffer, tab->lat_IP);
					skp->Text((9 + i * 6) * W / 16, 20 * H / 26, Buffer, strlen(Buffer));
					FormatLongitude(Buffer, tab->lng_IP);
					skp->Text((9 + i * 6) * W / 16, 21 * H / 26, Buffer, strlen(Buffer));
				}
				else
				{
					skp->Text((9 + i * 6) * W / 16, 13 * H / 26, "NO DATA", 7);
				}
			}
		}
	}
	else if (screen == 106)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 26, "Recovery Target Selection (MSK 1454)", 36);
		skp->SetFont(font3);

		skpTextFmt(skp, 7 * W / 8, 4 * H / 14, "%d/%d", GC->rtcc->RZDRTSD.CurrentPage, GC->rtcc->RZDRTSD.TotalNumPages);

		//skp->Text(2 * W / 44, 5 * H / 26, "N", 1);
		skp->Text(4 * W / 44, 10 * H / 26, "REV", 3);
		skp->Text(9 * W / 44, 10 * H / 26, "GET", 3);
		skp->Text(17 * W / 44, 10 * H / 26, "GMT", 3);
		skp->Text(24 * W / 44, 10 * H / 26, "LAT", 3);
		skp->Text(31 * W / 44, 10 * H / 26, "LNG", 3);
		skp->Text(39 * W / 44, 10 * H / 26, "AZI", 3);

		int j = (GC->rtcc->RZDRTSD.CurrentPage - 1) * 10;
		for (int i = 0; i < 10;i++)
		{
			if (GC->rtcc->RZDRTSD.table[i + j].DataIndicator) break;

			skpTextFmt(skp, 2 * W / 44, (12 + i) * H / 26, "%d", i + j + 1);
			skpTextFmt(skp, 4 * W / 44, (12 + i) * H / 26, "%d", GC->rtcc->RZDRTSD.table[i + j].Rev);

			GET_Display(Buffer, GC->rtcc->RZDRTSD.table[i + j].GET, false);
			skp->Text(9 * W / 44, (12 + i) * H / 26, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->RZDRTSD.table[i + j].GMT, false);
			skp->Text(17 * W / 44, (12 + i) * H / 26, Buffer, strlen(Buffer));

			FormatLatitude(Buffer, GC->rtcc->RZDRTSD.table[i + j].Latitude);
			skp->Text(24 * W / 44, (12 + i) * H / 26, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, GC->rtcc->RZDRTSD.table[i + j].Longitude);
			skp->Text(31 * W / 44, (12 + i) * H / 26, Buffer, strlen(Buffer));

			skpTextFmt(skp, 39 * W / 44, (12 + i) * H / 26, "%06.2lf°", GC->rtcc->RZDRTSD.table[i + j].Azimuth);
		}
	}
	else if (screen == 107)
	{
		skp->Text(4 * W / 8, 1 * H / 32, "Abort Scan Table Inputs", 23);

		if (G->RTEASTType == 75)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Unspecified Area", 21);
		}
		else if (G->RTEASTType == 76)
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Specific Site", 13);
		}
		else
		{
			skp->Text(1 * W / 16, 2 * H / 14, "Lunar Search", 12);
		}

		//4: Type TCUA, FCUA or Site
		//6: Vector time (all)
		//8: Abort time (F75 and F76), Min abort time (F77)
		//10: Delta V (F75), Max abort time (F77)
		//12: Landing time (F76 and F77)
		//4: Entry Profile
		//6: Miss Distance (F76 and F77, PTP only)
		//8: Inclination (F77)

		if (GC->MissionPlanningActive)
		{
			GET_Display(Buffer, GC->rtcc->med_f75_f77.T_V, false);
			skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));
		}

		GET_Display(Buffer, GC->rtcc->med_f75_f77.T_0_min, false);
		skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

		Text(skp, 10 * W / 16, 4 * H / 14, GC->rtcc->med_f75_f77.EntryProfile);

		skpTextFmt(skp, 10 * W / 16, 8 * H / 14, "%.2lf°", GC->rtcc->med_f75_f77.Inclination* DEG);
		
		if (G->RTEASTType == 75)
		{
			Text(skp, 1 * W / 16, 4 * H / 14, GC->rtcc->med_f75.Type);

			skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.0lf ft/s", GC->rtcc->med_f75.DVMAX);
		}
		else if (G->RTEASTType == 76)
		{
			Text(skp, 1 * W / 16, 4 * H / 14, GC->rtcc->med_f76.Site);

			GET_Display(Buffer, GC->rtcc->med_f75_f77.T_Z, false);
			skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			Text(skp, 1 * W / 16, 4 * H / 14, GC->rtcc->med_f77.Site);

			if (GC->rtcc->med_f77.Site != "FCUA")
			{
				GET_Display(Buffer, GC->rtcc->med_f75_f77.T_Z, false);
				skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));
			}
		}
	}
	else if (screen == 108)
	{
		skp->Text(2 * W / 8, 2 * H / 32, "ABORT SCAN TABLE (MSK 362)", 26);

		skp->SetFont(font2);
		skp->SetPen(pen2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT, oapi::Sketchpad::BASELINE);

		skp->Line(0, 5 * H / 32, W, 5 * H / 32);
		skp->Line(0, 9 * H / 32, W, 9 * H / 32);
		skp->Line(4 * W / 32, 5 * H / 32, 4 * W / 32, 31 * H / 32);
		skp->Line(8 * W / 32, 5 * H / 32, 8 * W / 32, 31 * H / 32);
		skp->Line(14 * W / 32, 5 * H / 32, 14 * W / 32, 31 * H / 32);
		skp->Line(18 * W / 32, 5 * H / 32, 18 * W / 32, 31 * H / 32);
		skp->Line(22 * W / 32, 5 * H / 32, 22 * W / 32, 31 * H / 32);
		skp->Line(55 * W / 64, 5 * H / 32, 55 * W / 64, 31 * H / 32);

		skp->Text(1 * W / 64, 6 * H / 32, "CODE", 4);

		skp->Text(5 * W / 32, 6 * H / 32, "SITE", 4);
		skp->Text(5 * W / 32, 7 * H / 32, "AM", 2);

		skp->Text(10 * W / 32, 6 * H / 32, "GETI", 4);
		skp->Text(10 * W / 32, 7 * H / 32, "GETV", 4);

		skp->Text(15 * W / 32, 6 * H / 32, "DV", 2);
		skp->Text(15 * W / 32, 7 * H / 32, "INCL", 4);
		skp->Text(15 * W / 32, 8 * H / 32, "HPC", 3);

		skp->Text(19 * W / 32, 6 * H / 32, "VEI", 3);
		skp->Text(19 * W / 32, 7 * H / 32, "GEI", 3);

		skp->Text(24 * W / 32, 6 * H / 32, "GETEI", 5);
		skp->Text(24 * W / 32, 7 * H / 32, "GETL", 4);

		skp->Text(56 * W / 64, 6 * H / 32, "LAT IP", 6);
		skp->Text(56 * W / 64, 7 * H / 32, "LNG IP", 6);

		ASTData *tab;
		for (int i = 0;i < 7;i++)
		{
			tab = &GC->rtcc->PZREAP.AbortScanTableData[i];
			if (tab->ASTCode == 0) continue;

			skpTextFmt(skp, 1 * W / 32, (10 + 3 * i) * H / 32, "%d", tab->ASTCode);

			Text(skp, 9 * W / 64, (10 + 3 * i) * H / 32, tab->SiteID);
			Text(skp, 9 * W / 64, (11 + 3 * i) * H / 32, tab->AbortMode);

			GET_Display(Buffer, GC->rtcc->GETfromGMT(tab->AbortGMT), false);
			skp->Text(9 * W / 32, (10 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->GETfromGMT(tab->VectorGMT), false);
			skp->Text(9 * W / 32, (11 + 3 * i) * H / 32, Buffer, strlen(Buffer));

			if (tab->dv >= 304.8)
			{
				sprintf_s(Buffer, "%.0lf", tab->dv / 0.3048);
			}
			else if (tab->dv >= 30.48)
			{
				sprintf_s(Buffer, "%.1lf", tab->dv / 0.3048);
			}
			else
			{
				sprintf_s(Buffer, "%.2lf", tab->dv / 0.3048);
			}
			skp->Text(29 * W / 64, (10 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			if (tab->incl_EI < 0)
			{
				sprintf(Buffer, "A%.2f", abs(tab->incl_EI*DEG));
			}
			else
			{
				sprintf(Buffer, "D%.2f", tab->incl_EI*DEG);
			}
			skp->Text(29 * W / 64, (11 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			skpTextFmt(skp, 29 * W / 64, (12 + 3 * i) * H / 32, "%.1lf", tab->h_PC / 1852.0);

			skpTextFmt(skp, 19 * W / 32, (10 + 3 * i) * H / 32, "%.0lf", tab->v_EI / 0.3048);
			skpTextFmt(skp, 19 * W / 32, (11 + 3 * i) * H / 32, "%.2lf", tab->gamma_EI* DEG);

			GET_Display(Buffer, GC->rtcc->GETfromGMT(tab->ReentryGMT), false);
			skp->Text(23 * W / 32, (10 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			GET_Display(Buffer, GC->rtcc->GETfromGMT(tab->SplashdownGMT), false);
			skp->Text(23 * W / 32, (11 + 3 * i) * H / 32, Buffer, strlen(Buffer));

			FormatLatitude(Buffer, tab->lat_SPL*DEG);
			skp->Text(56 * W / 64, (10 + 3 * i) * H / 32, Buffer, strlen(Buffer));
			FormatLongitude(Buffer, tab->lng_SPL*DEG);
			skp->Text(56 * W / 64, (11 + 3 * i) * H / 32, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 109)
	{
		skp->Text(2 * W / 8, 2 * H / 32, "RTE Digitals Manual Maneuver Input", 26);

		if (GC->MissionPlanningActive)
		{
			GET_Display(Buffer, GC->rtcc->med_f81.VectorTime, false);
			skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));
		}

		GET_Display(Buffer, GC->rtcc->med_f81.IgnitionTime, false);
		skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.2lf°", GC->rtcc->med_f81.lat_tgt * DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2lf°", GC->rtcc->med_f81.lng_tgt * DEG);

		if (GC->rtcc->med_f81.RefBody == BODY_EARTH)
		{
			skp->Text(1 * W / 16, 10 * H / 14, "EARTH", 5);
		}
		else
		{
			skp->Text(1 * W / 16, 10 * H / 14, "MOON", 4);
		}

		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%+.1lf %+.1lf %+.1lf", GC->rtcc->med_f81.XDV.x / 0.3048, GC->rtcc->med_f81.XDV.y / 0.3048, GC->rtcc->med_f81.XDV.z / 0.3048);

		skp->Text(8 * W / 16, 10 * H / 14, "Load splashdown target", 22);
	}
	else if (screen == 110)
	{
		skp->Text(2 * W / 8, 2 * H / 32, "RTE Digitals Entry Profile", 26);

		Text(skp, 1 * W / 16, 2 * H / 14, GC->rtcc->med_f82.PrimaryEP);

		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.2lf°", GC->rtcc->med_f82.PrimaryInitialBank * DEG);

		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.2lf", GC->rtcc->med_f82.PrimaryGLIT);

		if (GC->rtcc->med_f82.PrimaryEP == "HB1")
		{
			Text(skp, 1 * W / 16, 8 * H / 14, GC->rtcc->med_f82.PrimaryRollDirection);

			if (GC->rtcc->med_f82.PrimaryLongT > 999.9)
			{
				skp->Text(1 * W / 16, 10 * H / 14, "Constant G", 10);
			}
			else
			{
				skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.2lf°", GC->rtcc->med_f82.PrimaryLongT * DEG);
			}
		}

		Text(skp, 10 * W / 16, 2 * H / 14, GC->rtcc->med_f82.BackupEP);

		skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%.2lf°", GC->rtcc->med_f82.BackupInitialBank * DEG);

		skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%.2lf", GC->rtcc->med_f82.BackupGLIT);

		if (GC->rtcc->med_f82.BackupEP == "HB1")
		{
			Text(skp, 10 * W / 16, 8 * H / 14, GC->rtcc->med_f82.PrimaryRollDirection);

			if (GC->rtcc->med_f82.BackupLongT > 999.9)
			{
				skp->Text(10 * W / 16, 10 * H / 14, "Constant G", 10);
			}
			else
			{
				skpTextFmt(skp, 10 * W / 16, 10 * H / 14, "%.2lf°", GC->rtcc->med_f82.BackupLongT * DEG);
			}
		}
	}
	else if (screen == 111 || screen == 112)
	{
		int blocknum;
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		if (screen == 111)
		{
			skp->Text(1 * W / 2, 3 * H / 32, "CMC TIME INCREMENT UPDATE (MSK 340)", 35);
			blocknum = 0;
		}
		else
		{
			skp->Text(1 * W / 2, 3 * H / 32, "LGC TIME INCREMENT UPDATE (MSK 353)", 35);
			blocknum = 1;
		}
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		RTCC::AGCTimeIncrementMakeupTableBlock *block = &GC->rtcc->CZTMEINC.Blocks[blocknum];

		skp->Text(1 * W / 16, 3 * H / 14, "RTCC TIME:", 10);
		GET_Display2(Buffer, G->RTCCClockTime[blocknum]);
		skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 5 * H / 14, "AGC TIME:", 9);
		GET_Display2(Buffer, G->AGCClockTime[blocknum]);
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 7 * H / 14, "DELTA T:", 8);
		GET_Display2(Buffer, G->DeltaClockTime[blocknum]);
		skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 10 * H / 14, "AGC CLOCK ZERO:", 15);
		if (blocknum == 0)
		{
			GET_Display2(Buffer, GC->rtcc->GetCMCClockZero());
		}
		else
		{
			GET_Display2(Buffer, GC->rtcc->GetLGCClockZero());
		}
		skp->Text(1 * W / 16, 11 * H / 14, Buffer, strlen(Buffer));

		skp->Text(8 * W / 16, 5 * H / 14, "OID", 3);
		skp->Text(8 * W / 16, 6 * H / 14, "1", 1);
		skp->Text(8 * W / 16, 7 * H / 14, "2", 1);

		skp->Text(10 * W / 16, 5 * H / 14, "FCT", 3);
		skp->Text(10 * W / 16, 6 * H / 14, "DELTAT", 6);
		skp->Text(10 * W / 16, 7 * H / 14, "DELTAT", 6);

		skp->Text(12 * W / 16, 5 * H / 14, "DSKY V73", 8);

		skpTextFmt(skp, 13 * W / 16, 6 * H / 14, "%05d", block->Octals[0]);
		skpTextFmt(skp, 13 * W / 16, 7 * H / 14, "%05d", block->Octals[1]);

		skp->Text(8 * W / 16, 9 * H / 14, "INCREMENT:", 10);
		GET_Display2(Buffer, block->TimeIncrement);
		skp->Text(8 * W / 16, 10 * H / 14, Buffer, strlen(Buffer));
	}
	else if (screen == 113 || screen == 114)
	{
		int blocknum;
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		if (screen == 113)
		{
			skp->Text(1 * W / 2, 3 * H / 32, "CMC LIFTOFF TIME UPDATE (MSK 341)", 33);
			blocknum = 0;
		}
		else
		{
			skp->Text(1 * W / 2, 3 * H / 32, "LGC LIFTOFF TIME UPDATE (MSK 339)", 33);
			blocknum = 1;
		}
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		RTCC::AGCLiftoffTimeUpdateMakeupTableBlock *block = &GC->rtcc->CZLIFTFF.Blocks[blocknum];

		skp->Text(1 * W / 16, 3 * H / 14, "DESIRED LIFTOFF:", 16);
		GET_Display2(Buffer, G->DesiredRTCCLiftoffTime[blocknum]);
		skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 5 * H / 14, "AGC CLOCK ZERO:", 15);
		double lotime;
		if (blocknum == 0)
		{
			lotime = GC->rtcc->GetCMCClockZero();
		}
		else
		{
			lotime = GC->rtcc->GetLGCClockZero();
		}
		GET_Display2(Buffer, lotime);
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 7 * H / 14, "DELTA T:", 8);
		GET_Display2(Buffer, G->DesiredRTCCLiftoffTime[blocknum] - lotime);
		skp->Text(1 * W / 16, 8 * H / 14, Buffer, strlen(Buffer));

		skp->Text(1 * W / 16, 10 * H / 14, "RTCC LIFTOFF:", 15);
		GET_Display2(Buffer, GC->rtcc->GetGMTLO()*3600.0);
		skp->Text(1 * W / 16, 11 * H / 14, Buffer, strlen(Buffer));

		skp->Text(8 * W / 16, 4 * H / 14, "OID", 3);
		skp->Text(8 * W / 16, 5 * H / 14, "1", 1);
		skp->Text(8 * W / 16, 6 * H / 14, "2", 1);

		skp->Text(10 * W / 16, 4 * H / 14, "FCT", 3);
		skp->Text(10 * W / 16, 5 * H / 14, "DELTAT", 6);
		skp->Text(10 * W / 16, 6 * H / 14, "DELTAT", 6);

		skp->Text(12 * W / 16, 4 * H / 14, "DSKY V70", 8);

		skpTextFmt(skp, 13 * W / 16, 5 * H / 14, "%05d", block->Octals[0]);
		skpTextFmt(skp, 13 * W / 16, 6 * H / 14, "%05d", block->Octals[1]);

		skp->Text(8 * W / 16, 8 * H / 14, "INCREMENT:", 10);
		GET_Display2(Buffer, block->TimeIncrement);
		skp->Text(8 * W / 16, 9 * H / 14, Buffer, strlen(Buffer));

		//skp->Text(8 * W / 16, 10 * H / 14, "TEPHEM:", 10);

		//sprintf_s(Buffer, "%05o", OrbMech::DoubleToBuffer(lotime*100.0, 56.0, 0));
		//skp->Text(8 * W / 16, 11 * H / 14, Buffer, strlen(Buffer));
		//sprintf_s(Buffer, "%05o", OrbMech::DoubleToBuffer(lotime*100.0, 42.0, 0));
		//skp->Text(8 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));
		//sprintf_s(Buffer, "%05o", OrbMech::DoubleToBuffer(lotime*100.0, 28.0, 0));
		//skp->Text(8 * W / 16, 13 * H / 14, Buffer, strlen(Buffer));
	}
	else if (screen == 115)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(1 * W / 2, 3 * H / 32, "LUNAR TARGETING PROGRAM", 33);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		GET_Display(Buffer, G->LUNTAR_TIG, false);
		skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 4 * H / 14, "%.1lf s", G->LUNTAR_bt_guess);
		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%.2lf°", G->LUNTAR_pitch_guess* DEG);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.2lf°", G->LUNTAR_yaw_guess* DEG);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.2lf°", G->LUNTAR_lat* DEG);
		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.2lf°", G->LUNTAR_lng* DEG);

		if (G->target != NULL)
		{
			sprintf(Buffer, G->target->GetName());
			skp->Text(10 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(10 * W / 16, 2 * H / 14, "No S-IVB!", 9);
		}

		skp->Text(10 * W / 16, 9 * H / 20, "Burn Data:", 10);
		skpTextFmt(skp, 10 * W / 16, 10 * H / 20, "TIG: TB8 + %.0lf s", G->LUNTAR_Output.tig);
		skpTextFmt(skp, 10 * W / 16, 11 * H / 20, "Burn time: %.1lf s", G->LUNTAR_Output.bt);
		skpTextFmt(skp, 10 * W / 16, 12 * H / 20, "Pitch: %.1lf°", G->LUNTAR_Output.pitch * DEG);
		skpTextFmt(skp, 10 * W / 16, 13 * H / 20, "Yaw: %.1lf°", G->LUNTAR_Output.yaw * DEG);

		skp->Text(10 * W / 16, 14 * H / 20, "Impact:", 7);
		GET_Display(Buffer, G->LUNTAR_Output.get_imp);
		skp->Text(10 * W / 16, 15 * H / 20, Buffer, strlen(Buffer));
		skpTextFmt(skp, 10 * W / 16, 16 * H / 20, "Lat: %.2lf°", G->LUNTAR_Output.lat_imp * DEG);
		skpTextFmt(skp, 10 * W / 16, 17 * H / 20, "Lng: %.2lf°", G->LUNTAR_Output.lng_imp * DEG);

		if (G->LUNTAR_Output.err > 0)
		{
			const char *emsg = "";
			if (G->LUNTAR_Output.err == 1)
			{
				emsg = "INITIAL GUESS DID NOT IMPACT";
			}
			else if (G->LUNTAR_Output.err == 2)
			{
				emsg = "SOLUTION DID NOT CONVERGE";
			}
			else if (G->LUNTAR_Output.err == 3)
			{
				emsg = "TIMEBASE 8 NOT STARTED";
			}
			skp->Text(4 * W / 16, 26 * H / 28, emsg, strlen(emsg));
		}
	}
	else if (screen == 116)
	{
		int hh, mm;
		double secs;

		skp->Text(2 * W / 8, 2 * H / 26, "Retrofire Separation (MSK 355)", 30);

		skp->SetFont(font3);
		skp->SetPen(pen2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(2 * W / 44, 5 * H / 26, "AREA", 4);
		skp->Text(2 * W / 44, 6 * H / 26, "MATRIX", 6);
		skp->Text(2 * W / 44, 7 * H / 26, "WT TAA", 6);
		skp->Text(2 * W / 44, 8 * H / 26, "RLH PLH YLH", 11);
		skp->Text(2 * W / 44, 9 * H / 26, "RO PI YM", 8);
		skp->Text(2 * W / 44, 10 * H / 26, "VC BT", 5);
		skp->Text(2 * W / 44, 11 * H / 26, "VT U DT", 7);
		skp->Text(2 * W / 44, 12 * H / 26, "H", 1);
		skp->Text(2 * W / 44, 13 * H / 26, "GETI", 4);
		skp->Text(2 * W / 44, 14 * H / 26, "GMTI", 4);

		skp->Line(14 * W / 44, 6 * H / 26, 14 * W / 44, 24 * H / 26);

		RetrofireDisplayParametersTableData *tab = &GC->rtcc->RZRFDP.data[2];

		if (tab->Indicator_Sep == 0)
		{
			Text(skp, 16 * W / 44, 6 * H / 26, tab->RefsID);
			skpTextFmt(skp, 16 * W / 44, 7 * H / 26, "%.1lf", tab->CSMWeightSep);
			skpTextFmt(skp, 22 * W / 44, 7 * H / 26, "%.0lf", tab->TrueAnomalySep);
			skpTextFmt(skp, 16 * W / 44, 8 * H / 26, "%.1lf %.1lf %.1lf", tab->Att_LVLH_Sep.x, tab->Att_LVLH_Sep.y, tab->Att_LVLH_Sep.z);
			skpTextFmt(skp, 16 * W / 44, 9 * H / 26, "%.1lf %.1lf %.1lf", tab->Att_IMU_Sep.x, tab->Att_IMU_Sep.y, tab->Att_IMU_Sep.z);
			skpTextFmt(skp, 16 * W / 44, 10 * H / 26, "%.1lf", tab->DVC_Sep);
			SStoHHMMSS(tab->BurnTime_Sep, hh, mm, secs);
			skpTextFmt(skp, 22 * W / 44, 10 * H / 26, "%02d:%04.1lf", mm, secs);
			skpTextFmt(skp, 16 * W / 44, 11 * H / 26, "%.1lf", tab->DVT_Sep);
			skpTextFmt(skp, 21 * W / 44, 11 * H / 26, "%+d", tab->UllageQuads_Sep);
			skpTextFmt(skp, 24 * W / 44, 11 * H / 26, "%.1lf", tab->UllageDT_Sep);
			skpTextFmt(skp, 16 * W / 44, 12 * H / 26, "%.1lf", tab->H_Sep);
			GET_Display2(Buffer, tab->GETI_Sep);
			skp->Text(16 * W / 44, 13 * H / 26, Buffer, strlen(Buffer));
			GET_Display(Buffer, tab->GMTI_Sep, false);
			skp->Text(16 * W / 44, 14 * H / 26, Buffer, strlen(Buffer));
		}
		else if (tab->Indicator_Sep == 1)
		{
			skp->Text(16 * W / 44, 13 * H / 26, "NO DATA", 7);
		}
		else
		{
			skp->Text(16 * W / 44, 13 * H / 26, "ERROR - REFER TO ONLINE", 23);
		}
	}
	else if (screen == 117)
	{
		skp->Text(1 * W / 8, 2 * H / 26, "Definition of Separation/Shaping Maneuver", 41);

		if (GC->rtcc->RZJCTTC.R30_GETI_SH > 0)
		{
			GET_Display2(Buffer, GC->rtcc->RZJCTTC.R30_GETI_SH);
		}
		else
		{
			sprintf(Buffer, "No Shaping Maneuver");
		}
		skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->RZJCTTC.R30_GETI_SH > 0)
		{
			sprintf(Buffer, "No Separation Maneuver");
		}
		else
		{
			sprintf(Buffer, "%.1lf min", GC->rtcc->RZJCTTC.R30_DeltaT_Sep/60.0);
		}
		skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

		ThrusterName(Buffer, GC->rtcc->RZJCTTC.R30_Thruster);
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.1lf ft/s", GC->rtcc->RZJCTTC.R30_DeltaV / 0.3048);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.1lf s", GC->rtcc->RZJCTTC.R30_DeltaT);
		skpTextFmt(skp, 1 * W / 16, 12 * H / 14, "%.2lf %.2lf %.2lf", GC->rtcc->RZJCTTC.R30_Att.x* DEG, GC->rtcc->RZJCTTC.R30_Att.y* DEG, GC->rtcc->RZJCTTC.R30_Att.z* DEG);

		if (GC->rtcc->RZJCTTC.R30_Thruster == RTCC_ENGINETYPE_CSMSPS)
		{
			skpTextFmt(skp, 9 * W / 16, 2 * H / 14, "%.1lf s", GC->rtcc->RZJCTTC.R30_Ullage_DT);

			if (GC->rtcc->RZJCTTC.R30_Use4UllageThrusters)
			{
				sprintf(Buffer, "4 jet ullage");
			}
			else
			{
				sprintf(Buffer, "2 jet ullage");
			}
			skp->Text(9 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

			if (GC->rtcc->RZJCTTC.R30_GimbalIndicator == 1)
			{
				sprintf(Buffer, "Use System Parameters");
			}
			else
			{
				sprintf(Buffer, "Compute Gimbal Trims");
			}
			skp->Text(9 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 118)
	{
		skp->Text(4 * W / 8, 1 * H / 28, "Retrofire Planning", 18);

		skp->Text(1 * W / 8, 2 * H / 14, "Separation/Shaping Constraints", 30);
		skp->Text(1 * W / 8, 4 * H / 14, "Retrofire Constraints", 21);
		skp->Text(1 * W / 8, 6 * H / 14, "Target Selection Display", 24);
		skp->Text(1 * W / 8, 8 * H / 14, "Retrofire Maneuver", 18);
	}
	else if (screen == 119)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(4 * W / 8, 2 * H / 14, "CMC ENTRY UPDATE (343)", 38);

		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(5 * W / 32, 8 * H / 28, "LAT", 3);
		skpTextFmt(skp, 8 * W / 32, 8 * H / 28, "%+.2f°", G->EntryLatcor* DEG);

		skp->Text(14 * W / 32, 8 * H / 28, "LNG", 3);
		skpTextFmt(skp, 17 * W / 32, 8 * H / 28, "%+.2f°", G->EntryLngcor* DEG);

		skp->Text(5 * W / 32, 10 * H / 28, "OID", 3);
		skp->Text(10 * W / 32, 10 * H / 28, "FCT", 3);
		skp->Text(15 * W / 32, 10 * H / 28, "DSKY V71", 8);

		for (int i = 1;i <= 6;i++)
		{
			skpTextFmt(skp, 5 * W / 32, (i + 11) * H / 28, "%o", i);
		}

		skp->Text(10 * W / 32, 12 * H / 28, "INDEX", 5);
		skp->Text(10 * W / 32, 13 * H / 28, "ADD", 3);
		skp->Text(10 * W / 32, 14 * H / 28, "LAT", 3);
		skp->Text(10 * W / 32, 15 * H / 28, "LAT", 3);
		skp->Text(10 * W / 32, 16 * H / 28, "LONG", 4);
		skp->Text(10 * W / 32, 17 * H / 28, "LONG", 4);

		for (int i = 0;i < 6;i++)
		{
			skpTextFmt(skp, 15 * W / 32, (i + 12) * H / 28, "%05d", GC->rtcc->CZENTRY.Octals[i]);
		}

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		skp->Text(27 * W / 32, 10 * H / 28, "DECIMAL", 7);
		skpTextFmt(skp, 27 * W / 32, 14 * H / 28, "%+.2f°", GC->rtcc->CZENTRY.Lat);
		skpTextFmt(skp, 27 * W / 32, 16 * H / 28, "%+.2f°", GC->rtcc->CZENTRY.Lng);

	}
	else if (screen == 120)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 28, "LANDMARK ACQUISITION (MSK 1508)", 40);

		skp->SetFont(font2);

		skp->Text(22 * W / 32, 4 * H / 28, "STA ID", 6);

		skp->Text(4 * W / 64, 6 * H / 28, "STA", 3);
		skp->Text(13 * W / 64, 6 * H / 28, "GETAOS", 6);
		skp->Text(22 * W / 64, 6 * H / 28, "GETCA", 5);
		skp->Text(29 * W / 64, 6 * H / 28, "LAM", 3);
		skp->Text(33 * W / 64, 6 * H / 28, "H", 1);
		skp->Text(39 * W / 64, 6 * H / 28, "GETLOS", 6);
		skp->Text(48 * W / 64, 6 * H / 28, "GETSR", 5);
		skp->Text(57 * W / 64, 6 * H / 28, "GETSS", 5);

		skpTextFmt(skp, 6 * W / 32, 4 * H / 28, "PAGE %02d OF %02d", GC->rtcc->EZLANDU1.curpage, GC->rtcc->EZLANDU1.pages);

		Text(skp, 26 * W / 32, 4 * H / 28, GC->rtcc->EZLANDU1.VectorID);

		for (unsigned i = 0;i < GC->rtcc->EZLANDU1.numcontacts[GC->rtcc->EZLANDU1.curpage - 1];i++)
		{
			Text(skp, 4 * W / 64, (i + 8) * H / 28, GC->rtcc->EZLANDU1.STAID[GC->rtcc->EZLANDU1.curpage - 1][i]);

			if (GC->rtcc->EZLANDU1.BestAvailableAOS[GC->rtcc->EZLANDU1.curpage - 1][i])
			{
				skp->Text(12 * W / 64, (i + 8) * H / 28, "*", 1);
			}
			GET_Display(Buffer, GC->rtcc->EZLANDU1.GETAOS[GC->rtcc->EZLANDU1.curpage - 1][i], false);
			skp->Text(13 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));

			if (GC->rtcc->EZLANDU1.BestAvailableCA[GC->rtcc->EZLANDU1.curpage - 1][i])
			{
				skp->Text(21 * W / 64, (i + 8) * H / 28, "*", 1);
			}
			GET_Display(Buffer, GC->rtcc->EZLANDU1.GETCA[GC->rtcc->EZLANDU1.curpage - 1][i], false);
			skp->Text(22 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));

			skpTextFmt(skp, 29 * W / 64, (i + 8) * H / 28, "%.1lf", GC->rtcc->EZLANDU1.Lambda[GC->rtcc->EZLANDU1.curpage - 1][i]);

			skpTextFmt(skp, 33 * W / 64, (i + 8) * H / 28, "%.0lf", GC->rtcc->EZLANDU1.h[GC->rtcc->EZLANDU1.curpage - 1][i]);

			if (GC->rtcc->EZLANDU1.BestAvailableLOS[GC->rtcc->EZLANDU1.curpage - 1][i])
			{
				skp->Text(35 * W / 64, (i + 8) * H / 28, "*", 1);
			}
			GET_Display(Buffer, GC->rtcc->EZLANDU1.GETLOS[GC->rtcc->EZLANDU1.curpage - 1][i], false);
			skp->Text(39 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));

			GET_Display_HHMM(Buffer, GC->rtcc->EZLANDU1.GETSR[GC->rtcc->EZLANDU1.curpage - 1][i]);
			skp->Text(48 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));

			GET_Display_HHMM(Buffer, GC->rtcc->EZLANDU1.GETSS[GC->rtcc->EZLANDU1.curpage - 1][i]);
			skp->Text(57 * W / 64, (i + 8) * H / 28, Buffer, strlen(Buffer));
		}

		if (GC->rtcc->EZLANDU1.err > 0)
		{
			skpTextFmt(skp, 4 * W / 16, 26 * H / 28, "Error: %d", GC->rtcc->EZLANDU1.err);
		}
	}
	else if (screen == 121)
	{
		skp->Text(1 * W / 8, 2 * H / 28, "LAUNCH WINDOW PROCESSOR INPUTS", 30);

		switch (GC->rtcc->PZSLVCON.LOT)
		{
		case 1:
			sprintf(Buffer, "Input time");
			break;
		case 2:
			sprintf(Buffer, "Phase angle offset");
			break;
		case 3:
			sprintf(Buffer, "Biased phase zero (GMTLOR)");
			break;
		case 4:
			sprintf(Buffer, "Biased phase zero (TPLANE)");
			break;
		case 5:
			sprintf(Buffer, "In-plane");
			break;
		case 6:
			sprintf(Buffer, "In-plane with nodal regression");
			break;
		default:
			sprintf(Buffer, "");
			break;
		}
		skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->PZSLVCON.LOT == 1)
		{
			GET_Display2(Buffer, GC->rtcc->PZSLVCON.GMTLOR);
			skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
		}

		skpTextFmt(skp, 1 * W / 16, 6 * H / 14, "%+.2lf", GC->rtcc->PZSLVCON.RINS);
		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%+.2lf", GC->rtcc->PZSLVCON.VINS);

		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%+.3lf°", GC->rtcc->PZSLVCON.GAMINS* DEG);

		if (G->target != NULL)
		{
			sprintf(Buffer, G->target->GetName());
			skp->Text(5 * W / 8, 2 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			skp->Text(5 * W / 8, 2 * H / 14, "No Target!", 10);
		}

		if (GC->rtcc->PZSLVCON.DELNOF)
		{
			sprintf(Buffer, "Compute DELNO");
			skp->Text(10 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
		}
		else
		{
			sprintf(Buffer, "Input DELNO");
			skp->Text(10 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));

			skpTextFmt(skp, 10 * W / 16, 6 * H / 14, "%+.3lf°", GC->rtcc->PZSLVCON.DELNO* DEG);
		}
		
	}
	else if (screen == 122)
	{
		skp->Text(3 * W / 8, 2 * H / 28, "LAUNCH TARGETING", 16);

		skp->SetFont(font2);

		skp->Text(1 * W / 32, 5 * H / 32, "GMTLO", 5);
		skp->Text(1 * W / 32, 6 * H / 32, "TINS", 4);

		skp->Text(1 * W / 32, 8 * H / 32, "GMTLO*", 6);
		skp->Text(1 * W / 32, 9 * H / 32, "PFA", 3);
		skp->Text(1 * W / 32, 10 * H / 32, "PFT", 3);
		skp->Text(1 * W / 32, 11 * H / 32, "DN", 2);

		skp->Text(1 * W / 32, 13 * H / 32, "TYAW", 4);
		skp->Text(1 * W / 32, 14 * H / 32, "TPLANE", 6);

		skp->Text(1 * W / 32, 16 * H / 32, "AZL", 3);
		skp->Text(1 * W / 32, 17 * H / 32, "LATLS", 5);
		skp->Text(1 * W / 32, 18 * H / 32, "LONGLS", 6);
		skp->Text(1 * W / 32, 19 * H / 32, "GPAZ", 4);
		skp->Text(1 * W / 32, 20 * H / 32, "YP", 2);

		skp->Text(1 * W / 32, 22 * H / 32, "DELNO", 5);
		skp->Text(1 * W / 32, 23 * H / 32, "DELNOD", 6);

		skp->Text(21 * W / 32, 5 * H / 32, "TGRR", 4);
		skp->Text(21 * W / 32, 6 * H / 32, "VIGM", 4);
		skp->Text(21 * W / 32, 8 * H / 32, "RIGM", 4);
		skp->Text(21 * W / 32, 10 * H / 32, "GIGM", 4);
		skp->Text(21 * W / 32, 11 * H / 32, "IIGM", 4);
		skp->Text(21 * W / 32, 12 * H / 32, "TIGM", 4);
		skp->Text(21 * W / 32, 13 * H / 32, "TDIGM", 5);

		skp->Text(21 * W / 32, 15 * H / 32, "LAUNCH ORBIT", 12);

		skp->Text(18 * W / 32, 17 * H / 32, "APOGEE", 6);
		skp->Text(18 * W / 32, 18 * H / 32, "PERIGEE", 7);
		skp->Text(18 * W / 32, 19 * H / 32, "T ANOMALY", 9);
		skp->Text(18 * W / 32, 20 * H / 32, "ALTITUDE", 8);
		skp->Text(18 * W / 32, 21 * H / 32, "DH", 2);
		skp->Text(18 * W / 32, 22 * H / 32, "TIME", 4);

		skp->Text(21 * W / 32, 24 * H / 32, "TARGET ORBIT", 12);

		skp->Text(18 * W / 32, 26 * H / 32, "APOGEE", 6);
		skp->Text(18 * W / 32, 27 * H / 32, "PERIGEE", 7);
		skp->Text(18 * W / 32, 28 * H / 32, "INCLINATION", 11);
		skp->Text(18 * W / 32, 29 * H / 32, "INS PHASE", 9);
		skp->Text(18 * W / 32, 30 * H / 32, "DN TARGET", 9);
		skp->Text(18 * W / 32, 31 * H / 32, "BIAS", 4);

		switch (G->iuUplinkResult)
		{
		case 1:
			sprintf(Buffer, "Uplink accepted!");
			break;
		case 2:
			sprintf(Buffer, "Vessel has no IU!");
			break;
		case 3:
			sprintf(Buffer, "Uplink rejected!");
			break;
		case 4:
			sprintf(Buffer, "No targeting parameters!");
			break;
		default:
			sprintf(Buffer, "No Uplink");
			break;
		}
		skp->Text(2 * W / 32, 28 * H / 32, Buffer, strlen(Buffer));

		skp->SetTextAlign(oapi::Sketchpad::RIGHT);

		GET_Display2(Buffer, GC->rtcc->PZSLVTAR.GMTLO);
		skp->Text(15 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->PZSLVTAR.TINS);
		skp->Text(15 * W / 32, 6 * H / 32, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->PZSLVTAR.GSTAR);
		skp->Text(15 * W / 32, 8 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 15 * W / 32, 9 * H / 32, "%.3lf", GC->rtcc->PZSLVCON.PFA* DEG);
		skpTextFmt(skp, 15 * W / 32, 10 * H / 32, "%.3lf", GC->rtcc->PZSLVCON.PFT);
		skpTextFmt(skp, 15 * W / 32, 11 * H / 32, "%+.3lf", GC->rtcc->PZSLVTAR.DN);
		GET_Display2(Buffer, GC->rtcc->PZSLVTAR.TYAW);
		skp->Text(15 * W / 32, 13 * H / 32, Buffer, strlen(Buffer));
		GET_Display2(Buffer, GC->rtcc->PZSLVTAR.TPLANE);
		skp->Text(15 * W / 32, 14 * H / 32, Buffer, strlen(Buffer));

		skpTextFmt(skp, 15 * W / 32, 16 * H / 32, "%.3lf", GC->rtcc->PZSLVTAR.AZL);
		skpTextFmt(skp, 15 * W / 32, 17 * H / 32, "%+.3lf", GC->rtcc->PZSLVCON.LATLS* DEG);
		skpTextFmt(skp, 15 * W / 32, 18 * H / 32, "%+.3lf", GC->rtcc->PZSLVCON.LONGLS* DEG);

		skpTextFmt(skp, 15 * W / 32, 22 * H / 32, "%.8lf", GC->rtcc->PZSLVTAR.DELNO);
		skpTextFmt(skp, 15 * W / 32, 23 * H / 32, "%.8lf", GC->rtcc->PZSLVTAR.DELNOD);

		GET_Display2(Buffer, GC->rtcc->PZSLVTAR.TGRR);
		skp->Text(31 * W / 32, 5 * H / 32, Buffer, strlen(Buffer));
		skpTextFmt(skp, 31 * W / 32, 6 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.VIGM);
		skpTextFmt(skp, 31 * W / 32, 7 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.VIGM / 0.3048);
		skpTextFmt(skp, 31 * W / 32, 8 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.RIGM);
		skpTextFmt(skp, 31 * W / 32, 9 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.RIGM / 0.3048);
		skpTextFmt(skp, 31 * W / 32, 10 * H / 32, "%.8lf", GC->rtcc->PZSLVTAR.GIGM);
		skpTextFmt(skp, 31 * W / 32, 11 * H / 32, "%.3lf", GC->rtcc->PZSLVTAR.IIGM);
		skpTextFmt(skp, 31 * W / 32, 12 * H / 32, "%.3lf", GC->rtcc->PZSLVTAR.TIGM);
		skpTextFmt(skp, 31 * W / 32, 13 * H / 32, "%+.7lf", GC->rtcc->PZSLVTAR.TDIGM);

		skpTextFmt(skp, 31 * W / 32, 17 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.HA_C);
		skpTextFmt(skp, 31 * W / 32, 18 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.HP_C);
		skpTextFmt(skp, 31 * W / 32, 19 * H / 32, "%+.2lf", GC->rtcc->PZSLVTAR.TA_C);
		skpTextFmt(skp, 31 * W / 32, 20 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.H);
		skpTextFmt(skp, 31 * W / 32, 21 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.DH);
		skpTextFmt(skp, 31 * W / 32, 22 * H / 32, "%E", GC->rtcc->PZSLVTAR.GMTLO);

		skpTextFmt(skp, 31 * W / 32, 26 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.HA_T);
		skpTextFmt(skp, 31 * W / 32, 27 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.HP_T);
		skpTextFmt(skp, 31 * W / 32, 28 * H / 32, "%.2lf", GC->rtcc->PZSLVTAR.I_T);
		skpTextFmt(skp, 31 * W / 32, 29 * H / 32, "%+.3lf", GC->rtcc->PZSLVTAR.PA);
		skpTextFmt(skp, 31 * W / 32, 30 * H / 32, "%+.3lf", GC->rtcc->PZSLVTAR.DN_T);
		skpTextFmt(skp, 31 * W / 32, 31 * H / 32, "%.3lf", GC->rtcc->PZSLVTAR.BIAS);
	}
	else if (screen == 123)
	{
		skp->SetTextAlign(oapi::Sketchpad::CENTER);
		skp->Text(4 * W / 8, 2 * H / 32, "RENDEZVOUS PLANNING TABLE", 27);
		skp->SetFont(font2);
		skp->SetTextAlign(oapi::Sketchpad::LEFT);

		skp->Text(1 * W / 32, 4 * H / 32, "CSM STA", 7);
		skp->Text(1 * W / 32, 5 * H / 32, "GMTV", 4);
		skp->Text(1 * W / 32, 6 * H / 32, "GETV", 4);
		skp->Text(1 * W / 32, 7 * H / 32, "MVR VEH", 7);

		skp->Text(33 * W / 64, 4 * H / 32, "LM STA", 6);
		skp->Text(33 * W / 64, 5 * H / 32, "GMTV", 4);
		skp->Text(33 * W / 64, 6 * H / 32, "GETV", 4);

		skp->Text(49 * W / 64, 5 * H / 32, "DV MAX", 6);
		skp->Text(49 * W / 64, 6 * H / 32, "MIN H", 5);
		skp->Text(49 * W / 64, 7 * H / 32, "WT", 2);

		skp->SetTextAlign(oapi::Sketchpad::CENTER);

		skp->Text(2 * W / 32, 9 * H / 32, "ID", 2);
		skp->Text(4 * W / 32, 9 * H / 32, "M", 1);
		skp->Text(7 * W / 32, 9 * H / 32, "DV CSM", 6);
		skp->Text(11 * W / 32, 9 * H / 32, "DV LM", 5);
		skp->Text(27 * W / 64, 9 * H / 32, "NC1", 3);
		skp->Text(32 * W / 64, 9 * H / 32, "NH", 2);
		skp->Text(37 * W / 64, 9 * H / 32, "NCC", 3);
		skp->Text(42 * W / 64, 9 * H / 32, "NSR", 3);
		skp->Text(47 * W / 64, 9 * H / 32, "NPC", 3);
		skp->Text(28 * W / 32, 9 * H / 32, "GETTPI", 6);

		Text(skp, 32 * W / 64, 30 * H / 32, GC->rtcc->PZRPDT.ErrorMessage);

		for (int i = 0;i < GC->rtcc->PZRPDT.plans;i++)
		{
			skpTextFmt(skp, 2 * W / 32, (11 + 3 * i) * H / 32, "%d", GC->rtcc->PZRPDT.data[i].ID);
			skpTextFmt(skp, 4 * W / 32, (11 + 3 * i) * H / 32, "%d", GC->rtcc->PZRPDT.data[i].M);

			skpTextFmt(skp, 27 * W / 64, (11 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZRPDT.data[i].NC1);
			skpTextFmt(skp, 32 * W / 64, (11 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZRPDT.data[i].NH);
			if (GC->rtcc->PZRPDT.data[i].NCC >= 0.0)
			{
				skpTextFmt(skp, 37 * W / 64, (11 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZRPDT.data[i].NCC);
			}
			skpTextFmt(skp, 42 * W / 64, (11 + 3 * i) * H / 32, "%.1lf", GC->rtcc->PZRPDT.data[i].NSR);
			if (GC->rtcc->PZRPDT.data[i].NPC >= 0.0)
			{
				skpTextFmt(skp, 47 * W / 64, (11 + 3 * i) * H / 32, "%.2lf", GC->rtcc->PZRPDT.data[i].NPC);
			}
			GET_Display3(Buffer, GC->rtcc->PZRPDT.data[i].GETTPI);
			skp->Text(28 * W / 32, (11 + 3 * i) * H / 32, Buffer, strlen(Buffer));
		}
	}
	else if (screen == 124)
	{
		skp->Text(4 * W / 8, 2 * H / 32, "PERIGEE ADJUST INPUTS (K28)", 27);

		if (GC->MissionPlanningActive)
		{
			if (GC->rtcc->med_k28.VEH == RTCC_MPT_CSM)
			{
				sprintf(Buffer, "CSM");
			}
				else
			{
				sprintf(Buffer, "LEM");
			}
			skp->Text(1 * W / 16, 2 * H / 14, Buffer, strlen(Buffer));

			GET_Display(Buffer, GC->rtcc->med_k28.VectorTime, false);
			skp->Text(1 * W / 16, 4 * H / 14, Buffer, strlen(Buffer));
		}

		GET_Display(Buffer, GC->rtcc->med_k28.ThresholdTime, false);
		skp->Text(1 * W / 16, 6 * H / 14, Buffer, strlen(Buffer));

		skpTextFmt(skp, 1 * W / 16, 8 * H / 14, "%.0lf s", GC->rtcc->med_k28.TimeIncrement);
		skpTextFmt(skp, 1 * W / 16, 10 * H / 14, "%.0lf NM", GC->rtcc->med_k28.H_P);

		ThrusterName(Buffer, GC->rtcc->med_k28.Thruster);
		skp->Text(1 * W / 16, 12 * H / 14, Buffer, strlen(Buffer));

		if (GC->rtcc->med_k28.Thruster == RTCC_ENGINETYPE_LMDPS)
		{
			skpTextFmt(skp, 10 * W / 16, 4 * H / 14, "%.3lf", GC->rtcc->med_k28.DPSScaleFactor);
		}
	}
	else if (screen == 125)
	{
		skp->Text(1 * W / 8, 2 * H / 32, "PERIGEE ADJUST DISPLAY", 27);

		skp->Text(5 * W / 32, 5 * H / 32, "HP DES", 6);
		skp->Text(4 * W / 32, 4 * H / 16, "TIG", 3);
		skp->Text(11 * W / 32, 4 * H / 16, "DV", 2);
		skp->Text(15 * W / 32, 4 * H / 16, "BT", 2);
		skp->Text(19 * W / 32, 4 * H / 16, "TAA", 3);
		skp->Text(24 * W / 32, 4 * H / 16, "H", 1);
		skp->Text(27 * W / 32, 4 * H / 16, "HA", 2);

		if (GC->rtcc->MSK0050Buffer.size() == 0) return true;

		int j = 0;

		Text(skp, 10 * W / 32, 5 * H / 32, GC->rtcc->MSK0050Buffer[j]);
		j++;

		for (unsigned i = 0;i < 6;i++)
		{
			Text(skp, 2 * W / 32, (5 + i) * H / 16, GC->rtcc->MSK0050Buffer[j]);
			j++;

			Text(skp, 11 * W / 32, (5 + i) * H / 16, GC->rtcc->MSK0050Buffer[j]);
			j++;

			Text(skp, 15 * W / 32, (5 + i) * H / 16, GC->rtcc->MSK0050Buffer[j]);
			j++;

			Text(skp, 19 * W / 32, (5 + i) * H / 16, GC->rtcc->MSK0050Buffer[j]);
			j++;

			Text(skp, 23 * W / 32, (5 + i) * H / 16, GC->rtcc->MSK0050Buffer[j]);
			j++;

			Text(skp, 27 * W / 32, (5 + i) * H / 16, GC->rtcc->MSK0050Buffer[j]);
			j++;
		}
	}
	return true;
}
