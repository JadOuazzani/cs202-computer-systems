#include <stdio.h>
#include <math.h> // for log()
#include <float.h> // for DBL_MAX
/*
	Exercise 2

    Entropy of frequency counts.

    Objectives: learn to debug corner cases.
    
*/

#define MAX_NB_VARIABLES 10

// ==================================================
double log2(double x) {
  if (x <= 0.0) return -DBL_MAX;
  return log(x) / log(2.0);
}

// ==================================================
double xlog2x(double x) {
  if (x < 0.0) return -DBL_MAX;
  if (x == 0.0) return 0.0;
  return x*log2(x);
}

// ==================================================
double entropy(double* proba, size_t size)
{
  double sum = 0.0;
  for (size_t i = 0; i < size; ++i) {
    sum -= xlog2x(proba[i]);
  }
  return sum;
}

// ==================================================
void freq2proba(unsigned int* freq, double* proba, size_t size)
{
  unsigned int total_mass = 0;
  for (size_t i = 0; i < size; ++i) {
    total_mass += freq[i];
  }
 
  for (size_t i = 0; i < size; ++i) {
    proba[i] = (double)freq[i] / total_mass;
  }
}

// ==================================================
void input_freq(unsigned int* freq, size_t* size)
{
  *size = 0;
  do {
    printf("How many possible values (<= %d) ? ",
           MAX_NB_VARIABLES);
    scanf("%zu", size);
  } while ((*size == 0) || (*size > MAX_NB_VARIABLES));

  for (size_t i = 0; i < *size; ++i) {
    printf("Count (= frequency) for value #%zu ? ", i+1);
    scanf("%u", freq + i);
  }
}

// ==================================================
int main(void)
{
  unsigned int frequencies[MAX_NB_VARIABLES];
  size_t size;
  input_freq(frequencies, &size);

  double probabilities[MAX_NB_VARIABLES];
  freq2proba(frequencies, probabilities, size);

  printf("The entropy of your histogram is %g bit\n",
         entropy(probabilities, size));

  return 0;
}
