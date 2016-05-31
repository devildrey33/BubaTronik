#include "StdAfx.h"
#include "ControlMarca.h"
#include <DWLListaIconos.h>
#include "resource.h"
#include <DWLGraficos.h>
#include "BubaTronikApp.h"

ControlMarca::ControlMarca(void) {
}

ControlMarca::~ControlMarca(void) {
}

void ControlMarca::MarcaEx_Evento_PintarMarca(HDC hDC, RECT *Espacio, const int nEstado) {
//	if (nEstado != 2)	DWL::SO::DWLListaIconos::PintarIcono(hDC, 4, -1, 16, 16, IDI_MARCA_MENU);
//	else				DWL::SO::DWLListaIconos::PintarIcono(hDC, 5, 0, 16, 16, IDI_MARCA_MENU);
	DWL::SO::DWLListaIconos::PintarIcono(hDC, 4, -1, 16, 16, IDI_MARCA_MENU);
}




ControlMarcaMsg::ControlMarcaMsg(void) {
}

ControlMarcaMsg::~ControlMarcaMsg(void) {
}

void ControlMarcaMsg::MarcaEx_Evento_PintarFondo(HDC hDC, RECT *Espacio) {
	RECT R;
	GetClientRect(GetParent(_hWnd), &R);
    WINDOWPLACEMENT WP;
    WP.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(_hWnd, &WP);
	int y = WP.rcNormalPosition.top;
	DWL::GDI::PintarGradient(hDC, 0, 0 - y, Espacio->right, R.bottom - y, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior);
}