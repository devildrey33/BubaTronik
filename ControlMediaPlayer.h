#pragma once

/*#pragma comment(lib, "dinput8.lib")	// Agrego la libreria del direct input al proyecto
#pragma comment(lib, "dxguid.lib")	// Agrego la libreria del direct input al proyecto
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>*/
#include <DWLControlEx.h>
//#include <DWLDirectInput.h>


class ControlMediaPlayer : public DWL::ControlesEx::Base::DWLControlEx {
public:
								ControlMediaPlayer(void);
							   ~ControlMediaPlayer(void);
 void							Crear(HWND hWndParent, const int cX, const int cY, const int cAncho, const int cAlto, const UINT cID);
 LRESULT						Evento_Pintar(HDC hDC, PAINTSTRUCT &PS);
 LRESULT						Evento_Mouse_BotonSoltado(const UINT Boton, const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Mouse_Movimiento(const int cX, const int cY, const UINT Param);
 LRESULT						Evento_Temporizador(const UINT nID);
// LRESULT						Evento_Teclado_TeclaSoltada(const UINT Tecla, const UINT Repeticion, const UINT Param);
 LRESULT						Evento_CambioTam(const UINT Tipo, const int nAncho, const int nAlto);
// void							Evento_Teclado_TeclaPresionada(const int VirtKey, LPARAM lParam);
// void							Evento_Teclado_Caracter(const int Tecla, LPARAM lParam);
 //void							Evento_Teclado_CaracterDelSistema(const int Tecla, LPARAM lParam);
 LRESULT						Evento_Mouse_DobleClick(const UINT Boton, const int cX, const int cY, const UINT Param);
 void							PantallaCompleta(const bool VerCompleta);
 const bool						EstaEnPantallaCompleta(void) { return _PantallaCompleta; };
 void							HackVLCWNDPROC(const bool Activar);
 void							Teclado_Global(void);
 bool							Tecla_PlayPausaPresionada;
 bool							Tecla_StopPresionada;
 bool							Tecla_SiguientePresionada;
 bool							Tecla_AnteriorPresionada;
// static HWND					hWndVLC;
// static HWND					hWndVLC_Video;
protected:
 static BOOL CALLBACK			EnumeracionVLC(HWND hWnd, LPARAM lParam);
// LRESULT CALLBACK				GestorMensajes(UINT uMsg, WPARAM wParam, LPARAM lParam);
 bool						   _PantallaCompleta;
// static ControlMediaPlayer     *This;
 static bool					WndEncontrada;
 bool							PlayerMaximizado;
 DWORD							DobleClick;
 DWORD							UltimoMovimiento;
// static WNDPROC					ProcVLC;
// static WNDPROC					ProcVLC_Video;
// static LRESULT CALLBACK		GestorMensajesVLC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// static LRESULT CALLBACK		GestorMensajesVLC_Video(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
 bool							ControlesMP_Abajo;
// DWL::DirectX::DWLDirectInput	DirectInput;
// LPDIRECTINPUT8				ObjetoDirectInput;
// LPDIRECTINPUTDEVICE8			dinkeyboard;
// static BOOL CALLBACK			EnumChildProc(HWND hwndChild, LPARAM lParam);
};
