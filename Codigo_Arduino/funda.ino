// --- PINES DEL SHIELD HW-130 ---
int ENABLE_595 = 7;   // Habilitación de salidas (OE#)
int MOTORCLK   = 4;   // Reloj (CLK)
int MOTORDATA  = 8;   // Datos (SER)
int MOTORLATCH = 12;  // Latch (RCLK)
int MOTOR1_PWM = 11;  // Velocidad de Motor 1 (PWM)

// --- PIN DE ENTRADA LÓGICA ---
int pinOptoacoplador = 2; // Pin que viene de tu circuito/optoacoplador

// Tiempo de movimiento para el cerrojo (Ajusta los milisegundos aquí)
int tiempoMovimiento = 300; 

bool cerrojoAbierto = false; 

// Función para enviar comandos al chip 74HC595 del HW-130
void enviarComandoMotor(byte comando) {
  digitalWrite(MOTORLATCH, LOW);
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, comando);
  digitalWrite(MOTORLATCH, HIGH);
}

void abrirCerrojo() {
  enviarComandoMotor(0b00000100); // Sentido 1
}

void cerrarCerrojo() {
  enviarComandoMotor(0b00001000); // Sentido 2 (Inverso)
}

void detenerMotor() {
  enviarComandoMotor(0b00000000); // Apagar motor
}

void setup() {
  // Configuración de pines del HW-130
  pinMode(ENABLE_595, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);

  // Despertar el chip del shield
  digitalWrite(ENABLE_595, LOW);
  analogWrite(MOTOR1_PWM, 255); // Fuerza máxima

  // Entrada de la contraseña
  pinMode(pinOptoacoplador, INPUT_PULLUP);
  
  detenerMotor();
  
  Serial.begin(9600);
  Serial.println("Sistema iniciado. Cerrojo cerrado.");
}

void loop() {
  int estadoContrasena = digitalRead(pinOptoacoplador);

  if (estadoContrasena == LOW && cerrojoAbierto == false) {
    Serial.println("Contraseña Correcta. Abriendo...");
    
    abrirCerrojo();
    delay(tiempoMovimiento); 
    detenerMotor();
    
    cerrojoAbierto = true; 
  } 
  else if (estadoContrasena == HIGH && cerrojoAbierto == true) {
    Serial.println("Contraseña cambiada/incorrecta. Cerrando...");
    
    cerrarCerrojo();
    delay(tiempoMovimiento); 
    detenerMotor();
    
    cerrojoAbierto = false; 
  }
}