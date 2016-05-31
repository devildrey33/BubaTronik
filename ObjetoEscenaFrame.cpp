#include "StdAfx.h"
#include "ObjetoEscenaFrame.h"
#include "DimensionesFijas.h"
#include "BubaTronikApp.h"
#include "ObjetoEscena.h"
#include <DWLListaIconos.h>


ObjetoEscenaFrame::~ObjetoEscenaFrame(void) {
	for (unsigned int z = 0; z < Objetos.size(); z++) {
		delete Objetos[z]; 
	}
}

ObjetoEscenaIcono *ObjetoEscenaFrame::AgregarIcono(const int IDImagen, const UINT cAncho, const UINT cAlto, const UINT PosX, const UINT PosY, const UINT IDComando) {
	ObjetoEscenaIcono *nIcono = new ObjetoEscenaIcono(IDImagen, PosX, PosY, IDComando);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Icono, (LPVOID)nIcono, cAncho, cAlto, Alineacion_Centrado);
	Objetos.push_back(nObjeto);
	return nIcono;
}


void ObjetoEscenaFrame::AgregarTexto13(const TCHAR *Texto, const AlineacionObjetoEscena nAlineacion, const UINT MaxAncho, ...) {
	va_list Marker;
	va_start(Marker, MaxAncho);
	ObjetoEscenaTexto *Str = new ObjetoEscenaTexto(Texto, Tahoma13, Marker);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Texto, Str, MaxAncho, 0, nAlineacion);
	Objetos.push_back(nObjeto);
}

void ObjetoEscenaFrame::AgregarTexto15b(const TCHAR *Texto, const AlineacionObjetoEscena nAlineacion, const UINT MaxAncho, ...) {
	va_list Marker;
	va_start(Marker, MaxAncho);
	ObjetoEscenaTexto *Str = new ObjetoEscenaTexto(Texto, Tahoma15b, Marker);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Texto, Str, MaxAncho, 0, nAlineacion);
	Objetos.push_back(nObjeto);
}



ObjetoEscenaFrame *ObjetoEscenaFrame::AgregarFrameH(const AlineacionObjetoEscena nAlineacion) {
	ObjetoEscenaFrame *FrameH = new ObjetoEscenaFrame(nAlineacion);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_FrameH, (LPVOID)FrameH, 0, 0, nAlineacion);
	Objetos.push_back(nObjeto);
	return FrameH;
}

ObjetoEscenaFrame *ObjetoEscenaFrame::AgregarFrameV(const AlineacionObjetoEscena nAlineacion) {
	ObjetoEscenaFrame *FrameV = new ObjetoEscenaFrame(nAlineacion);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_FrameV, (LPVOID)FrameV, 0, 0, nAlineacion);
	Objetos.push_back(nObjeto);
	return FrameV;
}

DWL::ControlesEx::DWLBotonEx *ObjetoEscenaFrame::AgregarBoton(const TCHAR *Texto, const int cAncho, const int cAlto, const int nID, const bool nActivado, const AlineacionObjetoEscena nAlineacion) {
	DWL::ControlesEx::DWLBotonEx *nBoton = new DWL::ControlesEx::DWLBotonEx;
	nBoton->CrearBotonEx(WS_CHILD, Sistema.App.PlayerEx.CP.hWnd(), Texto, 0, 0, cAncho, cAlto, nID);
	nBoton->Activado(nActivado);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Boton, nBoton, cAncho, cAlto, nAlineacion, nBoton->hWnd());
	Objetos.push_back(nObjeto);
	return nBoton;
}


ControlMarca *ObjetoEscenaFrame::AgregarMarca(const TCHAR *Texto, const int nID, const bool nMarcado, const AlineacionObjetoEscena nAlineacion) {
	ControlMarca *nMarca = new ControlMarca;
	nMarca->CrearMarcaEx(WS_CHILD, Sistema.App.PlayerEx.CP.hWnd(), Texto, 0, 0, nID, nMarcado);
    RECT RC;
    GetClientRect(nMarca->hWnd(), &RC);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Marca, nMarca, RC.right, RC.bottom, nAlineacion ,nMarca->hWnd());
	Objetos.push_back(nObjeto);
	return nMarca;
}


DWL::ControlesEx::DWLListaEx *ObjetoEscenaFrame::AgregarLista(const int cAlto, const int nID, const AlineacionObjetoEscena nAlineacion){ 
	DWL::ControlesEx::DWLListaEx *List = new DWL::ControlesEx::DWLListaEx;
	List->CrearListaEx(WS_CHILD, Sistema.App.PlayerEx.CP.hWnd(), 3, 0, 0, cAlto, nID, true);
    List->ListaEx_Estilos.Colores.Fondo.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro);
    List->ListaEx_Estilos.Colores.ColoresItem.FondoNormal.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro);
    List->BarraScrollEx_Colores.FondoNormal.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Lista, List, 0, cAlto, nAlineacion ,List->hWnd());
	Objetos.push_back(nObjeto);
	return List;
}


DWL::ControlesEx::DWLArbolEx *ObjetoEscenaFrame::AgregarArbol(const int cAlto, const int nID, const AlineacionObjetoEscena nAlineacion){ 
	DWL::ControlesEx::DWLArbolEx *Ar = new DWL::ControlesEx::DWLArbolEx;
	Ar->CrearArbolEx(WS_CHILD, Sistema.App.PlayerEx.CP.hWnd(), 3, 0, 0, cAlto, nID, true);
    Ar->ArbolEx_Estilos.Colores.Fondo.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Arbol, Ar, 0, cAlto, nAlineacion ,Ar->hWnd());
	Objetos.push_back(nObjeto);
	return Ar;
}

DWL::ControlesEx::DWLBarraEx *ObjetoEscenaFrame::AgregarBarraProgreso(const int nMinimo, const int nMaximo, const int nValor, const int nID, const AlineacionObjetoEscena nAlineacion) {
	DWL::ControlesEx::DWLBarraEx *Barra = new DWL::ControlesEx::DWLBarraEx;
	Barra->CrearBarraEx(WS_CHILD, Sistema.App.PlayerEx.CP.hWnd(), 3, 0, 0, 6, nID, DWL_BARRAEX_PROGRESO_HORIZONTAL, nMinimo, nMaximo);
	Barra->Valor(nValor);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Barra, Barra, 0, 6, nAlineacion, Barra->hWnd());
	Objetos.push_back(nObjeto);
	return Barra;
}

DWL::ControlesEx::DWLEdicionDesplegableEx *ObjetoEscenaFrame::AgregarEdicionDesplegable(const TCHAR *Texto, const int nID, const int nIcono, const bool BuscarDirectorio, const UINT AltoLista, const AlineacionObjetoEscena nAlineacion) {
	DWL::ControlesEx::DWLEdicionDesplegableEx *Combo = new DWL::ControlesEx::DWLEdicionDesplegableEx;
	DWL_Enum_EdicionDesplegableEx_Tipo TipoCombo;
	if (BuscarDirectorio == true) TipoCombo = DWL_Enum_EdicionDesplegableEx_Tipo_BuscarDirectorio;
	else						  TipoCombo = DWL_Enum_EdicionDesplegableEx_Tipo_ListaDesplegable;
	Combo->CrearEdicionDesplegableEx(WS_CHILD, Sistema.App.PlayerEx.CP.hWnd(), Texto, 3, 0, 340, 18, AltoLista, nID, false, nIcono, TipoCombo);
	Combo->EdicionEx_Estilos.Colores.Fondo.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Claro);
	Combo->EdicionEx_Estilos.Colores.FondoDesactivado.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);

	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_ComboBox, Combo, 0, 18, nAlineacion, Combo->hWnd());
	Objetos.push_back(nObjeto);
	return Combo;	
}

ControlTeclaRapida *ObjetoEscenaFrame::AgregarTeclaRapida(const TCHAR *nTexto, const int nID, Tecla *nTecla, const AlineacionObjetoEscena nAlineacion) {
	RECT R;
	GetClientRect(Sistema.App.PlayerEx.CP.hWnd(), &R);	
	ControlTeclaRapida *CTecla = new ControlTeclaRapida;
	CTecla->Crear(Sistema.App.PlayerEx.CP.hWnd(), nTexto, 0, 0, R.right - 24, 18, nID, nTecla);
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_TeclaRapida, CTecla, R.right - 24, 18, nAlineacion ,CTecla->hWnd());
	Objetos.push_back(nObjeto);
//	YO += 18;
	return CTecla;
}



void ObjetoEscenaFrame::AgregarSeparacion(const int nPixels) {
	ObjetoEscena *nObjeto = new ObjetoEscena(ObjetoEscena_Separacion, (LPVOID)nPixels, nPixels, nPixels, Alineacion_Derecha);
	Objetos.push_back(nObjeto);
}


DWL::ControlesEx::DWLBotonEx *ObjetoEscenaFrame::BuscarBoton(const int bID) {
	DWL::ControlesEx::DWLBotonEx *Ret = NULL;
	for (size_t i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Tipo() == ObjetoEscena_FrameH || Objetos[i]->Tipo() == ObjetoEscena_FrameV) {
			Ret = Objetos[i]->FrameH()->BuscarBoton(bID);
			if (Ret != NULL) return Ret;
		}
		if (Objetos[i]->Tipo() == ObjetoEscena_Boton && Objetos[i]->ID() == bID) return Objetos[i]->Boton();
	}
	return NULL;
};


ControlMarca *ObjetoEscenaFrame::BuscarMarca(const int bID) {
	ControlMarca *Ret = NULL;
	for (size_t i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Tipo() == ObjetoEscena_FrameH || Objetos[i]->Tipo() == ObjetoEscena_FrameV) {
			Ret = Objetos[i]->FrameH()->BuscarMarca(bID);
			if (Ret != NULL) return Ret;
		}
		if (Objetos[i]->Tipo() == ObjetoEscena_Marca && Objetos[i]->ID() == bID) return Objetos[i]->Marca();
	}
	return NULL;
};


DWL::ControlesEx::DWLListaEx *ObjetoEscenaFrame::BuscarLista(const int bID) {
	DWL::ControlesEx::DWLListaEx *Ret = NULL;
	for (size_t i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Tipo() == ObjetoEscena_FrameH || Objetos[i]->Tipo() == ObjetoEscena_FrameV) {
			Ret = Objetos[i]->FrameH()->BuscarLista(bID);
			if (Ret != NULL) return Ret;
		}
		if (Objetos[i]->Tipo() == ObjetoEscena_Lista && Objetos[i]->ID() == bID) return Objetos[i]->Lista();
	}
	return NULL;
};


DWL::ControlesEx::DWLArbolEx *ObjetoEscenaFrame::BuscarArbol(const int bID) {
	DWL::ControlesEx::DWLArbolEx *Ret = NULL;
	for (size_t i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Tipo() == ObjetoEscena_FrameH || Objetos[i]->Tipo() == ObjetoEscena_FrameV) {
			Ret = Objetos[i]->FrameH()->BuscarArbol(bID);
			if (Ret != NULL) return Ret;
		}
		if (Objetos[i]->Tipo() == ObjetoEscena_Arbol && Objetos[i]->ID() == bID) return Objetos[i]->Arbol();
	}
	return NULL;
};


DWL::ControlesEx::DWLBarraEx *ObjetoEscenaFrame::BuscarBarraProgreso(const int bID) {
	DWL::ControlesEx::DWLBarraEx *Ret = NULL;
	for (size_t i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Tipo() == ObjetoEscena_FrameH || Objetos[i]->Tipo() == ObjetoEscena_FrameV) {
			Ret = Objetos[i]->FrameH()->BuscarBarraProgreso(bID);
			if (Ret != NULL) return Ret;
		}
		if (Objetos[i]->Tipo() == ObjetoEscena_Barra && Objetos[i]->ID() == bID) return Objetos[i]->Barra();
	}
	return NULL;
};



DWL::ControlesEx::DWLEdicionDesplegableEx *ObjetoEscenaFrame::BuscarEdicionDesplegable(const int bID) {
	DWL::ControlesEx::DWLEdicionDesplegableEx *Ret = NULL;
	for (size_t i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Tipo() == ObjetoEscena_FrameH || Objetos[i]->Tipo() == ObjetoEscena_FrameV) {
			Ret = Objetos[i]->FrameH()->BuscarEdicionDesplegable(bID);
			if (Ret != NULL) return Ret;
		}
		if (Objetos[i]->Tipo() == ObjetoEscena_ComboBox && Objetos[i]->ID() == bID) return Objetos[i]->ComboBox();
	}
	return NULL;
};




const UINT ObjetoEscenaFrame::PintarV(HDC hDCDest, const UINT cX, const UINT cY, const UINT cAncho, const UINT cAlto) {

	// Calculamos los tamaños para las filas / columnas
	unsigned int z;
/*	for (z = 0; z < Objetos.size(); z++) {
		if (Objetos[z]->Tipo() == ObjetoEscena_Marco) Objetos[z]->Alto(Objetos[z]->Marco()->CalcularFila());
	}*/

	UINT YActual = cY;

	RECT Espacio = { 0, 0, 0, 0 };
	UINT AnchoFinal = cAncho - (MARGEN_ESCENA * 2);
	UINT AnchoControl = 0;
	UINT ControlPosY = 0;
	ObjetoEscenaIconoPulsable *Iko;
	for (z = 0; z < Objetos.size(); z++) {

		
		// Si es un objeto-control sin ancho definido obtenemos el ancho
		if (Objetos[z]->Ancho() == 0 && Objetos[z]->hWnd() != 0) {
			GetClientRect(Objetos[z]->hWnd(), &Espacio);
			AnchoControl = Espacio.right;
		}
		else {
			AnchoControl = Objetos[z]->Ancho();
		}
		if (AnchoControl == 0) AnchoControl = AnchoFinal;

		// Calculamos la posición segun la alineacion
		switch (Objetos[z]->Alineacion()) {
			case Alineacion_Centrado :
				Espacio.left = ((cX + MARGEN_ESCENA) + ((AnchoFinal - AnchoControl) / 2));
				Espacio.right = Espacio.left + AnchoControl;
				break;
			case Alineacion_Derecha :
				Espacio.left = ((cX + MARGEN_ESCENA) + ((AnchoFinal - AnchoControl) - MARGEN_ESCENA));
				Espacio.right = Espacio.left + AnchoControl;
				break;
			case Alineacion_Izquierda :
				Espacio.left = (cX + MARGEN_ESCENA);
				Espacio.right = Espacio.left + AnchoControl;
				break;
			case Alineacion_Expandible :
				Espacio.left = (cX + MARGEN_ESCENA);
				Espacio.right = cAncho - MARGEN_ESCENA;
				break;
		}
		Espacio.top = YActual;
		Espacio.bottom = YActual + Objetos[z]->Alto();

		ControlPosY = YActual - Sistema.App.PlayerEx.CP.BarraScrollEx_ValorV();

		// Movemos y pintamos el control
		switch (Objetos[z]->Tipo()) {
			case ObjetoEscena_Texto :
				_PintarTexto(hDCDest, Objetos[z], Espacio.left, Espacio.top, AnchoFinal);
				break;
			case ObjetoEscena_Boton	:
				Objetos[z]->Boton()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->Boton()->Visible(true);
				Objetos[z]->Boton()->BotonEx_Pintar(hDCDest, &Espacio, false, true);
				break;
			case ObjetoEscena_Marca :
				Objetos[z]->Marca()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->Marca()->Visible(true);
				Objetos[z]->Marca()->MarcaEx_Pintar(hDCDest, &Espacio, false);
				break;
			case ObjetoEscena_Barra	:
				Objetos[z]->Barra()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->Barra()->Visible(true);
				Objetos[z]->Barra()->BarraEx_Pintar(hDCDest, &Espacio, false, true);
				break;
			case ObjetoEscena_Lista	:
				Objetos[z]->Lista()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->Lista()->Visible(true);
				Objetos[z]->Lista()->ListaEx_Pintar(hDCDest, &Espacio, false, true);
				break;
			case ObjetoEscena_Arbol	:
				Objetos[z]->Arbol()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->Arbol()->Visible(true);
				Objetos[z]->Arbol()->ArbolEx_Pintar(hDCDest, &Espacio, false, true);
				break;
			case ObjetoEscena_ComboBox : 
				Objetos[z]->ComboBox()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->ComboBox()->Visible(true);
				Objetos[z]->ComboBox()->EdicionDesplegableEx_Pintar(hDCDest, &Espacio, false, true);
				break;
			case ObjetoEscena_Icono :
				// Agregamos el icono a la lista de iconos pulsables
				if (Objetos[z]->Icono()->IDComando != 0) {
					Iko = new ObjetoEscenaIconoPulsable(Objetos[z]->Icono()->IDIcono, Objetos[z]->Icono()->IDComando, Objetos[z]->Icono()->X + Espacio.left, Objetos[z]->Icono()->Y + Espacio.top, Objetos[z]->Ancho(), Objetos[z]->Alto());
					Escena::IconosPulsables.push_back(Iko);
				}
				DWL::SO::DWLListaIconos::PintarIcono(hDCDest, Objetos[z]->Icono()->X + Espacio.left, Objetos[z]->Icono()->Y + Espacio.top, Objetos[z]->Ancho(), Objetos[z]->Alto(), Objetos[z]->Icono()->IDIcono);
				break;
			case ObjetoEscena_FrameH :
				Objetos[z]->FrameH()->PintarH(hDCDest, Espacio.left, Espacio.top, Espacio.right - Espacio.left, Espacio.bottom - Espacio.top);
				break;
			case ObjetoEscena_TeclaRapida :
				Objetos[z]->TeclaRapida()->Mover(Espacio.left, ControlPosY, Espacio.right - Espacio.left, Objetos[z]->Alto(), false);
				Objetos[z]->TeclaRapida()->Visible(true);
				Objetos[z]->TeclaRapida()->Pintar(hDCDest, &Espacio);
				break;

		}
		YActual = YActual + SEPARACION_ESCENA + Objetos[z]->Alto();
	}	

	return YActual + MARGEN_ESCENA;
}



const UINT ObjetoEscenaFrame::PintarH(HDC hDCDest, const UINT cX, const UINT cY, const UINT cAncho, const UINT cAlto) {
	size_t i   = 0;
	UINT   TAE = 0; // Total alineaciones expandibles
	UINT   AO  = (Objetos.size() - 1) * MARGEN_ESCENA; // Ancho ocupado
	UINT   AE  = 0; // Ancho expandible
	UINT   XActual = 0;
	ObjetoEscenaIconoPulsable *Iko;
	// Determinamos si esta fila sera expandible, o usara la alineacion propia
	for (i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Alineacion() == Alineacion_Expandible)	TAE ++;
		else {
			AO += Objetos[i]->Ancho();
			if (_Alto < Objetos[i]->Alto()) _Alto = Objetos[i]->Alto();
		}
	}

	if (TAE == 0) { // utilizamos la alineacion de la fila / marco
		switch (Alineacion()) {
			case Alineacion_Centrado :
				XActual = cX + ((cAncho - AO) / 2);
				break;
			case Alineacion_Derecha :
				XActual = cX + (cAncho - AO);
				break;
			case Alineacion_Izquierda :
				XActual = cX;
				break;
		}
	}
	else { // uno o mas controles tienen alineacion expandible
		AE = (cAncho - AO) / TAE;
		XActual = cX;
	}
	RECT RC;
	RC.top = cY;
	UINT ControlPosY = 0;
	for (i = 0; i < Objetos.size(); i++) {
		// Calculo el rectangulo que ocupa el control
		RC.left = XActual;
		if (Objetos[i]->Alineacion() == Alineacion_Expandible)  XActual += AE;
		else													XActual += Objetos[i]->Ancho();
		RC.bottom = RC.top + Objetos[i]->Alto();
		RC.right = XActual;

		// Corrección para el ultimo control expandible para que termine justo donde se le ha dicho
		// Esta corrección se hace porque si hay 2 o mas controles expandibles al hacer la division del espacio pueden
		//  haber decimales que no se tengan en cuenta, y a fin de cuentas estos decimales acaban siendo 1 pixel mas o menos 
		//  en la escena final, y por ello aveces el ultimo control no cuadra bien con el margen de la derecha.
		if (i == Objetos.size() -1 && TAE > 0) RC.right = cAncho + cX;
//		if (RC.right > static_cast<long>(cAncho)) RC.right = cAncho;

		ControlPosY = cY - Sistema.App.PlayerEx.CP.BarraScrollEx_ValorV();

		// Pinto el control segun su tipo
		switch (Objetos[i]->Tipo()) {
			case ObjetoEscena_Icono :
				// Agregamos el icono a la lista de iconos pulsables
				if (Objetos[i]->Icono()->IDComando != 0) {
					Iko = new ObjetoEscenaIconoPulsable(Objetos[i]->Icono()->IDIcono, Objetos[i]->Icono()->IDComando, Objetos[i]->Icono()->X + RC.left, Objetos[i]->Icono()->Y + RC.top, RC.right - RC.left,  RC.bottom - RC.top);
					Escena::IconosPulsables.push_back(Iko);
				}
				DWL::SO::DWLListaIconos::PintarIcono(hDCDest, Objetos[i]->Icono()->X + RC.left, Objetos[i]->Icono()->Y + RC.top, RC.right - RC.left,  RC.bottom - RC.top, Objetos[i]->Icono()->IDIcono);
				break;
			case ObjetoEscena_Texto :
//				_PintarTexto(hDCDest, Objetos[z], Espacio.left, Espacio.top, AnchoFinal);
				_PintarTexto(hDCDest, Objetos[i], RC.left, RC.top, (RC.right - RC.left) - (MARGEN_ESCENA * 2));
				break;
			case ObjetoEscena_Boton :
				Objetos[i]->Boton()->Mover(RC.left, ControlPosY, RC.right - RC.left, RC.bottom - RC.top, false);
				Objetos[i]->Boton()->Visible(true);
				Objetos[i]->Boton()->BotonEx_Pintar(hDCDest, &RC, false, true);
				break;
			case ObjetoEscena_ComboBox :
				Objetos[i]->ComboBox()->Mover(RC.left, ControlPosY, RC.right - RC.left, RC.bottom - RC.top, false);
				Objetos[i]->ComboBox()->Visible(true);
				Objetos[i]->ComboBox()->EdicionDesplegableEx_Pintar(hDCDest, &RC, false, true);
				break;
			case ObjetoEscena_FrameV :
				Objetos[i]->FrameV()->PintarV(hDCDest, RC.left, RC.top, RC.right - RC.left, RC.bottom - RC.top);
				break;
			case ObjetoEscena_FrameH :
				Objetos[i]->FrameH()->PintarV(hDCDest, RC.left, RC.top, RC.right - RC.left, RC.bottom - RC.top);
				break;
			case ObjetoEscena_Marca :
				Objetos[i]->Marca()->Mover(RC.left, ControlPosY, RC.right - RC.left, Objetos[i]->Alto(), false);
				Objetos[i]->Marca()->Visible(true);
				Objetos[i]->Marca()->MarcaEx_Pintar(hDCDest, &RC, false);
				break;
		}
		XActual += MARGEN_ESCENA;
	}

	return 0;
}


const UINT ObjetoEscenaFrame::CalcularAltoV(const UINT AnchoDisponible) {
	_Alto = 0;
	UINT nX = 0;
	for (size_t i = 0; i < Objetos.size(); i++) {
		switch (Objetos[i]->Tipo()) {
			case ObjetoEscena_Texto :
				Objetos[i]->Alto(_ObtenerAltoTexto(Objetos[i], AnchoDisponible - (MARGEN_ESCENA * 2)));
				_Alto += Objetos[i]->Alto();
				break;
			case ObjetoEscena_FrameH :
				Objetos[i]->Alto(Objetos[i]->FrameH()->CalcularAltoH(AnchoDisponible - (MARGEN_ESCENA * 2)));
				_Alto += Objetos[i]->Alto();
				break;
			default :
				_Alto += (SEPARACION_ESCENA + Objetos[i]->Alto());
				break;
		}
//		nX += Objetos[i]->Ancho();

		// Comprobacion para determinar si hay objetos con Altura 0
//			if (Objetos[i]->Alto() == 0) 
//				i = i;
	}
	return _Alto;
}


// En un frame horizontal puede haber objetos mas altos que otros, y esos objetos no suelen tener un tamaño fijo
// El objetivo es determinar el objeto mas alto, y asignar esa altura al frame.
const UINT ObjetoEscenaFrame::CalcularAltoH(const UINT AnchoDisponible) {
	UINT   TAE     = 0; // Total alineaciones expandibles
	UINT   AO      = MARGEN_ESCENA * 2; // Ancho ocupado
	UINT   TmpAlto = 0; // Altura temporal
	size_t i       = 0;
	// Primera pasada para ver si hay objetos expandibles
	for (i = 0; i < Objetos.size(); i++) {
		if (Objetos[i]->Alineacion() == Alineacion_Expandible)	TAE ++;
		else													AO += Objetos[i]->Ancho();
	}
	UINT AE = 0;
	if (TAE > 0) AE = (AnchoDisponible - AO) / TAE; // Ancho para los objetos expandibles

	// Segunda pasada para obtener las alturas
//	UINT nX = 0;
	for (i = 0; i < Objetos.size(); i++) {
		switch (Objetos[i]->Tipo()) {
			case ObjetoEscena_Texto :
				Objetos[i]->Alto(_ObtenerAltoTexto(Objetos[i], AE));
				if (Objetos[i]->Alto() > _Alto) _Alto = Objetos[i]->Alto();
				break;
			case ObjetoEscena_FrameV :
				Objetos[i]->Alto(Objetos[i]->FrameV()->CalcularAltoV(AE)); // No le pasamos bien el ancho disponible, hay que restarle la X del frame
				if (Objetos[i]->Alto() > _Alto) _Alto = Objetos[i]->Alto();
				break;
			default :
				if (Objetos[i]->Alto() > _Alto) _Alto = Objetos[i]->Alto();
				break;
		}
//		nX += Objetos[i]->Ancho();

		// Comprobacion para determinar si hay objetos con Altura 0
//			if (Objetos[i]->Alto() == 0) 
	//			i = i;
	}
	return _Alto;
}


// Esta funcion pinta una frase en el DC basandose en el ancho y fuente de este,
// Si hace falta pintara la frase en 2 o mas lineas segun se requiera por el espacio.
// Esta funcion retorna la altura en pixeles del texto pintado.
const UINT ObjetoEscenaFrame::_PintarTexto(HDC hDCDest, ObjetoEscena *Txt, const int cX, const int cY, const UINT AnchoEscena) {
	SIZE E; 
	SetBkMode(hDCDest, TRANSPARENT);
	UINT MaxAncho = cX + AnchoEscena; // 4*2 pixeles de margen por banda + 18 del scrollbar
	HFONT FuenteActual;
	switch (Txt->Texto()->Fuente) {
		case Tahoma13 :
			FuenteActual = DWL::SO::DWLEstilos::Fuentes.Tahoma13();
			break;
		case Tahoma15b :
			FuenteActual = Sistema.App.Tahoma15b();
			break;
	}
	HFONT VFONT = static_cast<HFONT>(SelectObject(hDCDest, FuenteActual));
	bool Parrafo = false;
	if (Txt->Texto()->Palabras[0]->Palabra.Tam() != 0) {
		if (Txt->Texto()->Palabras[0]->Palabra[0] == TEXT('-'))		Parrafo = true;
	}

	// Calculo el tamaño de un espacio
	GetTextExtentPoint32(hDCDest, TEXT(" "), 1, &E);
	int Espacio = E.cx;
	GetTextExtentPoint32(hDCDest, TEXT("- "), 2, &E);
	int LineaEspacio = Espacio + E.cx;
	UINT Altura  = 2;
	int AnchoTmp = Espacio + cX;
	
	ObjetoEscenaPalabraPulsable *PP = NULL;
	/*switch (Txt->Alineacion()) {
		case Alineacion_Derecha :
			AnchoTmp = MaxAncho - Txt->Ancho();
			break;
		case Alineacion_Centrado :
			AnchoTmp = (MaxAncho - Txt->Ancho()) / 2;
			break;
		case Alineacion_Izquierda :
		case Alineacion_Expandible :
			AnchoTmp = Espacio + cX;
			break;
	}*/

	for (size_t r = 0; r < Txt->Texto()->Palabras.size(); r++) {	
		// Si la linea excede del ancho (solo para expandibles)
		if (Txt->Alineacion() == Alineacion_Expandible) {
			if (AnchoTmp + Txt->Texto()->Palabras[r]->Ancho > MaxAncho) {
				Altura += E.cy;
				if (Parrafo == false)	AnchoTmp = Espacio + cX;
				else					AnchoTmp = LineaEspacio + cX;
			}
		}
		// Palabra pulsable / negrita
		if (Txt->Texto()->Palabras[r]->ID != 0) {
			if (Txt->Texto()->Palabras[r]->ID != -1) {
				PP = new ObjetoEscenaPalabraPulsable(AnchoTmp, cY + Altura, AnchoTmp + Txt->Texto()->Palabras[r]->Ancho, cY + Altura + E.cy, Txt->Texto()->Palabras[r]);
				Escena::PalabrasPulsables.push_back(PP);
				// Recta del texto pulsable
				SetTextColor(hDCDest, DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Seleccionado);
			}
			else {
				SetTextColor(hDCDest, DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Normal);
			}
			SelectObject(hDCDest, DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita());
		}
		// Palabra normal
		else {
			SetTextColor(hDCDest, DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Normal);
			SelectObject(hDCDest, FuenteActual);
		}
		TextOut(hDCDest, AnchoTmp, cY + Altura, Txt->Texto()->Palabras[r]->Palabra(), Txt->Texto()->Palabras[r]->Palabra.Tam());
		AnchoTmp += Txt->Texto()->Palabras[r]->Ancho + Espacio;	
	}

	Altura += E.cy;

	SelectObject(hDCDest, VFONT);
	return Altura;
}



// Mide el texto segun el ancho del ControlPresentacion 
const UINT ObjetoEscenaFrame::_ObtenerAltoTexto(ObjetoEscena *Txt, const UINT AnchoDisponible) {
	SIZE E; 

	UINT MaxAncho = AnchoDisponible + MARGEN_ESCENA;

	HDC hDC = GetDC(Sistema.App.PlayerEx.CP.hWnd());
	HFONT FuenteActual;
	switch (Txt->Texto()->Fuente) {
		default :
		case Tahoma13 :
			FuenteActual = DWL::SO::DWLEstilos::Fuentes.Tahoma13();
			break;
		case Tahoma15b :
			FuenteActual = Sistema.App.Tahoma15b();
			break;
	}
	HFONT VFONT = static_cast<HFONT>(SelectObject(hDC, FuenteActual));

	bool Parrafo = false;
	if (Txt->Texto()->Palabras[0]->Palabra.Tam() != 0) {
		if (Txt->Texto()->Palabras[0]->Palabra[0] == TEXT('-'))	Parrafo = true;
	}

	// Calculo el tamaño de un espacio
	GetTextExtentPoint32(hDC, TEXT(" "), 1, &E);
	UINT Espacio = E.cx;
	GetTextExtentPoint32(hDC, TEXT("- "), 2, &E);
	UINT LineaEspacio = Espacio + E.cx;
	UINT Altura   = 2;
	UINT AnchoTmp = Espacio;

	// Si no es expandible, el texto solo tiene una linea
	if (Txt->Alineacion() != Alineacion_Expandible) {
		AnchoTmp = 0;
		if (Txt->Ancho() == 0) {  // Si el ancho no esta asignado, lo calculamos
			for (size_t i = 0; i < Txt->Texto()->Palabras.size(); i++) AnchoTmp += Txt->Texto()->Palabras[i]->Ancho;
			Txt->Ancho(AnchoTmp + ((Txt->Texto()->Palabras.size() - 1) * Espacio));
		}
		ReleaseDC(Sistema.App.PlayerEx.CP.hWnd(), hDC);
		// retorno la altura de una linea
		return E.cy + 2;
	}

	// Es expandible, calculamos cuantas lineas se necesitan
	for (unsigned int r = 0; r < Txt->Texto()->Palabras.size(); r++) {	
		if (Txt->Texto()->Palabras[r]->ID != 0) SelectObject(hDC, DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita());
		else									SelectObject(hDC, FuenteActual);

		// Si la linea excede del ancho
		if (AnchoTmp + Txt->Texto()->Palabras[r]->Ancho > MaxAncho) {
			Altura += E.cy;
			if (Parrafo == false)	AnchoTmp = Espacio;
			else					AnchoTmp = LineaEspacio;
		}

		AnchoTmp += Txt->Texto()->Palabras[r]->Ancho + Espacio;	
	}

	Altura += E.cy;

//	SelectObject(hDC, VFONT);
	ReleaseDC(Sistema.App.PlayerEx.CP.hWnd(), hDC);
	return Altura;
}
