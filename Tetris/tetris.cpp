#include "miniwin.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <ctime>
using namespace std;
using namespace miniwin;

const int TAM=25;
const int FILAS=20;
const int COLUMNAS=10;
typedef int Tablero[COLUMNAS][FILAS];
struct Coord{
	int x,y;
};

struct Pieza{
	Coord orig;//bloque central o posici�n absoluta
	Coord perif[3]; //bloques perif�ricos o posiciones relativas
	int color;
	
	Coord posicion(int n) const;
};

Coord Pieza::posicion(int n)const {
	Coord ret={orig.x,orig.y};
	if(n!=0){
		ret.x +=perif[n-1].x;
		ret.y +=perif[n-1].y;
	}
	return ret;
		
	
}



void cuadrado(int x,int y){
	rectangulo_lleno(20+1+x*TAM,
	                 20+1+y*TAM,
					 20+x*TAM+TAM,
					 20+y*TAM+TAM);
}

void pinta_pieza(const Pieza& P){
	color(P.color);
	for(int i=0;i<4;i++){
		Coord c=P.posicion(i);
		cuadrado(c.x,c.y);
}
}

Coord rota_derecha(const Coord& c){
	Coord ret={-c.y,c.x};
	return ret;
}

Coord rota_izquierda(const Coord& c){
	Coord ret={c.y,-c.x};
	return ret;
}

void rota_derecha(Pieza& P){
	for(int i=0;i<3;i++){
	P.perif[i]=rota_derecha(P.perif[i]);
	}
}
void rota_izquierda(Pieza& P){
	for(int i=0;i<3;i++){
	P.perif[i]=rota_izquierda(P.perif[i]);
	}
}

void tablero_vacia(Tablero& T){
	for(int i=0;i<COLUMNAS;i++){
    	for(int j=0;j<FILAS;j++){
        	T[i][j]=NEGRO;
}
}
}

void tablero_pinta(const Tablero& T){
	for(int i=0;i<COLUMNAS;i++){
    	for(int j=0;j<FILAS;j++){
    		color(T[i][j]);
    		cuadrado(i,j);
    	}
    }
	
}

void tablero_incrusta_pieza(Tablero& T,const Pieza &P){
   for(int i=0;i<4;i++){
   Coord c=P.posicion(i);
   T[c.x][c.y]=P.color;

}
}

bool tablero_colision(const Tablero& T, const Pieza& P){
	for(int i=0;i<4;i++){
		Coord c=P.posicion(i);
		//Comprobar l�mites
		if(c.x<0 || c.x>=COLUMNAS ) return true;
		if(c.y<0 || c.y>=FILAS )    return true;
		if(T[c.x][c.y]!=NEGRO)      return true;
	}
	return false;
}

Coord perifs[7][3]={
	{{ 1,0 },{ 0,1 },{ 1,1 }},//cuadrado
	{{ 1,0 },{-1,1 },{ 0,1 }},//S
	{{ 0,1 },{ 1,1 },{-1,0 }},//2
	{{ 0,1 },{ 0,-1},{ 1,1 }},//L
	{{ 0,1 },{ 0,-1},{-1,1 }},//Lr
	{{-1,0 },{ 1,0 },{ 0,1 }},//T
	{{ 0,1 },{ 0,-1},{ 0,2 }},//Palito
};

void pieza_nueva(Pieza& P){
	P.orig.x=12;
	P.orig.y=2;
	P.color=1+rand()%6;
	int r=rand()%7;
	for(int i=0;i<3;i++){
		P.perif[i]=perifs[r][i];
	}
}

bool tablero_fila_llena(const Tablero& T,int fila){
	for(int i=0;i<COLUMNAS;i++)
	{if(T[i][fila]==NEGRO) return false;
	}
	return true;
}

void tablero_colapsa(Tablero& T,int fila){
	for(int j=fila;j>0;j--){
		for(int i=0;i<COLUMNAS;i++)
			T[i][j]=T[i][j-1];	
	}
	//Vaciar fila
	for(int i=0;i<COLUMNAS;i++)
	T[i][0]=NEGRO;
}


int tablero_cuenta_lineas(Tablero& T){
	int fila=FILAS-1,cont=0;
	while(fila>=0){
		if(tablero_fila_llena(T,fila))
		{
		tablero_colapsa(T,fila);
		cont++;
	}
		else 
	
		fila--;
	
	}
	return cont;
}

string a_string(int puntos){
	stringstream sout;
	sout<<puntos;
	return sout.str();
	
}
void repinta(const Tablero& T,const Pieza& p, const Pieza& sig,int puntos,int nivel){
	const int ancho=TAM*COLUMNAS;
	const int alto=TAM*FILAS;
	borra();
    tablero_pinta(T);
    color_rgb(128,128,128);
    linea(20,20,20,20+alto);
    linea(20,20+alto,20+ancho,20+alto);
    linea(20+ancho,20+alto,20+ancho,20);
    texto(40+ancho,20,"Pieza Siguiente");
    texto(40+ancho,150,"Nivel");
    texto(40+ancho,250,"Puntos");
    color(BLANCO);
    texto(40+ancho,270,a_string(puntos));
    texto(40+ancho,170,a_string(nivel+1));
	pinta_pieza(p);
	pinta_pieza(sig);
	refresca();
}

const int puntos_limite[10]={
30,45,60,70,75,80,85,90,95,100};

const int tics_nivel[10]=	{
35,30,25,20,15,12,10,6,4,2};

void game_over(){
		color(BLANCO);
		texto(140,240,"GAME OVER");
		refresca();
		espera(1000);
		vcierra();
	
}
int main(){
vredimensiona(TAM*COLUMNAS+220,TAM*FILAS+100);
srand(time(0)); //Inicializa la semilla del rand
int tic=0,puntos=0,nivel=0;
Tablero T;
tablero_vacia(T);
Pieza c,sig;
pieza_nueva(c);
pieza_nueva(sig);
c.orig.x=5;
repinta(T,c,sig,puntos,nivel);

int t=tecla();
while(t!=ESCAPE){
	//0.Guardar la pieza
	Pieza copia=c;
	if(t==NINGUNA && tic>tics_nivel[nivel]){
		tic=0;
		t=ABAJO;
	}
	
	if(t==int('N')){
		nivel++;
	}
	
	//1.Prueba el movimiento
	if(t==ABAJO) c.orig.y++;
	else if(t==ARRIBA) rota_derecha(c);
	else if(t==DERECHA) c.orig.x++;
	else if(t==IZQUIERDA) c.orig.x--;
	//2.Verificar colisi�n
	if(tablero_colision(T,c)){
		c=copia;
		if(t==ABAJO){
		tablero_incrusta_pieza(T,c);
		int cont=tablero_cuenta_lineas(T);
		puntos +=cont*cont;
		if(puntos>puntos_limite[nivel])
		nivel++;
		c=sig;
		pieza_nueva(sig	);
		c.orig.x=5;
		if(tablero_colision(T,c)){
		game_over();
			
		}
	}
	}
	
	//Repintar
	if(t!=NINGUNA){
	repinta(T,c,sig,puntos,nivel);
}
    espera(30);
    tic++;
	t=tecla();
		
 
}
vcierra();
return 0;
}
