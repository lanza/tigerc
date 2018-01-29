#ifndef LEXER_H
#define LEXER_H

/**
 * The generated Flex class name is yyFlexLexer by default. If you want to use more flex-generated
 * classes you need to name them differently. See scanner.l for prefix option.
 *
 * Unfortunately the implementation relies on this trick with redefining class name
 * with a preprocessor macro. See GNU flex manual, "Generating C++ Scanners" section
 */

#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer tlang_FlexLexer
#include <FlexLexer.h>
#endif

/**
 * The scanner method signature is defined by this macro. The original yylex() returns an
 * int. Since Bison 3 uses symbol_type, we must chagne the returned type. We also rename it
 * to something sane, since you cannot overload the return type.
 */
#undef YY_DECL
#define YY_DECL tlang::Parser::symbol_type tlang::Lexer::get_next_token()

#include <tigerc/Parse/Parser.h>

namespace tlang {
  class Interpreter;

  class Lexer : public yyFlexLexer {
    public:
      Lexer(std::istream* is = 0, std::ostream* os = 0) : yyFlexLexer(is,os) {}
      virtual ~Lexer() {}
      virtual tlang::Parser::symbol_type get_next_token();

    private:
      //Interpreter &m_driver;
  };
}

#endif











