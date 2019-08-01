//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   special_functions/test/tstErf.cc
 * \author Kendra Long
 * \date   Thu Aug  1 09:34:08 2019
 * \brief  Tests the Erf approximation
 * \note   Copyright (C) 2016-2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#include "ds++/Release.hh"
#include "ds++/ScalarUnitTest.hh"
#include "ds++/Soft_Equivalence.hh"
#include "special_functions/Erf.hh"
#include <sstream>

using rtt_dsxx::soft_equiv;
using namespace rtt_sf;

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

//
// Erf(x) Tests
//

// check Erf(0) = 0
void tst_erf0(rtt_dsxx::ScalarUnitTest &ut) {

  double val = Erf(0.0);

  std::ostringstream msg;
  if (soft_equiv(0.0, val)) {
    msg << "Erf(0) returned the expected value, " << val << ".";
    ut.passes(msg.str());
  } else {
    msg << "Erf(0) did NOT return the expected value.\n"
        << "\tExpected 0.0, but got " << val;
    ut.failure(msg.str());
  }
}

// check Erf(large) = 1
void tst_erflg(rtt_dsxx::ScalarUnitTest &ut) {

  double arg = 1.0e6;
  double val = Erf(arg);

  std::ostringstream msg;
  if (soft_equiv(1.0, val)) {
    msg << "Erf(" << arg << ") returned the expected value, " << val << ".";
    ut.passes(msg.str());
  } else {
    msg << "Erf(" << arg << ") did NOT return the expected value.\n"
        << "\tExpected 1.0, but got " << val;
    ut.failure(msg.str());
  }
}

// check Erf(large) = 1
void tst_erflgneg(rtt_dsxx::ScalarUnitTest &ut) {

  double arg = -1.0e6;
  double val = Erf(arg);

  std::ostringstream msg;
  if (soft_equiv(-1.0, val)) {
    msg << "Erf(" << arg << ") returned the expected value, " << val << ".";
    ut.passes(msg.str());
  } else {
    msg << "Erf(" << arg << ") did NOT return the expected value.\n"
        << "\tExpected -1.0, but got " << val;
    ut.failure(msg.str());
  }
}

// check Erf(intermediate) ~= benchmark
void tst_erfim(rtt_dsxx::ScalarUnitTest &ut) {

  double arg = 0.5;
  double val = Erf(arg);
  // This 'benchmark' is the value of Erf(0.5) expected from the approximation
  // (the actual, benchmark value is 0.5204998778)
  double bench = 0.5206605500913;

  std::ostringstream msg;
  if (soft_equiv(bench, val)) {
    msg << "Erf(" << arg << ") returned the expected value, " << val << ".";
    ut.passes(msg.str());
  } else {
    msg << "Erf(" << arg << ") did NOT return the expected value.\n"
        << "\tExpected " << bench << ", but got " << val;
    ut.failure(msg.str());
  }
}

// check Erf(highest error point) returns expected (known) error of ~3.6127e-3
void tst_erfhe(rtt_dsxx::ScalarUnitTest &ut) {

  double arg = 1.3796;
  double val = Erf(arg);
  // This benchmark is the ACTUAL value of Erf(1.3796)
  double bench = 0.948948786;
  double err_bound = 3.6127e-3;

  std::ostringstream msg;
  if (std::fabs(bench - val) <= err_bound) {
    msg << "Erf(" << arg << ") was within the expected error bound, "
        << bench - err_bound << " <= " << val << " <= " << bench + err_bound
        << ".";
    ut.passes(msg.str());
  } else {
    msg << "Erf(" << arg << ") was NOT within the expected error bound.\n"
        << "\tExpected in range of (" << bench - err_bound << ", "
        << bench + err_bound << "), but got " << val;
    ut.failure(msg.str());
  }
}

//--------------------------------------------------------------------------//
// RUN TESTS
//--------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
  rtt_dsxx::ScalarUnitTest ut(argc, argv, rtt_dsxx::release);
  try {
    tst_erf0(ut);
    tst_erflg(ut);
    tst_erflgneg(ut);
    tst_erfim(ut);
    tst_erfhe(ut);
  }
  UT_EPILOG(ut);
}

//--------------------------------------------------------------------------//
// end of tstExpInt.cc
//--------------------------------------------------------------------------//
