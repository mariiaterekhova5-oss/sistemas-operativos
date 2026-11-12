const int SIZE_BUFFER = 5;

const 
	int RED = 1; int BLACK = 2; int BLUE = 3; int YELLOW = 4; int GREEN = 5;
	int MAGENTA = 6; int WHITE = 7;
const
	int CIRCLE = 1; int LINE = 2; int RECTANGLE = 3; int TRIANGLE = 4; int RNDRECT = 6;

const
	int ID_DE_PROD = 20; int ID_DE_CONS = 30;

const int TIEMPO_DE_ESPERA = 50;

semaphore mutex = 1;
semaphore huecos = SIZE_BUFFER;
semaphore llenos = 0;

int buffer[SIZE_BUFFER];

int contProd = 0;
int contCons = 0;
int dato = 1;

void inicializarGraficos(){
	int i;
	for(i = 0; i < SIZE_BUFFER; i++){
		create(i + 1, RNDRECT, WHITE, 160, 40 + (i * 45), 70, 35);
	}
	create(ID_DE_PROD, TRIANGLE, YELLOW, 60, 45, 50, 40);
	create(ID_DE_CONS, CIRCLE, YELLOW, 280, 45, 45, 45);
}

void espera(int ms){
	int k;
	int espera;
	espera = 0;
	for(k = 0; k < ms; k = k + 1){
		espera++;
	}
}

void productor(){
	int i;
	for (i = 0; i < 15; i++){
		espera(TIEMPO_DE_ESPERA);

		wait(huecos);
		wait(mutex);
		
		buffer[contProd] = dato;
		dato = dato + 1;

		changecolor(contProd + 1, GREEN);

		changecolor(ID_DE_PROD, BLUE);
		changecolor(ID_DE_CONS, YELLOW);

		contProd = (contProd + 1) % SIZE_BUFFER;
		
		signal(mutex);
		signal(llenos);
	}
}

void consumidor(){
	int i;
	int dato_sacado;
	for (i = 0; i < 15; i++){
		wait(llenos);
		wait(mutex);

		dato_sacado=buffer[contCons];

		changecolor(contCons + 1, WHITE);

		changecolor(ID_DE_PROD, YELLOW);
		changecolor(ID_DE_CONS, MAGENTA);

		contCons=(contCons + 1) % SIZE_BUFFER;

		signal(mutex);
		signal(huecos);

		espera(TIEMPO_DE_ESPERA);
	}
}

void main(){
	int i;

	for( i = 0; i < SIZE_BUFFER; i++){
		buffer[i] = 0;
	}
	
	inicializarGraficos();

	cobegin{
		productor();
		consumidor();
	}
}