// TODO make sure this works for static linking - see
// http://forum.openscenegraph.org/viewtopic.php?t=14841
#include <osgDB/Registry>

USE_EXTEND_OBJECT_WRAPPER(scripting_osg_Billboard)
USE_EXTEND_OBJECT_WRAPPER(scripting_osg_Camera)
USE_EXTEND_OBJECT_WRAPPER(scripting_osg_Drawable)
USE_EXTEND_OBJECT_WRAPPER(scripting_osg_Node)

DECLARE_SERIALIZER_WRAPPER_LIBRARY(scripting_osg)

