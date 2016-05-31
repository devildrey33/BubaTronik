#pragma once

class AsociarArchivos {
public:
					AsociarArchivos(void);
				   ~AsociarArchivos(void);
					// Registra la aplicación y asigna las extensiones
 void				RegistrarApp(void);
 const bool         ComprobarAsociaciones(void);
 void				DesRegistrarApp(void);

 /*
 void				Asociar_Archivos_Audio(const bool Actualizar, const bool ReproducirPorDefecto = false);
 void				Asociar_Archivos_Video(const bool Actualizar, const bool ReproducirPorDefecto = false);
 const bool			Comprobar_Video(void);
 const bool			Comprobar_Audio(void);
// const TCHAR       *Extensiones_Video(const int Pos);
// const TCHAR       *Extensiones_Audio(const int Pos);
 void				AsociarExtensionEjecutable(const TCHAR *Extension, const TCHAR *PathApp, const TCHAR *AppArg, const TCHAR *NombreApp, const TCHAR *PathIcono, const TCHAR *Descripcion, const TCHAR *Comando, const TCHAR *NombreBackup);*/
// void				DesacerExtensionEjecutable(const TCHAR *Extension, const TCHAR *NombreBackup);
 const bool			ReproducirPorDefecto(void);

 void				AsignarReproducirPorDefecto(const bool Reproducir = false);
};
