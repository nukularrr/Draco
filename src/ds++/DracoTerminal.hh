//--------------------------------------------*-C++-*---------------------------------------------//
/*!
 * \file   ds++/DracoTerminal.hh
 * \author Kelly Thompson
 * \date   Sat Oct 05 2019
 * \brief  Wrapper for a Terminal class that provides colored output.
 * \note   Copyright (C) 2019-2021 Triad National Security, LLC., All rights reserved.
 *
 * \sa https://github.com/certik/terminal/blob/master/terminal.h
 *
 * \todo Consider an enum class for colors that derives from $LS_COLORS on Linux.  This would allow
 *       color selection based on users's terminal colors (e.g.: light vs dark scheme). */
//------------------------------------------------------------------------------------------------//

#ifndef dsxx_dracoterminal_hh
#define dsxx_dracoterminal_hh

#include "terminal/terminal.h"
#include "ds++/config.h"

namespace Term {

//================================================================================================//
/*!
 * \class DracoTerminal
 * \brief Global scope singleton object to ensure terminal setup/teardown is done correctly.
 *
 * This will self construct on first call to Term::ccolor and it will remain in scope until the
 * program exits.
 */
class DracoTerminal {

  // >> DATA <<

  /*! Construct a terminal object on creation.  This is will do some terminal initialization that is
   *  required for some older software (like Windows cmd.exe). */
  Term::Terminal term;

  /*!
   * \brief Private constructor and destructor so that no objects can be created or destroyed.
   * \param[in] useColor_in if true use ansi-color for terminal output.
   */
  explicit DracoTerminal(bool useColor_in = true) { useColor = useColor_in; }
  //! Private default destructor.
  ~DracoTerminal() = default;

public:
  //! Disable copy construction to prevent making copies (this is a singleton pattern).
  DracoTerminal(const DracoTerminal &) = delete;
  //! Disable assigment to prevent making copies (this is a singleton pattern).
  DracoTerminal &operator=(const DracoTerminal &) = delete;

  // >> DATA <<

  //! Set to false to disable all color.
  DLL_PUBLIC_dsxx static bool useColor;

  //! Colors based on condition (error, warn, etc).
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> error;
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> warning;
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> note;
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> quote;
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> pass;
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> fail;
  DLL_PUBLIC_dsxx static const std::vector<uint32_t> reset;

  // >> MANIPULATORS <<

  /*! \brief Calling this public function should always return true. Creates the singleton object
   *         on the first call.
   *
   * \param[in] useColor_in If false, then avoid inserting color control strings when Term::ccolor
   *               is called.
   *
   * If this singleton has not be created, then it will be created.
   *
   * \note This is a  Meyer's Singleton. There is no heap allocation.
   */
  static DracoTerminal &getInstance(bool useColor_in = true) {
    static DracoTerminal instance(useColor_in);
    return instance;
  }
};

//------------------------------------------------------------------------------------------------//
/*!
 * \brief Replacement for terminal/terminal.h's color function that will ensure the global
 *         singleton terminal object has been constructed prior to the use of color strings.
 *
 * \param[in] value A value that represents an ansi-color once \c value is converted to an \c
 *               int. Normally, this will be one of {error, warning, ...} as defined in the
 *               DracoTerminal class definition.
 */
template <typename T> std::string ccolor(T const value) {
  std::string retVal;
  Term::DracoTerminal::getInstance();
  if (Term::DracoTerminal::useColor)
    retVal += "\033[" + std::to_string(static_cast<int>(value)) + "m";
  return retVal;
}
//! specialization for vector<uint32_t> to be used with Term::DracoTerminal::error, etc.
std::string ccolor(std::vector<uint32_t> const &value);

} // namespace Term

#endif // dsxx_dracoterminal_hh

//------------------------------------------------------------------------------------------------//
// end of ds++/DracoTerminal.hh
//------------------------------------------------------------------------------------------------//
