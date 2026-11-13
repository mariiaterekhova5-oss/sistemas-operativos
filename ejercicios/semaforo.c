const int SIZE_BUFFER = 5;

const int RED = 1; int BLACK = 2; int BLUE = 3; int YELLOW = 4; int GREEN = 5;
const int MAGENTA = 6; int WHITE = 7;
const int CIRCLE = 1; int LINE = 2; int RECTANGLE = 3; int TRIANGLE = 4; int RNDRECT = 6;

const int ID_DE_PROD = 20; int ID_DE_CONS = 30;

const int TIEMPO_DE_ESPERA = 5; 

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

	create(ID_DE_PROD, CIRCLE, BLACK, 60, 130, 45, 45);
	create(ID_DE_CONS, RNDRECT, BLACK, 280, 130, 50, 40);
}

void espera(int ms){
	int k;
	int j;
	int dummy;
	dummy = 0;
	for(k = 0; k < ms; k = k + 1){

		for(j = 0; j < 50; j = j + 1){
			dummy++;
		}
	}
}

// Función que imprime en la ventana "Console"
void imprimirEstado(int esProductor, int valor) {
	int k;
	if (esProductor == 1) {
		cout << "Productor inserta " << valor << " | Buffer: ";
	} else {
		cout << "Consumidor extrae " << valor << " | Buffer: ";
	}
	
	for (k = 0; k < SIZE_BUFFER; k = k + 1) {
		if (buffer[k] == 0) {
			cout << "- ";
		} else {
			cout << buffer[k] << " ";
		}
	}
	cout << "\n";
}

void productor(){
	int i;
	for (i = 0; i < 15; i++){
		espera(TIEMPO_DE_ESPERA);

		changecolor(ID_DE_PROD, YELLOW);

		wait(huecos);
		wait(mutex);
		
		changecolor(ID_DE_PROD, BLUE);

		buffer[contProd] = dato;
		imprimirEstado(1, dato);
		dato = dato + 1;
		
		changecolor(contProd + 1, GREEN);
		espera(TIEMPO_DE_ESPERA); 
		contProd = (contProd + 1) % SIZE_BUFFER;
		
		signal(mutex);
		signal(llenos);

		changecolor(ID_DE_PROD, BLACK); 
	}
}

void consumidor(){
	int i;
	int dato_sacado;
	for (i = 0; i < 15; i++){
		changecolor(ID_DE_CONS, YELLOW); 

		wait(llenos);
		wait(mutex);

		changecolor(ID_DE_CONS, RED); 
		dato_sacado = buffer[contCons];
		buffer[contCons] = 0; 
		imprimirEstado(0, dato_sacado); 
		
		changecolor(contCons + 1, WHITE); 
		espera(TIEMPO_DE_ESPERA); 

		contCons = (contCons + 1) % SIZE_BUFFER;

		signal(mutex);
		signal(huecos);

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