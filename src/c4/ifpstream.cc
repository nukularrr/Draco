//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   c4/ifpstream.cc
 * \author Mathew Cleveland
 * \date   Mon Jun 25 11:36:43 MDT 2018
 * \brief  Define methods of class ifpstream
 * \note   Copyright (C) 2018-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#include "ifpstream.hh"
#include "C4_Functions.hh"

namespace rtt_c4 {
using namespace std;

//----------------------------------------------------------------------------//
/*! Create an ifpstream to for a parallel file read.
 *
 * Create an ifpstream that reads and brocasts input file streams to all ranks
 *
 * \param[in] filename Name of the file to which is being read
 * \param[in] mode File read mode (ascii/binary)-- defaults to ascii
 */
ifpstream::ifpstream(std::string const &filename,
                     ios_base::openmode const mode) {
  mode_ = mode;
  if (rtt_c4::node() == 0) {
    in_.open(filename, mode);
  }
}

//----------------------------------------------------------------------------//
/*! Fill parallel buffers with data from the input file
 *
 * \param[in] local process buffer size
 */
void ifpstream::fill_buffers(unsigned const buffer_size) {
  // initialize N process vector of buffer sizes
  std::vector<unsigned> buffer_sizes(rtt_c4::nodes(), 0);
  // fill in local buffer size
  buffer_sizes[rtt_c4::node()] = buffer_size;
  // reduce buffer_size to all ranks
  rtt_c4::global_sum(&buffer_sizes[0], rtt_c4::nodes());


  if (rtt_c4::node() == 0) {
    // populate node zero buffer
    std::string local_string(buffer_size, '\0');
    in_.read(&local_string[0], buffer_size);
    str(local_string);
    // loop over and broadcast remaining buffers
    for (int node = 1; node < rtt_c4::nodes(); node++) {
      std::string broadcast_string(buffer_sizes[node], '\0');
      in_.read(&broadcast_string[0], buffer_sizes[node]);
      rtt_c4::send(&broadcast_string[0], buffer_sizes[node], node);
    }
  } else {
    // populate local buffer
    std::string local_string(buffer_size, '\0');
    rtt_c4::receive(&local_string[0], buffer_size, 0);
    str(local_string);
  }
}


} // end namespace rtt_c4

//----------------------------------------------------------------------------//
// end of ifpstream.cc
//----------------------------------------------------------------------------//
