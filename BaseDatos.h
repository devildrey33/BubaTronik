#pragma once

#include "ConfigBubaTronik.h"
#include "Definiciones.h"
#if defined UTILIZAR_FMOD
	#include "FMOD.hpp"
	#include "FMOD_ERRORS.h"
#endif

#include "TMedio.h"
#include "ItemListaMedios.h"
#include "NodoArbolBaseDatos.h"
#include "ArchivoBinarioPlayer.h"
#include "ConfiguracionPlayer.h"
//#include <DWLListaEx.h>
#include <DWLArbolEx.h>
//#include <DWLString.h>
#include <DWLBarraEx.h>
#include <DWLBotonEx.h>

class TMedio;

class AnomaliaGen;
class AnomaliaGrp;
class AnomaliaDsk;




// Base de datos para los archivos de audio
class BaseDatos {
public:
								BaseDatos(void);
	                           ~BaseDatos(void);
 const bool						Iniciar(const TCHAR *Path);
 void                           Guardar(const TCHAR *Path);
 void                           Terminar(void);

 StrTp						   *AgregarStrTp(DWL::DWLString *Str, std::vector<StrTp *> &Vec);
 StrTp						   *AgregarStrTp(const TCHAR *Str, std::vector<StrTp *> &Vec);

 TMedio                        *AgregarMedioBD(TMedio *nMedio);
								// Funcion que retorna si el path existe en la BD (utilizada por el ThreadBuscarArchivos mediante un SendMessage)
 TMedio                        *BuscarPath(const TCHAR *Path);

 const int						RevisarBD(DWL::ControlesEx::DWLBarraEx *BarraProgreso);
 void							CargarListaInicio(void);
 const int						SeleccionAleatoria(const int Al = -1);
 void							GeneroSeleccionado(const int Pos);
 const int						GeneroAleatorio(StrTp *StrGenero = NULL);
 const int						GrupoAleatorio(StrTp *StrGrupo = NULL);
 const int						DiscoAleatorio(StrTp *StrDisco = NULL);
 const int						CancionesAleatorias(const int Num);
 const int						CancionesMasEscuchadas(const int Num);
 void							Estrellas5(void);

 void							GeneroMasEscuchado(void);
 void							GrupoMasEscuchado(void);
 void							DiscoMasEscuchado(void);

 StrTp					       *GeneroMasEscuchadoStrTp(void);
 StrTp						   *GrupoMasEscuchadoStrTp(void);
 StrTp					       *DiscoMasEscuchadoStrTp(void);

 void                           CrearSugerenciasGenero(void);
 void                           CrearSugerenciasGrupo(void);
 void                           CrearSugerenciasDisco(void);

 inline StrTp                  *ObtenerSugerenciaGenero1(void) { return _SugerenciaGenero1;  };
 inline StrTp                  *ObtenerSugerenciaGrupo1(void)  { return _SugerenciaGrupo1;   };
 inline StrTp                  *ObtenerSugerenciaDisco1(void)  { return _SugerenciaDisco1;   };
 inline StrTp                  *ObtenerSugerenciaGenero2(void) { return _SugerenciaGenero2;  };
 inline StrTp                  *ObtenerSugerenciaGrupo2(void)  { return _SugerenciaGrupo2;   };
 inline StrTp                  *ObtenerSugerenciaDisco2(void)  { return _SugerenciaDisco2;   };

 void							RepararAnomaliasMediosAudio(DWL::ControlesEx::DWLBarraEx *Barra);

 void							BorrarMedio(const UINT C);
 void							BorrarMedio(TMedio *C);
 
 std::vector<TMedio *>			Medios;

 std::vector<TDisco *>			Discos;
 std::vector<TGrupo *>			Grupos;
 std::vector<TGenero *>			Generos;
 std::vector<TTipo *>			Tipos;
 std::vector<StrTp *>		    StrDiscos;
 std::vector<StrTp *>		    StrGrupos;
 std::vector<StrTp *>		    StrGeneros;
 std::vector<StrTp *>			StrTipos;
// const bool						BuscandoArchivos(void) {	return Buscando;	};
 const int						BuscarStrTp(const TCHAR *Nombre, std::vector<StrTp *> &Vec);
 void							ActualizarArbol(void);
// void							ActualizarArbolVideo(void);
 const int						MiraIconoCancion(const int Escuchada);
 const int						MiraIconoVideo(const int Reproducido);

 inline TMedio                 *MedioReciente(const UINT Pos) { return _MediosRecientes[Pos]; };
 void                           AgregarMedioReciente(TMedio *nMedio);
 /*
 void							FC_AnalizarGeneros(DWL::ControlesEx::DWLBarraEx *Barra, DWL::ControlesEx::DWLArbolEx *Arbol);
 void							FC_AnalizarGrupos(DWL::ControlesEx::DWLBarraEx *Barra, DWL::ControlesEx::DWLArbolEx *Arbol);
 void							FC_AnalizarDiscos(DWL::ControlesEx::DWLBarraEx *Barra, DWL::ControlesEx::DWLArbolEx *Arbol);
 void							FC_TerminarAnalisis(void);
// bool							Buscando;

 void							FC_AplicarGeneros(void);
 void							FC_AplicarGrupos(void);
 void							FC_AplicarDiscos(void);

 void							FC_EliminarGenero(void);
 void							FC_EliminarGrupo(void);
 void							FC_EliminarDisco(void);

 void							FC_ElegirEntradaGenero(void);
 void							FC_ElegirEntradaGrupo(void);
 void							FC_ElegirEntradaDisco(void);

 void							FC_ModificarNombreGenero(const TCHAR *nTxt);
 void							FC_ModificarNombreGrupo(const TCHAR *nTxt);
 void							FC_ModificarNombreDisco(const TCHAR *nTxt);*/

 inline const bool				RevisandoBD(void) { return Revisando; };
 void							Parar_RevisarBD(void) { Revisando = false; };
private:
 void                          _ArbolBD_AgregarM3U(NodoArbolBaseDatos *nListas, TMedio *MedioM3U);

 StrTp                        *_CrearSugerenciaGenero(void);
 StrTp                        *_CrearSugerenciaGrupo(void);
 StrTp                        *_CrearSugerenciaDisco(void);

 void							ConstruyeTGGD(void);
 void							ConstruyeGGD(void);
 void							ConstruyeGD(void);
 void							ConstruyeD(void);

 const int					   _CompararStrTp(StrTp &Origen, StrTp &Destino, const int Tipo);
 const int					   _Distancia(DWL::DWLString &Origen, DWL::DWLString &Destino);

 TTipo					      *_BuscaTipo(StrTp *Tp);
 TGenero                      *_BuscaGenero(StrTp *Gen, TTipo *Tp, StrTp *StrTipo);
 TGrupo                       *_BuscaGrupo(StrTp *Grp, TTipo *Tp, TGenero *Gen, StrTp *StrTipo, StrTp *StrGenero);
 TDisco                       *_BuscaDisco(StrTp *Dsk, TTipo *Tp, TGenero *Gen, TGrupo *Grp, StrTp *StrTipo, StrTp *StrGenero, StrTp *StrGrupo);
	
	
 void			   	           _CargarTipos(ArchivoBinarioPlayer &BDG);
 void			               _CargarGeneros(ArchivoBinarioPlayer &BDG);
 void			               _CargarGrupos(ArchivoBinarioPlayer &BDG);
 void	                       _CargarDiscos(ArchivoBinarioPlayer &BDG);
 void	                       _CargarMedios(ArchivoBinarioPlayer &BDG);
 const UINT	                   _CargarMedios057(ArchivoBinarioPlayer &BDG);
								// La funcion _CargarTag se mantiene por compatibilidad con la version 0.57 de la BD
 void 						   _CargarTag(ArchivoBinarioPlayer &BDG); 
 void                          _CargarMediosRecientes(ArchivoBinarioPlayer &BDG);

 void                          _GuardarTipos(ArchivoBinarioPlayer &BDG);
 void                          _GuardarGeneros(ArchivoBinarioPlayer &BDG);
 void                          _GuardarGrupos(ArchivoBinarioPlayer &BDG);
 void                          _GuardarDiscos(ArchivoBinarioPlayer &BDG);
 void                          _GuardarMedios(ArchivoBinarioPlayer &BDG);
 void                          _GuardarMediosRecientes(ArchivoBinarioPlayer &BDG);

 StrTp                        *_SugerenciaGenero1;
 StrTp                        *_SugerenciaGrupo1;
 StrTp                        *_SugerenciaDisco1;
 StrTp                        *_SugerenciaGenero2;
 StrTp                        *_SugerenciaGrupo2;
 StrTp                        *_SugerenciaDisco2;
 
 TMedio                       *_MediosRecientes[10];

 bool							Revisando;

};

