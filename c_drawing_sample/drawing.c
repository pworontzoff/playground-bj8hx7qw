#include "drawing.h"

void _init_drawing(struct _drawing *draw, int width, int height) {
    draw->height=height;
    draw->width=width;
    draw->angle = 0;
    draw->nbLines = 0;
    draw->anim_duration = 0;
    draw->current.x = width/2;
    draw->current.y = height/2;
    draw->color.blue = 120;
    draw->color.green = 120;
    draw->color.red = 120;
}

void _change_color(struct _drawing * draw, short red, short green, short blue) {
    draw->color.blue = blue;
    draw->color.green = green;
    draw->color.red = red;
}

void _display_drawing(struct _drawing draw, struct _coordinate_lst *lst) {
    int numLine=0,cpt,cpt_all_lines,size;
    struct _coordinate prec;
    struct _coordinate_lst *initLst = lst;
    FILE *fp;
    char buffer[500];

    fp = fopen("output.html","wt");
/*
    fputs("<style>\n",fp);

    prec = lst->coordinate;
    lst = lst->next;

    cpt = cpt_all_lines = 0;

    while (lst!=NULL) {
        if (lst->color.red != -1 || lst->color.green != -1 || lst->color.blue != -1) { // ligne normale
            sprintf(buffer, "@keyframes expand%d {\n",cpt);
            fputs(buffer,fp);

            size = _getSizeOfLineFrom(initLst,cpt_all_lines);

            for (numLine = 0; numLine < draw.nbLines; numLine++) {
                sprintf(buffer, "%.3f% { stroke-dasharray: %d %d; }\n",(numLine*100.0)/draw.nbLines,numLine <= cpt?0:size,size);
                fputs(buffer,fp);
            }
            sprintf(buffer, "100% { stroke-dasharray: %d %d; }\n",size,size);
            fputs(buffer,fp);
            fputs("}\n",fp);

            sprintf(buffer, "line:nth-child(%d) {\n"
                        "\tanimation: expand%d %.2fs linear forwards\n"
                        "}\n\n",cpt_all_lines+1,cpt,draw.nbLines*draw.anim_duration);
            fputs(buffer,fp);
            cpt++;
        } // --> si ligne transparente, non traitée dans l'animation
        cpt_all_lines++;
        prec = lst->coordinate;
        lst = lst->next;
    }
    fputs("</style>",fp);

*/
    lst = initLst;
    prec = lst->coordinate;
    lst = lst->next;

    sprintf(buffer,"<svg height='%d' width='%d' style='border: 1px solid black'>\n",draw.height,draw.width);
    fputs(buffer,fp);
    
    sprintf(buffer,"<circle cx='%f' cy='%f' r='2' style='stroke:rgb(0,0,255);stroke-width:2' />\n",prec.x,prec.y);
    fputs(buffer,fp);
    
    while (lst!=NULL) {
        if(numLine == draw.nbLines/2) {
            sprintf(buffer,"<circle cx='%f' cy='%f' r='2' style='stroke:rgb(255,0,0);stroke-width:2' />\n",prec.x,prec.y);
            fputs(buffer,fp);
        }
        if (lst->color.red == -1 && lst->color.green == -1 && lst->color.blue == -1) {
            sprintf(buffer,"<line x1='%f' y1='%f' x2='%f' y2='%f' style='stroke:rgba(0,0,0,0);stroke-width:2' />\n",prec.x,prec.y,lst->coordinate.x,lst->coordinate.y);
        } else {
            sprintf(buffer,"<line x1='%f' y1='%f' x2='%f' y2='%f' style='stroke:rgb(%d,%d,%d);stroke-width:2' />\n",prec.x,prec.y,lst->coordinate.x,lst->coordinate.y,lst->color.red,lst->color.green,lst->color.blue);
        }
        fputs(buffer,fp);
        prec = lst->coordinate;
        lst = lst->next;
        numLine++;
    }
    fputs("</svg>",fp);
    fclose(fp);
    //system("cmd /c start output.html");
}

void _turn(struct _drawing *draw, double angle, int side) {
    draw->angle = draw->angle + side * (angle * PI / 180);
}


void _move(struct _drawing *draw, struct _coordinate_lst **movement, int length) {
    struct _coordinate new_coord;
    struct RGB cTranspa;

    // should be handle as rgba(0,0,0,0), thus transparent line, and not animated :
    cTranspa.red = cTranspa.green = cTranspa.blue = -1;

    //printf("x : %d - y : %d \n",draw->current.x,draw->current.y);

    new_coord.x = length * cos(draw->angle) + draw->current.x;
    new_coord.y = length * sin(draw->angle) + draw->current.y;

    if (*movement==NULL) {
        _push(movement,draw->current, length, cTranspa);
    }

    draw->current = new_coord;
    _push(movement, new_coord, length, cTranspa);
}

void _draw(struct _drawing *draw, struct _coordinate_lst **movement, int length) {
    struct _coordinate new_coord;

    //printf("x : %d - y : %d \n",draw->current.x,draw->current.y);

    new_coord.x = length * cos(draw->angle) + draw->current.x;
    new_coord.y = length * sin(draw->angle) + draw->current.y;

    if (*movement==NULL) {
        _push(movement,draw->current, length, draw->color);
    }

    draw->nbLines = draw->nbLines + 1;

    draw->current = new_coord;
    _push(movement, new_coord, length, draw->color);
}

void _push(struct _coordinate_lst **lst, struct _coordinate coord, int length, struct RGB color) {
    struct _coordinate_lst *new_c, *current, *prec;

    new_c = malloc(sizeof(struct _coordinate_lst));

    new_c->coordinate = coord;
    new_c->length = 0;
    new_c->color = color;

    prec=NULL;
    current = *lst;

    while (current!=NULL) {
        prec=current;
        current = current->next;
    }

    new_c->next = NULL;
    if (prec==NULL) {
        *lst = new_c;
    } else {
        prec->next = new_c;
        prec->length = length;
    }

}


int _getSizeOfLineFrom(struct _coordinate_lst *lst, int numLine){
    int cpt=0;
    while (lst!=NULL && cpt<numLine) {
        cpt++;
        lst = lst->next;
    }

    return lst==NULL?-1:lst->length;
}

void init_drawing(int width, int height) {
     _movements = NULL;
    _init_drawing(&_the_draw,width,height);
}

void display_drawing() {
    _display_drawing(_the_draw,_movements);
}

void change_color(short red, short green, short blue) {
    _change_color(&_the_draw,red,green,blue);
}

void turn(double angle, int side) {
    _turn(&_the_draw,angle,side);
}

void draw(int length) {
    _draw(&_the_draw,&_movements,length);
}

void move(int length) {
    _move(&_the_draw,&_movements,length);
}

