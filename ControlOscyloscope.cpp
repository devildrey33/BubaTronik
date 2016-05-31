#include "StdAfx.h"
#include "ControlOscyloscope.h"
#include "VentanaEx.h"
#include "BubaTronikApp.h"

//DWL_APP(BubaTronikApp);



// Altura apropiada 40 de la barra mes 15 per el texte, mes 5 per les barretes de tope = [60]
// Hauria d'optimitzar les funcions de pintat


ControlOscyloscope::ControlOscyloscope(void) {
	for (int i = 0; i < NUM_BARRITAS + 1; i++) LineasOscy[i] = 40; 
	EstadoTexto = 0;
	hThread = NULL;
	Mutex = NULL;
	_Terminar = false;
}

ControlOscyloscope::~ControlOscyloscope(void) {
}

/*LRESULT CALLBACK ControlOscyloscope::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		ENLACE_Evento_PintarHDC();
		ENLACE_Evento_BorrarFondoHDC();
		ENLACE_Evento_Mouse_Movimiento();
		ENLACE_Evento_Mouse_BotonPresionado();
		ENLACE_Evento_Mouse_BotonSoltado();
//		ENLACE_Evento_Mouse_Saliendo();
//		case WM_MOUSELEAVE :
//			this->Evento_Mouse_Saliendo();
//			this->_MouseDentro = false;
//			return 0;
	}
	return DWL::ControlesEx::DWLBaseControlEx::GestorMensajes(uMsg, wParam, lParam);
}*/


void ControlOscyloscope::Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const UINT nID) {
	CrearControl(hWndParent, TEXT("Oscyloscope"), WS_CHILD | WS_VISIBLE, NULL, cX, cY, cAncho, cAlto, nID);
	_MouseDentro = false;
}


LRESULT ControlOscyloscope::Evento_BorrarFondo(HDC hDC) {
/*	RECT R;
	GetClientRect(_hWnd, &R);
	HBRUSH Fondo = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
	FillRect(hDC, &R, Fondo);
	DeleteObject(Fondo);*/
	return 0; 
};


unsigned long ControlOscyloscope::ThreadOscy(void *pThis) {
	HDC hDC = NULL;
	RECT R;
	ControlOscyloscope *This = static_cast<ControlOscyloscope *>(pThis);
	HWND hWndOscy = This->hWnd();
	while (This->_Terminar == false) {
		WaitForSingleObject(This->Mutex, INFINITE);
		GetClientRect(hWndOscy, &R);
		hDC = GetDC(hWndOscy);
		This->PintarOscy(hDC, &R);
		ReleaseDC(hWndOscy, hDC);
		hDC = NULL;
		ReleaseMutex(This->Mutex);
		Sleep(15); // 1000ms / 60frames = 16.66666ms per cada frame
	}

	CloseHandle(This->Mutex);
	This->Mutex = NULL;
	CloseHandle(This->hThread);
	This->hThread = NULL;
	return 0;
}

void ControlOscyloscope::Terminar(void) {
	if (Mutex != NULL) {
		WaitForSingleObject(Mutex, INFINITE);
		_Terminar = true;
		ReleaseMutex(Mutex);
	}
}

void ControlOscyloscope::IniciarThread(void) {
	if (Mutex == NULL) {
		Mutex = CreateMutex(NULL, FALSE, NULL);
		if (hThread == NULL) {
			_Terminar = false;
			hThread = CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))this->ThreadOscy, (void *)this, 0, NULL);
			if (hThread) SetThreadPriority(hThread, 0);
		}
		ReleaseMutex(Mutex);
	}
}

void ControlOscyloscope::PintarFondoVacio(void) {
	// Pinto el fondo en blanco cuando termino el thread
	DWL::GDI::DWLhDC	Buffer(_hWnd);
	RECT				FondoOscy = { 0, -21, ANCHO_OSCY_PINTABLE, VENTANAEX_ALTURA_PARTE_SUPERIOR -21 };
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
		FondoOscy.top = 0;
		FondoOscy.bottom = VENTANAEX_ALTURA_PARTE_SUPERIOR - 23;
	}
	Buffer.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, &FondoOscy, false);
	for (int i = 0; i < NUM_BARRITAS + 1; i++) LineasOscy[i] = 40; 
}
/*
// TerminateThread no es SAFE.....
void ControlOscyloscope::TerminarThread(void) {
	if (Mutex != NULL) {
		WaitForSingleObject(Mutex, INFINITE);
		if (hThread != NULL) TerminateThread(hThread, 0);
		hThread = NULL;
	}

	// Pinto el fondo en blanco cuando termino el thread
	DWL::GDI::DWLhDC	Buffer(_hWnd);
	RECT				FondoOscy = { 0, -21, ANCHO_OSCY_PINTABLE, VENTANAEX_ALTURA_PARTE_SUPERIOR -21 };
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
		FondoOscy.top = 0;
		FondoOscy.bottom = VENTANAEX_ALTURA_PARTE_SUPERIOR - 23;
	}
	Buffer.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, &FondoOscy, false);
	for (int i = 0; i < NUM_BARRITAS + 1; i++) LineasOscy[i] = 40; 

	if (Mutex != NULL) {
		ReleaseMutex(Mutex);
		CloseHandle(Mutex);
		Mutex = NULL;
	}
}
*/

LRESULT ControlOscyloscope::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	RECT R;
	GetClientRect(_hWnd, &R);
	WaitForSingleObject(Mutex, INFINITE);
	PintarOscy(hDC, &R);
	ReleaseMutex(Mutex);
    return 0;
}



void ControlOscyloscope::PintarOscy(HDC hDC, RECT *Espacio) {
	DWL::GDI::DWLRecta		Recta(0, 0, ANCHO_OSCY_PINTABLE, ALTO_OSCY_PINTABLE);
//	RECT					RO = { 0, 0, ANCHO_OSCY_PINTABLE, ALTO_OSCY_PINTABLE };
	static DWL::GDI::DWLhDC	Buffer(hDC, ANCHO_OSCY_PINTABLE, ALTO_OSCY_PINTABLE);
	int						Medida = (4 - static_cast<int>(Sistema.App.Config.TipoOscy));
	float				   *Resultado = NULL;
	size_t					Contador;
	int						XF = 0;
//	HBRUSH Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
//	FillRect(Buffer.hDC(), Recta(), Brocha);
//	DeleteObject(Brocha);
	static float			ArraySpectrum[512];
	RECT				FondoOscy = { 0, -21, ANCHO_OSCY_PINTABLE, VENTANAEX_ALTURA_PARTE_SUPERIOR -21 };
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) {
		FondoOscy.top = 0;
		FondoOscy.bottom = VENTANAEX_ALTURA_PARTE_SUPERIOR - 23;
	}
	Buffer.PintaGradient(DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.VentanaEx_Fondo_DegradadoInferior, &FondoOscy, false);

	#if defined UTILIZAR_FMOD
		Resultado   = Sistema.App.FMod.PillaSpectrum(512, ArraySpectrum);
	#endif
	if (Resultado == NULL) { // Si no esta sonando una cancion inicio los parametros a 0
		Resultado = ArraySpectrum;
		for (Contador = 0; Contador < NUM_BARRITAS; Contador ++) {
			Resultado[Contador] = 0.0f;
		}
	}
	/*else {
		Buffer.Fuente(&Sistema.App.Tahoma13b);
		switch (EstadoTexto) {
			case 0 :
				Buffer.PintarTexto(Sistema.App.FMod.CancionActual()->Cancion()->Nombre(), 0, 0, (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 18, Sistema.App.Skin.Ventana_TextoNormal, true);
				break;
			case 1 :
				Buffer.PintarTexto(Sistema.App.FMod.CancionActual()->Cancion()->Nombre(), 0, 0, (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 18, Sistema.App.Skin.Ventana_TextoResaltado, true);
				break;
			case 2 :
				Buffer.PintarTexto(Sistema.App.FMod.CancionActual()->Cancion()->Nombre(), 1, 1, (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 18, Sistema.App.Skin.Ventana_TextoPresionado, true);
				break;
		}
		if (Sistema.App.SliderTiempo.Estado() == BarraEx_Enum_Estados_Presionado)	Buffer.PintarTexto(Sistema.App.FMod.TiempoStr(Sistema.App.SliderTiempo.Valor()), ANCHO_OSCY_PINTABLE - ANCHO_TIEMPO, 0, Sistema.App.Skin.Ventana_TextoNormal, false);
		else																	Buffer.PintarTexto(Sistema.App.FMod.TiempoStr(Sistema.App.FMod.TiempoActual()), ANCHO_OSCY_PINTABLE - ANCHO_TIEMPO, 0, Sistema.App.Skin.Ventana_TextoNormal, false);
	}*/

	// Pinto barritas
	static DWL::GDI::DWLBrocha Negro(DWL_Enum_BrochasStock_Negra);
	for (Contador = 0; Contador < NUM_BARRITAS; Contador ++) {
		XF = static_cast<int>(MAXMEDIDA - (Resultado[Contador] * MULTIPLICADOR_OSCY));
//		XF = static_cast<int>(MAXMEDIDA - (Resultado[Contador] * (MULTIPLICADOR_OSCY + (Contador * 3) )));

		if (LineasOscy[Contador] >= XF - 3) {
			LineasOscy[Contador] = XF - 3;
			if (-3 > LineasOscy[Contador]) LineasOscy[Contador] = -3;
		}
		else {
			LineasOscy[Contador] ++;
		}
//		if (XF > 40) XF = 40;
		if (XF < 0)  XF = 0;

		Sistema.App.Barrita.PintarMapaBits(Buffer.hDC(), (Contador * Medida) + 1, XF + Y_ORIGEN, ((Contador + 1) * Medida), MAXMEDIDA, static_cast<int>(Sistema.App.Config.TipoOscy), XF);

		Recta.Asignar((Contador * Medida) + 1, Y_ORIGEN + LineasOscy[Contador], ((Contador + 1) * Medida), Y_ORIGEN + LineasOscy[Contador] + 1);
		Buffer.PintarRecta(&Recta, &Negro);   // Pinto Lineas
	}

	//RECT RC;
	//GetClientRect(_hWnd, &RC);

	Buffer.PintarMapaBits(hDC, (Espacio->right - ANCHO_OSCY_PINTABLE) / 2, 0, ANCHO_OSCY_PINTABLE, ALTO_OSCY_PINTABLE, 0, 0);
}
/*
void ControlOscyloscope::PintarOscy(HDC hDC, RECT *Espacio) {
//	bool					DR = false;
	RECT R;
	GetClientRect(_hWnd, &R);

	float				   *Resultado = NULL;
	DWL::GDI::DWLhDC		Buffer(hDC, R.right, R.bottom);
	DWL::GDI::DWLRecta		Recta(0, -24, Espacio->right - Espacio->left, ALTURA_PARTE_SUPERIOR -25);
	int						Medida = (4 - static_cast<int>(Sistema.App.Config.TipoOscy));
//	int						NumBarritas = ((Espacio->right - Espacio->left) / Medida);
	int						NumBarritas = 160;
	int						NumBarritasSpec = 256;
	//if (NumBarritas > 32)   NumBarritasSpec = 256;
	//if (NumBarritas > 64)   NumBarritasSpec = 512;
	//if (NumBarritas > 128)  NumBarritasSpec = 1024;
	int						Contador	= 0;
	int						XF			= 0;
	float					ArraySpectrum[1024];

	Resultado   = Sistema.App.FMod.PillaSpectrum(NumBarritasSpec, ArraySpectrum);
//	int						TopeBarras  = Medida + 1;
	//Buffer.PintaGradient(DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoSuperior, DWL::SO::DWLEstilos::Colores.ControlEx_Fondo_DegradadoInferior, &Recta, false);
	DWL::GDI::DWLBrocha Brocha(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
	FillRect(Buffer.hDC(), &R, Brocha());

	
	if (Resultado == NULL) { // Si no esta sonando una cancion inicio los parametros a 0
		//Resultado = new float[NumBarritas];
		Resultado = ArraySpectrum;
		for (Contador = 0; Contador < NumBarritas; Contador ++) {
			Resultado[Contador] = 0.0f;
		}
//		DR = true;
	}
	else {
//		if (Sistema.App.FMod.CancionActual() != NULL) {
//			if (Sistema.App.FMod.CancionActual()->Estado() != EnStop) {
		Buffer.Fuente(&Sistema.App.Tahoma13);
		switch (EstadoTexto) {
			case 0 :
				Buffer.PintarTexto(Sistema.App.FMod.CancionActual()->Cancion()->Nombre(), 0, 0, (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 18, Sistema.App.Skin.Ventana_TextoNormal, true);
				break;
			case 1 :
				Buffer.PintarTexto(Sistema.App.FMod.CancionActual()->Cancion()->Nombre(), 0, 0, (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 18, Sistema.App.Skin.Ventana_TextoResaltado, true);
				break;
			case 2 :
				Buffer.PintarTexto(Sistema.App.FMod.CancionActual()->Cancion()->Nombre(), 1, 1, (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 18, Sistema.App.Skin.Ventana_TextoPresionado, true);
				break;
		}
		if (Sistema.App.SliderTiempo.Estado() == BarraEx_Enum_Estados_Presionado)	Buffer.PintarTexto(Sistema.App.FMod.TiempoStr(Sistema.App.SliderTiempo.Valor()), (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 0, Sistema.App.Skin.Ventana_TextoNormal, false);
		else																	Buffer.PintarTexto(Sistema.App.FMod.TiempoStr(Sistema.App.FMod.TiempoActual()), (Espacio->right - Espacio->left) - ANCHO_TIEMPO, 0, Sistema.App.Skin.Ventana_TextoNormal, false);
//			}
//		}
	}

	DWL::GDI::DWLBrocha Negro(Brocha_Negra);

	for (Contador = 0; Contador < NumBarritas; Contador ++) {
		XF = static_cast<int>(MAXMEDIDA - (Resultado[Contador] * (MULTIPLICADOR_OSCY + (Contador * 3) )));

		if (LineasOscy[Contador] >= XF - 3) {
			LineasOscy[Contador] = XF - 3;
			if (-3 > LineasOscy[Contador]) LineasOscy[Contador] = -3;
		}
		else {
			LineasOscy[Contador] ++;
		}
//		if (XF > 40) XF = 40;
		if (XF < 0)  XF = 0;

		Sistema.App.Barrita.PintarMapaBits(Buffer.hDC(), (Contador * Medida) + 1, XF + Y_ORIGEN, ((Contador + 1) * Medida), MAXMEDIDA, static_cast<int>(Sistema.App.Config.TipoOscy), XF);

		Recta.Asignar((Contador * Medida) + 1, Y_ORIGEN + LineasOscy[Contador], ((Contador + 1) * Medida), Y_ORIGEN + LineasOscy[Contador] + 1);
		Buffer.PintarRecta(&Recta, &Negro);   // Pinto Lineas

	}

	Buffer.PintarMapaBits(hDC, Espacio->left, Espacio->top, R.right, R.bottom, 0, 0);


//	if (DR == true) delete [] Resultado;
}*/


LRESULT ControlOscyloscope::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
    return 0;
}


LRESULT ControlOscyloscope::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	if (Sistema.App.Media.Actual > -1 && Sistema.App.Media.Actual < static_cast<int>(Sistema.App.ListaC.TotalItems())) {
		Sistema.App.ToolTip.Destruir();
		Sistema.App.ArbolBaseDatos.MostrarToolTipNodo(Sistema.App.ListaC.ItemMedio(Sistema.App.Media.Actual)->Medio->Nodo);
//		Sistema.App.ListaC.ListaEx_Evento_MostrarToolTip(Sistema.App.ListaC.ItemMedio(Sistema.App.Media.Actual));
	}
    return 0;
}


LRESULT ControlOscyloscope::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
//	_MirarMouseLeave();
//	Sistema.App.ListaC.ToolTip.Visible(false);
	SendMessage(GetParent(_hWnd), WM_NCLBUTTONDOWN, HTCAPTION, 0); //  Muevo el Formulario siguiendo la posicion del raton
    return 0;
}


LRESULT ControlOscyloscope::Evento_Mouse_Saliendo(void) {
	Sistema.App.ToolTip.Destruir();
//	_MouseDentro = false;
    return 0;
}