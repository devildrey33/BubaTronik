#pragma once

#include <DWLString.h>
#include <vector>
#include "ArchivoBinarioPlayer.h"
#include "TMedio.h"

/*enum TipoArchivo_Tipo {
    Nada    = -1,
    Audio   = 0,
    Video   = 1,
    Lista   = 2,
    CDAudio = 3
};*/

class TipoArchivo_Datos {
 public:
                    TipoArchivo_Datos(void) { Tipo = Tipo_Medio_INDEFINIDO; ID = 0; Buscar_Asociar = true; };
                    TipoArchivo_Datos(const TCHAR *nExtension, const Tipo_Medio nTipo, const int nID) { Extension = nExtension, Tipo = nTipo; ID = nID; Buscar_Asociar = true; };
                   ~TipoArchivo_Datos(void) { };
  DWL::DWLString    Extension;
  Tipo_Medio		Tipo;
  int               ID;
                    // Indica si este tipo se buscara al realizar una busqueda, y si se asociara este tipo con el repoductor para el explorer.
  bool              Buscar_Asociar;
};

class TipoArchivo {
 public:
                                     TipoArchivo(void);
                                    ~TipoArchivo(void);
  void                               BorrarTodosLosTipos(void);
                                     // Restaura los tipos de archivo por defecto
  void                               TiposPorDefecto(void);
  const bool                         AgregarTipoArchivo(const TCHAR *nExtension, const Tipo_Medio nTipo, const int nID);
  TipoArchivo_Datos                 *BuscarTipoArchivo(const TCHAR *nExtension);
  void                               CopiarTipos(TipoArchivo &Origen);
  const bool                         CargarTipos(ArchivoBinarioPlayer &Archivo);
  void                               GuardarTipos(ArchivoBinarioPlayer &Archivo);
  TipoArchivo_Datos				    *Extension(const TCHAR *Path);
  std::vector<TipoArchivo_Datos *>   Tipos;
  UINT                               UltimaID;

};
