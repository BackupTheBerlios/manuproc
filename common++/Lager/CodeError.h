#ifndef CODEERROR_H
#define CODEERROR_H

#include <iostream>
#include <string>
#include <exception>

class CodeError : std::exception
{
public:
 int code;
 std::string context;

 CodeError(int c, const std::string &ct)
 : code(c), context(ct) {}
 friend std::ostream &operator<<(std::ostream&,const CodeError &);
 virtual const char* what() const throw() { return "CodeError"; }
};

inline std::ostream &operator<<(std::ostream&o,const CodeError &e)
{  return o<<e.context<<": "<<e.code;
}

#endif
