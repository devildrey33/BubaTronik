#pragma once

// Las siguientes macros definen la plataforma m�nima requerida. Esta plataforma es la primera
// versi�n de Windows, Internet Explorer, etc. que tiene las caracter�sticas necesarias para ejecutar 
// la aplicaci�n. Las macros funcionan habilitando todas las caracter�sticas disponibles 
// en las versiones de la plataforma hasta la versi�n especificada, incluida esta.

// Modifique las siguientes definiciones si tiene que seleccionar como destino una plataforma antes que las especificadas a continuaci�n.
// Consulte la referencia MSDN para obtener la informaci�n m�s reciente sobre los valores correspondientes a las diferentes plataformas.
#ifndef WINVER                          // Especifica que la plataforma m�nima requerida es Windows Vista.
#define WINVER 0x0600           // Cambiar al valor apropiado correspondiente a otras versiones de Windows.
#endif

#ifndef _WIN32_WINNT            // Especifica que la plataforma m�nima requerida es Windows Vista.
#define _WIN32_WINNT 0x0600     // Cambiar al valor apropiado correspondiente a otras versiones de Windows.
#endif

#ifndef _WIN32_WINDOWS          // Especifica que la plataforma m�nima requerida es Windows 98.
#define _WIN32_WINDOWS 0x0410 // Cambiar al valor apropiado correspondiente a Windows Me o posterior.
#endif

#ifndef _WIN32_IE                       // Especifica que la plataforma m�nima requerida es Internet Explorer 7.0.
#define _WIN32_IE 0x0700        // Cambiar al valor apropiado correspondiente a otras versiones de IE.
#endif
