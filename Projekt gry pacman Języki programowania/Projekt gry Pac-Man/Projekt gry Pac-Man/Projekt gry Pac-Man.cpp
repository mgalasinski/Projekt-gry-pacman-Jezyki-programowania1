#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>


//Funkcja gotoxy(int x, int y) służy do ustawienia kursora w określonym miejscu na konsoli.


void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
//hideCursor jest funkcją, która ukrywa kursor na konsoli. 


void hideCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

//Zmienna life przechowuje liczbę żyć gracza.
//Stałe screenHeight i screenWidth określają rozmiary planszy.


int life = 3;
const int screenHeight = 22;
const int screenWidth = 22;
char map[screenHeight][screenWidth];

char stage[screenHeight][screenWidth] = { " ################### ",
                                          " #........#........# ",
                                          " #........#........# ",
                                          " #.##.#.#####.#.##.# ",
                                          " #.................# ",
                                          " ####.###.#.###.#### ",
                                          "    #.#.......#.#    ",
                                          "#####.# ##.##.#.#####",
                                          "#...................#",
                                          "#####.#.#####.#.#####",
                                          "    #.#.......#.#    ",
                                          " ####.#.#####.#.#### ",
                                          " #........#........# ",
                                          " #.##.###.#.###.##.# ",
                                          " #....#...#...#....# ",
                                          " ##.#.#.........#.## ",
                                          " #.................# ",
                                          " ################### " };


//Klasa Pacman reprezentuje postać Pacmana i zawiera 
//funkcje do ruchu, kolizji, zbierania kropek itp.


class Pacman {
public:
    int x;
    int y;
    char dir;
    int delay;
    int count_delay;
    int dotsCollected;


    //Definiuje funkcję draw, która rysuje aktualny stan planszy na konsoli.
    //Wykorzystuje znaki odpowiednie dla poszczególnych elementów planszy : '#' - ściana, '.' - kropka, '^' - 
     //pac - man w górę, 'v' - pac - man w dół, '<' - pac - man w lewo, '>' - pac - man w prawo, '5' - wróg

    void draw() {
        if (dir == 'U')
            map[y][x] = '2';
        else if (dir == 'D')
            map[y][x] = '1';
        else if (dir == 'L')
            map[y][x] = '3';
        else if (dir == 'R' || dir == 'S')
            map[y][x] = '4';
    }

    //Definiuje funkcję move, która obsługuje poruszanie się pacmana 
    //Sprawdza warunki, czy pac - man może poruszać się w danym kierunku, i aktualizuje jego położenie na planszy.

    void move() {
        dead();
        if (count_delay == delay) {
            if (dir == 'U' && !collision(x, y - 1))
                y--;
            if (dir == 'D' && !collision(x, y + 1))
                y++;
            if (dir == 'L' && !collision(x - 1, y))
                x--;
            if (dir == 'R' && !collision(x + 1, y))
                x++;
            count_delay = 0;
            eat();
        }
        count_delay++;
    }
    //Definiuje funkcję collision, która sprawdza 
    //kolizję między pac-manem a ściana.

    bool collision(int fx, int fy) {
        if (map[fy][fx] == '9')
            return true;
        return false;
    }

    //Definiuje funkcję dead, która 
    //sprawdza kolizję między pac - manem a wrogami.
    //Jeśli nastąpi kolizja, zmniejsza liczbę żyć pacmana

    void dead() {
        if (map[y][x] == '5') {
            dir = 'S';
            x = 10;
            y = 15;
            life--;

            gotoxy(2, 1);
            std::cout << "LIFE: " << life << std::endl;


            // Sprawdź, czy skończyły się życia
            if (life == 0) {
                std::cout << "Game Over " << std::endl;
                std::cout << "Liczba zebranych punktow: " << dotsCollected;
            }
        }
    }

    //Definiuje funkcję eat która usuwa kropki z planszy jeśli pacman na nie wejdzie

    void eat() {
        if (stage[y][x] == '.') {
            stage[y][x] = ' ';
            dotsCollected++; // Zwiększ licznik zebranych kropek

            // Sprawdź, czy wszystkie kropki zostały zebrane

            bool allDotsCollected = true;
            for (int i = 0; i < screenHeight; i++) {
                for (int j = 0; j < screenWidth; j++) {
                    if (map[i][j] == '8') {
                        allDotsCollected = false;
                        break;
                    }
                }
                if (!allDotsCollected) {
                    break;
                }
            }

            // Jeśli wszystkie kropki zostały zebrane, zakończ grę
            if (allDotsCollected) {
                std::cout << "Gratulacje wygrałeś!!! " << std::endl;
                std::cout << "Liczba zebranych punktow: " << dotsCollected;
            }
        }
    }
};



//Klasa Enemy reprezentuje przeciwników
//i zawiera funkcje do ruchu i kolizji.


class Enemy {
public:
    int x;
    int y;
    int dir;
    int delay;
    int count_delay;

    void draw() {
        map[y][x] = '5'; // "5"Znak reprezentujący przeciwnika
    }

    //Definiuje funkcję move, która obsługuje poruszanie się wrogów.
  //Losowo ustawia kierunek wroga, jeśli napotka przeszkodę.



    void move() {
        if (count_delay == delay) {
            float fx = x;
            float fy = y;


            if (dir == 0 && !collision(x, y - 0.25))
                fy--;
            if (dir == 1 && !collision(x, y + 0.25))
                fy++;
            if (dir == 2 && !collision(x - 0.25, y))
                fx--;
            if (dir == 3 && !collision(x + 0.25, y))
                fx++;


            if (fx >= 0 && fx < screenWidth && fy >= 0 && fy < screenHeight) {
                if (!collision(fx, fy)) {
                    x = fx;
                    y = fy;
                }
            }

            count_delay = 0;
            dir = rand() % 4;
        }
        count_delay++;
    }

    //Definiuje funkcję collision, która 
    //sprawdza kolizję między duchem a ścianą.

    bool collision(int fx, int fy) {
        if (map[fy][fx] == '9')
            return true;
        return false;
    }
};

//Funkcja setup() inicjalizuje początkowe
//położenie Pacmana i przeciwników.

void setup(Pacman& pacman, Enemy enemy[]) {
    srand(time(NULL));
    pacman.x = 10;
    pacman.y = 15;
    pacman.dir = 'S';
    pacman.delay = 8; //  wartość opóźnienia dla Pacmana

    enemy[0].x = 9;      //Położenie początkowe wroga
    enemy[0].y = 9;
    enemy[0].delay = 20; //  wartość opóźnienia dla wroga 
    enemy[1].x = 10;
    enemy[1].y = 9;
    enemy[1].delay = 30;
    enemy[2].x = 11;
    enemy[2].y = 9;
    enemy[2].delay = 10;
    enemy[3].x = 2;
    enemy[3].y = 1;
    enemy[3].delay = 30;
    enemy[4].x = 6;
    enemy[4].y = 6;
    enemy[4].delay = 30;
    enemy[5].x = 2;
    enemy[5].y = 3;
    enemy[5].delay = 30;


}
//Funkcja layout() przekształca układ planszy z 
//tablicy stage na tablicę map

void layout() {
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            if (stage[i][j] == '#')
                map[i][j] = '9';
            else if (stage[i][j] == '.')
                map[i][j] = '8';
            else
                map[i][j] = '0';
        }
    }
}

//Funkcja movements() wykonuje ruchy Pacmana i 
//przeciwników oraz sprawdza kolizje.


void movements(Pacman& pacman, Enemy enemy[]) {
    pacman.move();

    // Sprawdź kolizję Pacmana z przeciwnikami
    for (int i = 0; i < 7; i++) {
        if (pacman.x == enemy[i].x && pacman.y == enemy[i].y) {
            pacman.dead();
            break;
        }
    }

    for (int i = 0; i < 7; i++)
        enemy[i].move();
    // Sprawdź ponownie kolizję Pacmana z przeciwnikami po ich ruchu
    for (int i = 0; i < 7; i++) {
        if (pacman.x == enemy[i].x && pacman.y == enemy[i].y) {
            pacman.dead();
            break;
        }
    }
}
//Funkcja input() obsługuje wejście 
//gracza i zmienia kierunek Pacmana.

void input(Pacman& pacman) {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
        case 'W':
            if (!pacman.collision(pacman.x, pacman.y - 1))
                pacman.dir = 'U';
            break;
        case 's':
        case 'S':
            if (!pacman.collision(pacman.x, pacman.y + 1))
                pacman.dir = 'D';
            break;
        case 'a':
        case 'A':
            if (!pacman.collision(pacman.x - 1, pacman.y))
                pacman.dir = 'L';
            break;
        case 'd':
        case 'D':
            if (!pacman.collision(pacman.x + 1, pacman.y))
                pacman.dir = 'R';
            break;
        }
    }
}


Pacman pacman;
Enemy enemy[5];
void display();


//W funkcji main() odbywa się główna pętla gry, w której wywoływane są funkcje 
//do wyświetlania, obsługi wejścia i ruchu.

int main() {
    HWND s = GetConsoleWindow();
    MoveWindow(s, 300, 200, 250, 450, true);
    hideCursor();

    setup(pacman, enemy);

    while (life > 0) {
        display();
        input(pacman);
        movements(pacman, enemy);
    }

    return 0;
}
//Funkcja display() wyświetla aktualny stan 
//planszy i inne informacje na konsoli.
void display() {
    gotoxy(2, 1);
    std::cout << "LIFE: " << life;
    layout();
    pacman.draw();
    for (int i = 0; i < 4; i++)
        enemy[i].draw();
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            gotoxy(j + 2, i + 3);
            if (map[i][j] == '9')
                std::cout << char(219);
            else if (map[i][j] == '8')
                std::cout << char(250);
            else if (map[i][j] == '1')
                std::cout << char(86);
            else if (map[i][j] == '2')
                std::cout << char(94);
            else if (map[i][j] == '3')
                std::cout << char(60);
            else if (map[i][j] == '4')
                std::cout << char(62);
            else if (map[i][j] == '5')
                std::cout << '^';
            else if (map[i][j] == '0')
                std::cout << ' ';
            else if (map[i][j] == '8')
                std::cout << '.';
        }
    }



}
