#include "StdAfx.h"
#include "VentanaPlayerEx.h"
#include <DWLMedidasSistema.h>
#include "resource.h"
#include <shellapi.h>
#include <dbt.h>		// Device Management Events
#include "InformacionArchivoEx.h"
#include <DWLDialogosComunes.h>
#include <DWLPortaPapeles.h>
#include "Definiciones.h"
#include "BubaTronikApp.h"
#include <DWLMouse.h>
#include "TipoArchivo.h"




#define WM_ACTUALIZARBDA WM_USER + 5000


// IDs de controles
#define ID_OPCIONES_BASICAS			1000
#define ID_PREFERENCIAS				1001


LRESULT CALLBACK VentanaPlayerEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
 		case WM_DROPFILES :
			SoltarArchivos(wParam);
			break;

		case WM_GETMINMAXINFO :
			MINMAXINFO *mmi;
			mmi = (MINMAXINFO*) lParam;
			mmi->ptMinTrackSize.x = MINIMO_PLAYER_ANCHO; // Width
			mmi->ptMinTrackSize.y = MINIMO_PLAYER_ALTO; // Height
			break;

		case WM_NCLBUTTONDBLCLK :
			Maximizar(!Maximizado());
			return 0;

        case MENSAJE_ACTUALIZACION_ENCONTRADA :
            VentanaActualizacion.Mostrar();
			break;

		case WM_ACTUALIZARBDA  :
			Sistema.App.BD.ActualizarArbol();
			break;
		case WM_MENUCHAR :	
			EndMenu();
			SetFocus(MenusPlayer.hWnd());
			SetCapture(MenusPlayer.hWnd());
			return 0;
		case WM_INITMENU :   
			EndMenu();
			SetFocus(MenusPlayer.hWnd());
			SetCapture(MenusPlayer.hWnd());
			return 0;																																	
		case WM_INITMENUPOPUP :   
			EndMenu();
			SetFocus(MenusPlayer.hWnd());
			SetCapture(MenusPlayer.hWnd());
			return 0;																																
		case WM_ENTERMENULOOP :
		    EndMenu();
			if (Sistema.App.EditandoTeclasRapidas == false) {
				Sistema.App.Eventos_Mirar();
				MenusPlayer.Mostrar(_hWnd);
			}
			return 0;
			break;
		case WM_PARAMETROS :
			ParametrosInstancia(wParam, lParam);
			break;

		case WM_DEVICECHANGE :
			PDEV_BROADCAST_HDR *lpdb;
			lpdb = (PDEV_BROADCAST_HDR *)lParam;
			if (wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE) {
				Sistema.App.EscanearUnidades();
				Sistema.App.BD.ActualizarArbol();
			}
			break;

		case WM_PAINT : 
			
			if (Sistema.App.MediaPlayer.EstaEnPantallaCompleta() == true) return DefWindowProc(_hWnd, WM_PAINT, wParam, lParam);
			break;

	}
	return DWLVentana::GestorMensajes(uMsg, wParam, lParam);	
};


LRESULT VentanaPlayerEx::Evento_BotonBarraTareas_Creado(void) {
	BarraTareas.Boton_Agregar(IDI_ATRAS_N		, IDIOMA__MEDIO_SIGUIENTE	, ID_BOTON_CANCION_ATRAS);
	BarraTareas.Boton_Agregar(IDI_PLAY_N		, IDIOMA__PLAY				, ID_BOTON_CANCION_PLAY);
	BarraTareas.Boton_Agregar(IDI_PAUSA_N		, IDIOMA__PAUSA				, ID_BOTON_CANCION_PAUSA);
	BarraTareas.Boton_Agregar(IDI_STOP_N		, IDIOMA__STOP				, ID_BOTON_CANCION_STOP);
	BarraTareas.Boton_Agregar(IDI_ADELANTE_N	, IDIOMA__MEDIO_ANTERIOR	, ID_BOTON_CANCION_ADELANTE);
	return 0;
}


void VentanaPlayerEx::ParametrosInstancia(WPARAM wParam, LPARAM lParam) {
	// Si se esta buscando archivos NO SE PUEDE AGREGAR NADA EXTERNO o la liaremos xd
	if (Sistema.App.PlayerEx.CP.BuscarArchivos != NULL) {
		static VentanaMensaje_Aceptar MsgAceptar;
		MsgAceptar.MostrarMensaje(IDIOMA__NO_PUEDES_ANADIR_MEDIOS_EXTERNAMENTE_MIENTRAS_SE_ESTA_REALIZANDO_UNA_BUSQUEDA, IDIOMA__ERROR);
		return;
	}
	bool ListaNueva = true;
	if (Sistema.App.ListaC.TotalItems() > 0) ListaNueva = false;
	DWL::DWLString Str = DWL::SO::DWLPortaPapeles::ObtenerTexto(_hWnd);
	DWL::DWLString Tmp;
	ItemListaMedios *nMedio = NULL;
	for (unsigned int i = 0; i < Str.Tam(); i++) {
		switch (Str[i]) {
			case TEXT('<') :
				Tmp = TEXT("");
				break;
			case TEXT('>') :
				if (Tmp == TEXT("-r")) { // reproducir
					ListaNueva = true;
					Sistema.App.Media.EliminarMedioActual();
					Sistema.App.ListaC.EliminarTodosLosMedios();
				}
				else {
					nMedio = AgregarArchivoLista(Tmp());
				}
				break;
			default :
				Tmp += Str[i];
				break;
		}
	}

	Sistema.App.ListaC.ActualizarTodo();
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	if (ListaNueva == true) {
		Sistema.App.Media.AbrirPrimerMedioLista();
	}
	else { // Si no es una lista nueva, comprobamos que se esta reproduciendo algo
		bool Ret = false;
		if (Sistema.App.Media.ComprobarEstado() != EnPlay && nMedio != NULL) { // Si no se esta reproduciendo reproducimos el medio que se acaba de agregar
			for (size_t z = 0; z < Sistema.App.ListaC.TotalItems(); z++) {
				if (Sistema.App.ListaC.ItemMedio(z) == nMedio) { 
					Sistema.App.Media.Actual = z;
					Ret = Sistema.App.Media.AbrirMedio(nMedio);
					if (Ret == true) Sistema.App.Media.Play();
					break;
				}
			}
			
		}
	}
	// Si la lista esta visible, la repinto
	if (Sistema.App.ListaC.Visible() == TRUE) Sistema.App.ListaC.Repintar(true);
	Sistema.App.BD.ActualizarArbol();
}

VentanaPlayerEx::VentanaPlayerEx(void) {
	EditandoNombre = false;
	NoValidarControles = true;
}

VentanaPlayerEx::~VentanaPlayerEx(void) {
}

void VentanaPlayerEx::Crear(void) {
	DWL::DWLString Titulo;
	Titulo.sprintf(TEXT("%s %s"), APP_NOMBRE, APP_STR_VERSION);
	CrearVentanaEx(APP_NOMBRE, Sistema.App.Config.X, Sistema.App.Config.Y, Sistema.App.Config.Ancho, Sistema.App.Config.Alto, IDI_DAP, Titulo());

	int VentanaExBorde = VENTANAEX_BORDE;
	int VentanaExAltoParteSuperior = VENTANAEX_ALTURA_PARTE_SUPERIOR;
	int An = Sistema.App.Config.Ancho;
	int Al = Sistema.App.Config.Alto;
	int InicioControles = 24;
	CM.Crear(_hWnd, An - CM_ANCHO, 0, ID_CM);

	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		VentanaExBorde = 0;
		VentanaExAltoParteSuperior -= 22;
		InicioControles = 3;
		Al -= (Sistema.Medidas.Alto_BarraTitulo() + Sistema.Medidas.Alto_Borde());
		An -= (Sistema.Medidas.Ancho_Borde() * 2);
	}
	else {
		CM.Visible(true);
	}

	Oscy.Crear(_hWnd, X_OSCY, InicioControles - 3, ANCHO_OSCY, ALTO_OSCY, ID_OSCY);

	DWL::DWLString PathBarra = Sistema.App.AppPath();
	PathBarra += TEXT("Graficos\\Barrita.bmp");
	if (Sistema.App.Barrita.CrearBitmapArchivo(PathBarra()) == false ) {
		// Error cargando el archivo (%s)\nEste error no es grave y el reproductor puede continuar la ejecucion del programa.\nReinstala el reproductor para solucionar el problema
		Titulo.sprintf(IDIOMA__ERROR_CARGANDO_EL_ARCHIVO_S_ESTE_ERROR_NO_ES_GRAVE, PathBarra());
        VentanaMensaje_Aceptar MsgErr(Titulo(), IDIOMA__ERROR, 0);
	}

	Botones.CrearBotonera(_hWnd, X_BOTONERA, InicioControles);
	Botones.Boton_Stop_Desactivado(true, false);

	BotonShufleRepeat.CrearShufleRepeat(_hWnd, X_BOTON_SHUFLE_REPEAT, InicioControles);
//	if (App.Config.Repeat != Repeat_Desactivado) BotonShufleRepeat.RepeatMarcado(true);

	TiempoStrActual.CrearCTA(_hWnd, X_TIEMPO_ACTUAL, InicioControles);
	Volumen.Crear(_hWnd, X_VOLUMEN, InicioControles + 14, ANCHO_VOLUMEN, ALTO_VOLUMEN, ID_CONTROL_VOLUMEN);

//	Preferencias.Crear(_hWnd, VentanaExBorde, VentanaExAltoParteSuperior, Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), ID_PREFERENCIAS);
	Ancho_Opciones_Basicas = OpcionesBasicas.CalcularAncho(_hWnd);
	OpcionesBasicas.Crear(_hWnd, ID_OPCIONES_BASICAS, An - (VentanaExBorde + Ancho_Opciones_Basicas), VentanaExAltoParteSuperior, Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1));
	CP.Crear(_hWnd, VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), ID_CONTROL_PRESENTACION);
	Sistema.App.ArbolBaseDatos.Crear(_hWnd, VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), ID_ARBOL_BASEDATOS);

	Sistema.App.ListaC.Crear(_hWnd, VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), ID_LISTA_CANCIONES);
	Sistema.App.ListaC.BarraScrollEx_VisibleV(TRUE);
	Sistema.App.ListaC.ActualizarTodo();
//	Sistema.App.ListaC.MostrarToolTip(true);
	
	Sistema.App.MediaPlayer.Crear(_hWnd, VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), ID_MEDIA_PLAYER);

	Sistema.App.SliderTiempo.CrearBarraEx(WS_CHILD | WS_VISIBLE, _hWnd, VENTANAEX_MARGEN, InicioControles + 26, An - (VENTANAEX_MARGEN * 2), 9, ID_SLIDER_TIEMPO, DWL_BARRAEX_DESPLAZAMIENTO_HORIZONTAL, 0, 5000);
	Sistema.App.SliderTiempo.Valor(0);

	CrearMenus();

	Sistema.App.BD.ActualizarArbol();
//	CrearTemporizador(TIMER_OSCY, INTERVALO_OSCY);
	CrearTemporizador(TIMER_LISTA, 500);
	CrearTemporizador(TIMER_BARRATIEMPO, 200);
	CrearTemporizador(TIMER_BLINK, 1500);

	Visible(true);
	if (Sistema.App.Config.SiempreDelante == true) SetWindowPos(hWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


//	Sistema.DEBUG_MostrarErrorGrave();
}


LRESULT VentanaPlayerEx::Evento_Cerrar(void) {
	static VentanaMensaje_Aceptar MsgErr;

	Sistema.App.PlayerEx.CP.CancelarBusquedaArchivos();
	Sistema.App.BD.Parar_RevisarBD();

	// Damos tiempo a que terminen las revisiones / busquedas
	Sistema.App.Eventos_Mirar();
	Sistema.App.Eventos_Mirar();
	Sistema.App.Eventos_Mirar();
	Sistema.App.Eventos_Mirar();
	Sistema.App.Eventos_Mirar();

	EliminarTemporizador(TIMER_BLINK);
	EliminarTemporizador(TIMER_LISTA);
	EliminarTemporizador(TIMER_BARRATIEMPO);
	DWL::SO::DWLMouse::Visible(TRUE);
	// Espero y termino el thread del oscy
//	WaitForSingleObject(Oscy.Mutex, INFINITE);
	Oscy.Terminar();
	// Guardo posición de la ventana
	WINDOWPLACEMENT WP;
	WP.length = sizeof(WINDOWPLACEMENT); 
	GetWindowPlacement(_hWnd, &WP); 
	if (WP.rcNormalPosition.left != -32000) {
		Sistema.App.Config.X = WP.rcNormalPosition.left;
		Sistema.App.Config.Y = WP.rcNormalPosition.top;
		Sistema.App.Config.Ancho = WP.rcNormalPosition.right - WP.rcNormalPosition.left;
		Sistema.App.Config.Alto = WP.rcNormalPosition.bottom - WP.rcNormalPosition.top;
	}
	Visible(false);
	PostQuitMessage(0);
	return 0;
}



LRESULT VentanaPlayerEx::Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto) {
	_Maximizado = Maximizada();
/*	LONG Estilos = GetWindowLongPtr(_hWnd, GWL_STYLE);
	if (Estilos & WS_MAXIMIZE == true) 
		_Maximizado = true;
	else
		_Maximizado = false;*/

//	if (Rectangulo->right - Rectangulo->left < MINIMO_PLAYER_ANCHO) Rectangulo->right = Rectangulo->left + MINIMO_PLAYER_ANCHO;
//	if (Rectangulo->bottom - Rectangulo->top < MINIMO_PLAYER_ALTO) Rectangulo->bottom = Rectangulo->top + MINIMO_PLAYER_ALTO; // - DWL::SO::DWLMedidasSistema::Alto_BarraTitulo();
	int An = nAncho; //Rectangulo->right - Rectangulo->left;
	int Al = nAlto;  //Rectangulo->bottom - Rectangulo->top;

	int InicioControles = 24;
	int VentanaExBorde = VENTANAEX_BORDE;
	int VentanaExAltoParteSuperior = VENTANAEX_ALTURA_PARTE_SUPERIOR;
	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		VentanaExBorde = 0;
		InicioControles = 3;
		VentanaExAltoParteSuperior -= 22;
//		Al -= (DWL::DWLSO::Medidas.Alto_BarraTitulo() + DWL::DWLSO::Medidas.Alto_Borde());
//		An -= (DWL::DWLSO::Medidas.Ancho_Borde() * 2);
	}
	else {
		CM.Mover(An - CM_ANCHO, 0, CM_ANCHO, CM_ALTO, false);
	}

	if (_Maximizado == false) {
		OpcionesBasicas.Mover(An - (VentanaExBorde + Ancho_Opciones_Basicas), VentanaExAltoParteSuperior, Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		Sistema.App.ListaC.Mover(VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		Sistema.App.ArbolBaseDatos.Mover(VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		CP.Mover(VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		Sistema.App.MediaPlayer.Mover(VentanaExBorde, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas - (VentanaExBorde * 2), Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
	}
	else {
		OpcionesBasicas.Mover(An - (Ancho_Opciones_Basicas), VentanaExAltoParteSuperior, Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		Sistema.App.ListaC.Mover(0, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		Sistema.App.ArbolBaseDatos.Mover(0, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		CP.Mover(0, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
		Sistema.App.MediaPlayer.Mover(0, VentanaExAltoParteSuperior, An - Ancho_Opciones_Basicas, Al - ((VentanaExAltoParteSuperior + VENTANAEX_ALTURA_PARTE_INFERIOR) - 1), true);
	}

	Sistema.App.SliderTiempo.Mover(VENTANAEX_MARGEN, InicioControles + 26, An - (VENTANAEX_MARGEN * 2), 9, false);
	Oscy.Mover(245, InicioControles - 3, ANCHO_OSCY, ALTO_OSCY, false);

	if (Sistema.App.MediaPlayer.Visible() == TRUE) Sistema.App.MediaPlayer.Evento_CambioTam(Tipo, An, Al);
	Sistema.App.ListaC.Evento_CambioTam(Tipo, An, Al);

	if (Sistema.App.Config.UtilizarBarraTituloWindows == true) { 
		InvalidateRect(_hWnd, NULL, FALSE);
		Evento_ValidarControles();
	}
    return 0;
}

// Evento que salta cuando estamos redimensionando la ventana, y se requiere validar los controles para que no salten
void VentanaPlayerEx::Evento_ValidarControles(void) {
	if (NoValidarControles == true) { 
		// Cuando se crea la ventana se recibe un Evento_CambioTam que evitamos con esto, ya que la primera vez aun no se han pintado los controles
		NoValidarControles = false;
		return;
	}

    WINDOWPLACEMENT WP;
    WP.length = sizeof(WINDOWPLACEMENT);
	HRGN Region = CreateRectRgn(0, 0, 0, 0);
	// Validamos slider del tiempo
	GetWindowRgn(Sistema.App.SliderTiempo.hWnd(), Region);
    GetWindowPlacement(Sistema.App.SliderTiempo.hWnd(), &WP);
	OffsetRgn(Region, WP.rcNormalPosition.left, WP.rcNormalPosition.top);
	ValidateRgn(_hWnd, Region);
	DeleteObject(Region);
	/*// Validamos botonera
	Region = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(Botones.hWnd(), Region);
    GetWindowPlacement(Botones.hWnd(), &WP);
	OffsetRgn(Region, WP.rcNormalPosition.left, WP.rcNormalPosition.top);
	ValidateRgn(_hWnd, Region);
	DeleteObject(Region);
	// Validamos shufle / repeat
	Region = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(BotonShufleRepeat.hWnd(), Region);
    GetWindowPlacement(BotonShufleRepeat.hWnd(), &WP);
	OffsetRgn(Region, WP.rcNormalPosition.left, WP.rcNormalPosition.top);
	ValidateRgn(_hWnd, Region);
	DeleteObject(Region);
	// Validamos volumen
	Region = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(Volumen.hWnd(), Region);
    GetWindowPlacement(Volumen.hWnd(), &WP);
	OffsetRgn(Region, WP.rcNormalPosition.left, WP.rcNormalPosition.top);
	ValidateRgn(_hWnd, Region);
	DeleteObject(Region);
/*	// Validamos control cerrar max min
	Region = CreateRectRgn(0, 0, 0, 0);
	GetWindowRgn(CM.hWnd(), Region);
	OffsetRgn(Region, CM.X(), CM.Y());
	ValidateRgn(_hWnd, Region);
	DeleteObject(Region);
	*/

	// Oscy
	GetWindowPlacement(Oscy.hWnd(), &WP); 
	ValidateRect(_hWnd, &WP.rcNormalPosition);
}


LRESULT VentanaPlayerEx::Evento_ArbolEx_LabelEdit_Inicio(DWL::ControlesEx::DWLArbolEx_Nodo *eNodo, const UINT IDArbolEx) {
	if (IDArbolEx == ID_ARBOL_BASEDATOS) {
		EditandoNombre = true;
		HWND hWndEdit = Sistema.App.ArbolBaseDatos.hWndEditBox();
		if (eNodo != NULL) {
			switch (eNodo->Icono()) {
				case IDI_C0 :	case IDI_C1 :	case IDI_C2 :	case IDI_C3 :	case IDI_PLAY :		case IDI_PISTAAUDIO :
					SetWindowText(hWndEdit, static_cast<NodoArbolBaseDatos *>(eNodo)->Medio->Nombre());
//					static_cast<NodoArbolBaseDatos *>(eNodo)->Medio->Nombre();
					break;
			}
		}
	}
    return 0;
}


LRESULT VentanaPlayerEx::Evento_ArbolEx_LabelEdit_Fin(DWL::ControlesEx::DWLArbolEx_TerminarLabelEdit *Edicion, const UINT IDArbolEx) {
	if (IDArbolEx == ID_ARBOL_BASEDATOS) {
		DWL::DWLString		Str;
		NodoArbolBaseDatos *Nodo = static_cast<NodoArbolBaseDatos *>(Edicion->Nodo());
		int					Chun = 0;
		//TCHAR				NuevoTexto[512];
		//GetWindowText(Sistema.App.ArbolBaseDatos.ObtenerEditBox_LabelEdit(), NuevoTexo, 512);
		switch (Edicion->Nodo()->Icono()) {
			case IDI_VIDEO	:	case IDI_PLAY	:	case IDI_CDAUDIO	:   case IDI_PISTAAUDIO	: 
			case IDI_C0		:	case IDI_C1		:	case IDI_C2			:	case IDI_C3		: // es una cancion / video, hay que poner la pista delante
				Nodo->Medio->Nombre = Edicion->NuevoTexto();
				switch (Nodo->Medio->TipoMedio) {
					case Tipo_Medio_Audio :
						if (static_cast<TMedioAudio *>(Nodo->Medio)->Pista > 0)	Str.sprintf(TEXT("%02d %s"), static_cast<TMedioAudio *>(Nodo->Medio)->Pista, Nodo->Medio->Nombre());
						else													Str = Nodo->Medio->Nombre();
						break;
					case Tipo_Medio_CDAudio :
						if (static_cast<TMedioCDAudio *>(Nodo->Medio)->Pista > 0)	Str.sprintf(TEXT("%02d %s"), static_cast<TMedioCDAudio *>(Nodo->Medio)->Pista, Nodo->Medio->Nombre());
						else														Str = Nodo->Medio->Nombre();
						break;
					case Tipo_Medio_Video :
						if (static_cast<TMedioVideo *>(Nodo->Medio)->Pista > 0)	Str.sprintf(TEXT("%02d %s"), static_cast<TMedioVideo *>(Nodo->Medio)->Pista, Nodo->Medio->Nombre());
						else													Str = Nodo->Medio->Nombre();
						break;
				}
			 	if (Nodo->Medio->Item != NULL) Nodo->Medio->Item->Texto(1, Edicion->NuevoTexto());
				Edicion->NuevoTexto = Str;
				return 0;
			case IDI_TIPO :
				Chun = Sistema.App.BD.BuscarStrTp(Edicion->NuevoTexto(), Sistema.App.BD.StrTipos);
				Nodo->Tipo->Nombre->Nombre = Edicion->NuevoTexto;
				break;
			case IDI_GENERO :
				Chun = Sistema.App.BD.BuscarStrTp(Edicion->NuevoTexto(), Sistema.App.BD.StrGeneros);
				Nodo->Genero->Nombre->Nombre = Edicion->NuevoTexto;
				break;
			case IDI_GRUPO :
				Chun = Sistema.App.BD.BuscarStrTp(Edicion->NuevoTexto(), Sistema.App.BD.StrGrupos);
				Nodo->Grupo->Nombre->Nombre = Edicion->NuevoTexto;
				break;
			case IDI_DISCO :
				Chun = Sistema.App.BD.BuscarStrTp(Edicion->NuevoTexto(), Sistema.App.BD.StrDiscos);
				Nodo->Disco->Nombre->Nombre = Edicion->NuevoTexto;
				break;
		}
		if (Chun != -1) PostMessage(_hWnd, WM_ACTUALIZARBDA, 0, 0);
		EditandoNombre = false;
	}
    return 0;
}


LRESULT VentanaPlayerEx::Evento_ArbolEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDArbolEx) {
	switch (IDArbolEx) {
		case ID_ARBOL_BASEDATOS :
			if (TeclaVirtual == VK_DELETE) BDAEliminar();
			break;
	}
    return 0;
}


LRESULT VentanaPlayerEx::Evento_ArbolEx_Mouse_Click(const UINT Boton, const int cX, const int cY, DWL::ControlesEx::DWLArbolEx_Nodo *nNodo, const UINT IDArbolEx) {
	bool			MedioValido = false;
	switch (IDArbolEx) {
		case ID_ARBOL_BASEDATOS :
			if (Boton == 1) MostrarMenuBDA();
			break;

		case ID_OPCIONES_BASICAS :
			if (nNodo == NULL) return 0;

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__INICIO) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_Inicio();
                SetFocus(CP.hWnd());
				return 0;
			}
			
			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__ABRIR_ARCHIVO) == 0) {
				AbrirArchivo();
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__BASE_DE_DATOS) == 0) {
				if (nNodo->Padre() == NULL) {	// Ver base de datos
					CP.Visible(false);
					Sistema.App.ListaC.Visible(false);
					Sistema.App.MediaPlayer.Visible(false);
					Sistema.App.ArbolBaseDatos.Visible(true);
                    SetFocus(Sistema.App.ArbolBaseDatos.hWnd());
					return 0;
				}
				else {							// Opciones base de datos
					Sistema.App.ArbolBaseDatos.Visible(false);
					Sistema.App.ListaC.Visible(false);
					Sistema.App.MediaPlayer.Visible(false);
					CP.Visible(true);
					CP.Mostrar_Opciones_BD();
                    SetFocus(CP.hWnd());
					return 0;
				}
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__BUSCAR_ARCHIVOS) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_BuscarArchivos();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__REVISAR_MEDIOS) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_RevisarBD();
                SetFocus(CP.hWnd());
				return 0;
			}

/*			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__REVISAR_GENEROS) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_CoincidenciasGenero();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__REVISAR_GRUPOS) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_CoincidenciasGrupo();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__REVISAR_DISCOS) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_CoincidenciasDisco();
                SetFocus(CP.hWnd());
				return 0;
			}*/

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__ECUALIZADOR) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_Ecualizador();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__VER_VIDEO) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				CP.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(true);
                SetFocus(Sistema.App.MediaPlayer.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__LISTA_DE_REPRODUCCION) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(false);
				Sistema.App.ListaC.Visible(true);
                SetFocus(Sistema.App.ListaC.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__OPCIONES) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
                SetFocus(CP.hWnd());
				CP.Mostrar_Opciones();
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__TECLAS_RAPIDAS) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_Opciones_Teclado();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__ASPECTO) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_Opciones_Apariencia();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__LISTA) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_Opciones_Lista();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__VIDEO) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_Opciones_Video();
                SetFocus(CP.hWnd());
				return 0;
			}

			if (DWLStrCmpi(nNodo->Texto(), IDIOMA__SOBRE_BUBATRONIK) == 0) {
				Sistema.App.ArbolBaseDatos.Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				CP.Visible(true);
				CP.Mostrar_SobreBubaTronik();
                SetFocus(CP.hWnd());
				return 0;
			}

			#ifdef MOSTRAR_LOG
				if (DWLStrCmpi(nNodo->Texto(), IDIOMA__MOSTRAR_LOG) == 0) {
					Sistema.App.VentanaLogs.Crear();
					SetFocus(Sistema.App.VentanaLogs.hWnd());
					return 0;
				}
			#endif

			break;
	}
    return 0;
}

void VentanaPlayerEx::AbrirArchivo(void) {
    // Creamos la lista con las extensiones necesarias para el dialogo abrir
    TCHAR Tipos[4096];
    ZeroMemory(Tipos, 4096 * sizeof(TCHAR));
    UINT  Pos = 0;
    
    DWL::DWLString TmpTxt = IDIOMA__MEDIOS_REPRODUCIBLES;
    DWL::DWLString TmpTxt2;
    CopyMemory(&Tipos[0], TmpTxt(), (TmpTxt.Tam() + 1) * sizeof(TCHAR));
    Pos = TmpTxt.Tam() + 1;
    TmpTxt.Borrar();
    for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
		TmpTxt2.sprintf(TEXT("*.%s;"), Sistema.App.Tipos.Tipos[i]->Extension());
        TmpTxt += TmpTxt2;
    }
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() - 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam();

    TmpTxt = IDIOMA__ARCHIVOS_DE_AUDIO;
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() + 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam() + 1;
    for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
        if (Sistema.App.Tipos.Tipos[i]->Tipo == Tipo_Medio_Audio) {
            TmpTxt2.sprintf(TEXT("*.%s;"), Sistema.App.Tipos.Tipos[i]->Extension());
            TmpTxt += TmpTxt2;
        }
    }
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() - 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam();

    TmpTxt = IDIOMA__ARCHIVOS_DE_VIDEO;
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() + 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam() + 1;
    for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
        if (Sistema.App.Tipos.Tipos[i]->Tipo == Tipo_Medio_Video) {
            TmpTxt2.sprintf(TEXT("*.%s;"), Sistema.App.Tipos.Tipos[i]->Extension());
            TmpTxt += TmpTxt2;
        }
    }
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() - 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam();

    TmpTxt = IDIOMA__LISTAS;
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() + 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam() + 1;
    for (size_t i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
        if (Sistema.App.Tipos.Tipos[i]->Tipo == Tipo_Medio_Lista) {
            TmpTxt2.sprintf(TEXT("*.%s;"), Sistema.App.Tipos.Tipos[i]->Extension());
            TmpTxt += TmpTxt2;
        }
    }
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() - 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam();

	TmpTxt.sprintf(TEXT("%s\0*.*"), IDIOMA__TODOS_LOS_ARCHIVOS);
    CopyMemory(&Tipos[Pos], TmpTxt(), (TmpTxt.Tam() + 1) * sizeof(TCHAR));
    Pos += TmpTxt.Tam() + 1;

//	DWL::DWLString Filtro = IDIOMA__ARCHIVOS_REPRODUCIBLES_ASF_AVI_DIVX_DV_FLV_GXF_M1V;
	DWL::Ventanas::DWLDialogoAbrir Dlg(Sistema.App.Config.Abrir_Path(), Tipos, IDIOMA__ABRIR_ARCHIVO);
	unsigned int Total = Dlg.TotalArchivos();
	if (Total == 0) return;
	// Obtengo el path para asignarlo a la config
	DWL::DWLString PathFinal = Dlg.Archivos(0);
	//size_t TamPath = DWLStrLen(PathFinal);
	for (size_t z = PathFinal.Tam(); z > 0; z--) {
		if (PathFinal[z] == TEXT('\\')) {
			PathFinal[z + 1] = 0;
			break;
		}
	}
	Sistema.App.Config.Abrir_Path = PathFinal;
	
	bool NL = false;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	// Añado los archivos nuevos a la lista
//	Sistema.App.Media.EliminarMedioActual();
//	Sistema.App.ListaC.EliminarTodosLosMedios();
	for (unsigned int i = 0; i < Total; i++) AgregarArchivoLista(Dlg.Archivos(i));
	Sistema.App.ListaC.NombreLista = IDIOMA__MODIFICADA;
	Sistema.App.ListaC.ActualizarTodo();
	if (Sistema.App.Config.Shufle == true) Sistema.App.ListaC.Shufle(true);
	if (NL == true) Sistema.App.Media.AbrirPrimerMedioLista();	

//	Sistema.App.BD.RepararAnomaliasMediosAudio();
	Sistema.App.BD.ActualizarArbol();
}


// Esta funcion recibe archivos que no tienen que estar en la BD necesariamente.
// Si es audio/cdaudio se identificara el medio con InformacionArchivoEx
// Si es video se añadira directamente el medio a la BD, de forma que quede pendiente de analisis al ser reproducido. (no quiero esperar 1 segundo de analisis si puede ser reproducido directamente)
// Si es una lista sera necesario un parsing para obtener sus medios, luego se analizaran los medios que no esten en la base de datos
// Por ultimo se puede seleccionar archivos que no entren dentro de la lista de tipos de archivo, pero se mostrara un mensaje de que al hacer esto es posible que el reproductor pueda llegar a quedar bloqueado o incluso llegue a petar.
// OGG y ASF sera escaneado con informacionEx, los demas videos no seran escaneados
ItemListaMedios *VentanaPlayerEx::AgregarArchivoLista(const TCHAR *Path) {
	TCHAR PathCorto[512];
	ZeroMemory(PathCorto, sizeof(TCHAR) * 512);

	// Comprobamos que es un path válido
	GetShortPathName(Path, PathCorto, 512);
	if (PathCorto[0] == TEXT('\0')) return NULL;


	if (DWLStrCmpi(Path, TEXT("-r")) == 0) {
		Sistema.App.Media.EliminarMedioActual();
		Sistema.App.ListaC.EliminarTodosLosMedios();
		return NULL;
	}
	// Buscamos el medio en la BD
	for (size_t n = 0; n < Sistema.App.BD.Medios.size(); n++) {
		if (Sistema.App.BD.Medios[n]->Path == Path) {
			if (Sistema.App.BD.Medios[n]->TipoMedio == Tipo_Medio_Lista) {
				ParsearM3U(Path, false);
				return NULL;
			}
			else {
				return Sistema.App.ListaC.AgregarMedio(Sistema.App.BD.Medios[n]);
			}
		}
	}

	// No está en la BD
	TipoArchivo_Datos *Datos = Sistema.App.Tipos.Extension(Path);
	Tipo_Medio         Tipo;
	if (Datos == NULL)  Tipo = Tipo_Medio_Video;
	else				Tipo = Datos->Tipo;

	InformacionArchivoEx  Info;
	ItemListaMedios      *ItemMedio = NULL;
	TMedioVideo          *nVideo = NULL;
	TMedio               *nMedio = NULL;
	switch (Tipo) {
		case Tipo_Medio_Lista :
			// parsear m3u
			// buscar medios, y en caso de no existir en la bd, analizarlos y añadirlos
			ParsearM3U(Path, true);
			break;
		/*case Tipo_Medio_Video :
		case Tipo_Medio_Audio :
		case Tipo_Medio_CDAudio :*/
		default :
			nMedio = Info.ObtenerInfo(Path, Sistema.App.Tipos, Sistema.App.UnidadesDisco, Sistema.App.Config.Buscar_Paths, Sistema.App.VLC.InstanciaVLC(), Sistema.App.FMod.Sistema_FMOD());
			if (nMedio != NULL) {
				if (nMedio->Longitud != 0)	
					return Sistema.App.ListaC.AgregarMedio(Sistema.App.BD.AgregarMedioBD(nMedio));
			}
			break;
	}
	return NULL;
}


void VentanaPlayerEx::ParsearM3U(const TCHAR *Path, const bool AgregarM3UaBD) {
	InformacionArchivoEx  Info;
	TMedio               *nMedio = NULL;
	// Abro y leo todo el M3U
	DWL::Archivos::DWLArchivoBinario	Archivo;
	Archivo.AbrirArchivoLectura(Path);
	DWORD Tam = Archivo.Tam();
	char *CharArchivo = new char [Tam + 1];
	CharArchivo[Tam] = 0;
	Archivo.Leer(CharArchivo, Tam);
	DWL::DWLString TCharArchivo(CharArchivo);

	size_t i = 0;
	// Obtengo el directorio de donde proviene el M3U
	DWL::DWLString DirectorioM3U;
	UINT TamPath = DWLStrLen(Path);
	for (i = TamPath; i != 0; i--) {
		if (Path[i] == TEXT('\\')) {
			DirectorioM3U.Borrar();
			DirectorioM3U.AgregarTexto(Path, i + 1);
			break;
		}
	}

	DWL::DWLString TmpMedio;
	DWL::DWLTexto_Separado LineasTexto(TCharArchivo, TEXT('\n'));

	// Obtengo los paths del m3u
	for (i = 0; i < LineasTexto.TotalTextos(); i++) {
		if (LineasTexto[i].Caracter(0) != TEXT('#')) { // linea valida
			if (LineasTexto[i].Caracter(1) == TEXT(':')) { // Ruta absoluta
				TmpMedio = LineasTexto[i];
			}
			else { // ruta relativa
				TmpMedio = DirectorioM3U;
				TmpMedio += LineasTexto[i];
			}
			// Buscamos el medio en la BD
			nMedio = Sistema.App.BD.BuscarPath(TmpMedio());
			if (nMedio == NULL) {
				nMedio = Info.ObtenerInfo(TmpMedio(), Sistema.App.Tipos, Sistema.App.UnidadesDisco, Sistema.App.Config.Buscar_Paths, Sistema.App.VLC.InstanciaVLC(), Sistema.App.FMod.Sistema_FMOD());
				if (nMedio != NULL) {
					nMedio = Sistema.App.BD.AgregarMedioBD(nMedio);
					if (nMedio != NULL)	Sistema.App.ListaC.AgregarMedio(nMedio);
				}
			}
			else {
				Sistema.App.ListaC.AgregarMedio(nMedio);
			}


		}
	}

	if (AgregarM3UaBD == true) {
		// Agregamos el M3u a la bd
		nMedio = Info.ObtenerInfo(Path, Sistema.App.Tipos, Sistema.App.UnidadesDisco, Sistema.App.Config.Buscar_Paths, Sistema.App.VLC.InstanciaVLC(), Sistema.App.FMod.Sistema_FMOD());
		if (nMedio != NULL)	Sistema.App.BD.AgregarMedioBD(nMedio);
	}
}


void VentanaPlayerEx::Titulo(const TCHAR *nTitulo) {
	SetWindowText(_hWnd, nTitulo);
	PintarTitulo(true, NULL);
}


LRESULT VentanaPlayerEx::Evento_Temporizador(const UINT cID) {
	static bool Blink   = false;
	UINT		nAncho  = 0;
	UINT		nAlto   = 0;
	UINT64		nTiempo = 0;
	switch (cID) {


		case TIMER_LISTA :
			Evento_TemporizadorLista();
			break;
		case TIMER_BARRATIEMPO : 
			if (Sistema.App.ControlesMP.SliderTiempo.Estado() != DWL_Enum_BarraEx_Estados_Presionado) {
				Sistema.App.ControlesMP.SliderTiempo.Valor(static_cast<long>(Sistema.App.Media.TiempoActual())); 
			}
			if (Sistema.App.SliderTiempo.Estado() != DWL_Enum_BarraEx_Estados_Presionado) {
				Sistema.App.SliderTiempo.Valor(static_cast<long>(Sistema.App.Media.TiempoActual())); 
			}
			break;
		case TIMER_BLINK :
			if (GetActiveWindow() != _hWnd) break;
			if (Sistema.App.Media.MedioActual != NULL) {
				if (Sistema.App.Media.MedioActual->Medio != NULL) {
					if (Sistema.App.MediaPlayer.Visible() == FALSE && Sistema.App.Media.MedioActual->Medio->TipoMedio == Tipo_Medio_Video && Sistema.App.Media.ComprobarEstado() == EnPlay) {
						Blink = !Blink;
						if (Blink == false)	{
							Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Colores()->TextoNormal.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Normal);
							Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Colores()->TextoResaltado.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Resaltado);
						}
						else {
							Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Colores()->TextoNormal = RGB(175, 0, 0);
							Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Colores()->TextoNormal = RGB(175, 0, 0);
						}
						Sistema.App.PlayerEx.OpcionesBasicas.Repintar(true);
					}
					else {
						Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Colores()->TextoNormal.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Normal);
						Sistema.App.PlayerEx.OpcionesBasicas.Nodo(OPCIONES_BASICAS_VER_VIDEO)->Colores()->TextoResaltado.Enlazar(&DWL::SO::DWLEstilos::Colores.ContenedorEx_Texto_Resaltado);
					}
				}
			}
			break;
	}
	VentanaEx::Evento_Temporizador(cID);
    return 0;
}


LRESULT VentanaPlayerEx::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
//	TextoCancion.Evento_Mouse_BotonPresionado(Boton, cX, cY, wParam);
	return VentanaEx::Evento_Mouse_BotonPresionado(Boton, cX, cY, Param);
	// Mirar WM_XBUTTONDOWN UP i tal per pulsacions de butons extras del mouse
}


LRESULT VentanaPlayerEx::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
//	TextoCancion.Evento_Mouse_BotonSoltado(Boton, cX, cY, wParam);
	VentanaEx::Evento_Mouse_BotonSoltado(Boton, cX, cY, Param);
	if (Boton == 1) MenusPlayer.Mostrar(_hWnd);
    return 0;
}


LRESULT VentanaPlayerEx::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
//	TextoCancion.Evento_Mouse_Movimiento(cX, cY, wParam);
	return VentanaEx::Evento_Mouse_Movimiento(cX, cY, Param);
}


void VentanaPlayerEx::CrearMenus(void) {
	MenusPlayer.BorrarTodo();
	MenusPlayer.Texto(APP_NOMBRE);
	MenusPlayer.AgregarMenu(IDSM_ATRAS,		IDIOMA__CANCION_ATRAS	, IDI_CANCION_ATRAS);		// Cancion Atras
	MenusPlayer.AgregarMenu(IDSM_PLAY,	    IDIOMA__PLAY			, IDI_CANCION_PLAY);		// Play
	MenusPlayer.AgregarMenu(IDSM_PAUSA,	    IDIOMA__PAUSA			, IDI_CANCION_PAUSA);		// Pausa
	MenusPlayer.AgregarMenu(IDSM_STOP,	    IDIOMA__STOP			, IDI_CANCION_STOP);		// Stop
	MenusPlayer.AgregarMenu(IDSM_ADELANTE,  IDIOMA__CANCION_ADELANTE, IDI_CANCION_ADELANTE);	// Cancion Adelante
	MenusPlayer.AgregarSeparador() ;

	MenuShufle = static_cast<PlayerMenuEx *>(MenusPlayer.AgregarMenu(IDMG_SHUFLE, IDIOMA__SHUFLE, IDI_CANCION_SHUFLE)); // Shufle
	MenuShufle->AgregarMenu(IDMS_SHUFLESI,		IDIOMA__ACTIVADO	, 0);				// Activado
	MenuShufle->AgregarMenu(IDMS_SHUFLENO,		IDIOMA__DESACTIVADO	, 0);				// Desactivado
	if (Sistema.App.Config.Shufle == true)  MenuShufle->Menu(0)->Icono(IDI_MARCA_MENU);
	else                            MenuShufle->Menu(1)->Icono(IDI_MARCA_MENU); 

	MenuRepeat = static_cast<PlayerMenuEx *>(MenusPlayer.AgregarMenu(IDMG_REPEAT, IDIOMA__REPEAT, IDI_CANCION_REPEAT)); // Repeat
	MenuRepeat->AgregarMenu(IDMR_DESACTIVADO,	IDIOMA__DESACTIVADO);					// Desactivado
	MenuRepeat->AgregarMenu(IDMR_NORMAL,		IDIOMA__REPETIR_LISTA);					// Repetir Lista
	MenuRepeat->AgregarMenu(IDMR_NORMALSH,		IDIOMA__REPETIR_LISTA_SHUFLE);			// Repetir Lista + Shufle
	MenuRepeat->AgregarSeparador();
	MenuRepeat->AgregarMenu(IDMR_GENERO,		IDIOMA__GENERO_ALEATORIO);				// Genero Aleatorio
	MenuRepeat->AgregarMenu(IDMR_GRUPO,			IDIOMA__GRUPO_ALEATORIO);				// Grupo Aleatorio
	MenuRepeat->AgregarMenu(IDMR_DISCO,			IDIOMA__DISCO_ALEATORIO);				// Disco Aleatorio
	MenuRepeat->AgregarMenu(IDMR_CANCION,		IDIOMA__50_CANCIONES_ALEATORIAS);		// 50 Canciones Aleatorias
	MenuRepeat->AgregarSeparador();
	MenuRepeat->AgregarMenu(IDMR_APAGAR,		IDIOMA__APAGAR_WINDOWS);				// Apagar Windows
	MenuRepeat->AgregarMenu(IDMR_APAGARR,		IDIOMA__APAGAR_ESTE_REPRODUCTOR);		// Apagar este Reproductor
	MenuRepeat->Menu(Sistema.App.Config.Repeat)->Icono(IDI_MARCA_MENU);

	MenusPlayer.AgregarSeparador();
	MenuBotonBD       = static_cast<PlayerMenuEx *>(MenusPlayer.AgregarMenu(1, IDIOMA__BASE_DE_DATOS, IDI_BDV)); // Base de Datos
	MenuBotonBD->AgregarMenu(IDMBBD_BUSCAR,		IDIOMA__BUSCAR_ARCHIVOS_A_AGREGAR	, IDI_BD_BUSCAR); // Buscar Archivos a agregar
	MenuBotonBD->AgregarMenu(IDMBBD_REVISAR,	IDIOMA__REVISAR_LA_BASE_DE_DATOS	, IDI_BD_REVISAR); // Revisar la Base de Datos
	
	MenuBotonLista    = static_cast<PlayerMenuEx *>(MenusPlayer.AgregarMenu(2, IDIOMA__LISTA, IDI_LISTA)); // Lista
	
	MenuBotonLista->AgregarMenu(IDMBL_NUEVALISTA,				IDIOMA__NUEVA_LISTA					, IDI_LISTA_VACIA);		// Nueva Lista
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_NARCHIVO,		IDIOMA__ARCHIVO						, IDI_BD_SELECCIONAR);	// -Archivo
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_LISTAVACIA,		IDIOMA__LISTA_VACIA					, IDI_LISTA_VACIA);		// -Lista Vacia
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_NGEN,			IDIOMA__GENERO_ALEATORIO			, IDI_GENERO);			// -Genero Aleatorio
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_NGRP,			IDIOMA__GRUPO_ALEATORIO				, IDI_GRUPO);			// -Grupo Aleatorio
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_NDSK,			IDIOMA__DISCO_ALEATORIO				, IDI_DISCO);			// -Disco Aleatorio
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_NCAN,			IDIOMA__50_CANCIONES_ALEATORIAS		, IDI_AUDIO);			// -Canciones Aleatorias
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_N5E,				IDIOMA__CANCIONES_CON_5_ESTRELLAS	, IDI_ESTRELLA2);		// -Canciones con 5 estrellas
	MenuBotonLista->Menu(0)->AgregarMenu(IDMBL_NMAS_ESCUCHADAS, IDIOMA__CANCIONES_MAS_ESCUCHADAS	, IDI_INICIO2);			// -Canciones con 5 estrellas
	MenuBotonLista->AgregarMenu(IDMBL_ALISTA,					IDIOMA__AÑADIR_A_LISTA				, IDI_LISTA_LLENA);		// Añadir a Lista
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_ARCHIVO,			IDIOMA__ARCHIVO						, IDI_BD_SELECCIONAR);	// -Archivo
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_GEN,				IDIOMA__GENERO_ALEATORIO			, IDI_GENERO);			// -Genero Aleatorio
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_GRP,				IDIOMA__GRUPO_ALEATORIO				, IDI_GRUPO);			// -Grupo Aleatorio
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_DSK,				IDIOMA__DISCO_ALEATORIO				, IDI_DISCO);			// -Disco Aleatorio
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_CAN,				IDIOMA__50_CANCIONES_ALEATORIAS		, IDI_AUDIO);			// -Canciones Aleatorias
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_5E,				IDIOMA__CANCIONES_CON_5_ESTRELLAS	, IDI_ESTRELLA2);		// -Canciones con 5 estrellas
	MenuBotonLista->Menu(1)->AgregarMenu(IDMBL_MAS_ESCUCHADAS,	IDIOMA__CANCIONES_MAS_ESCUCHADAS	, IDI_INICIO2);			// -Canciones con 5 estrellas
	MenuBotonLista->AgregarMenu(IDMBL_GUARDAR,					IDIOMA__GUARDAR_LISTA				, IDI_GUARDAR , -1, false, false); // Guardar Lista
	ActualizarMenusGenero();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Video
	//////////////////////////////////////////////////////////////////////////////////////////////////
	MenuVideo = static_cast<PlayerMenuEx *>(MenusPlayer.AgregarMenu(2, IDIOMA__VIDEO, IDI_VIDEO));	// Video
	MenuVideo->AgregarMenu(IDMV_SUBTITULOS, IDIOMA__CARGAR_ARCHIVO_DE_SUBTITULOS, IDI_GUARDAR);		// Cargar archivo de Subtitulos
//	MenuVideo->AgregarMenu(IDMV_MODO_VIDEO, TEXT("Modo de video")				, IDI_LISTA_VACIA, false, false); // Subtitulos
	//////////////////////////////////////////////////////////////////////////////////////////////////



	MenusPlayer.AgregarSeparador();
	MenusPlayer.AgregarMenu(SC_RESTORE,		IDIOMA__RESTAURAR	, IDI_RESTAURAR_N, -1, true); // Restaurar
	MenusPlayer.AgregarMenu(SC_MINIMIZE,	IDIOMA__MINIMIZAR	, IDI_MINIMIZAR_N, -1, true); // Minimizar
	MenusPlayer.AgregarMenu(SC_CLOSE,		IDIOMA__CERRAR		, IDI_CERRAR_N, -1, true);	// Cerrar
	//////////////////////////////////////////////////////////////////////////////////////////


	MenusOcultos.BorrarTodo();
	MenuBDA				= static_cast<PlayerMenuEx *>(MenusOcultos.AgregarMenu(10, IDIOMA__BASE_DE_DATOS));							// Base de Datos
	MenuBDA->AgregarMenu(IDMBDA_AGREGAR_LISTA,			IDIOMA__AÑADIR_A_LISTA				, IDI_LISTA_LLENA);						// 0 (Añadir a la Lista)
	MenuBDA->AgregarMenu(IDMBDA_AGREGAR_NUEVA_LISTA,	IDIOMA__AÑADIR_A_UNA_NUEVA_LISTA	, IDI_LISTA_VACIA);						// 1 (Añadir a una nueva Lista)
	MenuBDA->AgregarMenu(IDMBDA_ELIMINAR,				IDIOMA__ELIMINAR					, IDI_ELIMINAR);						// 2 (Eliminar)
	MenuBDA->AgregarMenu(IDMBDA_MODIFICAR,				IDIOMA__MODIFICAR					, IDI_LISTA_MODIFICARNOMBRE);			// 3 (Modificar)
	MenuBDA->AgregarMenu(IDMBDA_NOTA,					IDIOMA__NOTA						, IDI_ESTRELLA2);						// 4 (Nota)
	MenuBDA->Menu(4)->AgregarMenu(IDMBDA_N5,			IDIOMA__5_ESTRELLAS					);										// 4.0 (5 Estrellas)
	MenuBDA->Menu(4)->AgregarMenu(IDMBDA_N4,			IDIOMA__4_ESTRELLAS					);										// 4.1 (4 Estrellas)
	MenuBDA->Menu(4)->AgregarMenu(IDMBDA_N3,			IDIOMA__3_ESTRELLAS					);										// 4.2 (3 Estrellas)
	MenuBDA->Menu(4)->AgregarMenu(IDMBDA_N2,			IDIOMA__2_ESTRELLAS					);										// 4.3 (2 Estrellas)
	MenuBDA->Menu(4)->AgregarMenu(IDMBDA_N1,			IDIOMA__1_ESTRELLA					);										// 4.4 (1 Estrella)
	MenuBDA->AgregarMenu(IDMBDA_ORDEN,					IDIOMA__ORDEN_DE_LAS_CANCIONES		, IDI_ORDEN);							// 5 (Orden de las Canciones)
	MenuBDA->Menu(5)->AgregarMenu(IDMBDA_TGGD,			IDIOMA__TIPO_GENERO_GRUPO_DISCO_CANCIONES);									// 5.0 (Tipo, Genero, Grupo, Disco, Canciones)
	MenuBDA->Menu(5)->AgregarMenu(IDMBDA_GGD,			IDIOMA__GENERO_GRUPO_DISCO_CANCIONES);										// 5.1 (Genero, Grupo, Disco, Canciones)
	MenuBDA->Menu(5)->AgregarMenu(IDMBDA_GD,			IDIOMA__GRUPO_DISCO_CANCIONES		);										// 5.2 (Grupo, Disco, Canciones)
	MenuBDA->Menu(5)->AgregarMenu(IDMBDA_D,				IDIOMA__DISCO_CANCIONES				);										// 5.3 (Disco, Canciones)
	MenuBDA->Menu(5)->Menu(static_cast<int>(Sistema.App.Config.OrdenBD))->Icono(IDI_MARCA_MENU);
	MenuBDA->AgregarSeparador();
	MenuBDA->AgregarMenu(IDMBDA_ABRIRDIR,				IDIOMA__ABRIR_SU_DIRECTORIO			, IDI_BD_SELECCIONAR, -1, false, false);// 6 (Abrir su directorio)
	MenuBDA->AgregarMenu(IDMBDA_PROPIS,					IDIOMA__PROPIEDADES					, IDI_RESTAURAR_N, -1, false, false);	// 7 (Propiedades)


	MenuLista			= static_cast<PlayerMenuEx *>(MenusOcultos.AgregarMenu(11, IDIOMA__LISTA));									// Lista
	MenuLista->AgregarMenu(IDMLI_BUSCARBD,		IDIOMA__BUSCAR_CANCION_EN_LA_BASE_DE_DATOS	, IDI_BD_BUSCAR);						// Buscar en la BD
	MenuLista->AgregarMenu(IDMLI_MPISTA,		IDIOMA__MODIFICAR_PISTA						, IDI_LISTA_MODIFICARPISTA);			// Modificar Pista
	MenuLista->AgregarMenu(IDMLI_MNOMBRE,		IDIOMA__MODIFICAR_NOMBRE					, IDI_LISTA_MODIFICARNOMBRE);			// Modificar Nombre
	MenuLista->AgregarMenu(IDMLI_ELIMINAR,		IDIOMA__ELIMINAR							, IDI_ELIMINAR);						// Eliminar
	MenuLista->AgregarMenu(IDMLI_NOTA,			IDIOMA__NOTA								, IDI_ESTRELLA2);						// Nota
	MenuLista->Menu(4)->AgregarMenu(IDMLI_N5,	IDIOMA__5_ESTRELLAS							);										// 5 Estrellas
	MenuLista->Menu(4)->AgregarMenu(IDMLI_N4,	IDIOMA__4_ESTRELLAS							);										// 4 Estrellas
	MenuLista->Menu(4)->AgregarMenu(IDMLI_N3,	IDIOMA__3_ESTRELLAS							);										// 3 Estrellas
	MenuLista->Menu(4)->AgregarMenu(IDMLI_N2,	IDIOMA__2_ESTRELLAS							);										// 2 Estrellas
	MenuLista->Menu(4)->AgregarMenu(IDMLI_N1,	IDIOMA__1_ESTRELLA							);										// 1 Estrella
	MenuLista->AgregarMenu(IDMLI_COPIAR,		IDIOMA__COPIAR_CANCIONES_A					, IDI_COPIAR, -1, false, false);		// Copiar Canciones a..
	MenuLista->AgregarMenu(IDMLI_ABRIRDIR,		IDIOMA__ABRIR_SU_DIRECTORIO					, IDI_BD_SELECCIONAR, -1, false, false);// Abrir su directorio
	MenuLista->AgregarMenu(IDMLI_PROPIS,		IDIOMA__PROPIEDADES							, IDI_RESTAURAR_N, -1, false, false);	// Propiedades

	MenuOscyTxt			= static_cast<PlayerMenuEx *>(MenusOcultos.AgregarMenu(13, IDIOMA__CANCION_ACTUAL));									// Canción Actual
	MenuOscyTxt->AgregarMenu(IDMOT_BUSCARBD,				IDIOMA__BUSCAR_CANCION_EN_LA_BASE_DE_DATOS	, IDI_BD_BUSCAR);						// Buscar cancion en la Base de datos
	MenuOscyTxt->AgregarMenu(IDMOT_BUSCARLISTA,				IDIOMA__BUSCAR_CANCION_EN_LA_LISTA			, IDI_LISTA_BUSCAR);					// Buscar cancion en la Lista
	MenuOscyTxt->AgregarMenu(IDMOT_ELIMINAR,				IDIOMA__ELIMINAR							, IDI_ELIMINAR);						// Eliminar
	MenuOscyTxt->Menu(2)->AgregarMenu(IDMOT_ELIMINARBD,		IDIOMA__ELIMINAR_DE_LA_BASE_DE_DATOS		, IDI_BD);								// Eliminar de la BD
	MenuOscyTxt->Menu(2)->AgregarMenu(IDMOT_ELIMINARLISTA,	IDIOMA__ELIMINAR_DE_LA_LISTA				, IDI_LISTA);							// Eliminar de la Lista
	MenuOscyTxt->AgregarMenu(IDMOT_NOTA,					IDIOMA__NOTA								, IDI_ESTRELLA2);						// Nota
	MenuOscyTxt->Menu(3)->AgregarMenu(IDMOT_NOTA5,			IDIOMA__5_ESTRELLAS							);										// 5 Estrellas
	MenuOscyTxt->Menu(3)->AgregarMenu(IDMOT_NOTA4,			IDIOMA__4_ESTRELLAS							);										// 4 Estrellas
	MenuOscyTxt->Menu(3)->AgregarMenu(IDMOT_NOTA3,			IDIOMA__3_ESTRELLAS							);										// 3 Estrellas
	MenuOscyTxt->Menu(3)->AgregarMenu(IDMOT_NOTA2,			IDIOMA__2_ESTRELLAS							);										// 2 Estrellas
	MenuOscyTxt->Menu(3)->AgregarMenu(IDMOT_NOTA1,			IDIOMA__1_ESTRELLA							);										// 1 Estrella
	MenuOscyTxt->AgregarMenu(IDMOT_PROPIEDADES,				IDIOMA__PROPIEDADES							, IDI_RESTAURAR_N, -1, false, false);	// Propiedades
}


class AMG {
  public:
					AMG(void) { };
					AMG(DWL::DWLString &nNombre, UINT nID) : Nombre(nNombre), ID(nID) { };
	DWL::DWLString  Nombre;
	UINT            ID;
};

void VentanaPlayerEx::ActualizarMenusGenero(void) {
	// Creamos una lista de generos ordenada alfabeticamente en memoria
	std::vector<AMG *> ListaGeneros;
	AMG               *AMGenero = NULL;
	bool               Insertado = false;

	// Primero creamos la lista des-ordenada
	size_t i;
	for (i = 0; i < Sistema.App.BD.StrGeneros.size(); i++) {
		AMGenero = new AMG(Sistema.App.BD.StrGeneros[i]->Nombre, MENUGEN + i);
		Insertado = false;
		if (ListaGeneros.size() != 0) {
			for (size_t z = 0; z < ListaGeneros.size(); z++) {
				if (DWLStrCmpi(Sistema.App.BD.StrGeneros[i]->Nombre(), ListaGeneros[z]->Nombre()) < 0) {
					ListaGeneros.insert(ListaGeneros.begin() + z, AMGenero);
					Insertado = true;
					break;
				}
			}
		}
		if (Insertado == false) ListaGeneros.push_back(AMGenero);
	}
	
	MenuBotonLista->Menu(0)->Menu(2)->BorrarTodo();
	MenuBotonLista->Menu(1)->Menu(1)->BorrarTodo();
	MenuBotonLista->Menu(0)->Menu(2)->AgregarMenu(4999, IDIOMA__ALEATORIO, IDI_AYUDA); // Aleatorio
	MenuBotonLista->Menu(0)->Menu(2)->AgregarSeparador();
	MenuBotonLista->Menu(1)->Menu(1)->AgregarMenu(5999, IDIOMA__ALEATORIO, IDI_AYUDA); // Aleatorio
	MenuBotonLista->Menu(1)->Menu(1)->AgregarSeparador();

	for (i = 0; i < ListaGeneros.size(); i++) {
		MenuBotonLista->Menu(0)->Menu(2)->AgregarMenu(ListaGeneros[i]->ID, ListaGeneros[i]->Nombre(), IDI_GENERO);
		MenuBotonLista->Menu(1)->Menu(1)->AgregarMenu(ListaGeneros[i]->ID + 1000, ListaGeneros[i]->Nombre(), IDI_GENERO);
	}	 
}


void VentanaPlayerEx::MostrarMenuBDA(void) {
	// -'Añadir a la lista' y 'Añadir a nueva lista' :
	//		Son posibles siempre que exista algun nodo selecionado y este contenga canciones
	// -'Eliminar' :
	//		Es posible mientras exista algun nodo selecionado y este/estos no sean la raiz de una unidad de CDRom
	// -'Modificar' :
	//		Solo deberia ser posible si hay un nodo MARCADO (un solo nodo).
	// -'Nota' :
	//		Sempre activo, se tiene que calcular la media antes de mostrar.
	// -'Orden de las canciones' :
	//		Siempre debe estar activado
	// -'Propiedades' :
	//		Solo si todos los nodos son canciones
	float				NotaFinal					= 0.0f;
	int					TotalMedios					= 0;	
	int					TotalNodosSeleccionados		= 0;
	int					TotalNodosSubSeleccionados	= 0;
	bool				MostrarPropiedades			= true;
	bool				MostrarModificar			= true;
	bool				MostrarEliminar				= true;
	bool                MostrarAbrirDir             = true;
	bool                MostrarNota		            = true;
	NodoArbolBaseDatos *NodoR						= NULL;
	DWL::DWLString		TxtNota;
//	Sistema.App.ArbolBaseDatos.EliminarTemporizador(555);
	MenuBDA->Menu(0)->Activado(false); //					Añadir a lista
	MenuBDA->Menu(1)->Activado(false); //					Añadir a nueva lista
	MenuBDA->Menu(2)->Activado(false); //					Eliminar
	MenuBDA->Menu(3)->Activado(false); //					Modificar
	MenuBDA->Menu(4)->Activado(false); //					Nota
				 // (5)										Orden de las canciones
	MenuBDA->Menu(7)->Activado(false); //					Abrir su directorio
	MenuBDA->Menu(8)->Activado(false); //					Propiedades

	// Recuento de todos los datos necesarios :
	// - Total Canciones Seleccionadas
	// - Total Canciones SubSeleccionadas
	// - Calculo de la nota media.
	if (Sistema.App.ArbolBaseDatos.NodoMarcado() != NULL) {
		for (unsigned int i = 0; i < Sistema.App.ArbolBaseDatos.TotalNodosLineales(); i ++) {
			if (Sistema.App.ArbolBaseDatos.NodoLinealMedio(i)->Seleccionado() == true) {
				TotalNodosSeleccionados ++;
				NodoR = Sistema.App.ArbolBaseDatos.NodoLinealMedio(i);
				if (NodoR->SubSeleccionado() == true) TotalNodosSubSeleccionados ++;
				switch (NodoR->Icono()) {
					case IDI_TIPO :	case IDI_GENERO : case IDI_GRUPO : case IDI_DISCO : 
						MostrarPropiedades	= false;
						MostrarAbrirDir		= false;
						break;
					case IDI_BDA  :	case IDI_BDV  : case IDI_BD_SELECCIONAR : 
						MostrarPropiedades	= false;
						MostrarModificar	= false;
						MostrarAbrirDir		= false;
						break;
					case IDI_CDAUDIO :
						MostrarPropiedades	= false;
						MostrarModificar	= false;
						MostrarEliminar		= false;
						MostrarAbrirDir		= false;
						break;
					case IDI_LISTA_MODIFICARPISTA :
						MostrarPropiedades	= false;
						MostrarModificar	= false;
						MostrarEliminar		= false;
						MostrarAbrirDir		= false;
						MostrarNota			= false;
						break;
					case IDI_VIDEO :
					case IDI_AUDIO :
					case IDI_PLAY :
					case IDI_PISTAAUDIO :
						TotalMedios ++;
						NotaFinal += NodoR->Medio->Nota;
						// Si es una pista de audio desactivo el eliminar
						if (NodoR->Medio->TipoMedio == Tipo_Medio_CDAudio)	MostrarEliminar	= false;
						break;
				}
			}
		}
		if (TotalMedios > 0) {
			MenuBDA->Menu(0)->Activado(true);  // Añadir a lista
			MenuBDA->Menu(1)->Activado(true);  // Añadir a nueva lista
		}
		// Miro el submenu eliminar
		if (MostrarEliminar == true) MenuBDA->Menu(2)->Activado(true);  // Eliminar

		// Miro el submenu modificar
		if (TotalNodosSeleccionados - TotalNodosSubSeleccionados != 1) MostrarModificar = false;
		if (MostrarModificar == true) MenuBDA->Menu(3)->Activado(true); // Modificar
		// Miro el submenu nota
		if (MostrarNota == true && TotalMedios > 0) MenuBDA->Menu(4)->Activado(true); // Nota 
		MenuBDA->Menu(4)->Texto(IDIOMA__NOTA);
		if (TotalMedios > 0) {
			int NotaFinalInt = static_cast<int>(NotaFinal / TotalMedios);
			for (int i2 = 0; i2 < 5; i2++)  MenuBDA->Menu(4)->Menu(i2)->Icono(NULL); 
			if (NotaFinalInt > 0 && NotaFinalInt < 6) { MenuBDA->BuscarID((IDMBDA_N1 -1) + NotaFinalInt)->Icono(IDI_MARCA_MENU); }
//			MenuBDA->Menu(4)->Activado(true); // Nota 
			TxtNota.sprintf(TEXT("%s [%.02f]"), IDIOMA__NOTA, NotaFinal / static_cast<float>(TotalMedios));
			MenuBDA->Menu(4)->Texto(TxtNota());
		}
		// Miro el menu para abrir el directorio
		if (MostrarAbrirDir == true) MenuBDA->Menu(7)->Activado(true); 
		// Miro el submenu propiedades
		//if (MostrarPropiedades == true) MenuBDA->Menu(8)->Activado(true); // Propiedades (DESMARCAR CUANDO SE PROGRAMEN LAS PROPIEDADES)
	}
	
	MenuBDA->Mostrar(_hWnd);
}


void VentanaPlayerEx::SoltarArchivos(WPARAM wParam) {
	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_Procesando);
	// Si se esta buscando archivos NO SE PUEDE AGREGAR NADA EXTERNO o la liaremos xd
	if (Sistema.App.PlayerEx.CP.BuscarArchivos != NULL) {
		static VentanaMensaje_Aceptar MsgAceptar;
		MsgAceptar.MostrarMensaje(IDIOMA__NO_PUEDES_ANADIR_MEDIOS_EXTERNAMENTE_MIENTRAS_SE_ESTA_REALIZANDO_UNA_BUSQUEDA, IDIOMA__ERROR);
		return;
	}
	
	bool ListaNueva = false;
	if (Sistema.App.ListaC.TotalItems() == 0) ListaNueva = true;
	TCHAR        Archivo[MAX_PATH];
	unsigned int z = 0;

	unsigned int TotalSoltados = DragQueryFile((HDROP)wParam, static_cast<unsigned int>(-1), 0, 0);
	for (unsigned int i = 0; i < TotalSoltados; i++) {
		DragQueryFile((HDROP)wParam, i, Archivo, MAX_PATH);
		if (FILE_ATTRIBUTE_DIRECTORY & GetFileAttributes(Archivo))	SoltarArchivos_BuscarArchivos(Archivo);
		else														SoltarArchivos_AgregarArchivo(Archivo);
	}
	DragFinish((HDROP)wParam);

	Sistema.App.ListaC.ActualizarTodo();
	if (ListaNueva == true) Sistema.App.Media.AbrirPrimerMedioLista();
	DWL::SO::DWLMouse::CambiarCursorEstandard(DWL_Enum_Mouse_Cursor_Flecha);
}


void VentanaPlayerEx::SoltarArchivos_AgregarArchivo(const TCHAR *Archivo) {
	TMedio *Ret = Sistema.App.BD.BuscarPath(Archivo);
	if (Ret == NULL) {
		InformacionArchivoEx Info;
		TipoArchivo_Datos *DatosArchivo;
		ItemListaMedios *MedioActual = NULL;
		Ret = Info.ObtenerInfo(Archivo, Sistema.App.Tipos, Sistema.App.UnidadesDisco, Sistema.App.Config.Buscar_Paths, Sistema.App.VLC.InstanciaVLC(), Sistema.App.FMod.Sistema_FMOD());
		if (Ret != NULL) {
			switch (Ret->TipoMedio) {
				case Tipo_Medio_Subtitulos :
					MedioActual = Sistema.App.Media.MedioActual;
					if (MedioActual != NULL) {
						if (MedioActual->Medio->TipoMedio == Tipo_Medio_Video) {
							if (Sistema.App.Media.ComprobarEstado() == EnPlay) {
								Sistema.App.VLC.Asignar_Subtitulos_MedioActual(Archivo);
							}
						}
					}
					break;
				case Tipo_Medio_Lista :
					Ret = Sistema.App.BD.AgregarMedioBD(Ret);
					ParsearM3U(Ret->Path(), false);
					break;
				case Tipo_Medio_Audio :
				case Tipo_Medio_CDAudio :
					Ret = Sistema.App.BD.AgregarMedioBD(Ret);
					Sistema.App.ListaC.AgregarMedio(Ret);
					break;
				case Tipo_Medio_Video :
					DatosArchivo = Sistema.App.Tipos.BuscarTipoArchivo(Ret->Path());
					if (DatosArchivo != NULL) {
						if (DatosArchivo->Tipo == Tipo_Medio_Video) { 
							Sistema.App.BD.AgregarMedioBD(Ret);
							Sistema.App.ListaC.AgregarMedio(Ret);
							return;
						}
					}
					delete static_cast<TMedioVideo *>(Ret);
					break;
			}
		}
	}
	else { // existe en la bd (los subtitulos no se guardan en la bd por el momento)
		switch (Ret->TipoMedio) {
			case Tipo_Medio_Audio :
			case Tipo_Medio_CDAudio :
			case Tipo_Medio_Video :
				Sistema.App.ListaC.AgregarMedio(Ret);
				break;
			case Tipo_Medio_Lista :
				ParsearM3U(Archivo, false);
				break;
		}
	}
}


void VentanaPlayerEx::SoltarArchivos_BuscarArchivos(const TCHAR *Directorio) {
	WIN32_FIND_DATA  FindInfoPoint;                                                                     
	HANDLE           hFind   = NULL;                                                                           
	DWL::DWLString   Path = Directorio; Path += TEXT("\\*.*");
	DWL::DWLString   Tmp;

	hFind = FindFirstFile(Path(), &FindInfoPoint);                                              
	while (FindNextFile(hFind, &FindInfoPoint) != 0) {                                                 
		Tmp = Directorio; Tmp += TEXT('\\'); Tmp += FindInfoPoint.cFileName;
		if (FindInfoPoint.cFileName[0] != TEXT('.') && FindInfoPoint.cFileName[1] != TEXT('.')) {
			if (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(Tmp())) SoltarArchivos_BuscarArchivos(Tmp());
			else                                                      SoltarArchivos_AgregarArchivo(Tmp());
		}
	}
	FindClose(hFind);     
}


void VentanaPlayerEx::AgregarAListaA(void) {
	bool NL				= false;
	int  TotalSel		= 0;
	int  TotalSubSel	= 0;
	if (Sistema.App.ListaC.TotalItems() == 0) NL = true;
	for (unsigned int i = 0; i < Sistema.App.ArbolBaseDatos.TotalNodosLineales(); i++) {
		if (Sistema.App.ArbolBaseDatos.NodoLinealMedio(i)->Seleccionado() == true) {
			TotalSel ++;
			if (Sistema.App.ArbolBaseDatos.NodoLinealMedio(i)->SubSeleccionado() == true) TotalSubSel ++;
//			Sistema.App.ListaC.NombreLista = Sistema.App.ArbolBaseDatos.NodoLineal(i)->Texto();
			if (Sistema.App.ArbolBaseDatos.NodoLinealMedio(i)->Medio != NULL)  {
				switch (Sistema.App.ArbolBaseDatos.NodoLinealMedio(i)->Medio->TipoMedio) {
					case Tipo_Medio_Audio : 
					case Tipo_Medio_CDAudio : 
					case Tipo_Medio_Video : 
						Sistema.App.ListaC.AgregarMedio(Sistema.App.ArbolBaseDatos.NodoLinealMedio(i)->Medio);   
						break;
				}
			}
		}
	}
	Sistema.App.ListaC.ActualizarTodo();
//	if (TotalSel - TotalSubSel != 1)			NL = false;
	if (NL == false) Sistema.App.ListaC.NombreLista = IDIOMA__MODIFICADA; // Modificado
	else			 Sistema.App.ListaC.NombreLista = Sistema.App.ArbolBaseDatos.NodoMarcado()->Texto();
	if (Sistema.App.Config.Shufle == true)  Sistema.App.ListaC.Shufle(true);
	if (Sistema.App.Media.MedioActual == NULL) 
		Sistema.App.Media.AbrirPrimerMedioLista();
}


void VentanaPlayerEx::AbrirDirectorioMedio(TMedio *nMedio) {
	if (nMedio == NULL) return;
	DWL::DWLString PathFinal;
	for (size_t i = nMedio->Path.Tam(); i != 0; i--) {
		if (nMedio->Path[i - 1] == TEXT('\\')) {
			PathFinal.Borrar();
			PathFinal.AgregarTexto(nMedio->Path(), i);
			break;
		}
	}
	ShellExecute(NULL, TEXT("Open"), PathFinal(), NULL, PathFinal(), SW_SHOW);
}

LRESULT VentanaPlayerEx::Evento_MenuEx_Menu_Pulsado(DWL::ControlesEx::DWLMenuEx *MenuPulsado, const UINT IDMenuExPadre) {
	DWL::Ventanas::DWLDialogoAbrir DlgSubtitulos;
	ItemListaMedios *nItem = NULL;
	DWL::DWLString PathSubtitulos;
	switch(MenuPulsado->IDMenu()) {
////////// Menu BDA
////////// -Agregar a la lista
		case IDMBDA_AGREGAR_LISTA :
			AgregarAListaA();
			break;
////////// -Agregar a una nueva lista
		case IDMBDA_AGREGAR_NUEVA_LISTA :
			Sistema.App.Media.Stop();
			Sistema.App.ListaC.EliminarTodosLosMedios();
			AgregarAListaA();
			break;
////////// -Eliminar de la BD
		case IDMBDA_ELIMINAR :  
			BDAEliminar();                    
			break;
////////// -Modificar
		case IDMBDA_MODIFICAR : 
			Sistema.App.ArbolBaseDatos.EmpezarLabelEdit(Sistema.App.ArbolBaseDatos.NodoMarcado());
			break;
////////// -Orden
		case IDMBDA_TGGD : 
		case IDMBDA_GGD : 
		case IDMBDA_GD : 
		case IDMBDA_D : 
			MenuBDA->Menu(5)->Menu(static_cast<int>(Sistema.App.Config.OrdenBD))->Icono(0);
			Sistema.App.Config.OrdenBD = static_cast<ORDEN_BASE_DATOS>(MenuPulsado->IDMenu() - IDMBDA_TGGD);
			Sistema.App.BD.ActualizarArbol();
			MenuBDA->Menu(5)->Menu(static_cast<int>(Sistema.App.Config.OrdenBD))->Icono(IDI_MARCA_MENU);
			break;
////////// -Nota
		case IDMBDA_N5 : case IDMBDA_N4 : case IDMBDA_N3 : case IDMBDA_N2 : case IDMBDA_N1 : 
			for (unsigned int nn = 0; nn < Sistema.App.ArbolBaseDatos.TotalNodosLineales(); nn++) {
				if (Sistema.App.ArbolBaseDatos.NodoLinealMedio(nn)->Seleccionado() == true) {
					if (Sistema.App.ArbolBaseDatos.NodoLinealMedio(nn)->Medio != NULL) Sistema.App.ArbolBaseDatos.NodoLinealMedio(nn)->Medio->Nota = (MenuPulsado->IDMenu() - IDMBDA_N1) +1;
				}
			}
			break;
//////////
		case IDMBDA_ABRIRDIR :
			AbrirDirectorioMedio(Sistema.App.ArbolBaseDatos.NodoMarcadoMedio()->Medio);
			break;

////////// Menu general para los controles standard
		case IDSM_ATRAS :
			Sistema.App.Media.Anterior();
			break;
		case IDSM_ADELANTE :
			Sistema.App.Media.Siguiente();
			break;
		case IDSM_PLAY :
			Sistema.App.Media.Play();
			break;
		case IDSM_PAUSA :
			Sistema.App.Media.Pausa();
			break;
		case IDSM_STOP :
			Sistema.App.Media.Stop();
			break;

		// Menu Lista
		case IDMLI_ELIMINAR : Sistema.App.ListaC.EliminarMediosSeleccionados(); break;

		case IDMLI_MPISTA : Sistema.App.ListaC.EmpezarLabelEdit(Sistema.App.ListaC.ItemMarcado(), 0, true);   break;

		case IDMLI_BUSCARBD : 
			Sistema.App.ArbolBaseDatos.Visible(true);
			Sistema.App.ListaC.Visible(false);
			nItem = Sistema.App.ListaC.ItemMedioMarcado();
			if (nItem != NULL) {
				if (nItem->Medio != NULL) {
					if (nItem->Medio->Nodo != NULL) {
						Sistema.App.ArbolBaseDatos.HacerVisible(nItem->Medio->Nodo);
						nItem->Medio->Nodo->Seleccionado(true);
					}
				}
			}
			break;

		case IDMLI_MNOMBRE :  Sistema.App.ListaC.EmpezarLabelEdit(Sistema.App.ListaC.ItemMarcado(), 1, false);  break;

		case IDMLI_N5 : case IDMLI_N4 : case IDMLI_N3 : case IDMLI_N2 : case IDMLI_N1 : 
			size_t i;
			int T; T = MenuPulsado->IDMenu() - (IDMLI_N1 -1);
			for (i = 0; i < Sistema.App.ListaC.TotalItems(); i++) {
				if (Sistema.App.ListaC.ItemMedio(i)->Seleccionado() == true)	Sistema.App.ListaC.ItemMedio(i)->Medio->Nota = static_cast<unsigned char>(T);
			}
			break;

		case IDMLI_PROPIS : // MostrarPropiedades
			for (i = 0; i < Sistema.App.ListaC.TotalItems(); i++) {
				if (Sistema.App.ListaC.ItemMedio(i)->Seleccionado() == true) {
					// Propis.MostrarPropiedades(SO.Sistema.App.BDList.BDItems(i)->Medio);
				}
			}
			break;
		case IDMLI_ABRIRDIR :
			AbrirDirectorioMedio(Sistema.App.ListaC.ItemMedioMarcado()->Medio);
			break;

		// Repeat
		case 3000 : case 3001 : case 3002 : case 3003 : case 3004 : case 3005 : case 3006 : case 3007 : case 3008 : case 3009 : case 3010 :
			BotonRepeat(MenuPulsado->IDMenu());
			break;

		// Shufle
		case IDMS_SHUFLESI       :                      Sistema.App.ListaC.Shufle(true);         break;
		case IDMS_SHUFLENO       :                      Sistema.App.ListaC.Shufle(false);        break;

		// Boton Lista
		case IDMBL_NGEN : 
			Sistema.App.ListaC.EliminarTodosLosMedios();
			Sistema.App.BD.GeneroAleatorio();
			Sistema.App.Media.AbrirPrimerMedioLista();
			break;
		case IDMBL_NGRP : 
			Sistema.App.ListaC.EliminarTodosLosMedios();
			Sistema.App.BD.GrupoAleatorio();
			Sistema.App.Media.AbrirPrimerMedioLista();
			break;
		case IDMBL_NDSK : 
			Sistema.App.ListaC.EliminarTodosLosMedios();
			Sistema.App.BD.DiscoAleatorio();
			Sistema.App.Media.AbrirPrimerMedioLista();
			break;
		case IDMBL_NCAN :  
			Sistema.App.ListaC.EliminarTodosLosMedios();
			Sistema.App.BD.CancionesAleatorias(50);
			Sistema.App.Media.AbrirPrimerMedioLista();
			break;
		case IDMBL_N5E :
			Sistema.App.Media.Stop();
			Sistema.App.ListaC.EliminarTodosLosMedios();
			Sistema.App.BD.Estrellas5();
			Sistema.App.Media.AbrirPrimerMedioLista();
			break;
		case IDMBL_NMAS_ESCUCHADAS :
			Sistema.App.Media.Stop();
			Sistema.App.ListaC.EliminarTodosLosMedios();
			Sistema.App.BD.CancionesMasEscuchadas(50);
			Sistema.App.Media.AbrirPrimerMedioLista();
			break;
		case IDMBL_GEN :
			Sistema.App.BD.GeneroAleatorio();
			break;
		case IDMBL_GRP :
			Sistema.App.BD.GrupoAleatorio();
			break;
		case IDMBL_DSK :
			Sistema.App.BD.DiscoAleatorio();
			break;
		case IDMBL_CAN :
			Sistema.App.BD.CancionesAleatorias(50);
			break;
		case IDMBL_5E :
			Sistema.App.BD.Estrellas5();
			break;
		case IDMBL_MAS_ESCUCHADAS:
			Sistema.App.BD.CancionesMasEscuchadas(50);
			break;
		case IDMBL_LISTAVACIA :
			Sistema.App.ListaC.EliminarTodosLosMedios();
			break;
		case IDMBL_GUARDAR : // guardar lista
			break;
		case IDMBL_NARCHIVO : 
			Sistema.App.ListaC.EliminarTodosLosMedios();
			AbrirArchivo();
			break;
		case IDMBL_ARCHIVO :
			AbrirArchivo();
			break;

		// Video
		case IDMV_SUBTITULOS :
			size_t PosicionPath;
			PosicionPath = Sistema.App.Media.MedioActual->Medio->Path.Tam();
			while (Sistema.App.Media.MedioActual->Medio->Path[PosicionPath] != '\\') PosicionPath --;
			PathSubtitulos.Borrar();
			PathSubtitulos.AgregarTexto(Sistema.App.Media.MedioActual->Medio->Path(), PosicionPath + 1);
			int Ret;
			Ret = DlgSubtitulos.MostrarAbrir(PathSubtitulos(), IDIOMA__ARCHIVOS_DE_SUBTITULOS_STR, IDIOMA__ABRIR_SUBTITULOS, false);
			if (Ret == 1) Sistema.App.VLC.Asignar_Subtitulos_MedioActual(DlgSubtitulos[0]);
			
			break;

		// BD -> Buscar archivos
		case IDMBBD_BUSCAR :
			Sistema.App.ArbolBaseDatos.Visible(false);
			Sistema.App.ListaC.Visible(false);
			Sistema.App.MediaPlayer.Visible(false);
			CP.Visible(true);
			CP.Mostrar_BuscarArchivos();
            SetFocus(CP.hWnd());
			break;
		// BD -> Revisar medios
		case IDMBBD_REVISAR :
			Sistema.App.ArbolBaseDatos.Visible(false);
			Sistema.App.ListaC.Visible(false);
			Sistema.App.MediaPlayer.Visible(false);
			CP.Visible(true);
			CP.Mostrar_RevisarBD();
            SetFocus(CP.hWnd());
			break;
	}
	int cID = MenuPulsado->IDMenu();
	if (cID >= MENUNGEN && cID <= MENUNGEN + 998) { // Seleccion de genero en el menu de la lista
		Sistema.App.BD.GeneroSeleccionado(cID - MENUNGEN);
		if (Sistema.App.PlayerEx.CP.Visible() == TRUE && Sistema.App.PlayerEx.CP.Estado == EstadoCP_Inicio) Sistema.App.PlayerEx.CP.Mostrar_Inicio();
	}
	else if (cID >= MENUGEN && cID <= MENUGEN + 998) { // Seleccion de nuevo genero en el menu de la lista
		Sistema.App.Media.Stop();
		Sistema.App.ListaC.EliminarTodosLosMedios();
		Sistema.App.BD.GeneroSeleccionado(cID - MENUGEN);
		Sistema.App.Media.AbrirPrimerMedioLista();
		if (Sistema.App.PlayerEx.CP.Visible() == TRUE && Sistema.App.PlayerEx.CP.Estado == EstadoCP_Inicio) Sistema.App.PlayerEx.CP.Mostrar_Inicio();
	}
	if (cID == 5999) {
		Sistema.App.BD.GeneroAleatorio();
		if (Sistema.App.PlayerEx.CP.Visible() == TRUE && Sistema.App.PlayerEx.CP.Estado == EstadoCP_Inicio) Sistema.App.PlayerEx.CP.Mostrar_Inicio();
	}
	else if (cID == 4999) {
		Sistema.App.Media.Stop();
		Sistema.App.ListaC.EliminarTodosLosMedios();
		Sistema.App.BD.GeneroAleatorio();
		Sistema.App.Media.AbrirPrimerMedioLista();
		if (Sistema.App.PlayerEx.CP.Visible() == TRUE && Sistema.App.PlayerEx.CP.Estado == EstadoCP_Inicio) Sistema.App.PlayerEx.CP.Mostrar_Inicio();
	}
    return 0;
}

LRESULT VentanaPlayerEx::Evento_ComandoDelSistema(const UINT nID, const int cX, const int cY) {
	switch (nID) {
		// Minimizar
		case SC_MINIMIZE :
			Oscy.Terminar();
			Oscy.PintarFondoVacio();
//			EliminarTemporizador(TIMER_OSCY);
			break;
		// Restaurar
		case SC_RESTORE :
			Maximizar(false);
			Oscy.IniciarThread();
//			CrearTemporizador(TIMER_OSCY, INTERVALO_OSCY);
			break;
		case SC_MAXIMIZE :
			Maximizar(true);
			Oscy.IniciarThread();
			break;
	}
    return DefWindowProc(_hWnd, WM_SYSCOMMAND, static_cast<WPARAM>(nID), MAKELPARAM(cX, cY));
}


// Esta funcion controla cuando se termina una cancion y luego decide lo que se debe hacer
// Si existe una cancion mas abajo la pondra, si ese no es el caso entonces mirara que tipo de repeat debe utilizar.
void VentanaPlayerEx::Evento_TemporizadorLista(void) {
	if (Sistema.App.Media.MedioActual == NULL) return;

	// Repinto el control del tiempo actual en modo normal
	HDC hDC = GetDC(TiempoStrActual.hWnd());
	TiempoStrActual.Repintar(hDC, true);
	ReleaseDC(TiempoStrActual.hWnd(), hDC);

	// Repinto el control del tiempo actual en modo pantalla completa
	HDC hDC2 = GetDC(Sistema.App.ControlesMP.TiempoStrActual.hWnd());
	Sistema.App.ControlesMP.TiempoStrActual.Repintar(hDC2, true);
	ReleaseDC(Sistema.App.ControlesMP.TiempoStrActual.hWnd(), hDC2);

	bool ExistenMedios = false;
	//if (Sistema.App.Media.MedioActual->Medio != NULL) {
	Estados_Medio EstadoActual = Sistema.App.Media.ComprobarEstado();
	if (EstadoActual == Terminada) {
		// Sumo 1 a los valores del medio que cuentan las veces que se ha escuchado
		Sistema.App.Media.SumarReproduccion();
		// Restauro el icono del medio en la lista y la base de datos
		Sistema.App.Media.ReemplazarIcono(!Sistema.App.Media.MedioActual->Existe);
		Sistema.App.Media.EliminarMedioActual();
		Sistema.App.ControlesMP.Ocultar(Sistema.App.MediaPlayer.hWnd());

		// Miro el Repeat
		if (static_cast<int>(Sistema.App.ListaC.TotalItems() -1) <= Sistema.App.Media.Actual) { // Repeat
			switch (Sistema.App.Config.Repeat) {
				case 0 : // Desactivado
					Sistema.App.Media.Stop();
					Sistema.App.Media.Actual = 0;
					break;
				case 1 : // Repeat Normal
				case 2 : // Repeat + Shufle
					// Compruebo que almenos un medio de la lista exista
					for (size_t i = 0; i < Sistema.App.ListaC.TotalItems(); i++) {
						if (Sistema.App.ListaC.ItemMedio(i)->Existe == true) {
							ExistenMedios = true;
							break;
						}
					}
					// Abro siguiente medio
					if (ExistenMedios == true) {
						if (Sistema.App.Config.Repeat == 2) Sistema.App.ListaC.Shufle(true); // repetir lista con shufle
						Sistema.App.Media.AbrirPrimerMedioLista();
						//Sistema.App.Media.Siguiente();
					}
					break;
/*					// Compruebo que almenos un medio de la lista exista
					for (i = 0; i < Sistema.App.ListaC.TotalItems(); i++) {
						if (Sistema.App.ListaC.ItemMedio(i)->Existe == true) {
							ExistenMedios = true;
							break;
						}
					}
					if (ExistenMedios == true) {
						Sistema.App.ListaC.Shufle(true);
						Sistema.App.Media.AbrirPrimerMedioLista();
//						Sistema.App.Media.Actual = -1;
//						Sistema.App.Media.Siguiente();
					}
					break;*/
				case 4 : // Genero aleatorio
					Sistema.App.Media.Stop();
					Sistema.App.ListaC.EliminarTodosLosMedios();
					Sistema.App.Media.Actual = -1;
					Sistema.App.BD.GeneroAleatorio();
					Sistema.App.Media.AbrirPrimerMedioLista();
					break;
				case 5 : // Grupo aleatorio
					Sistema.App.Media.Stop();
					Sistema.App.ListaC.EliminarTodosLosMedios();
					Sistema.App.Media.Actual = -1;
					Sistema.App.BD.GrupoAleatorio();
					Sistema.App.Media.AbrirPrimerMedioLista();
					break;
				case 6 : // Disco aleatorio
					Sistema.App.Media.Stop();
					Sistema.App.ListaC.EliminarTodosLosMedios();
					Sistema.App.Media.Actual = -1;
					Sistema.App.BD.DiscoAleatorio();
					Sistema.App.Media.AbrirPrimerMedioLista();
					break;
				case 7 : // Canciones aleatorias
					Sistema.App.ListaC.EliminarTodosLosMedios();  
					Sistema.App.Media.Actual = -1;
					Sistema.App.BD.CancionesAleatorias(50);
					Sistema.App.Media.AbrirPrimerMedioLista();
					break;
				case 9 :  // Apagar el Equipo
					Sistema.App.Config.Repeat = Repeat_Desactivado;
					Sistema.App.Media.Stop();
					PostMessage(_hWnd, WM_CLOSE, 0, 0);
					Sistema.App.CerrarSistema(CS_Apagar, false);
					return;
				case 10 :  // Apagar el reproductor
					Sistema.App.Config.Repeat = Repeat_Desactivado;
					Sistema.App.Media.Stop();
					PostMessage(_hWnd, WM_CLOSE, 0, 0);
					return;
			}
		}
		else {
			Sistema.App.Media.Siguiente();
		}
	} 
}

LRESULT VentanaPlayerEx::Evento_Comando(const int cID, const UINT CodigoNotificacion, HWND hWndControl) {
	if (CodigoNotificacion == THBN_CLICKED) {
		switch (cID) {
			case ID_BOTON_CANCION_ATRAS		:		Sistema.App.Media.Anterior();					break;
			case ID_BOTON_CANCION_PLAY		:		Sistema.App.Media.Play();						break;
			case ID_BOTON_CANCION_PAUSA		:		Sistema.App.Media.Pausa();						break;
			case ID_BOTON_CANCION_STOP		:		Sistema.App.Media.Stop();						break;
			case ID_BOTON_CANCION_ADELANTE	:		Sistema.App.Media.Siguiente();					break;
		}
	}
	return 0;
}


LRESULT VentanaPlayerEx::Evento_BotonEx_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
	if (Boton != 0) return 0;
	switch (IDBotonEx) {
		case ID_BOTON_CANCION_ATRAS		:		Sistema.App.Media.Anterior();					break;
		case ID_BOTON_CANCION_PLAY		:		Sistema.App.Media.Play();						break;
		case ID_BOTON_CANCION_PAUSA		:		Sistema.App.Media.Pausa();						break;
		case ID_BOTON_CANCION_STOP		:		Sistema.App.Media.Stop();						break;
		case ID_BOTON_CANCION_ADELANTE	:		Sistema.App.Media.Siguiente();					break;
		case ID_BOTON_SHUFLE			:		
			Sistema.App.ListaC.Shufle(!Sistema.App.Config.Shufle);	
			Sistema.App.ControlesMP.BotonShufleRepeat.ShufleMarcado(Sistema.App.Config.Shufle);
			break;	
		case ID_BOTON_REPEAT			:		Sistema.App.PlayerEx.MenuRepeat->Mostrar(_hWnd);		break;
	}
    return 0;
}

void VentanaPlayerEx::BDAEliminarConfirmado(const bool BorrarArchivosDisco) {

	BOOL R;
	BOOL UserEsAdmin = IsUserAnAdmin();
/*	
	if (IsUserAnAdmin() == FALSE) { // requiere elevacion de privilegios
		HANDLE Token;
		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &Token);
		TOKEN_PRIVILEGES Privi;
		LUID createPrivilege = { 0 };
		Privi.PrivilegeCount = 1;
		Privi.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		Privi.Privileges[0].Luid = createPrivilege;
		R = AdjustTokenPrivileges(Token, FALSE, &Privi, 0, NULL, NULL);
		CloseHandle(Token);
	}*/
	std::vector<DWL::DWLString> PathsEliminar;
	

	ArbolBaseDatos     *Arbol = &Sistema.App.ArbolBaseDatos;
	NodoArbolBaseDatos *Tmp   = NULL;
//	UINT		    size_t = Arbol->TotalNodosLineales();
	int				Respuesta = IDOK;
	size_t i = 0,z = 0;
	for (i = Arbol->TotalNodosLineales(); i > 0; i--) {
		if (Arbol->NodoLineal(i - 1)->Seleccionado() == true && Arbol->NodoLineal(i - 1)->SubSeleccionado() == false) {
			Respuesta = IDOK;
			switch (Arbol->NodoLineal(i - 1)->Icono()) {
				case IDI_TIPO :
					for (z = Arbol->TotalNodosLineales(); z > 0; z --) {
						if (Arbol->NodoLinealMedio(z - 1)->Medio != NULL) {
							if (static_cast<TMedioAudio *>(Arbol->NodoLinealMedio(z - 1)->Medio)->Tipo == Arbol->NodoLinealMedio(i -1)->Tipo) {
								PathsEliminar.push_back(Arbol->NodoLinealMedio(z - 1)->Medio->Path());
								Sistema.App.BD.BorrarMedio(Arbol->NodoLinealMedio(z - 1)->Medio);
								Arbol->NodoLinealMedio(z - 1)->Icono(0, false);
							}
						}
					}
					break;
				case IDI_GENERO :
					for (z = Arbol->TotalNodosLineales(); z > 0; z --) {
						if (Arbol->NodoLinealMedio(z - 1)->Medio != NULL) {
							if (static_cast<TMedioAudio *>(Arbol->NodoLinealMedio(z - 1)->Medio)->Genero == Arbol->NodoLinealMedio(i - 1)->Genero) {
								PathsEliminar.push_back(Arbol->NodoLinealMedio(z - 1)->Medio->Path());
								Sistema.App.BD.BorrarMedio(Arbol->NodoLinealMedio(z - 1)->Medio);
								Arbol->NodoLinealMedio(z - 1)->Icono(0, false);
							}
						}
					}
					break;
				case IDI_GRUPO :
					for (z = Arbol->TotalNodosLineales(); z > 0; z --) {
						if (Arbol->NodoLinealMedio(z - 1)->Medio != NULL) {
							if (static_cast<TMedioAudio *>(Arbol->NodoLinealMedio(z - 1)->Medio)->Grupo == Arbol->NodoLinealMedio(i - 1)->Grupo) {
								PathsEliminar.push_back(Arbol->NodoLinealMedio(z - 1)->Medio->Path());
								Sistema.App.BD.BorrarMedio(Arbol->NodoLinealMedio(z - 1)->Medio);
								Arbol->NodoLinealMedio(z - 1)->Icono(0, false);
							}
						}
					}
					break;
				case IDI_DISCO :
					for (z = Arbol->TotalNodosLineales(); z > 0; z --) {
						if (Arbol->NodoLinealMedio(z - 1)->Medio != NULL) {
							if (static_cast<TMedioAudio *>(Arbol->NodoLinealMedio(z - 1)->Medio)->Disco == Arbol->NodoLinealMedio(i - 1)->Disco) {
								PathsEliminar.push_back(Arbol->NodoLinealMedio(z - 1)->Medio->Path());
								Sistema.App.BD.BorrarMedio(Arbol->NodoLinealMedio(z - 1)->Medio);
								Arbol->NodoLinealMedio(z - 1)->Icono(0, false);
							}
						}
					}
					break;
				case IDI_AUDIO :
				case IDI_VIDEO :
					Tmp = Arbol->NodoLinealMedio(i - 1);
					if (Tmp->Medio != NULL) {
						PathsEliminar.push_back(Tmp->Medio->Path());
						Sistema.App.BD.BorrarMedio(Tmp->Medio);
						Arbol->NodoLinealMedio(i - 1)->Icono(0 , false);
					}
					break;
				// Si es play puede ser una pista de audio
				case IDI_PLAY :
					Tmp = Arbol->NodoLinealMedio(i - 1);
					if (Tmp->Medio != NULL) {
						if (Tmp->Medio->TipoMedio != Tipo_Medio_CDAudio) { 
							// No es una pista de audio
							PathsEliminar.push_back(Tmp->Medio->Path());
							Sistema.App.BD.BorrarMedio(Tmp->Medio);
							Arbol->NodoLinealMedio(i - 1)->Icono(0 , false);
						}
					}
					break;
				// solo el M3U
				case IDI_M3U :
					Tmp = Arbol->NodoLinealMedio(i - 1);
					if (Tmp->Medio != NULL) {
						PathsEliminar.push_back(Tmp->Medio->Path());
						Sistema.App.BD.BorrarMedio(Tmp->Medio);
						Arbol->NodoLinealMedio(i - 1)->Icono(0 , false);
					}
					break;
			}
		}
	}
	// Elimino los nodos
	UINT Total = 0;
	for (i = Arbol->TotalNodosLineales(); i > 0; i--) {
		if (Arbol->NodoLinealMedio(i - 1)->Icono() == 0) {
			Arbol->EliminarNodo(Sistema.App.ArbolBaseDatos.NodoLinealMedio(i - 1), true);
		}
	}
	if (UserEsAdmin == FALSE && BorrarArchivosDisco == true) {
		Total = PathsEliminar.size();
		if (Total == 0) {
			Arbol->ActualizarTodo();
			return;
		}
		ArchivoBinarioPlayer Archivo;
		DWL::DWLString AppPath;
		Sistema.Directorio.AppData(AppPath);
		AppPath += TEXT("\\Eliminar.btr");
		Archivo.AbrirArchivoEscritura(AppPath());
		Archivo.GuardarUINT(Total);
		for (size_t nz = 0; nz < Total; nz ++) Archivo.GuardarString(PathsEliminar[nz]);
		AppPath = Sistema.App.AppPath(); AppPath += TEXT("BubaTronik.exe");
		Archivo.CerrarArchivo();
		ShellExecute(NULL, TEXT("RunAs"), AppPath(), TEXT("-EliminarArchivosDisco"), Sistema.App.AppPath(), SW_SHOWNORMAL);
	}
	else if (UserEsAdmin == FALSE && BorrarArchivosDisco == true) {
		Total = PathsEliminar.size();
		if (Total == 0) {
			Arbol->ActualizarTodo();
			return;
		}
		for (size_t nzr = 0; nzr < Total; nzr ++) R = DeleteFile(PathsEliminar[nzr].Texto());
	}
	Arbol->ActualizarTodo();
}


void VentanaPlayerEx::BDAEliminar(void) {
/*	if (IsUserAnAdmin() == FALSE) { // requiere elevacion de privilegios
		BDAEliminarConfirmado(false);
	}
	else { // Es administrador, puede borrar archivos*/
	if (Sistema.App.ArbolBaseDatos.NodoMarcado() == NULL) return;
	static VentanaMensaje_AceptarCancelar MsgBox;
	if (IDI_M3U != Sistema.App.ArbolBaseDatos.NodoMarcado()->Icono()) {	
		MsgBox.MostrarMensaje(IDIOMA__DESEAS_ELIMINAR_TAMBIEN_LOS_MEDIOS_DEL_DISCO_DURO, IDIOMA__ELIMINAR_MEDIOS, ID_MENSJAE_ELIMINAR_MEDIOS, IDIOMA__ELIMINAR, IDIOMA__OMITIR);
	}
	else { // Si es un M3U remarco que solo se eliminara el M3U
		MsgBox.MostrarMensaje(IDIOMA__DESEAS_ELIMINAR_EL_M3U_DEL_DISCO_ESTO_NO_AFECTARA_A_LOS_MEDIOS_QUE_CONTIENE_EL_M3U, IDIOMA__ELIMINAR_MEDIOS, ID_MENSJAE_ELIMINAR_MEDIOS, IDIOMA__ELIMINAR, IDIOMA__OMITIR);
	}

}




LRESULT VentanaPlayerEx::Evento_BarraEx_Mouse_BotonSoltado(const UINT cID, const UINT nBoton) {
	switch (cID) {
		case ID_SLIDER_TIEMPO :
			Sistema.App.Media.TiempoActual(Sistema.App.SliderTiempo.Valor());
			break;
	}
    return 0;
}

void VentanaPlayerEx::MostrarMenuLista(void) {
	if (Sistema.App.ListaC.ItemMarcado() != NULL) {
		int   Div   = 0;
		int   NotaT = 0;
		size_t   i;
		// Escaneo todos los items para obtener la nota media
		for (i = 0; i < Sistema.App.ListaC.TotalItems(); i++) {
			if (Sistema.App.ListaC.ItemMedio(i)->Seleccionado() == true) {
				Div ++;
				NotaT += Sistema.App.ListaC.ItemMedio(i)->Medio->Nota;
			}
		}
		if (NotaT == 0) {
			NotaT = Sistema.App.ListaC.ItemMedioMarcado()->Medio->Nota;
			Div ++;
		}
		NotaT = NotaT / Div;
		for (i = 0; i < 5; i++) MenuLista->Menu(4)->Menu(i)->Icono(NULL);
		MenuLista->BuscarID((IDMLI_N1 -1) + NotaT)->Icono(IDI_MARCA_MENU);
		MenuLista->Menu(2)->Activado(true);
		MenuLista->Menu(3)->Activado(true);
		MenuLista->Menu(4)->Activado(true);
//		MenuLista->Menu(5)->Activado(true); // copiar a... (DESMARCAR CUANDO SE PROGRAME LA FUNCION)
		MenuLista->Menu(6)->Activado(true); // Abrir su directorio
		if (Div == 1) {
			MenuLista->Menu(0)->Activado(true);
			MenuLista->Menu(1)->Activado(true);
		}
		else {
			MenuLista->Menu(0)->Activado(false);
			MenuLista->Menu(1)->Activado(false);
		}
	}
	else {
		MenuLista->Menu(0)->Activado(false);
		MenuLista->Menu(1)->Activado(false);
		MenuLista->Menu(2)->Activado(false);
		MenuLista->Menu(3)->Activado(false);
		MenuLista->Menu(4)->Activado(false); // copiar a...
		MenuLista->Menu(5)->Activado(false); // Abrir su directorio
		MenuLista->Menu(6)->Activado(false); // propiedades
	}
	MenuLista->Mostrar(_hWnd);
}



void VentanaPlayerEx::BotonRepeat(const int cID) {
	Sistema.App.Config.Repeat = static_cast<TIPOS_REPEAT>(cID - 3000);
	for (int i = 0; i < 11; i++) MenuRepeat->Menu(i)->Icono(NULL); //.Item(i)->Checked(false);
	MenuRepeat->Menu(Sistema.App.Config.Repeat)->Icono(IDI_MARCA_MENU);
	if (Sistema.App.Config.Repeat != 0) {
		BotonShufleRepeat.RepeatMarcado(true);
		Sistema.App.ControlesMP.BotonShufleRepeat.RepeatMarcado(true);
	}
	else {
		BotonShufleRepeat.RepeatMarcado(false); 
		Sistema.App.ControlesMP.BotonShufleRepeat.RepeatMarcado(false);
	}
//	DhDC hDCO(Oscy.BG[6].hWnd());
//	Oscy.BG[6].Evento_PintarHDC(hDCO.hDC());
}


LRESULT VentanaPlayerEx::Evento_Mouse_Rueda(const short Delta, const short cX, const short cY, const UINT VirtKey) {
    if (CP.Visible() == TRUE)           
        return CP.Evento_Mouse_Rueda(Delta, cX, cY, VirtKey);
    if (Sistema.App.ArbolBaseDatos.Visible() == TRUE) 
        return Sistema.App.ArbolBaseDatos.Evento_Mouse_Rueda(Delta, cX, cY, VirtKey);
    if (Sistema.App.ListaC.Visible() == TRUE)   
        return Sistema.App.ListaC.Evento_Mouse_Rueda(Delta, cX, cY, VirtKey);
    return 0;
}

LRESULT VentanaPlayerEx::Evento_VentanaMensaje(const UINT IDMensaje, const RespuestaVentanaMensaje &Respuesta) {
	bool ReproducirPorDefecto = false;
	DWL::DWLString AppPath;
    switch (IDMensaje) {
		case ID_MENSJAE_SOBREESCRIBIRM3U :
			if (Respuesta == Respuesta_Aceptar) Sistema.App.ListaC.GuardarLista();
			break;
		case ID_MENSJAE_ELIMINAR_MEDIOS :
			if		(Respuesta == Respuesta_Cancelar)	BDAEliminarConfirmado(false);
			else if (Respuesta == Respuesta_Aceptar)	BDAEliminarConfirmado(true);	
			break;
        // Asociar archivos
        case ID_MENSAJE_ASOCIAR :
            switch (Respuesta) {
				case Respuesta_Aceptar :
				case Respuesta_Aceptar_NoMostrar :
					if (IsUserAnAdmin() == FALSE) {
						AppPath = Sistema.App.AppPath(); AppPath += TEXT("BubaTronik.exe");
						ShellExecute(NULL, TEXT("RunAs"), AppPath(), TEXT("-InstalarRegistroWindows"), Sistema.App.AppPath(), SW_SHOWNORMAL);
					}
					else {
						Sistema.App.Asociar.RegistrarApp();
						ReproducirPorDefecto = Sistema.App.Asociar.ReproducirPorDefecto();
					}
					break;
				case Respuesta_Cancelar :
					break;
				case Respuesta_Cancelar_NoMostrar :
					break;
			}
			Sistema.App.Config.AsociarArchivos = Respuesta;
            break;
    }
    return 0;
}

LRESULT VentanaPlayerEx::Evento_ListaEx_LabelEdit_Inicio(DWL::ControlesEx::DWLListaEx_ParametrosLabelEdit *Edicion, const UINT IDListaEx) {
	if (IDListaEx == ID_LISTA_CANCIONES) EditandoNombre = true;
	return 0;
}

LRESULT VentanaPlayerEx::Evento_ListaEx_LabelEdit_Fin(DWL::ControlesEx::DWLListaEx_ParametrosLabelEdit *Edicion, const UINT IDListaEx) {
	if (IDListaEx == ID_LISTA_CANCIONES) {
		DWL::DWLString TmpStr;
		DWL::DWLString TmpNombre;
		int            TmpPista = 0;
		ItemListaMedios *eItem = static_cast<ItemListaMedios *>(Edicion->Item());
		if (Edicion->NumSubItem() == 0) { // pista
			TmpPista = _wtoi(Edicion->NuevoTexto());
			switch (Sistema.App.ListaC.Ver_DigitosPista()) {
				case 2 : Edicion->NuevoTexto.sprintf(TEXT("%02d"), TmpPista);	break;
				case 3 : Edicion->NuevoTexto.sprintf(TEXT("%03d"), TmpPista);	break;
				case 4 : Edicion->NuevoTexto.sprintf(TEXT("%04d"), TmpPista);	break;
			}
			
			switch (eItem->Medio->TipoMedio) {
				case Tipo_Medio_Audio	:	static_cast<TMedioAudio *>(eItem->Medio)->Pista = TmpPista;		TmpNombre = static_cast<TMedioAudio *>(eItem->Medio)->Nombre;		break;
				case Tipo_Medio_CDAudio :	static_cast<TMedioCDAudio *>(eItem->Medio)->Pista = TmpPista;	TmpNombre = static_cast<TMedioCDAudio *>(eItem->Medio)->Nombre;		break;
				case Tipo_Medio_Video	:	static_cast<TMedioVideo *>(eItem->Medio)->Pista = TmpPista;		TmpNombre = static_cast<TMedioVideo *>(eItem->Medio)->Nombre;		break;
			}
			// Editamos el nodo
			if (eItem->Medio->Nodo != NULL) {
				TmpStr.sprintf(TEXT("%02d %s"), TmpPista, TmpNombre());
				eItem->Medio->Nodo->Texto(TmpStr(), true);
			}
		}
		if (Edicion->NumSubItem() == 1) { // nombre
			switch (eItem->Medio->TipoMedio) {
				case Tipo_Medio_Audio	:	static_cast<TMedioAudio *>(eItem->Medio)->Nombre = Edicion->NuevoTexto;		TmpPista = static_cast<TMedioAudio *>(eItem->Medio)->Pista;		break;
				case Tipo_Medio_CDAudio :	static_cast<TMedioCDAudio *>(eItem->Medio)->Nombre = Edicion->NuevoTexto;	TmpPista = static_cast<TMedioCDAudio *>(eItem->Medio)->Pista;	break;
				case Tipo_Medio_Video	:	static_cast<TMedioVideo *>(eItem->Medio)->Nombre = Edicion->NuevoTexto;		TmpPista = static_cast<TMedioVideo *>(eItem->Medio)->Pista;		break;
			}
			// Editamos el nodo
			if (eItem->Medio->Nodo != NULL) {
				TmpStr.sprintf(TEXT("%02d %s"), TmpPista, Edicion->NuevoTexto());
				eItem->Medio->Nodo->Texto(TmpStr(), true);
			}
		}
		EditandoNombre = false;
	}
	return 0;
}


LRESULT VentanaPlayerEx::Evento_MostrarToolTip(const UINT IDControl, LPARAM Param) {
	switch (IDControl) {
		case ID_ARBOL_BASEDATOS :
			if (Sistema.App.ArbolBaseDatos.NodoResaltado() != NULL) {
				Sistema.App.ArbolBaseDatos.MostrarToolTipNodo(reinterpret_cast<NodoArbolBaseDatos *>(Sistema.App.ArbolBaseDatos.NodoResaltado()));
			}
			break;
		case ID_LISTA_CANCIONES :
			if (Sistema.App.ListaC.ItemMedioResaltado() != NULL) {
				if (Sistema.App.ListaC.ItemMedioResaltado()->Medio != NULL) {
					Sistema.App.ArbolBaseDatos.MostrarToolTipNodo(Sistema.App.ListaC.ItemMedioResaltado()->Medio->Nodo);
				}
			}
			break;
	}
	return 0;
}


LRESULT VentanaPlayerEx::Evento_OcultarToolTip(const UINT IDControl, LPARAM Param) {
	Sistema.App.ToolTip.Destruir();
	return 0;
}