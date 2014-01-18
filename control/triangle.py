from PySide import QtCore, QtGui

import base
import colors

class TriangleControl(base.NodeControl):

    @classmethod
    def new(cls, canvas, x, y, z, scale):
        """ Construct a new triangle at the given location.
            Location should be specified in units.
        """
        a = Point(get_name('a'), x - scale, y - scale)
        b = Point(get_name('b'), x, y + scale)
        c = Point(get_name('c'), x + scale, y - scale)
        tri = Triangle('t', a, b, c)

        for p in [a, b, c]:     PointControl(canvas, p)
        return cls(canvas, tri)


    def __init__(self, canvas, target):
        """ Construct the triangle control widget
        """
        self.point_nodes = [target.a, target.b, target.c]
        self.point_positions = [None for pt in self.point_nodes]

        super(TriangleControl, self).__init__(canvas, target)

        self.drag_control = base.DragManager(self, self.drag)
        self.editor_datums = ['name','shape']

        self.sync()
        self.make_mask()

        self.show()
        self.raise_()



    def raise_(self):
        """ Overload raise_ so that points stay above triangle lines.
        """
        super(TriangleControl, self).raise_()
        for p in self.point_nodes:   p.control.raise_()


    def make_mask(self):
        """ Make the triangular mask image.
        """
        self.drag_control.mask = self.paint_mask(self.draw_center)
        self.setMask(self.drag_control.mask |
                     self.paint_mask(self.draw_triangle))


    def drag(self, v, p):
        for pt in self.point_nodes:
            pt.control.drag_control.dragXY(v, None)


    def _sync(self):
        changed = any(pt.control.sync() for pt in self.point_nodes)
        positions = [pt.control.position for pt in self.point_nodes]
        changed |= (positions != self.point_positions)
        return changed

    def reposition(self):
        # Get bounding box from painter path
        rect = self.triangle_path().boundingRect().toRect()
        rect.setTop(rect.top() - 5)
        rect.setBottom(rect.bottom() + 5)
        rect.setLeft(rect.left() - 5)
        rect.setRight(rect.right() + 5)

        self.setGeometry(rect)
        self.make_mask()
        self.update()


    def paintEvent(self, paintEvent):
        painter = QtGui.QPainter(self)
        self.draw(painter)


    def triangle_path(self, offset=QtCore.QPoint()):
        """ Returns a QPainterPath that draws the triangle.
        """
        coords = [QtGui.QVector3D(pt.control.position)
                  for pt in self.point_nodes]
        coords.append(coords[0])
        return self.draw_lines([coords], offset)


    def draw_triangle(self, painter, mask=False):
        """ Draws the triangle on the given painter.
        """
        self.set_pen(painter, mask, None, colors.blue)
        painter.drawPath(self.triangle_path(self.pos()))


    def draw_center(self, painter, mask=False):
        position = QtCore.QPointF(
                sum(pt.control.position.x() for pt in self.point_nodes) / 3.0,
                sum(pt.control.position.y() for pt in self.point_nodes) / 3.0)
        pos = self.canvas.unit_to_pixel(position) - self.pos()
        x, y = pos.x(), pos.y()

        self.set_brush(painter, mask, colors.blue)

        if mask:                                                d = 22
        elif self.drag_control.hover or self.drag_control.drag: d = 20
        else:                                                   d = 16

        painter.drawEllipse(x - d/2, y - d/2, d, d)
        if mask:    return

        lines = [QtCore.QLine(x-4, y+2, x, y-4),
                 QtCore.QLine(x, y-4, x+4, y+2),
                 QtCore.QLine(x+4, y+2, x-4, y+2)]
        painter.drawLines(lines)

    def editor_position(self):
        """ Returns a QPoint defining the position at which we should
            open up the editor.
        """
        return self.canvas.unit_to_pixel(QtCore.QPointF(
                sum(pt.control.position.x() for pt in self.point_nodes) / 3.0,
                sum(pt.control.position.y() for pt in self.point_nodes) / 3.0))


    def draw(self, painter, mask=False):
        self.draw_triangle(painter, mask)
        self.draw_center(painter, mask)


from node.point import Point
from node.triangle import Triangle
from node.base import get_name

from point import PointControl
