#include <stdio.h>
#include <stdlib.h>
#include "drawing.h"


#define COURBE_NIVEAU 1


#define LARGEUR 1100
#define HAUTEUR 1100
#define TAILLE_SEGMENT_INIT 400


int chemin[65536]={0};
int etape=0;

int nbVirages();
void ajoutePlis();


int main() {
    int longueur_segment = TAILLE_SEGMENT_INIT;
    int i=0;
    
    repeat (COURBE_NIVEAU) {
        ajoutePlis();
        longueur_segment = (longueur_segment * 7.15) / 10;        
    } loop;

    init_drawing(LARGEUR,HAUTEUR);
	turn(90,RIGHT);

    draw(longueur_segment);

    repeat (nbVirages()) {
        turn(90,(G_OU_D(chemin, i) != DROITE) ? LEFT : RIGHT);
        draw(longueur_segment);
        i = i + 1;
    } loop;
    
    display_drawing();

    return 0;
}

int nbVirages() {
    /*
    etape  chemin                       - nombre de virages
           debut    milieu fin            debut milieu fin
    0                                   - 0     0      0   =   0 + 2 x 0  =  0  virage 
    1               G                   - 0     1      0   =   1 + 2 x 0  =  1  virage
    2      G        G      D            - 1     1      1   =   1 + 2 x 1  =  3  virages
    3      GGD      G      GDD          - 3     1      3   =   1 + 2 x 3  =  7  virages
    4      GGDGGDD  G      GGDDGDD      - 7     1      7   =   1 + 2 x 7  =  15 virages
    ...
    x-1                                                                   =  p  virages
    x                                                      =   1 + 2 x p        virages
    ...
    */

    int p=0;

    repeat (etape) {
        p = 1 + 2 * p;
    } loop;

    return p;
}

void ajoutePlis() {
    int nouveauMilieu = nbVirages();
    int i=1;

    G(chemin, nouveauMilieu);

    repeat(nouveauMilieu) {
        if ( G_OU_D(chemin, nouveauMilieu - i) != DROITE ) {
            D(chemin, nouveauMilieu + i);
        } else {
            G(chemin, nouveauMilieu + i);
        }
        i++;
    } loop;

    etape = etape + 1;
    return;
}