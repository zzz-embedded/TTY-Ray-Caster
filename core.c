#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define FOV 60.0
#define FOH 30.0
#define MAP_SIZE 15
#define SIGHT 20
#define FPS 60
#define FPS_DELAY (1000 / FPS)

#define SPEED 0.1

struct player
{
    float x;
    float y;
    float dir;  //degree
};

char map[MAP_SIZE][MAP_SIZE] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

char viewer[(int)FOH][(int)FOV] = {0};

struct player *p = NULL;

void init_player()
{
    p = (struct player *)malloc(sizeof(struct player));
    p->x = 2;
    p->y = 2;
    p->dir = -90;
}

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

void scan_map()
{
    double scan_dir = p->dir - (FOV / 2) + 0.5;
    int viwer_line = 0;
    memset(viewer, ' ', sizeof(viewer));
    for (viwer_line = 0;viwer_line < (int)FOV; viwer_line++)
    {
        double k = 0.000001;
        double height = 0;
        double rad;
        rad = degreesToRadians(scan_dir);
        double rad_cos = cos(rad);
        double rad_sin = sin(rad);
        while (k < SIGHT)
        {
            double scan_x = p->x + k * rad_cos;
            double scan_y = p->y + k * rad_sin;
            if (scan_x >= 0 && scan_x < MAP_SIZE && scan_y >= 0 && scan_y < MAP_SIZE)
            {
                if (map[(int)scan_y][(int)scan_x])
                {
                    double corrected_k = k * cos(degreesToRadians(scan_dir - p->dir));
                    height = FOH / corrected_k;
                    // height = FOH / k;
                    if (height > FOH)
                        height = FOH;
                    for (int i = (int)(FOH / 2 - height / 2); i < (int)(FOH / 2 + height / 2); i++)
                    {
                        if (height >= 0 && height < FOH * 0.1)
                            viewer[i][viwer_line] = '.';
                        else if (height >= FOH * 0.1 && height < FOH * 0.2)
                            viewer[i][viwer_line] = ':';
                        else if (height >= FOH * 0.2 && height < FOH * 0.3)
                            viewer[i][viwer_line] = '-';
                        else if (height >= FOH * 0.3 && height < FOH * 0.4)
                            viewer[i][viwer_line] = '=';
                        else if (height >= FOH * 0.4 && height < FOH * 0.5)
                            viewer[i][viwer_line] = '+';
                        else if (height >= FOH * 0.5 && height < FOH * 0.6)
                            viewer[i][viwer_line] = '*';
                        else if (height >= FOH * 0.6 && height < FOH * 0.7)
                            viewer[i][viwer_line] = '%';
                        else if (height >= FOH * 0.7 && height < FOH * 0.8)
                            viewer[i][viwer_line] = '#';
                        else if (height >= FOH * 0.8 && height < FOH * 0.9)
                            viewer[i][viwer_line] = '@';
                        else
                            viewer[i][viwer_line] = '$';
                    }
                    break;
                }
            }
            else
            {
                break;
            }
            k += 0.1;
        }
        scan_dir += 1;
    }
}

void viewer_print()
{
    char temp[((int)FOV + 1) * (int)FOH * 2 + 1];
    int idx = 0;
    for (int i = 0; i < (int)FOH; i++)
    {
        for (int j = 0; j < (int)FOV; j++)
        {
            temp[idx++] = viewer[i][j];
            temp[idx++] = ' ';
        }
        temp[idx++] = '\n';
    }
    temp[idx] = '\0';
    printf("%s", temp);
    // printf("x:%.1f, y:%.1f, dir:%.1f\r\n", p->x, p->y, p->dir);
}

void reset_cursor()
{
    printf("\033[?25l");
    printf("\033[H");
}

void player_move()
{
    char c = 0;
    if (kbhit())
    {
        c = getch();
    }
    switch (c)
    {
        case 'w':p->x += SPEED * cos(degreesToRadians(p->dir));p->y +=
            SPEED * sin(degreesToRadians(p->dir));break;
        case 's':p->x -= SPEED * cos(degreesToRadians(p->dir));p->y -=
            SPEED * sin(degreesToRadians(p->dir));break;
        case 'd':p->x -= SPEED * sin(degreesToRadians(p->dir));p->y +=
            SPEED * cos(degreesToRadians(p->dir));break;
        case 'a':p->x += SPEED * sin(degreesToRadians(p->dir));p->y -=
            SPEED * cos(degreesToRadians(p->dir));break;
        case 'e':p->dir += 2;break;
        case 'q':p->dir -= 2;break;
        default:break;
    }
    c = 0;
}

int main()
{
    init_player();
    printf("\033[2J");
    while (1)
    {
        player_move();
        scan_map();
        reset_cursor();
        viewer_print();
        Sleep(FPS_DELAY);
    }
    return 0;
}