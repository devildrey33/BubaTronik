#pragma once

#include <DWLString.h>

enum Tipo_Unidad_Disco {
	Tipo_Desconocido		= 0,
	Tipo_Path_Invalido		= 1,
	Tipo_Extraible			= 2,
	Tipo_Fijo				= 3,
	Tipo_Remoto				= 4,
	Tipo_CDRom				= 5,
	Tipo_RamDisk			= 6
};

// Aquesta clase podria posarla dintre de la DWL al ser tan generica
class Unidad_Disco {
public:
							Unidad_Disco(void) { 
							};

							Unidad_Disco(TCHAR *nPath) { 
								Obtener(nPath); 
							};

				           ~Unidad_Disco(void) { 
						    };

 BOOL						Obtener(const TCHAR *nPath) {
								_Letra = nPath[0];
								TCHAR Nombre[512];
								TCHAR Tipo[512];
								BOOL R = GetVolumeInformation(nPath, Nombre, 512, &_Numero_Serie, &_Longitud_Componente, &_Flags, Tipo, 512);
								_Nombre_Volumen = Nombre;
								_Tipo_Particion = Tipo;
								_Tipo_Disco = static_cast<Tipo_Unidad_Disco>(GetDriveType(nPath));
								if (R != FALSE) R = TRUE;
								return R;
							};

 const bool					Solo_Lectura(void) {
								if (_Flags & FILE_READ_ONLY_VOLUME) return true; 
								return false;
							};

 const DWORD				Numero_Serie(void) { 
								return _Numero_Serie; 
							};

 const DWORD				Longitud_Componente(void) { 
								return _Numero_Serie; 
							};

 const TCHAR			   *Nombre_Volumen(void) {
								return _Nombre_Volumen();
							}

 const TCHAR			   *Tipo_Particion(void) {
								return _Tipo_Particion();
							}
							// Retorna la letra de la unidad en mayuscula
 const TCHAR				Letra(void) {
								return _Letra;
							};
 const Tipo_Unidad_Disco	Tipo_Disco(void) {
								return _Tipo_Disco;
							};
private:
 DWL::DWLString			   _Nombre_Volumen;
 DWL::DWLString			   _Tipo_Particion;
 DWORD					   _Numero_Serie;
 DWORD					   _Longitud_Componente;
 DWORD					   _Flags;
 TCHAR					   _Letra;
 Tipo_Unidad_Disco		   _Tipo_Disco;
};




class Unidades_Disco {
public :
								Unidades_Disco(void) { 
								};

			                   ~Unidades_Disco(void) { 
								   BorrarMemoria();
								};
								// Mira las unidades locales de la 'C' a la 'Z' evitando las disqueteras.
 const unsigned int				Escanear_Unidades_Locales(void) {
									BorrarMemoria();
									TCHAR Path[4];
									BOOL  Exito = FALSE;
									DWLStrCopy(Path, 4, TEXT("?:\\"));
									Unidad_Disco *Unidad = new Unidad_Disco;
									for (TCHAR n = TEXT('C'); n < TEXT('Z'); n++) {
										Path[0] = n;
										Exito = Unidad->Obtener(Path);
										if (Exito == TRUE) {
											_Unidades.push_back(Unidad);
											Unidad = new Unidad_Disco;
										}
									}
									delete Unidad;
									return static_cast<unsigned int>(_Unidades.size());
								};

 Unidad_Disco			       *Unidad(const int Pos) {
									return _Unidades[Pos];
								};

 Unidad_Disco				   *Buscar_Numero_Serie(const DWORD Numero) {
									for (size_t n = 0; n < _Unidades.size(); n++) {
										if (_Unidades[n]->Numero_Serie() == Numero) return _Unidades[n];
									}
									return NULL;
								};

 Unidad_Disco				   *Buscar_Letra(const TCHAR Letra) {
									TCHAR L2 = toupper(Letra);
									for (size_t n = 0; n < _Unidades.size(); n++) {
										if (_Unidades[n]->Letra() == L2) return _Unidades[n];
									}
									return NULL;
								};

 const unsigned int				TotalUnidades(void) {
									return static_cast<unsigned int>(_Unidades.size());
								};
private :
 void							BorrarMemoria(void) {
									for (size_t n = 0; n < _Unidades.size(); n++) delete _Unidades[n];
									_Unidades.resize(0);
								};

 std::vector<Unidad_Disco *>   _Unidades;
};
