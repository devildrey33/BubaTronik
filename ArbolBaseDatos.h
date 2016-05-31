#pragma once
#include "BaseDatos.h"
#include <DWLArbolEx.h>

class ArbolBaseDatos : public DWL::ControlesEx::DWLArbolEx {
public:
									ArbolBaseDatos(void);
					               ~ArbolBaseDatos(void);
 HWND								Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const int nID);
 NodoArbolBaseDatos				   *AgregarNodoMedio(NodoArbolBaseDatos *Parent, const TCHAR *cTexto, const int cIko, TTipo *Tipo, TGenero *Genero, TGrupo *Grupo, TDisco *Disco, TMedio *Cancion, const bool AgregarAlFinal);
 NodoArbolBaseDatos				   *AgregarNodoMedio(NodoArbolBaseDatos *Parent, const int nIcono, const TCHAR *nTexto, TMedio *Video = NULL);
 NodoArbolBaseDatos				   *AgregarNodoMedioM3u(NodoArbolBaseDatos *Parent, const int nIcono, const TCHAR *nTexto, TMedio *Video = NULL);
// inline NodoArbolBaseDatos    *NodoBD(const int Pos) { return static_cast<NodoArbolBaseDatos *>(_NodosLineales[Pos]); };
 inline NodoArbolBaseDatos		   *NodoMedio(const int Pos)		{	return reinterpret_cast<NodoArbolBaseDatos *>(_ArbolEx_Nodos[Pos]);			};
 inline NodoArbolBaseDatos	       *NodoLinealMedio(const int Pos)	{	return reinterpret_cast<NodoArbolBaseDatos *>(_ArbolEx_NodosLineales[Pos]);	};
 inline NodoArbolBaseDatos         *NodoMarcadoMedio(void)			{	return reinterpret_cast<NodoArbolBaseDatos *>(_ArbolEx_NodoMarcado);			};
 void								MostrarToolTipNodo(NodoArbolBaseDatos *ttNodo);
 void								MostrarToolTipNodoCancion(NodoArbolBaseDatos * ttNodo);
 void								MostrarToolTipNodoVideo(NodoArbolBaseDatos * ttNodo);
 void								MostrarToolTipNodoPistaCD(NodoArbolBaseDatos *ttNodo);
 void								ArbolEx_Evento_Pintar_Fondo(HDC hDC, RECT *Espacio);
 LRESULT                            ArbolEx_Evento_Mouse_DobleClick(const UINT Boton, const int cX, const int cY);
 NodoArbolBaseDatos                *BuscarNodoR(const TCHAR *Txt, NodoArbolBaseDatos *Padre = NULL) {
										if (_ArbolEx_Nodos.size() == 0) return NULL;
										NodoArbolBaseDatos *Ret; 
										NodoArbolBaseDatos *Tmp = NULL;
										if (Padre != NULL)	Ret = reinterpret_cast<NodoArbolBaseDatos *>(Padre->Hijo(0));
										else				Ret = reinterpret_cast<NodoArbolBaseDatos *>(_ArbolEx_Nodos[0]);
										while (Ret != NULL) {
											if (DWLStrCmpi(Ret->Texto(), Txt) == 0) {
												if (Ret->Padre() == Padre)	return Ret;
											}
											if (Ret->TotalHijos() > 0) {
												Tmp = BuscarNodoR(Txt, Ret);
												if (Tmp != NULL) return Tmp;
											}
											Ret = reinterpret_cast<NodoArbolBaseDatos *>(Ret->Siguiente());
										}
										return NULL;
									};
private:
// void								ArbolEx_Evento_MostrarToolTip(DWL::ControlesEx::DWLArbolEx_Nodo *ttNodo);
// LRESULT CALLBACK					GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
