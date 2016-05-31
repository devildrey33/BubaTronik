#include "stdafx.h"
#include "ControlPresentacionEx.h"
#include "resource.h"
#include "AsociarArchivos.h"
#include "IDS_Definidas.h"
#include "BubaTronikApp.h"
#include <DWLListaIconos.h>
#include "TipoArchivo.h"
#include "InformacionArchivoEx.h"
#include <shellapi.h>

//DWL_APP(BubaTronikApp);

#define ID_FC_ASIGNAR_NOMBRE	2033
#define ID_FC_MODIFICAR_NOMBRE  2034
#define ID_FC_ELIMINAR			2035


void ControlPresentacionEx::IniciarBusquedaArchivos(void) {
	if (BuscarArchivos == NULL) { // Nueva busqueda
		DWL::ControlesEx::DWLListaEx               *ListaDirectorios	= EscenaActual->BuscarLista(ID_DIRECTORIOS_BUSCAR);
		DWL::ControlesEx::DWLBotonEx               *BotonBuscar			= EscenaActual->BuscarBoton(ID_BOTON_BUSCAR2);
		DWL::ControlesEx::DWLBotonEx               *BotonAgregarDir		= EscenaActual->BuscarBoton(ID_BOTON_AGREGAR_DIR);
		DWL::ControlesEx::DWLEdicionDesplegableEx  *ComboDir			= EscenaActual->BuscarEdicionDesplegable(ID_COMBO_DIR);
		DWL::ControlesEx::DWLEdicionDesplegableEx  *ComboOrden			= EscenaActual->BuscarEdicionDesplegable(ID_COMBO_ORDEN_DIR);
		DWL::ControlesEx::DWLBarraEx               *Barra				= EscenaActual->BuscarBarraProgreso(ID_BARRA_BUSCAR);
		DWL::ControlesEx::DWLMarcaEx               *Marca				= EscenaActual->BuscarMarca(ID_FILTRO_CARACTERES);
		DWL::ControlesEx::DWLMarcaEx               *Marca2				= EscenaActual->BuscarMarca(ID_FILTRO_COINCIDENCIAS);
		DWL::ControlesEx::DWLMarcaEx               *Marca3				= EscenaActual->BuscarMarca(ID_MARCA_TAG);
		DWL::ControlesEx::DWLMarcaEx               *Marca4				= EscenaActual->BuscarMarca(ID_MARCA_PATH);

		Sistema.App.PlayerEx.OpcionesBasicas.Activado(false);
		Barra->Valor(0);
		Barra->BarraEx_Estilos.Colores.ReiniciarColores();
		BotonBuscar->Texto(IDIOMA__CANCELAR);
		ListaDirectorios->Activado(false);
		BotonAgregarDir->Activado(false);
		ComboDir->Activado(false);
		ComboOrden->Activado(false);
		Marca->Activado(false);
		Marca2->Activado(false);
		Marca3->Activado(false);
		Marca4->Activado(false);

		BuscarArchivos = new ThreadBuscarArchivos;
		BuscarArchivos->IniciarBusqueda(EscenaActual->BuscarLista(ID_DIRECTORIOS_BUSCAR));
	}
	else {
		BuscarArchivos->Cancelar();
		Sistema.App.BD.Parar_RevisarBD();
	}
}


void ControlPresentacionEx::CancelarBusquedaArchivos(void) {
	if (BuscarArchivos != NULL) BuscarArchivos->Cancelar();
}


void ControlPresentacionEx::TerminarBusquedaArchivos(void) {
	DWL::ControlesEx::DWLListaEx               *ListaDirectorios	= EscenaActual->BuscarLista(ID_DIRECTORIOS_BUSCAR);
	DWL::ControlesEx::DWLBotonEx               *BotonBuscar			= EscenaActual->BuscarBoton(ID_BOTON_BUSCAR2);
	DWL::ControlesEx::DWLBotonEx               *BotonAgregarDir		= EscenaActual->BuscarBoton(ID_BOTON_AGREGAR_DIR);
	DWL::ControlesEx::DWLEdicionDesplegableEx  *ComboDir			= EscenaActual->BuscarEdicionDesplegable(ID_COMBO_DIR);
	DWL::ControlesEx::DWLEdicionDesplegableEx  *ComboOrden			= EscenaActual->BuscarEdicionDesplegable(ID_COMBO_ORDEN_DIR);
	DWL::ControlesEx::DWLBarraEx               *Barra				= EscenaActual->BuscarBarraProgreso(ID_BARRA_BUSCAR);
	DWL::ControlesEx::DWLMarcaEx               *Marca				= EscenaActual->BuscarMarca(ID_FILTRO_CARACTERES);
	DWL::ControlesEx::DWLMarcaEx               *Marca2				= EscenaActual->BuscarMarca(ID_FILTRO_COINCIDENCIAS);
	DWL::ControlesEx::DWLMarcaEx               *Marca3				= EscenaActual->BuscarMarca(ID_MARCA_TAG);
	DWL::ControlesEx::DWLMarcaEx               *Marca4				= EscenaActual->BuscarMarca(ID_MARCA_PATH);

	Barra->Valor(0);	
	if (BuscarArchivos->Cancelado() == false) {
		Sistema.App.BD.RepararAnomaliasMediosAudio(Barra);
		Sistema.App.BD.ActualizarArbol();
	}
	else { // Se ha cancelado
		Barra->Valor(0);
	}
    delete BuscarArchivos;
    BuscarArchivos = NULL;

	Sistema.App.PlayerEx.OpcionesBasicas.Activado(true);
	BotonBuscar->Texto(IDIOMA__BUSCAR);
	ListaDirectorios->Activado(true);
//	BotonAgregarDir->Activado(true);
	ComboDir->Activado(true);
	Marca->Activado(true);
	Marca2->Activado(true);
	if (ComboOrden->LongitudTexto() > 0) {
		ComboOrden->Activado(true);
		Marca3->Activado(true);
		Marca4->Activado(true);
		BotonAgregarDir->Activado(true);
	}
	Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Ocultar();
	Sistema.App.PlayerEx.BarraTareas.Resaltar();
}



LRESULT CALLBACK ControlPresentacionEx::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    DWL::ControlesEx::DWLListaEx *Lista = NULL;
    DWL::DWLString               *Texto = NULL;
    DWL::ControlesEx::DWLBarraEx *Barra = NULL;
	DWL::DWLString_char StrChar;
    switch (uMsg) {

        case MENSAJE_MEDIO_ENCONTRADO :
/*            Texto = reinterpret_cast<DWL::DWLString *>(wParam);
            Lista = EscenaActual->BuscarLista(ID_LISTA_MEDIOS_ENCONTRADOS);
            if (Lista != NULL) {
                Lista->AgregarItem(NULL, DWL::SO::DWLListaIconos::CargarIcono(Texto->Texto()), DWL_LISTAEX_PRINCIPIO, Texto->Texto());
                Lista->FinAgregarItems();
            }
            delete Texto;*/
            break;

		case MENSAJE_MEDIO_NO_EXISTE : // Mensaje para preguntar a la BD si existe el medio
			if (Sistema.App.BD.BuscarPath(reinterpret_cast<TCHAR *>(wParam)) != NULL)	return 0;
			else																		return true;
			

        case MENSAJE_BUSCAR_ARCHIVOS_TERMINADO :
			TerminarBusquedaArchivos();
            break;

        case MENSAJE_OBTENIENDO_DATOS_MEDIOS :
			EscenaActual->BuscarBarraProgreso(ID_BARRA_BUSCAR)->Maximo(static_cast<size_t>(wParam));
//    		Mostrar_ObteniendoDatos(static_cast<size_t>(wParam));
            break;

        case MENSAJE_MEDIO_ANALIZANDO :
			StrChar = IDIOMA__ANALIZANDO_;
			Sistema.App.VentanaLogs.AgregarMensaje(Log_BubaTronik, Log_Info, StrChar(), reinterpret_cast<DWL::DWLString *>(wParam)->Texto());
			delete reinterpret_cast<DWL::DWLString *>(wParam);
			break;

        case MENSAJE_MEDIO_ANALIZADO :
            Barra = EscenaActual->BuscarBarraProgreso(ID_BARRA_BUSCAR);
			Barra->Valor_SumarValor(1);
			Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Valor(Barra->Valor(), Barra->Maximo());
			Sistema.App.BD.AgregarMedioBD(reinterpret_cast<TMedio *>(wParam));
            break;
		case MENSAJE_MEDIO_INVALIDO :
            Barra = EscenaActual->BuscarBarraProgreso(ID_BARRA_BUSCAR);
			Barra->Valor_SumarValor(1);
			Sistema.App.PlayerEx.BarraTareas.BarraProgreso_Valor(Barra->Valor(), Barra->Maximo());
			break;
    }
    return DWL::Ventanas::DWLVentana::GestorMensajes(uMsg, wParam, lParam);
}


LRESULT ControlPresentacionEx::Evento_Temporizador(const UINT IDTimer) {
	if (IDTimer == DWL_TIMER_TOOLTIP) {
		if (EscenaActual->UltimaIDResaltada != -1) Evento_MostrarToolTip(EscenaActual->UltimaIDResaltada, 0);
	}
	BarraScrollEx_Evento_Temporizador(_hWnd, IDTimer, false);
    return 0;
}

LRESULT ControlPresentacionEx::Evento_BarraScrollEx_BotonEx_Click(const UINT IDBotonEx, const UINT nBoton) {
    DWL::ControlesEx::DWLListaEx *Lista = NULL;
    size_t                        i     = 0;
    switch (IDBotonEx) {
        case ID_BOTON_ELIMINAR_DIRECTORIO : 
            Lista = EscenaActual->BuscarLista(ID_DIRECTORIOS_BUSCAR);
            for (i = 0; i < Lista->TotalItems(); i++) {
                if (Lista->Item(i)->Seleccionado() == true) Sistema.App.Config.EliminarPathBuscar(Lista->Item(i)->Texto(0));
            }
            Lista->EliminarItemsSeleccionados();
            if (Lista->TotalItems() == 0) EscenaActual->BuscarBoton(ID_BOTON_BUSCAR2)->Activado(false);
            break;
    }
    return 0;
}



LRESULT ControlPresentacionEx::Evento_MarcaEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT cID) {
	ControlMarca *nMarca = EscenaActual->BuscarMarca(cID);
	switch (cID) {
		case ID_MARCA_TAG :
			nMarca->Marcar(!nMarca->Marcado());
			EscenaActual->BuscarMarca(ID_MARCA_PATH)->Marcar(!nMarca->Marcado());
			break;
		case ID_MARCA_PATH :
			nMarca->Marcar(!nMarca->Marcado());
			EscenaActual->BuscarMarca(ID_MARCA_TAG)->Marcar(!nMarca->Marcado());
			break;
		case ID_FILTRO_CARACTERES :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.Buscar_FiltroCaracteres = nMarca->Marcado();
			break;
		case ID_FILTRO_COINCIDENCIAS :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.Buscar_FiltroCoincidencias = nMarca->Marcado();
			break;
/*		case ID_OMITIR_TAGS1 :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.Buscar_OmitirTagsV1 = nMarca->Marcado();
			break;*/
		case ID_ASOCIAR_PREGUNTAR :
			EscenaActual->BuscarMarca(ID_ASOCIAR_SIEMPRE)->Marcar(false);
			EscenaActual->BuscarMarca(ID_ASOCIAR_NUNCA)->Marcar(false);
			Sistema.App.Config.AsociarArchivos = Respuesta_Aceptar;
			nMarca->Marcar(true);
			break;
		case ID_ASOCIAR_SIEMPRE :
			EscenaActual->BuscarMarca(ID_ASOCIAR_PREGUNTAR)->Marcar(false);
			EscenaActual->BuscarMarca(ID_ASOCIAR_NUNCA)->Marcar(false);
			nMarca->Marcar(true);
			Sistema.App.Config.AsociarArchivos = Respuesta_Aceptar_NoMostrar;
			break;
		case ID_ASOCIAR_NUNCA :
			EscenaActual->BuscarMarca(ID_ASOCIAR_SIEMPRE)->Marcar(false);
			EscenaActual->BuscarMarca(ID_ASOCIAR_PREGUNTAR)->Marcar(false);
			nMarca->Marcar(true);
			Sistema.App.Config.AsociarArchivos = Respuesta_Cancelar_NoMostrar;
			break;
		case ID_ASOCIAR_ADD :
			EscenaActual->BuscarMarca(ID_ASOCIAR_REP)->Marcar(false);
			nMarca->Marcar(true);
			Sistema.App.Asociar.AsignarReproducirPorDefecto(false);
			break;
		case ID_ASOCIAR_REP :
			EscenaActual->BuscarMarca(ID_ASOCIAR_ADD)->Marcar(false);
			nMarca->Marcar(true);
			Sistema.App.Asociar.AsignarReproducirPorDefecto(true);
			break;
		case ID_EXCLUIR_NOTA :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.ExcluirNota1 = nMarca->Marcado();
			break;
		case ID_OBTENER_TECLADO :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.TecladoGlobal = nMarca->Marcado();
			break;
		case ID_BUSCAR_ACTUALIZACIONES :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.BuscarActualizaciones = nMarca->Marcado();
			break;
		case ID_SIEMPRE_DELANTE :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.SiempreDelante = nMarca->Marcado();
			if (Sistema.App.Config.SiempreDelante == true) SetWindowPos(Sistema.App.PlayerEx.hWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			else								           SetWindowPos(Sistema.App.PlayerEx.hWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case ID_OPCIONES_MOSTRAR_TAG :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.MostrarTagsEnToolTips = nMarca->Marcado();
			break;
		case ID_OPCIONES_OCULTAR_MOUSE :
			nMarca->Marcar(!nMarca->Marcado());
			if (nMarca->Marcado() == true)	Sistema.App.Config.OcultarMouseEnVideo = 2000;
			else							Sistema.App.Config.OcultarMouseEnVideo = -1;
			break;
		case ID_OPCIONES_UTILIZAR_BARRA_WIN :
			nMarca->Marcar(!nMarca->Marcado());
			Sistema.App.Config.UtilizarBarraTituloWindows = nMarca->Marcado();
		
			int InicioControles = 24;
			RECT WR;
			GetWindowRect(Sistema.App.PlayerEx.hWnd(), &WR);
			RECT RC = { 0, 0, WR.right - WR.left, WR.bottom - WR.top  };
			DWL::GDI::DWLRegion Region;
//			GetClientRect(Sistema.App.PlayerEx.hWnd(), &RC);
			if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
				SetWindowLongPtr(Sistema.App.PlayerEx.hWnd(), GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
				Region.CrearRegionRectaRedondeada(0, 0, RC.right, RC.bottom - 0, 11, 11);
				DWL::GDI::DWLRegion Region2(4, 4, RC.right - 4, RC.bottom - 4, 11, 11);
				// Aqui tenemos el borde para detectar si el mouse esta en el.
				Region.CombinarRegion(&Region2, &Sistema.App.PlayerEx.Borde, RGN_XOR);			
				Sistema.App.PlayerEx.CM.Visible(true);
			}
			else {												
				InicioControles = 3;
				SetWindowLongPtr(Sistema.App.PlayerEx.hWnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
				Sistema.App.PlayerEx.AsignarRegion(NULL, false);
				Sistema.App.PlayerEx.CM.Visible(false);
			}
			
			Sistema.App.PlayerEx.Oscy.Mover(X_OSCY, InicioControles - 3, ANCHO_OSCY, ALTO_OSCY, false);
			Sistema.App.PlayerEx.Botones.Mover(X_BOTONERA, InicioControles, (ANCHO_BOTON_BOTONERA * 5), ALTO_BOTONERA, false);
			Sistema.App.PlayerEx.BotonShufleRepeat.Mover(X_BOTON_SHUFLE_REPEAT, InicioControles, ANCHO_BOTON_SHUFLE_REPEAT * 2 , ALTO_BOTON_SHUFLE_REPEAT);
			Sistema.App.PlayerEx.TiempoStrActual.Mover(X_TIEMPO_ACTUAL, InicioControles, ANCHO_TIEMPO_ACTUAL, ALTO_TIEMPO_ACTUAL);
			Sistema.App.PlayerEx.Volumen.Mover(X_VOLUMEN, InicioControles + 14, ANCHO_VOLUMEN, ALTO_VOLUMEN);
			//Sistema.App.PlayerEx.Evento_CambioTam(NULL, RC.right, RC.bottom);

//			Sistema.App.PlayerEx.Evento_ValidarControles();
			InvalidateRect(Sistema.App.PlayerEx.hWnd(), &RC, FALSE);
			SetWindowPos(Sistema.App.PlayerEx.hWnd(), NULL, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			if (Sistema.App.Config.UtilizarBarraTituloWindows == false) {
				Region.AsignarRegionVentana(Sistema.App.PlayerEx.hWnd(), true);
			}
			GetClientRect(GetParent(_hWnd), &RC);
			Sistema.App.PlayerEx.Evento_CambioTam(NULL, RC.right, RC.bottom);


			break;
	}
    return 0;
}


void ControlPresentacionEx::EnumerarSkins(DWL::ControlesEx::DWLEdicionDesplegableEx *nCombo) {
	DWL::DWLString   Path    = Sistema.App.AppPath(); Path += TEXT("Graficos\\*.*");
	WIN32_FIND_DATA  FindInfoPoint;                                                                        //
	HANDLE           hFind   = NULL;                                                                                //
	TCHAR            Ext[16]  = TEXT("");
	TCHAR			 Nombre[512];
	nCombo->BorrarTodo();
	hFind = FindFirstFile(Path(), &FindInfoPoint);                                              //
	while (FindNextFile(hFind, &FindInfoPoint) != 0) {                                                     //
		size_t TamStr = DWLStrLen(FindInfoPoint.cFileName);
		if (TamStr > 15) DWLStrCopy(Ext, 16, &FindInfoPoint.cFileName[TamStr -15]);
		else			 Ext[0] = 0;
		if (DWLStrCmpi(TEXT("BubaTronik_Skin"), Ext) == 0) {
			DWLStrCopy(Nombre, 512, FindInfoPoint.cFileName);
			Nombre[TamStr - 16] = 0;
			nCombo->AgregarItem(Nombre);
		}  
	}
	FindClose(hFind);                                                                                      //
}


void ControlPresentacionEx::EnumerarIdiomas(DWL::ControlesEx::DWLEdicionDesplegableEx *nCombo) {
	HINSTANCE        Lib     = NULL;
	TpNombre        *Nombre  = NULL;
	TpVersion       *Version = NULL;
	TpIcono         *Icono   = NULL;
	DWL::DWLString   Path    = Sistema.App.AppPath(); Path += TEXT("Idiomas\\*.*");
	DWL::DWLString   LibPath;
	WIN32_FIND_DATA  FindInfoPoint;																			//
	HANDLE           hFind   = NULL;                                                                        //
	TCHAR            Ext[4]  = TEXT("");
	nCombo->BorrarTodo();
	hFind = FindFirstFile(Path(), &FindInfoPoint);													//
	while (FindNextFile(hFind, &FindInfoPoint) != 0) {														//
		LibPath = Sistema.App.AppPath(); LibPath += TEXT("Idiomas\\"); LibPath += FindInfoPoint.cFileName;
		DWLStrCopy(Ext, 4, &LibPath[LibPath.Tam() -3]);
		if (DWLStrCmpi(TEXT("dll"), Ext)  == 0) {
			Lib = LoadLibrary(LibPath()); 
			if (Lib) {
				Nombre  = (TpNombre *)GetProcAddress((HMODULE)Lib, "Nombre");
				Version = (TpVersion *)GetProcAddress((HMODULE)Lib, "Version");
				Icono  = (TpIcono *)GetProcAddress((HMODULE)Lib, "Icono");
				if (Nombre != NULL && Version != NULL) {
					if (Version() == APP_IDIOMA_VER) 	
						nCombo->AgregarItem(Nombre(), Icono());
				}
				FreeLibrary((HMODULE)Lib);
			}
		}
	}
	FindClose(hFind);                                                                                      
	nCombo->FinAgregarItems();
}

LRESULT ControlPresentacionEx::Evento_OcultarToolTip(const UINT IDControl, LPARAM Param) {
	Sistema.App.ToolTip.Destruir();
	return 0;
}

LRESULT ControlPresentacionEx::Evento_MostrarToolTip(const UINT IDControl, LPARAM Param) {
//	static DWL::ControlesEx::DWLToolTipEx ToolTip;
	DWL::DWLString Str;
	UINT           Num = 0;
	TMedio        *nMedio = NULL;
	Sistema.App.ToolTip.BorrarMemoria();
	Sistema.App.ToolTip.AgregarIconoTexto(IDI_AYUDA, 16, 16, IDIOMA__AYUDA_RAPIDA);
	switch (IDControl) {
		// Inicio
		case ID_GENERAR_LISTA : // Tooltips para el boton Generar de MostrarInicio()
			if (EscenaActual->BuscarBoton(ID_GENERAR_LISTA)->Activado() == FALSE) {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__DEBES_ELEGIR_UN_TIPO_DE_LISTA_PARA_PODER_GENERARLA);
			}
			else {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__AL_PRESIONAR_ESTE_BOTON_CREARAS_UNA_NUEVA_LISTA);
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__DE_UNAS_50_CANCIONES_DEL_TIPO_ELEGIDO);
			}
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_TIPOS_LISTA :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTE_CONTROL_TE_PERMITE_ELEGIR_ENTRE_VARIOS_TIPOS_DE_LISTAS_PREFABRICADAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LISTA_DE_CANCIONES_ALEATORIAS_BASADAS_EN_GENERO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LISTA_DE_CANCIONES_ALEATORIAS_BASADAS_EN_GRUPO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LISTA_DE_CANCIONES_ALEATORIAS_BASADAS_EN_DISCO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LISTA_DE_CANCIONES_ALEATORIAS_SIN_BASARSE_EN_NADA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LISTA_CON_LAS_CANCIONES_MAS_ESCUCHADAS_SI_SE_HA_ESCUCHADO_ALGUNA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LISTA_CON_CANCIONES_DE_5_ESTRELLAS_SI_SE_HA_ASIGNADO_ALGUNA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__NOTA_ESTAS_LISTAS_COMO_MAXIMO_PODRAN_TENER_50_CANCIONES);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_SUGERENCIA_GENERO1 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CADA_VEZ_QUE_ACCEDAS_A_LA_SECCION_DE_INICIO_SE_MOSTRARAN_NUEVAS_SUGERENCIAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_GENERO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.ObtenerSugerenciaGenero1()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_SUGERENCIA_GENERO2 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CADA_VEZ_QUE_ACCEDAS_A_LA_SECCION_DE_INICIO_SE_MOSTRARAN_NUEVAS_SUGERENCIAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_GENERO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.ObtenerSugerenciaGenero2()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_SUGERENCIA_GRUPO1 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CADA_VEZ_QUE_ACCEDAS_A_LA_SECCION_DE_INICIO_SE_MOSTRARAN_NUEVAS_SUGERENCIAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_GRUPO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.ObtenerSugerenciaGrupo1()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_SUGERENCIA_GRUPO2 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CADA_VEZ_QUE_ACCEDAS_A_LA_SECCION_DE_INICIO_SE_MOSTRARAN_NUEVAS_SUGERENCIAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_GRUPO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.ObtenerSugerenciaGrupo2()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_SUGERENCIA_DISCO1 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CADA_VEZ_QUE_ACCEDAS_A_LA_SECCION_DE_INICIO_SE_MOSTRARAN_NUEVAS_SUGERENCIAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_DISCO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.ObtenerSugerenciaDisco1()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_SUGERENCIA_DISCO2 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CADA_VEZ_QUE_ACCEDAS_A_LA_SECCION_DE_INICIO_SE_MOSTRARAN_NUEVAS_SUGERENCIAS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_DISCO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.ObtenerSugerenciaDisco2()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_GENERO_MAS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PARA_DETERMINAR_QUE_GENERO_SE_HA_ESCUCHADO_SE_HACE_UNA_LISTA_CON_TODOS_LOS_GENEROS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_LA_QUE_SE_SUMAN_LAS_VECES_QUE_SE_HA_ESCUCHADO_CADA_CANCION_DE_ESE_GENERO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_GENERO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.GeneroMasEscuchadoStrTp()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_GRUPO_MAS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PARA_DETERMINAR_QUE_GRUPO_SE_HA_ESCUCHADO_SE_HACE_UNA_LISTA_CON_TODOS_LOS_GRUPOS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_LA_QUE_SE_SUMAN_LAS_VECES_QUE_SE_HA_ESCUCHADO_CADA_CANCION_DE_ESE_GRUPO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_GRUPO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.GrupoMasEscuchadoStrTp()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_DISCO_MAS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PARA_DETERMINAR_QUE_DISCO_SE_HA_ESCUCHADO_SE_HACE_UNA_LISTA_CON_TODOS_LOS_DISCOS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_LA_QUE_SE_SUMAN_LAS_VECES_QUE_SE_HA_ESCUCHADO_CADA_CANCION_DE_ESE_DISCO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAZ_CLICK_PARA_CREAR_UNA_NUEVA_LISTA_CON_VARIAS_CANCIONES_DEL_DISCO);
			Sistema.App.ToolTip.AgregarTextoNegrita(Sistema.App.BD.DiscoMasEscuchadoStrTp()->Nombre());
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_MEDIO_RECIENTE1 : case CP_MEDIO_RECIENTE2 : case CP_MEDIO_RECIENTE3 : case CP_MEDIO_RECIENTE4 : case CP_MEDIO_RECIENTE5 :
		case CP_MEDIO_RECIENTE6 : case CP_MEDIO_RECIENTE7 : case CP_MEDIO_RECIENTE8 : case CP_MEDIO_RECIENTE9 : case CP_MEDIO_RECIENTE10 :
			Num = IDControl - CP_MEDIO_RECIENTE1;
			nMedio = Sistema.App.BD.MedioReciente(Num);
			if (nMedio != NULL) {
				if (nMedio->Nodo != NULL) Sistema.App.ArbolBaseDatos.MostrarToolTipNodo(nMedio->Nodo);
			}
			break;
		// Buscar archivos
		case ID_FILTRO_COINCIDENCIAS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ACTIVANDO_ESTA_OPCION_EL_REPRODUCTOR_REALIZARA_UNA_COMPROBACION_ENTRE_TODOS_SUS_GENEROS_GRUPOS_Y_DISCOS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DE_FORMA_QUE_SOLUCIONE_EL_80_DE_ERRATAS_EN_LOS_TAGS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PONGAMOS_POR_EJEMPLO_QUE_INICIALMENTE_SE_ENCUENTRAN_LOS_SIGUIENTES_DOS_GRUPOS);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__IRONMAIDEN_Y_IRON_MAIDEN); 
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_OS_FIJAIS_SON_PRACTICAMENTE_LO_MISMO_PERO_UNO_TIENE_UN_ESPACIO_EN_MEDIO_Y_EL_OTRO_NO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__UNA_COSA_TAN_SIMPLE_COMO_ESTA_HACE_QUE_EL_REPRODUCTOR_INTERPRETE_ESOS_RESULTADOS_COMO_2_GRUPOS_DISTINTOS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__Y_POR_ESO_SE_HA_CREADO_ESTA_OPCION_PARA_INTENTAR_CORREGIR_AL_MAXIMO_ESTE_TIPO_DE_ERRORES);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ACTIVANDO_ESTA_OPCION_EL_TIEMPO_TOTAL_DE_LA_BUSQUEDA_DE_ARCHIVOS_SE_INCREMENTA_NOTABLEMENTE);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_FILTRO_CARACTERES :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ACTIVANDO_ESTA_OPCION_EL_REPRODUCTOR_FILTRARA_DE_TODOS_LOS_NOMBRES_LOS_SIGUIENTES_CARACTERES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CARACTERESRAROS_ADEMAS_DE_LAS_TILDES_Y_SIMBOLOS_DE_PUNTUACION);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CON_ESTO_CONSEGUIMOS_MUCHA_MAS_PRECISION_A_LA_HORA_DE_IDENTIFICAR_GENEROS_GRUPOS_Y_DISCOS);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ES_MUY_RECOMENDABLE_DEJAR_ACTIVADA_ESTA_OPCION_ADEMAS_DE_QUE_NO_AFECTARA_EL_TIEMPO_DE_LA_BUSQUEDA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BOTON_BUSCAR2 :
			if (EscenaActual->BuscarBoton(ID_BOTON_BUSCAR2)->Activado() == TRUE) {
				if (DWLStrCmp(EscenaActual->BuscarBoton(ID_BOTON_BUSCAR2)->Texto(), IDIOMA__CANCELAR) == 0) {
					Sistema.App.ToolTip.AgregarTexto(IDIOMA__PRESIONA_ESTE_BOTON_PARA_CANCELAR_LA_BUSQUEDA);
				}
				else {
					Sistema.App.ToolTip.AgregarTexto(IDIOMA__PRESIONA_ESTE_BOTON_PARA_EMPEZAR_LA_BUSQUEDA);
					Sistema.App.ToolTip.AgregarTexto(IDIOMA__ANTES_DE_HACERLO_DEBERIAS_ASEGURARTE_DE_QUE_HAS_ANADIDO_LAS_RUTAS_QUE_DESEAS_BUSCAR_EN_LA_LISTA);
				}
			}
			else { 
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__PARA_BUSCAR_PRIMERO_TIENES_QUE_AGREGAR_UNA_RUTA_A_LA_LISTA_Y_ELEGIR_EL_TIPO_DE_ORDEN);
			}
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BOTON_AGREGAR_DIR :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__AL_PULSAR_ESTE_BOTON_ANADIRAS_EL_DIRECTORIO_DEL_CUADRO_DESPLEGABLE_A_LA_LISTA_DE_DIRECTORIOS);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__NO_ES_RECOMENDABLE_ANADIR_LA_RAIZ_DE_NINGUN_DISCO_DURO_EXCEPTUANDO_EL_CASO_DE_QUE_ESE_DISCO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SEA_UNICAMENTE_PARA_MEDIOS_DE_AUDIO_Y_VIDEO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_COMBO_DIR :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_DESPLEGABLE_PUEDES_BUSCAR_DIRECTORIOS_PARA_ANADIR_A_LA_LISTA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LOS_DIRECTORIOS_DE_LA_LISTA_DE_ARRIBA_SON_LOS_QUE_ESCANEARA_EL_REPRODUCTOR_PARA_ENCONTRAR_NUEVOS_MEDIOS);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_DIRECTORIOS_BUSCAR :			
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTA_LISTA_CONTIENE_LOS_DIRECTORIOS_EN_LOS_QUE_EL_REPRODUCTOR_BUSCARA_NUEVOS_MEDIOS_REPRODUCIBLES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__POR_DEFECTO_SE_HAN_ANADIDO_LAS_RUTAS_PREDETERMINADAS_DE_WINDOWS_SI_DESEAS_ELIMINARLAR_PUEDES_USAR);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_BOTON_CON_UNA_CRUZ_ROJA_SITUADO_EN_LA_PARTE_INFERIOR_DERECHA_DE_ESTA_LISTA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_QUIERES_AGREGAR_DIRECTORIOS_DEBES_UTILIZAR_EL_CONTROL_DESPLEGABLE_QUE_HAY_MAS_ABAJO_PARA_SELECCIONAR);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_DIRECTORIO_QUE_QUIERAS_Y_POR_ULTIMO_DEBES_PRESIONAR_EL_BOTON_AGREGAR_PARA_INSERTARLO_EN_ESTA_LISTA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_COMBO_ORDEN_DIR :
			if (EscenaActual->BuscarEdicionDesplegable(ID_COMBO_ORDEN_DIR)->Activado() == TRUE) {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTAS_NORMAS_SE_USAN_A_LA_HORA_DE_OBTENER_LOS_DATOS_DE_UN_MEDIO_PARA_DETERMINAR_MEJOR_SUS_DATOS);
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_POR_EJEMPLO_TENEMOS_EL_DIRECTORIO_C_MP3_Y_DENTRO_TENEMOS_MUSICA_ESTRUCTURADA_SEGUN_GRUPO_DISCO_PODEMOS);
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__ENCONTRAR_EL_CASO_EN_QUE_ADEMAS_EXISTA_UN_DIRECTORIO_PARA_DISTINTOS_CDS_EJEMPLO);
				Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__C_MP3_BARON_ROJO_BARON_AL_ROJO_VIVO_CD1);
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_ESTE_CASO_CONCRETO_SI_ESTAMOS_BUSCANDO_DATOS_BASANDONOS_EN_EL_PATH_Y_NO_SE_HA_ELEGIDO_EL_TIPO_GRUPO_DISCO);
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_REPRODUCTOR_PENSARA_QUE_EL_NOMBRE_DEL_GRUPO_ES_BARON_AL_ROJO_VIVO_Y_EL_DISCO_CD1_COSA_QUE_ES_INCORRECTA);
			}
			else {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__PRIMERO_DEBES_SELECCIONAR_UN_DIRECTORIO_ANTES_DE_PODER_ESTABLECER_EL_TIPO_DE_NORMA_A_UTILIZAR);
			}
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_MARCA_PATH :
			if (EscenaActual->BuscarMarca(ID_MARCA_PATH)->Activado() == TRUE) {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__MARCANDO_ESTA_OPCION_EL_REPRODUCTOR_DARA_PRIORIDAD_A_LOS_DATOS_DEL_PATH_POR_ENCIMA_DE_LOS_DEL_TAG_PARA_EL_DIRECTORIO_QUE_SE_VA_A_AGREGAR);
			}
			else {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__PRIMERO_DEBES_SELECCIONAR_UN_DIRECTORIO_ANTES_DE_PODER_ESTABLECER_LA_PRIORIDAD_DE_LOS_DATOS);
			}
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_MARCA_TAG :
			if (EscenaActual->BuscarMarca(ID_MARCA_TAG)->Activado() == TRUE) {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__MARCANDO_ESTA_OPCION_EL_REPRODUCTOR_DARA_PRIORIDAD_A_LOS_DATOS_DEL_TAG_POR_ENCIMA_DE_LOS_DEL_PATH_PARA_EL_DIRECTORIO_QUE_SE_VA_A_AGREGAR);
			}
			else {
				Sistema.App.ToolTip.AgregarTexto(IDIOMA__PRIMERO_DEBES_SELECCIONAR_UN_DIRECTORIO_ANTES_DE_PODER_ESTABLECER_LA_PRIORIDAD_DE_LOS_DATOS);
			}
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		// Opciones
		case ID_LISTA_IDIOMAS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTA_LISTA_CONTIENE_LOS_IDIOMAS_DISPONIBLES_PARA_ESTE_REPRODUCTOR);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_ESTAS_INTERESADO_EN_AGREGAR_UN_NUEVO_IDIOMA_A_ESTA_APLICACION_PONTE_EN_CONTACTO_CON_DEVILDREY33);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__A_TRAVES_DEL_FORO_DE_HTTP_WWW_DEVILDREY33_ES);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BUSCAR_ACTUALIZACIONES :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ACTIVANDO_ESTA_OPCION_EL_REPRODUCTOR_SE_CONECTARA_UNA_VEZ_AL_DIA_COMO_MAXIMO_PARA_COMPROBAR_SI_EXISTEN);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__NUEVAS_ACTUALIZACIONES_POR_INSTALAR);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_SIEMPRE_DELANTE :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ACTIVANDO_ESTA_OPCION_EL_REPRODUCTOR_SE_QUEDARA_SIEMPRE_DELANTE_DE_LAS_OTRAS_VENTANAS);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_LISTA_TIPOS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTA_LISTA_CONTIENE_TODOS_LOS_TIPOS_DE_AUDIO_Y_DE_VIDEO_QUE_EL_REPRODUCTOR_ACEPTA_POR_DEFECTO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PUEDES_ELIMINAR_AQUELLAS_ESTENSIONES_QUE_NO_TE_INTERESE_QUE_EL_REPRODUCTOR_ACEPTE);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BOTON_ELIMINAR_TIPOS_AUDIO :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTE_BOTON_ELIMINARA_DE_LA_LISTA_TODOS_LOS_TIPOS_QUE_SEAN_PREDEFINIDOS_PARA_AUDIO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BOTON_ELIMINAR_TIPOS_VIDEO :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTE_BOTON_ELIMINARA_DE_LA_LISTA_TODOS_LOS_TIPOS_QUE_SEAN_PREDEFINIDOS_PARA_VIDEO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BOTON_ELIMINAR_TIPOS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTE_BOTON_ELIMINARA_DE_LA_LISTA_TODOS_LOS_TIPOS_QUE_SE_ENCUENTREN_SELECCIONADOS_EN_ESTE_MOMENTO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_BOTON_RESTAURAR_TIPOS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ESTE_BOTON_RESTAURARA_TODOS_LOS_TIPOS_QUE_PUEDE_USAR_BUBATRONIK_POR_DEFECTO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_ASOCIAR_PREGUNTAR :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SELECCIONADO_ESTA_OPCION_EL_REPRODUCTOR_MIRARA_CADA_VEZ_QUE_SE_INICIE_SI_ESTA_ASOCIADO_A_LOS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__TIPOS_DE_LA_LISTA_EN_CASO_DE_NO_ESTAR_ASOCIADO_A_ALGUNO_DE_ESOS_TIPOS_EL_REPRODUCTOR_MOSTRARA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__UN_MENSAJE_QUE_TE_PEDIRA_SI_DESEAS_ASOCIAR_EL_REPRODUCTOR_A_LOS_TIPOS_DE_LA_LISTA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_ASOCIAR_SIEMPRE :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SELECCIONADO_ESTA_OPCION_EL_REPRODUCTOR_MIRARA_CADA_VEZ_QUE_SE_INICIE_SI_ESTA_ASOCIADO_A_LOS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__TIPOS_DE_LA_LISTA_EN_CASO_DE_NO_ESTAR_ASOCIADO_A_ALGUNO_DE_ESOS_TIPOS_EL_REPRODUCTOR_SE_ASOCIARA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__A_ELLOS_AUTOMATICAMENTE_SIN_PREGUNTAR_NADA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_ASOCIAR_NUNCA :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SELECCIONANDO_ESTA_OPCION_EL_REPRODUCTOR_OMITIRA_ESCANEAR_EL_REGISTRO_PARA_DETERMINAR_SI_ESTA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__ASOCIADO_O_NO_A_LOS_TIPOS_DE_ARCHIVO_DE_LA_LISTA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_ASOCIAR_ADD :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__AL_EJECUTAR_UN_ARCHIVO_ASOCIADO_AL_REPRODUCTOR_DESDE_EL_EXPLORADOR_SE_ANADIRA_A_LA_LISTA_SIN);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__INTERRUMPIR_LA_REPRODUCCION_ACTUAL);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_ASOCIAR_REP :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__AL_EJECUTAR_UN_ARCHIVO_ASOCIADO_AL_REPRODUCTOR_DESDE_EL_EXPLORADOR_SE_ANADIRA_A_LA_LISTA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__INTERRUMPIENDO_LA_REPRODUCCION_ACTUAL_Y_SE_PONDRA_A_REPRODUCIR_EL_PRIMER_MEDIO_ANADIDO_DESDE);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_EXPLORADOR);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		// Opciones aspecto
		case ID_OPCIONES_FUENTE :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_ESTA_LISTA_PUEDES_SELECCIONAR_EL_TAMANO_DE_LA_FUENTE_QUE_MAS_SE_AJUSTE_A_TUS_NECESIDADES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__HAY_TRES_TIPOS_DE_TAMANO_PEQUENO_11PX_MEDIANO_13PX_Y_GRANDE_15_PX);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_SKIN :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_ESTA_LISTA_PUEDES_SELECCIONAR_LA_CONFIGURACION_DE_COLORES_QUE_MAS_TE_GUSTE);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_DESEAS_CREAR_UNA_CONFIGURACION_DE_COLORES_PARA_BUBATRONIK_Y_NO_LO_VES_MUY_CLARO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PUEDES_CONSULTAR_TUS_DUDAS_EN_EL_FORO_DE_HTTP_WWW_DEVILDREY33_ES);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_OSCY :			
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_ESTA_LISTA_PUEDES_SELECCIONAR_EL_TAMANO_DE_LAS_BARRITAS_DEL_ESPECTRO_GRAFICO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_PEQUENO_HACE_1_PIXEL_DE_ANCHO_EL_MEDIANO_2_PIXELES_Y_EL_GRANDE_3_PIXELES);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_UTILIZAR_BARRA_WIN :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__MARCANDO_ESTA_OPCION_EL_REPRODUCTOR_UTILIZARA_LA_BARRA_DE_WINDOWS_ESTANDAR_CON_SUS_BORDES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_PRINCIPIO_EL_REPRODUCTOR_SE_DISEÑO_PARA_WINDOWS_XP_POR_LO_QUE_SE_CREO_UNA_BARRA_DE_TITULO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DISTINTA_A_LA_DE_WINDOWS_QUE_QUEDABA_MAS_ACORDE_CON_EL_REPRODUCTOR);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_ESTAS_USANDO_WINDOWS_VISTA_O_WINDOWS_7_SE_RECOMIENDA_UTILIZAR_LA_BARRA_DE_TITULO_DE_WINDOWS);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PARA_NO_PERDER_LAS_CAPACIDADES_EXTENDIDAS_DE_MAXIMIZADO_PARCIAL);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		// Opciones base de datos
		case ID_OPCIONES_MOSTRAR_TAG :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__MARCANDO_ESTA_OPCION_SE_CARGARAN_AL_VUELO_Y_SE_MOSTRARAN_LOS_TAG_DE_LAS_CANCIONES_AL_MOSTRAR);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_TOOLTIP_INFORMATIVO);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CON_EL_CUARTO_BOTON_DEL_MOUSE_PUEDES_HACER_APARECER_ESOS_TOOLTIPS_DENTRO_DE_LA_LISTA_Y_DE_LA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__BASE_DE_DATOS_SIN_TENER_QUE_ESPERAR_2_SEGUNDOS_ENCIMA_DE_LA_CANCION);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		// Opciones lista
		case ID_LISTA_INICIO :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EN_ESTA_LISTA_DESPLEGABLE_PUEDES_SELECCIONAR_QUE_QUIERES_QUE_HAGA_EL_REPRODUCTOR_AL_INICIARSE);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CREAR_UNA_LISTA_PARTIENDO_DE_UN_GENERO_ALEATORIO_CON_UN_MAXIMO_DE_50_CANCIONES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CREAR_UNA_LISTA_PARTIENDO_DE_UN_GRUPO_ALEATORIO_CON_UN_MAXIMO_DE_50_CANCIONES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CREAR_UNA_LISTA_PARTIENDO_DE_UN_DISCO_ALEATORIO_CON_UN_MAXIMO_DE_50_CANCIONES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CREAR_UNA_LISTA_ALEATORIA_CON_UN_MAXIMO_DE_50_CANCIONES);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CUALQUIER_COSA_UNA_DE_LAS_4_OPCIONES_ANTERIORES_ALEATORIAMENTE);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__CREAR_UNA_LISTA_VACIA);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__SI_EL_REPRODUCTOR_SE_INICIA_DESDE_EL_EXPLORADOR_PORQUE_HAS_ABIERTO_UN_MEDIO_DESDE_ALLI_LA_CREACION);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DE_LA_LISTA_ALEATORIA_SE_OMITIRA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_EXCLUIR_NOTA :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__MARCANDO_ESTA_OPCION_EL_REPRODUCTOR_EVITARA_CREAR_LISTAS_ALEATORIAS_CON_CANCIONES_QUE_SOLO_TENGAN_UNA_ESTRELLA);
			Sistema.App.ToolTip.AgregarTextoNegrita(IDIOMA__NOTA_);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__EL_SISTEMA_DE_NOTAS_PARA_LOS_MEDIOS_POR_DEFECTO_LE_DA_2_ESTRELLAS_A_TODOS_LOS_MEDIOS_SI_DESEAS_QUE_UN_MEDIO);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__TENGA_MAS_NOTA_HASTA_5_ESTRELLAS_PUEDES_CAMBIARLE_LA_NOTA_UTILIZANDO_EL_MENU_DESPLEGABLE_DESDE_LA_LISTA_O_LA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__BASE_DE_DATOS_);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		// Opciones teclado
		case ID_OPCIONES_TECLA_PLAY :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_PUEDES_SELECCIONAR_LA_TECLA_QUE_SE_USARA_PARA_REALIZAR_LA_ACCION_PLAY_PAUSA);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_TECLA_STOP :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_PUEDES_SELECCIONAR_LA_TECLA_QUE_SE_USARA_PARA_REALIZAR_LA_ACCION_STOP);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_TECLA_VOLUMEN_MAS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_PUEDES_SELECCIONAR_LA_TECLA_QUE_SE_USARA_PARA_REALIZAR_LA_ACCION_SUBIR_EL_VOLUMEN);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_TECLA_VOLUMEN_MENOS :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_PUEDES_SELECCIONAR_LA_TECLA_QUE_SE_USARA_PARA_REALIZAR_LA_ACCION_BAJAR_EL_VOLUMEN);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_TECLA_SIGUIENTE :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_PUEDES_SELECCIONAR_LA_TECLA_QUE_SE_USARA_PARA_REALIZAR_LA_ACCION_SIGUIENTE_MEDIO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case ID_OPCIONES_TECLA_ANTERIOR :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__DESDE_ESTE_CONTROL_PUEDES_SELECCIONAR_LA_TECLA_QUE_SE_USARA_PARA_REALIZAR_LA_ACCION_ANTERIOR_MEDIO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
//		case ID_OBTENER_TECLADO : // Obtener teclado fuera del reproductor
//			break;
		// Opciones video
		case ID_OPCIONES_OCULTAR_MOUSE :			
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__MARCANDO_ESTA_OPCION_EN_EL_MODO_DE_PANTALLA_COMPLETA_DENTRO_DE_UN_VIDEO_EL_MOUSE_SE_OCULTARA);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PASADOS_2_SEGUNDOS_SIN_HACER_NADA_ESTO_OCURRIRA_SIEMPRE_QUE_EL_MOUSE_NO_ESTE_EN_LA_PARTE_DE_LOS_CONTROLES);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_FMOD :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PAGINA_WEB_DE_ESTA_FABULOSA_LIBRERIA_PARA_AUDIO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_LIBVLC :			
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PAGINA_WEB_DE_ESTA_EXCELENTE_LIBRERIA_PARA_AUDIO_VIDEO);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_DEVILDREY33 :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PAGINA_WEB_DE_DEVILDREY33);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_MALEIT :			
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PAGINA_WEB_DEL_GRUPO_DE_ESTE_CRACK_A_LA_BATERIA_MALEIT);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_BUBATRONIK :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PAGINA_WEB_PRINCIPAL_DE_ESTE_REPRODUCTOR);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_DWL :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__PAGINA_WEB_DE_LA_DREY_WINDOWS_LIB);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__LA_DWL_ES_UNA_LIBRERIA_PARA_HACER_APLICACIONES_BAJO_WINDOWS_EN_C);
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__QUE_SE_DISTRIBUYE_BAJO_LICENCIA_GPL);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
		case CP_URL_FORO :
			Sistema.App.ToolTip.AgregarTexto(IDIOMA__FORO_DE_HTTP_WWW_DEVILDREY33_ES);
			Sistema.App.ToolTip.MostrarToolTipEx();
			break;
	}
	return 0;
}


void ControlPresentacionEx::Evento_ObjetoPulsado(const UINT Boton, const UINT IDP) {
	static VentanaMensaje_Aceptar MsgAceptar;
	if (Boton == 0) {
		UINT	Num    = 0;
		TMedio *nMedio = NULL;

		switch (IDP) {
			case CP_SUGERENCIA_GENERO1 :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.GeneroAleatorio(Sistema.App.BD.ObtenerSugerenciaGenero1());
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;
			case CP_SUGERENCIA_GRUPO1 :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.GrupoAleatorio(Sistema.App.BD.ObtenerSugerenciaGrupo1());
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;
			case CP_SUGERENCIA_DISCO1 :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.DiscoAleatorio(Sistema.App.BD.ObtenerSugerenciaDisco1());
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;

			case CP_SUGERENCIA_GENERO2 :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.GeneroAleatorio(Sistema.App.BD.ObtenerSugerenciaGenero2());
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;
			case CP_SUGERENCIA_GRUPO2 :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.GrupoAleatorio(Sistema.App.BD.ObtenerSugerenciaGrupo2());
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;
			case CP_SUGERENCIA_DISCO2 :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.DiscoAleatorio(Sistema.App.BD.ObtenerSugerenciaDisco2());
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;

			case CP_GENERO_MAS :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.GeneroMasEscuchado();
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;
			case CP_GRUPO_MAS :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.GrupoMasEscuchado();
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;
			case CP_DISCO_MAS :
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				Sistema.App.BD.DiscoMasEscuchado();
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
				break;

			case ID_ICONO_BD:
				Visible(false);
				Sistema.App.ListaC.Visible(false);
				Sistema.App.MediaPlayer.Visible(false);
				Sistema.App.ArbolBaseDatos.Visible(true);
                SetFocus(Sistema.App.ArbolBaseDatos.hWnd());
				break;

			case CP_MEDIO_RECIENTE1 : case CP_MEDIO_RECIENTE2 : case CP_MEDIO_RECIENTE3 : case CP_MEDIO_RECIENTE4 : case CP_MEDIO_RECIENTE5 :
			case CP_MEDIO_RECIENTE6 : case CP_MEDIO_RECIENTE7 : case CP_MEDIO_RECIENTE8 : case CP_MEDIO_RECIENTE9 : case CP_MEDIO_RECIENTE10 :
				Num = IDP - CP_MEDIO_RECIENTE1;
				nMedio = Sistema.App.BD.MedioReciente(Num);
				if (nMedio != NULL) {
					if (Sistema.App.UnidadesDisco.Buscar_Numero_Serie(nMedio->ID_Disco) != NULL) {
						Sistema.App.Media.Stop();
						Sistema.App.ListaC.EliminarTodosLosMedios();
						Sistema.App.ListaC.AgregarMedio(nMedio);
						Sistema.App.ListaC.ActualizarTodo();
						Sistema.App.Media.AbrirPrimerMedioLista();
						Mostrar_Inicio();
					}
					else {
						MsgAceptar.MostrarMensaje(IDIOMA__ERROR_LA_UNIDAD_QUE_CONTIENE_EL_MEDIO_NO_SE_ENCUENTRA_DISPONIBLE_POR_ELLO_ES_IMPOSIBLE_REPRODUCIR_EL_MEDIO, IDIOMA__ERROR, 0);
					}
				}
				break;
			case CP_URL_FMOD :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://www.fmod.org"), NULL, NULL, SW_SHOW);
				break;
			case CP_URL_LIBVLC :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://www.videolan.org/"), NULL, NULL, SW_SHOW);
				break;
			case CP_URL_DEVILDREY33 :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://www.devildrey33.es"), NULL, NULL, SW_SHOW);
				break;
			case CP_URL_DWL :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://dwl.devildrey33.es"), NULL, NULL, SW_SHOW);
				break;
			case CP_URL_BUBATRONIK :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://bubatronik.devildrey33.es"), NULL, NULL, SW_SHOW);
				break;
			case CP_URL_FORO :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://foro.devildrey33.es"), NULL, NULL, SW_SHOW);
				break;
			case CP_URL_MALEIT :
				ShellExecute(NULL, TEXT("Open"), TEXT("http://www.maleit.es"), NULL, NULL, SW_SHOW);
				break;
		}
	}
}


ControlPresentacionEx::ControlPresentacionEx(void) {
	EscenaActual = NULL;
	BuscarArchivos = NULL;
}


ControlPresentacionEx::~ControlPresentacionEx(void) {
	delete EscenaActual;
}


void ControlPresentacionEx::Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const UINT nID) {
	CrearVentana(hWndParent, TEXT("ControlPresentacionEx"), WS_CHILD | WS_VISIBLE, NULL, cX, cY, cAncho, cAlto, NULL); 
	RECT RC = { 0, 0, cAncho, cAlto };
	BarraScrollEx_CrearV(TRUE, cAlto, cAlto, 0);
//	BarraScrollEx_CrearH(_hWnd, &RC, DWL_BARRASCROLLEX_AUTOMATICO, 1000, 500, 0, &Sistema.App.Sk.ColoresScrollBarEx);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	FC_Menu.Texto(IDIOMA__EDITAR_ANOMALIAS);
	FC_Menu.AgregarMenu(ID_FC_ASIGNAR_NOMBRE,	IDIOMA__ASIGNAR_NOMBRE	, IDI_PLAY);
	FC_Menu.AgregarMenu(ID_FC_MODIFICAR_NOMBRE, IDIOMA__MODIFICAR_NOMBRE, IDI_LISTA_MODIFICARNOMBRE);
	FC_Menu.AgregarMenu(ID_FC_ELIMINAR,			IDIOMA__ELIMINAR		, IDI_ELIMINAR);

}

void ControlPresentacionEx::Repintar(const bool Forzar) {
	if (Forzar == false) return;
	DWL::GDI::DWLhDC hDC(_hWnd);
    PAINTSTRUCT PS;
	Evento_Pintar(hDC(), PS);
//	SOLO_DEBUG(DWL::DWLDebug::ImprimirDebug(TEXT("ControlPresentacion::Repintar()\n")));
}


LRESULT ControlPresentacionEx::Evento_Pintar(HDC hDC, PAINTSTRUCT &PS) {
	DWL::GDI::DWLRecta		R(_hWnd);
	DWL::GDI::DWLhDC		Buffer(hDC, R.Right(), R.Bottom());
	DWL::GDI::DWLBrocha		Brocha(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);

	Buffer.PintarRecta(&R, &Brocha);
	if (EscenaActual != NULL) {
		EscenaActual->PintarEscena();
		EscenaActual->Pintar(Buffer.hDC(), 0, _BarraScrollEx_ValorV);
	}

	BarraScrollEx_PintarV(Buffer.hDC());
//	BarraScrollEx_PintarH(Buffer.hDC());

	Buffer.PintarMapaBits(hDC, 0, 0, R.Right(), R.Bottom(), 0, 0);
    return 0;
}

/*
void ControlPresentacionEx::Evento_Mouse_Saliendo(void) {
	EscenaActual->PintarEscena();
	BaseControlExScroll::Evento_Mouse_Saliendo();
}*/


void ControlPresentacionEx::Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool nRepintar) {
	SetWindowPos(_hWnd, NULL, cX, cY, cAncho, cAlto, SWP_NOZORDER);
	int AlturaEscena = cAlto; 
	if (EscenaActual != NULL) {
		AlturaEscena = EscenaActual->PintarEscena();
	}
	long UPaginaV = _BarraScrollEx_PaginaV;
	BarraScrollEx_MaximoV(AlturaEscena);
	BarraScrollEx_PaginaV(cAlto);
	if (UPaginaV < _BarraScrollEx_PaginaV && _BarraScrollEx_ValorV > _BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV) _BarraScrollEx_ValorV = _BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV;
	RECT RC = { 0, 0, cAncho, cAlto };
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	
	DWL::GDI::DWLhDC hDC(_hWnd);
    PAINTSTRUCT PS;
	Evento_Pintar(hDC(), PS);
}


LRESULT ControlPresentacionEx::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	if (BarraScrollEx_Mouse_Movimiento(_hWnd, &RC, cX, cY, Param, false) == TRUE) {
		Repintar();
		return 0;
	}
	if (EscenaActual != NULL)	EscenaActual->Evento_Mouse_Movimiento(cX, cY + _BarraScrollEx_ValorV, Param, _BarraScrollEx_ValorV);
    return 0;
}


LRESULT ControlPresentacionEx::Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	if (BarraScrollEx_Mouse_BotonPresionado(_hWnd, &RC, Boton, cX, cY, Param) == TRUE) return 0;
	if (EscenaActual != NULL) EscenaActual->Evento_Mouse_BotonPresionado(Boton, cX, cY + _BarraScrollEx_ValorV, Param, _BarraScrollEx_ValorV);
    return 0;
}


LRESULT ControlPresentacionEx::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	SetFocus(_hWnd);
    RECT RC;
	GetClientRect(_hWnd, &RC);
	if (BarraScrollEx_Mouse_BotonSoltado(_hWnd, &RC, Boton, cX, cY, Param) == TRUE) return 0;
	if (EscenaActual != NULL) EscenaActual->Evento_Mouse_BotonSoltado(Boton, cX, cY + _BarraScrollEx_ValorV, Param, _BarraScrollEx_ValorV);
    return 0;
}

LRESULT ControlPresentacionEx::Evento_Mouse_Saliendo(void) {
	KillTimer(_hWnd, DWL_TIMER_TOOLTIP);
	BarraScrollEx_Mouse_Saliendo(_hWnd);
    return 0;
}

LRESULT ControlPresentacionEx::Evento_Mouse_Rueda(const short Delta, const short cX, const short cY, const UINT VirtKey) {
	if ((_BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV) > 0 && _BarraScrollEx_ValorV < (_BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV) +1) {
		if (Delta > 0) { // Hacia arriba
			_BarraScrollEx_ValorV -= static_cast<int>(_BarraScrollEx_MaximoV / 20);
			if (_BarraScrollEx_ValorV < 0) _BarraScrollEx_ValorV = 0;

		}
		else { // Hacia abajo
			_BarraScrollEx_ValorV += static_cast<int>(_BarraScrollEx_MaximoV / 20);
			if (_BarraScrollEx_ValorV > (_BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV)) _BarraScrollEx_ValorV = (_BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV);
		}
		_BarraScrollEx_PosActualV = _BarraScrollEx_ValorV;

		BarraScrollEx_CalcularScrollV();		
		Repintar(true);
	}
    return 0;
}


void ControlPresentacionEx::Mostrar_Ayuda(void) {
	ListaTabuladores.BorrarLista();
	Estado = EstadoCP_Ayuda;
	Mostrar_NODISPONIBLE();
}

void ControlPresentacionEx::Mostrar_CargandoVLC(void) {
	ListaTabuladores.BorrarLista();
	Estado = EstadoCP_Inicio;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	DWL::DWLString Txt;
	Txt.sprintf(IDIOMA__CARGANDO_NUCLEO_DE_LA_LIBVLC_S, LIBVLC_STR_VERSION);
//	Txt.sprintf(TEXT(", APP_NOMBRE, APP_STR_VERSION);
	EscenaActual->AgregarTexto15b(Txt(), Alineacion_Centrado);
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarIcono(IDI_VLC, 128, 128);

	RECT RC;
	GetClientRect(_hWnd, &RC);
    BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}


void ControlPresentacionEx::Mostrar_Inicio(void) {
	ListaTabuladores.BorrarLista();
	Estado = EstadoCP_Inicio;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	DWL::DWLString Txt;
	StrTp *GeneroME = Sistema.App.BD.GeneroMasEscuchadoStrTp();
	StrTp *GrupoME = Sistema.App.BD.GrupoMasEscuchadoStrTp();
	StrTp *DiscoME = Sistema.App.BD.DiscoMasEscuchadoStrTp();
	UINT TotalCanciones = Sistema.App.ListaC.TotalItems();
	UINT64 TotalTiempoMS = 0;
	for (UINT i = 0; i < TotalCanciones; i++) {
		if (Sistema.App.ListaC.ItemMedio(i)->Medio != NULL) TotalTiempoMS += Sistema.App.ListaC.ItemMedio(i)->Medio->Tiempo;
//		if (Sistema.App.ListaC.ItemMedio(i)->Video != NULL) TotalTiempoMS += static_cast<DWORD>(Sistema.App.ListaC.ItemMedio(i)->Video->Tiempo);
	}

	// Calculo cuantos de los medios son canciones
	UINT Videos = 0;
	for (size_t n = 0; n < Sistema.App.BD.Medios.size(); n++) {
		if (Sistema.App.BD.Medios[n]->TipoMedio == Tipo_Medio_Video) Videos ++;
	}


	Txt.sprintf(IDIOMA__BIENVENIDO_A_S_S, APP_NOMBRE, APP_STR_VERSION);
	EscenaActual->AgregarTexto15b(Txt(), Alineacion_Centrado);
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA);

	
	// Base de datos
	ObjetoEscenaFrame *nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	nFila->AgregarIcono(IDI_BDV, 24, 24, 3, 4, ID_ICONO_BD);
	ObjetoEscenaFrame *nColumna = nFila->AgregarFrameV();
	Txt.sprintf(IDIOMA__D_GENEROS_D_GRUPOS, Sistema.App.BD.StrGeneros.size(), Sistema.App.BD.StrGrupos.size(), Sistema.App.BD.StrDiscos.size(), Sistema.App.BD.Medios.size() - Videos, Videos);
	nColumna->AgregarTexto13(Txt());
	nColumna->AgregarTexto13(IDIOMA__NO_HAY_SUGERENCIAS_BD);
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA);

	// Lista actual
	nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	nFila->AgregarIcono(IDI_LISTA, 24, 24, 3, 4, ID_ICONO_LISTA);
	nColumna = nFila->AgregarFrameV();
	Txt.sprintf(IDIOMA__LISTA_ACTUAL_S, Sistema.App.ListaC.NombreLista());
	nColumna->AgregarTexto13(Txt());
	Txt.sprintf(IDIOMA__NUMERO_DE_CANCIONES_LISTA, TotalCanciones, Sistema.App.Media.TiempoStrL(TotalTiempoMS));
	nColumna->AgregarTexto13(Txt());
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA);


	// Generos
	nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	nFila->AgregarIcono(IDI_GENERO, 24, 24, 3, 4, ID_ICONO_GENERO);
	nColumna = nFila->AgregarFrameV();
	if (GeneroME != NULL) {
		Txt.sprintf(IDIOMA__EL_GENERO_MAS_ESCUCHADO_ES, GeneroME->Nombre(), GeneroME->Escuchado);
		nColumna->AgregarTexto13(Txt(), Alineacion_Expandible, 0, CP_GENERO_MAS);
	}
	Sistema.App.BD.CrearSugerenciasGenero();
	if (Sistema.App.BD.ObtenerSugerenciaGenero1() != NULL && Sistema.App.BD.ObtenerSugerenciaGenero2() != NULL) {
		Txt.sprintf(IDIOMA__SUGERENCIAS_S_S, Sistema.App.BD.ObtenerSugerenciaGenero1()->Nombre(), Sistema.App.BD.ObtenerSugerenciaGenero2()->Nombre());
		nColumna->AgregarTexto13(Txt(), Alineacion_Expandible, 0, CP_SUGERENCIA_GENERO1, CP_SUGERENCIA_GENERO2);
	}
	else {
		nColumna->AgregarTexto13(IDIOMA__SIN_SUGERENCIAS);
	}
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA);

	// Grupos
	nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	nFila->AgregarIcono(IDI_GRUPO, 24, 24, 3, 4, ID_ICONO_GRUPO);
	nColumna = nFila->AgregarFrameV();
	if (GrupoME != NULL) {
		Txt.sprintf(IDIOMA__EL_GRUPO_MAS_ESCUCHADO_ES, GrupoME->Nombre(), GrupoME->Escuchado);
		nColumna->AgregarTexto13(Txt(), Alineacion_Expandible, 0, CP_GRUPO_MAS);
	}
	Sistema.App.BD.CrearSugerenciasGrupo();
	if (Sistema.App.BD.ObtenerSugerenciaGrupo1() != NULL && Sistema.App.BD.ObtenerSugerenciaGrupo2() != NULL) {
		Txt.sprintf(IDIOMA__SUGERENCIAS_S_S, Sistema.App.BD.ObtenerSugerenciaGrupo1()->Nombre(), Sistema.App.BD.ObtenerSugerenciaGrupo2()->Nombre());
		nColumna->AgregarTexto13(Txt(), Alineacion_Expandible, 0, CP_SUGERENCIA_GRUPO1, CP_SUGERENCIA_GRUPO2);
	}
	else {
		nColumna->AgregarTexto13(IDIOMA__SIN_SUGERENCIAS);
	}
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA);

	// Discos
	nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	nFila->AgregarIcono(IDI_DISCO, 24, 24, 3, 5, ID_ICONO_DISCO);
	nColumna = nFila->AgregarFrameV();
	if (DiscoME != NULL) {
		Txt.sprintf(IDIOMA__EL_DISCO_MAS_ESCUCHADO_ES, DiscoME->Nombre(), DiscoME->Escuchado);
		nColumna->AgregarTexto13(Txt(), Alineacion_Expandible, 0, CP_DISCO_MAS);
	}
	Sistema.App.BD.CrearSugerenciasDisco();
	if (Sistema.App.BD.ObtenerSugerenciaDisco1() != NULL && Sistema.App.BD.ObtenerSugerenciaDisco2() != NULL) {
		Txt.sprintf(IDIOMA__SUGERENCIAS_S_S, Sistema.App.BD.ObtenerSugerenciaDisco1()->Nombre(), Sistema.App.BD.ObtenerSugerenciaDisco2()->Nombre());
		nColumna->AgregarTexto13(Txt(), Alineacion_Expandible, 0, CP_SUGERENCIA_DISCO1, CP_SUGERENCIA_DISCO2);
	}
	else {
		nColumna->AgregarTexto13(IDIOMA__SIN_SUGERENCIAS);
	}
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA * 3);

	// Compruebo si hay canciones escuchadas y canciones con 5 estrellas
	bool HayCancionesEscuchadas = false;
	bool HayCanciones5Estrellas = false;
	for (size_t i2 = 0; i2 < Sistema.App.BD.Medios.size(); i2++) {
		if (Sistema.App.BD.Medios[i2]->Nota == 5)		HayCanciones5Estrellas = true;
		if (Sistema.App.BD.Medios[i2]->Reproducido > 0) HayCancionesEscuchadas = true;
		if (HayCanciones5Estrellas == true && HayCancionesEscuchadas == true) break;
	}

	// Generar listas
	EscenaActual->AgregarTexto15b(IDIOMA__GENERADOR_DE_LISTAS, Alineacion_Centrado);
	nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	DWL::ControlesEx::DWLEdicionDesplegableEx *nCombo = nFila->AgregarEdicionDesplegable(TEXT(""), ID_TIPOS_LISTA, 0, false, 106);
	nCombo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GENERO, IDI_GENERO);
	nCombo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GRUPO, IDI_GRUPO);
	nCombo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_DISCO, IDI_DISCO);
	nCombo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_ALEATORIAS, IDI_AYUDA);
	if (HayCancionesEscuchadas == true) nCombo->AgregarItem(IDIOMA__LISTA_CON_MAS_ESCUCHADAS, IDI_INICIO2);
	if (HayCanciones5Estrellas == true) nCombo->AgregarItem(IDIOMA__LISTA_CON_5ESTRELLAS, IDI_ESTRELLA2);
	bool BotonActivado = true;
	if (Sistema.App.Config.Combo_Inicio_Generar_Lista == -1)	BotonActivado = false;
	else														nCombo->SeleccionarItem(Sistema.App.Config.Combo_Inicio_Generar_Lista);
	nFila->AgregarBoton(IDIOMA__GENERAR, 80, 18, ID_GENERAR_LISTA, BotonActivado);
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA * 3);

	EscenaActual->AgregarTexto15b(IDIOMA__MEDIOS_ABIERTOS_RECIENTEMENTE, Alineacion_Centrado);
	TMedio *TmpMedio;
	// Añadimos los medios abiertos recientemente
	for (size_t i = 0; i < 10; i++) {
		TmpMedio = Sistema.App.BD.MedioReciente(i);
		if (TmpMedio != NULL) {
			switch (TmpMedio->TipoMedio) {
				case Tipo_Medio_Audio :
					Txt.sprintf(TEXT("%02d %s : $%d %s$"), i + 1, IDIOMA__AUDIO, static_cast<TMedioAudio *>(TmpMedio)->Pista, TmpMedio->Nombre());
					break;
				case Tipo_Medio_CDAudio :
					Txt.sprintf(TEXT("%02d %s : $%d %s$"), i + 1, IDIOMA__CDAUDIO, static_cast<TMedioCDAudio *>(TmpMedio)->Pista, TmpMedio->Nombre());
					break;
				case Tipo_Medio_Video :
					Txt.sprintf(TEXT("%02d %s : $%d %s$"), i + 1, IDIOMA__VIDEO, static_cast<TMedioVideo *>(TmpMedio)->Pista, TmpMedio->Nombre());
					break;
			}
			EscenaActual->AgregarTexto13(Txt(), Alineacion_Expandible, 0U, CP_MEDIO_RECIENTE1 + i);
		}
	}
	
	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}

void ControlPresentacionEx::Mostrar_SobreBubaTronik(void) {
	ListaTabuladores.BorrarLista();
	BarraScrollEx_ValorV(0);
	Estado = EstadoCP_Ayuda;
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	ObjetoEscenaFrame *nFrame, *nFrame2; 
	
	EscenaActual->AgregarTexto15b(IDIOMA__SOBRE_BUBATRONIK, Alineacion_Expandible);

	EscenaActual->AgregarTexto13(IDIOMA__BUBATRONIK_ES_UNA_APLICACION_QUE_NACIO_ANTE_LA_NECESIDAD_DE_ORDENAR_Y_REPRODUCIR_MEDIOS_FACILMENTE, Alineacion_Expandible);
	EscenaActual->AgregarTexto13(IDIOMA__BUBATRONIK_LA_DWL_Y_WWW_DEVILDREY33_ES_SON_PROPIEDAD_DE_JOSE_ANTONIO_BOVER_COMAS, Alineacion_Expandible, 0U, CP_URL_BUBATRONIK, CP_URL_DWL, CP_URL_DEVILDREY33);
	EscenaActual->AgregarTexto13(IDIOMA__SI_DESEAIS_PONEROS_EN_CONTACTO_CON_EL_AUTOR_PODEIS_HACERLO_ENVIANDO_UN_MENSAJE_EN_EL_FORO_DE_WWW_DEVILDREY33_ES, Alineacion_Expandible, 0U, CP_URL_FORO);
	EscenaActual->AgregarSeparacion(3);
	EscenaActual->AgregarTexto13(IDIOMA__HAY_MUCHA_GENTE_QUE_HA_COLABORADO_ACTIVA_Y_PASIVAMENTE_EN_ESTE_PROYECTO_COSA_QUE_DEBO_AGRADECER, Alineacion_Expandible);

	EscenaActual->AgregarTexto13(IDIOMA__BARBA_SIEMPRE_DA_BUENAS_OPINIONES_PARA_PROBLEMAS_POCO_COMUNES, Alineacion_Expandible);
	EscenaActual->AgregarTexto13(IDIOMA__FONG_APORTACION_DE_VARIAS_IDEAS_Y_UNA_VALIOSA_COBAYA, Alineacion_Expandible);
	EscenaActual->AgregarTexto13(IDIOMA__MICHEL_APORTACION_DE_VARIAS_IDEAS_Y_UNA_VALIOSA_COBAYA, Alineacion_Expandible);
	EscenaActual->AgregarTexto13(IDIOMA__PARRIS_OTRA_VALIOSA_COBAYA, Alineacion_Expandible);
	EscenaActual->AgregarTexto13(IDIOMA__TXEMA_OTRA_VALIOSA_COBAYA, Alineacion_Expandible, 0U, CP_URL_MALEIT);
	EscenaActual->AgregarTexto13(IDIOMA__TAMBIEN_TENGO_QUE_AGRADECER_A_TODA_LA_GENTE_QUE_ME_HA_DADO_APOYO_Y_SOPORTE_DURANTE_ESTE_PROYECTO_GRACIAS_A_TODOS, Alineacion_Expandible);
	EscenaActual->AgregarSeparacion(3);
	EscenaActual->AgregarTexto13(IDIOMA__POR_ULTIMO_TAMBIEN_DEBO_MENCIONAR_QUE_ESTE_REPRODUCTOR_UTILIZA_OTRAS_2_LIBRERIAS_PARA_CUMPLIR_SU_COMETIDO_Y_SON, Alineacion_Expandible);
	
	nFrame = EscenaActual->AgregarFrameH(Alineacion_Izquierda);	
	nFrame->AgregarIcono(IDI_FMOD, 32, 32, 0, 0, CP_URL_FMOD);
	nFrame2 = nFrame->AgregarFrameV(Alineacion_Izquierda);
	nFrame2->AgregarTexto13(IDIOMA__FMOD_EX_426_POTENTE_LIBRERIA_DE_AUDIO_UTILIZADA_EN_MUCHOS_JUEGOS_ACTUALES, Alineacion_Izquierda); 
	nFrame2->AgregarTexto13(IDIOMA__PUEDES_ENCONTRARLA_EN_EL_SIGUIENTE_ENLACE_HTTP_WWW_FMOD_ORG, Alineacion_Izquierda, 0U, CP_URL_FMOD);

	EscenaActual->AgregarSeparacion(2);

	nFrame = EscenaActual->AgregarFrameH(Alineacion_Izquierda);	
	nFrame->AgregarIcono(IDI_VLC, 32, 32, 0, 0, CP_URL_LIBVLC);
	nFrame2 = nFrame->AgregarFrameV(Alineacion_Izquierda);
	DWL::DWLString LibVlcVer;
	LibVlcVer.sprintf(IDIOMA__LIBVLC_S_LIBRERIA_PARA_REPRODUCIR_PRACTICAMENTE_CUALQUIER_MEDIO, LIBVLC_STR_VERSION);
	nFrame2->AgregarTexto13(LibVlcVer(), Alineacion_Izquierda);
	nFrame2->AgregarTexto13(IDIOMA__PUEDES_ENCONTRARLA_EN_EL_SIGUIENTE_ENLACE_HTTP_WWW_VIDEOLAN_ORG, Alineacion_Izquierda, 0U, CP_URL_LIBVLC);

	EscenaActual->AgregarSeparacion(3);
	EscenaActual->AgregarTexto15b(IDIOMA__DATOS_ADICIONALES, Alineacion_Izquierda);
	DWL::DWLString TmpStr, TmpStr2, Fecha;
	#ifdef _DEBUG
		TmpStr2 = TEXT("DEBUG");
	#else 
		TmpStr2 = TEXT("RELEASE");
	#endif
	TmpStr.sprintf(IDIOMA__BUBATRONIK_F_S_COMPILADO_CON_MSVC_D_EL_S, APP_NUM_VER, TmpStr2(), _MSC_VER, APP_FECHA_COMPILACION);
	EscenaActual->AgregarTexto13(TmpStr(), Alineacion_Izquierda);
	TmpStr.sprintf(IDIOMA__VERSION_DE_WINDOWS_D_D_SERVICE_PACK_D_D, Sistema.VersionMayor(), Sistema.VersionMenor(), Sistema.VersionServicePackMayor(), Sistema.VersionServicePackMenor());
	EscenaActual->AgregarTexto13(TmpStr(), Alineacion_Izquierda);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}

void ControlPresentacionEx::Mostrar_NODISPONIBLE(void) {
	ListaTabuladores.BorrarLista();
//	VerPreferencias(false);
	Estado = EstadoCP_Ayuda;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
/*	ObjetoEscenaFrame *nFila = EscenaActual->AgregarFrameH(Alineacion_Izquierda);
	nFila->AgregarIcono(IDI_BDV, 24, 24, 3, 4, ID_ICONO_BD);
	ObjetoEscenaFrame *nColumna = nFila->AgregarFrameV();
	nColumna->AgregarTexto13(TEXT("a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a"));
	nColumna->AgregarTexto13(TEXT("b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b"));
	EscenaActual->AgregarSeparacion(MARGEN_ESCENA);*/

	EscenaActual->AgregarTexto15b(TEXT("Sección no disponible hasta futuras versiones."), Alineacion_Centrado);
/*	EscenaActual->AgregarTexto15b(TEXT("Sección no disponible hasta futuras versiones."), Alineacion_Derecha);
	EscenaActual->AgregarTexto15b(TEXT("Sección no disponible hasta futuras versiones."), Alineacion_Izquierda);
	EscenaActual->AgregarTexto13(TEXT("Sección no disponible hasta futuras versiones."), Alineacion_Centrado);
	EscenaActual->AgregarTexto13(TEXT("Sección no disponible hasta futuras versiones."), Alineacion_Derecha);
	EscenaActual->AgregarTexto13(TEXT("Sección no disponible hasta futuras versiones."), Alineacion_Izquierda);*/

//	Espacio_Oscy(false);
	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}



void ControlPresentacionEx::Mostrar_Opciones_Lista(void) {
	ListaTabuladores.BorrarLista();
	
	Estado = EstadoCP_Opciones;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	
	EscenaActual->AgregarTexto15b(IDIOMA__LISTA_INICIAL);
	EscenaActual->AgregarTexto13(IDIOMA__ELIGE_QUE_DEBE_HACER_EL_REPRODUCTOR_SI_SE_INICIA_CON_UNA_LISTA_VACIA);
	EscenaActual->AgregarTexto13(IDIOMA__NOTA_SI_ELIGES_LA_OPCION_CUALQUIER_COSA_EL_REPRODUCTOR_SELECCIONARA_UNA_DE_LAS_OPCIONES_DE_LA_LISTA_ALEATORIAMENTE);
	EscenaActual->AgregarSeparacion(5);
	
	DWL::DWLString Str;
	UINT IconoLista = 0;
	switch (Sistema.App.Config.Inicio) {
		case Inicio_Genero_Aleatorio		: 
			Str = IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GENERO;		
			IconoLista = IDI_GENERO;
			break;
		case Inicio_Grupo_Aleatorio			: 
			Str = IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GRUPO;	
			IconoLista = IDI_GRUPO;
			break;
		case Inicio_Disco_Aleatorio			: 
			Str = IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_DISCO;		
			IconoLista = IDI_DISCO;
			break;
		case Inicio_Canciones_Aleatorias	: 
			Str = IDIOMA__LISTA_CON_50_CANCIONES_ALEATORIAS;
			IconoLista = IDI_AUDIO;
			break;
		case Inicio_Cualquier_Cosa			: 
			Str = IDIOMA__CUALQUIER_COSA;	
			IconoLista = IDI_AYUDA;
			break;
		case Inicio_Nada					: 
			Str = IDIOMA__LISTA_VACIA;	
			break;
	}

	DWL::ControlesEx::DWLEdicionDesplegableEx *Combo = NULL;
	Combo = EscenaActual->AgregarEdicionDesplegable(TEXT(""), ID_LISTA_INICIO, 0, false, 15*6);
	Combo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GENERO, IDI_GENERO);
	Combo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GRUPO, IDI_GRUPO);
	Combo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_DISCO, IDI_DISCO);
	Combo->AgregarItem(IDIOMA__LISTA_CON_50_CANCIONES_ALEATORIAS, IDI_AUDIO);
	Combo->AgregarItem(IDIOMA__CUALQUIER_COSA, IDI_AYUDA);
	Combo->AgregarItem(IDIOMA__LISTA_VACIA);
	Combo->Texto(Str(), IconoLista);
	
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__EXCLUIR_CANCIONES);
	EscenaActual->AgregarTexto13(IDIOMA__ESTA_OPCION_TE_PERMITE_EXCLUIR_LAS_CANCIONES_QUE_TENGAN_UNA_ESTRELLA_DE_TODAS_LAS_LISTAS_ALEATORIAS);
	EscenaActual->AgregarSeparacion(5);
	EscenaActual->AgregarMarca(IDIOMA__EXCLUIR_CANCIONES_CON_UNA_ESTRELLA, ID_EXCLUIR_NOTA, Sistema.App.Config.ExcluirNota1);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}


void ControlPresentacionEx::Mostrar_Opciones_BD(void) {
	ListaTabuladores.BorrarLista();
	
	Estado = EstadoCP_Opciones;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);

	EscenaActual->AgregarTexto15b(IDIOMA__MOSTRAR_TAGS);
	EscenaActual->AgregarTexto13(IDIOMA__SI_ACTIVAS_ESTA_OPCION_EN_LOS_TOOLTIPS_INFORMATIVOS_SE_MOSTRARAN_TODOS_LOS_DATOS_DEL_TAG);
	EscenaActual->AgregarSeparacion(5);
	EscenaActual->AgregarMarca(IDIOMA__MOSTRAR_TAGS_EN_LOS_TOOLTIPS, ID_OPCIONES_MOSTRAR_TAG, Sistema.App.Config.MostrarTagsEnToolTips);
	
	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}

void ControlPresentacionEx::Mostrar_Opciones_Video(void) {
	ListaTabuladores.BorrarLista();
	
	Estado = EstadoCP_Opciones;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	EscenaActual->AgregarTexto15b(IDIOMA__OPCIONES_DE_VIDEO);
	EscenaActual->AgregarTexto13(IDIOMA__ESTA_OPCION_OCULTARA_EL_MOUSE_CUANDO_EL_REPRODUCTOR_ESTE_EN_MODO_PANTALLA_COMPLETA);
	EscenaActual->AgregarSeparacion(5);
	bool bMarcado = false;
	if (Sistema.App.Config.OcultarMouseEnVideo > 0)	bMarcado = true;
	DWL::ControlesEx::DWLMarcaEx *Marca = EscenaActual->AgregarMarca(IDIOMA__OCULTAR_MOUSE_EN_PANTALLA_COMPLETA, ID_OPCIONES_OCULTAR_MOUSE, bMarcado);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}


void ControlPresentacionEx::Mostrar_Opciones_Apariencia(void) {
	ListaTabuladores.BorrarLista();
	
	Estado = EstadoCP_Opciones;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	EscenaActual->AgregarTexto15b(IDIOMA__TAMANO_DE_LA_FUENTE);
	EscenaActual->AgregarTexto13(IDIOMA__SELECCIONA_EL_TAMANO_DE_LA_FUENTE_QUE_USARA_EL_REPRODUCTOR);
	EscenaActual->AgregarSeparacion(4);
	DWL::DWLString Str;
	switch (Sistema.App.Config.TamFuente) {
		case 12		: Str = IDIOMA__PEQUEÑA;			break;
		case 13		: Str = IDIOMA__MEDIANA;			break;
		case 14		: Str = IDIOMA__GRANDE;				break;
	}
	const TCHAR TxtTahoma[] = TEXT("Tahoma");
	DWL::ControlesEx::DWLEdicionDesplegableEx *Combo = NULL;
	Combo = EscenaActual->AgregarEdicionDesplegable(TEXT(""), ID_OPCIONES_FUENTE);
	Combo->AgregarItem(IDIOMA__PEQUEÑA);
	Combo->Item(0)->SubItem(0)->Fuentes()->Normal.Crear(TxtTahoma, 12);
	Combo->Item(0)->SubItem(0)->Fuentes()->SubRayada.Crear(TxtTahoma, 12, FW_NORMAL, false, true);
	Combo->AgregarItem(IDIOMA__MEDIANA);
	Combo->Item(1)->SubItem(0)->Fuentes()->Normal.Crear(TxtTahoma, 13);
	Combo->Item(1)->SubItem(0)->Fuentes()->SubRayada.Crear(TxtTahoma, 13, FW_NORMAL, false, true);
	Combo->AgregarItem(IDIOMA__GRANDE);
	Combo->Item(2)->SubItem(0)->Fuentes()->Normal.Crear(TxtTahoma, 14);
	Combo->Item(2)->SubItem(0)->Fuentes()->SubRayada.Crear(TxtTahoma, 14, FW_NORMAL, false, true);
	Combo->Texto(Str());

	//////////////////////////////////
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__CONFIGURACION_DE_COLORES);

	EscenaActual->AgregarTexto13(IDIOMA__ELIGE_LA_CONFIGURACION_DE_COLORES_QUE_PREFIERAS_PARA_EL_REPRODUCTOR);
	EscenaActual->AgregarSeparacion(4);

	Combo = EscenaActual->AgregarEdicionDesplegable(TEXT(""), ID_OPCIONES_SKIN);
	EnumerarSkins(Combo);
	Combo->Texto(Sistema.App.Config.Skin_Path());
	EscenaActual->AgregarTexto13(IDIOMA__NOTA_PUEDES_CREAR_EDITAR_UNA_CONFIGURACION_DE_COLORES_EDITANDO_LOS_ARCHIVOS_CON_EXTENSION);

	//////////////////////////////////
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__TAMANO_DEL_ESPECTRO_GRAFICO);
	EscenaActual->AgregarTexto13(IDIOMA__SELECCIONA_EL_TAMANO_DE_LAS_BARRAS_PARA_EL_ESPECTRO_GRAFICO_DEL_SONIDO);
	EscenaActual->AgregarSeparacion(4);
	Combo = EscenaActual->AgregarEdicionDesplegable(TEXT(""), ID_OPCIONES_OSCY);
	Combo->AgregarItem(IDIOMA__PEQUEÑA);
	Combo->AgregarItem(IDIOMA__MEDIANA);
	Combo->AgregarItem(IDIOMA__GRANDE);
	switch (Sistema.App.Config.TipoOscy) {
		case Oscy_Tres_Pixeles	: Str = IDIOMA__GRANDE;		break;
		case Oscy_Dos_Pixeles	: Str = IDIOMA__MEDIANA;	break;
		case Oscy_Un_Pixel		: Str = IDIOMA__PEQUEÑA;	break;
	}
	Combo->Texto(Str());

	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__BARRA_DE_TITULO);
	EscenaActual->AgregarTexto13(IDIOMA__UTILIZAR_BARRA_DE_TITULO_DE_WINDOWS_RECOMENDADO_EN_WINDOWS_VISTA_Y_SUPERIORES);
	EscenaActual->AgregarSeparacion(2);
	EscenaActual->AgregarMarca(IDIOMA__UTILIZAR_BARRA_DE_TITULO_DE_WINDOWS, ID_OPCIONES_UTILIZAR_BARRA_WIN, Sistema.App.Config.UtilizarBarraTituloWindows);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}

void ControlPresentacionEx::Mostrar_Opciones(void) {
	ListaTabuladores.BorrarLista();
	
	Estado = EstadoCP_Opciones;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	EscenaActual->AgregarTexto15b(IDIOMA__IDIOMA);
	EscenaActual->AgregarTexto13(IDIOMA__ELIGE_QUE_IDIOMA_PREFIERES_QUE_SE_MUESTRE_EN_EL_REPRODUCTOR);
	EscenaActual->AgregarSeparacion(5);
	DWL::ControlesEx::DWLEdicionDesplegableEx *Combo = NULL;
	int Ico = Sistema.App.Idioma_Icono();
	Combo = EscenaActual->AgregarEdicionDesplegable(Sistema.App.Idioma_Nombre(), ID_LISTA_IDIOMAS, Sistema.App.Idioma_Icono());
	EnumerarIdiomas(Combo);
	//////////////////////////////////
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__ACTUALIZACIONES);

	EscenaActual->AgregarTexto13(IDIOMA__BUSCAR_EN_INTERNET_UNA_NUEVA_VERSION_DEL_REPRODUCTOR_AL_INICIAR);
	EscenaActual->AgregarSeparacion(3);
	EscenaActual->AgregarMarca(IDIOMA__BUSCAR_ACTUALIZACIONES, ID_BUSCAR_ACTUALIZACIONES, Sistema.App.Config.BuscarActualizaciones);
	//////////////////////////////////
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__REPRODUCTOR_SIEMPRE_DELANTE);

	EscenaActual->AgregarTexto13(IDIOMA__ESTA_OPCION_TE_PERMITE_TENER_POR_DELANTE_DE_TODAS_LAS_OTRAS_VENTANAS_LA_VENTANA_DEL_REPRODUCTOR);
	EscenaActual->AgregarSeparacion(3);
	EscenaActual->AgregarMarca(IDIOMA__REPRODUCTOR_SIEMPRE_DELANTE, ID_SIEMPRE_DELANTE, Sistema.App.Config.SiempreDelante);
	//////////////////////////////////
	EscenaActual->AgregarSeparacion(10);
  	EscenaActual->AgregarTexto15b(IDIOMA__TIPOS_DE_ARCHIVO);
    EscenaActual->AgregarTexto13(IDIOMA__LOS_TIPOS_DE_ARCHIVO_QUE_SE_VEN_A_CONTINUACION_SON_LOS_TIPOS_QUE_USARA_EL_REPRODUCTOR);
    DWL::ControlesEx::DWLListaEx *ListaTipos = EscenaActual->AgregarLista(200, ID_LISTA_TIPOS);

    ListaTipos->AgregarColumna(100);
    ListaTipos->AgregarColumna(200);

    DWL::DWLString TmpTxt;
    bool RestaurarTipos = false;
    for (size_t ti = 0; ti < Sistema.App.Tipos.Tipos.size(); ti++) {
        if (Sistema.App.Tipos.Tipos[ti]->Buscar_Asociar == true) {
            switch (Sistema.App.Tipos.Tipos[ti]->Tipo) {
				case Tipo_Medio_Audio   : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__AUDIO);              break;
                case Tipo_Medio_Video   : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__VIDEO);              break;
				case Tipo_Medio_Lista   : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__LISTA_DE_CANCIONES); break;
                case Tipo_Medio_CDAudio : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__CDAUDIO);            break;
            }
            ListaTipos->AgregarItem(NULL, 0, DWL_LISTAEX_ORDENADO, Sistema.App.Tipos.Tipos[ti]->Extension(), TmpTxt());
        }
        else {
            RestaurarTipos = true;
        }
    }
    ListaTipos->ActualizarTodo();
    
	EscenaActual->AgregarSeparacion(4);
	ObjetoEscenaFrame *nFila = EscenaActual->AgregarFrameH(Alineacion_Centrado);
    nFila->AgregarBoton(IDIOMA__ELIMINAR_AUDIO, 80, 18, ID_BOTON_ELIMINAR_TIPOS_AUDIO, true); 
    nFila->AgregarBoton(IDIOMA__ELIMINAR_VIDEO, 80, 18, ID_BOTON_ELIMINAR_TIPOS_VIDEO, true); 
    nFila->AgregarBoton(IDIOMA__ELIMINAR_SELECCIONADOS, 125, 18, ID_BOTON_ELIMINAR_TIPOS, true);
    nFila->AgregarBoton(IDIOMA__RESTAURAR_TIPOS, 90, 18, ID_BOTON_RESTAURAR_TIPOS, RestaurarTipos); 

	bool Tmp = false;	
	EscenaActual->AgregarTexto15b(IDIOMA__ASOCIAR_TIPOS);    
    EscenaActual->AgregarTexto13(IDIOMA__ESTA_OPCION_TE_PERMITE_ASOCIAR_LOS_TIPOS_DE_ARCHIVO_DE_LA_LISTA_ANTERIOR_AL_REPRODUCTOR);
	EscenaActual->AgregarSeparacion(5);
	if (Sistema.App.Config.AsociarArchivos == Respuesta_Aceptar ||  Sistema.App.Config.AsociarArchivos == Respuesta_Cancelar) Tmp = true;
	EscenaActual->AgregarMarca(IDIOMA__PREGUNTAR_AL_INICIAR_CUANDO_SEA_NECESARIO, ID_ASOCIAR_PREGUNTAR, Tmp);
	if (Sistema.App.Config.AsociarArchivos == Respuesta_Aceptar_NoMostrar)  Tmp = true;
	else					        										Tmp = false;
	EscenaActual->AgregarMarca(IDIOMA__SIEMPRE_, ID_ASOCIAR_SIEMPRE, Tmp);
	if (Sistema.App.Config.AsociarArchivos == Respuesta_Cancelar_NoMostrar) Tmp = true;
	else															        Tmp = false;
	EscenaActual->AgregarMarca(IDIOMA__NUNCA_, ID_ASOCIAR_NUNCA, Tmp);
	EscenaActual->AgregarTexto13(IDIOMA__SELECCIONA_COMO_QUIERES_QUE_SE_EJECUTEN_LOS_ARCHIVOS_POR_DEFECTO);
	EscenaActual->AgregarSeparacion(5);
	bool ReproducirPorDefecto = Sistema.App.Asociar.ReproducirPorDefecto();
	EscenaActual->AgregarMarca(IDIOMA__ANADIR_A_LISTA, ID_ASOCIAR_ADD, !ReproducirPorDefecto);
	EscenaActual->AgregarMarca(IDIOMA__REPRODUCIR, ID_ASOCIAR_REP, ReproducirPorDefecto);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC, false);
	Repintar(true);
}

void ControlPresentacionEx::Mostrar_Opciones_Teclado(void) {
	ListaTabuladores.BorrarLista();
	
	Estado = EstadoCP_Opciones;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	EscenaActual->AgregarTexto15b(IDIOMA__TECLAS_DE_ACCESO_RAPIDO);
	EscenaActual->AgregarTexto13(IDIOMA__PUEDES_CAMBIAR_LAS_TECLAS_PARA_CUALQUIER_ACCION_DE_LAS_QUE_SE_LISTAN_A_CONTINUACION);
	EscenaActual->AgregarSeparacion(5);
	EscenaActual->AgregarTeclaRapida(IDIOMA__PLAY_PAUSA, ID_OPCIONES_TECLA_PLAY, &Sistema.App.Config.Tecla_PlayPausa);
	EscenaActual->AgregarTeclaRapida(IDIOMA__STOP, ID_OPCIONES_TECLA_STOP, &Sistema.App.Config.Tecla_Stop);
	EscenaActual->AgregarTeclaRapida(IDIOMA__SUBIR_VOLUMEN, ID_OPCIONES_TECLA_VOLUMEN_MAS, &Sistema.App.Config.Tecla_VolumenMas);
	EscenaActual->AgregarTeclaRapida(IDIOMA__BAJAR_VOLUMEN, ID_OPCIONES_TECLA_VOLUMEN_MENOS, &Sistema.App.Config.Tecla_VolumenMenos);
	EscenaActual->AgregarTeclaRapida(IDIOMA__REPRODUCIR_MEDIO_SIGUIENTE, ID_OPCIONES_TECLA_SIGUIENTE, &Sistema.App.Config.Tecla_Siguiente);
	EscenaActual->AgregarTeclaRapida(IDIOMA__REPRODUCIR_MEDIO_ANTERIOR, ID_OPCIONES_TECLA_ANTERIOR, &Sistema.App.Config.Tecla_Anterior);
	EscenaActual->AgregarSeparacion(10);
	EscenaActual->AgregarTexto15b(IDIOMA__OBTENER_TECLADO_FUERA_DE_LA_APLICACION);
	EscenaActual->AgregarTexto13(IDIOMA__MARCA_ESTA_OPCION_PARA_QUE_EL_REPRODUCTOR_DETECTE_LAS_TECLAS_DE_ACCESO_RAPIDO_INCLUSO_BAJO_UN_JUEGO);
	EscenaActual->AgregarSeparacion(2);
	ControlMarca *nMarca = EscenaActual->AgregarMarca(IDIOMA__OBTENER_TECLADO_CUANDO_EL_REPRODUCTOR_NO_ESTE_ACTIVO, ID_OBTENER_TECLADO, Sistema.App.Config.TecladoGlobal);
	nMarca->Activado(false);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}

void ControlPresentacionEx::Mostrar_Ecualizador(void) {
	ListaTabuladores.BorrarLista();
	Mostrar_NODISPONIBLE();
}

void ControlPresentacionEx::Mostrar_RevisarBD(void) {
	ListaTabuladores.BorrarLista();
	Estado = EstadoCP_RevisarBD;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	EscenaActual->AgregarTexto15b(IDIOMA__REVISAR_LA_BASE_DE_DATOS);
	EscenaActual->AgregarTexto13(IDIOMA__DESDE_ESTE_PANEL_PUEDES_REVISAR_LA_BASE_DE_DATOS_PARA_DETERMINAR_SI_SE_HAN_BORRADO_CANCIONES_DE_LOS_DISCOS);
	EscenaActual->AgregarTexto13(IDIOMA__ES_IMPORTANTE_QUE_REVISES_LA_BASE_DE_DATOS_CUANDO_BORRES_CANCIONES_DESDE_FUERA_DEL_REPRODUCTOR);
	EscenaActual->AgregarTexto13(IDIOMA__POR_ULTIMO_TAMBIEN_PUEDES_HACER_UNA_REVISION_PARA_DETERMINAR_QUE_GENEROS_GRUPOS_DISCOS_SE_PARECEN_ENTRE_SI);
	EscenaActual->AgregarSeparacion(4);
	EscenaActual->AgregarMarca(IDIOMA__REVISAR_ANOMALIAS_EN_GENEROS_GRUPOS_DISCOS, ID_FILTRO_COINCIDENCIAS, Sistema.App.Config.Buscar_FiltroCoincidencias);

	EscenaActual->AgregarSeparacion(4);
	EscenaActual->AgregarBarraProgreso(0, Sistema.App.BD.Medios.size(), 0, ID_BARRA_REVISAR);
	EscenaActual->AgregarSeparacion(4);
	bool Activado = true;
	if (Sistema.App.BD.Medios.size() == 0) Activado = false;
	EscenaActual->AgregarBoton(IDIOMA__REVISAR_MEDIOS, 100, ALTO_BOTON_CP, ID_BOTON_REVISAR, Activado);

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}


void ControlPresentacionEx::Mostrar_BuscarArchivos(void) {
	ListaTabuladores.BorrarLista();
	Estado = EstadoCP_BuscarArchivos;
	BarraScrollEx_ValorV(0);
	if (EscenaActual != NULL) delete EscenaActual;
	EscenaActual = new Escena(this);
	EscenaActual->AgregarTexto15b(IDIOMA__BUSCAR_ARCHIVOS, Alineacion_Centrado);
	EscenaActual->AgregarTexto13(IDIOMA__EL_REPRODCUTOR_BUSCARA_NUEVOS_MEDIOS_REPRODUCIBLES_EN_LOS_DIRECTORIOS_QUE_SE_MUESTRAN_EN_LA_LISTA_DE_ABAJO);
	EscenaActual->AgregarSeparacion(4);
    DWL::ControlesEx::DWLListaEx *TmpLista= EscenaActual->AgregarLista(60, ID_DIRECTORIOS_BUSCAR);
    TmpLista->AgregarColumna(209);
    TmpLista->AgregarColumna(125);
    TmpLista->AgregarColumna(29);
    TmpLista->BarraScrollEx_AgregarBotonV(IDI_ELIMINAR2, ID_BOTON_ELIMINAR_DIRECTORIO);
    TmpLista->BarraScrollEx_VisibleV(TRUE);
	DWL::DWLString PathTag;
	DWL::DWLString TipoBusqueda;
    for (size_t z = 0; z < Sistema.App.Config.Buscar_Paths.size(); z++) {
		if (Sistema.App.Config.Buscar_Paths[z].PathAntesQueTag == true) PathTag = IDIOMA___PATH_;
		else															PathTag = IDIOMA___TAG_;
		switch (Sistema.App.Config.Buscar_Paths[z].TipoOrden) {
			case 1 :				TipoBusqueda = IDIOMA__PATH_GENERO_GRUPO_DISCO;	break;
			case 2 :				TipoBusqueda = IDIOMA__PATH_GRUPO_DISCO;		break;
			case 3 :				TipoBusqueda = IDIOMA__PATH_DISCO;				break;
			default :				TipoBusqueda = IDIOMA__INDEFINIDO;				break;
		}
		TmpLista->AgregarItem(NULL, DWL::SO::DWLListaIconos::CargarIcono(Sistema.App.Config.Buscar_Paths[z].Path.Texto()), DWL_LISTAEX_PRINCIPIO, Sistema.App.Config.Buscar_Paths[z].Path(), TipoBusqueda(), PathTag());
    }
    TmpLista->ActualizarTodo();

	EscenaActual->AgregarSeparacion();

	DWL::ControlesEx::DWLEdicionDesplegableEx *ComboDirs = EscenaActual->AgregarEdicionDesplegable(IDIOMA__SELECCIONA_UN_DIRECTORIO, ID_COMBO_DIR, 0, true, 172);
	EscenaActual->AgregarSeparacion();
	
	DWL::DWLString TmpStr;
	DWL::ControlesEx::DWLExplorarDirectorios_Nodo *SugerenciaPadre = NULL;
	// Buscamos si existen los directorios \\MP3, \\Musica y \\Música en la raíz de los discos.
	for (size_t i = 0; i < Sistema.App.UnidadesDisco.TotalUnidades(); i++) {
		if (Sistema.App.UnidadesDisco.Unidad(i)->Tipo_Disco() == Tipo_Fijo) {
			// C:\\MP3
			TmpStr.sprintf(TEXT("%c:\\MP3"), Sistema.App.UnidadesDisco.Unidad(i)->Letra());
			DWORD FA = GetFileAttributes(TmpStr());
			if (FA != INVALID_FILE_ATTRIBUTES) {
				if (FA & FILE_ATTRIBUTE_DIRECTORY) {
					if (SugerenciaPadre == NULL) SugerenciaPadre = ComboDirs->BuscarDirectorio_AgregarSugerencia(IDIOMA__SUGERENCIAS, false, NULL, IDI_ESTRELLA2, true);
					ComboDirs->BuscarDirectorio_AgregarSugerencia(TmpStr(), true, SugerenciaPadre);
				}
			}

			// C:\\Musica
			TmpStr.sprintf(TEXT("%c:\\Musica"), Sistema.App.UnidadesDisco.Unidad(i)->Letra());
			FA = GetFileAttributes(TmpStr());
			if (FA != INVALID_FILE_ATTRIBUTES) {
				// Esto hay que añadirlo como sugerencia al cargar el Mostrar_BuscarArchivos
				if (FA & FILE_ATTRIBUTE_DIRECTORY) {
					if (SugerenciaPadre == NULL) SugerenciaPadre = ComboDirs->BuscarDirectorio_AgregarSugerencia(IDIOMA__SUGERENCIAS, false, NULL, IDI_ESTRELLA2, true);
					ComboDirs->BuscarDirectorio_AgregarSugerencia(TmpStr(), true, SugerenciaPadre);
				}
			}

			// C:\\Música
			TmpStr.sprintf(TEXT("%c:\\Música"), Sistema.App.UnidadesDisco.Unidad(i)->Letra());
			FA = GetFileAttributes(TmpStr());
			if (FA != INVALID_FILE_ATTRIBUTES) {
				// Esto hay que añadirlo como sugerencia al cargar el Mostrar_BuscarArchivos
				if (FA & FILE_ATTRIBUTE_DIRECTORY) {
					if (SugerenciaPadre == NULL) SugerenciaPadre = ComboDirs->BuscarDirectorio_AgregarSugerencia(IDIOMA__SUGERENCIAS, false, NULL, IDI_ESTRELLA2, true);
					ComboDirs->BuscarDirectorio_AgregarSugerencia(TmpStr(), true, SugerenciaPadre);
				}
			}
		}
	}

	EscenaActual->AgregarTexto13(IDIOMA__SELECCIONA_QUE_TIPO_DE_NORMA_DEBERIAS_UTILIZAR_PARA_LA_ESTRUCTURACCION_DEL_DIRECTORIO_QUE_HAS_ELEGIDO);

	EscenaActual->AgregarSeparacion();

	ObjetoEscenaFrame *nFila;
	nFila = EscenaActual->AgregarFrameH(Alineacion_Expandible);
	DWL::ControlesEx::DWLEdicionDesplegableEx *Combo = nFila->AgregarEdicionDesplegable(TEXT(""), ID_COMBO_ORDEN_DIR, 0, false, 172);
	// AGREGAR EN TOOLTIP QUE EN ESPECIAL AYUDA A DETERCAR DISCOS DOBLES
	Combo->AgregarItem(IDIOMA__1_GENERO_GRUPO_DISCO_ESCANEO_POR_PATH);
	Combo->AgregarItem(IDIOMA__2_GRUPO_DISCO_ESCANEO_POR_PATH);
	Combo->AgregarItem(IDIOMA__3_DISCO_ESCANEO_POR_TAG);
	Combo->AgregarItem(IDIOMA__4_UN_POCO_DE_TODO_INDEFINIDO_ESCANEO_POR_TAG);
	Combo->FinAgregarItems();
	Combo->Activado(false);
	EscenaActual->AgregarSeparacion();
	
	nFila= EscenaActual->AgregarFrameH();
	nFila->AgregarMarca(IDIOMA__PATH_ANTES_DEL_TAG, ID_MARCA_PATH, false, Alineacion_Centrado)->Activado(false);
	nFila->AgregarSeparacion(10);
	nFila->AgregarMarca(IDIOMA__TAG_ANTES_DEL_PATH, ID_MARCA_TAG, false, Alineacion_Centrado)->Activado(false);
	nFila->AgregarSeparacion(10);
	nFila->AgregarBoton(IDIOMA__AGREGAR, 70, 18, ID_BOTON_AGREGAR_DIR, true, Alineacion_Izquierda)->Activado(false);
	EscenaActual->AgregarSeparacion();
	
	EscenaActual->AgregarTexto13(IDIOMA__ESTE_PROCESO_PUEDE_TARDAR_VARIOS_MINUTOS_DEPENDIENDO_DEL_ORDENADOR_Y_EL_NUMERO_DE_ARCHIVOS_REPRODUCIBLES);
	EscenaActual->AgregarSeparacion();
	EscenaActual->AgregarBarraProgreso(0, 100, 0, ID_BARRA_BUSCAR);
	EscenaActual->AgregarSeparacion();
	bool BuscarActivado = false;
	if (TmpLista->TotalItems() > 0) BuscarActivado = true;
	nFila = EscenaActual->AgregarFrameH();
	nFila->AgregarMarca(IDIOMA__FILTRADO_DE_CARACTERES, ID_FILTRO_CARACTERES, Sistema.App.Config.Buscar_FiltroCaracteres, Alineacion_Centrado)->Activado(BuscarActivado);
	nFila->AgregarSeparacion(10);
	nFila->AgregarMarca(IDIOMA__ANALISIS_POST_ESCANEO, ID_FILTRO_COINCIDENCIAS, Sistema.App.Config.Buscar_FiltroCoincidencias, Alineacion_Centrado)->Activado(BuscarActivado);
	nFila->AgregarSeparacion(10);
	DWL::ControlesEx::DWLBotonEx *nBoton = nFila->AgregarBoton(IDIOMA__BUSCAR, 70, ALTO_BOTON_CP, ID_BOTON_BUSCAR2, Alineacion_Izquierda);
	nBoton->Activado(BuscarActivado);
	nBoton->BotonEx_Estilos.Fuentes.Normal = DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita;
	nBoton->BotonEx_Estilos.Fuentes.SubRayada = DWL::SO::DWLEstilos::Fuentes.Tahoma13_Negrita_SubRayada;

	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_MaximoV(EscenaActual->PintarEscena());
	BarraScrollEx_PaginaV(RC.bottom);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC);
	Repintar(true);
}





LRESULT ControlPresentacionEx::Evento_EdicionDesplegableEx_CambioSeleccion(const TCHAR *NuevoTexto, const UINT IDEdicionTexto) {
	DWL::DWLString Str = NuevoTexto;
    PAINTSTRUCT PS;
	RECT RC;
	HDC hDC;
	DWL::ControlesEx::DWLEdicionDesplegableEx *nCombo = NULL;

	switch (IDEdicionTexto) {
		case ID_COMBO_DIR :
			nCombo = EscenaActual->BuscarEdicionDesplegable(ID_COMBO_ORDEN_DIR);
			if (nCombo != NULL) {
				nCombo->Activado(true);
				nCombo->Texto(IDIOMA__SELECCIONA_EL_TIPO_DE_NORMA);
			}
			break;
		case ID_COMBO_ORDEN_DIR :
			ControlMarca *MarcaTag, *MarcaPath;
			MarcaTag = EscenaActual->BuscarMarca(ID_MARCA_TAG);
			MarcaPath = EscenaActual->BuscarMarca(ID_MARCA_PATH);
			switch (NuevoTexto[0]) {
				case TEXT('1') : // Genero\Grupo\Disco\ 
					MarcaPath->Marcar(true);
					break;
				case TEXT('2') : // Grupo\Disco\ 
					MarcaPath->Marcar(true);
					break;
				case TEXT('3') : // Disco\ 
					MarcaTag->Marcar(true);
					break;
				default : // Indefinido 
					MarcaTag->Marcar(true);
					break;
				
			}
			EscenaActual->BuscarBoton(ID_BOTON_AGREGAR_DIR)->Activado(true);
			MarcaTag->Activado(true);
			MarcaPath->Activado(true);
			break;
		case ID_LISTA_INICIO :
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GENERO)	Sistema.App.Config.Inicio = Inicio_Genero_Aleatorio;
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GRUPO)	Sistema.App.Config.Inicio = Inicio_Grupo_Aleatorio;
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_DISCO)	Sistema.App.Config.Inicio = Inicio_Disco_Aleatorio;
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_ALEATORIAS)			Sistema.App.Config.Inicio = Inicio_Canciones_Aleatorias;
//			if (Str == TEXT("Lista con las canciones mas escuchadas."))		Sistema.App.Config.Inicio = Inicio_Canciones_MasEscuchadas;
			if (Str == IDIOMA__CUALQUIER_COSA)								Sistema.App.Config.Inicio = Inicio_Cualquier_Cosa;
			if (Str == IDIOMA__LISTA_VACIA)									Sistema.App.Config.Inicio = Inicio_Nada;
			break;
		case ID_OPCIONES_FUENTE :
			if (Str == IDIOMA__PEQUEÑA)	Sistema.App.Config.TamFuente = 12;
			if (Str == IDIOMA__MEDIANA)	Sistema.App.Config.TamFuente = 13;
			if (Str == IDIOMA__GRANDE)	Sistema.App.Config.TamFuente = 14;
			Sistema.App.Config.CargarFuentes();
			Sistema.App.ArbolBaseDatos.ArbolEx_ActualizarTamTexto();
			Sistema.App.PlayerEx.Ancho_Opciones_Basicas = Sistema.App.PlayerEx.OpcionesBasicas.CalcularAncho(GetParent(_hWnd));
			Sistema.App.PlayerEx.OpcionesBasicas.ArbolEx_ActualizarTamTexto();
			GetClientRect(GetParent(_hWnd), &RC);
			Sistema.App.PlayerEx.Evento_CambioTam(NULL, RC.right, RC.bottom);
			hDC = GetDC(Sistema.App.PlayerEx.hWnd());
			Sistema.App.PlayerEx.Evento_Pintar(hDC, PS);
			ReleaseDC(Sistema.App.PlayerEx.hWnd(), hDC);
			Mostrar_Opciones_Apariencia();
			Sistema.App.PlayerEx.BotonShufleRepeat.Repintar();
			Sistema.App.PlayerEx.Botones.Repintar();
			Sistema.App.SliderTiempo.Repintar();
			Sistema.App.PlayerEx.CM.Repintar();
			Sistema.App.PlayerEx.Volumen.Repintar(true);	
			break;
		case ID_OPCIONES_SKIN :
			Sistema.App.Config.Skin_Path = NuevoTexto;
			Sistema.App.Skin.CargarSkin(Sistema.App.Config.Skin_Path());

			GetClientRect(GetParent(_hWnd), &RC);
			Sistema.App.PlayerEx.Evento_CambioTam(NULL, RC.right, RC.bottom);

			hDC = GetDC(Sistema.App.PlayerEx.hWnd());
			Sistema.App.PlayerEx.Evento_Pintar(hDC, PS);
			ReleaseDC(Sistema.App.PlayerEx.hWnd(), hDC);
			Mostrar_Opciones_Apariencia();
			Sistema.App.PlayerEx.BotonShufleRepeat.Repintar();
			Sistema.App.PlayerEx.Botones.Repintar();
			Sistema.App.SliderTiempo.Repintar();
			Sistema.App.PlayerEx.CM.Repintar();
			Sistema.App.PlayerEx.Volumen.Repintar(true);
			break;
		case ID_OPCIONES_OSCY :
			if (Str == IDIOMA__GRANDE)	 Sistema.App.Config.TipoOscy = Oscy_Tres_Pixeles;
			if (Str == IDIOMA__MEDIANA)  Sistema.App.Config.TipoOscy = Oscy_Dos_Pixeles;
			if (Str == IDIOMA__PEQUEÑA)  Sistema.App.Config.TipoOscy = Oscy_Un_Pixel;
			break;
		case ID_LISTA_IDIOMAS :
			Sistema.App.Config.Idioma_Path = TEXT("Idioma"); Sistema.App.Config.Idioma_Path += NuevoTexto; Sistema.App.Config.Idioma_Path += TEXT(".dll");
			Sistema.App.DescargarIdioma();
			Sistema.App.CargarIdioma(Sistema.App.Config.Idioma_Path());

			Sistema.App.PlayerEx.Ancho_Opciones_Basicas = Sistema.App.PlayerEx.OpcionesBasicas.CalcularAncho(GetParent(_hWnd));
			Sistema.App.PlayerEx.OpcionesBasicas.RecargarIdioma();

			GetClientRect(GetParent(_hWnd), &RC);
			Sistema.App.PlayerEx.Evento_CambioTam(NULL, RC.right, RC.bottom);

			Mostrar_Opciones();
			break;
		case ID_TIPOS_LISTA :
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GENERO)		Sistema.App.Config.Combo_Inicio_Generar_Lista = 0;
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GRUPO)		Sistema.App.Config.Combo_Inicio_Generar_Lista = 1;
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_DISCO)		Sistema.App.Config.Combo_Inicio_Generar_Lista = 2;
			if (Str == IDIOMA__LISTA_CON_50_CANCIONES_ALEATORIAS)				Sistema.App.Config.Combo_Inicio_Generar_Lista = 3;
			if (Str == IDIOMA__LISTA_CON_MAS_ESCUCHADAS)						Sistema.App.Config.Combo_Inicio_Generar_Lista = 4;
			if (Str == IDIOMA__LISTA_CON_5ESTRELLAS)							Sistema.App.Config.Combo_Inicio_Generar_Lista = 5;
			
			if (Str == TEXT(""))	EscenaActual->BuscarBoton(ID_GENERAR_LISTA)->Activado(false);
			else					EscenaActual->BuscarBoton(ID_GENERAR_LISTA)->Activado(true);

			break;
	}

    return 0;
}



LRESULT ControlPresentacionEx::Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param) {
    DWL::DWLString TmpTxt;
    int TAudio = 0;
    int TVideo = 0;
    DWL::ControlesEx::DWLListaEx              *TmpLista = NULL;
    DWL::ControlesEx::DWLEdicionDesplegableEx *TmpCombo = NULL;
	DWL::ControlesEx::DWLMarcaEx			  *TmpMarca = NULL;
    TipoArchivo_Datos *TmpDatos = NULL;
    size_t i = 0;
	DWL::DWLString				StrOrden;
	DWL::DWLString				StrTipoBusqueda;

    switch (IDBotonEx) {
		case ID_BOTON_REVISAR :
			if (DWLStrCmpi(EscenaActual->BuscarBoton(ID_BOTON_REVISAR)->Texto(), IDIOMA__CANCELAR) == 0) {
				Sistema.App.BD.Parar_RevisarBD();
			}
			else {
				EscenaActual->BuscarBoton(ID_BOTON_REVISAR)->Texto(IDIOMA__CANCELAR);
				TAudio = Sistema.App.BD.RevisarBD(EscenaActual->BuscarBarraProgreso(ID_BARRA_REVISAR));
				EscenaActual->BuscarBoton(ID_BOTON_REVISAR)->Texto(IDIOMA__REVISAR_MEDIOS);
				DWL::DWLString Str;
				Str.sprintf(IDIOMA__REVISION_TERMINADA_SE_HAN_ELIMINADO_UN_TOTA_DE_D_ENTRADAS_NO_VALIDAS, TAudio);
				static VentanaMensaje_Aceptar MsgErr;
				MsgErr.MostrarMensaje(Str(), IDIOMA__REVISION_DE_LA_BASE_DE_DATOS, 0);
			}
			break;
		case ID_BOTON_BUSCAR2 :
            IniciarBusquedaArchivos();
			break;
        case ID_BOTON_ELIMINAR_TIPOS :
            TmpLista = EscenaActual->BuscarLista(ID_LISTA_TIPOS);
            for (i = 0; i < TmpLista->TotalItems(); i++) {
                if (TmpLista->Item(i)->Seleccionado() == true) {
					TmpDatos = Sistema.App.Tipos.BuscarTipoArchivo(TmpLista->Item(i)->Texto(0));
                    if (TmpDatos != NULL) { 
                        TmpDatos->Buscar_Asociar = false;
                        if (TmpDatos->Tipo == Tipo_Medio_Audio || TmpDatos->Tipo == Tipo_Medio_CDAudio) TAudio ++;
                        if (TmpDatos->Tipo == Tipo_Medio_Video) TVideo ++;
                    }
                }
            }
            
            if (TmpLista->EliminarItemsSeleccionados(true) > 0) {
                EscenaActual->BuscarBoton(ID_BOTON_RESTAURAR_TIPOS)->Activado(true);
            }
            if (TmpLista->TotalItems() == 0) EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS)->Activado(false);
            if (TAudio == 0)                 EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS_AUDIO)->Activado(false);
            if (TVideo == 0)                 EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS_AUDIO)->Activado(false);
            break;
        case ID_BOTON_RESTAURAR_TIPOS :
            TmpLista = EscenaActual->BuscarLista(ID_LISTA_TIPOS);
            TmpLista->EliminarTodosLosItems(false);
            Sistema.App.Tipos.TiposPorDefecto();
            for (i = 0; i < Sistema.App.Tipos.Tipos.size(); i++) {
				
				switch (Sistema.App.Tipos.Tipos[i]->Tipo) {
					case Tipo_Medio_Audio   : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__AUDIO);              break;
                    case Tipo_Medio_Video   : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__VIDEO);              break;
					case Tipo_Medio_Lista   : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__LISTA_DE_CANCIONES); break;
                    case Tipo_Medio_CDAudio : TmpTxt.sprintf(TEXT("(%s)"), IDIOMA__CDAUDIO);            break;
                }
                TmpLista->AgregarItem(NULL, 0, DWL_LISTAEX_ORDENADO, Sistema.App.Tipos.Tipos[i]->Extension(), TmpTxt());
            }
            TmpLista->ActualizarTodo();
            TmpLista->Repintar(true);
            EscenaActual->BuscarBoton(ID_BOTON_RESTAURAR_TIPOS)->Activado(false);
            EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS)->Activado(true);
            EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS_AUDIO)->Activado(true);
            EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS_VIDEO)->Activado(true);
            break;
        case ID_BOTON_ELIMINAR_TIPOS_AUDIO :
            TmpLista = EscenaActual->BuscarLista(ID_LISTA_TIPOS);
            TmpLista->DesSeleccionarTodo(false);
            for (i = 0; i < TmpLista->TotalItems(); i++) {
				TmpDatos = Sistema.App.Tipos.BuscarTipoArchivo(TmpLista->Item(i)->Texto(0));
                if (TmpDatos != NULL) {
                    if (TmpDatos->Tipo == Tipo_Medio_Audio || TmpDatos->Tipo == Tipo_Medio_CDAudio) {
                        TmpDatos->Buscar_Asociar = false;
                        TmpLista->Item(i)->Seleccionado(true, false);
                    }
                }
            }
            TmpLista->EliminarItemsSeleccionados(true);
            EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS_AUDIO)->Activado(false);
            EscenaActual->BuscarBoton(ID_BOTON_RESTAURAR_TIPOS)->Activado(true);
            break;
        case ID_BOTON_ELIMINAR_TIPOS_VIDEO :
            TmpLista = EscenaActual->BuscarLista(ID_LISTA_TIPOS);
            TmpLista->DesSeleccionarTodo(false);
            for (i = 0; i < TmpLista->TotalItems(); i++) {
				TmpDatos = Sistema.App.Tipos.BuscarTipoArchivo(TmpLista->Item(i)->Texto(0));
                if (TmpDatos != NULL) {
                    if (TmpDatos->Tipo == Tipo_Medio_Video) {
                        TmpDatos->Buscar_Asociar = false;
                        TmpLista->Item(i)->Seleccionado(true, false);
                    }
                }
            }
            TmpLista->EliminarItemsSeleccionados(true);
            EscenaActual->BuscarBoton(ID_BOTON_ELIMINAR_TIPOS_VIDEO)->Activado(false);
            EscenaActual->BuscarBoton(ID_BOTON_RESTAURAR_TIPOS)->Activado(true);
            break;

		// Agregar directorio a la busqueda de medios
		case ID_BOTON_AGREGAR_DIR :
			TmpTxt = EscenaActual->BuscarEdicionDesplegable(ID_COMBO_DIR)->Texto();
			const TCHAR                *Num;
			OrdenEstructuraDirectorios	Orden;
			if (EscenaActual->BuscarMarca(ID_MARCA_TAG)->Marcado() == true)	StrTipoBusqueda = IDIOMA___TAG_;
			else															StrTipoBusqueda = IDIOMA___PATH_;
			Num = EscenaActual->BuscarEdicionDesplegable(ID_COMBO_ORDEN_DIR)->Texto();
			switch (Num[0]) {
				case TEXT('1') : 
					Orden = GeneroGrupoDisco;	
					StrOrden = IDIOMA___GENERO_GRUPO_DISCO;
					break;
				case TEXT('2') : 
					Orden = GrupoDisco;		
					StrOrden = IDIOMA___GRUPO_DISCO;
					break;
				case TEXT('3') : 
					Orden = Disco;				
					StrOrden = IDIOMA___DISCO;
					break;
				default        : 
					Orden = Indefinido;	
					StrOrden = IDIOMA__INDEFINIDO;
					break;
			}
//			TmpTxt = TmpCombo->Texto();
			if (Sistema.App.Config.AgregarPathBuscar(TmpTxt(), Orden, EscenaActual->BuscarMarca(ID_MARCA_PATH)->Marcado()) == true) {
				TmpLista = EscenaActual->BuscarLista(ID_DIRECTORIOS_BUSCAR);
				if (TmpLista != NULL) {
					TmpLista->AgregarItem(NULL, DWL::SO::DWLListaIconos::CargarIcono(TmpTxt()), DWL_LISTAEX_FIN, TmpTxt(), StrOrden(), StrTipoBusqueda());
					TmpLista->ActualizarTodo();
					TmpLista->Repintar(true);
					EscenaActual->BuscarBoton(ID_BOTON_BUSCAR2)->Activado(true);
					EscenaActual->BuscarMarca(ID_FILTRO_CARACTERES)->Activado(true);
					EscenaActual->BuscarMarca(ID_FILTRO_COINCIDENCIAS)->Activado(true);

					TmpMarca = EscenaActual->BuscarMarca(ID_MARCA_TAG);
					TmpMarca->Marcar(false);
					TmpMarca->Activado(false);
					TmpMarca = EscenaActual->BuscarMarca(ID_MARCA_PATH);
					TmpMarca->Marcar(false);
					TmpMarca->Activado(false);
					EscenaActual->BuscarEdicionDesplegable(ID_COMBO_DIR)->Texto(IDIOMA__SELECCIONA_UN_DIRECTORIO, 0, true);
					TmpCombo = EscenaActual->BuscarEdicionDesplegable(ID_COMBO_ORDEN_DIR);
					TmpCombo->Activado(false);
					TmpCombo->Texto(TEXT(""));
				}
			}
			else {
				static VentanaMensaje_Aceptar Msg;
				
				Msg.MostrarMensaje(IDIOMA__ERROR_LA_RUTA_INTRODUCIDA_NO_ES_VALIDA_O_YA_EXISTE_EN_LA_LISTA, IDIOMA__ERROR, 0);
			}
			break;

		case ID_GENERAR_LISTA :
			TmpCombo = EscenaActual->BuscarEdicionDesplegable(ID_TIPOS_LISTA);
			TmpTxt = TmpCombo->Texto();
			i = 0;
			if (TmpTxt.Tam() > 0) {
				if		(DWLStrCmpi(TmpTxt(), IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GENERO) == 0)		i = 1;
				else if (DWLStrCmpi(TmpTxt(), IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_GRUPO) == 0)		i = 2;
				else if (DWLStrCmpi(TmpTxt(), IDIOMA__LISTA_CON_50_CANCIONES_BASADAS_EN_UN_DISCO) == 0)		i = 3;
				else if (DWLStrCmpi(TmpTxt(), IDIOMA__LISTA_CON_50_CANCIONES_ALEATORIAS) == 0) i = 4;
				else if (DWLStrCmpi(TmpTxt(), IDIOMA__LISTA_CON_5ESTRELLAS) == 0)			i = 5;
				else if (DWLStrCmpi(TmpTxt(), IDIOMA__LISTA_CON_MAS_ESCUCHADAS) == 0)	 	i = 6;
			}

			if (i != 0) {
				Sistema.App.Config.Combo_Inicio_Generar_Lista = TmpCombo->ItemSeleccionado();
				Sistema.App.Media.Stop();
				Sistema.App.ListaC.EliminarTodosLosMedios();
				switch (i) {
					case 1 : Sistema.App.BD.GeneroAleatorio();			break;
					case 2 : Sistema.App.BD.GrupoAleatorio();			break;
					case 3 : Sistema.App.BD.DiscoAleatorio();			break;
					case 4 : Sistema.App.BD.CancionesAleatorias(50);	break;
					case 5 : Sistema.App.BD.Estrellas5();               break;
					case 6 : Sistema.App.BD.CancionesMasEscuchadas(50); break;

				}
				Sistema.App.Media.AbrirPrimerMedioLista();
				Mostrar_Inicio();
			}
			break;
	}
    return 0;
}














