// Clase creada a partir de la LibMetaTag originalmente creada por Pipian.
// En ve de aprovechar la libreria de Pipian he optado por crear una yo mismo a partir de esta libreria,
// y incorporando algunos cambios para que funcione mucho mejor y de forma mas agil con BubaTronik.

// Tags soportados : ID3V1, ID3V2, WMA y Vorbis
#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <DWLString.h>
#include <DWLArchivoLog.h>

//#define synchsafe2int(synch) ((synch[0] << 21) | (synch[1] << 14) | (synch[2] << 7) | (synch[3] << 0))

#define TAM_BUFFER		4096
// CABECERAS PARA LOS TAGS WMA

enum TipoMetaDato {
	Tipo_NO_DEFINIDO	= -1,
	Tipo_ID3V1			=  0,
	Tipo_ID3V2			=  1,
	Tipo_WMA			=  2,
	Tipo_Vorbis			=  3
};
			
class MetaDato {
public:
				MetaDato(void) { };
	           ~MetaDato(void) { Tipo = Tipo_NO_DEFINIDO; };
 DWL::DWLString Nombre; // El nombre del metadato se guarda en formato ANSI
 DWL::DWLString	Datos;  // Los datos del frame se guardan en formato UNICODE
 TipoMetaDato	Tipo;
};

class Meta_ID3V2_UnSync {
public:
		Meta_ID3V2_UnSync(void) { Datos = NULL; Total = 0; };
	   ~Meta_ID3V2_UnSync(void) { };
 char  *Datos;
 int    Total;
};

class Meta_WMA {
public:
							Meta_WMA(void) { };
	                       ~Meta_WMA(void) { };
 std::vector<MetaDato *>	MetaDatos;
};

class Meta_ID3V1 {
public:
							Meta_ID3V1(void) { SubVersion = 0; };
	                       ~Meta_ID3V1(void) { };
 short						SubVersion;
 std::vector<MetaDato *>	MetaDatos;
};

class Meta_ID3V2 {
public:
							Meta_ID3V2(void) { Tam = 0; SubVersion = 0; Revision = 0; UnSync = 0; Extendido = false; };
	                       ~Meta_ID3V2(void) { };
 short						SubVersion;
 short						Revision;
 bool						UnSync;
 bool						Extendido;
 int						Tam;
 std::vector<MetaDato *>	MetaDatos;
};

class Meta_Vorbis {
public:
							Meta_Vorbis(void) { };
	                       ~Meta_Vorbis(void) { };
 DWL::DWLString				Vendor;
 std::vector<MetaDato *>	MetaDatos;
};

class MetaDatosTag  {
public :
								MetaDatosTag(void) { 
									TCHAR PathApp[1024];
									DWORD Size		= GetModuleFileName(NULL, PathApp, 1024);
									bool bDebug		= false;
									for (Size; Size > 0; Size --) {
										if (PathApp[Size] == TCHAR('\\')) {
											#if defined _DEBUG
												if (bDebug == false) {
													bDebug = true;
												}
												else {
													break;
												}
											#else // #if defined _DEBUG
												break;
											#endif
										}
									}
									PathApp[Size + 1] = 0;

									DWL::DWLString PathTmp;
									SYSTEMTIME T;
									GetLocalTime(&T);
									PathTmp.sprintf(TEXT("%sLogs\\Buscar Archivos [%04d-%02d-%02d] - [%02dh %02dm].txt"), PathApp, T.wYear, T.wMonth, T.wDay, T.wHour, T.wMinute);
									LogBuscarArchivos.Archivo(PathTmp(), false);
								};

							   ~MetaDatosTag(void) {
									BorrarMemoria();
									LogBuscarArchivos.TerminarLog();
								};

								// Función que analiza un archivo y alamcena los metadatos que hay en el
 void							AbrirArchivo(const TCHAR *Path) {
									DWORD TiempoInicio = GetTickCount();
									BorrarMemoria();
									std::fstream Archivo;
									int Pos = -1;
									Archivo.open(Path, std::ios::in | std::ios::binary);
									if (Archivo.is_open() == false) return;
									// Busco tags ID3V2
									Pos = BuscarID3V2(Archivo);
									if (Pos > -1) ObtenerDatosID3V2(Archivo, Pos);

									// Busco tags WMA
									Pos = BuscarWMA(Archivo);
									if (Pos > -1) ObtenerDatosWMA(Archivo, Pos);

									// Busco tags Vorbis
									Pos = BuscarVorbis(Archivo);
									if (Pos > -1) ObtenerDatosVorbis(Archivo, Pos);

									// Busco tags ID3V1
									if (BuscarID3V1(Archivo) == 1) ObtenerDatosID3V1(Archivo);

									Archivo.close();
									DWORD Tiempo = GetTickCount() - TiempoInicio;
								};

								// Función que borra la memoria almacenada en esta clase
 void							BorrarMemoria(void) {
									for (size_t i = 0; i < MetaDatos.size(); i++) delete MetaDatos[i];
									MetaDatos.resize(0);
									WMA.MetaDatos.resize(0);
									ID3V2.MetaDatos.resize(0);
									ID3V2.Tam			= 0; 
									ID3V2.SubVersion	= 0;
									ID3V2.Revision		= 0;
									ID3V2.UnSync		= 0;
									ID3V2.Extendido		= false;
									ID3V1.MetaDatos.resize(0);
									Vorbis.MetaDatos.resize(0);
									Vorbis.Vendor.Borrar();
								};

 Meta_WMA						WMA;

 Meta_ID3V2						ID3V2;

 Meta_ID3V1						ID3V1;

 Meta_Vorbis					Vorbis;

 std::vector<MetaDato *>		MetaDatos;

 DWL::Archivos::DWLArchivoLog	LogBuscarArchivos;
protected :
								// Retorna la posicion inicial del tag, -1 si no existe
 const int						BuscarVorbis(std::fstream &Archivo) { 
									char TagID[5] = "";
									char *pc = NULL;
									unsigned char Tipo;
									Archivo.seekg(0, std::ios::beg);
									Archivo.seekp(0, std::ios::beg);
									Archivo.read(TagID, 4);
									if (strcmp(TagID, "OggS"))	return -1;
									char *TagBuf = new char[23];
									int Status = 0;
									int Posicion = -1;
									Archivo.read(TagBuf, 23);
									pc = TagBuf;
									while (Status == 0) {
										unsigned char Segments;
										char *Lacing;
										unsigned int Pagina = 0, i;
										pc += 22;
										Segments = static_cast<unsigned char>(*pc);
										Lacing = new char [Segments];
										Archivo.read(Lacing, Segments);
										for (i = 0; i < Segments; i++) Pagina += static_cast<unsigned char>(Lacing[i]);
										delete [] TagBuf;
										TagBuf = new char [Pagina];
										Archivo.read(TagBuf, Pagina);
										pc = TagBuf;
										for (i = 0; i < Segments && Status == 0;) {
											if (strncmp(pc + 1, "vorbis", 6) == 0) {
												Tipo = static_cast<unsigned char>(*pc);
												if (Tipo == 0x03) {
													Posicion = static_cast<int>(Archivo.tellg()) - Pagina + (pc - TagBuf);
													Status = 1;
												}
											}
											pc += static_cast<unsigned char>(Lacing[i++]);
										}
										if (Status == 1 || Archivo.eof()) {
											delete [] Lacing;
											break;
										}
										delete [] TagBuf;
										TagBuf = new char [27];
										Archivo.read(TagBuf, 27);
										pc = TagBuf + 4;
										delete [] Lacing;
									}
									delete [] TagBuf;
									if (Archivo.eof())	return -1;
									else				return Posicion;
								};

								// Retorna la posicion inicial del tag, -1 si no existe
 const int						BuscarWMA(std::fstream &Archivo) {
									unsigned char WMA_GUID[16]			= {	0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
									unsigned char WMA_CONTENT_GUID[16]	= {	0x33, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C };
									char TagBuffer[TAM_BUFFER];
									char *pc = NULL;
									Archivo.seekg(0, std::ios::beg);
									Archivo.seekp(0, std::ios::beg);
									Archivo.read(TagBuffer, TAM_BUFFER);
									pc = TagBuffer;
									if (memcmp(pc, WMA_GUID, 16))			return -1;
									pc += 30;
									if (memcmp(pc, WMA_CONTENT_GUID, 16))	return -1;
									return pc - TagBuffer + 16;
								};

								// Retorna 1 si exsite, -1 si no existe.
 const int						BuscarID3V1(std::fstream &Archivo) {
									Archivo.seekg(-128, std::ios::end);
									Archivo.seekp(-128, std::ios::end);
									char TagID[4] = "";
									Archivo.read(TagID, 3);
									if (strncmp(TagID, "TAG", 3) == 0)	return 1;
									else								return -1;
								};

								// Retorna la posicion inicial del tag, -1 si no existe
 const int						BuscarID3V2(std::fstream &Archivo) {
									char TagBuffer[TAM_BUFFER];
									char *pc = TagBuffer;
									int Estado = 0;
									int CaracteresLeidos = 0;
									int Busqueda = -1;
									int Pos = 0;
									Archivo.seekg(1, std::ios::beg);
	//								Archivo.seekp(0, std::ios::beg);
									Archivo.read(TagBuffer, 10);
									CaracteresLeidos = Archivo.gcount(); 
									while (Archivo.eof() == false && Estado == 0) {
										if (Busqueda == -1) {
											if ((strncmp(pc, "ID3", 3) == 0 || strncmp(pc, "3DI", 3) == 0))	{ 
												Estado = 1;
											}
											else {
												Archivo.seekg(3, std::ios::end);
					//							Archivo.seekp(3, std::ios::end);
												Archivo.read(TagBuffer, 3);
												CaracteresLeidos = Archivo.gcount(); 
												Busqueda = -2;
											}
										}
										else {
											if (Busqueda == -2) {
												pc = TagBuffer;
												Pos = Archivo.tellg();
												if ((strncmp(pc, "ID3", 3) == 0 || strncmp(pc, "3DI", 3) == 0)) Estado = 1;
												Busqueda = 1;
											}
											if (Estado != 1) {
												Pos = static_cast<int>(Archivo.tellg()) - TAM_BUFFER;
					//							Archivo.seekp(3, std::ios::beg);
												Archivo.seekg(3, std::ios::beg);
												Archivo.read(TagBuffer, TAM_BUFFER);
												CaracteresLeidos = Archivo.gcount(); 
												pc = TagBuffer;
												for (int i = 0; i < CaracteresLeidos - 3 && Estado == 0; i++) {
													pc ++;
													if ((strncmp(pc, "ID3", 3) == 0 || strncmp(pc, "3DI", 3) == 0)) Estado = 1;
												}
												if (Estado == 1) Pos += pc - TagBuffer;
												Pos -= TAM_BUFFER - 9;
												
												if ((Pos < -TAM_BUFFER + 9 || Archivo.fail()) && Estado != 1) Estado = -1;						
					//							if ((Pos < -TAM_BUFFER + 9) && Estado != 1) Estado = -1;						
											}
										}
										if (Estado == 1 && *(pc + 3) < 0xFF && *(pc + 4) < 0xFF &&	*(pc + 6) < 0x80 && *(pc + 7) < 0x80 &&	*(pc + 8) < 0x80 && *(pc + 9) < 0x80)	
											Estado = 1;
										else if (Estado != -1) Estado = 0;

										if (Busqueda == 0)	Busqueda = -1;
									}
									if (Estado < 0 || Archivo.eof())	return -1;
									else								return Pos;
								};

 void							ObtenerDatosID3V1(std::fstream &Archivo) {
									char TagV1[125];
									Archivo.read(TagV1, 125);
									char *pc = TagV1;

									// Nombre
									MetaDato *TmpMeta;
									TmpMeta = new MetaDato;
									TmpMeta->Tipo = Tipo_ID3V1;
									TmpMeta->Datos.CopiarTexto(pc);
									TmpMeta->Nombre = TEXT("Title");
									if (TmpMeta->Datos.Tam() == 0) {
										delete TmpMeta;
									}
									else {							
										MetaDatos.push_back(TmpMeta);
										ID3V1.MetaDatos.push_back(TmpMeta);
									}

									// Grupo
									pc += 30;
									TmpMeta = new MetaDato;
									TmpMeta->Tipo = Tipo_ID3V1;
									TmpMeta->Datos.CopiarTexto(pc);
									TmpMeta->Nombre = TEXT("Artist");
									if (TmpMeta->Datos.Tam() == 0) {
										delete TmpMeta;
									}
									else {							
										MetaDatos.push_back(TmpMeta);
										ID3V1.MetaDatos.push_back(TmpMeta);
									}

									// Disco
									pc += 30;
									TmpMeta = new MetaDato;
									TmpMeta->Tipo = Tipo_ID3V1;
									TmpMeta->Datos.CopiarTexto(pc);
									TmpMeta->Nombre = TEXT("Album");
									if (TmpMeta->Datos.Tam() == 0) {
										delete TmpMeta;
									}
									else {							
										MetaDatos.push_back(TmpMeta);
										ID3V1.MetaDatos.push_back(TmpMeta);
									}

									// Año
									pc += 30;
									TmpMeta = new MetaDato;
									TmpMeta->Tipo = Tipo_ID3V1;
									TmpMeta->Datos.CopiarTexto(pc);
									TmpMeta->Nombre = TEXT("Year");
									if (TmpMeta->Datos.Tam() == 0) {
										delete TmpMeta;
									}
									else {							
										MetaDatos.push_back(TmpMeta);
										ID3V1.MetaDatos.push_back(TmpMeta);
									}

									// Comentario
									pc += 4;
									TmpMeta = new MetaDato;
									TmpMeta->Tipo = Tipo_ID3V1;
									TmpMeta->Datos.CopiarTexto(pc, 30);
									TmpMeta->Nombre = TEXT("Comment");
									if (TmpMeta->Datos.Tam() == 0) {
										delete TmpMeta;
									}
									else {							
										MetaDatos.push_back(TmpMeta);
										ID3V1.MetaDatos.push_back(TmpMeta);
									}
										
									// Pista
									pc += 28;
									if (pc[0] == '\0' && pc[1] != '\0') {
										TmpMeta = new MetaDato;
										TmpMeta->Tipo = Tipo_ID3V1;
										TmpMeta->Datos = static_cast<int>(pc[1]);
										TmpMeta->Nombre = TEXT("Track");
										if (TmpMeta->Datos.Tam() == 0) {
											delete TmpMeta;
										}
										else {
											ID3V1.MetaDatos.push_back(TmpMeta);
											MetaDatos.push_back(TmpMeta);
											ID3V1.SubVersion = 1;
										}
									}

									// Genero
									pc += 2;
									TmpMeta = new MetaDato;
									TmpMeta->Tipo = Tipo_ID3V1;
									TmpMeta->Datos = static_cast<int>(pc[0]);
									TmpMeta->Nombre = TEXT("Genre");
									if (TmpMeta->Datos.Tam() == 0) {
										delete TmpMeta;
									}
									else {							
										MetaDatos.push_back(TmpMeta);
										ID3V1.MetaDatos.push_back(TmpMeta);
									}				
								};

								// La cabecera del ID3TAGV2 son 10 bytes distribuidos de la siguiente manera :
								// CCCSROLLLL
								// 0123456789
								//
								// C  = Identificador de 3 bytes "ID3" para el principio o "3DI" si esta al final
								// SR = Version de 2 bytes, el primero es la subversion y el segundo es la revision
								// O  = Opciones de 1 byte [0x80 Unsync] [0x40 Extendido] 
								// L  = Longitud del tag 4 bytes
 void							ObtenerDatosID3V2(std::fstream &Archivo, const int Posicion) {
									char TmpBuf[10];
									char *TagBuf;
									char *pc = TmpBuf;
									bool AlFinal = false;
									MetaDato *TmpMeta = NULL;
									Archivo.seekg(Posicion, std::ios::beg);
									Archivo.read(TmpBuf, 10); // Leo toda la cabecera
									if (TmpBuf[0] == '3' && TmpBuf[1] == 'D' && TmpBuf[2] == 'I')	AlFinal = true;
				//					// Obtengo el tamaño del tag y su version
									ID3V2.SubVersion = TmpBuf[3];
									ID3V2.Revision   = TmpBuf[4];
									if ((TmpBuf[5] & 0x80) == 0x80) ID3V2.UnSync = true;
									if ((TmpBuf[5] & 0x40) == 0x40 && ID3V2.SubVersion > 0x02) ID3V2.Extendido = true;
									ID3V2.Tam = ((static_cast<unsigned char>(TmpBuf[6]) << 21) | (static_cast<unsigned char>(TmpBuf[7]) << 14) | (static_cast<unsigned char>(TmpBuf[8]) << 7) | (static_cast<unsigned char>(TmpBuf[9]) << 0));
									if (AlFinal == true) Archivo.seekg(-10 - ID3V2.Tam, std::ios::cur);
									// Leo el resto del tag en un buffer
									TagBuf = new char[ID3V2.Tam];
									Archivo.read(TagBuf, ID3V2.Tam);
									pc = TagBuf;
									if (ID3V2.Extendido == true) { // Si existe un tag extendido lo omitimos
										memcpy(TmpBuf, pc, 4);
										pc += 4;
										if (ID3V2.SubVersion == 0x03 && ID3V2.UnSync == true) ID3V2_UnSync(TmpBuf, pc);

										if (ID3V2.SubVersion == 0x03)	pc += ((static_cast<unsigned char>(TmpBuf[0]) << 21) | (static_cast<unsigned char>(TmpBuf[1]) << 14) | (static_cast<unsigned char>(TmpBuf[2]) << 7) | (static_cast<unsigned char>(TmpBuf[3]) << 0));
										else							pc += ((static_cast<unsigned char>(TmpBuf[3]) << 0) | (static_cast<unsigned char>(TmpBuf[2]) << 8) | (static_cast<unsigned char>(TmpBuf[1]) << 16) | (static_cast<unsigned char>(TmpBuf[0]) << 24));
									}
									// leer frames
									char *Fin = TagBuf + ID3V2.Tam;
									while (pc < Fin) {
										if (*pc == '\0') break;
										TmpMeta = ID3V2_ParsearFrame(&pc, Fin);
										if (TmpMeta != NULL) {
											if (TmpMeta->Datos.Tam() == 0)	{
												delete TmpMeta;
											}
											else {
												ID3V2.MetaDatos.push_back(TmpMeta);
												MetaDatos.push_back(TmpMeta);
											}
										}
									}

									delete [] TagBuf;
								};

 Meta_ID3V2_UnSync	           *ID3V2_MirarSync(char *Datos, int Tam) {
									Meta_ID3V2_UnSync  *Sync	= NULL;
									int					i		= 0;
									int					z		= 0;
									Sync = new Meta_ID3V2_UnSync;
									Sync->Datos = Datos;
									Sync->Total = 0;
									if (Tam == 0) Tam = strlen(Sync->Datos);
									for (i = 0; i < Tam; i++) {
										if (Sync->Datos[i] == 0xFF && Sync->Datos[i + 1] == 0x00) {
											for (z = i + 1; z < Tam -1; z++) Datos[z] = Datos[z + 1];
											Sync->Datos[z] = '\0';
											Sync->Total ++;
										}
									}
									return Sync;
								};

 void							ID3V2_UnSync(char *Datos, char *pc) {
									Meta_ID3V2_UnSync  *UnSync		= NULL;
									char			   *UnSyncTmp	= NULL;
									int					i			= 0;
									UnSync = ID3V2_MirarSync(Datos, 0);
									while (UnSync->Total > 0) {
										if (UnSyncTmp != NULL) delete [] UnSyncTmp;
										UnSyncTmp = new char[UnSync->Total];
										memcpy(UnSyncTmp, pc, UnSync->Total);
										pc += UnSync->Total;
										for (i = 0; i < UnSync->Total; i++) {
											Datos[4 - UnSync->Total + i] = UnSyncTmp[i];
										}
										delete UnSync;
										UnSync = ID3V2_MirarSync(Datos, 0);
									}
									delete UnSync;
									delete [] UnSyncTmp;
								};

 MetaDato			           *ID3V2_ParsearFrame(char **pc, char *Fin) {
									char FrameID[5];
									char OpcionesFrame[2] = "";
									char TmpChar[5];
									char *Ptr, *Datos = NULL, *Utf = NULL;
									int TamFrame;
									MetaDato *DatosFrame = NULL;
									switch (ID3V2.SubVersion) {
										case 2 : // Version 2.2
											if (Fin - *pc < 6) return NULL;
											memcpy(FrameID, *pc, 3);
											FrameID[3] = 0;
											*pc += 3;
											//IDFrame = ID3V2_BuscarFrame(FrameID, 2);
											memcpy(TmpChar, *pc, 3);
											if (ID3V2.UnSync == true) ID3V2_UnSync(TmpChar, *pc);
											TamFrame = ((static_cast<unsigned char>(TmpChar[2]) << 0) | (static_cast<unsigned char>(TmpChar[1]) << 8) | (static_cast<unsigned char>(TmpChar[0]) << 16));
											*pc += 3;
											break;
										case 3 : // Version 2.3
											if (Fin - *pc < 10) return NULL;
											memcpy(FrameID, *pc, 4);
											FrameID[4] = 0;
											*pc += 4;
											//IDFrame = ID3V2_BuscarFrame(FrameID, 3);
											memcpy(TmpChar, *pc, 4);
											if (ID3V2.UnSync == true) ID3V2_UnSync(TmpChar, *pc);
											TamFrame = ((static_cast<unsigned char>(TmpChar[3]) << 0) | (static_cast<unsigned char>(TmpChar[2]) << 8) | (static_cast<unsigned char>(TmpChar[1]) << 16) | (static_cast<unsigned char>(TmpChar[0]) << 24));
											*pc += 4;
											memcpy(OpcionesFrame, *pc, 2);
											*pc += 2;
											break;
										case 4 : // Version 2.4
											if (Fin - *pc < 10) return NULL;
											memcpy(FrameID, *pc, 4);
											FrameID[4] = 0;
											*pc += 4;
											//IDFrame = ID3V2_BuscarFrame(FrameID, 4);
											memcpy(TmpChar, *pc, 4);
											TamFrame = ((static_cast<unsigned char>(TmpChar[0]) << 21) | (static_cast<unsigned char>(TmpChar[1]) << 14) | (static_cast<unsigned char>(TmpChar[2]) << 7) | (static_cast<unsigned char>(TmpChar[3]) << 0));
											*pc += 4;
											break;
										default :
											return NULL;
									}
									if (TamFrame > Fin - *pc) return NULL;
									DatosFrame = new MetaDato;
									DatosFrame->Nombre = FrameID;
									DatosFrame->Tipo = Tipo_ID3V2;
									if (ID3V2.UnSync == true) OpcionesFrame[1] |= 0x02;
									if (ID3V2.SubVersion == 0x04) {
										if ((OpcionesFrame[1] & 0x08) == 0x08 || (OpcionesFrame[1] & 0x01) == 0x01) {
											*pc += 4;
											TamFrame -= 4;
										}
										if ((OpcionesFrame[1] & 0x04) == 0x04) {
											*pc ++;
											TamFrame --;
										}
										if ((OpcionesFrame[1] & 0x40) == 0x40) {
											*pc++;
											TamFrame --;
										}
									}
									else if (ID3V2.SubVersion == 0x03) {
										if ((OpcionesFrame[1] & 0x80) == 0x80) {
											char tmp[4];
											memcpy(tmp, *pc, 4);
											*pc += 4;
											if ((OpcionesFrame[1] & 0x02) == 0x02)	ID3V2_UnSync(tmp, *pc);
											TamFrame -= 4;
										}
										if ((OpcionesFrame[1] & 0x40) == 0x40)	{
											*pc ++;
											TamFrame --;
										}
										if((OpcionesFrame[1] & 0x20) == 0x20) {
											*pc ++;
											TamFrame --;
										}
									}

	//								if (FrameID[0] == 'T' && strcmp(FrameID, "TXXX") && strcmp(FrameID, "TXX")) {	
										int Codificado;
										Ptr = *pc;
										Codificado = *(Ptr++);
										Datos = new char[TamFrame];
										*(Datos + TamFrame -1) = '\0';
										memcpy(Datos, Ptr, TamFrame -1);
										if ((OpcionesFrame[1] & 0x02) == 0x02) {
											Meta_ID3V2_UnSync *UnSync = ID3V2_MirarSync(Datos, TamFrame);
											TamFrame -= UnSync->Total;
											delete UnSync;
										}
										switch (Codificado) {
											case 0x00 : // iso 88591
												DatosFrame->Datos.CopiarTexto(Datos, TamFrame);
			//									iso88591_to_utf8(Datos, TamFrame - 1, &Utf);
												break;
											case 0x01 : // utf16
			//									utf16bom_to_utf8(Datos, TamFrame - 1, &Utf);
												DatosFrame->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(Datos), TamFrame);
												break;
											case 0x02 : // utf16 be
												DatosFrame->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(Datos), TamFrame);
			//									utf16be_to_utf8(Datos, TamFrame - 1, &Utf);
												break;
											case 0x03 : // ASCII
			//									Utf = new char[TamFrame];
			//									strcpy(Utf, Datos);
												DatosFrame->Datos.CopiarTexto(Datos, TamFrame);
												break;
											default :
												_ASSERT(0);
												break;
										}
										delete [] Datos;
										//if (Utf != NULL) delete [] Utf;
	//								}

									/*else { // UnSync
										Ptr = *pc;
										Datos = new char[TamFrame];
										memcpy(Datos, Ptr, TamFrame);
										if ((OpcionesFrame[1] & 0x02) == 0x02) {
											Meta_ID3V2_UnSync *UnSync = ID3V2_MirarSync(Datos, TamFrame);
											TamFrame -= UnSync->Total;
											delete UnSync;
										}
										DatosFrame->Datos = Datos;
									}*/

									*pc += TamFrame;
									return DatosFrame;
								};

 void							ObtenerDatosWMA(std::fstream &Archivo, const int Posicion) { 
									unsigned char	WMA_EXTENDED_GUID[16] = {	0x40, 0xA4, 0xD0, 0xD2, 0x07, 0xE3, 0xD2, 0x11, 0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50 };
									MetaDato       *TmpMeta = NULL;
									char			TmpBuf[8];
									char           *TagBuf;
									char           *pc = NULL;
									int				TotalItems = 0;
									
									Archivo.seekg(Posicion, std::ios::beg);
									Archivo.read(TmpBuf, 8); // Leo el tamaño de la cabecera (64 bits)
									int Tam = ((static_cast<unsigned char>(TmpBuf[0]) << 0) | (static_cast<unsigned char>(TmpBuf[1]) << 8) | (static_cast<unsigned char>(TmpBuf[2]) << 16) | (static_cast<unsigned char>(TmpBuf[3]) << 24));
									TagBuf = new char [Tam];
									Archivo.read(TagBuf, Tam - 24); // Leo el tag base en memoria (faltara por leer la parte extendida)
									pc = TagBuf;
									int Tam_Titulo = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
									pc += 2;
									int Tam_Autor = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
									pc += 2;
									int Tam_Copyright = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
									pc += 2;
									int Tam_Descripcion = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
									pc += 2;
									int Tam_Nota = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
									pc += 2;
									if (Tam_Titulo > 0) {
										TmpMeta = new MetaDato;
										TmpMeta->Nombre = TEXT("WM/Title");
										TmpMeta->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Titulo);
										TmpMeta->Tipo = Tipo_WMA;
										MetaDatos.push_back(TmpMeta);
										WMA.MetaDatos.push_back(TmpMeta);
										pc += Tam_Titulo;
										TotalItems ++;
									}
									if (Tam_Autor > 0) {
										TmpMeta = new MetaDato;
										TmpMeta->Nombre = TEXT("WM/Author");
										TmpMeta->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Autor);
										TmpMeta->Tipo = Tipo_WMA;
										MetaDatos.push_back(TmpMeta);
										WMA.MetaDatos.push_back(TmpMeta);
										pc += Tam_Autor;
										TotalItems ++;
									}
									if (Tam_Copyright > 0) {
										TmpMeta = new MetaDato;
										TmpMeta->Nombre = TEXT("WM/Copyright");
										TmpMeta->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Copyright);
										TmpMeta->Tipo = Tipo_WMA;
										MetaDatos.push_back(TmpMeta);
										WMA.MetaDatos.push_back(TmpMeta);
										pc += Tam_Copyright;
										TotalItems ++;
									}
									if (Tam_Descripcion > 0) {
										TmpMeta = new MetaDato;
										TmpMeta->Nombre = TEXT("WM/Description");
										TmpMeta->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Descripcion);
										TmpMeta->Tipo = Tipo_WMA;
										MetaDatos.push_back(TmpMeta);
										WMA.MetaDatos.push_back(TmpMeta);
										pc += Tam_Descripcion;
										TotalItems ++;
									}
									if (Tam_Nota > 0) {
										TmpMeta = new MetaDato;
										TmpMeta->Nombre = TEXT("WM/Rating");
										TmpMeta->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam_Nota);
										TmpMeta->Tipo = Tipo_WMA;
										MetaDatos.push_back(TmpMeta);
										WMA.MetaDatos.push_back(TmpMeta);
										pc += Tam_Nota;
										TotalItems ++;
									}
									// Parte extendida del tag
									Archivo.read(TagBuf, 16); // Leo la cabecera del tag extendido
									if (memcmp(TagBuf, WMA_EXTENDED_GUID, 16)) { 
										delete [] TagBuf;
										return; // No hay tag extendido salgo.
									}
									Archivo.read(TmpBuf, 8); // Leo el tamaño del tag extendido
									Tam = ((static_cast<unsigned char>(TmpBuf[0]) << 0) | (static_cast<unsigned char>(TmpBuf[1]) << 8) | (static_cast<unsigned char>(TmpBuf[2]) << 16) | (static_cast<unsigned char>(TmpBuf[3]) << 24));
									delete [] TagBuf;
									TagBuf = new char[Tam];
									Archivo.read(TagBuf, Tam); // Leo el tag extendido en memoria
									pc = TagBuf;
									int NumItems = TotalItems + ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8));
									pc += 2;
									int Tipo = 0;
									for (int i = TotalItems; i < NumItems; i++) {
										Tam = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8)); // Tamaño del nombre
										pc += 2;
										TmpMeta = new MetaDato;
										TmpMeta->Tipo = Tipo_WMA;
										TmpMeta->Nombre.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam);
										pc += Tam;
										Tipo = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8)); // Tipo de datos
										pc += 2;
										Tam = ((static_cast<unsigned char>(pc[0]) << 0) | (static_cast<unsigned char>(pc[1]) << 8)); // Tamaño de los datos
										pc += 2;
										switch (Tipo) {
											case 0 : // UTF16 Little Endian
												TmpMeta->Datos.CopiarTexto(reinterpret_cast<wchar_t *>(pc), Tam);
												break;
											case 1 : // Binario
												break;
											case 2 : // Bool
												break;
											case 3 : // int32
												break;
											case 4 : // double
												break;
											default :
												TmpMeta->Datos.CopiarTexto(pc, Tam);
												break;
										}
										pc += Tam;
										if (TmpMeta->Datos.Tam() > 0) {
											MetaDatos.push_back(TmpMeta);
											WMA.MetaDatos.push_back(TmpMeta);
										}
										else {
											delete TmpMeta;
										}
									}
									delete [] TagBuf;
								};

void							ObtenerDatosVorbis(std::fstream &Archivo, const int Posicion) { 
									char Tmp[4];
									char *Tmp2;
									int i;
									MetaDato *TmpMeta;
									Archivo.seekg(Posicion + 7, std::ios::beg);
									Archivo.seekp(Posicion + 7, std::ios::beg);
									Archivo.read(Tmp, 4);
									int Tam = ((static_cast<unsigned char>(Tmp[0]) << 0) | (static_cast<unsigned char>(Tmp[1]) << 8) | (static_cast<unsigned char>(Tmp[2]) << 16) | (static_cast<unsigned char>(Tmp[3]) << 24));
									Tmp2 = new char [Tam];
									Archivo.read(Tmp2, Tam);
									Vorbis.Vendor.CopiarTexto(Tmp2, 29);
									delete Tmp2;
									Archivo.read(Tmp, 4);
									int TotalItems = ((static_cast<unsigned char>(Tmp[0]) << 0) | (static_cast<unsigned char>(Tmp[1]) << 8) | (static_cast<unsigned char>(Tmp[2]) << 16) | (static_cast<unsigned char>(Tmp[3]) << 24));

									for (i = 0; i < TotalItems; i++) {
										char *data, *pc;
										Archivo.read(Tmp, 4);
										Tam = ((static_cast<unsigned char>(Tmp[0]) << 0) | (static_cast<unsigned char>(Tmp[1]) << 8) | (static_cast<unsigned char>(Tmp[2]) << 16) | (static_cast<unsigned char>(Tmp[3]) << 24));
										data = new char [Tam];
										Archivo.read(data, Tam);
										pc = strchr(data, '=');
										*pc = '\0';
										pc++;
										TmpMeta = new MetaDato;
										TmpMeta->Tipo = Tipo_Vorbis;
										TmpMeta->Nombre = data;
										TmpMeta->Datos.CopiarTexto(pc, (Tam - TmpMeta->Nombre.Tam()) -1);
										MetaDatos.push_back(TmpMeta);
										Vorbis.MetaDatos.push_back(TmpMeta);
										delete [] data;
									}
								};
};