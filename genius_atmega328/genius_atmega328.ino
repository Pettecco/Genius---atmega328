#define LEDVM (1<<PD0) //led vermelho -> PCINT16
#define LEDVD (1<<PC5) //led verde -> PCINT13
#define LEDAZ (1<<PB5) //led azul -> PCINT5
#define BOTVM (1<<PD1) //botão do led vermelho -> PCINT17 -> PORTAL 2
#define BOTVD (1<<PC1) //botão do led verde -> PCINT9 -> PORTAL 1
#define BOTAZ (1<<PB3) //botão do led azul -> PCINT3 -> PORTAL 0
#define BOTLIGAR (1<<PD2) //botão de ligar -> INT0
#define BOTDESLIGAR (1<<PD3) //botão desligar -> INT1
 
int liga = 0;
int cont = 0;
int contador = 0;
int conte = 0;
unsigned long int contar = 0;
 
ISR(TIMER0_OVF_vect){
  contar++;
  if(contar == 312500){// = 1 segundo sem prescaler
    //excedeu os 5 segundos iniciais
    PORTD |= LEDVM;
    _delay_ms(500);
    PORTD &= ~LEDVM;
    _delay_ms(500);
    PORTD |= LEDVM;
   
  }
}
 
//LIGAR
ISR(INT0_vect){
  cont = 0;
  contador = 0;
  conte = 0;
  PORTC |= LEDVD;
  _delay_ms(1500);
  PORTC &=~ LEDVD;
}
 
//DESLIGAR
ISR(INT1_vect){
  //desligar
  cont = 0;
  contador = 0;
  conte = 0;
 
  PORTD = BOTLIGAR + BOTDESLIGAR + BOTVM;
  PORTC = BOTVD;
  PORTB = BOTAZ;
}
 
//AZUL
ISR(PCINT0_vect){
  contador++;
  if(cont == 4 && contador == 2){
    PORTC |= LEDVD;
    _delay_ms(1500);
    PORTC &=~ LEDVD;
 
    PORTB |= LEDAZ;
    _delay_ms(1500);
    PORTB &=~ LEDAZ;
 
    PORTD |= LEDVM;
    _delay_ms(1500);
    PORTD &= ~LEDVM;
  }
}
 
//VERDE
ISR(PCINT1_vect){
  cont++;
 
  if(cont == 2){
    PORTC |= LEDVD;
    _delay_ms(1500);
    PORTC &=~ LEDVD;
 
    PORTB |= LEDAZ;
    _delay_ms(1500);
    PORTB &=~ LEDAZ;
  }
  if(cont == 10){
    for(int i = 0; i<3; i++){
      PORTC |= LEDVD;
      PORTD |= LEDVM;
      PORTB |= LEDAZ;
      _delay_ms(500);
      PORTC &= ~LEDVD;
      PORTD &= ~LEDVM;
      PORTB &= ~LEDAZ;
      _delay_ms(500);
      PORTC |= LEDVD;
      PORTD |= LEDVM;
      PORTB |= LEDAZ;
    }
  }
}
 
//VERMELHO
ISR(PCINT2_vect){
  conte++;
 
  if(cont == 6 && contador == 4 && conte == 2){
    PORTC |= LEDVD;
    _delay_ms(1500);
    PORTC &=~ LEDVD;
 
    PORTB |= LEDAZ;
    _delay_ms(1500);
    PORTB &=~ LEDAZ;
 
    PORTD |= LEDVM;
    _delay_ms(1500);
    PORTD &= ~LEDVM;
 
    PORTC |= LEDVD;
    _delay_ms(1500);
    PORTC &=~ LEDVD;
  }
}
 
int main(){
 
  //SETUP DO GPIO
  DDRD = 0;
  DDRD |= LEDVM; //led vermelho como saída
  PORTD = 0;
  PORTD |= BOTVM | BOTLIGAR | BOTDESLIGAR; //leds iniciam desligados e botões de liga e desliga em PULL-UP
 
  DDRC = 0;
  DDRC |= LEDVD; //botões como entradas e led verde como saída
  PORTC = 0;
  PORTC |=  BOTVD ; //botões com pull-up interno ativado
 
  DDRB = 0;
  DDRB |= LEDAZ; //led azul como saída
  PORTB |= BOTAZ; //led desligado
 
  //Interrupções
  EICRA |= (0<<ISC00) + (1<<ISC01);
  EIMSK |= (1<<INT0);
 
  EICRA |= (1<<ISC11) + (0<<ISC10);
  EIMSK |= (1<<INT1);
 
  PCICR |= (1<<PCIE0) | (1<<PCIE1) | (1<<PCIE2); //ativar interrupção em todas as portas
  PCMSK0 |= (1<<PCINT3);
  PCMSK1 |= (1<<PCINT9);
  PCMSK2 |= (1<<PCINT17);
 
  sei();

  //TIMER: tempo de resposta da pessoa (máx 5 segundos)
  //SETUP DO TIMER
  TCCR0B = 0; //sem clock
  TCCR0B |= (1<<CS00); //clock interno sem prescaler
 
  TIMSK0 |= (1<<TOIE0); //ativação da interrupção por OVERFLOW
 
  for(;;){
    
  }
}
 
 