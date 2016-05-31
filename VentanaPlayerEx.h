#pragma once
#include "ventanaex.h"
#include "ArbolOpcionesBasicas.h"
#include "ControlPresentacionEx.h"
#include "Botonera.h"
#include "ControlVolumen.h"
#include "ShufleRepeat.h"
#include "ControlOscyloscope.h"
#include "Definiciones.h"
#include "IDS_Definidas.h"
#include "ControlCerrarMinimizar.h"
#include "PlayerMenuEx.h"
#include "ControlTiempoActual.h"
#include <DWLControlEx.h>
#include "VentanaMensaje_Actualizacion.h"

//#define ID_BOTON_CANCION_ATRAS		33005
//#define ID_BOTON_CANCION_PLAY		33006
//#define ID_BOTON_CANCION_PAUSA		33007
//#define ID_BOTON_CANCION_STOP		33008
//#define ID_BOTON_CANCION_ADELANTE	33009



// Tamaños fijos

/*
enum PantallasPlayer {
	Pantalla_Inicio = 0,
	Pantalla_BD     = 1,
	Pantalla_Buscar = 2,
	Pantalla_Lista  = 3
};*/

class ItemListaMedios;

class VentanaPlayerEx :	public VentanaEx {
public:
								VentanaPlayerEx(void);
			                   ~VentanaPlayerEx(void);
 void							Crear(void);
 ArbolOpcionesBasicas			OpcionesBasicas;
 ControlPresentacionEx			CP;
 ControlVolumen					Volumen;
 void							CrearMenus(void);
 void							ActualizarMenusGenero(void);
 void							MostrarMenuBDA(void);
 void							AbrirArchivo(void);
 ItemListaMedios			   *AgregarArchivoLista(const TCHAR *Path);
 Botonera						Botones;
 ShufleRepeat					BotonShufleRepeat;
 ControlTiempoActual			TiempoStrActual;
 void							SoltarArchivos(WPARAM wParam);
 void							AgregarAListaA(void);
 void							ParametrosInstancia(WPARAM wParam, LPARAM lParam);
 void                           ParsearM3U(const TCHAR *Path, const bool AgregarM3UaBD);

 PlayerMenuEx				   *MenuShufle;
 PlayerMenuEx				   *MenuRepeat;
 PlayerMenuEx				   *MenuVideo;
 void							MostrarMenuLista(void);
 ControlOscyloscope				Oscy;
 ControlCerrarMinimizar			CM;
 int							Ancho_Opciones_Basicas;
 LRESULT						Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto);
// void							Evento_CambiandoTam(WPARAM wParam, RECT *Rectangulo);
 void							Evento_ValidarControles(void);
 VentanaMensaje_Actualizacion   VentanaActualizacion;
 void                           Titulo(const TCHAR *nTxt);
 LRESULT                        Evento_VentanaMensaje(const UINT IDMensaje, const enum RespuestaVentanaMensaje &Respuesta);
 bool                           EditandoNombre;
private :
 LRESULT						Evento_BotonBarraTareas_Creado(void);
 void                           AbrirDirectorioMedio(TMedio *nMedio);
// void			                Evento_PintarEx(HDC hDC);
 void							BotonRepeat(const int cID);
 void							PintarControl(DWL::ControlesEx::Base::DWLControlEx *Control, DWL::GDI::DWLhDC *hDC);
 void							SoltarArchivos_AgregarArchivo(const TCHAR *Archivo);
 void							SoltarArchivos_BuscarArchivos(const TCHAR *Archivo);
 LRESULT						Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Temporizador(const UINT cID);
 void							Evento_TemporizadorLista(void);
 LRESULT                        Evento_Comando(const int cID, const UINT CodigoNotificacion, HWND hWndControl);
 LRESULT						Evento_ComandoDelSistema(const UINT nID, const int cX, const int cY);
 LRESULT                        Evento_Mouse_Rueda(const short Delta, const short cX, const short cY, const UINT VirtKey);

 LRESULT                        Evento_BarraEx_Mouse_BotonSoltado(const UINT cID, const UINT nBoton);
 LRESULT						Evento_ArbolEx_Mouse_Click(const UINT Boton, const int cX, const int cY, DWL::ControlesEx::DWLArbolEx_Nodo *nNodo, const UINT IDArbolEx);
// LRESULT						Evento_ArbolEx_Mouse_DobleClick(const UINT Boton, const int cX, const int cY, DWL::ControlesEx::DWLArbolEx_Nodo *nNodo, const UINT IDArbolEx);
 LRESULT						Evento_ArbolEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDArbolEx);
 LRESULT						Evento_ArbolEx_LabelEdit_Inicio(DWL::ControlesEx::DWLArbolEx_Nodo *eNodo, const UINT IDArbolEx);
 LRESULT						Evento_ArbolEx_LabelEdit_Fin(DWL::ControlesEx::DWLArbolEx_TerminarLabelEdit *Edicion, const UINT IDArbolEx);
// LRESULT						Evento_ListaEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDListaEx);
// LRESULT						Evento_ListaEx_Mouse_DobleClick(const UINT Boton, const int cX, const int cY, const UINT IDListaEx);
 LRESULT						Evento_ListaEx_LabelEdit_Inicio(DWL::ControlesEx::DWLListaEx_ParametrosLabelEdit *Edicion, const UINT IDListaEx);
 LRESULT                        Evento_ListaEx_LabelEdit_Fin(DWL::ControlesEx::DWLListaEx_ParametrosLabelEdit *Edicion, const UINT IDListaEx);
 LRESULT						Evento_MenuEx_Menu_Pulsado(DWL::ControlesEx::DWLMenuEx *MenuPulsado, const UINT IDMenuExPadre);
 LRESULT						Evento_BotonEx_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT wParam);

 LRESULT						Evento_MostrarToolTip(const UINT IDControl, LPARAM Param);
 LRESULT						Evento_OcultarToolTip(const UINT IDControl, LPARAM Param);

 LRESULT						Evento_Cerrar(void);
 void							BDAEliminar();
 void							BDAEliminarConfirmado(const bool BorrarArchivosDisco);
 LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);

 PlayerMenuEx					MenusPlayer;
 PlayerMenuEx					MenusOcultos;
// Accesos a menus
 PlayerMenuEx				   *MenuBotonBD;
 PlayerMenuEx				   *MenuBotonLista;
 PlayerMenuEx				   *MenuBDA;
 PlayerMenuEx				   *MenuLista;
 PlayerMenuEx				   *MenuOscyTxt;
 bool                           NoValidarControles;
};
