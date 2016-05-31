#include "StdAfx.h"
#include "PlayerMenuEx.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);

PlayerMenuEx::PlayerMenuEx(void) {
}

PlayerMenuEx::~PlayerMenuEx(void) {
}

void PlayerMenuEx::Mostrar(HWND hWndParent) {
	bool DesactivarBlend = false;
	if (Sistema.App.Media.MedioActual != NULL) {
		if (Sistema.App.Media.MedioActual->Medio != NULL) {
			if (Sistema.App.Media.MedioActual->Medio->TipoMedio == Tipo_Medio_Video) {
				if (Sistema.App.Media.ComprobarEstado() == EnPlay) DesactivarBlend = true;
			}
		}
	}
	MostrarMenuEx(hWndParent, false, false);
}