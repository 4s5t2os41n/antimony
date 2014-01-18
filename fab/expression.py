import math

def wrapped(f):
    """ Function decorator that (for convenience) attempts to convert
        all arguments into Expression objects.
    """
    def wrapped(*args, **kwargs):
        return f(*[Expression(a) if not isinstance(a, Expression)
                                else a for a in args],
                **{k:Expression(a) if not isinstance(a, Expression)
                                else a for k, a in kwargs.items()})
    return wrapped

class Expression(object):
    """ Represents a math expression which may be parsed into a tree.
        May also keep track of expression bounds (if known).
    """

    def __init__(self, math):
        if math is None:
            self.math = 'f1.0'
        elif type(math) in [int, float]:
            self.math = 'f%s' % math
        elif type(math) is str:
            self.math = math
        else:
            print type(math)
            raise TypeError("Cannot construct a Expression from '%s'" % math)

        if self.math is None:
            self.xmin = self.ymin = self.zmin = -1
            self.xmax = self.ymax = self.zmax = 1
        else:
            self.xmin = self.ymin = self.zmin = float('-infinity')
            self.xmax = self.ymax = self.zmax = float('+infinity')

    def __eq__(self, other):
        return all(getattr(self, a) == getattr(other, a) for a in
                ['math','xmin','ymin','zmin','xmax','ymax','zmax'])

    def __repr__(self):
        return self.math

    def check(self):
        """ Returns True if this is a valid math string.
        """
        try:                    self.to_tree()
        except RuntimeError:    return False
        else:                   return True

    def to_tree(self):
        return tree.MathTree.from_expression(self)

    # Numerical addition
    @wrapped
    def __add__(self, rhs):
        return Expression('+' + self.math + rhs.math)
    @wrapped
    def __radd__(self, lhs):
        return Expression('+' + lhs.math + self.math)

    # Numerical subtraction
    @wrapped
    def __sub__(self, rhs):
        return Expression('-' + self.math + rhs.math)
    @wrapped
    def __rsub__(self, lhs):
        return Expression('-' + lhs.math + self.math)

    # Multiplication
    @wrapped
    def __mul__(self, rhs):
        return Expression('*' + self.math + rhs.math)
    @wrapped
    def __rmul__(self, lhs):
        return Expression('*' + lhs.math + self.math)

    # Division
    @wrapped
    def __div__(self, rhs):
        return Expression('/' + self.math + rhs.math)
    @wrapped
    def __rdiv__(self, lhs):
        return Expression('/' + lhs.math + self.math)

    # Negation
    def __neg__(self):
        return Expression('n' + self.math)
    def __invert__(self):
        return Expression('n' + self.math)

    # Logical intersection
    @wrapped
    def __and__(self, rhs):
        e = Expression('a' + self.math + rhs.math)
        e.xmin = max(self.xmin, rhs.xmin)
        e.xmax = min(self.xmax, rhs.xmax)
        e.ymin = max(self.ymin, rhs.ymin)
        e.ymax = min(self.ymax, rhs.ymax)
        e.zmin = max(self.zmin, rhs.zmin)
        e.zmax = min(self.zmax, rhs.zmax)
        return e
    @wrapped
    def __rand__(self, rhs):
        e = Expression('a' + lhs.math + self.math)
        e.xmin = max(lhs.xmin, self.xmin)
        e.xmax = min(lhs.xmax, self.xmax)
        e.ymin = max(lhs.ymin, self.ymin)
        e.ymax = min(lhs.ymax, self.ymax)
        e.zmin = max(lhs.zmin, self.zmin)
        e.zmax = min(lhs.zmax, self.zmax)
        return e

    # Logical union
    @wrapped
    def __or__(self, rhs):
        e = Expression('i' + self.math + rhs.math)
        e.xmin = min(self.xmin, rhs.xmin)
        e.xmax = max(self.xmax, rhs.xmax)
        e.ymin = min(self.ymin, rhs.ymin)
        e.ymax = max(self.ymax, rhs.ymax)
        e.zmin = min(self.zmin, rhs.zmin)
        e.zmax = max(self.zmax, rhs.zmax)
        return e
    @wrapped
    def __ror__(self, rhs):
        e = Expression('i' + lhs.math + self.math)
        e.xmin = min(lhs.xmin, self.xmin)
        e.xmax = max(lhs.xmax, self.xmax)
        e.ymin = min(lhs.ymin, self.ymin)
        e.ymax = max(lhs.ymax, self.ymax)
        e.zmin = min(lhs.zmin, self.zmin)
        e.zmax = max(lhs.zmax, self.zmax)
        return e

    @wrapped
    def map(self, X=None, Y=None, Z=None):
        """ Applies a map operator to an expression.
            Produces a new expression with unknown bounds.
        """
        return Expression(
                'm' +
                (X.math if X else ' ')+
                (Y.math if Y else ' ')+
                (Z.math if Z else ' ')+
                self.math)

    def transform(self, M, M_i):
        """ Applies a matrix transformation to our coordinates.
            M and M_inverse should be a matrix and its inverse which support
            M[0,0]-style indexing.
        """
        x, y, z = Expression('X'), Expression('Y'), Expression('Z')
        e = self.map(X=M[0,0]*x + M[0,1]*y + M[0,2]*z + M[0,3],
                     Y=M[1,0]*x + M[1,1]*y + M[1,2]*z + M[1,3],
                     Z=M[2,0]*x + M[2,1]*y + M[2,2]*z + M[2,3])
        e.set_bounds(*self.remap_bounds(
                X=M_i[0,0]*x + M_i[0,1]*y + M_i[0,2]*z + M_i[0,3],
                Y=M_i[1,0]*x + M_i[1,1]*y + M_i[1,2]*z + M_i[1,3],
                Z=M_i[2,0]*x + M_i[2,1]*y + M_i[2,2]*z + M_i[2,3]))
        return e

    def transformXY(self, M, M_i):
        """ Applies a matrix transformation to our coordinates,
            ignoring all z transformations.
            M and M_inverse should be a matrix and its inverse which support
            M[0,0]-style indexing.
        """
        x, y, z = Expression('X'), Expression('Y'), Expression('Z')
        e = self.map(X=M[0,0]*x + M[0,1]*y + M[0,3],
                     Y=M[1,0]*x + M[1,1]*y + M[1,3])
        e.set_bounds(*self.remap_bounds(
                X=M_i[0,0]*x + M_i[0,1]*y + M_i[0,3],
                Y=M_i[1,0]*x + M_i[1,1]*y + M_i[1,3]))
        return e

    @wrapped
    def remap_bounds(self, X=None, Y=None, Z=None):
        """ Remaps the bounds of this expression.

            X, Y, and Z should be the inverse of a coordinate mapping
            i.e. if our coordinate transform is x' = f(x), we want
            X to be f' such that x = f'(x')

            Returns xmin, xmax, ymin, ymax, zmin, zmax
        """
        x = interval.Interval(self.xmin, self.xmax)
        y = interval.Interval(self.ymin, self.ymax)
        z = interval.Interval(self.zmin, self.zmax)

        if X:   a = X.to_tree().eval_i(x, y, z)
        else:   a = x

        if Y:   b = Y.to_tree().eval_i(x, y, z)
        else:   b = y

        if Z:   c = Z.to_tree().eval_i(x, y, z)
        else:   c = z

        for i in [a, b, c]:
            if math.isnan(i.lower):  i.lower = float('-inf')
            if math.isnan(i.upper):  i.upper = float('+inf')

        return a.lower, a.upper, b.lower, b.upper, c.lower, c.upper

    def set_bounds(self, *args):
        """ Sets the bounds of this expression (this function should be
            used in conjunction with remap_bounds above).
        """
        (self.xmin, self.xmax,
         self.ymin, self.ymax,
         self.zmin, self.zmax) = args

    def has_xy_bounds(self):
        """ Returns True if this expression has valid XY bounds.
        """
        return (not math.isinf(self.xmax - self.xmin) and
                not math.isinf(self.ymax - self.ymin) and
                (self.xmax > self.xmin) and
                (self.ymax > self.ymin))

    def has_xyz_bounds(self):
        """ Returns True if this expression has valid XYZ bounds.
        """
        return (not math.isinf(self.xmax - self.xmin) and
                not math.isinf(self.ymax - self.ymin) and
                not math.isinf(self.zmax - self.zmin) and
                (self.xmax > self.xmin) and
                (self.ymax > self.ymin) and
                (self.zmax > self.zmin))

    def get_xy_region(self, voxels_per_unit):
        """ Returns a region appropriate for 2D or height-map rendering.
        """
        zmin = 0 if math.isinf(self.zmin) else self.zmin
        zmax = 0 if math.isinf(self.zmax) else self.zmax

        return region.Region((self.xmin, self.ymin, zmin),
                             (self.xmax, self.ymax, zmax), voxels_per_unit)


    def get_xyz_region(self, voxels_per_unit):
        """ Returns a region appropriate for 3D rendering.
        """
        return region.Region((self.xmin, self.ymin, self.zmin),
                             (self.xmax, self.ymax, self.zmax), voxels_per_unit)


import tree
import region
import interval
