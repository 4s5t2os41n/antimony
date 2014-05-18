import math

from PySide import QtCore, QtGui

from sb.controls.axes import AxesControl

class Canvas(QtGui.QGraphicsView):
    rotated = QtCore.Signal(QtGui.QMatrix4x4)

    def __init__(self, parent=None):
        self.scene = QtGui.QGraphicsScene(parent)
        super(Canvas, self).__init__(self.scene, parent)

        self.setStyleSheet('QGraphicsView { border-style: none; }')
        self.setBackgroundBrush(QtCore.Qt.black)

        self.setSceneRect(-self.width()/2, -self.height()/2,
                           self.width(), self.height())

        self._yaw = 0
        self._pitch = 0

    @property
    def matrix(self):
        """ Rotation transform matrix
        """
        M = QtGui.QMatrix4x4()
        M.rotate(math.degrees(self._pitch), QtGui.QVector3D(1, 0, 0))
        M.rotate(math.degrees(self._yaw), QtGui.QVector3D(0, 0, 1))
        return M


    def wheelEvent(self, event):
        """ On mouse scroll, zoom about the cursor's position.
        """
        p = self.mapToScene(event.pos())
        r = self.sceneRect()
        s = 1.001 ** -event.delta()
        self.setSceneRect(p.x() + (r.x() - p.x()) / s,
                          p.y() + (r.y() - p.y()) / s,
                          r.width() / s, r.height() / s)
        self.scale(s, s)

    def mousePressEvent(self, event):
        """ On mouse click, save the click position.
                (in scene coordinates for left-click,
                 pixel coordinates for right-click)
        """
        super(Canvas, self).mousePressEvent(event)
        if event.buttons() == QtCore.Qt.LeftButton:
            self._mouse_click_pos = self.mapToScene(event.pos())
        else:
            self._mouse_click_pos = event.pos()

    def mouseMoveEvent(self, event):
        """ On mouse move, pan or rotate.
        """
        if event.buttons() == QtCore.Qt.LeftButton:
            p = self.mapToScene(event.pos())
            r = self.sceneRect()
            r.translate(self._mouse_click_pos - p)
            self.setSceneRect(r)
        elif event.buttons() == QtCore.Qt.RightButton:
            p = event.pos()
            dp = 0.01 * (self._mouse_click_pos.x() - p.x())
            dy = 0.01 * (self._mouse_click_pos.y() - p.y())
            self._pitch = min(0, max(-math.pi, self._pitch + dp))
            self._yaw = ((self._yaw + dy + math.pi) % (2 * math.pi)) - math.pi
            self._mouse_click_pos = p
            self.rotated.emit(self.matrix)

