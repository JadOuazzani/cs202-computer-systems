/**
 * @file to_be_debugged.c
 * @brief A nasty C code that does not exactly what it should.
 *
 * Computes the average and the standard deviation of the ages of a few people.
 *
 * @author J.-C. Chappelier
 * @date 31 Jan 2018
 */

#include <stdio.h>
#include <math.h> // for sqrt()

int main(void)
{
    const int PEOPLE_MAX = 1024;
    int ages[PEOPLE_MAX];

    int nb_people = 0;
    do {
        printf("Combien de personnes (<= %d) ? ", PEOPLE_MAX);
        scanf("%d", &nb_people); /* We will assume that a int is typed in.
                                  * We do not bother I/O errors for the
                                  * moment (week < 4).
                                  */
    } while (nb_people >= PEOPLE_MAX);

    int somme;
    int somme2;
    for (int i = 0; i <= PEOPLE_MAX; ++i) {
        printf("Age (entier, en annees) de la personne %d : ", i+1);
        scanf("%d", &ages[i]);
        somme  += ages[i];
        somme2 += ages[i] * ages[i];
    }

    double moyenne  = somme  / nb_people;
    double moyenne2 = somme2 / nb_people;
    double ecart_type = sqrt( (nb_people / (nb_people - 1.0)) // biais correction
                              * (moyenne2 - moyenne * moyenne) );

    printf("\nMoyenne    : %g\n", moyenne);
    printf("Ecart-type : %g\n", ecart_type);

    return 0;
}
