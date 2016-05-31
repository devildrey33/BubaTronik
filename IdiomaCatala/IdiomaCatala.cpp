// Librer�a para el Idioma 'Catal�'

// Este archivo contiene las frases para el idioma catalan del reproductor BubaTronik.
// Si quieres traducir este archivo para crear otro idioma, deber�s editar las frases que hay dentro de las macros TEXT("")
// Veras que hay varias l�neas numeradas de la siguiente forma : 'case 1 : return TEXT("Archivo");'
// - El numero indica la ID de la frase, y la frase esta dentro de la macro TEXT(""), entonces si queremos traducir esa frase en ingles,
//   quedar�a de la siguiente forma : 'case 1 : return TEXT("File");'
// - Tambi�n debemos tener en cuenta que existen caracteres que son 'especiales' para el reproductor, y no deben ser modificados 
//   ya que podr�an originar errores graves. A continuaci�n pod�is ver una lista de los caracteres especiales explicados :
//--------------------------------------------------------------------------------------------------------------------------------------------
//   $  - Este car�cter delimita una frase pulsable, es decir si tenemos TEXT("Hola pulsa $aqu�$") el reproductor interpretara que la 
//        palabra 'aqu�' es pulsable y la enlazara a un comando para ejecutarla internamente cuando se requiera.
//--------------------------------------------------------------------------------------------------------------------------------------------
//   #  - Este car�cter delimita una frase con una parte de texto resaltada, ya sea por negrita, cursiva, etc..
//--------------------------------------------------------------------------------------------------------------------------------------------
//   %d - Esta combinaci�n de caracteres simboliza un digito decimal, tambi�n nos podemos encontrar combinaciones como %02d, etc...
//		  que deben ser tratadas igual. Por ejemplo : TEXT("Total de canciones : %d")	= 'Total de canciones 100'
//--------------------------------------------------------------------------------------------------------------------------------------------
//   %f - Esta combinaci�n de caracteres simboliza un digito decimal,, tambi�n nos podemos encontrar combinaciones como %.02f, etc...
//		  que deben ser tratadas igual. Por ejemplo : TEXT("Nota media del disco : %.02f") = 'Nota media del disco : 4.60'
//--------------------------------------------------------------------------------------------------------------------------------------------
//	 %s - Esta combinaci�n de caracteres simboliza una frase, por ejemplo :	TEXT("Lista actual : %s") = 'Lista actual : Disco Iron Maiden'
//--------------------------------------------------------------------------------------------------------------------------------------------
//   \n - Esta combinaci�n indica que hay un salto de l�nea, por ejemplo : TEXT("L�nea 1\nL�nea 2") = 'L�nea 1
//																									   L�nea 2'
//--------------------------------------------------------------------------------------------------------------------------------------------
//   \0 - Esta combinaci�n indica que se ha terminado la cadena, puede que encuentres mas cadenas detr�s
//--------------------------------------------------------------------------------------------------------------------------------------------

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#define VERSION			0.5f
#define NOMBRE_IDIOMA	TEXT("Catal�")       // Nombre del Idioma
//#define PATH_AYUDA		TEXT("ayuda\\ayuda.htm") // Nombre del archivo de ayuda


extern "C" _declspec(dllexport) const TCHAR *TextoIdioma(const int Num) {
	switch (Num) {
		case 0	   : return TEXT("Error carrega�n l�idioma.\nLa llibreria no te la mateixa versi� que el reproductor\n%s\nVersi� actual : %02f, Versi� Requerida : %02f\nPer solucionar el problema, reinstal�li el reproductor.");
		case 1     : return TEXT("Obrir Arxiu");
		case 2     : return TEXT("Base de Dades");
		case 3     : return TEXT("Equalitzador");
		case 4     : return TEXT("Veure V�deo");
		case 5     : return TEXT("Llista de reproducci�");
		case 6     : return TEXT("Buscar Arxius");
		case 7     : return TEXT("Opcions");
		case 8     : return TEXT("Ajuda");
		case 9     : return TEXT("Tecles rapides");
		case 10    : return TEXT("Associar Arxius");
		case 11    : return TEXT("Llista");
		case 12    : return TEXT("Aspecte");
		case 13    : return TEXT("Buscar Arxius");
		case 14    : return TEXT("Cancel�lar");
		case 15    : return TEXT("Can�� Enrere");
		case 16    : return TEXT("Play");
		case 17    : return TEXT("Pausa");
		case 18    : return TEXT("Stop");
		case 19    : return TEXT("Can�� Endavant");
		case 20    : return TEXT("Activat");
		case 21    : return TEXT("Desactivat");
		case 22    : return TEXT("Shufle");
		case 23    : return TEXT("Repeat");
		case 24    : return TEXT("Aquest reproductor no esta assignat per reproduir can�ons i v�deos per defecte en windows.\nVols que aquest reproductor s'utilitzi per defecte?");
		case 25    : return TEXT("El path especificat no es v�lid!!");
		case 26    : return TEXT("Error!");
		case 27    : return TEXT("Editar Anomalies");
		case 28    : return TEXT("Assignar Nom");
		case 29    : return TEXT("Modificar Nom");
		case 30    : return TEXT("Eliminar");
		case 31    : return TEXT("Inici");
		case 32	   : return TEXT("Revisar medis");
		case 33	   : return TEXT("V�deo");
		case 34	   : return TEXT("Mida");
		case 35	   : return TEXT("Repetir llista");
		case 36	   : return TEXT("Repetir llista + shufle");
		case 37	   : return TEXT("Estil aleatori");
		case 38	   : return TEXT("Grup aleatori");
		case 39	   : return TEXT("Disc aleatori");
		case 40	   : return TEXT("50 can�ons aleat�ries");
		case 41	   : return TEXT("Apagar windows");
		case 42	   : return TEXT("Apagar aquest reproductor");
		case 43	   : return TEXT("Buscar arxius a agregar");
		case 44	   : return TEXT("Revisar la base de dades");
		case 45	   : return TEXT("Arxiu");
		case 46	   : return TEXT("Nova Llista");
		case 47	   : return TEXT("Llista Buida");
		case 48	   : return TEXT("Can�ons amb 5 estrelles");
		case 49	   : return TEXT("Can�ons mes escoltades");
		case 50	   : return TEXT("Afegir a llista");
		case 51	   : return TEXT("Guardar Llista");
		case 52	   : return TEXT("Carregar arxiu de Subt�tols");
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
		case 64	   : return TEXT("Ordre de les can�ons");
		case 65	   : return TEXT("Tipus, Estil, Grup, Disc, Can�ons");
		case 66	   : return TEXT("Estil, Grup, Disc, Can�ons");
		case 67	   : return TEXT("Grup, Disc, Can�ons");
		case 68	   : return TEXT("Disc, Can�ons");
		case 69	   : return TEXT("Propietats");
		case 70	   : return TEXT("Buscar can�� en la base de dades");
		case 71	   : return TEXT("Modificar pista");
		case 72	   : return TEXT("Temps");
		case 73	   : return TEXT("Copiar can�ons a...");
		case 74	   : return TEXT("Can�� actual");
		case 75	   : return TEXT("Buscar can�� en la llista");
		case 76	   : return TEXT("Eliminar de la base de dades");
		case 77	   : return TEXT("Eliminar de la llista");
		case 78	   : return TEXT("Aleatori");
		case 79	   : return TEXT("Modificada");
		case 80	   : return TEXT("Agregar");
		case 81	   : return TEXT("Generar");
		case 82	   : return TEXT("Error carrega�n l'arxiu (%s)\nAquest error no es greu i el reproductor pot continuar l�execuci� del programa.\nReinstal�la el reproductor per solucionar el problema");
		case 83    : return TEXT("Analitzant :");
		case 84    : return TEXT("Arxius de Subt�tols\0*.srt\0");
		case 85    : return TEXT("Obrir Subt�tols");
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
		case 98    : return TEXT("Dades del tag versi� 1");
		case 99    : return TEXT("Dades del tag versi� 2");
		case 100   : return TEXT("Dades del tag ASF");
		case 101   : return TEXT("El total de valors trobats en els tags es de : %d");
		case 102   : return TEXT("Estils");
		case 103   : return TEXT("Grups");
		case 104   : return TEXT("Discs");
		case 105   : return TEXT("Can�ons");
		case 106   : return TEXT("Pressiona la nova tecla per aquesta acci�");
		case 107   : return TEXT("Editar");
		case 108   : return TEXT("Control + ");
		case 109   : return TEXT("Alt + ");
		case 110   : return TEXT("Shif + ");
		case 111   : return TEXT("Combinaci� de tecles INVALIDA.");
		case 112   : return TEXT("Acceptar");
		case 113   : return TEXT("No mostrar mes aquest missatge");
		case 114   : return TEXT("Buscar");
		case 115   : return TEXT("Obrir el seu directori");
		case 116   : return TEXT("Mostrar log");
		case 117   : return TEXT("Sobre BubaTronik...");
		case 118   : return TEXT("Medis reprodu�bles");
		case 119   : return TEXT("Arxius d��udio");
		case 120   : return TEXT("Arxius de v�deo");
		case 121   : return TEXT("Llistes");
		case 122   : return TEXT("Tots els arxius");
		case 123   : return TEXT("Desitges eliminar tamb� els medis del disc dur?\nPresiona ometre per eliminar-los nomes de la base de dades.");
		case 124   : return TEXT("Eliminar medis");
		case 125   : return TEXT("Ometre");
		case 126   : return TEXT("Ajuda r�pida");
		case 127   : return TEXT("�udio");
		case 128   : return TEXT("CDAudio");
		case 129   : return TEXT("Selecciona el tipus de norma");
		case 130   : return TEXT("Llista de can�ons");
		case 131   : return TEXT("Revisi� de la Base de dades.");
		case 132   : return TEXT("\\Estil\\Grup\\Disc\\");
		case 133   : return TEXT("\\Grup\\Disc\\");
		case 134   : return TEXT("\\Disc\\");
		case 135   : return TEXT("Medi Seg�ent");
		case 136   : return TEXT("Medi Anterior");
		case 137   : return TEXT("Grup desconegut");
		case 138   : return TEXT("Disc desconegut");
		case 139   : return TEXT("CD d��udio (%s)");
		case 140   : return TEXT("Estil seleccionat : #%s#");
		case 141   : return TEXT("#Modificada#");
		case 142   : return TEXT("Estil aleatori #%s#");
		case 143   : return TEXT("Estil suggerit #%s#");
		case 144   : return TEXT("Grup aleatori #%s#");
		case 145   : return TEXT("Grup suggerit #%s#");
		case 146   : return TEXT("Disc aleatori #%s#");
		case 147   : return TEXT("Disc suggerit #%s#");
		case 148   : return TEXT("#%d Can�ons aleat�ries#");
		case 149   : return TEXT("#Can�ons amb 5 estrelles#");
		case 150   : return TEXT("#Les %d can�ons mes escoltades#");
		case 151   : return TEXT("Estil mes escoltat #%s#");
		case 152   : return TEXT("Grup mes escoltat #%s#");
		case 153   : return TEXT("Disc mes escoltat #%s#");
		case 154   : return TEXT("Error greu!!");
		case 155   : return TEXT("Enviar");
		case 156   : return TEXT("Acabar");
		case 157   : return TEXT("L'operaci� s'ha completat correctament.");
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
		case 171   : return TEXT("Retroc�s");
		case 172   : return TEXT("Insert");
		case 173   : return TEXT("Suprimir");
//		case 174   : return TEXT("Inici");
		case 175   : return TEXT("Fi");
		case 176   : return TEXT("Esc"); 
		case 177   : return TEXT("Shift");
		case 178   : return TEXT("Imprimir Pantalla");
		case 179   : return TEXT("Arxiu d��udio");
		case 180   : return TEXT("Arxiu de v�deo");
		case 181   : return TEXT("Numeraci�");
		case 182   : return TEXT("Ample");
		case 183   : return TEXT("Altura"); 
		case 184   : return TEXT("Dades del tag Vorbis :");
	
		// ControlPresentacionEx::Mostrar_Inicio
		case 1000  : return TEXT("Benvingut a %s %s");
		case 1001  : return TEXT("Llista actual : %s");
		case 1002  : return TEXT("No hi ha suggeriments per la base de dades.");
		case 1003  : return TEXT("Numero de can�ons #%d# , Temps total de la llista #%s#.");
		case 1004  : return TEXT("%d Estils, %d Grups, %d Discs, %d Can�ons i %d V�deos.");
		case 1005  : return TEXT("Estil mes escoltat $%s$ amb un total de #%d# cops.");
		case 1006  : return TEXT("Grup mes escoltat $%s$ amb un total de #%d# cops.");
		case 1007  : return TEXT("Disc mes escoltat $%s$ amb un total de #%d# cops.");
		case 1008  : return TEXT("Suggeriments: $%s$ y $%s$");
		case 1009  : return TEXT("Sense suggeriments.");
		case 1010  : return TEXT("Generador de llistes :");
		case 1011  : return TEXT("Llista amb 50 can�ons basada en un estil aleatori");
		case 1012  : return TEXT("Llista amb 50 can�ons basada en un grup aleatori");
		case 1013  : return TEXT("Llista amb 50 can�ons basada en un disc aleatori");
		case 1014  : return TEXT("Llista amb 50 can�ons aleat�ries");
		case 1015  : return TEXT("Llista amb les can�ons que tenen 5 estrelles");
		case 1016  : return TEXT("Llista amb les 50 can�ons mes escoltades");
		case 1017  : return TEXT("Medis oberts recentment :");
		// ControlPresentacionEx::Mostrar_CargandoVLC
		case 1020  : return TEXT("Carregant nucli de la LIBVLC %s ....");
		// ControlPresentacionEx::Mostrar_SobreBubaTronik
		case 1030  : return TEXT("BubaTronik es una aplicaci� que va n�ixer davant la necessitat de ordenar i reproduir medis f�cilment. Ames es el exemple mes complet que existeix sobre la llibreria DWL.");
		case 1031  : return TEXT("$BubaTronik$, la $DWL$ y $www.devildrey33.es$ son propietat de #Josep Antoni Bover Comas#.");
		case 1032  : return TEXT("Si desitgeu posar-vos en contacte amb l'autor podeu fer-ho enviant un missatge en el $f�rum$ de www.devildrey33.es.");
		case 1033  : return TEXT("Hi ha molta gent que ha col�laborat activa i passivament en aquest projecte cosa que haig d'agrair. En especial les seg�ents persones han contribu�t �ltimament en el desenvolupament :");
		case 1034  : return TEXT("- #Barba# Sempre dona bones opinions per problemes poc comuns.");
		case 1035  : return TEXT("- #Fong# Aportaci� de varies idees i una valuosa zona de proves.");
		case 1036  : return TEXT("- #Michel# Aportaci� de varies ideas i una valuosa zona de proves.");
		case 1037  : return TEXT("- #Parris# Un altre valuosa zona de proves.");
		case 1038  : return TEXT("- $Txema$ Un altre valuosa zona de proves.");
		case 1039  : return TEXT("- Tamb� haig d'agrair a tota la gent que m'ha donat suport durant aquest projecte. #Gracies a TOTS!#.");
		case 1040  : return TEXT("Per �ltim tamb� haig de mencionar que aquest reproductor utilitza 2 llibreries per complir amb la seva funci�, y son :");
		case 1041  : return TEXT("#FMOD EX# 4.26 Potent llibreria d��udio utilitzada en molts jocs actuals.");
		case 1042  : return TEXT("Pots trobar-la en el seg�ent enlla� : $http://www.fmod.org$");
		case 1043  : return TEXT("#LIBVLC# %s Llibreria per reproduir pr�cticament qualsevol medi.");
		case 1044  : return TEXT("Pots trobar-la en el seg�ent enlla� : $http://www.videolan.org$");
		case 1045  : return TEXT("Dades addicionals");
		case 1046  : return TEXT("BubaTronik #%.03f# #%s# compilat amb #MSVC %d# el %s.");
		case 1047  : return TEXT("Versi� de windows #%d.%d# Service Pack #%d.%d#");
		// ControlPresentacionEx::Mostrar_Opciones_Lista
		case 1050  : return TEXT("Llista inicial");
		case 1051  : return TEXT("- Escull eix que ha de fer el reproductor si s'inicia amb una llista buida. Per defecte s�iniciar� amb una llista buida, si no se li pasa cap parametre.");
		case 1052  : return TEXT("- #NOTA# : si escull eixes la opci� 'Qualsevol cosa', el reproductor seleccionar� una de las opcions de la llista aleat�riament, #exceptuant# la opci� '#Llista buida#'");
		case 1053  : return TEXT("Qualsevol cosa.");
		case 1054  : return TEXT("Excloure can�ons");
		case 1055  : return TEXT("- Aquesta opci� et permet excloure les can�ons que tinguin una estrella de totes les llistes aleat�ries que genera el reproductor, aixi pots controlar aquelles can�ons que no vols afegir.");
		case 1056  : return TEXT("Excloure can�ons amb una estrella.");
		// ControlPresentacionEx::Mostrar_Opciones_BD
		case 1060  : return TEXT("Mostrar tags");
		case 1061  : return TEXT("- Si actives aquesta opci�, en els tooltips informatius es mostraran totes les dades del tag.");
		case 1062  : return TEXT("Mostrar Tags en els Tooltips.");
		// ControlPresentacionEx::Mostrar_Opciones_Video
		case 1070  : return TEXT("Opcions de V�deo");
		case 1071  : return TEXT("- Aquesta opci� ocultar� el mouse, quan el reproductor estigui en mode pantalla completa, i l'usuari estigui mes de 2 segons sense fer res.");
		case 1072  : return TEXT("Ocultar mouse en pantalla completa.");
		// ControlPresentacionEx::Mostrar_Opciones_Apariencia
		case 1080  : return TEXT("Mida de la font");
		case 1081  : return TEXT("- Selecciona la mida de la font que s�utilitzar� en el reproductor.");
		case 1082  : return TEXT("Configuraci� de colors");
		case 1083  : return TEXT("- Escull eix la configuraci� de colors que prefereixis pel reproductor.");
		case 1084  : return TEXT("#NOTA :# Pots crear/editar una configuraci� de colors, editant els arxius amb extensi� '.BubaTronik_Skin' i modificant els seus valors RGB pels colors. Aquests arxius els trobar�s en la subcarpeta 'Graficos' situada en la arrel del reproductor. Per mes informaci� edita un arxiu, y llegeix les instruccions que hi ha dintre.");
		case 1085  : return TEXT("Mida de l'espectre gr�fic");
		case 1086  : return TEXT("- Selecciona la mida de les barres per l'espectre gr�fic del so.");
		case 1087  : return TEXT("Barra de t�tol");
		case 1088  : return TEXT("- Utilitzar barra de t�tol de windows (recomanat en windows vista i superiors).");
		case 1089  : return TEXT("Utilitzar barra de t�tol de windows.");
		// ControlPresentacionEx::Mostrar_Opciones
		case 1090  : return TEXT("Idioma");
		case 1091  : return TEXT("- Escull eix quin idioma prefereixes que es mostri en el reproductor. Per defecte s�escull eix el mateix que el sistema operatiu si esta disponible.");
		case 1092  : return TEXT("Actualizacions");
		case 1093  : return TEXT("- Buscar a internet una nova versi� del reproductor al iniciar. En el cas de trobar una nova versi�, es mostrar� un missatge que et permetr� descarregar i instal�lar la nova actualitzaci�.");
		case 1094  : return TEXT("Buscar Actualitzacions");
		case 1095  : return TEXT("Reproductor sempre davant");
		case 1096  : return TEXT("- Aquesta opcio et permet tenir per davant de totes les altres finestres la finestra del reproductor.");
		case 1097  : return TEXT("Tipus d'arxiu");
		case 1098  : return TEXT("- Els tipus d'arxiu que es veuen a continuaci� son els tipus que utilitzar� el reproductor tant per buscar arxius, como per associar-se a aquells tipus per reproduir-los per defecte. Pots eliminar els tipus que no t�interessin.");
		case 1099  : return TEXT("Eliminar �udio");
		case 1100  : return TEXT("Eliminar v�deo");
		case 1101  : return TEXT("Eliminar seleccionats");
		case 1102  : return TEXT("Restaurar tipus");
		case 1103  : return TEXT("Associar tipus");
		case 1104  : return TEXT("- Aquesta opci� et permet associar els tipus d'arxiu de la llista anterior al reproductor. D'aquesta forma quan facis dobleclick en l'explorador a un arxiu que correspongui a algun dels tipus acceptats s�engegar� aquest reproductor per defecte.");
		case 1105  : return TEXT("Preguntar al iniciar, quan sigui necessari.");
		case 1106  : return TEXT("Sempre.");
		case 1107  : return TEXT("Mai.");
		case 1108  : return TEXT("- Selecciona vols que s'executin els arxius per defecte.");
		case 1109  : return TEXT("Afegir a la llista.");
		case 1110  : return TEXT("Reproduir.");
		// ControlPresentacionEx::Mostrar_Opciones_Teclado
		case 1120  : return TEXT("Tecles d�acc�s r�pid");
		case 1121  : return TEXT("- Pots canviar les tecles per qualsevol acci� de les que es llisten a continuaci�. Es possible combinar les tecles Control i Shift amb altres tecles sense cap problema. Tamb� pots utilitzar algunes combinacions amb la tecla Alt, sempre que la tecla control estigui pressionada, per� no es molt recomanable perqu� moltes aplicacions desplegaran men�s emergents.");
		case 1122  : return TEXT("Play / Pausa");
		case 1123  : return TEXT("Pujar Volum");
		case 1124  : return TEXT("Baixar Volum");
		case 1125  : return TEXT("Reproduir medi seg�ent");
		case 1126  : return TEXT("Reproduir medi anterior");
		case 1127  : return TEXT("Obtenir teclat fora de l�aplicaci�");
		case 1128  : return TEXT("- Marca aquesta opci� per que el reproductor detecti les tecles d�acc�s r�pid fins i tot sota un joc. #NOTA#, si maques aquesta opci� es recomanable que t�asseguris de que les combinacions de tecles no s'utilitzin en altres programes / jocs.");
		case 1129  : return TEXT("Obtenir teclat quan el reproductor no estigui actiu.");
		// NOTA DEJAMOS 10 ESPACIOS MAS, PORQUE ES PROBABLE QUE ACABE MODIFICANDO ESTA ESCENA
		// ControlPresentacionEx::Mostrar_RevisarBD 
		case 1140  : return TEXT("Desde aquest panell pots revisar la base de dades per determinar si s'han d�eliminar medis dels discs.");
		case 1141  : return TEXT("Es important que revisis la base de dades quan eliminis can�ons desde fora del reproductor, ka que despr�s el reproductor podria afegir-les a les llistes aleat�ries.");
		case 1142  : return TEXT("Per �ltim tamb� pots fer una revisi� per determinar quins estils/grups/discs s�assemblen entre si.");
		case 1143  : return TEXT("Revisar anomalies en estils / grups / discs");
		// ControlPresentacionEx::Mostrar_BuscarArchivos
		case 1150  : return TEXT("El reproductor buscar� nous medis reprodu�bles en els directoris que es mostren en la llista de sota. Pots agregar o eliminar directoris al teu gust de la llista sempre que com a m�nim quedi un");
		case 1151  : return TEXT("(Path)");
		case 1152  : return TEXT("(Tag)");
		case 1153  : return TEXT("PATH\\Estil\\Grup\\Disc\\");
		case 1154  : return TEXT("PATH\\Grup\\Disc\\");
		case 1155  : return TEXT("PATH\\Disc\\");
		case 1156  : return TEXT("Indefinit");
		case 1157  : return TEXT("Selecciona un directori");
		case 1158  : return TEXT("Suggeriments");
		case 1159  : return TEXT("Selecciona quin tipus de norma hauries d'utilitzar per la estructuraci� del directori que has escollit.");
		case 1160  : return TEXT("1 - \\Estil\\Grup\\Disc\\ (Escaneig per Path)");
		case 1161  : return TEXT("2 - \\Grup\\Disc\\ (Escaneig per Path)");
		case 1162  : return TEXT("3 - \\Disc\\ (Escaneig per Tag)");
		case 1163  : return TEXT("4 - Una mica de tot / Indefinit (Escaneig per Tag)");
		case 1164  : return TEXT("Path abans del Tag");
		case 1165  : return TEXT("Tag abans del Path");
		case 1166  : return TEXT("Aquest proc�s pot trigar varis minuts depenen del ordinador i el numero d'arxius reprodu�bles que es trobin.");
		case 1167  : return TEXT("Filtrat de car�cters");
		case 1168  : return TEXT("An�lisis post escaneig");


		// Mensajes
		case 4000  : return TEXT("Error la unitat que conte el medi no es troba disponible,\nper aix� es impossible reproduir el medi.");
		case 4001  : return TEXT("Revisi� acabada, s'han eliminat un total de %d entrades invalides");
		case 4002  : return TEXT("Error el directori introdu�t no es v�lid, o ja existeix en la llista.");
		case 4003  : return TEXT("Desitges eliminar el M3U del disc? Aix� no afectar� als medis que contingui el M3U.");
		case 4004  : return TEXT("Bubatronik ha trobat un error cr�tic i ames no ha pogut crear un informe d'errors.....");
		case 4005  : return TEXT("La versi� de la base de dades no es v�lida per aquest reproductor.");
		case 4006  : return TEXT("No es pot carregar la base de dades!");
		case 4007  : return TEXT("Hi han %d medis que no s'han carregat perqu� la seva informaci� podria ser invalida i necessiten ser analitzats de nou.\nEs recomana executar la cerca d'arxius per tornar a tenir aquests medis en la base de dades.");
		case 4008  : return TEXT("Ignorada carrega d'alguns medis");
		case 4009  : return TEXT("BubaTronik ha trobat un error greu i es tancar�.");
		case 4010  : return TEXT("S'ha creat un informe d'errors, vols enviar-lo a www.devildrey33.es?");
		case 4011  : return TEXT("Pressiona el boto 'Enviar' per enviar el missatge i acabar, o pressiona el boto");
		case 4012  : return TEXT("'Acabar' per sortir sense enviar el missatge.");
		case 4013  : return TEXT("Informaci�");
		case 4014  : return TEXT("Informe d'errors enviat correctament.");
		case 4015  : return TEXT("Error enviant l'informe de errors....");
		case 4016  : return TEXT("S'ha trobat una nova actualitzaci� de BubaTronik, vols descarregar-la?? \n%s");
		case 4017  : return TEXT("Nova actualitzaci�!");
		case 4018  : return TEXT("El reproductor s�apagar� per actualizar-se, i es tornar� a engegar un cop acabada la actualitzaci�.\nPresiona intro per continuar.");
		case 4019  : return TEXT("Actualitzar!");
		case 4020  : return TEXT("Error descarregant la actualitzaci�, pots prova-ho mes tard, \no pots provar a baixar l�actualitzaci� manualment desde http://bubatronik.devildrey33.es");
		case 4021  : return TEXT("Comen�ant actualitzaci�");
		case 4022  : return TEXT("Error de descarrega");
		case 4023  : return TEXT("Error carrega�n el Winsock.");
		case 4024  : return TEXT("Error al obtenir la IP del servidor.");
		case 4025  : return TEXT("Error socket inv�lid.");
		case 4026  : return TEXT("Error conectan al servidor.");
		case 4027  : return TEXT("Error durant la transmissi� de dades.");
		case 4028  : return TEXT("Error!! la versi� del skin no es v�lida per aquest reproductor.");
		case 4029  : return TEXT("Error carrega�n el arxiu de skin (%s).\nNo hi ha suficients colors, i s'ha carregat una configuraci� per defecte.\nAquest error no es greu, i es pot continuar amb l�execuci� del programa.\nReinstal�la el reproductor per solucionar el problema.");
		case 4030  : return TEXT("No pots afegir medis externament mentres s'esta realitzan una cerca d'arxius.");

		// ToolTips :
		// -ID_GENERAR_LISTA
		case 5000  : return TEXT("Has d'elegir un tipus de llista per poder generar-la.");
		case 5001  : return TEXT("Al pressionar aquest bot�, crear�s una nova llista");
		case 5002  : return TEXT("de unes 50 can�ons del tipus especificat.");
		// -ID_TIPOS_LISTA
		case 5010  : return TEXT("Aquest control et permet elegir entre varis tipus de llistes prefabricades :");
		case 5011  : return TEXT("- Llista de can�ons aleat�ries basades en l'estil.");
		case 5012  : return TEXT("- Llista de can�ons aleat�ries basades en el grup.");
		case 5013  : return TEXT("- Llista de can�ons aleat�ries basades en el disc.");
		case 5014  : return TEXT("- Llista de can�ons aleat�ries sense basar-se en res.");
		case 5015  : return TEXT("- Llista amb les can�ons mes escoltades (si s'ha escoltat alguna)");
		case 5016  : return TEXT("- Llista amb can�ons de 5 estrelles (si s'ha assignat alguna)");
		case 5017  : return TEXT("NOTA : Aquestes llistes com a m�xim podr�n tenir 50 can�ons.");
		// -CP_SUGERENCIA_GENERO1 -CP_SUGERENCIA_GENERO2 -CP_SUGERENCIA_GRUPO1 -CP_SUGERENCIA_GRUPO2 -CP_SUGERENCIA_DISCO1 -CP_SUGERENCIA_DISCO2
		case 5020  : return TEXT("Cada cop que accedeixis a la secci� d'inici es mostraran nous suggeriments.");
		// -CP_SUGERENCIA_GENERO1 -CP_SUGERENCIA_GENERO2 -CP_GENERO_MAS
		case 5021  : return TEXT("Fes click per crear una nova llista amb varies can�ons de l'estil :");
		// -CP_SUGERENCIA_GRUPO1 -CP_SUGERENCIA_GRUPO2 -CP_GRUPO_MAS
		case 5022  : return TEXT("Fes click per crear una nova llista amb varies can�ons del grup :");
		// -CP_SUGERENCIA_DISCO1 -CP_SUGERENCIA_DISCO2 -CP_DISCO_MAS
		case 5023  : return TEXT("Fes click per crear una nova llista amb varies can�ons del disc :");
		// -CP_GENERO_MAS
		case 5024  : return TEXT("Per determinar quin estil s'ha escoltat es fa una llista amb tots els estils,");
		case 5025  : return TEXT("en la que es sumen els cops que s'ha escoltat cada can�� de l'estil.");
		// -CP_GRUPO_MAS
		case 5026  : return TEXT("Per determinar quin grup s'ha escoltat es fa una llista amb tots els grups,");
		case 5027  : return TEXT("en la que es sumen els cops que s'ha escoltat cada can�� del grup.");
		// -CP_DISCO_MAS
		case 5028  : return TEXT("Per determinar quin disc s'ha escoltat es fa una llista amb tots els discs,");
		case 5029  : return TEXT("en la que es sumen els cops que s'ha escoltat cada can�� del disc.");
		// -ID_FILTRO_COINCIDENCIAS
		case 5030  : return TEXT("Activant aquesta opci� el reproductor realitzar� una comparaci� entre tots els seus estils, grups, i discs");
		case 5031  : return TEXT("de forma que solucioni el 80% d'errors tipogr�fics en els tags.");
		case 5032  : return TEXT("Posem per exemple que inicialment es troben el seg�ents dos grups :");
		case 5033  : return TEXT("'IronMaiden' i ' Iron Maiden'");
		case 5034  : return TEXT("Si us fixeu son pr�cticament el mateix per� un te un espai al mig, i l'altre no.");
		case 5035  : return TEXT("Una cosa tan simple com aquesta fa que el reproductor interpreti aquests resultats com 2 grups diferents.");
		case 5036  : return TEXT("I per aix� s'ha creat aquesta opci�, per intentar corregir al m�xim aquest tipus d'errors.");
		case 5037  : return TEXT("NOTA :");
		case 5038  : return TEXT("Activant aquesta opci�, el temps total de la cerca d'arxius s�incrementar� notablement.");
		// -ID_FILTRO_CARACTERES
		case 5040  : return TEXT("Activant aquesta opci� el reproductor filtrar� de tots els noms els seg�ents car�cters :");
		case 5041  : return TEXT(": \\ / ' \" { } [ ] ? � ; % # @ ^ ames dels accents y signes de puntuaci�.");
		case 5042  : return TEXT("Amb aix� aconseguim molta mes precisi� a l'hora d'identificar els estils, grups i discs.");
		case 5043  : return TEXT("Es molt recomanable deixar activada aquesta opci�, ames de que no afectar� al temps de la cerca.");
		// -ID_BOTON_BUSCAR2
		case 5050  : return TEXT("Pressiona aquest boto per comen�ar la cerca.");
		case 5051  : return TEXT("Abans de fer-ho hauries d�assegurar-te que has afegit els directoris que vols buscar en la llista.");
		case 5052  : return TEXT("Pressiona aquest boto per cancel�lar la cerca.");
		case 5053  : return TEXT("Per buscar primer tens que agregar un directori a la llista, i elegir el tipus d'ordre.");
		// -ID_BOTON_AGREGAR_DIR
		case 5060  : return TEXT("Al pressionar aquest boto afegir�s el directori del quadre desplegable a la llista de directoris.");
		case 5061  : return TEXT("No es recomanable afegir la arrel de cap disc dur, exceptuant el cas de que aquell disc sigui");
		case 5062  : return TEXT("�nicament per a medis d��udio i v�deo.");
		// -ID_COMBO_DIR
		case 5070  : return TEXT("Desde aquest control desplegable pots buscar directoris per afegir a la llista.");
		case 5071  : return TEXT("Els directoris de la llista d'adalt son els que escanejar� el reproductor per trobar nous medis.");
		// -ID_DIRECTORIOS_BUSCAR
		case 5080  : return TEXT("Aquesta llista conte els directoris en els que el reproductor buscar� els nous medis reprodu�bles.");
		case 5081  : return TEXT("Per defecte s'han afegit els directoris predeterminats de windows, si desitges eliminar-les pots utilitzar");
		case 5082  : return TEXT("el boto amb una creu vermella situat a la part inferior dreta de aquesta llista.");
		case 5083  : return TEXT("Si vols agregar directoris, has d'utilitzar el control desplegable que hi ha mes abaix per seleccionar");
		case 5084  : return TEXT("el directori que vulguis, i per �ltim has de pressionar el boto agregar per insertarlo en aquesta llista.");
		// -ID_LISTA_IDIOMAS
		case 5090  : return TEXT("Aquesta llista conte els idiomes disponibles per aquest reproductor.");
		case 5091  : return TEXT("Si est�s interessat en agregar un nou idioma a aquesta aplicaci�, posat en contacte amb devldrey33");
		case 5092  : return TEXT("a traves del f�rum de http://www.devildrey33.es");
		// -ID_BUSCAR_ACTUALIZACIONES
		case 5100  : return TEXT("Activant aquesta opci� el reproductor es connectar� un cop al dia com a m�xim per comprovar si existeixen");
		case 5101  : return TEXT("noves actualitzacions per instal�lar.");
		// -ID_SIEMPRE_DELANTE
		case 5110  : return TEXT("Activant aquesta opci� el reproductor es quedar� sempre davant de les altres finestres.");
		// -ID_LISTA_TIPOS
		case 5120  : return TEXT("Aquesta llista conte tots els tipus d��udio i de v�deo que el reproductor accepta per defecte.");
		case 5121  : return TEXT("Pots eliminar aquelles extensions que no t�interessi que el reproductor accepti.");
		// -ID_BOTON_ELIMINAR_TIPOS_AUDIO
		case 5130  : return TEXT("Aquest boto eliminar� de la llista tots els tipus que siguin predefinits per �udio.");
		// -ID_BOTON_ELIMINAR_TIPOS_VIDEO
		case 5140  : return TEXT("Aquest boto eliminar� de la llista tots els tipus que siguin predefinits per v�deo.");
		// -ID_BOTON_ELIMINAR_TIPOS
		case 5150  : return TEXT("Aquest boto eliminar� de la llista tots els tipus que es trobin seleccionats en aquest moment.");
		// -ID_BOTON_RESTAURAR_TIPOS
		case 5160  : return TEXT("Aquest boto restaurar� tots els tipus que pot utilitzar BubaTronik per defecte.");
		// -ID_ASOCIAR_PREGUNTAR
		case 5170  : return TEXT("Seleccionant aquesta opci�, el reproductor mirar� cada cop que s�inici� si esta associat a els");
		case 5171  : return TEXT("tipus de la llista, en cas de no estar associat a algun d'aquets tipus el reproductor mostrar�");
		case 5172  : return TEXT("un missatge que et demanar� si vols associar el reproductor als tipus de la llista.");
		// -ID_ASOCIAR_SIEMPRE
//		case 5180  : Es el mateix texte que el 5170
		case 5181  : return TEXT("tipus de la llista, en cas de no estar associat a algun d'aquets tipus el reproductor s�associar�");
		case 5182  : return TEXT("a ells autom�ticament, sense preguntar res.");
		// -ID_ASOCIAR_NUNCA
		case 5190  : return TEXT("Seleccionant aquesta opci�, el reproductor ometr� escanejar el registre per determinar si esta");
		case 5191  : return TEXT("associat o no als tipus d'arxiu de la llista.");
		// -ID_ASOCIAR_ADD
		case 5200  : return TEXT("Al executar un arxiu associat al reproductor desde l'explorador, s�afegir� a la llista sense");
		case 5201  : return TEXT("interrompre la reproducci� actual.");
		// -ID_ASOCIAR_REP
		case 5210  : return TEXT("Al executar un arxiu associat al reproductor desde l'explorador, s�afegir� a la llista ");
		case 5211  : return TEXT("interrompent la reproducci� actual, i es posar� a reproduir el primer medi afegit desde");
		case 5212  : return TEXT("l'explorador.");
		// -ID_OPCIONES_FUENTE
		case 5220  : return TEXT("En aquesta llista pots seleccionar la mida de la font que mes s'ajusti a les teves necessitats.");
		case 5221  : return TEXT("Hi ha tres tipus de mida : petit (11px), mitja (13px), i gran (15px).");
		// -ID_OPCIONES_SKIN
		case 5230  : return TEXT("En aquesta llista pots seleccionar la configuraci� de colors que mes t'agradi.");
		case 5231  : return TEXT("Si desitges crear una configuraci� de colors per a BubaTronik i no ho veus molt clar,");
		case 5232  : return TEXT("pots consultar els teus dubtes en el f�rum de  http://www.devildrey33.es");
		// -ID_OPCIONES_OSCY
		case 5240  : return TEXT("En aquesta llista pots seleccionar la mida de les barretes del espectre gr�fic.");
		case 5241  : return TEXT("El petit fa 1 p�xel d'ample, el mitja fa 2 p�xels, i el gran fa 3 p�xels.");
		// -ID_OPCIONES_UTILIZAR_BARRA_WIN
		case 5250  : return TEXT("Marcant aquesta opci� el reproductor utilitzar� la barra de windows est�ndard amb la seva bora.");
		case 5251  : return TEXT("En principi el reproductor es va dissenyar per a windows XP, per lo que es va crear una barra de t�tol");
		case 5252  : return TEXT("diferent a la de windows que quedava mes be amb aquest reproductor.");
		case 5253  : return TEXT("Si est�s utilitzant windows vista o windows 7 es recomana utilitzar la barra de t�tol de windows");
		case 5254  : return TEXT("per no perdre les capacitats esteses de maximitzat parcial.");
		// -ID_OPCIONES_MOSTRAR_TAG
		case 5260  : return TEXT("Marcant aquesta opci� es carregaran al vol i es mostraran els tags de les can�ons al mostrar");
		case 5261  : return TEXT("el tooltip informatiu.");
		case 5262  : return TEXT("Amb el quart boto del mouse pots fer apar�ixer aquets tooltips dintre de la llista i de la");
		case 5264  : return TEXT("base de dades sense tenir que esperar 2 segons sobre de la can��.");
		// -ID_LISTA_INICIO
		case 5270  : return TEXT("En aquesta llista desplegable pots seleccionar que vols que faci el reproductor al iniciar-se :");
		case 5271  : return TEXT("- Crear una llista partint d'un estil aleatori amb un m�xim de 50 can�ons.");
		case 5272  : return TEXT("- Crear una llista partint d'un grup aleatori amb un m�xim de 50 can�ons.");
		case 5273  : return TEXT("- Crear una llista partint d'un disc aleatori amb un m�xim de 50 can�ons.");
		case 5274  : return TEXT("- Crear una llista aleat�ria amb un m�xim de 50 can�ons.");
		case 5275  : return TEXT("- (Qualsevol cosa) Una de les 4 opcions anteriors aleat�riament.");
		case 5276  : return TEXT("- Crear una llista buida.");
		case 5277  : return TEXT("Si el reproductor s'inicia desde l'explorador peque has obert un medi desde all�, la creaci�");
		case 5278  : return TEXT("de la llista aleat�ria s'ometr�.");
		// -ID_EXCLUIR_NOTA
		case 5280  : return TEXT("Marcant aquesta opci� el reproductor evitar� crear llistes aleat�ries amb can�ons que nomes tinguin una estrella.");
		case 5281  : return TEXT("El sistema de notes pels medis per defecte li dona 2 estrelles a tots els medis. Si vols que un medi tingui");
		case 5282  : return TEXT("mes nota (fins a 5 estrelles) pots canviar-li la nota utilitzant el men� desplegable de la llista o la base de dades.");
		case 5283  : return TEXT(""); // en castell� es necessiten 4 l�nees, aquesta la deixem buida
		// -ID_OPCIONES_TECLA_PLAY
		case 5290  : return TEXT("Desde aquest control pots seleccionar la tecla que s�utilitzar� per realitzar l�acci� : Play / Pausa");
		// -ID_OPCIONES_TECLA_STOP
		case 5300  : return TEXT("Desde aquest control pots seleccionar la tecla que s�utilitzar� per realitzar l�acci� : Stop");
		// -ID_OPCIONES_TECLA_VOLUMEN_MAS
		case 5310  : return TEXT("Desde aquest control pots seleccionar la tecla que s�utilitzar� per realitzar l�acci� : Pujar el volum");
		// -ID_OPCIONES_TECLA_VOLUMEN_MENOS
		case 5320  : return TEXT("Desde aquest control pots seleccionar la tecla que s�utilitzar� per realitzar l�acci� : Baixar el volum");
		// -ID_OPCIONES_TECLA_SIGUIENTE
		case 5330  : return TEXT("Desde aquest control pots seleccionar la tecla que s�utilitzar� per realitzar l�acci� : Medi seg�ent");
		// -ID_OPCIONES_TECLA_ANTERIOR
		case 5340  : return TEXT("Desde aquest control pots seleccionar la tecla que s�utilitzar� per realitzar l�acci� : Medi anterior");
		// -ID_OPCIONES_OCULTAR_MOUSE
		case 5350  : return TEXT("Marcant aquesta opci� en el mode pantalla completa (dintre de un v�deo) el mouse s�ocultar�");
		case 5351  : return TEXT("pasats 2 segons sense fer res. Aix� pasara sempre que el mouse no estigui en la part dels controls.");
		// -CP_URL_FMOD
		case 5360  : return TEXT("P�gina web d'aquesta fabulosa llibreria per �udio");
		// -CP_URL_LIBVLC
		case 5370  : return TEXT("P�gina web d'aquesta excel�lent llibreria per �udio i V�deo");
		// -CP_URL_DEVILDREY33
		case 5380  : return TEXT("P�gina web de devildrey33");
		// -CP_URL_MALEIT
		case 5390  : return TEXT("P�gina web del grup d'aquest crack a la bateria. MALE�T");
		// -CP_URL_BUBATRONIK
		case 5400  : return TEXT("P�gina web principal d'aquest reproductor");
		// -CP_URL_DWL
		case 5410  : return TEXT("P�gina web de la DReY Windows Lib");
		case 5411  : return TEXT("La DWL es una llibreria per fer aplicacions sota windows en C++");
		case 5412  : return TEXT("que es distribueix sota llic�ncia GPL.");
		// -CP_URL_FORO
		case 5420  : return TEXT("F�rum de http://www.devildrey33.es");
		// -ID_COMBO_ORDEN_DIR
		case 5430  : return TEXT("Aquestes normes s'utilitzen a l'hora d'obtindre dades d'un medi, per determinar millor les seves dades.");
		case 5431  : return TEXT("Si per exemple tenim el directori 'C:\\MP3' i dintre tenim m�sica estructurada segons 'Grup\\Disc' podem");
		case 5432  : return TEXT("trobar el cas en que ames existeixi un directori separat per cada cd, exemple :");
		case 5433  : return TEXT("C:\\MP3\\Baron rojo\\Baron al Rojo Vivo\\CD1");
		case 5434  : return TEXT("En aquest cas concret, si estem buscant dades basant-nos en el path i no s'ha elegit el tipus 'Grup\\Disc'");
		case 5435  : return TEXT("el reproductor pensar� que el nom del grup es 'Baron al Rojo Vivo' i el disc 'CD1' cosa que es incorrecta.");
		case 5436  : return TEXT("Primer has de seleccionar un directori abans de poder establir el tipus de norma a utilitzar.");
		// -ID_MARCA_PATH
		case 5440  : return TEXT("Marcant aquesta opci� el reproductor donar� prioritat a les dades del path per sobre de les dades del tag, pel directori que s'ha d'agregar.");
		case 5441  : return TEXT("Primer has de seleccionar un directori abans de poder establir la prioritat de les dades.");
		// -ID_MARCA_TAG
		case 5442  : return TEXT("Marcant aquesta opci� el reproductor donar� prioritat a les dades del tag per sobre de les dades del path, pel directori que s'ha d'agregar.");

			

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
