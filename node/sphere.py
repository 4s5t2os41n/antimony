import math

import base
import datum

class Sphere(base.Node3D):
    def __init__(self, name, x, y, z, r):
        """ Constructs a sphere.
        """
        super(Sphere, self).__init__(name, x, y, z)
        self.add_datum('r', datum.FloatDatum(self, r))
        self.add_datum('shape',
                       datum.ExpressionFunctionDatum(self, 'make_shape'))

    def get_control(self):
        import control.sphere
        return control.sphere.SphereControl

    def make_shape(self):
        return fab.shapes.sphere(self.x, self.y, self.z, self.r)

import fab.shapes

