
#include <tigerc/Interpreter/Interpreter.h>

#include <sstream>

using namespace tlang;

Interpreter::Interpreter() : 
  m_lexer(),
  m_parser(m_lexer),
  m_location(0) { }

int Interpreter::parse() {
  m_location = 0;
  return m_parser.parse();
}

void Interpreter::clear() {
  m_location = 0;
}

std::string Interpreter::str() const {
  std::stringstream s;
  s << "Hi";
  return s.str();
}

unsigned int Interpreter::location() const {
  return m_location;
}
