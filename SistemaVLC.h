#pragma once

#include "BaseDatos.h"
#include "..\vlc-1.1.5\include\vlc\vlc.h"
/*#include "..\vlc-1.1.4\include\vlc\libvlc_structures.h"
#include "..\vlc-1.1.4\include\vlc\libvlc.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_media.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_media_player.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_media_list.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_media_list_player.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_media_library.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_media_discoverer.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_events.h"
#include "..\vlc-1.1.4\include\vlc\libvlc_vlm.h"
#include "..\vlc-1.1.4\include\vlc\deprecated.h"*/
//#include "ConfigBubaTronik.h"
/*#if (LIBVLC_NUM_VERSION == 120)
    #include "LibVlc_Base_120.h"
#else 
    #include "libvlc_base.h"
#endif*/



// Clase para crear un array de chars con los argumentos que requiera el VLC, se ha diseñado de forma que resulte facil agregar nuevos argumentos
class ArgumentosVLC {
public :
								ArgumentosVLC(void) { 
									Argumentos = NULL; 
								};

							   ~ArgumentosVLC(void) { 
								    if (Argumentos != NULL) delete Argumentos;
									for (size_t i = 0; i < Args.size(); i++) {
										delete Args[i];
									}
							    };

								// Lo ideal es pasar un char a esta funcion ya que al final el vlc mira los parametros como char
								// De todas formas se han implementado funciones para pasar los argumentos directamente con wchar_t y DWL::DWLString
 void							AgregarArgumento(const char *Argumento) {
									char *Str = new char[512];
									strcpy_s(Str, 512, Argumento);
									Args.push_back(Str);
								};

 void							AgregarArgumento(const wchar_t *Argumento) {
									char *Str = new char[512];
									int	TamRes = WideCharToMultiByte(CP_UTF8, NULL, Argumento, wcslen(Argumento), Str, 512, NULL, NULL);
									Str[TamRes] = 0; // Termino el string, ya que en algunas compilaciones la funcion WideCharToMultiByte no pone el terminador.
									Args.push_back(Str);
								};

 /*void							AgregarArgumento(DWL::DWLString &Argumento) {
									char *Str = new char[512];
									int	TamRes = WideCharToMultiByte(CP_UTF8, NULL, Argumento(), Argumento.Tam(), Str, 512, NULL, NULL);
									Str[TamRes] = 0; // Termino el string, ya que en algunas compilaciones la funcion WideCharToMultiByte no pone el terminador.
									Args.push_back(Str);
								};*/

 const char					  **ObtenerArgumentos(void) {
									if (Argumentos != NULL) delete Argumentos;
									Argumentos = new const char * [Args.size()];
									for (size_t i = 0; i < Args.size(); i++) {
										Argumentos[i] = Args[i];
									}
									return Argumentos;
								};

 const char					  **operator () (void) {
									return ObtenerArgumentos();
								};

 const int						TotalArgumentos(void) {
									return static_cast<int>(Args.size());
								}
private :
 const char					  **Argumentos;
 std::vector<char *>			Args;
};


class SistemaVLC //: public VLC_BASE {
{
public:
										SistemaVLC(void);
								       ~SistemaVLC(void);
 const bool								IniciarInstanciaVLC(void);
 void									TerminarInstancia(void);

 const bool								AbrirMedio(const TCHAR *Path);

 const int								Asignar_Subtitulos_MedioActual(const TCHAR *Path);

// const bool								ExaminarStreamVideo(UINT &nAncho, UINT &nAlto, UINT64 &Tiempo);
 const BOOL                             ComprobarError(void);
 const bool								Play(void);
 const bool								Pausa(void);
 const bool								Stop(void);
 Estados_Medio							ComprobarEstado(void);
 const float							TiempoActual(void);
 void									TiempoActual(float nTiempo);
 const UINT64							TiempoTotal(void);
 const UINT64							TiempoActualMs(void);
 const int								Volumen(void);
 void									Volumen(int nVolumen);
 void									RateAdelanteX4(void);
 void									RateAtrasX4(void);
 void									RateNormal(void);

 void									EliminarMedioActual(void);
 libvlc_instance_t					   *InstanciaVLC(void) { return _Instancia; };

 inline libvlc_log_t				   *Log(void) { return _Log; };
 /////////////////////////////////////////
// const bool								MostrarError(void);
private :
 libvlc_instance_t					  *_Instancia;
 libvlc_media_player_t				  *_MediaPlayer;
 libvlc_log_t						  *_Log;
 bool								   _RateModificado;
 int                                   _Comprobacion;
};

/*
// Clase para crear un array de chars con los argumentos que requiera el VLC, se ha diseñado de forma que resulte facil agregar nuevos argumentos
class ArgumentosVLC {
public :
								ArgumentosVLC(void) { 
									Argumentos = NULL; 
								};

							   ~ArgumentosVLC(void) { 
								    if (Argumentos != NULL) delete Argumentos;
									for (size_t i = 0; i < Args.size(); i++) {
										delete Args[i];
									}
							    };

								// Lo ideal es pasar un char a esta funcion ya que al final el vlc mira los parametros como char
								// De todas formas se han implementado funciones para pasar los argumentos directamente con wchar_t y DWL::DWLString
 void							AgregarArgumento(const char *Argumento) {
									char *Str = new char[512];
									strcpy_s(Str, 512, Argumento);
									Args.push_back(Str);
								};

 void							AgregarArgumento(const wchar_t *Argumento) {
									char *Str = new char[512];
									int	TamRes = WideCharToMultiByte(CP_UTF8, NULL, Argumento, wcslen(Argumento), Str, 512, NULL, NULL);
									Str[TamRes] = 0; // Termino el string, ya que en algunas compilaciones la funcion WideCharToMultiByte no pone el terminador.
									Args.push_back(Str);
								};

 void							AgregarArgumento(DWL::DWLString &Argumento) {
									char *Str = new char[512];
									int	TamRes = WideCharToMultiByte(CP_UTF8, NULL, Argumento(), Argumento.Tam(), Str, 512, NULL, NULL);
									Str[TamRes] = 0; // Termino el string, ya que en algunas compilaciones la funcion WideCharToMultiByte no pone el terminador.
									Args.push_back(Str);
								};

 const char					  **ObtenerArgumentos(void) {
									if (Argumentos != NULL) delete Argumentos;
									//const char *Argumentos = new char [4];
									Argumentos = new const char * [Args.size()];
									for (size_t i = 0; i < Args.size(); i++) {
										Argumentos[i] = Args[i];
									}
									return Argumentos;
								};

 const char					  **operator () (void) {
									return ObtenerArgumentos();
								};

 const int						TotalArgumentos(void) {
									return static_cast<int>(Args.size());
								}
private :
 const char					  **Argumentos;
 std::vector<char *>			Args;
};*/