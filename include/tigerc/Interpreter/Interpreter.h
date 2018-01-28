#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <tigerc/Lex/Lexer.h>
#include <tigerc/Parse/Parser.h>

namespace tlang {
  class Interpreter {
    public:
      Interpreter();

      int parse();
      void clear();
      std::string str() const;
      
      friend class Parser;
      friend class Lexer;
    private:
      unsigned int location() const;
    private:
      Lexer m_lexer;
      Parser m_parser;
      unsigned int m_location;
  };
}

#endif
