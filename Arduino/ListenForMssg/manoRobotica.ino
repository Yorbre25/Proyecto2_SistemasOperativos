#include <Servo.h> 
Servo Lado_izq;
Servo Lado_der;
Servo Base;
void reset_servos();
void setup() 
{
  Serial.begin(9600);
  Base.attach(3);
  Lado_izq.attach(5);
  Lado_der.attach(6);
  reset_servos();
 // leftRight.write(90);
  //backFront.write(180);
  //upDown.write(0);
}
void reset_servos(){
  Lado_der.write(20);
  Lado_izq.write(70);
  delay(2000);
  Base.write(20);
  delay(2000);
  Lado_der.write(60);
  delay(2000);
  //RESET DE LA POSICION
  Lado_der.write(10);
  Lado_izq.write(110);
  Base.write(115); 
  Serial.println("Reset");
  delay(2000);
}

void touch_key(int base, int izq, int der){
  Base.write(base);
  delay(4000);
  Lado_izq.write(izq);
  delay(4000);
  Lado_der.write(der);
  delay(4000);
  reset_servos();
}


void loop() {
  
  //MOTOR DERECHA va de 0 a 120; -> para mover hacia arriba el numero debe bajar el angulo.
  //motor izquierda va de 30 a 110; -> para mover hacia arriba el numero debe subir el angulo.
  //Motor de base va de 80 a 150; -> derecha bajar angulo; izquierda subir anglo, P 70 y Q 150
  //touch_key(73,30,90); //Tecla A
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    char incomingByte = Serial.read();
    if(incomingByte == 'a'){
      touch_key(143,40,150);
    }
    else if(incomingByte == 'b'){
      touch_key(111,0,90);
    }
    else if(incomingByte == 'c'){
      touch_key(127,0,90);
    }
    else if(incomingByte == 'd'){
      touch_key(123,60,120);
    }
    else if(incomingByte == 'e'){
      touch_key(125,40,50);
    }
    else if(incomingByte == 'f'){
      touch_key(115,66,150);
    }
    else if(incomingByte == 'g'){
      touch_key(100,70,150);
    }
    else if(incomingByte == 'h'){
      touch_key(90,65,150);
    }
    else if(incomingByte == 'i'){
      touch_key(76,45,150);
    }
    else if(incomingByte == 'j'){
      touch_key(78,65,150);
    }
    else if(incomingByte == 'k'){
      touch_key(70,55,150);
    }
    else if(incomingByte == 'l'){
      touch_key(63,50,150);
    }
    else if(incomingByte == 'm'){
      touch_key(84,30,150);
    }
    else if(incomingByte == 'n'){
      touch_key(93,30,150);
    }
    else if(incomingByte == 'o'){
      touch_key(70,40,90);
    }
    else if(incomingByte == 'p'){
      touch_key(61,30,90);
    }
    else if(incomingByte == 'q'){
      touch_key(139,30,70);
    }
    else if(incomingByte == 'r'){
      touch_key(115,50,150);
    }
    else if(incomingByte == 's'){
      touch_key(135,55,150);
    }
    else if(incomingByte == 't'){
      touch_key(103,53,150);
    }
    else if(incomingByte == 'u'){
      touch_key(85,48,90);
    }
    else if(incomingByte == 'v'){
      touch_key(110,30,60);
    }
    else if(incomingByte == 'w'){
      touch_key(133,40,90);
    }
    else if(incomingByte == 'x'){
      touch_key(135,0,90);
    }
    else if(incomingByte == 'y'){
      touch_key(95,50,90);
    }
    else if(incomingByte == 'z'){
      touch_key(77,24,90);
    }


    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }
}


