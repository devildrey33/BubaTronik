// Librería para el Idioma 'Catalá'

// Este archivo contiene las frases para el idioma catalan del reproductor BubaTronik.
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
#define NOMBRE_IDIOMA	TEXT("Catalá")       // Nombre del Idioma
//#define PATH_AYUDA		TEXT("ayuda\\ayuda.htm") // Nombre del archivo de ayuda


extern "C" _declspec(dllexport) const TCHAR *TextoIdioma(const int Num) {
	switch (Num) {
		case 0	   : return TEXT("Error carrega’n l’idioma.\nLa llibreria no te la mateixa versió que el reproductor\n%s\nVersió actual : %02f, Versió Requerida : %02f\nPer solucionar el problema, reinstal·li el reproductor.");
		case 1     : return TEXT("Obrir Arxiu");
		case 2     : return TEXT("Base de Dades");
		case 3     : return TEXT("Equalitzador");
		case 4     : return TEXT("Veure Vídeo");
		case 5     : return TEXT("Llista de reproducció");
		case 6     : return TEXT("Buscar Arxius");
		case 7     : return TEXT("Opcions");
		case 8     : return TEXT("Ajuda");
		case 9     : return TEXT("Tecles rapides");
		case 10    : return TEXT("Associar Arxius");
		case 11    : return TEXT("Llista");
		case 12    : return TEXT("Aspecte");
		case 13    : return TEXT("Buscar Arxius");
		case 14    : return TEXT("Cancel·lar");
		case 15    : return TEXT("Cançó Enrere");
		case 16    : return TEXT("Play");
		case 17    : return TEXT("Pausa");
		case 18    : return TEXT("Stop");
		case 19    : return TEXT("Cançó Endavant");
		case 20    : return TEXT("Activat");
		case 21    : return TEXT("Desactivat");
		case 22    : return TEXT("Shufle");
		case 23    : return TEXT("Repeat");
		case 24    : return TEXT("Aquest reproductor no esta assignat per reproduir cançons i vídeos per defecte en windows.\nVols que aquest reproductor s'utilitzi per defecte?");
		case 25    : return TEXT("El path especificat no es vàlid!!");
		case 26    : return TEXT("Error!");
		case 27    : return TEXT("Editar Anomalies");
		case 28    : return TEXT("Assignar Nom");
		case 29    : return TEXT("Modificar Nom");
		case 30    : return TEXT("Eliminar");
		case 31    : return TEXT("Inici");
		case 32	   : return TEXT("Revisar medis");
		case 33	   : return TEXT("Vídeo");
		case 34	   : return TEXT("Mida");
		case 35	   : return TEXT("Repetir llista");
		case 36	   : return TEXT("Repetir llista + shufle");
		case 37	   : return TEXT("Estil aleatori");
		case 38	   : return TEXT("Grup aleatori");
		case 39	   : return TEXT("Disc aleatori");
		case 40	   : return TEXT("50 cançons aleatòries");
		case 41	   : return TEXT("Apagar windows");
		case 42	   : return TEXT("Apagar aquest reproductor");
		case 43	   : return TEXT("Buscar arxius a agregar");
		case 44	   : return TEXT("Revisar la base de dades");
		case 45	   : return TEXT("Arxiu");
		case 46	   : return TEXT("Nova Llista");
		case 47	   : return TEXT("Llista Buida");
		case 48	   : return TEXT("Cançons amb 5 estrelles");
		case 49	   : return TEXT("Cançons mes escoltades");
		case 50	   : return TEXT("Afegir a llista");
		case 51	   : return TEXT("Guardar Llista");
		case 52	   : return TEXT("Carregar arxiu de Subtítols");
		case 53	   : return TEXT("Restaurar");
		case 54	   : return TEXT("Minimitzar");
		case 55	   : return TEXT("Tancar");
		case 56	   : return TEXT("Afegir a una nova llista");
		case 57	   : return TEXT("Modificar");
		case 58	   : return TEXT("Nota");
		case 59	   : return TEXT("5 Estrelles");
		case 60	   : return TEXT("4 Estrelles");
		case 61	   : return TEXT("3 Estrelles");
		case 62	   : return TEXT("2 Estrelles");
		case 63	   : return TEXT("1 Estrella");
		case 64	   : return TEXT("Ordre de les cançons");
		case 65	   : return TEXT("Tipus, Estil, Grup, Disc, Cançons");
		case 66	   : return TEXT("Estil, Grup, Disc, Cançons");
		case 67	   : return TEXT("Grup, Disc, Cançons");
		case 68	   : return TEXT("Disc, Cançons");
		case 69	   : return TEXT("Propietats");
		case 70	   : return TEXT("Buscar cançó en la base de dades");
		case 71	   : return TEXT("Modificar pista");
		case 72	   : return TEXT("Temps");
		case 73	   : return TEXT("Copiar cançons a...");
		case 74	   : return TEXT("Cançó actual");
		case 75	   : return TEXT("Buscar cançó en la llista");
		case 76	   : return TEXT("Eliminar de la base de dades");
		case 77	   : return TEXT("Eliminar de la llista");
		case 78	   : return TEXT("Aleatori");
		case 79	   : return TEXT("Modificada");
		case 80	   : return TEXT("Agregar");
		case 81	   : return TEXT("Generar");
		case 82	   : return TEXT("Error carrega’n l'arxiu (%s)\nAquest error no es greu i el reproductor pot continuar l’execució del programa.\nReinstal·la el reproductor per solucionar el problema");
		case 83    : return TEXT("Analitzant :");
		case 84    : return TEXT("Arxius de Subtítols\0*.srt\0");
		case 85    : return TEXT("Obrir Subtítols");
		case 86    : return TEXT("Petita");
		case 87    : return TEXT("Mitjana");
		case 88    : return TEXT("Gran");
		case 89    : return TEXT("Pista");
		case 90    : return TEXT("Path");
		case 91    : return TEXT("Tipus");
		case 92    : return TEXT("Estil");
		case 93    : return TEXT("Grup");
		case 94    : return TEXT("Disc");
		case 95    : return TEXT("Escoltada");
		case 96    : return TEXT("%d cops");
		case 97    : return TEXT("SENSE DADES");
		case 98    : return TEXT("Dades del tag versió 1");
		case 99    : return TEXT("Dades del tag versió 2");
		case 100   : return TEXT("Dades del tag ASF");
		case 101   : return TEXT("El total de valors trobats en els tags es de : %d");
		case 102   : return TEXT("Estils");
		case 103   : return TEXT("Grups");
		case 104   : return TEXT("Discs");
		case 105   : return TEXT("Cançons");
		case 106   : return TEXT("Pressiona la nova tecla per aquesta acció");
		case 107   : return TEXT("Editar");
		case 108   : return TEXT("Control + ");
		case 109   : return TEXT("Alt + ");
		case 110   : return TEXT("Shif + ");
		case 111   : return TEXT("Combinació de tecles INVALIDA.");
		case 112   : return TEXT("Acceptar");
		case 113   : return TEXT("No mostrar mes aquest missatge");
		case 114   : return TEXT("Buscar");
		case 115   : return TEXT("Obrir el seu directori");
		case 116   : return TEXT("Mostrar log");
		case 117   : return TEXT("Sobre BubaTronik...");
		case 118   : return TEXT("Medis reproduïbles");
		case 119   : return TEXT("Arxius d’àudio");
		case 120   : return TEXT("Arxius de vídeo");
		case 121   : return TEXT("Llistes");
		case 122   : return TEXT("Tots els arxius");
		case 123   : return TEXT("Desitges eliminar també els medis del disc dur?\nPresiona ometre per eliminar-los nomes de la base de dades.");
		case 124   : return TEXT("Eliminar medis");
		case 125   : return TEXT("Ometre");
		case 126   : return TEXT("Ajuda ràpida");
		case 127   : return TEXT("Àudio");
		case 128   : return TEXT("CDAudio");
		case 129   : return TEXT("Selecciona el tipus de norma");
		case 130   : return TEXT("Llista de cançons");
		case 131   : return TEXT("Revisió de la Base de dades.");
		case 132   : return TEXT("\\Estil\\Grup\\Disc\\");
		case 133   : return TEXT("\\Grup\\Disc\\");
		case 134   : return TEXT("\\Disc\\");
		case 135   : return TEXT("Medi Següent");
		case 136   : return TEXT("Medi Anterior");
		case 137   : return TEXT("Grup desconegut");
		case 138   : return TEXT("Disc desconegut");
		case 139   : return TEXT("CD d’Àudio (%s)");
		case 140   : return TEXT("Estil seleccionat : #%s#");
		case 141   : return TEXT("#Modificada#");
		case 142   : return TEXT("Estil aleatori #%s#");
		case 143   : return TEXT("Estil suggerit #%s#");
		case 144   : return TEXT("Grup aleatori #%s#");
		case 145   : return TEXT("Grup suggerit #%s#");
		case 146   : return TEXT("Disc aleatori #%s#");
		case 147   : return TEXT("Disc suggerit #%s#");
		case 148   : return TEXT("#%d Cançons aleatòries#");
		case 149   : return TEXT("#Cançons amb 5 estrelles#");
		case 150   : return TEXT("#Les %d cançons mes escoltades#");
		case 151   : return TEXT("Estil mes escoltat #%s#");
		case 152   : return TEXT("Grup mes escoltat #%s#");
		case 153   : return TEXT("Disc mes escoltat #%s#");
		case 154   : return TEXT("Error greu!!");
		case 155   : return TEXT("Enviar");
		case 156   : return TEXT("Acabar");
		case 157   : return TEXT("L'operació s'ha completat correctament.");
		case 158   : return TEXT("Llista M3U\0*.m3u\0\0");
//		case 159   : return TEXT("Guardar Llista");
		case 160   : return TEXT("L'arxiu %s ja existeix, el vols sobreescriure?");
		case 161   : return TEXT("Ja existeix un arxiu amb aquest nom!");
		case 162   : return TEXT("No s'ha pogut guardar l'arxiu, comprova que tinguis els privilegis suficients en el directori.");
		case 163   : return TEXT("Error guardant la llista!");
		case 164   : return TEXT("La llista s'ha guardat en : %s");
		case 165   : return TEXT("Llista guardada!");
		case 166   : return TEXT("Cursor Endavant");
		case 167   : return TEXT("Cursor Enrere");
		case 168   : return TEXT("Cursor Esquerra");
		case 169   : return TEXT("Cursor Dreta");
		case 170   : return TEXT("Espai");
		case 171   : return TEXT("Retrocés");
		case 172   : return TEXT("Insert");
		case 173   : return TEXT("Suprimir");
//		case 174   : return TEXT("Inici");
		case 175   : return TEXT("Fi");
		case 176   : return TEXT("Esc"); 
		case 177   : return TEXT("Shift");
		case 178   : return TEXT("Imprimir Pantalla");
		case 179   : return TEXT("Arxiu d’àudio");
		case 180   : return TEXT("Arxiu de vídeo");
		case 181   : return TEXT("Numeració");
		case 182   : return TEXT("Ample");
		case 183   : return TEXT("Altura"); 
		case 184   : return TEXT("Dades del tag Vorbis :");
	
		// ControlPresentacionEx::Mostrar_Inicio
		case 1000  : return TEXT("Benvingut a %s %s");
		case 1001  : return TEXT("Llista actual : %s");
		case 1002  : return TEXT("No hi ha suggeriments per la base de dades.");
		case 1003  : return TEXT("Numero de cançons #%d# , Temps total de la llista #%s#.");
		case 1004  : return TEXT("%d Estils, %d Grups, %d Discs, %d Cançons i %d Vídeos.");
		case 1005  : return TEXT("Estil mes escoltat $%s$ amb un total de #%d# cops.");
		case 1006  : return TEXT("Grup mes escoltat $%s$ amb un total de #%d# cops.");
		case 1007  : return TEXT("Disc mes escoltat $%s$ amb un total de #%d# cops.");
		case 1008  : return TEXT("Suggeriments: $%s$ y $%s$");
		case 1009  : return TEXT("Sense suggeriments.");
		case 1010  : return TEXT("Generador de llistes :");
		case 1011  : return TEXT("Llista amb 50 cançons basada en un estil aleatori");
		case 1012  : return TEXT("Llista amb 50 cançons basada en un grup aleatori");
		case 1013  : return TEXT("Llista amb 50 cançons basada en un disc aleatori");
		case 1014  : return TEXT("Llista amb 50 cançons aleatòries");
		case 1015  : return TEXT("Llista amb les cançons que tenen 5 estrelles");
		case 1016  : return TEXT("Llista amb les 50 cançons mes escoltades");
		case 1017  : return TEXT("Medis oberts recentment :");
		// ControlPresentacionEx::Mostrar_CargandoVLC
		case 1020  : return TEXT("Carregant nucli de la LIBVLC %s ....");
		// ControlPresentacionEx::Mostrar_SobreBubaTronik
		case 1030  : return TEXT("BubaTronik es una aplicació que va néixer davant la necessitat de ordenar i reproduir medis fàcilment. Ames es el exemple mes complet que existeix sobre la llibreria DWL.");
		case 1031  : return TEXT("$BubaTronik$, la $DWL$ y $www.devildrey33.es$ son propietat de #Josep Antoni Bover Comas#.");
		case 1032  : return TEXT("Si desitgeu posar-vos en contacte amb l'autor podeu fer-ho enviant un missatge en el $fòrum$ de www.devildrey33.es.");
		case 1033  : return TEXT("Hi ha molta gent que ha col·laborat activa i passivament en aquest projecte cosa que haig d'agrair. En especial les següents persones han contribuït últimament en el desenvolupament :");
		case 1034  : return TEXT("- #Barba# Sempre dona bones opinions per problemes poc comuns.");
		case 1035  : return TEXT("- #Fong# Aportació de varies idees i una valuosa zona de proves.");
		case 1036  : return TEXT("- #Michel# Aportació de varies ideas i una valuosa zona de proves.");
		case 1037  : return TEXT("- #Parris# Un altre valuosa zona de proves.");
		case 1038  : return TEXT("- $Txema$ Un altre valuosa zona de proves.");
		case 1039  : return TEXT("- També haig d'agrair a tota la gent que m'ha donat suport durant aquest projecte. #Gracies a TOTS!#.");
		case 1040  : return TEXT("Per últim també haig de mencionar que aquest reproductor utilitza 2 llibreries per complir amb la seva funció, y son :");
		case 1041  : return TEXT("#FMOD EX# 4.26 Potent llibreria d’àudio utilitzada en molts jocs actuals.");
		case 1042  : return TEXT("Pots trobar-la en el següent enllaç : $http://www.fmod.org$");
		case 1043  : return TEXT("#LIBVLC# %s Llibreria per reproduir pràcticament qualsevol medi.");
		case 1044  : return TEXT("Pots trobar-la en el següent enllaç : $http://www.videolan.org$");
		case 1045  : return TEXT("Dades addicionals");
		case 1046  : return TEXT("BubaTronik #%.03f# #%s# compilat amb #MSVC %d# el %s.");
		case 1047  : return TEXT("Versió de windows #%d.%d# Service Pack #%d.%d#");
		// ControlPresentacionEx::Mostrar_Opciones_Lista
		case 1050  : return TEXT("Llista inicial");
		case 1051  : return TEXT("- Escull eix que ha de fer el reproductor si s'inicia amb una llista buida. Per defecte s’iniciarà amb una llista buida, si no se li pasa cap parametre.");
		case 1052  : return TEXT("- #NOTA# : si escull eixes la opció 'Qualsevol cosa', el reproductor seleccionarà una de las opcions de la llista aleatòriament, #exceptuant# la opció '#Llista buida#'");
		case 1053  : return TEXT("Qualsevol cosa.");
		case 1054  : return TEXT("Excloure cançons");
		case 1055  : return TEXT("- Aquesta opció et permet excloure les cançons que tinguin una estrella de totes les llistes aleatòries que genera el reproductor, aixi pots controlar aquelles cançons que no vols afegir.");
		case 1056  : return TEXT("Excloure cançons amb una estrella.");
		// ControlPresentacionEx::Mostrar_Opciones_BD
		case 1060  : return TEXT("Mostrar tags");
		case 1061  : return TEXT("- Si actives aquesta opció, en els tooltips informatius es mostraran totes les dades del tag.");
		case 1062  : return TEXT("Mostrar Tags en els Tooltips.");
		// ControlPresentacionEx::Mostrar_Opciones_Video
		case 1070  : return TEXT("Opcions de Vídeo");
		case 1071  : return TEXT("- Aquesta opció ocultarà el mouse, quan el reproductor estigui en mode pantalla completa, i l'usuari estigui mes de 2 segons sense fer res.");
		case 1072  : return TEXT("Ocultar mouse en pantalla completa.");
		// ControlPresentacionEx::Mostrar_Opciones_Apariencia
		case 1080  : return TEXT("Mida de la font");
		case 1081  : return TEXT("- Selecciona la mida de la font que s’utilitzarà en el reproductor.");
		case 1082  : return TEXT("Configuració de colors");
		case 1083  : return TEXT("- Escull eix la configuració de colors que prefereixis pel reproductor.");
		case 1084  : return TEXT("#NOTA :# Pots crear/editar una configuració de colors, editant els arxius amb extensió '.BubaTronik_Skin' i modificant els seus valors RGB pels colors. Aquests arxius els trobaràs en la subcarpeta 'Graficos' situada en la arrel del reproductor. Per mes informació edita un arxiu, y llegeix les instruccions que hi ha dintre.");
		case 1085  : return TEXT("Mida de l'espectre gràfic");
		case 1086  : return TEXT("- Selecciona la mida de les barres per l'espectre gràfic del so.");
		case 1087  : return TEXT("Barra de títol");
		case 1088  : return TEXT("- Utilitzar barra de títol de windows (recomanat en windows vista i superiors).");
		case 1089  : return TEXT("Utilitzar barra de títol de windows.");
		// ControlPresentacionEx::Mostrar_Opciones
		case 1090  : return TEXT("Idioma");
		case 1091  : return TEXT("- Escull eix quin idioma prefereixes que es mostri en el reproductor. Per defecte s’escull eix el mateix que el sistema operatiu si esta disponible.");
		case 1092  : return TEXT("Actualizacions");
		case 1093  : return TEXT("- Buscar a internet una nova versió del reproductor al iniciar. En el cas de trobar una nova versió, es mostrarà un missatge que et permetrà descarregar i instal·lar la nova actualització.");
		case 1094  : return TEXT("Buscar Actualitzacions");
		case 1095  : return TEXT("Reproductor sempre davant");
		case 1096  : return TEXT("- Aquesta opcio et permet tenir per davant de totes les altres finestres la finestra del reproductor.");
		case 1097  : return TEXT("Tipus d'arxiu");
		case 1098  : return TEXT("- Els tipus d'arxiu que es veuen a continuació son els tipus que utilitzarà el reproductor tant per buscar arxius, como per associar-se a aquells tipus per reproduir-los per defecte. Pots eliminar els tipus que no t’interessin.");
		case 1099  : return TEXT("Eliminar àudio");
		case 1100  : return TEXT("Eliminar vídeo");
		case 1101  : return TEXT("Eliminar seleccionats");
		case 1102  : return TEXT("Restaurar tipus");
		case 1103  : return TEXT("Associar tipus");
		case 1104  : return TEXT("- Aquesta opció et permet associar els tipus d'arxiu de la llista anterior al reproductor. D'aquesta forma quan facis dobleclick en l'explorador a un arxiu que correspongui a algun dels tipus acceptats s’engegarà aquest reproductor per defecte.");
		case 1105  : return TEXT("Preguntar al iniciar, quan sigui necessari.");
		case 1106  : return TEXT("Sempre.");
		case 1107  : return TEXT("Mai.");
		case 1108  : return TEXT("- Selecciona vols que s'executin els arxius per defecte.");
		case 1109  : return TEXT("Afegir a la llista.");
		case 1110  : return TEXT("Reproduir.");
		// ControlPresentacionEx::Mostrar_Opciones_Teclado
		case 1120  : return TEXT("Tecles d’accés ràpid");
		case 1121  : return TEXT("- Pots canviar les tecles per qualsevol acció de les que es llisten a continuació. Es possible combinar les tecles Control i Shift amb altres tecles sense cap problema. També pots utilitzar algunes combinacions amb la tecla Alt, sempre que la tecla control estigui pressionada, però no es molt recomanable perquè moltes aplicacions desplegaran menús emergents.");
		case 1122  : return TEXT("Play / Pausa");
		case 1123  : return TEXT("Pujar Volum");
		case 1124  : return TEXT("Baixar Volum");
		case 1125  : return TEXT("Reproduir medi següent");
		case 1126  : return TEXT("Reproduir medi anterior");
		case 1127  : return TEXT("Obtenir teclat fora de l’aplicació");
		case 1128  : return TEXT("- Marca aquesta opció per que el reproductor detecti les tecles d’accés ràpid fins i tot sota un joc. #NOTA#, si maques aquesta opció es recomanable que t’asseguris de que les combinacions de tecles no s'utilitzin en altres programes / jocs.");
		case 1129  : return TEXT("Obtenir teclat quan el reproductor no estigui actiu.");
		// NOTA DEJAMOS 10 ESPACIOS MAS, PORQUE ES PROBABLE QUE ACABE MODIFICANDO ESTA ESCENA
		// ControlPresentacionEx::Mostrar_RevisarBD 
		case 1140  : return TEXT("Desde aquest panell pots revisar la base de dades per determinar si s'han d’eliminar medis dels discs.");
		case 1141  : return TEXT("Es important que revisis la base de dades quan eliminis cançons desde fora del reproductor, ka que desprès el reproductor podria afegir-les a les llistes aleatòries.");
		case 1142  : return TEXT("Per últim també pots fer una revisió per determinar quins estils/grups/discs s’assemblen entre si.");
		case 1143  : return TEXT("Revisar anomalies en estils / grups / discs");
		// ControlPresentacionEx::Mostrar_BuscarArchivos
		case 1150  : return TEXT("El reproductor buscarà nous medis reproduïbles en els directoris que es mostren en la llista de sota. Pots agregar o eliminar directoris al teu gust de la llista sempre que com a mínim quedi un");
		case 1151  : return TEXT("(Path)");
		case 1152  : return TEXT("(Tag)");
		case 1153  : return TEXT("PATH\\Estil\\Grup\\Disc\\");
		case 1154  : return TEXT("PATH\\Grup\\Disc\\");
		case 1155  : return TEXT("PATH\\Disc\\");
		case 1156  : return TEXT("Indefinit");
		case 1157  : return TEXT("Selecciona un directori");
		case 1158  : return TEXT("Suggeriments");
		case 1159  : return TEXT("Selecciona quin tipus de norma hauries d'utilitzar per la estructuració del directori que has escollit.");
		case 1160  : return TEXT("1 - \\Estil\\Grup\\Disc\\ (Escaneig per Path)");
		case 1161  : return TEXT("2 - \\Grup\\Disc\\ (Escaneig per Path)");
		case 1162  : return TEXT("3 - \\Disc\\ (Escaneig per Tag)");
		case 1163  : return TEXT("4 - Una mica de tot / Indefinit (Escaneig per Tag)");
		case 1164  : return TEXT("Path abans del Tag");
		case 1165  : return TEXT("Tag abans del Path");
		case 1166  : return TEXT("Aquest procés pot trigar varis minuts depenen del ordinador i el numero d'arxius reproduïbles que es trobin.");
		case 1167  : return TEXT("Filtrat de caràcters");
		case 1168  : return TEXT("Anàlisis post escaneig");


		// Mensajes
		case 4000  : return TEXT("Error la unitat que conte el medi no es troba disponible,\nper això es impossible reproduir el medi.");
		case 4001  : return TEXT("Revisió acabada, s'han eliminat un total de %d entrades invalides");
		case 4002  : return TEXT("Error el directori introduït no es vàlid, o ja existeix en la llista.");
		case 4003  : return TEXT("Desitges eliminar el M3U del disc? Això no afectarà als medis que contingui el M3U.");
		case 4004  : return TEXT("Bubatronik ha trobat un error crític i ames no ha pogut crear un informe d'errors.....");
		case 4005  : return TEXT("La versió de la base de dades no es vàlida per aquest reproductor.");
		case 4006  : return TEXT("No es pot carregar la base de dades!");
		case 4007  : return TEXT("Hi han %d medis que no s'han carregat perquè la seva informació podria ser invalida i necessiten ser analitzats de nou.\nEs recomana executar la cerca d'arxius per tornar a tenir aquests medis en la base de dades.");
		case 4008  : return TEXT("Ignorada carrega d'alguns medis");
		case 4009  : return TEXT("BubaTronik ha trobat un error greu i es tancarà.");
		case 4010  : return TEXT("S'ha creat un informe d'errors, vols enviar-lo a www.devildrey33.es?");
		case 4011  : return TEXT("Pressiona el boto 'Enviar' per enviar el missatge i acabar, o pressiona el boto");
		case 4012  : return TEXT("'Acabar' per sortir sense enviar el missatge.");
		case 4013  : return TEXT("Informació");
		case 4014  : return TEXT("Informe d'errors enviat correctament.");
		case 4015  : return TEXT("Error enviant l'informe de errors....");
		case 4016  : return TEXT("S'ha trobat una nova actualització de BubaTronik, vols descarregar-la?? \n%s");
		case 4017  : return TEXT("Nova actualització!");
		case 4018  : return TEXT("El reproductor s’apagarà per actualizar-se, i es tornarà a engegar un cop acabada la actualització.\nPresiona intro per continuar.");
		case 4019  : return TEXT("Actualitzar!");
		case 4020  : return TEXT("Error descarregant la actualització, pots prova-ho mes tard, \no pots provar a baixar l’actualització manualment desde http://bubatronik.devildrey33.es");
		case 4021  : return TEXT("Començant actualització");
		case 4022  : return TEXT("Error de descarrega");
		case 4023  : return TEXT("Error carrega’n el Winsock.");
		case 4024  : return TEXT("Error al obtenir la IP del servidor.");
		case 4025  : return TEXT("Error socket invàlid.");
		case 4026  : return TEXT("Error conectan al servidor.");
		case 4027  : return TEXT("Error durant la transmissió de dades.");
		case 4028  : return TEXT("Error!! la versió del skin no es vàlida per aquest reproductor.");
		case 4029  : return TEXT("Error carrega’n el arxiu de skin (%s).\nNo hi ha suficients colors, i s'ha carregat una configuració per defecte.\nAquest error no es greu, i es pot continuar amb l’execució del programa.\nReinstal·la el reproductor per solucionar el problema.");
		case 4030  : return TEXT("No pots afegir medis externament mentres s'esta realitzan una cerca d'arxius.");

		// ToolTips :
		// -ID_GENERAR_LISTA
		case 5000  : return TEXT("Has d'elegir un tipus de llista per poder generar-la.");
		case 5001  : return TEXT("Al pressionar aquest botó, crearàs una nova llista");
		case 5002  : return TEXT("de unes 50 cançons del tipus especificat.");
		// -ID_TIPOS_LISTA
		case 5010  : return TEXT("Aquest control et permet elegir entre varis tipus de llistes prefabricades :");
		case 5011  : return TEXT("- Llista de cançons aleatòries basades en l'estil.");
		case 5012  : return TEXT("- Llista de cançons aleatòries basades en el grup.");
		case 5013  : return TEXT("- Llista de cançons aleatòries basades en el disc.");
		case 5014  : return TEXT("- Llista de cançons aleatòries sense basar-se en res.");
		case 5015  : return TEXT("- Llista amb les cançons mes escoltades (si s'ha escoltat alguna)");
		case 5016  : return TEXT("- Llista amb cançons de 5 estrelles (si s'ha assignat alguna)");
		case 5017  : return TEXT("NOTA : Aquestes llistes com a màxim podràn tenir 50 cançons.");
		// -CP_SUGERENCIA_GENERO1 -CP_SUGERENCIA_GENERO2 -CP_SUGERENCIA_GRUPO1 -CP_SUGERENCIA_GRUPO2 -CP_SUGERENCIA_DISCO1 -CP_SUGERENCIA_DISCO2
		case 5020  : return TEXT("Cada cop que accedeixis a la secció d'inici es mostraran nous suggeriments.");
		// -CP_SUGERENCIA_GENERO1 -CP_SUGERENCIA_GENERO2 -CP_GENERO_MAS
		case 5021  : return TEXT("Fes click per crear una nova llista amb varies cançons de l'estil :");
		// -CP_SUGERENCIA_GRUPO1 -CP_SUGERENCIA_GRUPO2 -CP_GRUPO_MAS
		case 5022  : return TEXT("Fes click per crear una nova llista amb varies cançons del grup :");
		// -CP_SUGERENCIA_DISCO1 -CP_SUGERENCIA_DISCO2 -CP_DISCO_MAS
		case 5023  : return TEXT("Fes click per crear una nova llista amb varies cançons del disc :");
		// -CP_GENERO_MAS
		case 5024  : return TEXT("Per determinar quin estil s'ha escoltat es fa una llista amb tots els estils,");
		case 5025  : return TEXT("en la que es sumen els cops que s'ha escoltat cada cançó de l'estil.");
		// -CP_GRUPO_MAS
		case 5026  : return TEXT("Per determinar quin grup s'ha escoltat es fa una llista amb tots els grups,");
		case 5027  : return TEXT("en la que es sumen els cops que s'ha escoltat cada cançó del grup.");
		// -CP_DISCO_MAS
		case 5028  : return TEXT("Per determinar quin disc s'ha escoltat es fa una llista amb tots els discs,");
		case 5029  : return TEXT("en la que es sumen els cops que s'ha escoltat cada cançó del disc.");
		// -ID_FILTRO_COINCIDENCIAS
		case 5030  : return TEXT("Activant aquesta opció el reproductor realitzarà una comparació entre tots els seus estils, grups, i discs");
		case 5031  : return TEXT("de forma que solucioni el 80% d'errors tipogràfics en els tags.");
		case 5032  : return TEXT("Posem per exemple que inicialment es troben el següents dos grups :");
		case 5033  : return TEXT("'IronMaiden' i ' Iron Maiden'");
		case 5034  : return TEXT("Si us fixeu son pràcticament el mateix però un te un espai al mig, i l'altre no.");
		case 5035  : return TEXT("Una cosa tan simple com aquesta fa que el reproductor interpreti aquests resultats com 2 grups diferents.");
		case 5036  : return TEXT("I per això s'ha creat aquesta opció, per intentar corregir al màxim aquest tipus d'errors.");
		case 5037  : return TEXT("NOTA :");
		case 5038  : return TEXT("Activant aquesta opció, el temps total de la cerca d'arxius s’incrementarà notablement.");
		// -ID_FILTRO_CARACTERES
		case 5040  : return TEXT("Activant aquesta opció el reproductor filtrarà de tots els noms els següents caràcters :");
		case 5041  : return TEXT(": \\ / ' \" { } [ ] ? ¿ ; % # @ ^ ames dels accents y signes de puntuació.");
		case 5042  : return TEXT("Amb això aconseguim molta mes precisió a l'hora d'identificar els estils, grups i discs.");
		case 5043  : return TEXT("Es molt recomanable deixar activada aquesta opció, ames de que no afectarà al temps de la cerca.");
		// -ID_BOTON_BUSCAR2
		case 5050  : return TEXT("Pressiona aquest boto per començar la cerca.");
		case 5051  : return TEXT("Abans de fer-ho hauries d’assegurar-te que has afegit els directoris que vols buscar en la llista.");
		case 5052  : return TEXT("Pressiona aquest boto per cancel·lar la cerca.");
		case 5053  : return TEXT("Per buscar primer tens que agregar un directori a la llista, i elegir el tipus d'ordre.");
		// -ID_BOTON_AGREGAR_DIR
		case 5060  : return TEXT("Al pressionar aquest boto afegiràs el directori del quadre desplegable a la llista de directoris.");
		case 5061  : return TEXT("No es recomanable afegir la arrel de cap disc dur, exceptuant el cas de que aquell disc sigui");
		case 5062  : return TEXT("únicament per a medis d’àudio i vídeo.");
		// -ID_COMBO_DIR
		case 5070  : return TEXT("Desde aquest control desplegable pots buscar directoris per afegir a la llista.");
		case 5071  : return TEXT("Els directoris de la llista d'adalt son els que escanejarà el reproductor per trobar nous medis.");
		// -ID_DIRECTORIOS_BUSCAR
		case 5080  : return TEXT("Aquesta llista conte els directoris en els que el reproductor buscarà els nous medis reproduïbles.");
		case 5081  : return TEXT("Per defecte s'han afegit els directoris predeterminats de windows, si desitges eliminar-les pots utilitzar");
		case 5082  : return TEXT("el boto amb una creu vermella situat a la part inferior dreta de aquesta llista.");
		case 5083  : return TEXT("Si vols agregar directoris, has d'utilitzar el control desplegable que hi ha mes abaix per seleccionar");
		case 5084  : return TEXT("el directori que vulguis, i per últim has de pressionar el boto agregar per insertarlo en aquesta llista.");
		// -ID_LISTA_IDIOMAS
		case 5090  : return TEXT("Aquesta llista conte els idiomes disponibles per aquest reproductor.");
		case 5091  : return TEXT("Si estàs interessat en agregar un nou idioma a aquesta aplicació, posat en contacte amb devldrey33");
		case 5092  : return TEXT("a traves del fòrum de http://www.devildrey33.es");
		// -ID_BUSCAR_ACTUALIZACIONES
		case 5100  : return TEXT("Activant aquesta opció el reproductor es connectarà un cop al dia com a màxim per comprovar si existeixen");
		case 5101  : return TEXT("noves actualitzacions per instal·lar.");
		// -ID_SIEMPRE_DELANTE
		case 5110  : return TEXT("Activant aquesta opció el reproductor es quedarà sempre davant de les altres finestres.");
		// -ID_LISTA_TIPOS
		case 5120  : return TEXT("Aquesta llista conte tots els tipus d’àudio i de vídeo que el reproductor accepta per defecte.");
		case 5121  : return TEXT("Pots eliminar aquelles extensions que no t’interessi que el reproductor accepti.");
		// -ID_BOTON_ELIMINAR_TIPOS_AUDIO
		case 5130  : return TEXT("Aquest boto eliminarà de la llista tots els tipus que siguin predefinits per àudio.");
		// -ID_BOTON_ELIMINAR_TIPOS_VIDEO
		case 5140  : return TEXT("Aquest boto eliminarà de la llista tots els tipus que siguin predefinits per vídeo.");
		// -ID_BOTON_ELIMINAR_TIPOS
		case 5150  : return TEXT("Aquest boto eliminarà de la llista tots els tipus que es trobin seleccionats en aquest moment.");
		// -ID_BOTON_RESTAURAR_TIPOS
		case 5160  : return TEXT("Aquest boto restaurarà tots els tipus que pot utilitzar BubaTronik per defecte.");
		// -ID_ASOCIAR_PREGUNTAR
		case 5170  : return TEXT("Seleccionant aquesta opció, el reproductor mirarà cada cop que s’iniciï si esta associat a els");
		case 5171  : return TEXT("tipus de la llista, en cas de no estar associat a algun d'aquets tipus el reproductor mostrarà");
		case 5172  : return TEXT("un missatge que et demanarà si vols associar el reproductor als tipus de la llista.");
		// -ID_ASOCIAR_SIEMPRE
//		case 5180  : Es el mateix texte que el 5170
		case 5181  : return TEXT("tipus de la llista, en cas de no estar associat a algun d'aquets tipus el reproductor s’associarà");
		case 5182  : return TEXT("a ells automàticament, sense preguntar res.");
		// -ID_ASOCIAR_NUNCA
		case 5190  : return TEXT("Seleccionant aquesta opció, el reproductor ometrà escanejar el registre per determinar si esta");
		case 5191  : return TEXT("associat o no als tipus d'arxiu de la llista.");
		// -ID_ASOCIAR_ADD
		case 5200  : return TEXT("Al executar un arxiu associat al reproductor desde l'explorador, s’afegirà a la llista sense");
		case 5201  : return TEXT("interrompre la reproducció actual.");
		// -ID_ASOCIAR_REP
		case 5210  : return TEXT("Al executar un arxiu associat al reproductor desde l'explorador, s’afegirà a la llista ");
		case 5211  : return TEXT("interrompent la reproducció actual, i es posarà a reproduir el primer medi afegit desde");
		case 5212  : return TEXT("l'explorador.");
		// -ID_OPCIONES_FUENTE
		case 5220  : return TEXT("En aquesta llista pots seleccionar la mida de la font que mes s'ajusti a les teves necessitats.");
		case 5221  : return TEXT("Hi ha tres tipus de mida : petit (11px), mitja (13px), i gran (15px).");
		// -ID_OPCIONES_SKIN
		case 5230  : return TEXT("En aquesta llista pots seleccionar la configuració de colors que mes t'agradi.");
		case 5231  : return TEXT("Si desitges crear una configuració de colors per a BubaTronik i no ho veus molt clar,");
		case 5232  : return TEXT("pots consultar els teus dubtes en el fòrum de  http://www.devildrey33.es");
		// -ID_OPCIONES_OSCY
		case 5240  : return TEXT("En aquesta llista pots seleccionar la mida de les barretes del espectre gràfic.");
		case 5241  : return TEXT("El petit fa 1 píxel d'ample, el mitja fa 2 píxels, i el gran fa 3 píxels.");
		// -ID_OPCIONES_UTILIZAR_BARRA_WIN
		case 5250  : return TEXT("Marcant aquesta opció el reproductor utilitzarà la barra de windows estàndard amb la seva bora.");
		case 5251  : return TEXT("En principi el reproductor es va dissenyar per a windows XP, per lo que es va crear una barra de títol");
		case 5252  : return TEXT("diferent a la de windows que quedava mes be amb aquest reproductor.");
		case 5253  : return TEXT("Si estàs utilitzant windows vista o windows 7 es recomana utilitzar la barra de títol de windows");
		case 5254  : return TEXT("per no perdre les capacitats esteses de maximitzat parcial.");
		// -ID_OPCIONES_MOSTRAR_TAG
		case 5260  : return TEXT("Marcant aquesta opció es carregaran al vol i es mostraran els tags de les cançons al mostrar");
		case 5261  : return TEXT("el tooltip informatiu.");
		case 5262  : return TEXT("Amb el quart boto del mouse pots fer aparèixer aquets tooltips dintre de la llista i de la");
		case 5264  : return TEXT("base de dades sense tenir que esperar 2 segons sobre de la cançó.");
		// -ID_LISTA_INICIO
		case 5270  : return TEXT("En aquesta llista desplegable pots seleccionar que vols que faci el reproductor al iniciar-se :");
		case 5271  : return TEXT("- Crear una llista partint d'un estil aleatori amb un màxim de 50 cançons.");
		case 5272  : return TEXT("- Crear una llista partint d'un grup aleatori amb un màxim de 50 cançons.");
		case 5273  : return TEXT("- Crear una llista partint d'un disc aleatori amb un màxim de 50 cançons.");
		case 5274  : return TEXT("- Crear una llista aleatòria amb un màxim de 50 cançons.");
		case 5275  : return TEXT("- (Qualsevol cosa) Una de les 4 opcions anteriors aleatòriament.");
		case 5276  : return TEXT("- Crear una llista buida.");
		case 5277  : return TEXT("Si el reproductor s'inicia desde l'explorador peque has obert un medi desde allà, la creació");
		case 5278  : return TEXT("de la llista aleatòria s'ometrà.");
		// -ID_EXCLUIR_NOTA
		case 5280  : return TEXT("Marcant aquesta opció el reproductor evitarà crear llistes aleatòries amb cançons que nomes tinguin una estrella.");
		case 5281  : return TEXT("El sistema de notes pels medis per defecte li dona 2 estrelles a tots els medis. Si vols que un medi tingui");
		case 5282  : return TEXT("mes nota (fins a 5 estrelles) pots canviar-li la nota utilitzant el menú desplegable de la llista o la base de dades.");
		case 5283  : return TEXT(""); // en castellà es necessiten 4 línees, aquesta la deixem buida
		// -ID_OPCIONES_TECLA_PLAY
		case 5290  : return TEXT("Desde aquest control pots seleccionar la tecla que s’utilitzarà per realitzar l’acció : Play / Pausa");
		// -ID_OPCIONES_TECLA_STOP
		case 5300  : return TEXT("Desde aquest control pots seleccionar la tecla que s’utilitzarà per realitzar l’acció : Stop");
		// -ID_OPCIONES_TECLA_VOLUMEN_MAS
		case 5310  : return TEXT("Desde aquest control pots seleccionar la tecla que s’utilitzarà per realitzar l’acció : Pujar el volum");
		// -ID_OPCIONES_TECLA_VOLUMEN_MENOS
		case 5320  : return TEXT("Desde aquest control pots seleccionar la tecla que s’utilitzarà per realitzar l’acció : Baixar el volum");
		// -ID_OPCIONES_TECLA_SIGUIENTE
		case 5330  : return TEXT("Desde aquest control pots seleccionar la tecla que s’utilitzarà per realitzar l’acció : Medi següent");
		// -ID_OPCIONES_TECLA_ANTERIOR
		case 5340  : return TEXT("Desde aquest control pots seleccionar la tecla que s’utilitzarà per realitzar l’acció : Medi anterior");
		// -ID_OPCIONES_OCULTAR_MOUSE
		case 5350  : return TEXT("Marcant aquesta opció en el mode pantalla completa (dintre de un vídeo) el mouse s’ocultarà");
		case 5351  : return TEXT("pasats 2 segons sense fer res. Això pasara sempre que el mouse no estigui en la part dels controls.");
		// -CP_URL_FMOD
		case 5360  : return TEXT("Pàgina web d'aquesta fabulosa llibreria per Àudio");
		// -CP_URL_LIBVLC
		case 5370  : return TEXT("Pàgina web d'aquesta excel·lent llibreria per Àudio i Vídeo");
		// -CP_URL_DEVILDREY33
		case 5380  : return TEXT("Pàgina web de devildrey33");
		// -CP_URL_MALEIT
		case 5390  : return TEXT("Pàgina web del grup d'aquest crack a la bateria. MALEÏT");
		// -CP_URL_BUBATRONIK
		case 5400  : return TEXT("Pàgina web principal d'aquest reproductor");
		// -CP_URL_DWL
		case 5410  : return TEXT("Página web de la DReY Windows Lib");
		case 5411  : return TEXT("La DWL es una llibreria per fer aplicacions sota windows en C++");
		case 5412  : return TEXT("que es distribueix sota llicència GPL.");
		// -CP_URL_FORO
		case 5420  : return TEXT("Fòrum de http://www.devildrey33.es");
		// -ID_COMBO_ORDEN_DIR
		case 5430  : return TEXT("Aquestes normes s'utilitzen a l'hora d'obtindre dades d'un medi, per determinar millor les seves dades.");
		case 5431  : return TEXT("Si per exemple tenim el directori 'C:\\MP3' i dintre tenim música estructurada segons 'Grup\\Disc' podem");
		case 5432  : return TEXT("trobar el cas en que ames existeixi un directori separat per cada cd, exemple :");
		case 5433  : return TEXT("C:\\MP3\\Baron rojo\\Baron al Rojo Vivo\\CD1");
		case 5434  : return TEXT("En aquest cas concret, si estem buscant dades basant-nos en el path i no s'ha elegit el tipus 'Grup\\Disc'");
		case 5435  : return TEXT("el reproductor pensarà que el nom del grup es 'Baron al Rojo Vivo' i el disc 'CD1' cosa que es incorrecta.");
		case 5436  : return TEXT("Primer has de seleccionar un directori abans de poder establir el tipus de norma a utilitzar.");
		// -ID_MARCA_PATH
		case 5440  : return TEXT("Marcant aquesta opció el reproductor donarà prioritat a les dades del path per sobre de les dades del tag, pel directori que s'ha d'agregar.");
		case 5441  : return TEXT("Primer has de seleccionar un directori abans de poder establir la prioritat de les dades.");
		// -ID_MARCA_TAG
		case 5442  : return TEXT("Marcant aquesta opció el reproductor donarà prioritat a les dades del tag per sobre de les dades del path, pel directori que s'ha d'agregar.");

			

	}
	static TCHAR Error[512];
	wsprintf(Error, TEXT("(%d) NULL"), Num);
	return Error; // NO MODIFICAR, Este texto se retorna cuando le paso un numero invalido, y asi me doy cuenta de que falta.
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
		default  : return TEXT("Estil desconegut");
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
	return 215;
}
