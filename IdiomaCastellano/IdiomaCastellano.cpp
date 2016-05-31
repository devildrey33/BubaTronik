// Librería para el Idioma 'Castellano'

// Este archivo contiene las frases para el idioma castellano del reproductor BubaTronik.
// Si quieres traducir este archivo para crear otro idioma, deberás editar las frases que hay dentro de las macros TEXT("")
// Veras que hay varias líneas numeradas de la siguiente forma : 'case 1 : return TEXT("Archivo");'
// - El numero indica la ID de la frase, y la frase esta dentro de la macro TEXT(""), entonces si queremos traducir esa frase en ingles,
//   quedaría de la siguiente forma : 'case 1 : return TEXT("File");'
// - También debemos tener en cuenta que existen caracteres que son 'especiales' para el reproductor, y no deben ser modificados 
//   ya que podrían originar errores graves. A continuación podéis ver una lista de los caracteres especiales explicados :
//--------------------------------------------------------------------------------------------------------------------------------------------
//   $  - Este carácter delimita una frase pulsable, es decir si tenemos TEXT("Hola pulsa $aquí$") el reproductor interpretara que la 
//        palabra 'aquí' es pulsable y la enlazara a un comando para ejecutarla internamente cuando se requiera.
//--------------------------------------------------------------------------------------------------------------------------------------------
//   #  - Este carácter delimita una frase con una parte de texto resaltada, ya sea por negrita, cursiva, etc..
//--------------------------------------------------------------------------------------------------------------------------------------------
//   %d - Esta combinación de caracteres simboliza un digito decimal, también nos podemos encontrar combinaciones como %02d, etc...
//		  que deben ser tratadas igual. Por ejemplo : TEXT("Total de canciones : %d")	= 'Total de canciones 100'
//--------------------------------------------------------------------------------------------------------------------------------------------
//   %f - Esta combinación de caracteres simboliza un digito decimal,, también nos podemos encontrar combinaciones como %.02f, etc...
//		  que deben ser tratadas igual. Por ejemplo : TEXT("Nota media del disco : %.02f") = 'Nota media del disco : 4.60'
//--------------------------------------------------------------------------------------------------------------------------------------------
//	 %s - Esta combinación de caracteres simboliza una frase, por ejemplo :	TEXT("Lista actual : %s") = 'Lista actual : Disco Iron Maiden'
//--------------------------------------------------------------------------------------------------------------------------------------------
//   \n - Esta combinación indica que hay un salto de línea, por ejemplo : TEXT("Línea 1\nLínea 2") = 'Línea 1
//																									   Línea 2'
//--------------------------------------------------------------------------------------------------------------------------------------------
//   \0 - Esta combinación indica que se ha terminado la cadena, puede que encuentres mas cadenas detrás
//--------------------------------------------------------------------------------------------------------------------------------------------

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#define VERSION			0.5f
#define NOMBRE_IDIOMA	TEXT("Castellano")       // Nombre del Idioma
//#define PATH_AYUDA		TEXT("ayuda\\ayuda.htm") // Nombre del archivo de ayuda


extern "C" _declspec(dllexport) const TCHAR *TextoIdioma(const int Num) {
	switch (Num) {
		case 0	   : return TEXT("Error cargando el idioma.\nLa librería no tiene la misma versión que el reproductor\n%s\nVersión actual : %02f, Versión Requerida : %02f\nPara solucionar el problema, reinstale el reproductor.");
		case 1     : return TEXT("Abrir Archivo");
		case 2     : return TEXT("Base de Datos");
		case 3     : return TEXT("Ecualizador");
		case 4     : return TEXT("Ver Video");
		case 5     : return TEXT("Lista de reproducción");
		case 6     : return TEXT("Buscar Archivos");
		case 7     : return TEXT("Opciones");
		case 8     : return TEXT("Ayuda");
		case 9     : return TEXT("Teclas rápidas");
		case 10    : return TEXT("Asociar Archivos");
		case 11    : return TEXT("Lista");
		case 12    : return TEXT("Aspecto");
		case 13    : return TEXT("Buscar Archivos");
		case 14    : return TEXT("Cancelar");
		case 15    : return TEXT("Canción Atrás");
		case 16    : return TEXT("Play");
		case 17    : return TEXT("Pausa");
		case 18    : return TEXT("Stop");
		case 19    : return TEXT("Canción Adelante");
		case 20    : return TEXT("Activado");
		case 21    : return TEXT("Desactivado");
		case 22    : return TEXT("Shufle");
		case 23    : return TEXT("Repeat");
		case 24    : return TEXT("Este reproductor no está asignado para reproducir canciones y videos por defecto en windows.\nDeseas que este reproductor se use por defecto?");
		case 25    : return TEXT("La ruta especificada no es válida!!");
		case 26    : return TEXT("Error!");
		case 27    : return TEXT("Editar Anomalías");
		case 28    : return TEXT("Asignar Nombre");
		case 29    : return TEXT("Modificar Nombre");
		case 30    : return TEXT("Eliminar");
		case 31    : return TEXT("Inicio");
		case 32	   : return TEXT("Revisar medios");
		case 33	   : return TEXT("Video");
		case 34	   : return TEXT("Tamaño");
		case 35	   : return TEXT("Repetir lista");
		case 36	   : return TEXT("Repetir lista + shufle");
		case 37	   : return TEXT("Genero aleatorio");
		case 38	   : return TEXT("Grupo aleatorio");
		case 39	   : return TEXT("Disco aleatorio");
		case 40	   : return TEXT("50 canciones aleatorias");
		case 41	   : return TEXT("Apagar windows");
		case 42	   : return TEXT("Apagar este reproductor");
		case 43	   : return TEXT("Buscar archivos a agregar");
		case 44	   : return TEXT("Revisar la base de datos");
		case 45	   : return TEXT("Archivo");
		case 46	   : return TEXT("Nueva Lista");
		case 47	   : return TEXT("Lista Vacía");
		case 48	   : return TEXT("Canciones con 5 estrellas");
		case 49	   : return TEXT("Canciones más escuchadas");
		case 50	   : return TEXT("Añadir a lista");
		case 51	   : return TEXT("Guardar Lista");
		case 52	   : return TEXT("Cargar archivo de Subtítulos");
		case 53	   : return TEXT("Restaurar");
		case 54	   : return TEXT("Minimizar");
		case 55	   : return TEXT("Cerrar");
		case 56	   : return TEXT("Añadir a una nueva lista");
		case 57	   : return TEXT("Modificar");
		case 58	   : return TEXT("Nota");
		case 59	   : return TEXT("5 Estrellas");
		case 60	   : return TEXT("4 Estrellas");
		case 61	   : return TEXT("3 Estrellas");
		case 62	   : return TEXT("2 Estrellas");
		case 63	   : return TEXT("1 Estrella");
		case 64	   : return TEXT("Orden de las canciones");
		case 65	   : return TEXT("Tipo, Genero, Grupo, Disco, Canciones");
		case 66	   : return TEXT("Genero, Grupo, Disco, Canciones");
		case 67	   : return TEXT("Grupo, Disco, Canciones");
		case 68	   : return TEXT("Disco, Canciones");
		case 69	   : return TEXT("Propiedades");
		case 70	   : return TEXT("Buscar canción en la base de datos");
		case 71	   : return TEXT("Modificar pista");
		case 72	   : return TEXT("Tiempo");
		case 73	   : return TEXT("Copiar canciones a...");
		case 74	   : return TEXT("Canción actual");
		case 75	   : return TEXT("Buscar canción en la lista");
		case 76	   : return TEXT("Eliminar de la base de datos");
		case 77	   : return TEXT("Eliminar de la lista");
		case 78	   : return TEXT("Aleatorio");
		case 79	   : return TEXT("Modificada");
		case 80	   : return TEXT("Agregar");
		case 81	   : return TEXT("Generar");
		case 82	   : return TEXT("Error cargando el archivo (%s)\nEste error no es grave y el reproductor puede continuar la ejecución del programa.\nReinstala el reproductor para solucionar el problema");
		case 83    : return TEXT("Analizando :");
		case 84    : return TEXT("Archivos de Subtítulos\0*.srt\0");
		case 85    : return TEXT("Abrir Subtítulos");
		case 86    : return TEXT("Pequeña");
		case 87    : return TEXT("Mediana");
		case 88    : return TEXT("Grande");
		case 89	   : return TEXT("Pista");
		case 90    : return TEXT("Path");
		case 91    : return TEXT("Tipo");
		case 92    : return TEXT("Genero");
		case 93    : return TEXT("Grupo");
		case 94    : return TEXT("Disco");
		case 95    : return TEXT("Escuchada");
		case 96    : return TEXT("%d veces");
		case 97    : return TEXT("SIN DATOS");
		case 98    : return TEXT("Datos del tag versión 1");
		case 99    : return TEXT("Datos del tag versión 2");
		case 100   : return TEXT("Datos del tag ASF");
		case 101   : return TEXT("El total de valores encontrados en los tags es de : %d");
		case 102   : return TEXT("Géneros");
		case 103   : return TEXT("Grupos");
		case 104   : return TEXT("Discos");
		case 105   : return TEXT("Canciones");
		case 106   : return TEXT("Presiona la nueva tecla para esta acción");
		case 107   : return TEXT("Editar");
		case 108   : return TEXT("Control + ");
		case 109   : return TEXT("Alt + ");
		case 110   : return TEXT("Shif + ");
		case 111   : return TEXT("Combinación de teclas INVALIDA.");
		case 112   : return TEXT("Aceptar");
		case 113   : return TEXT("No mostrar mas este mensaje");
		case 114   : return TEXT("Buscar");
		case 115   : return TEXT("Abrir su directorio");
		case 116   : return TEXT("Mostrar log");
		case 117   : return TEXT("Sobre BubaTronik...");
		case 118   : return TEXT("Medios reproducibles");
		case 119   : return TEXT("Archivos de audio");
		case 120   : return TEXT("Archivos de video");
		case 121   : return TEXT("Listas");
		case 122   : return TEXT("Todos los archivos");
		case 123   : return TEXT("Deseas eliminar también los medios del disco duro?\nPresiona omitir para eliminarlos solo de la base de datos.");
		case 124   : return TEXT("Eliminar medios");
		case 125   : return TEXT("Omitir");
		case 126   : return TEXT("Ayuda rápida");
		case 127   : return TEXT("Audio");
		case 128   : return TEXT("CDAudio");
		case 129   : return TEXT("Selecciona el tipo de norma");
		case 130   : return TEXT("Lista de canciones");
		case 131   : return TEXT("Revisión de la Base de datos.");
		case 132   : return TEXT("\\Genero\\Grupo\\Disco\\");
		case 133   : return TEXT("\\Grupo\\Disco\\");
		case 134   : return TEXT("\\Disco\\");
		case 135   : return TEXT("Medio Siguiente");
		case 136   : return TEXT("Medio Anterior");
		case 137   : return TEXT("Grupo desconocido");
		case 138   : return TEXT("Disco desconocido");
		case 139   : return TEXT("CD de Audio (%s)");
		case 140   : return TEXT("Genero seleccionado : #%s#");
		case 141   : return TEXT("#Modificada#");
		case 142   : return TEXT("Genero aleatorio #%s#");
		case 143   : return TEXT("Genero sugerido #%s#");
		case 144   : return TEXT("Grupo aleatorio #%s#");
		case 145   : return TEXT("Grupo sugerido #%s#");
		case 146   : return TEXT("Disco aleatorio #%s#");
		case 147   : return TEXT("Disco sugerido #%s#");
		case 148   : return TEXT("#%d Canciones aleatorias#");
		case 149   : return TEXT("#Canciones con 5 estrellas#");
		case 150   : return TEXT("#Las %d canciones mas escuchadas#");
		case 151   : return TEXT("Genero mas escuchado #%s#");
		case 152   : return TEXT("Grupo mas escuchado #%s#");
		case 153   : return TEXT("Disco mas escuchado #%s#");
		case 154   : return TEXT("Error grave!!");
		case 155   : return TEXT("Enviar");
		case 156   : return TEXT("Terminar");
		case 157   : return TEXT("La operación se ha completado correctamente.");
		case 158   : return TEXT("Lista M3U\0*.m3u\0\0");
//		case 159   : return TEXT("Guardar Lista");
		case 160   : return TEXT("El archivo %s ya existe, deseas sobre escribirlo?");
		case 161   : return TEXT("Ya existe un archivo con ese nombre!");
		case 162   : return TEXT("No se ha podido guardar el archivo, comprueba que tengas los privilegios suficientes en la ruta.");
		case 163   : return TEXT("Error guardando la lista!");
		case 164   : return TEXT("La lista se guardo en : %s");
		case 165   : return TEXT("Lista guardada!");
		case 166   : return TEXT("Cursor Adelante");
		case 167   : return TEXT("Cursor Atrás");
		case 168   : return TEXT("Cursor Izquierdo");
		case 169   : return TEXT("Cursor Derecho");
		case 170   : return TEXT("Espacio");
		case 171   : return TEXT("Retroceso");
		case 172   : return TEXT("Insert");
		case 173   : return TEXT("Suprimir");
//		case 174   : return TEXT("Inicio");
		case 175   : return TEXT("Fin");
		case 176   : return TEXT("Esc"); 
		case 177   : return TEXT("Shift");
		case 178   : return TEXT("Imprimir Pantalla");
		case 179   : return TEXT("Archivo de audio");
		case 180   : return TEXT("Archivo de vídeo");
		case 181   : return TEXT("Numeración");
		case 182   : return TEXT("Ancho");
		case 183   : return TEXT("Alto"); 
		case 184   : return TEXT("Datos del tag Vorbis :");

		// ControlPresentacionEx::Mostrar_Inicio
		case 1000  : return TEXT("Bienvenido a %s %s");
		case 1001  : return TEXT("Lista actual : %s");
		case 1002  : return TEXT("No hay sugerencias para la base de datos.");
		case 1003  : return TEXT("Numero de canciones #%d# , Tiempo total de la lista #%s#.");
		case 1004  : return TEXT("%d Géneros, %d Grupos, %d Discos, %d Canciones y %d Videos.");
		case 1005  : return TEXT("Genero mas escuchado $%s$ con un total de #%d# veces.");
		case 1006  : return TEXT("Grupo mas escuchado $%s$ con un total de #%d# veces.");
		case 1007  : return TEXT("Disco mas escuchado $%s$ con un total de #%d# veces.");
		case 1008  : return TEXT("Sugerencias : $%s$ y $%s$");
		case 1009  : return TEXT("Sin sugerencias.");
		case 1010  : return TEXT("Generador de listas :");
		case 1011  : return TEXT("Lista con 50 canciones basada en un genero aleatorio");
		case 1012  : return TEXT("Lista con 50 canciones basada en un grupo aleatorio");
		case 1013  : return TEXT("Lista con 50 canciones basada en un disco aleatorio");
		case 1014  : return TEXT("Lista con 50 canciones aleatorias");
		case 1015  : return TEXT("Lista con las canciones que tienen 5 estrellas");
		case 1016  : return TEXT("Lista con las 50 canciones mas escuchadas");
		case 1017  : return TEXT("Medios abiertos recientemente :");
		// ControlPresentacionEx::Mostrar_CargandoVLC
		case 1020  : return TEXT("Cargando núcleo de la LIBVLC %s ....");
		// ControlPresentacionEx::Mostrar_SobreBubaTronik
		case 1030  : return TEXT("BubaTronik es una aplicación que nació ante la necesidad de ordenar y reproducir medios fácilmente. Además es el ejemplo mas completo que existe sobre la librería DWL.");
		case 1031  : return TEXT("$BubaTronik$, la $DWL$ y $www.devildrey33.es$ son propiedad de #Jose Antonio Bover Comas#.");
		case 1032  : return TEXT("Si deseáis poneros en contacto con el autor podéis hacerlo enviando un mensaje en el $foro$ de www.devildrey33.es.");
		case 1033  : return TEXT("Hay mucha gente que ha colaborado activa y pasivamente en este proyecto cosa que debo agradecer. En especial las siguientes personas han contribuido ultimamente en el desarrollo :");
		case 1034  : return TEXT("- #Barba# Siempre da buenas opiniones para problemas poco comunes.");
		case 1035  : return TEXT("- #Fong# Aportación de varias ideas y una valiosa cobaya.");
		case 1036  : return TEXT("- #Michel# Aportación de varias ideas y una valiosa cobaya.");
		case 1037  : return TEXT("- #Parris# Otra valiosa cobaya.");
		case 1038  : return TEXT("- $Txema$ Otra valiosa cobaya.");
		case 1039  : return TEXT("- También tengo que agradecer a toda la gente que me ha dado apoyo y soporte durante este proyecto. #Gracias a TOD@S#.");
		case 1040  : return TEXT("Por último también debo mencionar que este reproductor utiliza otras 2 librerías para cumplir su cometido, y son :");
		case 1041  : return TEXT("#FMOD EX# 4.26 Potente librería de audio utilizada en muchos juegos actuales.");
		case 1042  : return TEXT("Puedes encontrarla en el siguiente enlace : $http://www.fmod.org$");
		case 1043  : return TEXT("#LIBVLC# %s Librería para reproducir prácticamente cualquier medio.");
		case 1044  : return TEXT("Puedes encontrarla en el siguiente enlace : $http://www.videolan.org$");
		case 1045  : return TEXT("Datos adicionales");
		case 1046  : return TEXT("BubaTronik #%.03f# #%s# compilado con #MSVC %d# el %s.");
		case 1047  : return TEXT("Versión de windows #%d.%d# Service Pack #%d.%d#");
		// ControlPresentacionEx::Mostrar_Opciones_Lista
		case 1050  : return TEXT("Lista inicial");
		case 1051  : return TEXT("- Elige que debe hacer el reproductor si se inicia con una lista vacía. Por defecto se iniciara con una lista vacía, si no se le pasa ningun parametro.");
		case 1052  : return TEXT("- #NOTA# : si eliges la opción 'Cualquier cosa', el reproductor seleccionara una de las opciones de la lista aleatoriamente, #exceptuando# la opción '#Lista Vacía#'");
		case 1053  : return TEXT("Cualquier cosa.");
		case 1054  : return TEXT("Excluir canciones");
		case 1055  : return TEXT("- Esta opción te permite excluir las canciones que tengan una estrella de todas las listas aleatorias que genera el reproductor, de esta forma esas canciones no se agregaran a ninguna lista.");
		case 1056  : return TEXT("Excluir canciones con una estrella.");
		// ControlPresentacionEx::Mostrar_Opciones_BD
		case 1060  : return TEXT("Mostrar tags");
		case 1061  : return TEXT("- Si activas esta opción, en los tooltips informativos se mostraran todos los datos del tag.");
		case 1062  : return TEXT("Mostrar Tags en los Tooltips.");
		// ControlPresentacionEx::Mostrar_Opciones_Video
		case 1070  : return TEXT("Opciones de Video");
		case 1071  : return TEXT("- Esta opción ocultara el mouse, cuando el reproductor este en modo pantalla completa, y el usuario este mas de 2 segundos sin hacer nada.");
		case 1072  : return TEXT("Ocultar mouse en pantalla completa.");
		// ControlPresentacionEx::Mostrar_Opciones_Apariencia
		case 1080  : return TEXT("Tamaño de la fuente");
		case 1081  : return TEXT("- Selecciona el tamaño de la fuente que usara el reproductor.");
		case 1082  : return TEXT("Configuración de colores");
		case 1083  : return TEXT("- Elige la configuración de colores que prefieras para el reproductor.");
		case 1084  : return TEXT("#NOTA :# Puedes crear/editar una configuración de colores, editando los archivos con extensión '.BubaTronik_Skin' y modificando sus valores RGB para los colores. Estos archivos los encontraras en la subcarpeta 'Gráficos' situada en la raiz del reproductor. Para mas información edita un archivo, y lee las instrucciones que hay dentro.");
		case 1085  : return TEXT("Tamaño del espectro gráfico");
		case 1086  : return TEXT("- Selecciona el tamaño de las barras para el espectro grafico del sonido.");
		case 1087  : return TEXT("Barra de titulo");
		case 1088  : return TEXT("- Utilizar barra de titulo de windows (recomendado en windows vista y superiores).");
		case 1089  : return TEXT("Utilizar barra de titulo de windows.");
		// ControlPresentacionEx::Mostrar_Opciones
		case 1090  : return TEXT("Idioma");
		case 1091  : return TEXT("- Elige que idioma prefieres que se muestre en el reproductor. Por defecto se elige el mismo que el sistema operativo si esta disponible.");
		case 1092  : return TEXT("Actualizaciones");
		case 1093  : return TEXT("- Buscar en internet una nueva versión del reproductor al iniciar. En caso de encontrar una nueva versión, se mostrara un mensaje que te permitirá descargar e instalar la nueva actualización.");
		case 1094  : return TEXT("Buscar Actualizaciones");
		case 1095  : return TEXT("Reproductor siempre delante");
		case 1096  : return TEXT("- Esta opción te permite tener por delante de todas las otras ventanas la ventana del reproductor.");
		case 1097  : return TEXT("Tipos de archivo");
		case 1098  : return TEXT("- Los tipos de archivo que se ven a continuación son los tipos que usara el reproductor tanto para buscar archivos, como para asociarse a esos tipos para reproducirlos por defecto. Puedes borrar los tipos que no te interesen.");
		case 1099  : return TEXT("Eliminar audio");
		case 1100  : return TEXT("Eliminar video");
		case 1101  : return TEXT("Eliminar seleccionados");
		case 1102  : return TEXT("Restaurar tipos");
		case 1103  : return TEXT("Asociar tipos");
		case 1104  : return TEXT("- Esta opción te permite asociar los tipos de archivo de la lista anterior al reproductor. De esta forma cuando hagas dobleclick en el explorador a un archivo que corresponda a alguno de los tipos aceptados se encenderá este reproductor por defecto.");
		case 1105  : return TEXT("Preguntar al iniciar, cuando sea necesario.");
		case 1106  : return TEXT("Siempre.");
		case 1107  : return TEXT("Nunca.");
		case 1108  : return TEXT("- Selecciona como quieres que se ejecuten los archivos por defecto.");
		case 1109  : return TEXT("Añadir a la lista.");
		case 1110  : return TEXT("Reproducir.");
		// ControlPresentacionEx::Mostrar_Opciones_Teclado
		case 1120  : return TEXT("Teclas de acceso rápido");
		case 1121  : return TEXT("- Puedes cambiar las teclas para cualquier acción de las que se listan a continuación. Es posible combinar las teclas Control y Shift con otras teclas sin ningún problema. También se puedes usar algunas combinaciones con la tecla Alt, siempre que la tecla control este pulsada, aunque no es muy recomendable porque muchas aplicaciones desplegaran menús emergentes.");
		case 1122  : return TEXT("Play / Pausa");
		case 1123  : return TEXT("Subir Volumen");
		case 1124  : return TEXT("Bajar Volumen");
		case 1125  : return TEXT("Reproducir medio siguiente");
		case 1126  : return TEXT("Reproducir medio anterior");
		case 1127  : return TEXT("Obtener teclado fuera de la aplicación");
		case 1128  : return TEXT("- Marca esta opción para que el reproductor detecte las teclas de acceso rápido incluso bajo un juego. #NOTA#, si marcas esta opción es recomendable que te asegures de que las combinaciones de teclas no se utilicen en otros programas / juegos.");
		case 1129  : return TEXT("Obtener teclado cuando el reproductor no este activo.");
		// NOTA DEJAMOS 10 ESPACIOS MAS, PORQUE ES PROBABLE QUE ACABE MODIFICANDO ESTA ESCENA
		// ControlPresentacionEx::Mostrar_RevisarBD 
		case 1140  : return TEXT("Desde este panel puedes revisar la base de datos para determinar si se han borrado canciones de los discos.");
		case 1141  : return TEXT("Es importante que revises la base de datos cuando borres canciones desde fuera del reproductor, ya que después el reproductor podria incluirlas en listas aleatorias.");
		case 1142  : return TEXT("Por último también puedes hacer una revisión para determinar que géneros/grupos/discos se parecen entre si.");
		case 1143  : return TEXT("Revisar anomalías en géneros / grupos / discos");
		// ControlPresentacionEx::Mostrar_BuscarArchivos
		case 1150  : return TEXT("El reproductor buscara nuevos medios reproducibles en los directorios que se muestran en la lista de abajo. Puedes agregar o quitar directorios a tu gusto de dicha lista siempre que como mínimo quede uno.");
		case 1151  : return TEXT("(Path)");
		case 1152  : return TEXT("(Tag)");
		case 1153  : return TEXT("PATH\\Genero\\Grupo\\Disco\\");
		case 1154  : return TEXT("PATH\\Grupo\\Disco\\");
		case 1155  : return TEXT("PATH\\Disco\\");
		case 1156  : return TEXT("Indefinido");
		case 1157  : return TEXT("Selecciona un directorio");
		case 1158  : return TEXT("Sugerencias");
		case 1159  : return TEXT("Selecciona que tipo de norma deberías utilizar para la estructuración del directorio que has elegido.");
		case 1160  : return TEXT("1 - \\Genero\\Grupo\\Disco\\ (Escaneo por Path)");
		case 1161  : return TEXT("2 - \\Grupo\\Disco\\ (Escaneo por Path)");
		case 1162  : return TEXT("3 - \\Disco\\ (Escaneo por Tag)");
		case 1163  : return TEXT("4 - Un poco de todo / Indefinido (Escaneo por Tag)");
		case 1164  : return TEXT("Path antes del Tag");
		case 1165  : return TEXT("Tag antes del Path");
		case 1166  : return TEXT("Este proceso puede tardar varios minutos dependiendo del ordenador y el numero de archivos reproducibles que se encuentren.");
		case 1167  : return TEXT("Filtrado de caracteres");
		case 1168  : return TEXT("Análisis post-escaneo");



		// Mensajes
		case 4000  : return TEXT("Error la unidad que contiene el medio no se encuentra disponible,\npor ello es imposible reproducir el medio.");
		case 4001  : return TEXT("Revisión terminada, se han eliminado un total de %d entradas no validas");
		case 4002  : return TEXT("Error la ruta introducida no es válida, o ya existe en la lista.");
		case 4003  : return TEXT("Deseas eliminar el M3U del disco? Esto no afectara a los medios que contiene el M3U.");
		case 4004  : return TEXT("BubaTronik ha encontrado un error critico y además no ha podido crear un informe de errores.....");
		case 4005  : return TEXT("La versión de la base de datos no es válida para este reproductor.");
		case 4006  : return TEXT("La base de datos parece estar dañada!");
		case 4007  : return TEXT("Hay %d medios que no se han cargado debido a que su información podría ser invalida y necesitan ser analizados de nuevo.\nSe recomienda ejecutar la búsqueda de archivos para volver a tener esos medios en la base de datos.");
		case 4008  : return TEXT("Ignorada carga de algunos medios");
		case 4009  : return TEXT("Ha sucedido un error grave en BubaTronik y este debe cerrarse.");
		case 4010  : return TEXT("Se ha creado un informe de errores, deseas enviarlo a www.devildrey33.es?");
		case 4011  : return TEXT("Pulsa el botón 'Enviar' para enviar el mensaje y terminar, o  pulsa el botón");
		case 4012  : return TEXT("'Terminar' para que no se mande el mensaje.");
		case 4013  : return TEXT("Información");
		case 4014  : return TEXT("Informe de errores enviado correctamente.");
		case 4015  : return TEXT("Error enviando el informe de errores....");
		case 4016  : return TEXT("Se ha encontrado una nueva actualización de BubaTronik, deseas descargarla?? \n%s");
		case 4017  : return TEXT("Nueva actualización!");
		case 4018  : return TEXT("El reproductor se apagara para actualizarse, y se volverá a encender una vez terminada la actualización.\nPresiona intro para continuar.");
		case 4019  : return TEXT("Actualizar!");
		case 4020  : return TEXT("Error descargando la actualización, puedes intentarlo mas tarde, \no puedes probar a bajar la actualización manualmente desde http://bubatronik.devildrey33.es");
		case 4021  : return TEXT("Empezando actualización");
		case 4022  : return TEXT("Error de descarga");
		case 4023  : return TEXT("Error cargando Winsock.");
		case 4024  : return TEXT("Error obteniendo la IP del servidor.");
		case 4025  : return TEXT("Error socket invalido .");
		case 4026  : return TEXT("Error conectando al servidor.");
		case 4027  : return TEXT("Error durante la transmisión de datos.");
		case 4028  : return TEXT("Error!! la versión del skin no es válida para este reproductor.");
		case 4029  : return TEXT("Error cargando archivo de skin (%s).\nNo hay suficientes colores, y se ha cargado una configuración por defecto.\nEste error no es grave, y se puede continuar con la ejecución del programa.\nReinstala el reproductor para solucionar el problema.");
		case 4030  : return TEXT("No puedes añadir medios externamente mientras se esta realizando una busqueda.");

		// ToolTips :
		// -ID_GENERAR_LISTA
		case 5000  : return TEXT("Debes elegir un tipo de lista para poder generarla.");
		case 5001  : return TEXT("Al presionar este botón, crearas una nueva lista");
		case 5002  : return TEXT("de unas 50 canciones del tipo elegido.");
		// -ID_TIPOS_LISTA
		case 5010  : return TEXT("Este control te permite elegir entre varios tipos de listas prefabricadas :");
		case 5011  : return TEXT("- Lista de canciones aleatorias basadas en el genero.");
		case 5012  : return TEXT("- Lista de canciones aleatorias basadas en el grupo.");
		case 5013  : return TEXT("- Lista de canciones aleatorias basadas en el disco.");
		case 5014  : return TEXT("- Lista de canciones aleatorias sin basarse en nada.");
		case 5015  : return TEXT("- Lista con las canciones mas escuchadas (si se ha escuchado alguna)");
		case 5016  : return TEXT("- Lista con canciones de 5 estrellas (si se ha asignado alguna)");
		case 5017  : return TEXT("NOTA : Estas listas como máximo podrán tener 50 canciones.");
		// -CP_SUGERENCIA_GENERO1 -CP_SUGERENCIA_GENERO2 -CP_SUGERENCIA_GRUPO1 -CP_SUGERENCIA_GRUPO2 -CP_SUGERENCIA_DISCO1 -CP_SUGERENCIA_DISCO2
		case 5020  : return TEXT("Cada vez que accedas a la sección de inicio se mostraran nuevas sugerencias.");
		// -CP_SUGERENCIA_GENERO1 -CP_SUGERENCIA_GENERO2 -CP_GENERO_MAS
		case 5021  : return TEXT("Haz click para crear una nueva lista con varias canciones del género :");
		// -CP_SUGERENCIA_GRUPO1 -CP_SUGERENCIA_GRUPO2 -CP_GRUPO_MAS
		case 5022  : return TEXT("Haz click para crear una nueva lista con varias canciones del grupo :");
		// -CP_SUGERENCIA_DISCO1 -CP_SUGERENCIA_DISCO2 -CP_DISCO_MAS
		case 5023  : return TEXT("Haz click para crear una nueva lista con varias canciones del disco :");
		// -CP_GENERO_MAS
		case 5024  : return TEXT("Para determinar que genero se ha escuchado se hace una lista con todos los géneros,");
		case 5025  : return TEXT("en la que se suman las veces que se ha escuchado cada canción de ese género.");
		// -CP_GRUPO_MAS
		case 5026  : return TEXT("Para determinar que grupo se ha escuchado se hace una lista con todos los grupos,");
		case 5027  : return TEXT("en la que se suman las veces que se ha escuchado cada canción de ese grupo.");
		// -CP_DISCO_MAS
		case 5028  : return TEXT("Para determinar que disco se ha escuchado se hace una lista con todos los discos,");
		case 5029  : return TEXT("en la que se suman las veces que se ha escuchado cada canción de ese disco.");
		// -ID_FILTRO_COINCIDENCIAS
		case 5030  : return TEXT("Activando esta opción el reproductor realizara una comparación entre todos sus géneros, grupos, y discos");
		case 5031  : return TEXT("de forma que solucione el 80% de erratas en los tags.");
		case 5032  : return TEXT("Pongamos por ejemplo, que inicialmente se encuentran los siguientes dos grupos :");
		case 5033  : return TEXT("'IronMaiden' y ' Iron Maiden'");
		case 5034  : return TEXT("Si os fijáis son prácticamente lo mismo pero uno tiene un espacio en medio, y el otro no.");
		case 5035  : return TEXT("Una cosa tan simple como esta hace que el reproductor interprete esos resultados como 2 grupos distintos.");
		case 5036  : return TEXT("Y por eso se ha creado esta opción, para intentar corregir al máximo este tipo de errores.");
		case 5037  : return TEXT("NOTA :");
		case 5038  : return TEXT("Activando esta opción, el tiempo total de la búsqueda de archivos se incrementara notablemente.");
		// -ID_FILTRO_CARACTERES
		case 5040  : return TEXT("Activando esta opción el reproductor filtrara de todos los nombres los siguientes caracteres :");
		case 5041  : return TEXT(": \\ / ' \" { } [ ] ? ¿ ; % # @ ^ además de las tildes y signos de puntuación.");
		case 5042  : return TEXT("Con esto conseguimos mucha mas precisión a la hora de identificar géneros, grupos y discos.");
		case 5043  : return TEXT("Es muy recomendable dejar activada esta opción, además de que no afectara al tiempo de la búsqueda.");
		// -ID_BOTON_BUSCAR2
		case 5050  : return TEXT("Presiona este botón para empezar la búsqueda.");
		case 5051  : return TEXT("Antes de hacerlo deberías asegurarte de que has añadido las rutas que deseas buscar en la lista.");
		case 5052  : return TEXT("Presiona este botón para cancelar la búsqueda.");
		case 5053  : return TEXT("Para buscar primero tienes que agregar una ruta a la lista, y elegir el tipo de orden.");
		// -ID_BOTON_AGREGAR_DIR
		case 5060  : return TEXT("Al pulsar este botón añadirás el directorio del cuadro desplegable a la lista de directorios.");
		case 5061  : return TEXT("No es recomendable añadir la raíz de ningún disco duro, exceptuando el caso de que ese disco");
		case 5062  : return TEXT("sea únicamente para medios de audio y video.");
		// -ID_COMBO_DIR
		case 5070  : return TEXT("Desde este control desplegable puedes buscar directorios para añadir a la lista.");
		case 5071  : return TEXT("Los directorios de la lista de arriba son los que escaneara el reproductor para encontrar nuevos medios.");
		// -ID_DIRECTORIOS_BUSCAR
		case 5080  : return TEXT("Esta lista contiene los directorios en los que el reproductor buscara nuevos medios reproducibles.");
		case 5081  : return TEXT("Por defecto se han añadido las rutas predeterminadas de windows, si deseas eliminarlas puedes usar");
		case 5082  : return TEXT("el botón con una cruz roja situado en la parte inferior derecha de esta lista.");
		case 5083  : return TEXT("Si quieres agregar directorios debes utilizar el control desplegable que hay mas abajo, para seleccionar");
		case 5084  : return TEXT("el directorio que quieras, y por último debes presionar el botón agregar para insertarlo en esta lista.");
		// -ID_LISTA_IDIOMAS
		case 5090  : return TEXT("Esta lista contiene los idiomas disponibles para este reproductor.");
		case 5091  : return TEXT("Si estas interesado en agregar un nuevo idioma a esta aplicación ponte en contacto con devildrey33");
		case 5092  : return TEXT("a través del foro de http://www.devildrey33.es");
		// -ID_BUSCAR_ACTUALIZACIONES
		case 5100  : return TEXT("Activado esta opción el reproductor se conectara una vez al día como máximo para comprobar si existen");
		case 5101  : return TEXT("nuevas actualizaciones por instalar.");
		// -ID_SIEMPRE_DELANTE
		case 5110  : return TEXT("Activado esta opción el reproductor se quedara siempre delante de las otras ventanas.");
		// -ID_LISTA_TIPOS
		case 5120  : return TEXT("Esta lista contiene todos los tipos de audio y de video que el reproductor acepta por defecto.");
		case 5121  : return TEXT("Puedes eliminar aquellas extensiones que no te interese que el reproductor acepte.");
		// -ID_BOTON_ELIMINAR_TIPOS_AUDIO
		case 5130  : return TEXT("Este botón eliminara de la lista todos los tipos que sean predefinidos para audio.");
		// -ID_BOTON_ELIMINAR_TIPOS_VIDEO
		case 5140  : return TEXT("Este botón eliminara de la lista todos los tipos que sean predefinidos para video.");
		// -ID_BOTON_ELIMINAR_TIPOS
		case 5150  : return TEXT("Este botón eliminara de la lista todos los tipos que se encuentren seleccionados en este momento.");
		// -ID_BOTON_RESTAURAR_TIPOS
		case 5160  : return TEXT("Este botón restaurara todos los tipos que puede usar BubaTronik por defecto.");
		// -ID_ASOCIAR_PREGUNTAR
		case 5170  : return TEXT("Seleccionando esta opción, el reproductor mirara cada vez que se inicie si está asociado a los");
		case 5171  : return TEXT("tipos de la lista, en caso de no estar asociado a alguno de esos tipos el reproductor mostrara");
		case 5172  : return TEXT("un mensaje que te pedirá si deseas asociar el reproductor a los tipos de la lista.");
		// -ID_ASOCIAR_SIEMPRE
//		case 5180  : Es el mismo texto que el 5170
		case 5181  : return TEXT("tipos de la lista, en caso de no estar asociado a alguno de esos tipos el reproductor se asociara");
		case 5182  : return TEXT("a ellos automáticamente, sin preguntar nada.");
		// -ID_ASOCIAR_NUNCA
		case 5190  : return TEXT("Seleccionando esta opción, el reproductor omitirá escanear el registro para determinar si esta");
		case 5191  : return TEXT("asociado o no a los tipos de archivo de la lista.");
		// -ID_ASOCIAR_ADD
		case 5200  : return TEXT("Al ejecutar un archivo asociado al reproductor desde el explorardor, se añadirá a la lista sin");
		case 5201  : return TEXT("interrumpir la reproducción actual.");
		// -ID_ASOCIAR_REP
		case 5210  : return TEXT("Al ejecutar un archivo asociado al reproductor desde el explorardor, se añadirá a la lista");
		case 5211  : return TEXT("interrumpiendo la reproducción actual, y se pondrá a reproducir el primer medio añadido desde");
		case 5212  : return TEXT("el explorador.");
		// -ID_OPCIONES_FUENTE
		case 5220  : return TEXT("En esta lista puedes seleccionar el tamaño de la fuente que mas se ajuste a tus necesidades.");
		case 5221  : return TEXT("Hay tres tipos de tamaño : pequeño (11px), mediano (13px) y grande (15px).");
		// -ID_OPCIONES_SKIN
		case 5230  : return TEXT("En esta lista puedes seleccionar la configuración de colores que mas te guste.");
		case 5231  : return TEXT("Si deseas crear una configuración de colores para BubaTronik y no lo ves muy claro,");
		case 5232  : return TEXT("puedes consultar tus dudas en el foro de http://www.devildrey33.es");
		// -ID_OPCIONES_OSCY
		case 5240  : return TEXT("En esta lista puedes seleccionar el tamaño de las barritas del espectro grafico.");
		case 5241  : return TEXT("El pequeño hace 1 pixel de ancho, el mediano 2 pixeles, y el grande 3 pixeles.");
		// -ID_OPCIONES_UTILIZAR_BARRA_WIN
		case 5250  : return TEXT("Marcando esta opción el reproductor utilizara la barra de windows estándar con sus bordes.");
		case 5251  : return TEXT("En principio el reproductor se diseño para windows XP, por lo que se creo una barra de titulo");
		case 5252  : return TEXT("distinta a la de windows que quedaba mas acorde con el reproductor.");
		case 5253  : return TEXT("Si estas usando windows vista o windows 7 se recomienda utilizar la barra de titulo de windows");
		case 5254  : return TEXT("para no perder las capacidades extendidas de maximizado parcial.");
		// -ID_OPCIONES_MOSTRAR_TAG
		case 5260  : return TEXT("Marcando esta opción se cargaran al vuelo y se mostraran los tag de las canciones al mostrar");
		case 5261  : return TEXT("el tooltip informativo.");
		case 5262  : return TEXT("Con el cuarto botón del mouse puedes hacer aparecer esos tooltips dentro de la lista y de la");
		case 5264  : return TEXT("base de datos sin tener que esperar 2 segundos encima de la canción.");
		// -ID_LISTA_INICIO
		case 5270  : return TEXT("En esta lista desplegable puedes seleccionar que quieres que haga el reproductor al iniciarse :");
		case 5271  : return TEXT("- Crear una lista partiendo de un genero aleatorio con un máximo de 50 canciones.");
		case 5272  : return TEXT("- Crear una lista partiendo de un grupo aleatorio con un máximo de 50 canciones.");
		case 5273  : return TEXT("- Crear una lista partiendo de un disco aleatorio con un máximo de 50 canciones.");
		case 5274  : return TEXT("- Crear una lista aleatoria con un máximo de 50 canciones.");
		case 5275  : return TEXT("- (Cualquier cosa) Una de las 4 opciones anteriores aleatoriamente.");
		case 5276  : return TEXT("- Crear una lista vacía.");
		case 5277  : return TEXT("Si el reproductor se inicia desde el explorador porque has abierto un medio desde allí, la creación");
		case 5278  : return TEXT("de la lista aleatoria se omitirá.");
		// -ID_EXCLUIR_NOTA
		case 5280  : return TEXT("Marcando esta opción el reproductor evitara crear listas aleatorias con canciones que solo tengan una estrella.");
		case 5281  : return TEXT("El sistema de notas para los medios por defecto le da 2 estrellas a todos los medios. Si deseas que un medio");
		case 5282  : return TEXT("tenga mas nota (hasta 5 estrellas) puedes cambiarle la nota utilizando el menú desplegable desde la lista o la");
		case 5283  : return TEXT("base de datos.");
		// -ID_OPCIONES_TECLA_PLAY
		case 5290  : return TEXT("Desde este control puedes seleccionar la tecla que se usara para realizar la acción : Play / Pausa");
		// -ID_OPCIONES_TECLA_STOP
		case 5300  : return TEXT("Desde este control puedes seleccionar la tecla que se usara para realizar la acción : Stop");
		// -ID_OPCIONES_TECLA_VOLUMEN_MAS
		case 5310  : return TEXT("Desde este control puedes seleccionar la tecla que se usara para realizar la acción : Subir el volumen");
		// -ID_OPCIONES_TECLA_VOLUMEN_MENOS
		case 5320  : return TEXT("Desde este control puedes seleccionar la tecla que se usara para realizar la acción : Bajar el volumen");
		// -ID_OPCIONES_TECLA_SIGUIENTE
		case 5330  : return TEXT("Desde este control puedes seleccionar la tecla que se usara para realizar la acción : Siguiente medio");
		// -ID_OPCIONES_TECLA_ANTERIOR
		case 5340  : return TEXT("Desde este control puedes seleccionar la tecla que se usara para realizar la acción : Anterior medio");
		// -ID_OPCIONES_OCULTAR_MOUSE
		case 5350  : return TEXT("Marcando esta opción en el modo pantalla completa (dentro de un video) el mouse se ocultara");
		case 5351  : return TEXT("pasados 2 segundos sin hacer nada. Esto ocurrirá siempre que el mouse no este en la parte de los controles.");
		// -CP_URL_FMOD
		case 5360  : return TEXT("Página web de esta fabulosa librería para Audio");
		// -CP_URL_LIBVLC
		case 5370  : return TEXT("Página web de esta excelente librería para Audio Y Video");
		// -CP_URL_DEVILDREY33
		case 5380  : return TEXT("Página web de devildrey33");
		// -CP_URL_MALEIT
		case 5390  : return TEXT("Página web del grupo de este crack a la batería. MALEÏT");
		// -CP_URL_BUBATRONIK
		case 5400  : return TEXT("Página web principal de este reproductor");
		// -CP_URL_DWL
		case 5410  : return TEXT("Página web de la DReY Windows Lib");
		case 5411  : return TEXT("La DWL es una librería para hacer aplicaciones bajo windows en C++");
		case 5412  : return TEXT("que se distribuye bajo licencia GPL.");
		// -CP_URL_FORO
		case 5420  : return TEXT("Foro de http://www.devildrey33.es");
		// -ID_COMBO_ORDEN_DIR
		case 5430  : return TEXT("Estas normas se usan a la hora de obtener los datos de un medio, para determinar mejor sus datos.");
		case 5431  : return TEXT("Si por ejemplo tenemos el directorio 'C:\\MP3' y dentro tenemos música estructurada según 'Grupo\\Disco' podemos");
		case 5432  : return TEXT("encontrar el caso en que además exista un directorio para distintos cds, ejemplo :");
		case 5433  : return TEXT("C:\\MP3\\Baron rojo\\Baron al Rojo Vivo\\CD1");
		case 5434  : return TEXT("En este caso concreto, si estamos buscando datos basandonos en el path y no se ha elegido el tipo 'Grupo\\Disco'");
		case 5435  : return TEXT("el reproductor pensara que el nombre del grupo es 'Baron al Rojo Vivo' y el disco 'CD1' cosa que es incorrecta.");
		case 5436  : return TEXT("Primero debes seleccionar un directorio antes de poder establecer el tipo de norma a utilizar.");
		// -ID_MARCA_PATH
		case 5440  : return TEXT("Marcando esta opción el reproductor dará prioridad a los datos del path por encima de los del tag, para el directorio que se va a agregar.");
		case 5441  : return TEXT("Primero debes seleccionar un directorio antes de poder establecer la prioridad de los datos.");
		// -ID_MARCA_TAG
		case 5442  : return TEXT("Marcando esta opción el reproductor dará prioridad a los datos del tag por encima de los del path, para el directorio que se va a agregar.");
	}
	static TCHAR Error[512];
	wsprintf(Error, TEXT("(%d) NULL"), Num);
	return Error; // NO MODIFICAR, Este texto se retorna cuando le paso un numero invalido, y así me doy cuenta de que falta.
}


extern "C" _declspec(dllexport) const TCHAR *TextoEstilo(const int Num) {
	switch (Num) {
		case 0   : return TEXT("Blues");
		case 1   : return TEXT("Classic Rock");
		case 2   : return TEXT("Country");
		case 3   : return TEXT("Dance");
		case 4   : return TEXT("Disco");
		case 5   : return TEXT("Funk");
		case 6   : return TEXT("Grunge");
		case 7   : return TEXT("Hip-Hop");
		case 8   : return TEXT("Jazz");
		case 9   : return TEXT("Metal");
		case 10  : return TEXT("New Age");
		case 11  : return TEXT("Oldies");
		case 12  : return TEXT("Other");
		case 13  : return TEXT("Pop");
		case 14  : return TEXT("R&B");
		case 15  : return TEXT("Rap");
		case 16  : return TEXT("Reggae");
		case 17  : return TEXT("Rock");
		case 18  : return TEXT("Techno");
		case 19  : return TEXT("Industrial");
		case 20  : return TEXT("Alternative");
		case 21  : return TEXT("Ska");
		case 22  : return TEXT("Death Metal");
		case 23  : return TEXT("Pranks");
		case 24  : return TEXT("Soundtrack");
		case 25  : return TEXT("Euro-Techno");
		case 26  : return TEXT("Ambient");
		case 27  : return TEXT("Trip-Hop");
		case 28  : return TEXT("Vocal");
		case 29  : return TEXT("Jazz+Funk");
		case 30  : return TEXT("Fusion");
		case 31  : return TEXT("Trance");
		case 32  : return TEXT("Classical");
		case 33  : return TEXT("Instrumental");
		case 34  : return TEXT("Acid");
		case 35  : return TEXT("House");
		case 36  : return TEXT("Game");
		case 37  : return TEXT("Sound Clip");
		case 38  : return TEXT("Gospel");
		case 39  : return TEXT("Noise");
		case 40  : return TEXT("AlternRock");
		case 41  : return TEXT("Bass");
		case 42  : return TEXT("Soul");
		case 43  : return TEXT("Punk");
		case 44  : return TEXT("Space");
		case 45  : return TEXT("Meditative");
		case 46  : return TEXT("Instrumental Pop");
		case 47  : return TEXT("Instrumental Rock");
		case 48  : return TEXT("Ethnic");
		case 49  : return TEXT("Gothic");
		case 50  : return TEXT("Darkwave");
		case 51  : return TEXT("Techno-Industrial"); 
		case 52  : return TEXT("Electronic");
		case 53  : return TEXT("Pop-Folk");
		case 54  : return TEXT("Eurodance");
		case 55  : return TEXT("Dream");
		case 56  : return TEXT("Southern Rock");
		case 57  : return TEXT("Comedy");
		case 58  : return TEXT("Cult");
		case 59  : return TEXT("Gangsta");
		case 60  : return TEXT("Top 40");
		case 61  : return TEXT("Christian Rap");
		case 62  : return TEXT("Pop/Funk");
		case 63  : return TEXT("Jungle");
		case 64  : return TEXT("Native American");
		case 65  : return TEXT("Cabaret");
		case 66  : return TEXT("New Wave");
		case 67  : return TEXT("Psychadelic");
		case 68  : return TEXT("Rave");
		case 69  : return TEXT("Showtunes");
		case 70  : return TEXT("Trailer");
		case 71  : return TEXT("Lo-Fi");
		case 72  : return TEXT("Tribal");
		case 73  : return TEXT("Acid Punk");
		case 74  : return TEXT("Acid Jazz");
		case 75  : return TEXT("Polka");
		case 76  : return TEXT("Retro");
		case 77  : return TEXT("Musical");
		case 78  : return TEXT("Rock & Roll");
		case 79  : return TEXT("Hard Rock");
		// Extensiones del winamp
		case 80  : return TEXT("Folk");
		case 81  : return TEXT("Folk-Rock");
		case 82  : return TEXT("National Folk");
		case 83  : return TEXT("Swing");
		case 84  : return TEXT("Fast Fusion");
		case 85  : return TEXT("Bebob");
		case 86  : return TEXT("Latin");
		case 87  : return TEXT("Revival");
		case 88  : return TEXT("Celtic");
		case 89  : return TEXT("Bluegrass");
		case 90  : return TEXT("Avantgarde");
		case 91  : return TEXT("Gothic Rock");
		case 92  : return TEXT("Progressive Rock");
		case 93  : return TEXT("Psychedelic Rock");
		case 94  : return TEXT("Symphonic Rock");
		case 95  : return TEXT("Slow Rock");
		case 96  : return TEXT("Big Band");
		case 97  : return TEXT("Chorus");
		case 98  : return TEXT("Easy Listening");
		case 99  : return TEXT("Acoustic");
		case 100 : return TEXT("Humour");
		case 101 : return TEXT("Speech");
		case 102 : return TEXT("Chanson");
		case 103 : return TEXT("Opera");
		case 104 : return TEXT("Chamber Music");
		case 105 : return TEXT("Sonata");
		case 106 : return TEXT("Symphony");
		case 107 : return TEXT("Booty Bass");
		case 108 : return TEXT("Primus");
		case 109 : return TEXT("Porn Groove");
		case 110 : return TEXT("Satire");
		case 111 : return TEXT("Slow Jam");
		case 112 : return TEXT("Club");
		case 113 : return TEXT("Tango");
		case 114 : return TEXT("Samba");
		case 115 : return TEXT("Folklore");
		case 116 : return TEXT("Ballad");
		case 117 : return TEXT("Power Ballad");
		case 118 : return TEXT("Rhythmic Soul");
		case 119 : return TEXT("Freestyle");
		case 120 : return TEXT("Duet");
		case 121 : return TEXT("Punk Rock");
		case 122 : return TEXT("Drum Solo");
		case 123 : return TEXT("A capella");
		case 124 : return TEXT("Euro-House");
		case 125 : return TEXT("Dance Hall");
		case 126 : return TEXT("Goa");
		case 127 : return TEXT("Drum & Bass");
		case 128 : return TEXT("Club-House");
		case 129 : return TEXT("Hardcore");
		case 130 : return TEXT("Terror");
		case 131 : return TEXT("Indie");
		case 132 : return TEXT("Britpop");
		case 133 : return TEXT("Negerpunk");
		case 134 : return TEXT("Polsk Punk");
		case 135 : return TEXT("Beat");
		case 136 : return TEXT("Christian Gangsta Rap");
		case 137 : return TEXT("Heavy Metal");
		case 138 : return TEXT("Black Metal");
		case 139 : return TEXT("Crossover");
		case 140 : return TEXT("Contemporary Christian");
		case 141 : return TEXT("Christian Rock");
		case 142 : return TEXT("Merengue");
		case 143 : return TEXT("Salsa");
		case 144 : return TEXT("Trash Metal");
		case 145 : return TEXT("Anime");
		case 146 : return TEXT("JPop");
		case 147 : return TEXT("Synthpop");
		default  : return TEXT("Genero desconocido");
	}
}

extern "C" _declspec(dllexport) const float Version(void) {
 return VERSION;
}


extern "C" _declspec(dllexport) const TCHAR *Nombre(void) {
 return NOMBRE_IDIOMA;
}


/*extern "C" _declspec(dllexport) const TCHAR *PathAyuda(void) {
 return PATH_AYUDA;
}*/


extern "C" _declspec(dllexport) const int Icono(void) {
	return 214;
}
