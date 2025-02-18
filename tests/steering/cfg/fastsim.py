from DDG4 import DetectorConstruction, Geant4, PhysicsList


class FastSimModelConfig:
    def __init__(self, kernel):
        self.kernel = kernel
        # Names of regions where the model will be applied
        self.region_names = []
        # Particles that the model will be applied to
        self.active_particles = []
        # Output file for transport data
        self.transport_output_file = ""
        # Use simplified geometry for transport?
        self.use_simplified_geo = False
        # Maximum number of transport steps
        self.max_transport_steps = 100
        # Volume until which transport will be performed
        self.transport_limit_volume = ""
        # Hook to generate input for parametrization
        self.parametrization_pdg_id = -1

    def setup(self):
        geant4 = Geant4(self.kernel)
        seq = geant4.detectorConstruction()

        # Configure the model
        for region in self.region_names:
            model = DetectorConstruction(self.kernel, "FastCaloSimModel")
            model.RegionName = region
            model.Enable = True
            model.ApplicableParticles = self.active_particles
            # Custom model properties
            model.TransportOutputFile = self.transport_output_file
            model.UseSimplifiedGeo = self.use_simplified_geo
            model.MaxTransportSteps = self.max_transport_steps
            model.TransportLimitVolume = self.transport_limit_volume
            model.ParametrizationPDG = self.parametrization_pdg_id
            # Attach the model to the region
            seq.adopt(model)

        # Set up the user physics for fast simulation
        physics_list = self.kernel.physicsList()
        fast_physics_list = PhysicsList(
            self.kernel, str("Geant4FastPhysics/FastPhysicsList")
        )
        fast_physics_list.EnabledParticles = self.active_particles
        fast_physics_list.BeVerbose = True
        fast_physics_list.enableUI()
        physics_list.adopt(fast_physics_list)
        physics_list.dump()
