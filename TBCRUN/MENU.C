#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_graph1.h"
#include "fusion-c/header/io.h"

#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_UP 30
#define KEY_DOWN 31
#define KEY_MENU_L 83
#define KEY_MENU_U 115

typedef struct 
{
    char *texto;
    char *textoSelecionado;
    int id;
} MenuItem;

// globals
static FCB file;
static MenuItem *menuItems = NULL;
static int menuItemCount = 0;
static int selectedOption = 0;
static int pageCount = 0;
static int pageSize = 13;

void ExecuteOption()
{
    Cls();
    Locate(1,1);Print("A:\>");
    Print("WRTSST ");
    Print(menuItems[selectedOption].texto);
    WaitKey();
    Cls();
    Exit(0);
}

void AddMenuItem(const char *text) 
{
    char *upperText = strdup(text);
    char *lowerText = strdup(text);

    StrToUpper(upperText);
    StrToLower(lowerText);
    
    menuItems = (MenuItem *)realloc(menuItems, (menuItemCount + 1) * sizeof(MenuItem));
    menuItems[menuItemCount].texto = upperText;
    menuItems[menuItemCount].textoSelecionado = lowerText;
    menuItems[menuItemCount].id = menuItemCount;
    menuItemCount++;
}

void DrawBox(int x, int y, int width, int height) 
{
    Locate(x,y);
    PrintChar(1);
    PrintChar(88);

    for (int i=0; i<width-2; i++) 
    {
        Locate(x+i+1,y);
        PrintChar(1);
        PrintChar(87);
    }

    Locate(width,y);
    PrintChar(1);
    PrintChar(89);

    for (int i=0; i<height-1; i++) 
    {
        Locate(x,y+i+1);
        PrintChar(1);
        PrintChar(86);

        Locate(width,y+i+1);
        PrintChar(1);
        PrintChar(86);
    }

    Locate(x,y+height);
    PrintChar(1);
    PrintChar(90);

    for (int i=0; i<width-2; i++) 
    {
        Locate(x+i+1,y+height);
        PrintChar(1);
        PrintChar(87);
    }

    Locate(width,y+height);
    PrintChar(1);
    PrintChar(91);
}

void ReadFiles(char *mask)
{
    char sbuf[255];
    int n;

    n=FindFirst(mask,sbuf,0);
    for(;!n;)
    {
        AddMenuItem(sbuf);
        n=FindNext(sbuf);
    }
    pageCount = menuItemCount / pageSize;
}

void PrintMenu()
{
    int xpos = 0;
    int ypos = 3;

    for (int j=0; j<menuItemCount; j++) 
    {
        
        if (j%2 == 0)
        {
            xpos = 1;
            ypos++;
        } 
        else
        {
            xpos = 16;
        }

        Locate(xpos,ypos);

        if (menuItems[j].id == selectedOption) 
        {
            Print(menuItems[j].textoSelecionado);
        } 
        else 
        {
            Print(menuItems[j].texto);
        }
    }
}

void IncreaseOption()
{
    selectedOption++;
    if (selectedOption >= menuItemCount) selectedOption = 0;
}

void DecreaseOption()
{
    selectedOption--;
    if (selectedOption < 0) selectedOption = menuItemCount - 1;
}

void PrepareChars()
{    
    for (int i=0x41*8;i<=0x5A*8;i++)
    {
        Vpoke(i+256, Vpeek(i));
    }
}

void ChangeColors()
{
    int color = 0x12; 
    Vpoke(8204,color);
    Vpoke(8205,color);
    Vpoke(8206,color);
    Vpoke(8207,color);
    Vpoke(8219,color/16+(color-(color && 0xF0))*16);
}

int Is80ColumnsMode()
{
    return (Vpeek(0xFCAF) & 0x80) != 0;
}

void DrawMenu()
{
    DrawBox(0,0,40,2);
    Locate(1,1);Print("SYSTEM 1");
    DrawBox(0,3,40,15);
    DrawBox(0,19,40,2);
    Locate(1,20);Print("ESCOLHA UM ARQUIVO");
}

int DetectMSXVersion()
{
    int version = 0;
    if (Peek(0x2D) == 0x00) 
    {
        version = 1; // MSX 1
    } 
    else if (Peek(0x2D) == 0x01) 
    {
        version = 2; // MSX 2
    } 
    else if (Peek(0x2D) == 0x02) 
    {
        version = 3; // MSX 2+
    } 
    else if (Peek(0x2D) == 0x03) 
    {
        version = 4; // MSX Turbo-R
    }
    return version;
}

void SetSystem(int version)
{
    if (version == 1) 
    {
        SetColors(2,0,0);
        Screen(1);
    } 
    else if (version == 2) 
    {
        SetColors(2,0,0);
        Screen(1);
        Width(80);
    } 
    else if (version == 3) 
    {
        SetColors(2,0,0);
        Screen(1);
        Width(80);
    } 
    else if (version == 4) 
    {
        SetColors(2,0,0);
        Screen(1);
        Width(80);
    }
}

void OpenMenu()
{
    DrawBox(30,3,40,15);
    Locate(31,4);Print("MENU");
    Locate(31,6);Print("1. Opção 1");
}

void main(void)
{
    int key;
    int msxVersion = DetectMSXVersion();

    Cls();
    SetSystem(msxVersion);
    PrepareChars();
    ChangeColors();
    DrawMenu();
    ReadFiles("*.TXT");
    
    while(1)
    {
        PrintMenu();
        while(key=Inkey(),!key);
        if (key == KEY_ESC) break;
        if (key == KEY_DOWN) IncreaseOption();
        if (key == KEY_UP) DecreaseOption();
        if (key == KEY_ENTER) ExecuteOption();
        if (key == KEY_MENU_L || key == KEY_MENU_U) OpenMenu();
    }
    Cls();
}