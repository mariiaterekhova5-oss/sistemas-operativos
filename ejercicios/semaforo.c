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
	
		create(i + 1, RECTANGLE, WHITE, 160, 40 + (i * 45), 70, 35);
	}

	create(ID_DE_PROD, CIRCLE, BLACK, 60, 45, 45, 45);

	create(ID_DE_CONS, RNDRECT, BLACK, 280, 45, 50, 40);
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

		// 1. Estado BLOQUEADO/ESPERANDO: Se pone AMARILLO antes de pedir semáforos
		changecolor(ID_DE_PROD, YELLOW);

		wait(huecos);
		wait(mutex);
		
		// 2. Estado SECCIÓN CRÍTICA: Consigue el mutex y se pone AZUL
		changecolor(ID_DE_PROD, BLUE);

		buffer[contProd] = dato;
		dato = dato + 1;

		// Llena el hueco del buffer (VERDE)
		changecolor(contProd + 1, GREEN);

		contProd = (contProd + 1) % SIZE_BUFFER;
		
		signal(mutex);
		signal(llenos);

		// 3. Estado INACTIVO: Sale de la sección crítica y vuelve a NEGRO
		changecolor(ID_DE_PROD, BLACK);
	}
}

void consumidor(){
	int i;
	int dato_sacado;
	for (i = 0; i < 15; i++){
		// 1. Estado BLOQUEADO/ESPERANDO: Se pone AMARILLO antes de pedir semáforos
		changecolor(ID_DE_CONS, YELLOW);

		wait(llenos);
		wait(mutex);

		// 2. Estado SECCIÓN CRÍTICA: Consigue el mutex y se pone ROJO
		changecolor(ID_DE_CONS, RED);

		dato_sacado=buffer[contCons];

		// Vacía el hueco del buffer (BLANCO)
		changecolor(contCons + 1, WHITE);

		contCons=(contCons + 1) % SIZE_BUFFER;

		signal(mutex);
		signal(huecos);

		// 3. Estado INACTIVO: Sale de la sección crítica y vuelve a NEGRO
		changecolor(ID_DE_CONS, BLACK);

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