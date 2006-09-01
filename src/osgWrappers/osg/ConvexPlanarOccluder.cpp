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

#include <osg/ConvexPlanarOccluder>
#include <osg/ConvexPlanarPolygon>
#include <osg/CopyOp>
#include <osg/Object>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(std::vector< osg::ConvexPlanarPolygon >, osg::ConvexPlanarOccluder::HoleList);

BEGIN_OBJECT_REFLECTOR(osg::ConvexPlanarOccluder)
	I_BaseType(osg::Object);
	I_Constructor0();
	I_ConstructorWithDefaults2(IN, const osg::ConvexPlanarOccluder &, cpo, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY);
	I_Method0(osg::Object *, cloneType);
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop);
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj);
	I_Method0(const char *, libraryName);
	I_Method0(const char *, className);
	I_Method1(void, setOccluder, IN, const osg::ConvexPlanarPolygon &, cpp);
	I_Method0(osg::ConvexPlanarPolygon &, getOccluder);
	I_Method0(const osg::ConvexPlanarPolygon &, getOccluder);
	I_Method1(void, addHole, IN, const osg::ConvexPlanarPolygon &, cpp);
	I_Method1(void, setHoleList, IN, const osg::ConvexPlanarOccluder::HoleList &, holeList);
	I_Method0(osg::ConvexPlanarOccluder::HoleList &, getHoleList);
	I_Method0(const osg::ConvexPlanarOccluder::HoleList &, getHoleList);
	I_Property(const osg::ConvexPlanarOccluder::HoleList &, HoleList);
	I_Property(const osg::ConvexPlanarPolygon &, Occluder);
END_REFLECTOR

STD_VECTOR_REFLECTOR(std::vector< osg::ConvexPlanarPolygon >);

