#ifndef TRACKMSG_H
#define TRACKMSG_H

#include <cmath>
#include <iomanip>
#include <sstream>

#include <G4ParticleDefinition.hh>
#include <G4Track.hh>

// You can define some ANSI color codes for convenience:
#define RESET "\033[0m"
#define BOLDGREEN "\033[1;32m"
#define BOLDYELLOW "\033[1;33m"
#define BOLDCYAN "\033[1;36m"
#define BOLDMAGENTA "\033[1;35m"

/**
 * @brief Build a formatted multi-line message from a G4Track alone,
 *        using an ASCII box layout for nicer readability, with colored headers.
 */
inline auto print_track(const G4Track* track) -> std::stringstream
{
  // Access necessary info
  const auto* pdef = track->GetDefinition();
  const auto& pos = track->GetPosition();
  const auto& mom = track->GetMomentum();

  // PDG ID (defaults to 0 if definition not found)
  const double pdgId = pdef ? pdef->GetPDGEncoding() : 0;

  // Kinetic Energy
  const double e_kin = track->GetKineticEnergy();

  // Create and fill the stringstream
  std::stringstream msg;
  msg << "\n"
      << "  +------------------------------------------------------------+\n"
      << "  |  " << BOLDCYAN << "Received particle: " << pdgId << RESET << "\n"
      << "  |                                                             \n"
      << std::fixed << std::setprecision(2) << "  |    " << BOLDGREEN
      << "Position Information" << RESET << "  \n"
      << "  |    x      = " << pos.x() << "\n"
      << "  |    y      = " << pos.y() << "\n"
      << "  |    z      = " << pos.z() << "\n"
      << "  |    r      = " << pos.perp() << "\n"
      << "  |    eta    = " << pos.eta() << "\n"
      << "  |    phi    = " << pos.phi() << "\n"
      << "  |                                                             \n"
      << "  |    " << BOLDGREEN << "Momentum Information" << RESET << "  \n"
      << "  |    px     = " << mom.x() << "\n"
      << "  |    py     = " << mom.y() << "\n"
      << "  |    pz     = " << mom.z() << "\n"
      << "  |    |p|    = " << mom.mag() << "\n"
      << "  |    Pt     = " << mom.perp() << "\n"
      << "  |    eta    = " << mom.eta() << "\n"
      << "  |    phi    = " << mom.phi() << "\n"
      << "  |    Ekin   = " << e_kin << "\n"
      << "  +------------------------------------------------------------+\n";

  return msg;
}

#endif  // TRACKMSG_H
