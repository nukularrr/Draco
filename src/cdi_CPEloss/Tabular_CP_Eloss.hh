//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_CPEloss/Tabular_CP_Eloss.hh
 * \author Ben R. Ryan
 * \date   2019 Nov 4
 * \brief  Tabular_CP_Eloss class definition.
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#ifndef cdi_CPEloss_Tabular_CP_Eloss_hh
#define cdi_CPEloss_Tabular_CP_Eloss_hh

#include "experimental/mdspan"
#include "cdi/CPCommon.hh"
#include "cdi/CPEloss.hh"
#include "ds++/Assert.hh"
#include "ds++/DracoMath.hh"
#include "units/PhysicalConstexprs.hh"
#include <cmath>
#include <fstream>
#include <string>

namespace rtt_cdi_cpeloss {

namespace stdex = std::experimental;

//============================================================================//
/*!
 * \class Tabular_CP_Eloss
 *
 * \brief Derived rtt_cdi::CPEloss class for tabular eloss.
 *        This class implements the interface found in cdi/CPEloss.hh for
 *        the case where CP energy loss data is in tabular form, stored
 *        in a file.
 */
//============================================================================//

class Tabular_CP_Eloss : public rtt_cdi::CPEloss {
public:
  typedef std::vector<double> sf_double;
  typedef stdex::extents<stdex::dynamic_extent,
                         std::experimental::dynamic_extent,
                         std::experimental::dynamic_extent>
      dynamic_extents_3;

private:
  const std::string filename; //!< Path to tabulated stopping power data file
  std::ifstream file;         //!< File object for stopping power data

  rtt_units::PhysicalConstexprs<rtt_units::CGS> pc; //!< Units

  uint32_t n_energy;          //!< Number of gridpoints in projectile energy
  uint32_t n_density;         //!< Number of gridpoints in target density
  uint32_t n_temperature;     //!< Number of gridpoints in target temperature
  double d_log_energy;        //!< Log spacing of projectile energy gridpoints
  double d_log_density;       //!< Log spacing of target density gridpoints
  double d_log_temperature;   //!< Log spacing of target temperature gridpoints
  double min_log_energy;      //!< Log of minimum projectile energy
  double min_log_density;     //!< Log of minimum target density
  double min_log_temperature; //!< Log of minimum target temperature
  double min_energy;          //!< Minimum target energy
  double max_energy;          //!< Maximum target energy
  double min_density;         //!< Minimum target density
  double max_density;         //!< Maximum target density
  double min_temperature;     //!< Minimum target temperature
  double max_temperature;     //!< Maximum target temperature
  sf_double energies;         //!< Vector of energy gridpoints
  sf_double densities;        //!< Vector of density gridpoints
  sf_double temperatures;     //!< Vector of temperature gridpoints
  // Note that after unit conversions, *_energy is really *_speed

  // Multidimensional view of stored tabulated data
  stdex::basic_mdspan<double, dynamic_extents_3, std::experimental::layout_left>
      stopping_data;

  // Storage for tabulated data
  std::vector<double> stopping_data_1d;

  // Utility for skipping lines
  void skip_lines(uint32_t nlines);

  // Utility for reading a line of an eloss file and as a vector of strings
  std::vector<std::string> const read_line();

public:
  // Constructor
  Tabular_CP_Eloss(const std::string &filename_in, rtt_cdi::CParticle target_in,
                   rtt_cdi::CParticle projectile_in,
                   rtt_cdi::CPModelAngleCutoff model_angle_cutoff_in);

  // >>> ACCESSORS

  //! Get a stopping power.
  double getEloss(const double temperature, const double density,
                  const double v0) const;

  //! Query to see if data is in tabular or functional form (true).
  static constexpr bool is_data_in_tabular_form() { return true; }

  //! Get the name of the associated data file.
  inline std::string getDataFilename() const { return filename; }

  //! Get the material temperature grid.
  sf_double getTemperatureGrid() const { return temperatures; }

  //! Get the material density grid.
  sf_double getDensityGrid() const { return densities; }

  //! Get the projectile energy grid.
  sf_double getEnergyGrid() const { return energies; }

  //! Get the number of material temperature grid points.
  size_t getNumTemperatures() const { return n_temperature; }

  //! Get the number of material density grid points.
  size_t getNumDensities() const { return n_density; }

  //! Get the number of projectile energy grid points.
  size_t getNumEnergies() const { return n_energy; }
};

} // namespace rtt_cdi_cpeloss

#endif // cdi_CPEloss_Tabular_CP_Eloss_hh

//----------------------------------------------------------------------------//
// End cdi_CPEloss/Tabular_CP_Eloss.hh
//----------------------------------------------------------------------------//
