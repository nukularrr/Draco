#include "ds++/Release.hh"
#include "ds++/ScalarUnitTest.hh"
#include "ds++/Soft_Equivalence.hh"
#include "units2/PhysicalConstants.hh"

// Debugging
#include <string>
#include <stdio.h>

void test_static_access(rtt_dsxx::UnitTest &ut) {
  constexpr rtt_units2::PhysicalConstants<rtt_units2::CGS> pc_cgs;
  constexpr rtt_units2::PhysicalConstants<rtt_units2::SI> pc_SI;
  printf("CGS c = %e cm s^-1\n", pc_cgs.c());
  printf("SI  c = %e m s^-1\n", pc_SI.c());
}

int main(int argc, char *argv[]) {
  rtt_dsxx::ScalarUnitTest ut(argc, argv, rtt_dsxx::release);
  try {
    test_static_access(ut);
  }
  UT_EPILOG(ut);
}
