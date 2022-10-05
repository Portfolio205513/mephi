#include <stdio.h>
#include <stdint.h>


double sum_n(int64_t n) {
	double previous_i = - (double) n / 6.0;
	double sum = previous_i;
	for (int64_t k = 1; k < n; k++){
		previous_i *= (double) (k - n) / (double) ((n - k + 1) * (2 * k + 2) * (2 * k + 3));
		sum += previous_i;
	}
	return sum;
}


void lim_e(double e) {
	if (e > 1.0 / 6.0 - 1.0 / 120.0)
		printf("limit summa = %f\n", -1.0 / 6.0);
	else
		printf("limit summa doesn't exist\n");
}


int main() {
	int64_t n;
	double e;
	printf("Enter natural number n:\n");
	scanf("%ld", &n);
	printf("n's summa: %lf\n", sum_n(n));
	printf("Enter positive number epsilon:\n");
	scanf("%lf", &e);
	lim_e(e);
	return 0;
}
