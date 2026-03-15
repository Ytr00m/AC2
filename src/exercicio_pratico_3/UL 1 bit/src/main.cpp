#include <Arduino.h>
int red_led = 13,
    yellow_led = 12,
    green_led = 11,
    blue_led = 10;

byte instrucao;
byte resultado;

byte lerInstrucao();
byte execInstrucao(byte);
void mostrarResultado(byte, byte);
byte AND(byte, byte);
byte OR(byte, byte);
byte NOT(byte);
byte XOR(byte, byte);
byte SOMA(byte, byte);

void setup()
{
  Serial.begin(9600);
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(blue_led, OUTPUT);

}

void loop()
{
  if (Serial.available() >= 3) {
    instrucao = lerInstrucao();
    resultado = execInstrucao(instrucao);
    mostrarResultado(instrucao, resultado);
  }

}
byte lerInstrucao() {
  char char_a = Serial.read();
  char char_b = Serial.read();
  char char_op = Serial.read();

  int a = char_a - '0';
  int b = char_b - '0';
  int op_code = char_op - '0';

  byte result = (a << 3) | (b << 2) | op_code;

  return result;
}

byte execInstrucao(byte instrucao) {
  byte a = (instrucao >> 3) & 1;
  byte b = (instrucao >> 2) & 1;
  byte op_code = instrucao & 0b0011;
  
  byte resultado = 0;

  switch(op_code) {
    case 0:
      resultado = AND(a, b);
      break;
    
    case 1:
      resultado = OR(a, b);
      break;

    case 2:
      resultado = NOT(a);
      break;

    case 3:
      resultado = SOMA(a, b);
      break;
  }

  return resultado;
}

void mostrarResultado(byte instrucao, byte resultado) {
  /**
   * a            -> led vermelho
   * b            -> led amarelo
   * resultado[0] -> led verde
   * resultado[1] -> led azul (carry out)
  */
  byte a = (instrucao >> 3) & 1;
  byte b = (instrucao >> 2) & 1;

  byte result_ula = resultado & 1;
  byte carry_out = (resultado >> 1) & 1;

  digitalWrite(red_led, a);
  digitalWrite(yellow_led, b);
  digitalWrite(green_led, result_ula);
  digitalWrite(blue_led, carry_out);

  Serial.println(instrucao, HEX);
}


byte AND(byte a, byte b) {
  return a & b;
}

byte OR(byte a, byte b) {
  return a | b;
}

byte NOT(byte a) {
  return (~a) & 1;
}

byte SOMA(byte a, byte b) {
  byte aux = 0;
  byte carry_out = 0;
  
  aux = XOR(a, b);
  carry_out = AND(a, b);

  byte resultado = (carry_out << 1) | aux;

  return resultado;
}

byte XOR(byte a, byte b) {
  return ((~a) & b) | (a & (~b));
}