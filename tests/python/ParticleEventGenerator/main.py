import numpy as np
import pyhepmc


class ParticleEventGenerator:
    """
    A class to generate particle events with configurable kinematics.

    This generator produces events using pyhepmc and supports:
      - Fixed eta/phi values.
      - Uniformly sampled eta/phi (via eta_min/eta_max or phi_min/phi_max).
      - Multiple particles per event via eta_list and/or phi_list.
    """

    def __init__(self, seed: int = None):
        """
        Initialize the ParticleEventGenerator.

        Parameters:
            seed (int, optional): Random seed for reproducible sampling.
                                  Defaults to None.
        """
        self.seed = seed
        self.rng = np.random.default_rng(seed)
        self.events = None
        self.momentum_unit = pyhepmc.Units.MomentumUnit.MEV
        self.length_unit = pyhepmc.Units.LengthUnit.MM
        self.status = 1  # Only stable particles

    @staticmethod
    def _create_particle(
        momentum: float, mass: float, phi: float, eta: float, pid: int, status: int
    ) -> pyhepmc.GenParticle:
        """
        Create a single particle with given momentum, mass, and directional angles.

        Returns:
            pyhepmc.GenParticle: The generated particle.
        """
        pT = momentum / np.cosh(eta)
        px = pT * np.cos(phi)
        py = pT * np.sin(phi)
        pz = pT * np.sinh(eta)
        energy = np.sqrt(px**2 + py**2 + pz**2 + mass**2)

        return pyhepmc.GenParticle(pyhepmc.FourVector(px, py, pz, energy), pid, status)

    @staticmethod
    def _validate_parameters(
        nEvents: int,
        eta: float,
        eta_min: float,
        eta_max: float,
        phi: float,
        phi_min: float,
        phi_max: float,
        eta_list: list,
        phi_list: list,
    ) -> int:
        """
        Validate the angular parameters and determine the number of particles per event.

        Raises:
            ValueError: If any conflicting parameter specifications are found.

        Returns:
            int: The number of particles per event.
        """
        if nEvents < 1:
            raise ValueError("nEvents must be greater than zero.")

        # Validate fixed vs. range for eta and phi.
        if eta is not None and (eta_min is not None or eta_max is not None):
            raise ValueError("Cannot specify both fixed and range eta.")
        if phi is not None and (phi_min is not None or phi_max is not None):
            raise ValueError("Cannot specify both fixed and range phi.")

        # Validate list versus fixed/range for eta and phi.
        if eta_list is not None and (
            eta is not None or eta_min is not None or eta_max is not None
        ):
            raise ValueError("Cannot specify eta_list with fixed or range eta.")
        if phi_list is not None and (
            phi is not None or phi_min is not None or phi_max is not None
        ):
            raise ValueError("Cannot specify phi_list with fixed or range phi.")

        # Determine the number of particles per event.
        if eta_list is not None and phi_list is not None:
            if len(eta_list) != len(phi_list):
                raise ValueError("eta_list and phi_list must have the same length.")
            return len(eta_list)
        elif eta_list is not None:
            return len(eta_list)
        elif phi_list is not None:
            return len(phi_list)
        else:
            return 1

    def generate(
        self,
        nEvents: int,
        pid: int,
        momentum: float,
        mass: float = 0.0,
        eta: float = None,
        eta_min: float = None,
        eta_max: float = None,
        phi: float = None,
        phi_min: float = None,
        phi_max: float = None,
        eta_list: list = None,
        phi_list: list = None,
        vertex: tuple = (0, 0, 0, 0),
    ) -> None:
        """
        Generate one or more particle events with configurable eta, phi, and vertex.

        Eta and phi can be provided as fixed values, as ranges for uniform sampling, or
        as lists to create multiple particles within the same event. When a list is provided,
        it overrides fixed or range specifications for that variable.

        Returns:
            pyhepmc.GenEvent or list[pyhepmc.GenEvent]:
                A single event if nEvents == 1, otherwise a list of events.
        """
        # Run validations and determine the number of particles per event.
        num_particles = ParticleEventGenerator._validate_parameters(
            nEvents, eta, eta_min, eta_max, phi, phi_min, phi_max, eta_list, phi_list
        )

        events = []
        for _ in range(nEvents):
            event = pyhepmc.GenEvent(self.momentum_unit, self.length_unit)
            gen_vtx = pyhepmc.GenVertex(pyhepmc.FourVector(*vertex))

            for i in range(num_particles):
                # Determine eta for this particle.
                if eta_list is not None:
                    particle_eta = eta_list[i]
                elif eta_min is not None or eta_max is not None:
                    particle_eta = self.rng.uniform(eta_min, eta_max)
                elif eta is not None:
                    particle_eta = eta
                else:
                    particle_eta = 0.0

                # Determine phi for this particle.
                if phi_list is not None:
                    particle_phi = phi_list[i]
                elif phi_min is not None or phi_max is not None:
                    particle_phi = self.rng.uniform(phi_min, phi_max)
                elif phi is not None:
                    particle_phi = phi
                else:
                    particle_phi = 0.0

                particle = ParticleEventGenerator._create_particle(
                    momentum, mass, particle_phi, particle_eta, pid, self.status
                )
                gen_vtx.add_particle_out(particle)

            event.add_vertex(gen_vtx)
            events.append(event)

        self.events = events

    def save(self, filename: str):
        """
        Save the stored pyhepmc event(s) to a file in ASCII format.
        """
        with pyhepmc.io.WriterAscii(filename) as writer:
            for i, evt in enumerate(self.events):
                evt.event_number = i
                writer.write_event(evt)
