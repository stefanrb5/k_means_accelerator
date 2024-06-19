#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "defines.hpp"
#include <systemc>
#include <iostream>
#include <string.h>
#include <bitset>
#include <math.h>
#include <string>
#include <tlm>
#include <vector>

using namespace std;
using namespace sc_core;
using namespace tlm;
using namespace cv;
using namespace sc_dt;

/*static const int DATA_WIDTH = 32;
//static const int FIXED_WIDTH = 3;
static const int CHAR_LENGHT = 8;
static const int CHARS_AMOUNT = DATA_WIDTH / CHAR_LENGHT;*/

int toInt(unsigned char *buf);
void toUchar(unsigned char *buf,int val);
unsigned char* scalarArray_to_UcharArray(const vector<Scalar>& scalarArray);
vector<Point3i> UcharArray_to_pointArray(unsigned char* charArray, int size);

#endif

