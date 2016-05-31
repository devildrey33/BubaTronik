#pragma once
#include "DWLBotonEx.h"
#include "ControlMarca.h"
#include "DWLVentana.h"
#include "ControlCerrarMinimizar.h"
#include "ArchivoBinarioPlayer.h"

#define ANCHO_BOTON_VM		80
#define ID_BOTON_ACEPTAR   100
#define ID_BOTON_CANCELAR  101
#define ID_MARCA_NOMOSTRAR 102
#define ID_BOTON_CERRAR    103
/*
enum TipoVentanaMensaje {
	Tipo_Aceptar						= 0,
	Tipo_Aceptar_NoMostrarMas			= 1,
	Tipo_AceptarCancelar				= 2,
	Tipo_AceptarCancelar_NoMostrarMas	= 3
};



class VentanaMensaje : public DWL::Ventanas::DWLVentana {
public:
								VentanaMensaje(void);
							   ~VentanaMensaje(void);
 const RespuestaVentanaMensaje	MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const TipoVentanaMensaje nTipo = Tipo_Aceptar);
								// Funcion que calcula el tamaño del texto necesario para mostrar el mensaje
 const SIZE						CalcularTamTexto(const TCHAR *Texto);
 LRESULT						Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 LRESULT						Evento_Cerrar(void);
 LRESULT						Evento_Foco_Obtenido(HWND hWndAnterior);
 LRESULT						Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT						Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
 LRESULT						Evento_BotonEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDBotonEx);
 LRESULT						Evento_MarcaEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT cID);
private:
 RespuestaVentanaMensaje		Respuesta;
 DWL::DWLString					Mensaje;
 DWL::DWLString					Titulo;
 SIZE							TamMensaje;
// LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
 DWL::ControlesEx::DWLBotonEx	Boton_Aceptar;
 DWL::ControlesEx::DWLBotonEx	Boton_Cancelar;
 ControlMarcaMsg				Marca_NoMostrarMas;
 ControlCerrarMinimizar			Boton_Cerrar;
 TipoVentanaMensaje				Tipo;
 bool							MensajeVisible;
};*/

// La idea es tener un conjunto de clases que sirva para mostrar mensajes de diferentes tipos :
// - Mensaje -> Aceptar :
//    Esta ventana no requiere ninguna respuesta.
// - Mensaje -> Aceptar no mostrar mas.
//    Esta ventana no requiere ninguna respuesta. Ademas guardara en una variable si debe o no mostrarse mas.
// - Mensaje -> Aceptar - Cancelar.
//    Esta ventana mandara un mensaje a la ventana padre con el resultado y la id del mensaje.
// - Mensaje -> Aceptar - Cancelar.
//    Esta ventana mandara un mensaje a la ventana padre con el resultado y la id del mensaje. Ademas guardara en una variable si debe o no mostrarse mas.


// Clase base para las ventanas con mensajes y opciones externas
class VentanaMensaje_Base : public DWL::Ventanas::DWLVentana {
 public:
								VentanaMensaje_Base(void);
							   ~VentanaMensaje_Base(void);
 protected :
//  virtual LRESULT				Evento_BotonEx_Teclado_TeclaPresionada(const UINT TeclaVirtual, const UINT IDBotonEx);
  virtual LRESULT				Evento_BotonEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDBotonEx);
  LRESULT						Evento_Foco_Obtenido(HWND hWndAnterior);
  LRESULT	         			Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);

  SIZE                          CrearVentanaMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje, const UINT AlturaExtra = 0);
  const SIZE					CalcularTamTexto(const TCHAR *Texto);
  void                          PintarDegradadoFondo(HDC hDC, RECT *Espacio);
  void                          PintarTextoSombra(HDC hDC, const TCHAR *Texto, const UINT TamTexto, COLORREF ColTexto, COLORREF ColSombra, const RECT &Espacio);
  
//  SIZE                          TamMensaje;
  DWL::DWLString				Mensaje;
  DWL::DWLString				Titulo;
  UINT                          IDMensaje;
  ControlCerrarMinimizar		Boton_Cerrar;
  DWL::ControlesEx::DWLBotonEx	Boton_Cancelar;
  bool                         _TeclaPresionada;
};


// Clase para los mensajes de error criticos (la función mostrar mensaje no terminara hasta que la ventana sea cerrada por el usuario)
class VentanaMensaje_ErrorCritico : public VentanaMensaje_Base {
 public:
								VentanaMensaje_ErrorCritico(void);
                                VentanaMensaje_ErrorCritico(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje);
							   ~VentanaMensaje_ErrorCritico(void);
  LRESULT						Evento_BotonEx_Teclado_TeclaSoltada(const UINT TeclaVirtual, const UINT IDBotonEx);
  void                          MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje);
  LRESULT                       Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
  LRESULT						Evento_Cerrar(void);
  bool                          Terminado;
};


// Clase para los mensajes que solo aceptan
class VentanaMensaje_Aceptar : public VentanaMensaje_Base {
 public:
								VentanaMensaje_Aceptar(void);
                                VentanaMensaje_Aceptar(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje);
							   ~VentanaMensaje_Aceptar(void);
  void                          MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje = 0);
  LRESULT                       Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
  LRESULT				        Evento_Cerrar(void);

};

// Clase para los mensajes que tienen la opcion Aceptar / Cancelar
class VentanaMensaje_AceptarCancelar : public VentanaMensaje_Base {
 public:
                                VentanaMensaje_AceptarCancelar(void);
                                VentanaMensaje_AceptarCancelar(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje, const TCHAR *AceptarTxt = NULL, const TCHAR *CancelarTxt = NULL);
                               ~VentanaMensaje_AceptarCancelar(void);
  void                          MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje, const TCHAR *AceptarTxt = NULL, const TCHAR *CancelarTxt = NULL);
  LRESULT						Evento_Cerrar(void);
  LRESULT                       Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
  DWL::ControlesEx::DWLBotonEx	Boton_Aceptar;
//  ControlMarcaMsg				Marca_NoMostrarMas;
};

// Clase para los mensajes que tienen la opcion Aceptar / Cancelar, con una marca de No mostrar mas.
class VentanaMensaje_AceptarCancelar_NoMostrarMas : public VentanaMensaje_Base {
 public:
                                VentanaMensaje_AceptarCancelar_NoMostrarMas(void);
                                VentanaMensaje_AceptarCancelar_NoMostrarMas(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje);
                               ~VentanaMensaje_AceptarCancelar_NoMostrarMas(void);
  void                          MostrarMensaje(const TCHAR *nTexto, const TCHAR *nTitulo, const UINT nIDMensaje);
  LRESULT						Evento_Cerrar(void);
  LRESULT						Evento_MarcaEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT cID);
  LRESULT                       Evento_BotonEx_Mouse_Click(const UINT Boton, const int cX, const int cY, const UINT IDBotonEx, const UINT Param);
  DWL::ControlesEx::DWLBotonEx	Boton_Aceptar;
  ControlMarcaMsg				Marca_NoMostrarMas;
};

