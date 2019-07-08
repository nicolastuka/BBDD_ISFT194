#include <iostream>
//#include <windows.h> hay quien dice que la necesita para poder usar a mysql???
#include <mysql.h>
#include <stdio.h>

using namespace std;

int main() {
	
	MYSQL *conector;//variable puntero de conexion
	MYSQL_RES *resultado;//matriz de resultados
	MYSQL_ROW fila;//
	
	char *servidor = "localhost";//direccion del servidor
	char *usuario = "root";//usuario del servidor
	char *pass = "1234"; //password del servidor
	char *database = "base_ejemplo";//nombre de la base de datos
	
	conector = mysql_init(NULL);//inicializo la variable de conexion
	
	//Realizo la conexion pasando los parametros, si falla hay mensaje de error
	if (!mysql_real_connect(conector, servidor, usuario, pass, database, 0, NULL, 0)) {
		int errorNum=mysql_errno(conector);//obtengo el numero de error y lo almaceno
		//un mensaje especializado para distintos problemas que surjan en un intento de conexion
		switch(errorNum){
			case 1045:cout<<"Usuario y/o password inválido"<<endl;break;
			case 1049:cout<<"Base de datos no encontrada en el servidor"<<endl;break;
			case 2003:cout<<"El servidor de base de datos parece estar apagado"<<endl;break;
			case 2005:cout<<"Servidor de Base de datos no encontrado"<<endl;break;
			default:cout<<"Ocurrió algún error, código de error: "<<errorNum<<endl;
		}
		//cout<<mysql_error(conector); muestra el mensaje propio del conector
		/*
		sale del programa, pero dependiendo del error podemos realizar otras
		tareas como por ejemplo crear la base si no existe, intentar reconexion, etc.
		*/
		return 0;
	}
	
	//Mi primera instruccion es para agregarme en la base de datos
	if(mysql_query(conector,"INSERT INTO personas(nombre,apellido,edad,direccion) VALUES('Fernando','Fernandez',31,'calle asfaltada 324')")){
		cout<<mysql_error(conector);
		return 0;
	}
	
	//Envío de consulta SQL, si falla mensaje del conector y salida del programa
	//generalmente la falla es una mala instruccion SQL
	if (mysql_query(conector, "SELECT * FROM personas")) {
		cout<<mysql_error(conector);
		return 0;
	}
	
	/*	la instruccion anterior arrojó un resultado probablemente, podemos 
		almacenarlo para operar con el
	*/
	resultado = mysql_use_result(conector);
	
	//por estética nomas...
	cout<<"Las personas registradas en la base de datos son:"<<endl;
	
	/*
		El resultado de una consulta siempre se trata de una coleccion de registros
		por ende debemos considerar que se trata de una matriz que debemos
		navegar o recorrer para mostrarla o realizar otras operaciones
		Esa matriz se almacena en la variable resultado y con la función 
		mysql_fetch_row mostramos cada registro obtenido de resultados. 
		En la variable fila se carga el registro actual a la que se accede en el 
		while hasta que no existan mas registros, las posiciones del vector fila
		corresponden con el numero de orden de campos en la tabla de base de
		datos
	*/
	while ((fila = mysql_fetch_row(resultado))){
		cout<<fila[0]<<"  "<<fila[1]<<"  "<<fila[2]<<"  "<<fila[3]<<"  "<<fila[4]<<endl;
	}
	
	//cerramos la conexion liberando el resultado obtenido de la consulta en 1er lugar
	mysql_free_result(resultado);
	mysql_close(conector);
	
	return 0;
}
