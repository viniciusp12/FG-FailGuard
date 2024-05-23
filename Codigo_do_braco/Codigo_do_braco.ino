/*
______                      ______      _           _   _           
| ___ \                     | ___ \    | |         | | (_)          
| |_/ /_ __ __ _  ___ ___   | |_/ /___ | |__   ___ | |_ _  ___ ___  
| ___ \ '__/ _` |/ __/ _ \  |    // _ \| '_ \ / _ \| __| |/ __/ _ \ 
| |_/ / | | (_| | (_| (_) | | |\ \ (_) | |_) | (_) | |_| | (_| (_) |
\____/|_|  \__,_|\___\___/  \_| \_\___/|_.__/ \___/ \__|_|\___\___/ 
                                                               
*/

// ------ Não modifique ------
// Pino de permissão
#define EN        8
// Pinos de direção
#define X_DIR     5
#define Y_DIR     6
#define Z_DIR     7
// Pinos de passo
#define X_STP     2
#define Y_STP     3
#define Z_STP     4
// ---------------------------

// -------- Variáveis --------
// Número de passos
int steps = 100; // 100 = 1, 50 = 0.5

// Arrays que armazenam as coordenadas da posição atual e do alvo do braço robótico. Seguem esse padrão [Motor da Base, Motor da Junta 1, Motor da Junta 2]
float target_coordinates[3];
float actual_coordinates[3];
// ---------------------------


void setup()
{
  Serial.begin(9600);

  // Configuração dos pinos. Não modifique
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(EN, OUTPUT);
}

void set_target(float x, float y, float z) {
  /*
  Define a posição do alvo a ser alcançado pelo braço robótico.
  x -> Motor da base
  y -> Junta 1
  z -> Junta 2
  */
  target_coordinates[0] = steps * x;
  target_coordinates[1] = steps * y;
  target_coordinates[2] = steps * z;

  Serial.println("--- Target coordinates ---");
  Serial.println("X: " + String(target_coordinates[0]));
  Serial.println("Y: " + String(target_coordinates[1]));
  Serial.println("Z: " + String(target_coordinates[2]));
  Serial.println("--------------------------");
}


void move_Base() {
  /*
  Movimenta a base do braço
  Motor da base | HIGH = Direita / LOW = Esquerda
  */

  bool right;

  // Analiza a posição atual e define se o braço vai para a direita ou para a esquerda.
  if (actual_coordinates[0] > target_coordinates[0]) {
    digitalWrite(X_DIR, LOW);
    right = false;
  } else if (actual_coordinates[0] < target_coordinates[0]) {
    digitalWrite(X_DIR, HIGH);
    right = true;
  }
  
  // Movimenta a base para a coordenada do alvo
  while (actual_coordinates[0] != target_coordinates[0]) {
    digitalWrite(X_STP, HIGH); // Execution of a step.
    delayMicroseconds(1500);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1500);

    if (right) {
      actual_coordinates[0]++;
    } else {
      actual_coordinates[0]--;
    }
  }
}


void move_Joint1() {
  /*
  Movimenta a Junta 1
  Como os tem 2 motores, um deles deve ser negativo enquanto o outro é positivo.

   _           _      |   _          _
  |_|         |_|     |  |_|        |_|
  HIGH        LOW     |  LOW        HIGH
  O braço se move     |  O braço se move 
   para baixo         |  para cima
  */

  bool down;

   // Analiza a posição atual e define se o braço deve subir ou descer.
  if (actual_coordinates[1] > target_coordinates[1]) {
    digitalWrite(Y_DIR, HIGH);
    digitalWrite(Z_DIR, LOW);
    down = false;
  } else if (actual_coordinates[1] < target_coordinates[1]) {
    digitalWrite(Y_DIR, LOW);
    digitalWrite(Z_DIR, HIGH);
    down = true;
  }
  
  
  // Movimenta a junta para a coordenada do alvo
  while (actual_coordinates[1] != target_coordinates[1]) {
    digitalWrite(Y_STP, HIGH); 
    digitalWrite(Z_STP, HIGH);
    delayMicroseconds(1500);
    digitalWrite(Z_STP, LOW);
    digitalWrite(Y_STP, LOW);
    delayMicroseconds(1500);

    if (down) {
      actual_coordinates[1]++;
    } else {
      actual_coordinates[1]--;
    }
  }
}

void move_block1() {
  /*
  Move o blobo 1 (Base e junta 1).
  */
  set_target(2,2,0);

  digitalWrite(EN, LOW); 
  
  // Define as direções dos motores
  digitalWrite(Y_DIR, LOW);
  digitalWrite(Z_DIR, HIGH);
  digitalWrite(X_DIR, HIGH);

  // Move os motores para as coordenadas
  while (actual_coordinates[0] != target_coordinates[0]){
    digitalWrite(X_STP, HIGH); // Execution of a step.
    delayMicroseconds(1500);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1500);

    actual_coordinates[0]++;

    if (actual_coordinates[1] != target_coordinates[1]){
      digitalWrite(Y_STP, HIGH); 
      digitalWrite(Z_STP, HIGH);
      delayMicroseconds(1500);
      digitalWrite(Z_STP, LOW);
      digitalWrite(Y_STP, LOW);
      delayMicroseconds(1500);

      actual_coordinates[1]++;
    }
  }

  set_target(0,0,0);
  delay(1000);

  // Define a direção dos motores
  digitalWrite(Y_DIR, HIGH);
  digitalWrite(Z_DIR, LOW);
  digitalWrite(X_DIR, LOW);

  // Move os motores para as coordenadas
  while (actual_coordinates[0] != target_coordinates[0]){
    digitalWrite(X_STP, HIGH); // Execution of a step.
    delayMicroseconds(1500);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1500);

    actual_coordinates[0]--;

    if (actual_coordinates[1] != target_coordinates[1]){
      digitalWrite(Y_STP, HIGH); 
      digitalWrite(Z_STP, HIGH);
      delayMicroseconds(1500);
      digitalWrite(Z_STP, LOW);
      digitalWrite(Y_STP, LOW);
      delayMicroseconds(1500);

      actual_coordinates[1]--;
    }

  }
  // Coloca os motores em standby
  digitalWrite(EN, HIGH);
}


void hello_world() {
  /*
  Realiza uma pequena movimentação de teste.
  */
  digitalWrite(EN, LOW);
  
  set_target(2,0,0);
  move_Base();

  delay(1000);
  set_target(0,0,0);
  move_Base();

  delay(1000);
  set_target(0,2,0);
  move_Joint1();

  delay(1000);
  set_target(0,0,0);
  move_Joint1();

  digitalWrite(EN, HIGH);
}

void loop() {
  hello_world();
  delay(2000);
  move_block1();
  delay(2000);
}

/*
  ___              _                            
 / _ \            | |                           
/ /_\ \_ __   ___ | |_ __ _  ___ ___   ___  ___ 
|  _  | '_ \ / _ \| __/ _` |/ __/ _ \ / _ \/ __|
| | | | | | | (_) | || (_| | (_| (_) |  __/\__ \
\_| |_/_| |_|\___/ \__\__,_|\___\___/ \___||___/
                                                
usar milis

*/



// Programa de consulta (pode ter alguma coisa de util aqui em baixo)
/*void loop() {

  if (Serial.available() > 0)
  {
    data = Serial.parseInt();

    if (data > 0) {

      digitalWrite(EN, LOW); 
    
      target_coordinates[0] = steps * data;
      target_coordinates[1] = steps * data;
      Serial.println("-----------------------");
    
      Serial.println("X: " + String(target_coordinates[0]));
      Serial.println("Y: " + String(target_coordinates[1]));

      digitalWrite(Y_DIR, LOW); 
      digitalWrite(Z_DIR, HIGH);
      digitalWrite(X_DIR, HIGH); // Motor da base.

      while (actual_coordinates[0] != target_coordinates[0]){
        digitalWrite(X_STP, HIGH); // Execution of a step.
        delayMicroseconds(1500);
        digitalWrite(X_STP, LOW);
        delayMicroseconds(1500);

        actual_coordinates[0]++;

        if (actual_coordinates[1] != target_coordinates[1]){
          digitalWrite(Y_STP, HIGH); 
          digitalWrite(Z_STP, HIGH);
          delayMicroseconds(1500);
          digitalWrite(Z_STP, LOW);
          digitalWrite(Y_STP, LOW);
          delayMicroseconds(1500);

          actual_coordinates[1]++;
        }
      }

      delay(1000);
    
      target_coordinates[0] = 0;
      target_coordinates[1] = 0;
      Serial.println("-----------------------");

      Serial.println("X: " + String(target_coordinates[0]));
      Serial.println("Y: " + String(target_coordinates[1]));

      digitalWrite(Y_DIR, HIGH);
      digitalWrite(Z_DIR, LOW);
      digitalWrite(X_DIR, LOW); // Motor da base

      while (actual_coordinates[0] != target_coordinates[0]){
        digitalWrite(X_STP, HIGH); // Execution of a step.
        delayMicroseconds(1500);
        digitalWrite(X_STP, LOW);
        delayMicroseconds(1500);

        actual_coordinates[0]--;

        if (actual_coordinates[1] != target_coordinates[1]){
          digitalWrite(Y_STP, HIGH); 
          digitalWrite(Z_STP, HIGH);
          delayMicroseconds(1500);
          digitalWrite(Z_STP, LOW);
          digitalWrite(Y_STP, LOW);
          delayMicroseconds(1500);

          actual_coordinates[1]--;
        }

      }
      // Colocar os motores em standby
      digitalWrite(EN, HIGH);

    }
  }
}*/


/*

  Attention!

  The A axis is not indicated as it will execute the same movements (in parallel)
  that the axis that is selected through the hardware configuration of the CNC Shield performs.
  For more information:https://www.handsontec.com/dataspecs/cnc-3axis-shield.pdf

  This sketch does NOT include the command of the servomotors.
*/
