#include "StdAfx.h"
#include "EnviarEmail.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
//#include <windows.h>
#include <winsock2.h>
#include <DWLArchivoBinario.h>
#include "VentanaMensaje.h"
#include "BubaTronikApp.h"

// Se escuchan las contestaciones del servidor
#define ESCUCHAR(sd)   sd 

// Solo habla el reproductor e ignora lo que dice el servidor (para evitar hangs en el recv)
//#define ESCUCHAR(sd)



#pragma comment(lib, "ws2_32.lib")


EnviarEmail::EnviarEmail(void) {
	
}


EnviarEmail::~EnviarEmail(void) {
}

const bool EnviarEmail::_Revisar(int iStatus, char *szFunction) {
	if ((iStatus != SOCKET_ERROR) && (iStatus))	return true;
	_Error = true;
	return false; // error
}

//#define CRLF "\r\n"                 // carriage-return/line feed pair


// Función que envia el archivo del path a bubatronik.dumps@devildrey33.es
const bool EnviarEmail::Enviar(const TCHAR *Path, DWL::ControlesEx::DWLBarraEx *Barra) {
	int         iProtocolPort        = 0;
	char        szSmtpServerName[64] = "smtp.1and1.es";
	char        szToAddr[64]         = "bubatronik.dumps@devildrey33.es";
	char        szFromAddr[64]       = "bubatronik.app@devildrey33.es";
	char        szBuffer[4096]       = "";
	char        szLine[256]          = "";
	char        szMsgLine[256]       = "";
	SOCKET      hServer = NULL;
	WSADATA     WSData;
	LPHOSTENT   lpHostEntry;
	LPSERVENT   lpServEntry;
	SOCKADDR_IN SockAddr;

	static VentanaMensaje_ErrorCritico Msg;

	char        szLog[]  = "YnViYXRyb25pay5hcHBAZGV2aWxkcmV5MzMuZXM=\r\n";     // bubatronik.app
	char        szPass[] = "Q3VlbnRhTWFuZGFEdW1wcw==\r\n"; // cuentamandadumps

	_Error = false;
	
	// Attempt to intialize WinSock (1.1 or later)
	if (WSAStartup(MAKEWORD(1, 1), &WSData)) {
		Msg.MostrarMensaje(IDIOMA__ERROR_CARGANDO_WINSOCK, IDIOMA__ERROR, 0);
		return false;
	}

	// Lookup email server's IP address.
	lpHostEntry = gethostbyname(szSmtpServerName);
	if (!lpHostEntry) {
		Msg.MostrarMensaje(IDIOMA__ERROR_OBTENIENDO_LA_IP, IDIOMA__ERROR, 0);
		return false;
	}

	// Create a TCP/IP socket, no specific protocol
	hServer = socket(PF_INET, SOCK_STREAM, 0);
	if (hServer == INVALID_SOCKET) {
		Msg.MostrarMensaje(IDIOMA__ERROR_SOCKET_INVALIDO, IDIOMA__ERROR, 0);
		return false;
	}

	// Get the mail service port
	lpServEntry = getservbyname("mail", 0);

	// Use the SMTP default port if no other port is specified
	if (!lpServEntry)
		iProtocolPort = htons(IPPORT_SMTP);
	else
		iProtocolPort = lpServEntry->s_port;

	// Setup a Socket Address structure
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port   = iProtocolPort;
	SockAddr.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
	
	// Establezco el timeout a 3 segundos para recv
/*	timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	if (setsockopt(hServer, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv))) {
		Msg.MostrarMensaje(TEXT("Error estableciendo propiedades del socket."), TEXT("ERROR"), 0);
		return false;
	}*/
	
	// Connect the Socket
	if (connect(hServer, (PSOCKADDR) &SockAddr, sizeof(SockAddr))) {
		Msg.MostrarMensaje(IDIOMA__ERROR_CONECTANDO_AL_SERVIDOR, IDIOMA__ERROR, 0);
		return false;
	}


	// Cargo y codifico el archivo a mandar para saber el tamaño que ocupa en base64
	DWL::Archivos::DWLArchivoBinario Archivo;
	Archivo.AbrirArchivoLectura(Path);
	UINT TamArchivo = Archivo.Tam();
	char *ArchivoChar = new char [TamArchivo];
	Archivo.Leer(ArchivoChar, TamArchivo);
	std::string &ArchivoStd = _base64_encode((unsigned char *)ArchivoChar, TamArchivo);
	delete [] ArchivoChar;
	size_t i = 0;

	Barra->Maximo(ArchivoStd.size());
	Barra->Valor(0);
	Sistema.App.Eventos_Mirar();

	// Receive initial response from SMTP server
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() Reply"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);
	// Send HELO server.com
	sprintf_s(szMsgLine, 255, "EHLO %s\r\n", szSmtpServerName);
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() EHLO");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() EHLO"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);

	_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() EHLO"); // autentificaciones
	ZeroMemory(szBuffer, sizeof(char) * 4096);

	// AUTENTIFICAMOS
	strcpy_s(szMsgLine, 255, "AUTH LOGIN\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() AUTH LOGIN");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() AUTH LOGIN")); // autentificaciones
	ZeroMemory(szBuffer, sizeof(char) * 4096);
	_Revisar(send(hServer, szLog, strlen(szLog), 0), "send() LOGIN");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() LOGIN")); // autentificaciones
	ZeroMemory(szBuffer, sizeof(char) * 4096);
	_Revisar(send(hServer, szPass, strlen(szPass), 0), "send() PASS");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() PASS")); // autentificaciones
	ZeroMemory(szBuffer, sizeof(char) * 4096);




	// Send MAIL FROM: <sender@mydomain.com>
	sprintf_s(szMsgLine, 255, "MAIL FROM:<%s>\r\n", szFromAddr);
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() MAIL FROM");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() MAIL FROM"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);

	// Send RCPT TO: <receiver@domain.com>
	sprintf_s(szMsgLine, 255, "RCPT TO:<%s>\r\n", szToAddr);
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() RCPT TO");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() RCPT TO"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);

	// Send SUBJECT: BubaTornik dump
	//  sprintf(szMsgLine, "SUBJECT: BubaTronik dump%s", szToAddr, CRLF);
	//  _Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() SUBJECT");
	//  _Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() SUBJECT");
	//ZeroMemory(szBuffer, sizeof(char) * 4096);

	// Send DATA
	strcpy_s(szMsgLine, 255, "DATA\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() DATA");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() DATA"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);


	strcpy_s(szMsgLine, 255, "MIME-Version: 1.0\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() MIME-Version");

	strcpy_s(szMsgLine, 255, "Content-Type:MULTIPART/MIXED;boundary=\"Boundary-=_StOHgENiGNeW\"\n\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Content-Type");

	strcpy_s(szMsgLine, 255, "--Boundary-=_StOHgENiGNeW\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Fin boundary");

	strcpy_s(szMsgLine, 255, "Content-Type: text/plain\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Content-Type");

	strcpy_s(szMsgLine, 255, "Content-Transfer-Encoding: 8bit\n\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Content-Transfer-Encoding");

	UINT VisualSudioVer = 2008;
	// mensaje 
	#if _MSC_VER == 1600
		VisualSudioVer = 2010;
	#endif
	sprintf_s(szMsgLine, 255, "BubaTronik : %f, WINVER : %d.%d SP%d.%d, Compilado con : VisualStudio %d\r\n", APP_NUM_VER, Sistema.VersionMayor(), Sistema.VersionMenor(), Sistema.VersionServicePackMayor(), Sistema.VersionServicePackMenor(), VisualSudioVer);
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Mensaje");

	strcpy_s(szMsgLine, 255, "--Boundary-=_StOHgENiGNeW\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Fin boundary");



	strcpy_s(szMsgLine, 255, "Content-Type:application/dump:\n name=\"BubaTronik.dmp\"\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Content-Type");

	strcpy_s(szMsgLine, 255, "Content-Transfer-Encoding: base64\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Content-Transfer-Encoding");

	strcpy_s(szMsgLine, 255, "Content-Disposition: attachment; filename=\"BubaTornik.dmp\"\n\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Content-Disposition");

	std::string Parte;
	// archivo codificado
	for (i = 0; i < ArchivoStd.size(); i += 997) {
		Parte = ArchivoStd.substr(i, 997);
		Parte += "\r\n";
		_Revisar(send(hServer, Parte.c_str(), Parte.size(), 0), "send() Archivo adjunto");
		Barra->Valor_SumarValor(997);
		Sistema.App.Eventos_Mirar();
	}
	//_Revisar(send(hServer, ArchivoStd.c_str(), ArchivoStd.size(), 0), "send() Archivo adjunto");


	strcpy_s(szMsgLine, 255, "--Boundary-=_StOHgENiGNeW\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() Fin boundary");


	// Send blank line and a period
	strcpy_s(szMsgLine, 255, "\r\n.\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() end-message");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() end-message"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);

	// Send QUIT
	strcpy_s(szMsgLine, 255, "QUIT\r\n");
	_Revisar(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() QUIT");
	ESCUCHAR(_Revisar(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() QUIT"));
	ZeroMemory(szBuffer, sizeof(char) * 4096);

	// Close server socket and prepare to exit.
	closesocket(hServer);

	WSACleanup();

	if (_Error == false) 	return true;
	Msg.MostrarMensaje(IDIOMA__ERROR_DURANTE_LA_TRANSMISION, IDIOMA__ERROR, 0);
	
	return false;
}



/* 
   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/
std::string &EnviarEmail::_base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)	ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 3; j++)	char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)	ret += base64_chars[char_array_4[j]];

		while((i++ < 3))	ret += '=';
	}
	return ret;
}