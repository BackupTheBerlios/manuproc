#include <string>
#include <Aux/itos.h>

std::string EmptyInt_4TeX(int i)
    {if (i==0) return ""; 
     else return itos(i);
    }
/*
std::string EmptyInt_4TeX(double i)
    {if (i==0.0) return ""; 
     else return dtos(i);
    }
*/