//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   c4/test/tstifpstream.cc
 * \author Mathew Cleveland
 * \date   Wed Apr 28 09:31:51 2010
 * \brief  Tests ifpstream class for paralle read
 * \note   Copyright (C) 2016-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#include "c4/ParallelUnitTest.hh"
#include "c4/ifpstream.hh"
#include "c4/ofpstream.hh"
#include "ds++/Release.hh"
#include <sstream>

using namespace std;
using namespace rtt_dsxx;
using namespace rtt_c4;

//----------------------------------------------------------------------------//
// TESTS
//----------------------------------------------------------------------------//

void write_stream_ascii() {

  unsigned const pid = rtt_c4::node();
  ofpstream out("tstifpstream.txt");

  if (pid == 0)
    out << "just rank 0" << endl;

  out << "MPI rank " << pid << " reporting ..." << endl;
  out.send();
  out.shrink_to_fit();

  out << "MPI rank " << pid << " reporting a second time ..." << endl;
  out.shrink_to_fit();
  out.send();
}

void write_stream_binary() {

  unsigned const pid = rtt_c4::node();
  ofpstream out("tstifpstream.bin", std::ofstream::binary);

  if (pid == 0)
    out << "just rank 0" << endl;

  out << "MPI rank " << pid << " reporting ..." << endl;
  out.send();
  out.shrink_to_fit();

  out << "MPI rank " << pid << " reporting a second time ..." << endl;
  out.shrink_to_fit();
  out.send();
}

//----------------------------------------------------------------------------//
void tstifpstream(UnitTest &ut) {

  int pid = rtt_c4::node();

  write_stream_ascii();
  // Read file on head rank, check for correct conversion and ordering
  ifpstream in("tstifpstream.txt");
  unsigned zeroth_buffer_size = 12;
  if (pid != 0)
    zeroth_buffer_size = 0;
  unsigned first_buffer_size = 25;
  unsigned second_buffer_size = 39;

  // read first strings only on proc 0
  std::string compare_string;
  if (pid == 0)
    compare_string = "just rank 0\n";
  std::string zeroth_string(zeroth_buffer_size, '\0');
  in.fill_buffers(zeroth_buffer_size);
  in.read(&zeroth_string[0], zeroth_buffer_size);
  if (zeroth_string.compare(compare_string) != 0)
    ITFAILS;

  // read first strings on each rank
  compare_string = "MPI rank " + to_string(pid) + " reporting ...\n";
  std::string first_string(first_buffer_size, '\0');
  in.fill_buffers(first_buffer_size);
  in.read(&first_string[0], first_buffer_size);
  if (first_string.compare(compare_string) != 0)
    ITFAILS;

  // read second strings on each rank
  compare_string =
      "MPI rank " + to_string(pid) + " reporting a second time ...\n";
  std::string second_string(second_buffer_size, '\0');
  in.fill_buffers(second_buffer_size);
  in.read(&second_string[0], second_buffer_size);
  if (second_string.compare(compare_string) != 0)
    ITFAILS;

  if (pid == 0)
    PASSMSG("completed serialized ascii read without hanging or segfaulting");
}

//----------------------------------------------------------------------------//
void tstifpstream_binary(UnitTest &ut) {

  int pid = rtt_c4::node();

  write_stream_binary();
  // Read file on head rank, check for correct conversion and ordering
  ifpstream in("tstifpstream.bin", std::ifstream::binary);
  unsigned zeroth_buffer_size = 12;
  if (pid != 0)
    zeroth_buffer_size = 0;
  unsigned first_buffer_size = 25;
  unsigned second_buffer_size = 39;

  // read first strings only on proc 0
  std::string compare_string;
  if (pid == 0)
    compare_string = "just rank 0\n";
  std::string zeroth_string(zeroth_buffer_size, '\0');
  in.fill_buffers(zeroth_buffer_size);
  in.read(&zeroth_string[0], zeroth_buffer_size);
  if (zeroth_string.compare(compare_string) != 0)
    ITFAILS;

  // read first strings on each rank
  compare_string = "MPI rank " + to_string(pid) + " reporting ...\n";
  std::string first_string(first_buffer_size, '\0');
  in.fill_buffers(first_buffer_size);
  in.read(&first_string[0], first_buffer_size);
  if (first_string.compare(compare_string) != 0)
    ITFAILS;

  // read second strings on each rank
  compare_string =
      "MPI rank " + to_string(pid) + " reporting a second time ...\n";
  std::string second_string(second_buffer_size, '\0');
  in.fill_buffers(second_buffer_size);
  in.read(&second_string[0], second_buffer_size);
  if (second_string.compare(compare_string) != 0)
    ITFAILS;

  if (pid == 0)
    PASSMSG("completed serialized binary read without hanging or segfaulting");
}

//----------------------------------------------------------------------------//
int main(int argc, char *argv[]) {
  rtt_c4::ParallelUnitTest ut(argc, argv, release);
  try {
    tstifpstream(ut);
    tstifpstream_binary(ut);
  }
  UT_EPILOG(ut);
}

//----------------------------------------------------------------------------//
// end of tstofpstream.cc
//----------------------------------------------------------------------------//
