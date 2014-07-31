SOURCES += \
    ../src/datum/datum.cpp \
    ../src/datum/eval.cpp \
    ../src/node/node.cpp \
    ../src/node/manager.cpp \
    ../src/node/proxy.cpp \
    ../src/datum/input.cpp \
    ../src/datum/link.cpp \
    ../src/datum/wrapper.cpp \
    ../src/node/meta/script_node.cpp \
    ../src/datum/shape_datum.cpp \
    ../src/datum/float_datum.cpp \
    ../src/datum/name_datum.cpp \
    ../src/datum/script_datum.cpp \
    ../src/node/3d/point3d_node.cpp \
    ../src/node/3d/extrude_node.cpp \
    ../src/node/3d/sphere_node.cpp \
    ../src/node/3d/cylinder_node.cpp \
    ../src/node/csg/union_node.cpp \
    ../src/node/csg/difference_node.cpp \
    ../src/node/csg/intersection_node.cpp \
    ../src/datum/output_datum.cpp \
    ../src/datum/function_datum.cpp \
    ../src/datum/string_datum.cpp \
    ../src/node/2d/circle_node.cpp \
    ../src/node/2d/point2d_node.cpp \
    ../src/node/2d/text_node.cpp \
    ../src/node/2d/triangle_node.cpp \
    ../src/node/3d/cube_node.cpp \
    ../src/node/serializer.cpp \
    ../src/node/deserializer.cpp \

HEADERS += \
    ../src/datum/datum.h \
    ../src/datum/eval.h \
    ../src/node/node.h \
    ../src/node/manager.h \
    ../src/node/proxy.h \
    ../src/datum/input.h \
    ../src/datum/link.h \
    ../src/datum/wrapper.h \
    ../src/node/meta/script_node.h \
    ../src/datum/shape_datum.h \
    ../src/datum/float_datum.h \
    ../src/datum/name_datum.h \
    ../src/datum/string_datum.h \
    ../src/datum/script_datum.h \
    ../src/node/3d/point3d_node.h \
    ../src/node/3d/sphere_node.h \
    ../src/node/3d/cylinder_node.h \
    ../src/node/3d/extrude_node.h \
    ../src/node/csg/union_node.h \
    ../src/node/csg/difference_node.h \
    ../src/node/csg/intersection_node.h \
    ../src/datum/output_datum.h \
    ../src/datum/function_datum.h \
    ../src/node/2d/circle_node.h \
    ../src/node/2d/point2d_node.h \
    ../src/node/2d/text_node.h \
    ../src/node/2d/triangle_node.h \
    ../src/node/3d/cube_node.h \
    ../src/node/serializer.h \
    ../src/node/deserializer.h \


INCLUDEPATH += ../src

macx {
    QMAKE_CXXFLAGS += $$system(/usr/local/bin/python3-config --includes)
    QMAKE_LFLAGS   += $$system(/usr/local/bin/python3-config --ldflags)
    LIBS += -L/usr/local/lib -lboost_python3
    QMAKE_CXXFLAGS += -isystem/usr/local/include
}
