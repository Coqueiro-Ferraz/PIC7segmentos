/*
 * Arquivo:   contador.c
 * Autor: HAGACEEF (Coqueiro)
 * 
 * Arquivo do Proteus: "contador7seg"
 * - a cada pulso no fim de curso, deve aumentar um número na contagem de 0 a F
 * - desafio 1: corrigir erros e executar o programa;
 * - desafio 2: usar um outro display para contar de 00 a 99 em decimal, pode-se usar o portD para o outro display;
 * - desafio 3: utilizar os LEDs de contagem ativa no momento que o botão pulsa (verde), 
 *              estouro da contagem (vermelho) e contagem parada (amarelo).
 * 
 * Este programa tem 2 erros, um impedirá a compilação, o outro atrapalhará a execução
 * observe o console "Output na parte de baixo da tela, após realizar a compilação (apertar o botão do martelo)
 * perceba que o erro apresentado será:  ***contador.c:76:23: error: expected ';' after expression*** - em azul
 * significa que o erro está na linha 82, e na 23° coluna (contando letras e espaços)
 * TRISB = 0b10000000  // RB 76543210
 *                   ^                     o sinal ^ aponta onde está o erro
 * 
 * Aulas da semana 27 a 31 de Março de 2023
 * PIC16F877A
 * Display 7 segmentos no port B6-B0 (a-g)
 * Detector (fim de curso) no RB7
 * 
 * Cristal externo 8 MHz
 * 
 *          DISPLAY DE 7 SEGMENTOS -
 *         1 - Faça o número 6 com o segmento f aceso;
 *         2 - Preencha o código para o segmento 'e';
 *         3 - Preencha os códigos de A a F (lembre-se que o b e o d são minúsculos).
 * 
 *           ___a___                    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
 *          |       |           a RB6   1   0   1   1   0   1   1   1   1   1   1   0   1   0   1   1
 *          |f      |b          b RB5   1   1   1   1   1   0   0   1   1   1   1   0   0   1   0   0  
 *          |       |           c RB4   1   1   0   1   1   1   1   1   1   1   1   1   0   1   0   0
 *           ---g---            d RB3   1   0   1   1   0   1   1   0   1   1   0   1   1   1   1   0
 *          |       |           e RB2   1   0   1   0   0   0   1   0   1   0   1   1   1   1   1   1
 *          |e      |c          f RB1   1   0   0   0   1   1   1   0   1   1   1   1   1   0   1   1
 *          |___d___|           g RB0   0   0   1   1   1   1   1   0   1   1   1   1   0   1   1   1
 *                      
 *                          código 0x   7E  30  6D  79  33  5B  5F  70  7F  7B  77  1F  4E  3D  4F  47
 * 
 * 
 */

// FUSÍVEIS DE CONFIGURAÇÕES 
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 8000000
#include <xc.h>     // biblioteca básica do PIC 877a
#include <stdint.h> // biblioteca para variáveis do tipo INT

#define cont RB7 // passa a identificar o port RB7 com o nome Cont, que será nossa entrada de dados

uint8_t contagem = 0; // declaração de variável do tipo inteiro de 8 bits não assinalada (somente valores positivos 0-255), inicializada com o valor 0
uint8_t codigo7seg [16] = // declaração de Array com 16 posições dos códigos dos números 0 a F para o display
{   
    0x7E, 0x30, 0x6D, 0x79, // 0, 1, 2, 3        -- perceba que aqui estão alguns erros no código
    0x33, 0x5B, 0x5F, 0x70, // 4, 5, 6, 7
    0x7F, 0x7B, 0x77, 0x1F, // 8, 9, A, B
    0x4E, 0x3D, 0x4F, 0x47, // C, D, E, F        -- de B a F não foi colocado o código hexa
};


void apresentar_contagem() // função para apresentar o valor contado (será usada posteriormente no código)
{
    PORTB &= 0b10000000;            //zera os bits de RB6, RB5, RB4, RB3, RB2, RB1 e RB0 sem afetar o RB7
    PORTB |= codigo7seg[contagem];  //apresenta o código do 7 segmentos nos pinos RB6 a RB0 sem afetar o RB7
}



void main(void) 
{
    // Definição de entradas e saídas [1] entrada e [0] saída
    TRISB = 0b10000000  // RB 76543210
    PORTB = 0x00;       // zera o portB
    
    while(1)            //laço infinito
    {
        if(cont = 1)   // pergunta se recebeu uma contagem na porta RB7
        {
            __delay_ms(100);    // aguarda 100 ms para desapertar o botão
            contagem++;         // ao receber o nível 1 na porta RB7, incrementa (soma 1) à variável contagem
            if(contagem == 16) contagem = 0; // reseta contagem no estouro
            while(cont == 1);   // segura o programa até soltar o botão de contagem
        }
        apresentar_contagem();  // chama a função que apresenta o valor contado
        __delay_ms(10);    // aguarda 10 ms para apresentar o valor no display
    }
}
