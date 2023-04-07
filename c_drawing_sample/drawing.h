#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define repeat(n) _pile_repeat++;for(_cpt_repeat[_pile_repeat]=0;_cpt_repeat[_pile_repeat]<n;_cpt_repeat[_pile_repeat]++)
#define loop _pile_repeat--

#define GAUCHE 1
#define DROITE 0
#define G(A,k)     ( A[(k)/32] |= (1 << ((k)%32)) )
#define D(A,k)   ( A[(k)/32] &= ~(1 << ((k)%32)) )
#define G_OU_D(A,k)    ( A[(k)/32] & (1 << ((k)%32)) )
// !!! G_OU_D(A,k) vaut 0 (DROITE) ou autre chose que 0 mais pas forcément 1 ! -> tester G_OU_D(A,k) == DROITE ou G_OU_D(A,k) != DROITE

int _cpt_repeat[100];
int _pile_repeat;

#define PI M_PI
#define LEFT -1
#define RIGHT 1

struct RGB {
    short red;
    short green;
    short blue;
};

struct _coordinate {
    double x;
    double y;
};

struct _coordinate_lst {
    struct _coordinate coordinate;
    int length; // 0 if next is null
    struct RGB color;
    struct _coordinate_lst *next;
};

struct _drawing {
    int height;
    int width;
    double angle;
    int nbLines;
    float anim_duration;
    struct RGB color;
    struct _coordinate current;
};

struct _drawing _the_draw;
struct _coordinate_lst *_movements;


void init_drawing(int, int);
void display_drawing();
void change_color(short, short, short);
void turn(double,int);
void draw(int);
void move(int);

void _init_drawing(struct _drawing *, int, int);
void _draw(struct _drawing *, struct _coordinate_lst **, int);
void _change_color(struct _drawing *,short, short, short);
void _push(struct _coordinate_lst **, struct _coordinate, int, struct RGB);
void _turn(struct _drawing *, double, int);
void _display_drawing(struct _drawing, struct _coordinate_lst *);
void _move(struct _drawing *, struct _coordinate_lst **, int);
int _getSizeOfLineFrom(struct _coordinate_lst *, int);

#endif // DRAWING_H_INCLUDED
