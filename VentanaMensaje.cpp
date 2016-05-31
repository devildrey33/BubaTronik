#include "StdAfx.h"
#include "VentanaMensaje.h"
#include "Resource.h"
#include "BubaTronikApp.h"


VentanaMensaje_Base::VentanaMensaje_Base(void) {
	_TeclaPresionada = false;
}

VentanaMensaje_Base::~VentanaMensaje_Base(void) {
}

// Función que crea la ventana para el mensaje
SIZE VentanaMensaje_Base::CrearVentanaMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje, const UINT AlturaExtra) {
    IDMensaje = nIDMensaje;
    Mensaje = nTexto;
    Titulo = nTitulo;
    SIZE TamMensaje = CalcularTamTexto(nTexto);
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		CrearVentana(NULL, TEXT("BubaTronik_Mensaje"), WS_OVERLAPPED | WS_SYSMENU, nTitulo, DWL_CENTRADO, DWL_CENTRADO, TamMensaje.cx + 25, TamMensaje.cy + AlturaExtra + 20 + (Sistema.Medidas.Alto_BarraTitulo() + (Sistema.Medidas.Alto_Borde() * 2)), NULL, NULL);
	}
	else {
		DWL::GDI::DWLRegion	Region(0, 0, TamMensaje.cx + 20, TamMensaje.cy + 50 + AlturaExtra, 11, 11);
		CrearVentana(NULL, TEXT("BubaTronik_Mensaje"), WS_POPUP, nTitulo, DWL_CENTRADO, DWL_CENTRADO, TamMensaje.cx + 20, TamMensaje.cy + 53 + AlturaExtra, NULL, NULL);
		Boton_Cerrar.Crear(_hWnd, (TamMensaje.cx + 20) - CM_ANCHO_SC, 0, ID_BOTON_CERRAR, true);
        Boton_Cerrar.Visible(true);
		Region.AsignarRegionVentana(_hWnd, FALSE);
	}
    return TamMensaje;
}

// Función que calcula el ancho y alto necesarios para un texto con la fuente por defecto del reproductor
const SIZE VentanaMensaje_Base::CalcularTamTexto(const TCHAR *Texto) {
	std::vector<DWL::DWLString *>	Lineas;
	DWL::DWLString			       *Tmp			= new DWL::DWLString;
	DWL::GDI::DWLhDC				hDC(_hWnd);
	size_t							TamTexto	= DWLStrLen(Texto);
	size_t							i			= 0;
	SIZE							TamTmp		= { 0 ,0 };
	SIZE							Retorno		= { 0 ,0 };
	// Separo el texto en lineas
	for (i = 0; i < TamTexto; i++) {
		switch (Texto[i]) {
			case TEXT('\n') :
				Lineas.push_back(Tmp);
				Tmp = new DWL::DWLString;
				break;
			case 0 :
				Lineas.push_back(Tmp);
				Tmp = NULL;
				break;
			default :
				Tmp->AgregarCaracter(Texto[i]);
				break;
		}
	}
	if (Tmp != NULL) {
		Lineas.push_back(Tmp);
		Tmp = NULL;
	}
	// Obtengo el tamaño de la linea mas larga
	hDC.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
	for (i = 0; i < Lineas.size(); i++) {
		if (Lineas[i]->Tam() > 0 ) GetTextExtentPoint32(hDC(), Lineas[i]->Texto(), Lineas[i]->Tam(), &TamTmp);
		if (TamTmp.cx > Retorno.cx) Retorno.cx = TamTmp.cx;
	}
	Retorno.cy = TamTmp.cy * Lineas.size();

	// Libero la memoria
	for (i = 0; i < Lineas.size(); i++) delete Lineas[i];

	// Retorno el tamaño
	return Retorno;
}

// Función para pintar el degradado de fondo del mensaje
void VentanaMensaje_Base::PintarDegradadoFondo(HDC hDC, RECT *Espacio) {
	TRIVERTEX     GCVertex[2];
	GRADIENT_RECT tGRect;
    COLORREF      Col1 = DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior;
    COLORREF      Col2 = DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior;
    
	GCVertex[0].Red		= DWL_RGB_OBTENER_R(Col1);
	GCVertex[0].Green	= DWL_RGB_OBTENER_G(Col1);
	GCVertex[0].Blue	= DWL_RGB_OBTENER_B(Col1);
	GCVertex[0].x = Espacio->left;
	GCVertex[0].y = Espacio->top;
	GCVertex[1].Red		= DWL_RGB_OBTENER_R(Col2);
	GCVertex[1].Green	= DWL_RGB_OBTENER_G(Col2);
	GCVertex[1].Blue	= DWL_RGB_OBTENER_B(Col2);
	GCVertex[1].x = Espacio->right;
	GCVertex[1].y = Espacio->bottom;
	tGRect.UpperLeft = 0;  tGRect.LowerRight = 1;
	GradientFill(hDC, GCVertex, 2, &tGRect, 1, DWLGRADIENT_FILL_RECT_V);
}



// Función para pintar texto con sombra
void VentanaMensaje_Base::PintarTextoSombra(HDC hDC, const TCHAR *Texto, const UINT TamTexto, COLORREF ColTexto, COLORREF ColSombra, const RECT &Espacio) {
    RECT EspacioTexto = { Espacio.left, Espacio.top, Espacio.right, Espacio.bottom };
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, ColSombra);
    DrawText(hDC, Texto, TamTexto, &EspacioTexto, DT_LEFT);
    EspacioTexto.left ++;
    EspacioTexto.top ++;
    EspacioTexto.right ++;
    EspacioTexto.bottom ++;
    SetTextColor(hDC, ColTexto);
    DrawText(hDC, Texto, TamTexto, &EspacioTexto, DT_LEFT);
}

/*
// Si se recibe la tecla ESC es que se quiere cancelar
LRESULT VentanaMensaje_Base::Evento_BotonEx_Teclado_TeclaPresionada(const UINT TeclaVirtual, const UINT IDBotonEx) {
//	_TeclaPresionada = true;
    return 0;
}
*/
// Si se recibe la tecla ESC es que se quiere cancelar
LRESULT VentanaMensaje_Base::Evento_BotonEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDBotonEx) {
//	if (_TeclaPresionada == true) {
	//	_TeclaPresionada = false;
		if (TeclaVirtual == VK_ESCAPE) DestroyWindow(_hWnd);
//	}
    return 0;
}

// Cuando se recibe el foco en la ventana lo redirigimos al boton cancelar
LRESULT VentanaMensaje_Base::Evento_Foco_Obtenido(HWND hWndAnterior) {
	SetFocus(Boton_Cancelar.hWnd());
    return 0;
}


LRESULT VentanaMensaje_Base::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	RECT R;
	GetClientRect(_hWnd, &R);
    DWL::GDI::DWLhDC Buffer(hDC, R.right, R.bottom);
    PintarDegradadoFondo(Buffer(), &R);

	if (Sistema.App.Config.UtilizarBarraTituloWindows == false) { 
		// Pinto el borde
    	DWL::GDI::DWLBrocha		Brocha(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Claro);
		DWL::GDI::DWLRegion		Region(_hWnd);
		Buffer.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDE, VENTANAEX_BORDE);
		Brocha.AsignarColor(DWL::SO::DWLEstilos::Colores.ControlEx_Borde_Normal);
		Buffer.EnmarcarRegion(&Region, &Brocha, VENTANAEX_BORDECLARO, VENTANAEX_BORDECLARO);

		// Pinto el titulo
		Buffer.PintarIcono(5, 5, 16, 16, IDI_DAP);
		Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloSombra);
		Buffer.ModoBk(TRANSPARENT);
		Buffer.PintarTexto(Titulo(), 26, 6);
		Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloNormal);
		Buffer.PintarTexto(Titulo(), 25, 5);

		// Pinto el texto
		Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
		Buffer.ModoBk(TRANSPARENT);
        RECT RT = { 11, 23, R.right - 9, R.bottom - 23 };
		Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloSombra);
		DrawText(Buffer.hDC(), Mensaje(), Mensaje.Tam(), &RT, DT_LEFT);
        RECT RT2 = { 10, 22, R.right - 9, R.bottom - 22 };
		Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloNormal);
		DrawText(Buffer.hDC(), Mensaje(), Mensaje.Tam(), &RT2, DT_LEFT);
	}
	else {
		// Pinto el texto
		Buffer.Fuente(DWL::SO::DWLEstilos::Fuentes.Tahoma13());
		Buffer.ModoBk(TRANSPARENT);
        RECT RT3 = { 11, 6, R.right - 9, R.bottom - 21 };
		Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloSombra);
		DrawText(Buffer.hDC(), Mensaje(), Mensaje.Tam(), &RT3, DT_LEFT);
        RECT RT4 = { 10, 5, R.right - 9, R.bottom - 20 };
		Buffer.ColorTexto(DWL::SO::DWLEstilos::Colores.VentanaEx_Texto_TituloNormal);
		DrawText(Buffer.hDC(), Mensaje(), Mensaje.Tam(), &RT4, DT_LEFT);
	}

	// Pinto el DC en la ventana
	Buffer.PintarMapaBits(hDC, 0, 0, R.right, R.bottom, 0, 0, SRCCOPY);
    return 0;
}





















VentanaMensaje_ErrorCritico::VentanaMensaje_ErrorCritico(void) {
}

VentanaMensaje_ErrorCritico::VentanaMensaje_ErrorCritico(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje) {
    MostrarMensaje(nTexto, nTitulo, nIDMensaje);
}

VentanaMensaje_ErrorCritico::~VentanaMensaje_ErrorCritico(void) {
}

// Función para mostrar el mensaje Aceptar
void VentanaMensaje_ErrorCritico::MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje) {
    SIZE TamMensaje = CrearVentanaMensaje(nTexto, nTitulo, nIDMensaje);
    DWL::DWLString TextoAceptar;
	if (Sistema.App.Idioma == NULL) TextoAceptar = TEXT("Aceptar");
	else		        			TextoAceptar = IDIOMA__ACEPTAR;
    UINT TmpY = 0;
   	if (Sistema.App.Config.UtilizarBarraTituloWindows == true)  TmpY = 8;
    else                                                        TmpY = 25;

	Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, TextoAceptar(), ((TamMensaje.cx + 20) - ANCHO_BOTON_VM) / 2, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	Boton_Cancelar.AsignarFoco();
	Visible(true);
	MSG Message;
    Terminado = false;
	while (Terminado == false) {
		if (TRUE == GetMessage(&Message, 0, 0, 0)) {		// Minetras la Ventana retorne Mensajes
			TranslateMessage(&Message);                     // Traduzco el Mensaje
			DispatchMessage(&Message);                      // Paso el Mensaje a la Ventana
		}  
		else { // Se ha recibido un QUIT MESSAGE pero se anulo con el bucle anterior, asi que lo repetimos.
			PostQuitMessage(0);
		}
//		App.Eventos_Mirar();
	}
    Visible(false);
    Destruir();
}

// Cuando recibimos un click en un boton mandamos el mensaje a la ventana del reproductor
LRESULT VentanaMensaje_ErrorCritico::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
//    if (IDBotonEx == ID_BOTON_ACEPTAR) {
//    }
    Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);
    Terminado = true;
    DestroyWindow(_hWnd);
    return 0;
}


LRESULT VentanaMensaje_ErrorCritico::Evento_Cerrar(void) {
    Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);
    Terminado = true;
	return 0;
}

// Si se recibe la tecla ESC es que se quiere cancelar
LRESULT VentanaMensaje_ErrorCritico::Evento_BotonEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDBotonEx) {
	if (_TeclaPresionada == true) {
		_TeclaPresionada = false;
		if (TeclaVirtual == VK_ESCAPE) {
			Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);
			Terminado = true;
			DestroyWindow(_hWnd);
		}
	}
    return 0;
}





















VentanaMensaje_Aceptar::VentanaMensaje_Aceptar(void) {
	_TeclaPresionada = false;
}

VentanaMensaje_Aceptar::VentanaMensaje_Aceptar(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje) {
	_TeclaPresionada = false;
    MostrarMensaje(nTexto, nTitulo, nIDMensaje);
}

VentanaMensaje_Aceptar::~VentanaMensaje_Aceptar(void) {
}

// Función para mostrar el mensaje Aceptar
void VentanaMensaje_Aceptar::MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje) {
    SIZE TamMensaje = CrearVentanaMensaje(nTexto, nTitulo, nIDMensaje);
    DWL::DWLString TextoAceptar;
	if (Sistema.App.Idioma == NULL) TextoAceptar = TEXT("Ok");
	else		        			TextoAceptar = IDIOMA__ACEPTAR;
    UINT TmpY = 0;
   	if (Sistema.App.Config.UtilizarBarraTituloWindows == true)  TmpY = 8;
    else                                                        TmpY = 25;

	Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, TextoAceptar(), ((TamMensaje.cx + 20) - ANCHO_BOTON_VM) / 2, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	Boton_Cancelar.AsignarFoco();
	Visible(true);
}

// Cuando recibimos un click en un boton mandamos el mensaje a la ventana del reproductor
LRESULT VentanaMensaje_Aceptar::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
    if (IDBotonEx == ID_BOTON_ACEPTAR) {
        Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);
    }
    Destruir();
    return 0;
}

LRESULT VentanaMensaje_Aceptar::Evento_Cerrar(void) {
    Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);
    Destruir();
	return 0;
}













VentanaMensaje_AceptarCancelar::VentanaMensaje_AceptarCancelar(void) {
	_TeclaPresionada = false;
}

VentanaMensaje_AceptarCancelar::VentanaMensaje_AceptarCancelar(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje, const TCHAR *AceptarTxt, const TCHAR *CancelarTxt) {
	_TeclaPresionada = false;
    MostrarMensaje(nTexto, nTitulo, nIDMensaje);
}

VentanaMensaje_AceptarCancelar::~VentanaMensaje_AceptarCancelar(void) {
}

// Función para mostrar el mensaje Aceptar
void VentanaMensaje_AceptarCancelar::MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje, const TCHAR *AceptarTxt, const TCHAR *CancelarTxt) {
    SIZE TamMensaje = CrearVentanaMensaje(nTexto, nTitulo, nIDMensaje);
    UINT TmpY = 0;
   	if (Sistema.App.Config.UtilizarBarraTituloWindows == true)  TmpY = 8;
    else                                                        TmpY = 25;

	UINT TmpInt = (((TamMensaje.cx + 20) - ((ANCHO_BOTON_VM * 2) + 10))) / 2;

	if (AceptarTxt == NULL)		Boton_Aceptar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__ACEPTAR, TmpInt, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	else						Boton_Aceptar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, AceptarTxt, TmpInt, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	if (CancelarTxt == NULL)	Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__CANCELAR, TmpInt + ANCHO_BOTON_VM + 10, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_CANCELAR);
	else						Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, CancelarTxt, TmpInt + ANCHO_BOTON_VM + 10, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_CANCELAR);
	Visible(true);
}

// Cuando recibimos un click en un boton mandamos el mensaje a la ventana del reproductor
LRESULT VentanaMensaje_AceptarCancelar::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
    switch (IDBotonEx) {
        case ID_BOTON_ACEPTAR :
			Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);        
			break;
        case ID_BOTON_CANCELAR :
			Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Cancelar);
            break;
    }
    Destruir();
    return 0;
}

LRESULT VentanaMensaje_AceptarCancelar::Evento_Cerrar(void) {
    Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Cancelar);
    Destruir();
	return 0;
}













VentanaMensaje_AceptarCancelar_NoMostrarMas::VentanaMensaje_AceptarCancelar_NoMostrarMas(void) {
	_TeclaPresionada = false;
}

VentanaMensaje_AceptarCancelar_NoMostrarMas::VentanaMensaje_AceptarCancelar_NoMostrarMas(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje) {
	_TeclaPresionada = false;
    MostrarMensaje(nTexto, nTitulo, nIDMensaje);
}

VentanaMensaje_AceptarCancelar_NoMostrarMas::~VentanaMensaje_AceptarCancelar_NoMostrarMas(void) {
}

// Función para mostrar el mensaje Aceptar
void VentanaMensaje_AceptarCancelar_NoMostrarMas::MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje) {
    SIZE TamMensaje = CrearVentanaMensaje(nTexto, nTitulo, nIDMensaje);
    UINT TmpY = 0;
   	if (Sistema.App.Config.UtilizarBarraTituloWindows == true)  TmpY = 8;
    else                                                        TmpY = 25;


	UINT TmpInt = (((TamMensaje.cx + 20) - ((ANCHO_BOTON_VM * 2) + 10)) - 180) / 2;
	Boton_Aceptar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__ACEPTAR, TmpInt, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__CANCELAR, TmpInt + ANCHO_BOTON_VM + 10, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_CANCELAR);
	Marca_NoMostrarMas.MarcaEx_Estilos.Colores.TextoNormal = DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Normal;
	Marca_NoMostrarMas.MarcaEx_Estilos.Colores.TextoResaltado = DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Resaltado;
	Marca_NoMostrarMas.MarcaEx_Estilos.Colores.TextoPresionado = DWL::SO::DWLEstilos::Colores.ControlEx_Texto_Presionado;
	Marca_NoMostrarMas.CrearMarcaEx(WS_CHILD | WS_VISIBLE, _hWnd, IDIOMA__NO_MOSTRAR_MAS_ESTE_MENSAJE, TamMensaje.cx - 180, TamMensaje.cy + TmpY, ID_MARCA_NOMOSTRAR, false);

//	Boton_Cancelar.CrearBotonEx(WS_CHILD | WS_VISIBLE, _hWnd, TextoAceptar(), ((TamMensaje.cx + 20) - ANCHO_BOTON_VM) / 2, TamMensaje.cy + TmpY, ANCHO_BOTON_VM, 18, ID_BOTON_ACEPTAR);
	Visible(true);
	Boton_Cancelar.AsignarFoco();
}

// Cuando recibimos un click en un boton mandamos el mensaje a la ventana del reproductor
LRESULT VentanaMensaje_AceptarCancelar_NoMostrarMas::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
    switch (IDBotonEx) {
        case ID_BOTON_ACEPTAR :
		    if (Marca_NoMostrarMas.Marcado() == true)	Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar_NoMostrar);
		    else										Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);        
            break;
        case ID_BOTON_CANCELAR :
		    if (Marca_NoMostrarMas.Marcado() == true)	Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Cancelar_NoMostrar);
		    else										Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Cancelar);
            break;
    }
    Destruir();
    return 0;
}

LRESULT VentanaMensaje_AceptarCancelar_NoMostrarMas::Evento_MarcaEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT cID) {
	Marca_NoMostrarMas.Marcar(!Marca_NoMostrarMas.Marcado());
    return 0;
}

LRESULT VentanaMensaje_AceptarCancelar_NoMostrarMas::Evento_Cerrar(void) {
    Sistema.App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Cancelar);
    Destruir();
	return 0;
}








/*

// Cuando recibimos un click en un boton mandamos el mensaje a la ventana del reproductor
LRESULT VentanaMensaje_AceptarCancelar::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
    switch (Boton) {
	    case ID_BOTON_ACEPTAR :
            App.PlayerEx.Evento_VentanaMensaje(IDMensaje, Respuesta_Aceptar);
            break;
	    case ID_BOTON_CANCELAR :
            break;
    }
    DestroyWindow(_hWnd);
}*/
