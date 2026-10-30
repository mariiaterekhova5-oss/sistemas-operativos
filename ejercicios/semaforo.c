int tamano_buffer=10;
semaphore e ,s, n;
void calculo(){
	int suma=1+1;
	printf(suma);
}

void calculoCon(){
	int suma=2+3;
	printf(suma);
}
void consumir_dato() {
    	printf("Consumidor procesando dato...\n");
}
void producir() {
   	 printf("Productor fabricando dato...\n");
}
void productor(){
    	while(1){
        		producir();
        		wait(e);
        		wait(s);
        		calculo();
        		signal(s);
        		signal(n);

    	}
}
void consumir(){
    	while(1){
        		wait(n);
        		wait(s);
        		calculoCon();
        		signal(s);
        		signal(e);
        		consumir_dato();
    	}
}



void main(){
    	initialsem(s, 1);
    	initialsem(n, 0);
    	initialsem(e, tamano_buffer);
    	cobegin{
		    productor();
        	consumidor();
	}
}