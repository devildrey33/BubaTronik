#include "StdAfx.h"
#include "ListaCanciones.h"
#include "resource.h"
#include <DWLListaIconos.h>
#include "BubaTronikApp.h"
#include <DWLDialogoGuardar.h>
#include "InformacionArchivoEx.h"


#define TIMER_TOOLTIPLISTA			1104

ListaCanciones::ListaCanciones(void) {
	_Ver_HorasMinutosSegundos = false;
	_Ver_DigitosPista = 2;
}


ListaCanciones::~ListaCanciones(void) {
}


HWND ListaCanciones::Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const int cID) {
	CrearListaEx(WS_CHILD, hWndParent, cX, cY, cAncho, cAlto, cID, false);
	AgregarColumna(40);
	AgregarColumna(cAncho - 128); // 128 == 18(icono) + 40(columna1) + 51(columna2) + 18(scroll)
	AgregarColumna(51);
	BarraScrollEx_AgregarBotonV(IDI_ELIMINAR2, ID_LISTA_ELIMINAR);
	BarraScrollEx_AgregarBotonV(IDI_DOWN, ID_LISTA_DOWN);
	BarraScrollEx_AgregarBotonV(IDI_UP, ID_LISTA_UP);
	BarraScrollEx_AgregarBotonV(IDI_GUARDAR, ID_LISTA_GUARDAR);

	ActualizarTodo();
	NombreLista = IDIOMA__LISTA_VACIA;
	return _hWnd;
}


void ListaCanciones::ActualizarTodo(const bool nRepintar) {
	// Calculamos los anchos necesarios para las columnas
	_Ver_HorasMinutosSegundos = false;
	_Ver_DigitosPista = 2;

	ItemListaMedios *aItem  = NULL;
	int              aPista = 0;
	for (size_t i = 0; i < _ListaEx_Items.size(); i++) {
		aItem = static_cast<ItemListaMedios *>(_ListaEx_Items[i]);
		switch (aItem->Medio->TipoMedio) {
			case Tipo_Medio_Audio :
				aPista = static_cast<TMedioAudio *>(aItem->Medio)->Pista;
				break;
			case Tipo_Medio_CDAudio :
				aPista = static_cast<TMedioCDAudio *>(aItem->Medio)->Pista;
				break;
			case Tipo_Medio_Video :
				aPista = static_cast<TMedioVideo *>(aItem->Medio)->Pista;
				break;
		}
		// Miro si la pista tiene 4 digitos
		if (aPista > 999) _Ver_DigitosPista = 4;
		if (aPista > 99 && _Ver_DigitosPista == 2) _Ver_DigitosPista = 3;
		// Miro si el tiempo llega a una hora
		if (static_cast<int>(((aItem->Medio->Tiempo / 1000) / 60) / 60) > 0) _Ver_HorasMinutosSegundos = true;
	}

	// Reasigno el ancho de las columnas
	RECT RC;
	GetClientRect(_hWnd, &RC);
	_ListaEx_Columnas[0]->Ancho(_Ver_DigitosPista * 10, false);
	if (_Ver_HorasMinutosSegundos == true) {
		_ListaEx_Columnas[1]->Ancho((RC.right - 36) - ((_Ver_DigitosPista * 10) + 51) , false);
		_ListaEx_Columnas[2]->Ancho(51, false);
	}
	else { // 36 = 18 icono + 18 scroll
		_ListaEx_Columnas[1]->Ancho((RC.right - 36) - ((_Ver_DigitosPista * 10) + 35) , false);
		_ListaEx_Columnas[2]->Ancho(35, false);
	}

	// Asignamos los textos de la lista correctamente
	DWL::DWLString PistaFinal;
	DWL::DWLString TiempoFinal;
	for (size_t i = 0; i < _ListaEx_Items.size(); i++) {
		aItem = static_cast<ItemListaMedios *>(_ListaEx_Items[i]);
		switch (aItem->Medio->TipoMedio) {
			case Tipo_Medio_Audio :
				aPista = static_cast<TMedioAudio *>(aItem->Medio)->Pista;
				break;
			case Tipo_Medio_CDAudio :
				aPista = static_cast<TMedioCDAudio *>(aItem->Medio)->Pista;
				break;
			case Tipo_Medio_Video :
				aPista = static_cast<TMedioVideo *>(aItem->Medio)->Pista;
				break;
		}
		if		(_Ver_DigitosPista == 4) PistaFinal.sprintf(TEXT("%04d"), aPista);
		else if (_Ver_DigitosPista == 3) PistaFinal.sprintf(TEXT("%03d"), aPista);
		else if (_Ver_DigitosPista == 2) PistaFinal.sprintf(TEXT("%02d"), aPista);
		_ListaEx_Items[i]->Texto(0, PistaFinal(), false);
		
		if (_Ver_HorasMinutosSegundos == true)	TiempoFinal = Sistema.App.Media.TiempoStrL(aItem->Medio->Tiempo);
		else									TiempoFinal = Sistema.App.Media.TiempoStr(aItem->Medio->Tiempo);
		_ListaEx_Items[i]->Texto(2, TiempoFinal(), false);
	}

	DWL::ControlesEx::DWLListaEx::ActualizarTodo(nRepintar);
}

ItemListaMedios *ListaCanciones::AgregarMedio(TMedio *nMedio) {
	for (size_t i = 0; i < _ListaEx_Items.size(); i++) {
		if (static_cast<ItemListaMedios *>(_ListaEx_Items[i])->Medio == nMedio) return static_cast<ItemListaMedios *>(_ListaEx_Items[i]); // La cancion ya existe en la lista
	}
	TCHAR Pista[12];
	int  Pt = 0;
	int nIcono = 0;
	switch (nMedio->TipoMedio) {
		case Tipo_Medio_Audio :
			Pt = static_cast<TMedioAudio *>(nMedio)->Pista;
			nIcono = Sistema.App.BD.MiraIconoCancion(nMedio->Reproducido);
			break;
		case Tipo_Medio_Video :
			nIcono = Sistema.App.BD.MiraIconoVideo(nMedio->Reproducido);
			Pt = static_cast<TMedioVideo *>(nMedio)->Pista;
			break;
		case Tipo_Medio_CDAudio :
			nIcono = IDI_PISTAAUDIO;
			Pt = static_cast<TMedioCDAudio *>(nMedio)->Pista;
			break;
	}
//	while (Pt > 100) Pt -= 100; // Si la pista es mas grande que 100 reduzco el numero a 2 digitos
	if (Pt == -1) Pt = 0;
	swprintf(Pista, 12, TEXT("%.04d"), Pt);

	ItemListaMedios *TmpMedio = new ItemListaMedios(static_cast<DWL::ControlesEx::DWLListaEx_Nucleo *>(this));
	TmpMedio->Medio = nMedio;
	TmpMedio->Medio->Item = static_cast<ItemListaMedios *>(AgregarItem(TmpMedio, nIcono, DWL_LISTAEX_FIN, Pista, nMedio->Nombre(), Sistema.App.Media.TiempoStrL(nMedio->Tiempo)));
	if (Sistema.App.Config.Shufle == true) Orig.push_back(TmpMedio);
	return TmpMedio;
}


void ListaCanciones::Shufle(const bool sShufle) {
	unsigned int i;
	unsigned int R;

	Sistema.App.PlayerEx.BotonShufleRepeat.ShufleMarcado(sShufle);
	Sistema.App.ControlesMP.BotonShufleRepeat.ShufleMarcado(sShufle);
	if (_ListaEx_Items.size() == 0) {
		Sistema.App.Config.Shufle = sShufle;
		// Cambio los iconos
		if (sShufle == true) {
			Sistema.App.PlayerEx.MenuShufle->Menu(0)->Icono(IDI_MARCA_MENU);
			Sistema.App.PlayerEx.MenuShufle->Menu(1)->Icono(NULL);
		}
		else {
			Sistema.App.PlayerEx.MenuShufle->Menu(0)->Icono(NULL);
			Sistema.App.PlayerEx.MenuShufle->Menu(1)->Icono(IDI_MARCA_MENU);
		}
		return;
	}

	if (sShufle == true) { // Mezclar la lista
		if (Sistema.App.Config.Shufle == false || Orig.size() == 0) {
			// Guardo el Orden original de las canciones
			Orig.clear();
			for (i = 0; i < _ListaEx_Items.size(); i++)  {
				Orig.push_back(ItemMedio(i));
			}
			// Mezclo los BDItems
		}
		for (i = 0; i < _ListaEx_Items.size(); i++)  _ListaEx_Items[i] = NULL;
		i = 0;
		while (i < _ListaEx_Items.size()) {
			R = static_cast<unsigned int>(rand()%_ListaEx_Items.size());
			while (_ListaEx_Items[R] != NULL) {
				R++;
				if (R == _ListaEx_Items.size()) R = 0;
			}
			_ListaEx_Items[R] = Orig[i]; // ha petat un cop, i el vector orig era nomes de 3 espais mentres que el de items era de 8.... molt chungo, s'ha de revisar i probar be.
			i++;
		}
	}

	else { // Restaurar orden de la lista
		for (i = 0; i < _ListaEx_Items.size(); i++) _ListaEx_Items[i] = Orig[i];
	}
	TMedio *bMedio = NULL;
	if (Sistema.App.Media.MedioActual != NULL) {
		if (Sistema.App.Media.MedioActual->Medio != NULL) {
			bMedio = Sistema.App.Media.MedioActual->Medio;
			for (i = 0; i < _ListaEx_Items.size(); i++) {
				if (bMedio == ItemMedio(i)->Medio) {
					switch (Sistema.App.Media.ComprobarEstado()) {
						case EnPlay :
							_ListaEx_Items[i]->Icono(IDI_PLAY, false);
							break;
						case EnPausa :
							_ListaEx_Items[i]->Icono(IDI_PAUSA, false);
							break;
						case EnStop :
							if (ItemMedio(i)->Medio->TipoMedio == Tipo_Medio_Audio)	_ListaEx_Items[i]->Icono(Sistema.App.BD.MiraIconoCancion(ItemMedio(i)->Medio->Reproducido), false);
							else													_ListaEx_Items[i]->Icono(Sistema.App.BD.MiraIconoVideo(ItemMedio(i)->Medio->Reproducido), false);
							break;

					}
					Sistema.App.Media.Actual = i;
				}
				else {
					if (ItemMedio(i)->Medio->TipoMedio == Tipo_Medio_Audio) 	_ListaEx_Items[i]->Icono(Sistema.App.BD.MiraIconoCancion(ItemMedio(i)->Medio->Reproducido), false);
					else														_ListaEx_Items[i]->Icono(Sistema.App.BD.MiraIconoVideo(ItemMedio(i)->Medio->Reproducido), false);
				}
			}
		}

	}
	// Reestructuro los nodos siguiente y anterior de la lista
	ReestructurarNodos();
	Sistema.App.Config.Shufle = sShufle;

	Repintar(true);

	// Cambio los iconos
	if (sShufle == true) {
		Sistema.App.PlayerEx.MenuShufle->Menu(0)->Icono(IDI_MARCA_MENU);
		Sistema.App.PlayerEx.MenuShufle->Menu(1)->Icono(NULL);
	}
	else {
		Sistema.App.PlayerEx.MenuShufle->Menu(0)->Icono(NULL);
		Sistema.App.PlayerEx.MenuShufle->Menu(1)->Icono(IDI_MARCA_MENU);
	}
}


void ListaCanciones::ReestructurarNodos(void) {
	// Reestructuro los nodos siguiente y anterior de la lista
	int ItemSize = TotalItems();
	ItemMedio(0)->Anterior(NULL);
	if (ItemSize > 1) {
		ItemMedio(0)->Siguiente(ItemMedio(1));
		for (int x = 1; x < ItemSize -1; x++) {
			ItemMedio(x)->Siguiente(ItemMedio(x +1));
			ItemMedio(x)->Anterior(ItemMedio(x -1));
		}
		ItemMedio(ItemSize -1)->Siguiente(NULL);
		ItemMedio(ItemSize -1)->Anterior(ItemMedio(ItemSize -2));
	}
	else {
		ItemMedio(0)->Siguiente(NULL);
	}
}


void ListaCanciones::EliminarTodosLosMedios(void) {
	for (unsigned int i = 0; i < _ListaEx_Items.size(); i++) {
		if (ItemMedio(i)->Medio != NULL)	ItemMedio(i)->Medio->Item = NULL;
	}
	_ListaEx_UItemResaltado = NULL;
	_ListaEx_ItemResaltado = NULL;
	EliminarTodosLosItems();
	Sistema.App.Media.MedioActual = NULL;
	Orig.resize(0);
	NombreLista = IDIOMA__LISTA_VACIA;
	Sistema.App.Media.EliminarMedioActual();
	Sistema.App.Media.Actual = -1;
}


// REVISAR
void ListaCanciones::EliminarMediosSeleccionados(void) {
	unsigned int i = static_cast<unsigned int>(_ListaEx_Items.size() -1);
	unsigned int Z = 0;
	bool         Parado = false;
	_ListaEx_UItemResaltado = NULL;
	_ListaEx_ItemResaltado = NULL;
	for (i = i; i >= 0; i--) {
		if (i > _ListaEx_Items.size() -1 || _ListaEx_Items.size() == 0) {
			if (_ListaEx_Items.size() == 0) {
				NombreLista = IDIOMA__LISTA_VACIA;
				Sistema.App.Media.Actual = -1;
			}
			else  NombreLista = IDIOMA__MODIFICADA; // Lista Modificada
			_BarraScrollEx_MaximoV = static_cast<long>(_ListaEx_Items.size());
			if ((_BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV) > 0) {
				if (_BarraScrollEx_ValorV > _BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV) _BarraScrollEx_ValorV = _BarraScrollEx_MaximoV - _BarraScrollEx_PaginaV;
				_ListaEx_ItemMarcado = NULL;
				_ListaEx_ItemShifteado = NULL;
			}
			else {
				_ListaEx_ItemMarcado = NULL;
				_ListaEx_ItemShifteado = NULL;
			}
			ActualizarTodo();
			Repintar(true);
			if (Parado == true) {
				if (_ListaEx_Items.size() > 0) {
					if (Sistema.App.Media.Actual < 0) Sistema.App.Media.Actual = 0;
					Sistema.App.Media.AbrirMedio(ItemMedio(Sistema.App.Media.Actual));
					Sistema.App.Media.Play();
				}
				else {
					Sistema.App.Media.EliminarMedioActual();
				}
			}
			return;
		}
		if (_ListaEx_Items[i]->Seleccionado() == true) {
			if (Sistema.App.Media.MedioActual != NULL) {
				if (Sistema.App.Media.MedioActual->Medio != NULL) {
					if (Sistema.App.Media.MedioActual->Medio == ItemMedio(i)->Medio) { 
						Sistema.App.Media.EliminarMedioActual();
						Parado = true;
					}
				}
			}
			else {
				Parado = true;
			}
			if (Sistema.App.Media.Actual >= static_cast<int>(i)) Sistema.App.Media.Actual --; //Sistema.App.FMod.AsignarActual(Sistema.App.FMod.Actual() -1); 
			if (Sistema.App.Config.Shufle == true) {
				for (Z = 0; Z < Orig.size(); Z++)  { if (Orig[Z] == ItemMedio(i))  break; }
				Orig.erase(Orig.begin() + Z);            // Borro el puntero 'ItemListaMedios' del vector
			}
			if (ItemMedio(i)->Medio != NULL)	ItemMedio(i)->Medio->Item = NULL;
			EliminarItem(_ListaEx_Items[i]);
		}
	}
}



void ListaCanciones::EliminarMedio(ItemListaMedios *Itm) {
	if (_ListaEx_Items.size() == 0) return;
	unsigned int i;
	for (i = 0; i < _ListaEx_Items.size(); i++) {
		if (static_cast<ItemListaMedios *>(_ListaEx_Items[i]) == Itm) break;
	}
	if (i == _ListaEx_Items.size()) return; // Si no se ha encontrado el item salgo de la funcion
	if (Sistema.App.Media.MedioActual != NULL) {
		if (Sistema.App.Media.MedioActual->Medio != NULL) {
			if (Sistema.App.Media.MedioActual->Medio == Itm->Medio) { 
				if (_ListaEx_Items.size() > 1) {
					Sistema.App.Media.Siguiente();
				}
				else {
					Sistema.App.Media.EliminarMedioActual();
				}
			}
		}
	}
	_ListaEx_UItemResaltado = NULL;
	_ListaEx_ItemResaltado = NULL;

	unsigned int Z;
	if (Sistema.App.Config.Shufle == true) {
		for (Z = 0; Z < Orig.size(); Z++) if (Orig[Z] == Itm)  break;
		Orig.erase(Orig.begin() + Z);            // Borro el puntero 'ItemListaMedios' del vector
	}

	EliminarItem(Itm);
 
	if (_ListaEx_Items.size() > 0) {
		NombreLista = IDIOMA__MODIFICADA; // Lista Modificada
	}
	else { 
		NombreLista = IDIOMA__LISTA_VACIA;
		Sistema.App.Media.Actual = -1;
	}
}


LRESULT ListaCanciones::Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto) {
	RECT RC;
	GetClientRect(_hWnd, &RC);
	BarraScrollEx_PosicionarScrolls(_hWnd, &RC, true, false);
    return 0;
}

void ListaCanciones::AbrirMedio(TMedio *nMedio) {
	size_t Tam = _ListaEx_Items.size();
    ItemListaMedios *TmpItem;
	for (int i = 0; i != Tam; i++) {
        TmpItem = static_cast<ItemListaMedios *>(_ListaEx_Items[i]);
        if (nMedio == TmpItem->Medio) {
			Sistema.App.Media.Stop();
			Sistema.App.Media.Actual = i;
            Sistema.App.Media.AbrirMedio(TmpItem);
			Sistema.App.Media.Play();
			return;
        }
    }
}


void ListaCanciones::AbrirMedioMarcado(void) {
	size_t Tam = _ListaEx_Items.size();
	for (int i = 0; i != Tam; i++) {
		if (_ListaEx_ItemMarcado == _ListaEx_Items[i]) {
			Sistema.App.Media.Stop();
			Sistema.App.Media.Actual = i;
			Sistema.App.Media.AbrirMedio(static_cast<ItemListaMedios *>(_ListaEx_Items[i]));
			Sistema.App.Media.Play();
			return;
		}
	}
}


void ListaCanciones::Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool cRepintar) {
	// 90 == 18(icono) + 20(columna1) + 34(columna3) + 18(scroll)
	if (_ListaEx_Columnas.size() > 0) {
		// Reasigno el ancho de las columnas
		if (_Ver_HorasMinutosSegundos == true)	_ListaEx_Columnas[1]->Ancho((cAncho - 36) - ((_Ver_DigitosPista * 10) + 51) , false); // 36 es el ancho del icono + el scroll
		else									_ListaEx_Columnas[1]->Ancho((cAncho - 36) - ((_Ver_DigitosPista * 10) + 35) , false); 
	}
	DWL::ControlesEx::DWLListaEx::Mover(cX, cY, cAncho, cAlto, cRepintar);
}


LRESULT ListaCanciones::Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param) {
	DWL::ControlesEx::DWLListaEx::Evento_Mouse_BotonSoltado(Boton, cX, cY, Param);
	ItemListaMedios *tt;
	switch (Boton) {
		case 1 : // Mostrar Menu Lista
			Sistema.App.PlayerEx.MostrarMenuLista();
			break;
		case 3 : // Mostrar Tooltip
			if (_ListaEx_ItemResaltado != NULL) {
				tt = static_cast<ItemListaMedios *>(_ListaEx_ItemResaltado);
				if (tt->Medio != NULL) Sistema.App.ArbolBaseDatos.MostrarToolTipNodo(tt->Medio->Nodo);
			}

			break;
	}
    return 0;
}

LRESULT ListaCanciones::Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param) {
	DWL::ControlesEx::DWLListaEx::Evento_Mouse_Movimiento(cX, cY, Param);
    return 0;
}

LRESULT ListaCanciones::Evento_Mouse_Rueda(const short Delta, const short cX, const short cY, const UINT VirtKey) {
	DWL::ControlesEx::DWLListaEx::Evento_Mouse_Rueda(Delta, cX, cY, VirtKey);
    return 0;
}

void ListaCanciones::Visible(const bool Vis) {
	if (Vis == true) {
		ShowWindow(_hWnd, SW_SHOW);
		SetTimer(GetParent(_hWnd), TIMER_TOOLTIPLISTA, 500, NULL);
	}
	else {
		ShowWindow(_hWnd, SW_HIDE);
		KillTimer(GetParent(_hWnd), TIMER_TOOLTIPLISTA);
	}
}


LRESULT ListaCanciones::Evento_Teclado_TeclaSoltada(const UINT Tecla, const UINT Repeticion, const UINT Param) {
	DWL::ControlesEx::DWLListaEx::Evento_Teclado_TeclaSoltada(Tecla, Repeticion, Param);
	switch (Tecla) {
		case VK_DELETE :
			EliminarMediosSeleccionados();
			break;
		case VK_RETURN :
			AbrirMedioMarcado();
			break;
	}
    return 0;
}


void ListaCanciones::ListaEx_Evento_Mouse_DobleClick(const UINT Boton, const int cX, const int cY) {
//	DWL::ControlesEx::DWLListaEx::Evento_Mouse_DobleClick(Boton, cX, cY);
	if (Boton == 0) AbrirMedioMarcado();
}


void ListaCanciones::ListaEx_Evento_Pintar_Fondo(HDC hDC, RECT *Espacio) {
	HBRUSH Brocha = CreateSolidBrush(ListaEx_Estilos.Colores.Fondo());
	FillRect(hDC, Espacio, Brocha);
	DWL::SO::DWLListaIconos::PintarIcono(hDC, Espacio->right -158, Espacio->bottom -138, 128, 128, IDI_LISTA);
	DeleteObject(Brocha);
	
}

void ListaCanciones::BarraScrollEx_Evento_BotonExtendido_Click(DWL::ControlesEx::DWLBarraScrollEx_Boton *BotonExtendido) {
	switch (BotonExtendido->IDControl) {
		case ID_LISTA_UP		:	MoverSeleccionArriba();			break;
		case ID_LISTA_DOWN		:	MoverSeleccionAbajo();			break;
		case ID_LISTA_ELIMINAR	:	EliminarMediosSeleccionados();	break;
		case ID_LISTA_GUARDAR   :   MostrarGuardarLista();          break;
	}
}

void ListaCanciones::MostrarGuardarLista(void) {
	DWL::Ventanas::DWLDialogoGuardar	DlgGuardar;
	const TCHAR                        *PathFinal = DlgGuardar.MostrarGuardar(TEXT(""), IDIOMA__LISTA_M3U_M3U, IDIOMA__GUARDAR_LISTA);
	_PathM3u = PathFinal;
	// Agregamos la extensión si no la tiene
	if (_PathM3u.Tam() > 4) if (_PathM3u.SubStr(_PathM3u.Tam() - 4) != TEXT(".M3U")) _PathM3u += TEXT(".M3U");
	// Si no tiene como minimo 8 caracteres no es una ruta válida
	if (_PathM3u.Tam() > 8) {
		DWL::Archivos::DWLArchivoBinario ArchivoM3U;
		// El archivo ya existe
		if (ArchivoM3U.AbrirArchivoLectura(_PathM3u()) == true) {
			DWL::DWLString MsgError;
			MsgError.sprintf(IDIOMA__EL_ARCHIVO_S_YA_EXISTE, _PathM3u());
			static VentanaMensaje_AceptarCancelar MsgArchivoExiste(MsgError(), IDIOMA__YA_EXISTE_UN_ARCHIVO, ID_MENSJAE_SOBREESCRIBIRM3U, TEXT("Si"), TEXT("No"));
		}
		else {
			GuardarLista();
		}
	}
}


void ListaCanciones::GuardarLista(void) {
	DWL::DWLString_char					LineaTmp;
	DWL::Archivos::DWLArchivoBinario	ArchivoM3U;
	static VentanaMensaje_Aceptar		MsgError;
	DWL::DWLString MensajeStr;

	ArchivoM3U.AbrirArchivoEscritura(_PathM3u());
	BOOL Error = FALSE;
	for (size_t i = 0; i < _ListaEx_Items.size(); i++) {
		LineaTmp = static_cast<ItemListaMedios *>(_ListaEx_Items[i])->Medio->Path();
		LineaTmp += "\r\n"; // separador de linea en win para archivos de texto
		if (ArchivoM3U.Guardar(LineaTmp(), LineaTmp.Tam()) == FALSE) Error = TRUE;
	}
	ArchivoM3U.CerrarArchivo();
	if (Error == TRUE) {
		MsgError.MostrarMensaje(IDIOMA__NO_SE_HA_PODIDO_GUARDAR_EL_ARCHIVO, IDIOMA__ERROR_GUARDANDO_LA_LISTA, 0);
	}
	else {
		MensajeStr.sprintf(IDIOMA__LA_LISTA_SE_HA_GUARDADO_EN_S, _PathM3u());
		MsgError.MostrarMensaje(MensajeStr(), IDIOMA__LISTA_GUARDADA, 0);
		InformacionArchivoEx Info;
		TMedio              *nMedio = NULL;
		nMedio = Sistema.App.BD.BuscarPath(_PathM3u());
		if (nMedio == NULL) {
			nMedio = Info.ObtenerInfo(_PathM3u(), Sistema.App.Tipos, Sistema.App.UnidadesDisco, Sistema.App.Config.Buscar_Paths, Sistema.App.VLC.InstanciaVLC(), Sistema.App.FMod.Sistema_FMOD());
			Sistema.App.BD.AgregarMedioBD(nMedio);
		}
		Sistema.App.BD.ActualizarArbol();
	}
	_PathM3u.Borrar();
}

void ListaCanciones::MoverSeleccionArriba(void) {
    ItemListaMedios *Tmp = NULL;	
	int i;
	for (i = 1; i < static_cast<int>(_ListaEx_Items.size()); i++) {
		if (_ListaEx_Items[i]->Seleccionado() == true && _ListaEx_Items[i -1]->Seleccionado() != true) {
			Tmp = static_cast<ItemListaMedios *>(_ListaEx_Items[i - 1]);
			_ListaEx_Items[i -1]	= _ListaEx_Items[i];
			_ListaEx_Items[i]		= Tmp;
		}
	}
	for (i = 0; i < static_cast<int>(_ListaEx_Items.size()); i++) {
		if (_ListaEx_Items[i]->Icono() == IDI_PLAY) {
			Sistema.App.Media.Actual = i;
			break;
		}
	}
	// Reestructuro los nodos siguiente y anterior de la lista
	ReestructurarNodos();
	if (_ListaEx_ItemMarcado != NULL) MostrarItem(_ListaEx_ItemMarcado);

	Repintar(true);
}


void ListaCanciones::MoverSeleccionAbajo(void) {
    ItemListaMedios *Tmp = NULL;	
	int i;
	for (i = static_cast<int>(_ListaEx_Items.size()) -2; i > -1; i--) {
		if (_ListaEx_Items[i]->Seleccionado() == true && _ListaEx_Items[i + 1]->Seleccionado() != true) {
			Tmp = static_cast<ItemListaMedios *>(_ListaEx_Items[i + 1]);
			_ListaEx_Items[i + 1]	= _ListaEx_Items[i];
			_ListaEx_Items[i]		= Tmp;
		}
	}
	for (i = 0; i < static_cast<int>(_ListaEx_Items.size()); i++) {
		if (_ListaEx_Items[i]->Icono() == IDI_PLAY) {
			Sistema.App.Media.Actual = i;
			break;
		}
	}
	// Reestructuro los nodos siguiente y anterior de la lista
	ReestructurarNodos();
	if (_ListaEx_ItemMarcado != NULL) MostrarItem(_ListaEx_ItemMarcado);

	Repintar(true);	
}

