/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "value_object.h"
#include <sstream>

using namespace boost;
using namespace std;

/***************
 * ValueObject *
 ***************/

/*****************
 * CPValueObject *
 *****************/
string CPValueObject::str() {
  ostringstream oss;
  oss << "[CPValue=" << value << "]";
  return oss.str();
}

/******************
 * DotValueObject *
 ******************/
string DotValueObject::str() {
  ostringstream oss;
  oss << "[DotValue=" << dotid << "]";
  return oss.str();
}
