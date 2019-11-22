#include "PhysicalConstants.hh"

template <typename UNITS>
PhysicalConstants::PhysicalConstants() {
  Energy = UNITS::Mass*pow(Units::Length,2)*pow(Units::Time,-2);
}
