import base
import datum

class Text(base.Node):
    def __init__(self, name, x, y, scale, txt):
        super(Text, self).__init__(name)
        self.add_datum('x', datum.FloatDatum(self, x))
        self.add_datum('y', datum.FloatDatum(self, y))

        self.add_datum('scale', datum.FloatDatum(self, scale))
        self.add_datum('txt', datum.StringDatum(self, txt))

        self.add_datum('shape',
                datum.ExpressionFunctionDatum(self, 'make_shape'))

    def get_control(self):
        import control.text
        return control.text.TextControl

    def make_shape(self):
        return fab.shapes.text(self.txt, self.x, self.y, self.scale)

import fab.shapes
