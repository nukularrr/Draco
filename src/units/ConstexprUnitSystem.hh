namespace rtt_units {
struct SI {
  static constexpr double Length = 1.;      // meter
  static constexpr double Mass = 1.;        // kilogram
  static constexpr double Time = 1.;        // second
  static constexpr double Temperature = 1.; // Kelvin
  static constexpr double Current = 1.;     // Amp
  static constexpr double Charge = 1.;      // Coulomb
  static constexpr double Capacitance = 1.; // Farad
  static constexpr double Angle = 1.;       // Radian
  static constexpr double Quantity = 1.;    // Mole
};

struct CGS {
  static constexpr double Length = 1.e2;    // centimeter
  static constexpr double Mass = 1.e3;      // gram
  static constexpr double Time = 1.;        // second
  static constexpr double Temperature = 1.; // Kelvin
  static constexpr double Current = 1.e-1;  // Biot
  static constexpr double Charge = 2.997924580e9 // Statcoulomb
  static constexpr double Capacitance = 8.9831483395497e11; // Statfarad
  static constexpr double Angle = 1.;       // Radian
  static constexpr double Quantity = 1.;    // Mole
};

struct CGSH {
  static constexpr double Length = 1.e2;                  // centimeter
  static constexpr double Mass = 1.e3;                    // gram
  static constexpr double Time = 1.e8;                    // shake
  static constexpr double Temperature = 8.61733238496e-8; // Kelvin
  static constexpr double Current = 1.e-1;                // Biot
  static constexpr double Charge = 2.997924580e9 // Statcoulomb
  static constexpr double Capacitance = 8.9831483395497e11; // Statfarad
  static constexpr double Angle = 1.;                     // Radian
  static constexpr double Quantity = 1.;                  // Mole
};

template <typename UNITS>
class PhysicalConstexprs {
public:
  constexpr PhysicalConstexprs() {}

  //! accesses Avogadro's number
  constexpr double avogadro() const { return d_avogadro; }
  constexpr double Na() const { return avogadro(); }

  //! accesses the fine structure constant
  constexpr double fineStructure() const { return d_fineStructure; }
  constexpr double alpha() const { return fineStructure(); }

  //! accesses Planck's constant
  constexpr double planck() const { return d_planck; }
  constexpr double h() const { return planck(); }

  //! accesses the molar gas constant
  constexpr double gasConstant() const { return d_gasConstant; }
  constexpr double R() const { return gasConstant(); }

  //! accesses the Boltzmann constant
  constexpr double boltzmann() const { return d_boltzmann; }
  constexpr double k() const { return boltzmann(); }

  //! accesses the electron charge
  constexpr double electronCharge() const { return d_electronCharge; }
  constexpr double e() const { return electronCharge(); }

  //! accesses the speed of light
  constexpr double speedOfLight() const { return d_speed_of_light_SI; }
  constexpr double c() const { return d_speed_of_light; }

  //! accesses the Stefan-Boltzmann constant
  constexpr double stefanBoltzmann() const { return d_stefanBoltzmann; }
  constexpr double sigma() const { return stefanBoltzmann(); }

  //! accesses the gravitational constant
  constexpr double gravitationalConstant() const { return d_gravitationalConstant; }
  constexpr double G() const 

private:
  // Derived unit conversions
  static constexpr double Force = UNITS::Mass*UNITS::Length/(UNITS::Time*UNITS::Time);
  static constexpr double Energy = Force*UNITS::Length;

  // Dimensionless constants
  //! Avogadro cosntant (CODATA 2018 exact value)
  static constexpr double d_avogadro = 6.02214076e23;
  
  //! Fine structure constant (CODATA 2018 value)
  static constexpr double d_finestructure = 7.2973525693e-3;

  // SI values of fundamental constants
  //! Planck constant (CODATA 2018 exact value)
  static constexpr double d_planck_SI = 6.62607015e-34;       // J s
  
  //! Molar gas constant (CODATA 2018 exact value up to precision)
  static constexpr double d_gasConstant = 8.3144621618;          // J mol^-1 K^-1
  
  //! Boltzmann constant (CODATA 2018 exact value)
  static constexpr double d_boltzmann = 1.380649e-23;     // J K^-1
  
  //! Electron charge (CODATA 2018 exact value)
  static constexpr double d_electronCharge = 1.602176634e-19; // C
  
  //! Speed of light (CODATA 2018 exact value)
  static constexpr double d_speed_of_light_SI = 2.99792458e8; // m s^-1
  
  //! Stefan-Boltzmann constant (CODATA 2018 exact value up to precision)
  static constexpr double d_stefanBoltzmann = 5.670374419e-8; // W m^-2 K^-4
  
  //! Gravitational constant (CODATA 2018 value)
  static constexpr double d_gravitationalConstant = 6.67430e-11; // m^3 kg^-1 s^-2
  
  //! Standard acceleration of gravity (CODATA 2018 exact value)
  static constexpr d_accelerationFromGravity = 9.80665; // m s^-2
  
  //! Faraday constant (CODATA 2018 exact value up to precision)
  static constexpr d_faradayConstant = 96485.33212; // C mol^-1
  
  //! Permeability of free space (CODATA 2018 value) 
  static constexpr d_permeabilityOfVacuum = 1.25663706212e-6; // N A^-2
  
  //! Permittivity of free space (CODATA 2018 value)
  static constexpr d_permittivityOfVacuum = 8.8541878128e-12; // F m^-1
  
  //! Classical electron radius (CODATA 2018 value)
  static constexpr d_classicalElectronRadius = 2.8179403262e10-15; // m
  
  //! Electron rest mass (CODATA 2018 value)
  static constexpr d_electronMass = 9.1093837015e-31; // kg
  
  //! Proton rest mass (CODATA 2018 value)
  static constexpr d_protonMass = 1.67262192369e-27; // kg


  // Values converted to templated unit system
  //static constexpr double d_planck =
  //    d_planck_SI *UNITS::Mass*pow(UNITS::Length,2)/UNITS::Time;
  static constexpr double d_planck = d_planck_SI*Energy*UNITS::Time;
  static constexpr double d_speed_of_light =
          d_speed_of_light_SI * UNITS::Length / UNITS::Time;
};
} // namespace rtt_constunits
