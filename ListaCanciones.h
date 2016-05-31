#pragma once

#include <DWLListaEx.h>
#include <DWLBotonEx.h>
#include "BaseDatos.h"

class ListaCanciones : public DWL::ControlesEx::DWLListaEx {
public:
									ListaCanciones(void);
			                       ~ListaCanciones(void);
 HWND								Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const int cID);
 ItemListaMedios				   *AgregarMedio(TMedio *nMedio);
// void								AgregarVideo(TVideo *Video);
 void								EliminarTodosLosMedios(void);
 void								EliminarMediosSeleccionados(void);
 void								EliminarMedio(ItemListaMedios *Itm);
 void								AbrirMedioMarcado(void);
 void                               AbrirMedio(TMedio *nMedio);
 inline ItemListaMedios			   *ItemMedio(const UINT Pos) {
										return static_cast<ItemListaMedios *>(_ListaEx_Items[Pos]);
									};
 inline ItemListaMedios			   *ItemMedioMarcado(void) {
										return static_cast<ItemListaMedios *>(_ListaEx_ItemMarcado);
									};
 inline ItemListaMedios			   *ItemMedioResaltado(void) {
										return static_cast<ItemListaMedios *>(_ListaEx_ItemResaltado);
									};
 void								Shufle(const bool sShufle);
 void								MostrarGuardarLista(void);
 void								GuardarLista(void);
 std::vector<ItemListaMedios *>		Orig;
 void								Mover(const int cX, const int cY, const int cAncho, const int cAlto, const bool cRepintar);
// LRESULT							Evento_Temporizador(const UINT cID);
 LRESULT							Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT wParam);
 LRESULT							Evento_Mouse_Rueda(const short Delta, const short cX, const short cY, const UINT VirtKey);
 LRESULT							Evento_Mouse_Movimiento(const int cX, const int cY, const UINT wParam);
 LRESULT							Evento_Teclado_TeclaSoltada(const UINT Tecla, const UINT Repeticion, const UINT Params);
// void								Evento_Pintar_ZonaMuerta(HDC hDC);
 void								Visible(const bool Vis);
 const BOOL							Visible(void) { return DWL::ControlesEx::Base::DWLControlEx::Visible(); };
// void								ListaEx_Evento_MostrarToolTip(DWL::ControlesEx::DWLListaEx_Item * ttItem);
 void								ListaEx_Evento_Pintar_Fondo(HDC hDC, RECT *Espacio);
 void								ListaEx_Evento_Mouse_DobleClick(const UINT Boton, const int cX, const int cY);
 LRESULT							Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto);
 void								MoverSeleccionAbajo(void);
 void								MoverSeleccionArriba(void);
 void                               ActualizarTodo(const bool nRepintar = false);
// DWL::ControlesEx::DWLBotonEx		Boton_Up;
// DWL::ControlesEx::DWLBotonEx		Boton_Down;
// DWL::ControlesEx::DWLBotonEx		Boton_Eliminar;
 DWL::DWLString						NombreLista;
 inline UINT                        Ver_DigitosPista(void) { return _Ver_DigitosPista; };
private:
 void								BarraScrollEx_Evento_BotonExtendido_Click(DWL::ControlesEx::DWLBarraScrollEx_Boton *BotonExtendido);
// void								BotonEx_Evento_Mouse_BotonSoltado(const int Boton, const int cX, const int cY, const int IDBotonEx, WPARAM wParam);
// void								Evento_CambioTam(WPARAM wParam, RECT *Rectangulo);
 void								ReestructurarNodos(void);

 bool                              _Ver_HorasMinutosSegundos;
 UINT                              _Ver_DigitosPista;

 DWL::DWLString                    _PathM3u;
// LRESULT CALLBACK					GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
