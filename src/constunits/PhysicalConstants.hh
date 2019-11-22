namespace rtt_constunits {
struct SI {
  static constexpr double Length = 1.;      // meter
  static constexpr double Mass = 1.;        // kilogram
  static constexpr double Time = 1.;        // second
  static constexpr double Temperature = 1.; // Kelvin
  static constexpr double Current = 1.;     // Amp
  static constexpr double Angle = 1.;       // Radian
  static constexpr double Quantity = 1.;    // Mole
};

struct CGS {
  static constexpr double Length = 1.e2;    // centimeter
  static constexpr double Mass = 1.e3;      // gram
  static constexpr double Time = 1.;        // second
  static constexpr double Temperature = 1.; // Kelvin
  static constexpr double Current = 1.e-1;  // Biot
  static constexpr double Angle = 1.;       // Radian
  static constexpr double Quantity = 1.;    // Mole
};

struct CGSH {
  static constexpr double Length = 1.e2;                  // centimeter
  static constexpr double Mass = 1.e3;                    // gram
  static constexpr double Time = 1.e8;                    // shake
  static constexpr double Temperature = 8.61733238496e-8; // Kelvin
  static constexpr double Current = 1.e-1;                // Biot
  static constexpr double Angle = 1.;                     // Radian
  static constexpr double Quantity = 1.;                  // Mole
}

template <typename UNITS>
class PhysicalConstants {
public:
  constexpr PhysicalConstants() {}

  //! accesses Avogadro's number
  constexpr double avogadro() const { return d_avogadro; }

  //! accesses Planck's constant (Energy Time)
  constexpr double planck() const { return d_planck; }
  constexpr double h() const { return planck(); }

  //! accesses the speed of light (Length Time^-1)
  constexpr double speedOfLight() const { return d_speed_of_light_SI; }
  constexpr double c() const { return speedOfLight; }

  //! accesses the gas constant (Energy Mole^-1 Temperatre^-1)
  constexpr double gasConstant() const { return d_gasConstant };

private:
  // Derived unit conversions
  constexpr double Energy;

  // Dimensionless constants
  static constexpr double d_avogadro = 6.022140857e23;

  // SI values of fundamental constants
  static constexpr double d_planck_SI = 6.62607015e-34;       // J s
  static constexpr double d_speed_of_light_SI = 2.99792458e8; // m s^-1

  // Values converted to templated unit system
  static constexpr double d_planck =
      d_planck_SI *UNITS::Mass*pow(UNITS::Length,2)/UNITS::Time;
  static constexpr double d_speed_of_light =
          d_speed_of_light_SI * UNITS::Length / UNITS::Time;
};
} // namespace rtt_constunits
