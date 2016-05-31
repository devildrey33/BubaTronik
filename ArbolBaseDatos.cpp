#include "StdAfx.h"
#include "ArbolBaseDatos.h"
#include "resource.h"
#include <DWLListaIconos.h>
#include "BubaTronikApp.h"
#include "InformacionArchivoEx.h"

//DWL_APP(BubaTronikApp);

/*LRESULT CALLBACK ArbolBaseDatos::GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	int prova = -1;
	switch (uMsg) {
//		case WM_MBUTTONUP :
//		case WM_XBUTTONUP :
//			App.ToolTip.TiempoMS = GetTickCount() + TIEMPO_TOOLTIP;
///			prova = HIWORD(wParam);
//			SendMessage(_hWnd, WM_TIMER, 555, 0);
			break;
//		case WM_MOUSEMOVE :
//		case WM_MOUSEWHEEL :
			break;
//		case WM_DROPFILES :
//			App.PlayerEx.SoltarArchivos(wParam);
			return 0;
	}
	return DWL::ControlesEx::DWLArbolEx::GestorMensajes(uMsg, wParam, lParam);
}*/

ArbolBaseDatos::ArbolBaseDatos(void) {
}

ArbolBaseDatos::~ArbolBaseDatos(void) {
}

HWND ArbolBaseDatos::Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const int nID) {
	CrearArbolEx(WS_CHILD, hWndParent, cX, cY, cAncho, cAlto, nID, false);
	BarraScrollEx_VisibleV(TRUE);
	ArbolEx_Estilos.SubSeleccion = true;
	ActualizarTodo(true);
	return _hWnd;
}


NodoArbolBaseDatos *ArbolBaseDatos::AgregarNodoMedio(NodoArbolBaseDatos *Parent, const TCHAR *cTexto, const int cIko, TTipo *Tipo, TGenero *Genero, TGrupo *Grupo, TDisco *Disco, TMedio *nMedio, const bool AgregarAlFinal) {
	NodoArbolBaseDatos *Nodo = new NodoArbolBaseDatos;
	Nodo->Tipo = Tipo;
	Nodo->Genero = Genero;
	Nodo->Grupo = Grupo;
	Nodo->Disco = Disco;
	Nodo->Medio = nMedio;
	AgregarNodo(Nodo, Parent, cIko, cTexto, (AgregarAlFinal == true) ? DWL_ARBOLEX_FIN : DWL_ARBOLEX_ORDENADO); 
	return Nodo;
}

NodoArbolBaseDatos *ArbolBaseDatos::AgregarNodoMedio(NodoArbolBaseDatos *Parent, const int nIcono, const TCHAR *nTexto, TMedio *nMedio) {
	NodoArbolBaseDatos *Nodo = new NodoArbolBaseDatos();
	Nodo->Medio		= nMedio;
	Nodo->Tipo		= NULL;
	Nodo->Genero	= NULL;
	Nodo->Grupo		= NULL;
	Nodo->Disco		= NULL;
	if (nMedio != NULL) Nodo->Medio->Nodo = Nodo;
	AgregarNodo(Nodo, Parent, nIcono, nTexto);
	return Nodo;
}

NodoArbolBaseDatos *ArbolBaseDatos::AgregarNodoMedioM3u(NodoArbolBaseDatos *Parent, const int nIcono, const TCHAR *nTexto, TMedio *nMedio) {
	NodoArbolBaseDatos *Nodo = new NodoArbolBaseDatos();
	Nodo->Medio		= nMedio;
	Nodo->Tipo		= NULL;
	Nodo->Genero	= NULL;
	Nodo->Grupo		= NULL;
	Nodo->Disco		= NULL;
	if (nMedio != NULL) Nodo->Medio->Nodo = Nodo;
	AgregarNodo(Nodo, Parent, nIcono, nTexto, Parent->TotalHijos());
	return Nodo;
}

void ArbolBaseDatos::MostrarToolTipNodoVideo(NodoArbolBaseDatos *nNodo) {
	DWL::ControlesEx::DWLToolTipEx *nToolTip = &Sistema.App.ToolTip;
	DWL::DWLString	Tmp;
	nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Medio->Nombre());
	Tmp = static_cast<TMedioVideo *>(nNodo->Medio)->Pista;
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__NUMERACION, TEXT(':'), Tmp());
	
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStr(nNodo->Medio->Tiempo));
	Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(nNodo->Medio->Longitud / 1000000.0f));
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());

	nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16, static_cast<float>(nNodo->Medio->Nota));
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__ANCHO, TEXT(':'), static_cast<TMedioVideo *>(nNodo->Medio)->Ancho);
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__ALTO, TEXT(':'), static_cast<TMedioVideo *>(nNodo->Medio)->Alto);
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__PATH, TEXT(':'), nNodo->Medio->Path());
}

void ArbolBaseDatos::MostrarToolTipNodoCancion(NodoArbolBaseDatos *nNodo) {

	DWL::ControlesEx::DWLToolTipEx *nToolTip = &Sistema.App.ToolTip;

	DWL::DWLString	Tmp;
	size_t			TotalEntradas = 0;
	size_t			Total = 0;
//	int				Divisor = 0;
//	float			Nota = 0.0f;

	TMedioAudio *TmpAudio = static_cast<TMedioAudio *>(nNodo->Medio);

	nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Medio->Nombre());
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIPO, TEXT(':'), TmpAudio->StrTipo->Nombre());
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__GENERO, TEXT(':'), TmpAudio->StrGenero->Nombre());
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__GRUPO, TEXT(':'), TmpAudio->StrGrupo->Nombre());
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__DISCO, TEXT(':'), TmpAudio->StrDisco->Nombre());

	Tmp = TmpAudio->Pista;
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__PISTA, TEXT(':'), Tmp());
	
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStr(TmpAudio->Tiempo));
	Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(TmpAudio->Longitud / 1000000.0f));
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());
	Tmp.sprintf(IDIOMA__D_VECES, nNodo->Medio->Reproducido);
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__ESCUCHADA, TEXT(':'), Tmp());
//	Tmp = nNodo->Medio->Nota;
	nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16, static_cast<float>(TmpAudio->Nota));
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__PATH, TEXT(':'), TmpAudio->Path());

	if (Sistema.App.Config.MostrarTagsEnToolTips == true) {
		InformacionArchivoEx Info;
		Info.ObtenerMetaDatosAudio(TmpAudio->Path(), Sistema.App.Tipos);

		// ID3V2
		UINT Tam = Info.ID3V2.MetaDatos.size();
		if (Tam > 0) {
			nToolTip->AgregarTextoNegrita(IDIOMA__DATOS_DEL_TAG_VERSION_2);
			for (UINT i = 0; i < Tam; i++) {
				nToolTip->AgregarTextoCaracterTexto(Info.ID3V2.MetaDatos[i]->Nombre(), TEXT(':'), Info.ID3V2.MetaDatos[i]->Datos());
			}
		}
		// WMA / ASF
		Tam = Info.WMA.MetaDatos.size();
		if (Tam > 0) {
			nToolTip->AgregarTextoNegrita(IDIOMA__DATOS_DEL_TAG_ASF);
			for (UINT i = 0; i < Tam; i++) {
				nToolTip->AgregarTextoCaracterTexto(Info.WMA.MetaDatos[i]->Nombre(), TEXT(':'), Info.WMA.MetaDatos[i]->Datos());
			}
		}
		// Vorbis
		Tam = Info.Vorbis.MetaDatos.size();
		if (Tam > 0) {
			nToolTip->AgregarTextoNegrita(IDIOMA__DATOS_DEL_TAG_VORBIS);
			for (UINT i = 0; i < Tam; i++) {
				nToolTip->AgregarTextoCaracterTexto(Info.Vorbis.MetaDatos[i]->Nombre(), TEXT(':'), Info.Vorbis.MetaDatos[i]->Datos());
			}
		}

		// ID3V1
		Tam = Info.ID3V1.MetaDatos.size();
		if (Tam > 0) {
			nToolTip->AgregarTextoNegrita(IDIOMA__DATOS_DEL_TAG_VERSION_1);
			for (UINT i = 0; i < Tam; i++) {
				nToolTip->AgregarTextoCaracterTexto(Info.ID3V1.MetaDatos[i]->Nombre(), TEXT(':'), Info.ID3V1.MetaDatos[i]->Datos());
			}
		}

	}
}


void ArbolBaseDatos::MostrarToolTipNodoPistaCD(NodoArbolBaseDatos *ttNodo) {
	DWL::ControlesEx::DWLToolTipEx *nToolTip = &Sistema.App.ToolTip;
	nToolTip->AgregarIconoTexto(ttNodo->Icono(), 16, 16, ttNodo->Medio->Nombre());
	DWL::DWLString Tmp = static_cast<TMedioAudio *>(ttNodo->Medio)->Pista;
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__PISTA, TEXT(':'), Tmp());
	nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStrL(ttNodo->Medio->Tiempo));
	nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16,static_cast<float>(ttNodo->Medio->Nota));
}


void ArbolBaseDatos::MostrarToolTipNodo(NodoArbolBaseDatos *ttNodo) {
	if (ttNodo == NULL) return;
	DWL::ControlesEx::DWLToolTipEx *nToolTip = &Sistema.App.ToolTip;
	BaseDatos *BD = &Sistema.App.BD;
	nToolTip->BorrarMemoria();
	NodoArbolBaseDatos *nNodo = ttNodo;
	DWL::DWLString	Tmp;
	size_t			TotalEntradas = 0;
	size_t			Total = 0;
	int				Divisor = 0;
	float			Nota = 0.0f;
	switch (nNodo->Icono()) {
		case IDI_PLAY :
			if		(nNodo->Medio->TipoMedio == Tipo_Medio_Video)	MostrarToolTipNodoVideo(nNodo);
			else if (nNodo->Medio->TipoMedio == Tipo_Medio_Audio)	MostrarToolTipNodoCancion(nNodo);
			else if (nNodo->Medio->TipoMedio == Tipo_Medio_CDAudio)	MostrarToolTipNodoPistaCD(nNodo);
			break;
		case IDI_VIDEO :
			MostrarToolTipNodoVideo(nNodo);
			break;
		case IDI_AUDIO : // cancion
			MostrarToolTipNodoCancion(nNodo);
			break;
		case IDI_PISTAAUDIO :
			MostrarToolTipNodoPistaCD(nNodo);
			break;
		case IDI_TIPO :
			nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Tipo->Nombre->Nombre());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__GENEROS, TEXT(':'), nNodo->Tipo->Generos);
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__GRUPOS, TEXT(':'), nNodo->Tipo->Grupos);
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__DISCOS, TEXT(':'), nNodo->Tipo->Discos);
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__CANCIONES, TEXT(':'), nNodo->Tipo->Canciones);
			Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(nNodo->Tipo->Longitud / 1000000.0f));
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStrL(nNodo->Tipo->Tiempo));
			for (Total = 0; Total < BD->Medios.size(); Total ++) {
				if (BD->Medios[Total]->TipoMedio == Tipo_Medio_Audio) {
					if (nNodo->Tipo == static_cast<TMedioAudio *>(BD->Medios[Total])->Tipo) {
						Divisor ++;
						Nota += BD->Medios[Total]->Nota;
					}
				}
			}
			Nota = Nota / static_cast<float>(Divisor);
			nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16, static_cast<float>(Nota));
			break;
		case IDI_GENERO :
			nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Genero->Nombre->Nombre());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__GRUPOS, TEXT(':'), nNodo->Genero->Grupos);
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__DISCOS, TEXT(':'), nNodo->Genero->Discos);
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__CANCIONES, TEXT(':'), nNodo->Genero->Canciones);
			Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(nNodo->Genero->Longitud / 1000000.0f));
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStrL(nNodo->Genero->Tiempo));
			for (Total = 0; Total < BD->Medios.size(); Total ++) {
				if (BD->Medios[Total]->TipoMedio == Tipo_Medio_Audio) {
					if (nNodo->Genero == static_cast<TMedioAudio *>(BD->Medios[Total])->Genero) {
						Divisor ++;
						Nota += BD->Medios[Total]->Nota;
					}
				}
			}
			Nota = Nota / static_cast<float>(Divisor);
			nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16,static_cast<float>(Nota));
			break;
		case IDI_GRUPO :
			nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Grupo->Nombre->Nombre());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__DISCOS, TEXT(':'), nNodo->Grupo->Discos);
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__CANCIONES, TEXT(':'), nNodo->Grupo->Canciones);
			Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(nNodo->Grupo->Longitud / 1000000.0f));
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStrL(nNodo->Grupo->Tiempo));
			for (Total = 0; Total < BD->Medios.size(); Total ++) {
				if (BD->Medios[Total]->TipoMedio == Tipo_Medio_Audio) {
					if (nNodo->Grupo == static_cast<TMedioAudio *>(BD->Medios[Total])->Grupo) {
						Divisor ++;
						Nota += BD->Medios[Total]->Nota;
					}
				}
			}
			Nota = Nota / static_cast<float>(Divisor);
			nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16, static_cast<float>(Nota));
			break;
		case IDI_DISCO :
			nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Disco->Nombre->Nombre());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__CANCIONES, TEXT(':'), nNodo->Disco->Canciones);
			Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(nNodo->Disco->Longitud / 1000000.0f));
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStrL(nNodo->Disco->Tiempo));
			for (Total = 0; Total < BD->Medios.size(); Total ++) {
				if (BD->Medios[Total]->TipoMedio == Tipo_Medio_Audio) {
					if (nNodo->Disco == static_cast<TMedioAudio *>(BD->Medios[Total])->Disco) {
						Divisor ++;
						Nota += BD->Medios[Total]->Nota;
					}
				}
			}
			Nota = Nota / static_cast<float>(Divisor);
			nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16, static_cast<float>(Nota));
			break;
		case IDI_M3U :
			nToolTip->AgregarIconoTexto(nNodo->Icono(), 16, 16, nNodo->Medio->Nombre());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__CANCIONES, TEXT(':'), nNodo->TotalHijos());
			Tmp.sprintf(TEXT("%.02f MB"), static_cast<float>(nNodo->Medio->Longitud / 1000000.0f));
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TAMAÑO, TEXT(':'), Tmp());
			nToolTip->AgregarTextoCaracterTexto(IDIOMA__TIEMPO, TEXT(':'), Sistema.App.Media.TiempoStrL(nNodo->Medio->Tiempo));
			nToolTip->AgregarTextoCaracterIcono(IDIOMA__NOTA, TEXT(':'), IDI_ESTRELLA2, 16, 16,static_cast<float>(nNodo->Medio->Nota));
			break;
	}

	nToolTip->MostrarToolTipEx();
}

void ArbolBaseDatos::ArbolEx_Evento_Pintar_Fondo(HDC hDC, RECT *Espacio) {
	HBRUSH Brocha = CreateSolidBrush(DWL::SO::DWLEstilos::Colores.ContenedorEx_Fondo_Normal);
	FillRect(hDC, Espacio, Brocha);
	DWL::SO::DWLListaIconos::PintarIcono(hDC, Espacio->right -158, Espacio->bottom -138, 128, 128, IDI_AUDIO);
	DeleteObject(Brocha);
}


LRESULT ArbolBaseDatos::ArbolEx_Evento_Mouse_DobleClick(const UINT Boton, const int cX, const int cY) {
	NodoArbolBaseDatos *Tmp = reinterpret_cast<NodoArbolBaseDatos *>(_ArbolEx_NodoMarcado);
	if (Tmp != NULL) {
		if (Tmp->Medio != NULL) {
			if (Tmp->Medio->TipoMedio == Tipo_Medio_Audio || Tmp->Medio->TipoMedio == Tipo_Medio_Video || Tmp->Medio->TipoMedio == Tipo_Medio_CDAudio)  {
				// Agrego la entrada a la lista por si no existe
				Sistema.App.ListaC.AgregarMedio(Tmp->Medio);
				Sistema.App.ListaC.ActualizarTodo();
				// Abro la entrada
				Sistema.App.ListaC.AbrirMedio(Tmp->Medio);
			}
		}
	}
	return 0;
}