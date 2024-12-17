#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>


#define WIDTH 30
#define HEIGHT 15

typedef struct {
    char name[20];
    int score;
} Player;

Player players[100];
int num = 0;

int s_x, s_y, f_x, f_y, score, len = 3;
int body_x[100], body_y[100];
int direction = 0;
int gameOver = 0;

void sort() {
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < num - i - 1; j++) {
            if (players[j].score < players[j+1].score) {
                Player temp=players[j];
                players[j]=players[j+1];
                players[j+1]=temp;
            }
        }
    }
}

void save() {
    FILE *fp = fopen("highscores.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < num; i++) {
        fprintf(fp, "%s %d\n", players[i].name, players[i].score);
    }
    fclose(fp);
}

void load() {
    FILE *fp = fopen("highscores.txt", "r");
    if (fp == NULL) {
        printf("No high score file found. Starting a new one.\n");
        return;
    }
    int i = 0;
    while (fscanf(fp, "%s %d", players[i].name, &players[i].score) != EOF) {
        i++;
        num++;
    }
    fclose(fp);
}

void setup() {
    score = 0;
    len=3;
    s_x = WIDTH / 2;
    s_y = HEIGHT / 2;

    do {
        f_x = rand() % WIDTH;
        f_y = rand() % HEIGHT;
    } while (f_x == s_x && f_y == s_y);
}

void draw() {
    system("cls");
    for (int i = 0; i < HEIGHT + 2; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1)
                printf("#");
            else if (i == s_y && j == s_x)
                printf("O");
            else if (f_x == j && f_y == i)
                printf("*");
            else {
                int isSnakeBody = 0;
                for (int k = 0; k < len; k++) {
                    if (body_x[k] == j && body_y[k] == i) {
                        isSnakeBody = 1;
                        break;
                    }
                }
                printf(isSnakeBody ? "o" : " ");
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void input() {
    if (kbhit()) {
        char ch = getch();
        switch (ch) {
            case 'w':
                if (direction != 2) {
                    direction = 0;
                }
                break;
            case 's':
                if (direction != 0) {
                    direction = 2;
                }
                break;
            case 'a':
                if (direction != 1) {
                    direction = 3;
                }
                break;
            case 'd':
                if (direction != 3) {
                    direction = 1;
                }
                break;
            case 'x':
                gameOver = 1;
                break;
        }
    }
}

void logic() {
    int prevX = s_x;
    int prevY = s_y;
    s_x += (direction==1)-(direction==3);
    s_y += (direction==2)-(direction==0);

    if (s_x==0 || s_x==WIDTH+1 || s_y==0 || s_y==HEIGHT+1) {
        gameOver = 1;
        return;
    }

    for (int i = 1; i < len; i++) {
        if (s_x==body_x[i] && s_y==body_y[i]) {
            gameOver = 1;
            return;
        }
    }


    for (int i=len-1; i>0;i--) {
        body_x[i] = body_x[i-1];
        body_y[i] = body_y[i-1];
    }
    body_x[0] = prevX;
    body_y[0] = prevY;

    if (s_x==f_x && s_y==f_y) {
        score+=10;
        len++;
        f_x=rand()%WIDTH;
        f_y=rand()%HEIGHT;
    }
}

void game(){
    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(100);
    }
}

int main() {
    load();
    int a=1;
    do{
    gameOver=0;
    game();
    printf("Game Over!\nYour Score: %d\n", score);

    printf("Enter your name: ");
    scanf("%s", players[num].name);
    players[num].score = score;
    num++;
    printf("do you want to play another round(yes=1,no=2);");
    scanf("%d",&a);
    if(a!=1){
        break;
    }
    }while(1);
    sort();
    save();

    printf("\nTop 10 Scores:\n");
    for (int i=0;i<10 && i<num;i++) {
        printf("%d. %s - %d\n",i+1, players[i].name, players[i].score);
    }

    return 0;
}