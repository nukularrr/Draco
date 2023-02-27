//--------------------------------------------*-C++-*---------------------------------------------//
/*!
 * \file   kde/kde.hh
 * \author Mathew Cleveland
 * \brief  Define class kernel density estimator class
 * \note   Copyright (C) 2021-2022 Triad National Security, LLC., All rights reserved. */
//------------------------------------------------------------------------------------------------//

#ifndef kde_kde_hh
#define kde_kde_hh

#include "quick_index.hh"
#include "c4/global.hh"
#include <array>
#include <vector>

namespace rtt_kde {

//================================================================================================//
/*!
 * \class kde
 * \brief kernel density estimator class for generated smoothed reconstructions of point wise PDF
 *        data
 *
 * Returns a KDE reconstruction of a multidimensional distribution
 */
//================================================================================================//
class kde {
public:
  //! Constructor
  explicit kde(const std::array<bool, 6> reflect_boundary_ = {false, false, false, false, false,
                                                              false})
      : reflect_boundary(reflect_boundary_) {}

  //! Reconstruct distribution
  std::vector<double> reconstruction(const std::vector<double> &distribution,
                                     const std::vector<int> &reconstruction_mask,
                                     const std::vector<std::array<double, 3>> &one_over_band_width,
                                     const quick_index &qindex,
                                     const double discontinuity_cutoff = 1.0) const;
  std::vector<double>
  weighted_reconstruction(const std::vector<double> &distribution,
                          const std::vector<double> &bandwidthweights,
                          const std::vector<int> &reconstruction_mask,
                          const std::vector<std::array<double, 3>> &one_over_band_width,
                          const quick_index &qindex, const double discontinuity_cutoff = 1.0) const;

  //! Reconstruct distribution by sampling the surrounding distribution to a fixed integration grid
  std::vector<double>
  sampled_reconstruction(const std::vector<double> &distribution,
                         const std::vector<int> &reconstruction_mask,
                         const std::vector<std::array<double, 3>> &one_over_band_width,
                         const quick_index &qindex, const double discontinuity_cutoff = 1.0) const;

  //! Reconstruct distribution in logarithmic space
  std::vector<double>
  log_reconstruction(const std::vector<double> &distribution,
                     const std::vector<int> &reconstruction_mask,
                     const std::vector<std::array<double, 3>> &one_over_band_width,
                     const quick_index &qindex, const double discontinuity_cutoff = 1.0) const;

  //! Apply conservation to reconstructed distribution
  void apply_conservation(const std::vector<double> &original_distribution,
                          const std::vector<int> &maskids,
                          const std::vector<int> &conservation_mask,
                          std::vector<double> &new_distribution, const bool domain_decompsed) const;
  // STATICS

  //! Epanechikov Kernel
  inline double epan_kernel(const double x) const;

  //! Transform the solution into log space
  inline double log_transform(const double value, const double bias) const;

  //! Move the solution back from log space
  inline double log_inv_transform(const double log_value, const double bias) const;

private:
  //! Private function to calculate kernel weight
  double calc_weight(const std::array<double, 3> &r0, const std::array<double, 3> &one_over_h0,
                     const std::array<double, 3> &r, const std::array<double, 3> &one_over_h,
                     const quick_index &qindex, const double &discontinuity_cutoff,
                     const double scale = 1.0) const;

  //! Private function to calculate the window bounds
  void calc_win_min_max(const quick_index &qindex, const std::array<double, 3> &position,
                        const std::array<double, 3> &one_over_bandwidth, std::array<double, 3> &min,
                        std::array<double, 3> &max) const;

  //! Private function to calculate the reconstruction wight on a spherical grid
  double calc_spherical_weight(const std::array<double, 3> &r0,
                               const std::array<double, 3> &one_over_h0,
                               const std::array<double, 3> &r,
                               const std::array<double, 3> &one_over_h, const quick_index &qindex,
                               const double &discontinuity_cutoff) const;

  //! Private function to calculate the reconstruction weight on a Cartesian weight
  double calc_cartesian_weight(const std::array<double, 3> &r0,
                               const std::array<double, 3> &one_over_h0,
                               const std::array<double, 3> &r,
                               const std::array<double, 3> &one_over_h, const quick_index &qindex,
                               const double &discontinuity_cutoff) const;

  // DATA
  //! reflecting boundary conditions [lower_x, upper_x, lower_y, upper_y, lower_z, upper_z]
  const std::array<bool, 6> reflect_boundary;
};

} // end namespace rtt_kde

#include "kde.i.hh"

#endif // kde_kde_hh

//------------------------------------------------------------------------------------------------//
// end of kde/kde.hh
//------------------------------------------------------------------------------------------------//
