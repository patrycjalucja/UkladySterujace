//Patrycja Syty, gr. 4

#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include "HD44780.h"
#include <util/delay.h>



void WriteNibble(unsigned char nibbleToWrite)
{
    PORTA |= (1<<E);//znak, ze bede cos wpisywac
    PORTA = (PORTA & 0xF0)|(nibbleToWrite&0x0F); //przesylanie danych
    PORTA = PORTA & ~(1<<E); //wyswietlam dane na wyswietlaczu
}

void LCD_Command(unsigned char cmd)
{
    PORTA &= ~(1<<RS); //daje znak, ze bede wpisywac instrukcje
    WriteNibble(cmd>>4);
    WriteNibble(cmd);

};

void LCD_Text(char *litera) // wypisywanie na ekran
{

    PORTA |= (1<<RS); //bede wypisywac dane
    char * tmp_a = litera; //kopiuje wskaznik na poczatek napisu
    while (*litera) //czytanie napisu literka po literce
    {
        WriteByte(*litera++);
        _delay_us(70);
    }

    int counter = str_len(tmp_a); //dlugosc napisu

    if(counter>16)  // jezeli dlugugosc napisu jest dluzsza niz dlugosc ekranu
    {
        int b = counter - 16;
        while(b-- > 0 )  // to przewijam
        {
            LCD_Command(0x18); // 8 oznacza, że tekst przesuwa się w lewo, bo przesuwam ekran
            _delay_ms(500);
        }
    }


};

void LCD_GoToXY(unsigned char x, unsigned char y) //ustawianie kursora
{
    LCD_Command((y *0x40 + x)|(0x80)); // ustawiam adres kolejnego zapisywanego znaku
    _delay_ms(1);

};


void LCD_Clear(void) //czyszczenie ekranu
{
    LCD_Command(0x01);
    _delay_ms(1);
};


void LCD_Home(void) //przesuwa kursor na poczatek ekranu
{
    LCD_Command(0x02);
    _delay_ms(1);
};


void LCD_Initalize(void) //inicjalizuje ekran
{
    _delay_ms(50);
    for(int i = 0; i < 3; i++)
    {
        WriteNibble(0x03);
        _delay_ms(7);
    }
    WriteNibble(0x02);
    _delay_ms(5);
    LCD_Command(0x28);
    _delay_ms(5);
    LCD_Command(0x08);
    _delay_ms(5);
    LCD_Command(0x01);
    _delay_ms(5);
    LCD_Command(0x06);
    _delay_ms(5);
    LCD_Command(0x0F);
    _delay_ms(5);
};

int str_len(char * txt) //tutaj trzymam dlugosc napisu
{
    int i = 0;
    while (*txt++)
    {
        i++;
    }

    return i;
}


int main(void)
{
    DDRA = 0xFF;
    LCD_Initalize();
    LCD_Text("Hello world!");
    _delay_ms(20);
    while(1)
    {
    }
}
