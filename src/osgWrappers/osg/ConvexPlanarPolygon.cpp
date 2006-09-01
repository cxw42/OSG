// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/ConvexPlanarPolygon>
#include <osg/Vec3>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(std::vector< osg::Vec3 >, osg::ConvexPlanarPolygon::VertexList);

BEGIN_VALUE_REFLECTOR(osg::ConvexPlanarPolygon)
	I_Constructor0();
	I_Method1(void, add, IN, const osg::Vec3 &, v);
	I_Method1(void, setVertexList, IN, const osg::ConvexPlanarPolygon::VertexList &, vertexList);
	I_Method0(osg::ConvexPlanarPolygon::VertexList &, getVertexList);
	I_Method0(const osg::ConvexPlanarPolygon::VertexList &, getVertexList);
	I_Property(const osg::ConvexPlanarPolygon::VertexList &, VertexList);
END_REFLECTOR

