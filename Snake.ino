/*
TODO:
- Agregar las reglas para el juego
- Agregar pantalla de presentacion
- Agregar puntaje
- Mejorar y agregar funciones
*/

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Wire.h>
#include <SPI.h>

// Pines LCD
#define RST 12
#define CE  13
#define DC  11
#define DIN  10
#define CLK  9

//Pines botones
#define buttonUp 	2
#define buttonDown 	4
#define buttonLeft	5
#define buttonRight	3

Adafruit_PCD8544 display = Adafruit_PCD8544(CLK, DIN, DC, CE, RST);

int size = 4;	// tamaño de cada bloque en la pantalla (columnas, filas, cuerpo serpiente y comida)
int moveX=0, moveY=0;	// movimiento en X y Y de la serpiente	
char direction;		// direccion en que se mueve la serpiente
int snakeX[100];	// posiciones de la serpiente en X
int snakeY[100];	// posiciiones de la serpiente en Y
int frameRate = 400;	// velocidad a la que se actualiza el juego
unsigned long previousTime = 0;	

// divide la pantalla en columnas y filas respecto al tamaño de los bloques
int cols = random(display.width()/size);	// posicion de la comida en X, valor aleatorio
int rows = random(display.height()/size);	// posicion de la comida en Y, valor aleatorio
int tail = 0;	// tamaño de la serpiente
bool start = true;	// estado del juego para generar la primer comida

void setup(){
	display.begin();
	display.setContrast(60);
	display.clearDisplay();
	display.display();

	randomSeed(analogRead(5));

	pinMode(buttonUp, INPUT);
	pinMode(buttonDown, INPUT);
	pinMode(buttonLeft, INPUT);
	pinMode(buttonRight, INPUT);

	moveX = 0;	// posicion en X donde inicia la serpiente
	moveY = 0;	// posicion en Y donde inicia la serpiente
}

void loop(){ 
	unsigned long timeNow = millis(); 

	readInputs();

	// Calcula cada cuanto tiempo se actualiza la pantalla 
	if(timeNow - previousTime >= frameRate){
		previousTime = timeNow;
		display.clearDisplay();
		update();
		display.display();
	}
}

void createFood(){
	// Divide la pantalla en columnas y filas del tamaño de la serpiente
	cols = random(display.width()/size);
	rows = random(display.height()/size);
	// se multiplica el valor de las columnas y filas por el tamaño de la serpiente
	// y asi se ubica en una casilla exacta
	//display.drawRoundRect(cols*size, rows*size, size, size, 3, 1);
}

void readInputs(){
	// Lee el estado de los botones y les asigna una letra para ser comparada y saber la direccion
	if(!digitalRead(buttonRight)){
		direction = 'r';		
	}
	if(!digitalRead(buttonLeft)){
		direction = 'l';		
	}
	if(!digitalRead(buttonUp)){
		direction = 'u';		
	}
	if(!digitalRead(buttonDown)){
		direction = 'd';		
	}	
}

void update(){
	// Si la posicion X y Y son iguales a las de la comida, incrementa el tamaño de la 
	// serpiente	
	if(moveX==cols*size && moveY==rows*size || start){
		createFood();
		start = false;
		tail++;
	}
	// dependiendo de la direccion incrementa la velocidad basado en el tamaño de los bloques
	if(direction == 'r')
		moveX += size;
	if(direction == 'd')
		moveY += size;
	if(direction == 'l')
		moveX -= size;
	if(direction == 'u')
		moveY -= size;

	// La ultima posicion de la serpiente (cabeza) es igual al valor X y Y mas reciente 
	snakeX[tail] = moveX;	
	snakeY[tail] = moveY;

	// mueve el cuerpo de la serpiente 
	for(int i=0; i<=tail-1; i++){
		snakeX[i] = snakeX[i+1];
		snakeY[i] = snakeY[i+1];
	}
	
	// Muestra en pantalla la serpiente completa
	for(int i=0; i<=tail; i++){	
		display.fillRect(snakeX[i], snakeY[i], size, size, 1);
	}
	// Dibuja la comida con valores aleatorios para la posicion
	display.drawRoundRect(cols*size, rows*size, size, size, 1, 1);
}
