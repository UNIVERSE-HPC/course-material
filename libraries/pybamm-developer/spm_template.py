#
# Single Particle Model (SPM)
#
import pybamm


class WorkshopSPM(pybamm.BaseModel):
    """Single Particle Model (SPM) model of a lithium-ion battery, from
    :footcite:t:`Marquis2019`.

    Parameters
    ----------
    name : str, optional
        The name of the model.
    """

    def __init__(self, name="Single Particle Model"):
        super().__init__({}, name)

        # This command registers the article where the model comes from so it appears
        # when calling `pybamm.print_citations()`
        pybamm.citations.register("Marquis2019")

        ######################
        # Variables
        ######################
        # Define the variables of the model.


        ######################
        # Parameters
        ######################
        # You need to fill this section with the parameters of the model. To ensure
        # it works within PyBaMM, you need to use the same names as in PyBaMM's parameter
        # sets (e.g. see https://github.com/pybamm-team/PyBaMM/blob/develop/pybamm/input/parameters/lithium_ion/Chen2020.py)


        ######################
        # Particle model
        ######################
        # Define the governing equations for each particle.


        ######################
        # Output variables
        ######################
        # Populate the variables dictionary with any variables you want to compute
        self.variables = {}

    # The following attributes are used to define the default properties of the model,
    # which are used by the simulation class if unspecified.
    @property
    def default_geometry(self):
        return None

    @property
    def default_submesh_types(self):
        return None

    @property
    def default_var_pts(self):
        return None

    @property
    def default_spatial_methods(self):
        return None

    @property
    def default_solver(self):
        return None
    
    @property
    def default_quick_plot_variables(self):
        return None