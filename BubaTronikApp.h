//#pragma once
#ifndef BUBATRONIK_APP
#define BUBATRONIK_APP

#include "VentanaPlayerEx.h"
#include "VentanaLog.h"
#include <DWLAplicacion.h>
#include <DWLArchivoLog.h>
#include "SkinPlayer.h"
#include <DWLGraficos.h>
#include <DWLString.h>
#include "ConfiguracionPlayer.h"
#include "BaseDatos.h"
#include "Unidades_de_disco.h"
#if defined UTILIZAR_FMOD
	#include "SistemaFMOD.h"
#endif
#include "ArbolBaseDatos.h"
#include "ListaCanciones.h"
#include "ControlMediaPlayer.h"
#include "ControlesMediaPlayer.h"
#include "SistemaVLC.h"
#include "SistemaMedia.h"
#include <DWLBarraEx.h>
#include "VentanaMensaje.h"
#include "Idioma.h"
#include "BuscarActualizaciones.h"
#include "AsociarArchivos.h"
#include "TipoArchivo.h"
#include <DWLFiltroErrores.h>
#include "VentanaDump.h"

#ifdef MOSTRAR_VENTANAPRINCIPALEX
	#include "VentanaPrincipalEx.h"
#endif

#define LOG_GUARDAR_PROTOTIPO()			SOLO_DEBUG(Sistema.App.Log.GuardarPrototipoFuncion(__FUNCSIG__));
#define LOG_BUSCAR_GUARDAR_PROTOTIPO()  SOLO_DEBUG(Sistema.App.LogBuscarArchivos.GuardarPrototipoFuncion(__FUNCSIG__));


// Formas disponibles para cerrar el sistema 
enum SOCerrarSistema {
	CS_Apagar			= 0,
	CS_ReIniciar 		= 1,
	CS_CerrarUsuario	= 2
};

typedef _declspec(dllimport) const TCHAR *(TpTextoIdioma)(const int Num);
typedef _declspec(dllimport) const TCHAR *(TpTextoEstilo)(const int Num);
typedef _declspec(dllimport) const float  (TpVersion)(void);
typedef _declspec(dllimport) const TCHAR *(TpNombre)(void);
typedef _declspec(dllimport) const int 	  (TpIcono)(void);
//typedef _declspec(dllimport) const TCHAR *(TpAyuda)(void);

class BubaTronikApp : public DWL::DWLAplicacion {
public:
								        		BubaTronikApp(void);
	                                           ~BubaTronikApp(void);


  const BOOL							        Inicio(void);
  void									        Fin(void);
  void									        Eventos_Mirar(void);
  void								        	CerrarSistema(const SOCerrarSistema Forma = CS_Apagar, const bool Forzar = false);
  const bool							        CargarIdioma(const TCHAR *Path);
  void									        DescargarIdioma(void);
  const int						    		    EscanearUnidades(void);
  const int								        EscanearProcesos(void);

  DWL::ControlesEx::DWLToolTipEx				ToolTip;
  TipoArchivo									Tipos;
  VentanaMensaje_AceptarCancelar_NoMostrarMas   VentanaAsociar;
#ifdef MOSTRAR_VENTANAPRINCIPALEX
  VentanaPrincipalEx				        	VentanaPrincipal;
#endif

  VentanaLog									VentanaLogs;
  VentanaPlayerEx				        		PlayerEx;
  SkinPlayer					        		Skin;
  DWL::SO::DWLEstilos_Fuente	        		FuentePeque;
  DWL::SO::DWLEstilos_Fuente	        		FuenteMediana;
  DWL::SO::DWLEstilos_Fuente	        		FuenteGrande;
  DWL::SO::DWLEstilos_Fuente	        		FuentePequeU;
  DWL::SO::DWLEstilos_Fuente	        		FuenteMedianaU;
  DWL::SO::DWLEstilos_Fuente	        		FuenteGrandeU;
  DWL::SO::DWLEstilos_Fuente	        		Tahoma15b;
  DWL::DWLString				        		AppPath;
  ConfiguracionPlayer			        		Config;
  BaseDatos					        			BD;
  Unidades_Disco			        			UnidadesDisco;
  AsociarArchivos			        			Asociar;
#if defined UTILIZAR_FMOD
  SistemaFMOD				        			FMod;
#endif
  SistemaMedia				        			Media;
  ArbolBaseDatos			        			ArbolBaseDatos;
  ListaCanciones			        			ListaC;
  DWL::ControlesEx::DWLBarraEx	        		SliderTiempo;
  unsigned short			        			LineasOscy[1024];
  DWL::GDI::DWLMapaBits		        			Barrita;
  SistemaVLC				        			VLC;
  ControlMediaPlayer		        			MediaPlayer;
  ControlesMediaPlayer			        		ControlesMP;
												// Clase que comprueba si hay nuevas actualizaciones
  BuscarActualizaciones		        			BuscarActu;
												// Punteros de las funciones pertenecientes a la DLL del idioma que este cargada
  HINSTANCE			     	        			LibActualIdioma;
												// Funcion que retorna el texto correspondiente al numero introducido del idioma actual
												// const TCHAR *Idioma(const int Num);
  TpTextoIdioma				        	 	   *Idioma;
												// Funcion que retorna el estilo correspondiente al numero introducido del idioma actual
												// const TCHAR *Idioma_EstiloStr(const int Num);
  TpTextoIdioma				        	 	   *Idioma_EstiloStr;
												// Funcion que retorna la version del idioma	
												// const float Idioma_Version(void);
  TpVersion					        		   *Idioma_Version;
												// Funcion que retorna el nombre del idioma	
												// const TCHAR *Idioma_Nombre(void);
  TpNombre					           		   *Idioma_Nombre;
												// Funcion que retorna el icono correspondiente al idioma actual
												// HICON Idioma_Icono(void);
  TpIcono					        		   *Idioma_Icono;

  bool					        				EditandoTeclasRapidas;

  HANDLE						        		MutexPlayer;
  bool							        		PlayerInicial;
  
  DWL::DWLFiltroErrores<VentanaDump>			FiltroErrores;
};

DWL_Enlazar_Sistema(BubaTronikApp);

#endif