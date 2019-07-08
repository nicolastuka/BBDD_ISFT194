#include <iostream>//flujos de entrada, salida y error
#include <stdio.h>//para hacer uso de la clase FILE
#include <cstdlib>//para hacer uso de malloc y realloc para la gestion de memoria
#include <conio2.h>//para ordenar un poco la pantalla

using namespace std;

/*	estructura que representa a un alumno, tambien es la estructura de registro 
	en fichero. Todos char para que sea más sencilla la captura de datos por 
	teclado, luego si es necesario se pueden castear los datos para darles
	alguna utilidad
*/
struct Alumno{
	int id;
	char anio[2];
	char seccion[2];
	char apellidos[30];
	char nombres[30];
};

int idAuxiliar;

Alumno *alumnos;

/*
	cuando abrimos el programa por segunda o mas veces y leemos el archivo
	donde almancenamos los datos es importante darle un tamaño inicial al 
	arreglo de alumnos. De este modo decimos que reserve tantas estructuras de
	alumno como cantidad de registros en archivo.
*/
void InicializarEstructura(){
	alumnos=(Alumno*) malloc(sizeof(Alumno)*idAuxiliar);
}

//Simplemente se dibuja el menu con las opciones
void DibujarMenu(){
	textbackground(WHITE);
	textcolor(BLACK);
	for(int i=1;i<81;i++){
		gotoxy(i,1);cout<<" ";
	}
	int indice=5;gotoxy(indice,1);cout<<"F1.-OPCIONES";
	indice+=16;gotoxy(indice,1);cout<<"F2.-ASISTENCIA";
	indice+=16;gotoxy(indice,1);cout<<"F3.-REGISTRAR";
	indice+=16;gotoxy(indice,1);cout<<"F4.-BAJAR";
	indice+=16;gotoxy(indice,1);cout<<"F5.-SALIR";
	textbackground(BLACK);
	textcolor(WHITE);
}

//Simplemente se dibuja la ficha en pantalla
void DibujarFichaRegistro(){
	gotoxy(17,3);cout<<"REGISTRO DE ALUMNOS";
	gotoxy(57,3);cout<<"LISTADO DE ALUMNOS";
	gotoxy(7,7);cout<<"EL ALUMNO CORRESPONDE AL REGISTRO: ";
	textbackground(RED);
	for(int i=0;i<45;i++){
		gotoxy(5+i,5);cout<<" ";
		gotoxy(5+i,20);cout<<" ";
	}
	for(int i=0;i<15;i++){
		gotoxy(5,5+i);cout<<" ";
		gotoxy(50,5+i);cout<<" ";
	}
	gotoxy(50,20);cout<<" ";
	textbackground(BLACK);
	gotoxy(7,9);cout<<"APELLIDOS:";
	gotoxy(7,11);cout<<"NOMBRES:";
	gotoxy(7,13);cout<<"AÑO:";
	gotoxy(7,15);cout<<"SECCION:";
	textbackground(LIGHTRED);
	for(int i=0;i<30;i++)gotoxy(18+i,9);cout<<" ";
	for(int i=0;i<30;i++)gotoxy(18+i,11);cout<<" ";
	gotoxy(18,13);cout<<"  ";
	gotoxy(18,15);cout<<"  ";
}

/*
	Lectura de datos por teclado, en esta oportunidad usamos la funcion gets
	para capturar datos, redimensionamos el arreglo de alumnos y un detalle
	particular, en el ejemplo del club deje un mensaje acerca de un funcionamiento
	anómalo... bueno, nadie mordió el anzuelo, las cargas por teclado se almacenan
	en un espacio de memoria que se denomina buffer, con fflush indicamos que lo
	limpie, de este modo no tenemos basura o lleno ese espacio para el almacenamiento
	de las teclas presionadas
*/
void ConsultarDatosTeclado(){
	fflush(stdin);
	Alumno aux;
	gotoxy(18,9);gets(aux.apellidos);
	gotoxy(18,11);gets(aux.nombres);
	gotoxy(18,13);gets(aux.anio);
	gotoxy(18,15);gets(aux.seccion);
	aux.id=idAuxiliar+1;
	alumnos=(Alumno*)realloc(alumnos,(idAuxiliar+1)*sizeof(Alumno));
	alumnos[idAuxiliar]=aux;
	idAuxiliar++;
	textbackground(BLACK);
}

//Sencillo, recorremos el vector de struct y lo mostramos en pantalla
void ImprimirListado(){
	textbackground(LIGHTRED);
	for(int i=0;i<idAuxiliar;i++){
		gotoxy(55,5+i);
		cout<<alumnos[i].id<<" - "<<alumnos[i].apellidos<<", "<<alumnos[i].nombres;
	}
	textbackground(BLACK);
}

/*
	Primer lectura de archivo que realiza el programa cuando se ejecuta, de este
	modo reconoce la cantidad de registros almacenados en el archivo.
	Observemos lo siguiente:
	*fopen arroja un valor booleano de acuerdo al éxito o no de la apertura del
	archivo, si es exitosa esa lectura se realizará una acción.
	*fseek posiciona el cursor (o punto donde iniciar las operaciones de archivo)
	en el final, sus parametros son el puntero de archivo, el corrimiento que hay
	que realizar a partir del tercer argumento y el tercer argumento que contiene
	en este caso una enumeracion que sirve para posicionarse al final del archivo.
	*idAuxiliar=posicion actual del cursor dividido el tamaño de una estructura:
		Ejemplo:
			idAuxiliar=204bytes/68bytes => idAuxiliar=3 o sea existen 3 registros
	*Siempre cerrar el archivo una vez finalizadas las operaciones de lectura/
	escritura.
	Si por alguna razón no se puede abrir el archivo idAuxiliar sera 0
*/
void RecuperarIndice(){
	if(FILE *archivo=fopen("datos.bin","rb")){
		fseek(archivo, 0, SEEK_END);
		idAuxiliar=ftell(archivo)/sizeof(Alumno);
		fclose(archivo);
	}else{
		idAuxiliar=0;
	}
}

/*
	Antes de recuperar los datos se obtuvieron la cantidad de registros, con eso
	ya sabemos que debemos dimensionar el arreglo alumnos a ese tamaño para que
	quepa el contenido del fichero.
	En este caso, empleamos fread para hacer la lectura, esta funcion requiere de
	cuatro argumentos:
	*Espacio en memoria donde almacenar los datos, en nuestro caso el arreglo de
	estructura de alumnos.
	*El tamaño de la estructura.
	*La cantidad de registros en el fichero (obtenida con idAuxiliar)
	*La fuente, el archivo de donde extrae los datos
*/
void RecuperarDatos(){
	alumnos=(Alumno*)realloc(alumnos,(idAuxiliar+1)*sizeof(Alumno));
	if(FILE *archivo=fopen("datos.bin","rb")){
		fseek(archivo, 0, SEEK_SET);//aquí indico que el cursor esté al inicio del fichero
		fread(alumnos,sizeof(struct Alumno),idAuxiliar,archivo);
		fclose(archivo);
	}else{
		gotoxy((80-26)/2,10);cout<<"No hay alumnos registrados";
	}
	ImprimirListado();
}

/*
	De forma similar a la lectura se realiza la escritura, simplemente reemplazamos
	la funcion fread por fwrite. Por comodidad siempre almacenamos al final del archivo
	por eso nos situamos allí con fseek
*/
void AlmacenarDatos(){
	if(FILE *archivo=fopen("datos.bin","wb")){
		fseek(archivo, 0, SEEK_END);
		fwrite(alumnos,sizeof(struct Alumno),idAuxiliar,archivo);
		fclose(archivo);
	}
}

//Cuerpo principal del programa
int main(int argc, char *argv[]) {
	RecuperarIndice();//Inmediatamente recupero el ultimo indice desde archivo
	InicializarEstructura();//inicializo la estructura con el mismo tamaño que tiene en archivo
	char opcionMenu=0;
	while(true){
		switch(opcionMenu){
		case 0:
			system("cls");
			gotoxy(1,2);cout<<"Cantidad de registros: "<<idAuxiliar;
			DibujarMenu();
			gotoxy(7,5);
			cout<<"Bienvenido al sistema de gestión áulica, desde el mismo podrá dar altas";
			gotoxy(10,6);
			cout<<"y bajas de alumnos. A su vez es posible llevar un control de";
			gotoxy(27,7);
			cout<<"asistencia de los mismos.";
			break;
		case 59:
			system("cls");
			gotoxy(1,2);cout<<"Cantidad de registros: "<<idAuxiliar;
			DibujarMenu();
			break;
		case 60:
			system("cls");
			gotoxy(1,2);cout<<"Cantidad de registros: "<<idAuxiliar;
			DibujarMenu();
			RecuperarDatos();
			ImprimirListado();
			break;
		case 61:
			system("cls");
			gotoxy(1,2);cout<<"Cantidad de registros: "<<idAuxiliar;
			DibujarMenu();
			ImprimirListado();
			DibujarFichaRegistro();
			ConsultarDatosTeclado();
			AlmacenarDatos();
			gotoxy(1,2);cout<<"Cantidad de registros: "<<idAuxiliar;
			break;
		case 62:
			system("cls");
			gotoxy(1,2);cout<<"Cantidad de registros: "<<idAuxiliar;
			DibujarMenu();
			break;
		case 27:
		case 63:
			system("cls");
			gotoxy(25,2);
			cout<<"Gracias por usar la aplicacion.";
			gotoxy(18,3);
			cout<<"Presione cualquier tecla para cerrar la consola.";
			gotoxy(1,22);
			exit(EXIT_SUCCESS);
		}
		//evitamos imprimir la opcion y ademas no espera un ENTER
		//luego en los case del switch de esta funcion estan los códigos de tecla
		//de funcion F1, F2, F3...
		opcionMenu=getch();
	}
	return 0;
}

