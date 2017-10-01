#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <iostream>
#include "utils.h"

Utils::Utils()
{

}

float Utils::toRadians(float angle){
    return angle * M_PI / 180.0 ;
}

