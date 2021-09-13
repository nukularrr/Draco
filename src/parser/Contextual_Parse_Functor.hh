//--------------------------------------------*-C++-*---------------------------------------------//
/*!
 * \file   parser/Contextual_Parse_Functor.hh
 * \author Kent Budge
 * \date   Thu Jul 17 14:08:42 2008
 * \brief  Member definitions of class Abstract_Class_Parser
 * \note   Copyright (C) 2010-2021 Triad National Security, LLC., All rights reserved. */
//------------------------------------------------------------------------------------------------//

#ifndef parser_Contextual_Parse_Functor_hh
#define parser_Contextual_Parse_Functor_hh

#include "parser/Parse_Table.hh"

namespace rtt_parser {

//================================================================================================//
/*!
 * \brief Closure class for wrapping context-dependent parse functions
 *
 * This template class is used to bind a get_context() function to a parse function requiring a
 * context argument, so that the function can be called with the usual two parameters (Token_Stream
 * and int) from an Abstract_Class_Parse_Table.
 *
 * See test/tstAbstract_Class_Contextual_Parser.cc for an example of how it is used.
 */
//================================================================================================//
template <typename Abstract_Class, typename Context, Context const &get_context()>
class Contextual_Parse_Functor {
public:
  //! Primary constructor
  Contextual_Parse_Functor(std::shared_ptr<Abstract_Class> parse_function(Token_Stream &,
                                                                          Context const &))
      : f_(parse_function) {}

  //! Return handle to the parse function.
  std::shared_ptr<Abstract_Class> operator()(Token_Stream &tokens) const {
    return f_(tokens, get_context());
  }

private:
  std::shared_ptr<Abstract_Class> (*f_)(Token_Stream &, Context const &);
};

} // end namespace rtt_parser

#endif // parser_Contextual_Parse_Functor_hh

//------------------------------------------------------------------------------------------------//
// end of parser/Contextual_Parse_Functor.hh
//------------------------------------------------------------------------------------------------//
