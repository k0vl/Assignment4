#include "innerproduct.h"
//#include <sstream>

const static int COARSENESS = 2;

double rec_cilkified(double *a, double *b, int n)
{
	if(n < COARSENESS){
		double sum = 0.0;
		for(int i = 0; i < n; i++){
			sum += a[i] * b[i];
		}
		return sum;
	} else {
		int pivot = n / 2;
		double a_sum = cilk_spawn rec_cilkified(a, b, pivot);
		double b_sum = rec_cilkified(a + pivot, b + pivot, n - pivot);
		cilk_sync;
		// std::ostringstream s;
		// s << n << ": " << (a_sum + b_sum) << std::endl;
		// std::cout << s.str();
		return a_sum + b_sum;
	}
}

double loop_cilkified(double *a, double *b, int n)
{
	double inner_sums[n/COARSENESS];
	cilk_for(int i = 0; i < n/COARSENESS; i++){
		inner_sums[i] = 0.0;
		for(int j = 0; j < COARSENESS; j++){
			inner_sums[i] += a[i*COARSENESS + j] * b[i*COARSENESS + j];
		}
	}

	double outer_sum = 0.0;
	for(int i = 0; i < n/COARSENESS; i++){
		outer_sum += inner_sums[i];
	}
	return outer_sum;
}

double hyperobject_cilkified(double *a, double *b, int n)
{
	return 1;
}

