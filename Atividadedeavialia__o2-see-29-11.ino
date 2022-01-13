#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (16UL * BAUD)) - 1)
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

char contador = 0;
char* sala = (char*)"\nMovimento na sala!";
char* cozinha = (char*)"\nMovimento na Cozinha!";
char* quarto1 = (char*)"\nMovimento no primeiro quarto!";
char* quarto2 = (char*)"\nMovimento no segundo quarto!";
char* alarmedes = (char*)"\nAlarmes desligados.";
char* salalig = (char*)"\nAlarmes da sala ligados.";
char* cozinhalig = (char*)"\nAlarmes da cozinha ligados.";
char* quarto1lig = (char*)"\nAlarmes da primeiro quarto ligados.";
char* quarto2lig = (char*)"\nAlarmes da segundo quarto ligados.";
char* zona1lig = (char*)"\nAlarmes da zona A ligados.";
char* zona2lig = (char*)"\nAlarmes da zona B ligados.";
char* tudolig = (char*)"\nTodos alarmes ligados.";

void USART_Init() {
  UBRR0L = BAUD_PRESCALE;
  UBRR0H = (BAUD_PRESCALE >> 8);
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
void USART_Tx(unsigned char dado) {
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = dado;
}
void USART_Tx_String (char *texto) {
  while (*texto != '\0')
  USART_Tx (*texto++);
}
char USART_Rx() {
  while (!(UCSR0A & (1<<RXC0)));
  return (UDR0);
}
ISR(INT0_vect) {
  contador++;
  if (contador > 7)
    contador = 0; 
  if (contador == 0)
    USART_Tx_String(alarmedes);
  if (contador == 1)
    USART_Tx_String(salalig);
  if (contador == 2)
    USART_Tx_String(cozinhalig);
  if (contador == 3)
    USART_Tx_String(quarto1lig);
  if (contador == 4)
    USART_Tx_String(quarto2lig);
  if (contador == 5)
    USART_Tx_String(zona1lig);
  if (contador == 6)
    USART_Tx_String(zona2lig);
  if (contador == 7)
    USART_Tx_String(tudolig);
}
void desligar(){
  PORTD &= ~(1 << PORTD3);
  PORTB &= ~(1 << PORTB5);
  PORTB &= ~(1 << PORTB3);
}
void vermelhobuzzer(){
  PORTD |= (1 << PORTD3);
  PORTB |= (1 << PORTB5);
}
void verdebuzzer(){
  PORTD |= (1 << PORTD3); 
  PORTB |= (1 << PORTB3);
}
int main() {
  USART_Init();
  DDRB |= (1 << DDB5) | (1 << DDB3);
  DDRD &= ~(1<< DDD2) | (1 << DDD3);
  DDRD &= ~(1<< DDD7) | ~(1<< DDD6) | ~(1<< DDD5) | ~(1<< DDD4);
  EICRA |= (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT0);
  while(1){
    SREG |= (1 << SREG_I);
    if (contador != 0){
      if ((PIND & (1 << PIND7)) == (1 << PIND7) && (contador == 1 || contador == 5 || contador == 7)){
        vermelhobuzzer();
        USART_Tx_String(sala);
      }
      if ((PIND & (1 << PIND6)) == (1 << PIND6) && (contador == 2 || contador == 5 || contador == 7)){
        vermelhobuzzer();
        USART_Tx_String(cozinha);
      }
      if ((PIND & (1 << PIND5)) == (1 << PIND5) && (contador == 3 || contador == 6 || contador == 7)){
        verdebuzzer();
        USART_Tx_String(quarto1);
      }
      if ((PIND & (1 << PIND4)) == (1 << PIND4) && (contador == 4 || contador == 6 || contador == 7)){
        verdebuzzer();
        USART_Tx_String(quarto2);
      }
      desligar();
    }
  }
}
