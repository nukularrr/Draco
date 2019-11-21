namespace rtt_units2 {
  struct SI {
    static constexpr double L = 1.; 
    static constexpr double T = 1.;
  };

  struct CGS {
    static constexpr double L = 100.;
    static constexpr double T = 1.;
  };

  template<typename UNITS>
  class PhysicalConstants {
    public:
    constexpr PhysicalConstants() {}

    constexpr double c() const { return speed_of_light_SI*UNITS::L/UNITS::T; }

    private:
    static constexpr double speed_of_light_SI = 2.99792458e8; // m s^-1
  };
} // namespace rtt_units2
