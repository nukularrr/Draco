//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   c4/ifpstream.hh
 * \author Mathew Cleveland
 * \brief  Define class ifpstream
 * \note   Copyright (C) 2018-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#ifndef c4_ifpstream_hh
#define c4_ifpstream_hh

#include <fstream>
#include <sstream>
#include <vector>

#include "c4/config.h"

namespace rtt_c4 {

//============================================================================//
/*!
 * \class ifpstream
 *
 * Input stream for serializing input to all MPI ranks.
 *
 * This offloads all file IO to a single processor. The master processor reads
 * all data from the file and broadcasts it to the MPI processors.
 *
 * \example c4/test/tstifpstream.cc
 */
//============================================================================//

class ifpstream : public std::stringstream {
public:
  //! Constructor -- default to standard output mode (ASCII)
  ifpstream(std::string const &filename,
            ios_base::openmode const mode = ios_base::out);

  //! Fill buffer for every rank
  void fill_buffers(unsigned const buffer_size);

private:
  ios_base::openmode mode_;
  std::ifstream in_;
};

} // end namespace rtt_c4

#endif // c4_ifpstream_hh

//----------------------------------------------------------------------------//
// end of c4/ifpstream.hh
//----------------------------------------------------------------------------//
