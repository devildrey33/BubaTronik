#pragma once

#include <DWLControlEx.h>

// El display sera de 320 pixeles de ancho por 60 de alto
// el fondo restante no se pintara con esta funcion
#define ANCHO_OSCY_PINTABLE 160
#define ALTO_OSCY_PINTABLE	 29
#define NUM_BARRITAS		 80

#define MULTIPLICADOR_OSCY	320								// Multiplicador del valor inicial del oscyloscope
#define Y_ORIGEN			 5								// Y desde donde empiezan las barritas a pintarse
#define MAXMEDIDA			 ALTO_OSCY_PINTABLE - Y_ORIGEN	// Tamaño maximo de la barrita
#define ANCHO_TIEMPO		 30								// Ancho en pixeles del texto del tiempo


class ControlOscyloscope : public DWL::ControlesEx::Base::DWLControlEx {
 public:
							ControlOscyloscope(void);
					       ~ControlOscyloscope(void);
	void					Crear(HWND hWndPadre, const int cX, const int cY, const int cAncho, const int cAlto, const UINT nID);
	LRESULT					Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
	LRESULT					Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
	LRESULT					Evento_Mouse_BotonPresionado(const UINT Boton, const int cX, const int cY, const UINT Param);
	LRESULT					Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
	LRESULT					Evento_Mouse_Saliendo(void);
	LRESULT					Evento_BorrarFondo(HDC hDC);
	void					PintarOscy(HDC hDC, RECT *Espacio);
	void                    PintarFondoVacio(void);
	void					IniciarThread(void);
//	void					TerminarThread(void);
	void					Terminar(void);
	HANDLE					Mutex;

	HANDLE					hThread;
 private:
    short					LineasOscy[NUM_BARRITAS + 1];
//	LRESULT CALLBACK		GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int						EstadoTexto;

	static unsigned long	ThreadOscy(void *pThis);
	bool                   _Terminar;
};
