#pragma once

#include "Escena.h"


#include <DWLVentana.h>
#include "PlayerMenuEx.h"

#include "ThreadBuscarArchivos.h"

// La idea es tenir una clase que contingui les escenes (amb texte / links / imgs)
// Aquesta clase s'encarregaria de actualitzar les mides dels textes i controls. 
// Apartir de aqui la ControlPresentacionEx, tindria varies presentacions prefabricades i aniria posan la que convingues.

enum EstadoCP {
	EstadoCP_Inicio					= 0,
	EstadoCP_BuscarArchivos			= 1,
	EstadoCP_Ayuda				    = 2,
	EstadoCP_Opciones				= 9,
	EstadoCP_RevisarBD				= 10
};



class ControlPresentacionEx;




/*
class ObjetoEscena2 {
  public:
							ObjetoEscena2(void);
							ObjetoEscena2(TipoObjetoEscena TipoObjeto, LPVOID NuevoObjeto, const UINT nAlto, const UINT nAlineacion);
						   ~ObjetoEscena2(void);
	UINT					Alto;
	TipoObjetoEscena        Tipo;
	UINT                    Alineacion;
	void                    Pintar(HDC hDC, const int Y);
	void                    Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool nRepintar = false);
  private:
	LPVOID                 _Objeto;
};




class Escena2 {
};*/



class ControlPresentacionEx :	public DWL::ControlesEx::DWLBarraScrollEx_Nucleo,
                                public DWL::Ventanas::DWLVentana {
public:
								ControlPresentacionEx(void);
				               ~ControlPresentacionEx(void);
 void							Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlton, const UINT nID);
 LRESULT					    Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 inline LRESULT					Evento_BorrarFondo(HDC hDC) { return 0; };
 LRESULT						Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_Rueda(const short Delta, const short cX, const short cY, const UINT VirtKey);
 LRESULT						Evento_Mouse_Saliendo(void);
 void							Evento_ObjetoPulsado(const UINT Boton, const UINT IDP);
 void	                        Evento_MostrarToolTip(const UINT IDToolTip);
// LRESULT                        Evento_BotonEx_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
 LRESULT						Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
 LRESULT						Evento_MarcaEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT cID);
// LRESULT						Evento_ArbolEx_Mouse_Click(const UINT Boton, const int cX, const int cY, DWL::ControlesEx::DWLArbolEx_Nodo *nNodo, const UINT IDArbolEx);
// LRESULT						Evento_ArbolEx_Mouse_DobleClick(const UINT Boton, const int cX, const int cY, DWL::ControlesEx::DWLArbolEx_Nodo *nNodo, const UINT IDArbolEx);
// LRESULT						Evento_ArbolEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDArbolEx);
// LRESULT						Evento_ArbolEx_LabelEdit_Fin(DWL::ControlesEx::DWLArbolEx_TerminarLabelEdit *Edicion, const UINT IDArbolEx);
// LRESULT						Evento_MenuEx_Menu_Pulsado(DWL::ControlesEx::DWLMenuEx *MenuPulsado, const UINT IDMenuEx);
 LRESULT                        Evento_BarraScrollEx_BotonEx_Click(const UINT IDBotonEx, const UINT nBoton);
 LRESULT						Evento_EdicionDesplegableEx_CambioSeleccion(const TCHAR *NuevoTexto, const UINT IDEdicionTexto);
 LRESULT                        Evento_MostrarToolTip(const UINT IDControl, LPARAM Param);
 LRESULT                        Evento_OcultarToolTip(const UINT IDControl, LPARAM Param);

 void							Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool nRepintar = true);
 void							Mostrar_Ayuda(void);
 void							Mostrar_CargandoVLC(void);
 void							Mostrar_BuscarArchivos(void);
 void							Mostrar_RevisarBD(void);
 void							Mostrar_Opciones(void);
 void							Mostrar_Opciones_Apariencia(void);
 void							Mostrar_Opciones_Teclado(void);
 void							Mostrar_Opciones_Lista(void);
 void							Mostrar_Opciones_BD(void);
 void							Mostrar_Opciones_Video(void);
 void							Mostrar_Ecualizador(void);
 void                           Mostrar_SobreBubaTronik(void);
// void							Mostrar_EnumerandoArchivos(void);
// void							Mostrar_ObteniendoDatos(size_t TotalArchivos);
// void							Mostrar_ConstruirJerarquia(void);
// void							Mostrar_CoincidenciasGenero(void);
// void							Mostrar_CoincidenciasGrupo(void);
// void							Mostrar_CoincidenciasDisco(void);
 void							Mostrar_Inicio(void);

 void                           IniciarBusquedaArchivos(void);
 void                           TerminarBusquedaArchivos(void);
 void                           CancelarBusquedaArchivos(void);
// inline const bool              Buscando(void) { if (BuscarArchivos != NULL) { return true; } return false; };
// void							Espacio_Oscy(const bool Espacio = true);
 void							Repintar(const bool Forzar = false);
 Escena						   *ObtenerEscenaActual(void) { return EscenaActual; };
 EstadoCP						Estado;
  ThreadBuscarArchivos         *BuscarArchivos;
private:
 LRESULT						Evento_Temporizador(const UINT IDTimer);
 void							EnumerarIdiomas(DWL::ControlesEx::DWLEdicionDesplegableEx *Combo);
 void							EnumerarSkins(DWL::ControlesEx::DWLEdicionDesplegableEx *nCombo);
// void							VerPreferencias(const bool Ver);
 void							Evento_Scroll_CambioPos(void);
 const int						PintarTexto(HDC hDCDest, const TCHAR *Txt, const int cY);
 LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
 Escena				           *EscenaActual;
 PlayerMenuEx					FC_Menu;

 int							Buscar_TotalArchivos;
// int							Buscar_NuevasCanciones;

 void							Mostrar_NODISPONIBLE(void);

/* DWL::ControlesEx::DWLListaEx_Fuentes FuentesPeques;
 DWL::ControlesEx::DWLListaEx_Fuentes FuentesMedianas;
 DWL::ControlesEx::DWLListaEx_Fuentes FuentesGrandes;*/
 friend class					ObjetoFrame;
};
