#include <iostream>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <stdio.h>

using namespace std;

bool gameover;
int width = 20;
int height = 17;
int first = 0;
int delay = 35000;
int dificultad = 1;

int explicaciones = 1;

const int RED = 10;
const int GREEN = 12;
const int WHITE = 15;
const int WALL = 240;
const int POISON = 80;
const int KEY = 14;

int x, y, fruitX, fruitY, score;
int mainInput;
int showKey = 0;
int expandMap = 0;

int badAppleX = 0;
int badAppleY = 0;

int keyX = 0;
int keyY = 0;

int tailX[100], tailY[100]; //coordenadas
int nTail;

enum eDirecton {STOP = 0, LEFT,RIGHT, UP, DOWN}; // Controls

eDirecton dir;

void txtColor(int code)
{

    switch (code) {

        case RED:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            break;
        case GREEN:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            break;
        case WHITE:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            break;
        case POISON:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 80);
            break;
        case KEY:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            break;
        case WALL:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
            break;
    }
}

void mainMenu()
{
    cout    <<  "       ----------------------------- \n" <<
                "         Super Snake Pablo Edition            \n" <<
                "       ----------------------------- \n\n\n" <<
                "             1..Comenzar juego\n" <<
                "             2...Opciones\n" <<
                "             3....Salir\n\n";
}

void startGameMenu()
{
    if (explicaciones == 2){
        cout    <<  "       ----------------------------- \n" <<
                    "         Super Snake Pablo Edition            \n" <<
                    "       ----------------------------- \n\n" <<
                    "             Elija dificultad:\n\n" <<
                    "             1...Practica\n";
                    txtColor(GREEN);
        cout    <<  "             2...Desafio\n";
                    txtColor(RED);
        cout    <<  "             3...Nightmare\n\n";
                    txtColor(WHITE);
    } else {
        cout    <<  "         ----------------------------- \n" <<
                    "           Super Snake Pablo Edition            \n" <<
                    "         ----------------------------- \n\n" <<
                    "               Elija dificultad:\n\n" <<
                    "               1...Practica\n" <<
                    "       Las paredes no matan ni se expanden\n" <<
                    "  La unica manera de perder es chocando con tu propia cola\n";
                    txtColor(GREEN);
        cout    <<  "               2...Desafio\n" <<
                    "     Las paredes matan y se expanden por puntaje\n";
                    txtColor(RED);
        cout    <<  "               3...Nightmare\n"<<
                    "             Las paredes matan\n" <<
                    "          Se expanden solo con llaves\n" <<
                    "  Desde de los 50 puntos, aparece una manzana envenenada\n" <<
                    "               Puntaje x 2\n";
                    txtColor(WHITE);
    }

}

void optionsMenu()
{
    cout    <<  "       ----------------------------- \n" <<
                "         Super Snake Pablo Edition            \n" <<
                "       ----------------------------- \n\n\n" <<
                "               Explicaciones:\n\n" <<
                "             1...Activar  \n" <<
                "             2...Desactivar\n" <<
                "             3...Cancelar    \n\n";
}

void gameOverMenu()
{
    cout    <<  "       ----------------------------- \n" <<
                "         Super Snake Pablo Edition            \n" <<
                "       ----------------------------- \n\n\n" <<
                "                 GAME OVER:      \n\n" <<
                "             1...Reintentar      \n" <<
                "             2...Menu principal  \n" <<
                "             3...Salir \n\n";
}

void Setup()
{
    gameover = false;
    nTail = 0;
    width = 20;
    height = 17;
    delay = 35000;
    badAppleX = 0;
    badAppleY = 0;
    dir = STOP;
    showKey = 0;
    x = width / 2;

    y = height / 2;

    fruitX = rand() % width; //ubicacion random de la fruta
    fruitY = rand() % height;
    keyX = rand() % width;
    keyY = rand() % height;


    score = 0;

}

void Draw()
{

    if (dificultad == 2 && score < 81) {
        switch (score)
        {
            case 30:
                delay = 20000;
                width = 40;
            break;
            case 60:
                delay = 12000;
                width = 45;
            break;
            case 80:
                delay = 0;
                width = 50;
            break;
            default:
                delay = delay;
            break;
        }
    }

    if (expandMap == 1){

        //tras expandir, no expandimos mas ni mostramos la llave
        showKey = 0;
        expandMap = 0;
        switch (width)
        {
            case 20:
                width = 40;
                break;
            case 40:
                width = 60;
                break;
            case 60:
                width = 80;
                break;
            case 80:
                width = 90;
                break;
        }
    }

    if (dificultad == 3){
        delay = 1000;
    }

    txtColor(WALL);
    cout << string(width+2, '#');
    txtColor(WHITE);

    cout << endl ;

    for (unsigned int i = 0; i < height ; i++)
    {

        for (unsigned int j = 0; j < width; j++)
        {

            if (j == 0) {

                txtColor(WALL);
                cout << "#"; //walls
                txtColor(WHITE);
            }
            if (i == y && j == x) {

                txtColor(GREEN);
                cout << "*";
                txtColor(WHITE);

            } else if (i == fruitY && j == fruitX ){

                txtColor(RED);
                cout << "O";
                txtColor(WHITE);

            } else {

                bool print = false;

                for (int k = 0; k< nTail ; k++)
                {

                    if (tailX [k] == j && tailY [k] == i)
                    {

                        txtColor(GREEN);
                        cout << "*";
                        txtColor(WHITE);
                        print = true;

                    }

                }

                if (dificultad == 3 && score > 79)
                {
                    if (badAppleX == j && badAppleY == i)
                    {
                        txtColor(POISON);
                        cout << "O";
                        txtColor(WHITE);
                        print = true;
                    }
                }
                if (dificultad == 3)
                {
                    if (showKey == 1)
                    {
                        if (keyX == j && keyY == i)
                        {
                            txtColor(KEY);
                            cout << "%";
                            txtColor(WHITE);
                            print = true;
                        }
                    }
                }

                if (!print) cout << " ";

            }

            if (j == width -1){

                txtColor(WALL);
                cout << "#";
                txtColor(WHITE);
            }
        }

        cout << endl;

    }
    txtColor(WALL);
    cout << string(width+2, '#');
    txtColor(WHITE);
    cout << endl;
    cout << "Score:" << score << endl ;
    cout << "Para salir, presione X" << endl ;

    if (dificultad == 3)
    {
        delay = 35000;
    }
    usleep(delay);


}

void Input ()
{

    if (_kbhit ())
    {

        switch (_getch ())
        {

        case 'a':
        case 'A':

            if (dir != RIGHT){
                dir = LEFT;
            }

            break;

        case 'd':
        case 'D':

            if (dir != LEFT){
                dir = RIGHT;
            }

            break;

        case 'w':
        case 'W':

            if (dir != DOWN){
                dir = UP;
            }

            break;

        case 's':
        case 'S':

            if (dir != UP){
                dir = DOWN ;
            }

            break;

        case 'x':
        case 'X':

            gameover = true;

            break;

        }

    }

}

void algorithm()
{

    int prevX = tailX [0];

    int prevY = tailY [0];

    int prev2X, prev2Y;

    tailX[0] = x;

    tailY[0] = y;

    for(int i = 1; i < nTail ; i++)
    {

        prev2X = tailX[i];

        prev2Y = tailY[i];

        tailX[i] = prevX;

        tailY[i] = prevY;

        prevX = prev2X;

        prevY = prev2Y ;

    }

    switch (dir)
    {

    case LEFT:

        x--;

        break;

    case RIGHT:

        x++;

        break;

    case UP:

        y--;

        break;

    case DOWN:

        y++;

        break;

    default:

        break;

    }

    if (dificultad > 1) {
        //normal y dificil
        if (x >= width) gameover = true;
        else if (x <0) gameover = true;

        if (y >= height) gameover = true;
        else if (y < 0) gameover = true;
    } else {
        //facil
        if (x >= width) x =0;
        else if (x <0) x = width -1;

        if (y >= height) y = 0;
        else if (y < 0) y = height - 1;
    }


    for (int i =0; i< nTail ; i++)
    {
        if ((tailX[i] == x && tailY[i] == y) ||
            (tailX[i] == badAppleX && tailY[i] == badAppleY))
            gameover = true;
        if (showKey == 1)
        {
            if (tailX[i] == keyX && tailY[i] == keyY)
            {
                expandMap = 1;
            }
        }

    }

    // si la serpiente choca la fruta
    if (x == fruitX && y == fruitY)
    {
        score = (dificultad == 3) ? score + 20 : score + 10 ;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail ++;

        if (dificultad == 3)
        {
            if (score > 79)
            {
                badAppleX = rand() % width;
                badAppleY = rand() % height;
            }
            if (score > 59 && (rand() % 100) % 2 == 0)
            {
                showKey = 1;
            }
        }
    }

    // si choca con la llave
    if (x == keyX && y == keyY)
    {
        //si seguimos en 40, sigue mostrando la llave, corregir eso
        if (dificultad == 3)
        {
            if (showKey == 1 && width < 90){
                //asignamos otra posicion
                keyX = rand() % width;
                keyY = rand() % height;
                //la ocultamos pero expandimos el mapa
                showKey = 0;
                expandMap = 1;
            }
            else
            {
                keyX = 300;
                keyY = 300;

            }
        }
        else
        {
            keyX = 300;
            keyY = 300;

        }
    }

}

void ClearScreen()
{
    // con esto evitamos el flickering molesto
    COORD cursorPosition;

    cursorPosition.X = 0;
    cursorPosition.Y = 0;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

}

int startScreen()
{
    int listo = 0;
    int explicacionesTmp = 0;

    while (listo == 0) {
        mainMenu();
        cout << "Elija una opcion: ";
        cin >> mainInput;

        switch ( mainInput )
        {
        case 1:
            system("cls");
            startGameMenu();
            cout << "Elija una opcion para la dificultad: ";
            cin >> dificultad;
            listo = 1;
            break;
        case 2:
            system("cls");
            optionsMenu();
            cout << "Elija una opcion: ";
            cin >> explicacionesTmp;
            if (explicacionesTmp == 2 || explicacionesTmp == 1){
                explicaciones = explicacionesTmp;
            }
            system("cls");
            break;
        case 3:
            return 0;
            break;
        default:
            return 0;
            break;
        }

    }
    return 1;

}

int main()
{

    int exit = 0;
    int gameOverOption = 2;

    while (!exit)
    {

        if (gameOverOption == 2)
        {
            if (startScreen() == 0)
            {
                exit = 1;
                return 0;
            }
        } else if (gameOverOption == 1) {
            //do nothing
        } else {
            return 0;
        }

        Setup();

        system("cls");

        while (!gameover)
        {
            ClearScreen();

            Draw ();

            Input ();

            algorithm ();

        }

        system("cls");
        gameOverMenu();
        cin >> gameOverOption;
        system("cls");
    }


    return 0;

}

