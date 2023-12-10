#include <omp.h>

#include "Debugger/Application/Pe/PEReader.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

/*
* The get_entropy function calculates the entropy of a range of data in a file.
* The entropy is a measure of the randomness or unpredictability of the data.
* The higher the entropy, the more random the data is.
* The function takes three parameters: a pointer to the data, and the start
* and end indices of the range of data to calculate the entropy for.
*/
//float get_entropy(char* ptr, int start, int end) {
	/*
	* The function first checks if the range is empty (i.e., the start index is equal to the end index).
	* If it is, the entropy is 0.
	*/
	//if ((end - start) == 0) { return 0; }
	/*
	* Next, the function calculates the length of the range and initializes the entropy to 0.
	*/
	//float entropy = 0;
	//int length = end - start;

	/*
	* The function then enters a loop that runs 256 times (since there are 256 possible byte values).
	* For each iteration, it calculates the count of occurrences of the current byte value in the range.
	*/
	//#pragma omp parallel for reduction(+:entropy)
	//for (int a = 0; a < 256; a++) {
	//	int len = length;
		// Count byte by byte, 0x00 to 0xFF
	//	BYTE* p = (BYTE*)(ptr + start);
	//	int count = 0;
	//	while (len--) {
	//		if (*p++ == a) {
	//			count++;
	//		}
	//	}
	/*
	* The function then calculates the probability of the current byte value (p_c), which is the count of occurrences divided by the length of the range.
	*/
	//	double p_c = double(double(count) / double(length));
	/*
	* If the probability is greater than 0, the function adds the negative product of the probability and the base-2 logarithm of the probability to the entropy.
	* Finally, the function returns the calculated entropy.
	*/
	//	if (p_c > 0)entropy += -double(p_c * loga(p_c, 2));
	//}
	//return entropy;
//}

static double loga(double n, double base) {
	return double(log(n) / log(base));
}

float PeReader::CalcEntropy(char* ptr, int start, int end)
{
	if ((end - start) == 0) { return 0; }
	int length = end - start;
	int histogram[256] = { 0 };

	for (int i = start; i < end; i++) {
		histogram[(unsigned char)ptr[i]]++;
	}

	double entropy = 0;
	#pragma omp parallel for reduction(+:entropy)
	for (int i = 0; i < 256; i++) {
		double p_c = double(histogram[i]) / double(length);
		if (p_c > 0) entropy += -double(p_c * loga(p_c, 2));
	}
	return entropy;
}