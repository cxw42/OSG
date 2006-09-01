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

#include <osg/CopyOp>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osg/Vec3>
#include <osgSim/ScalarBar>
#include <osgSim/ScalarsToColors>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ENUM_REFLECTOR(osgSim::ScalarBar::Orientation)
	I_EnumLabel(osgSim::ScalarBar::HORIZONTAL);
	I_EnumLabel(osgSim::ScalarBar::VERTICAL);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgSim::ScalarBar)
	I_BaseType(osg::Geode);
	I_Constructor0();
	I_ConstructorWithDefaults7(IN, int, numColors, , IN, int, numLabels, , IN, osgSim::ScalarsToColors *, stc, , IN, const std::string &, title, , IN, osgSim::ScalarBar::Orientation, orientation, osgSim::ScalarBar::HORIZONTAL, IN, float, aspectRatio, 0.25, IN, osgSim::ScalarBar::ScalarPrinter *, sp, new osgSim::ScalarBar::ScalarPrinter);
	I_Constructor2(IN, const osgSim::ScalarBar &, rhs, IN, const osg::CopyOp &, co);
	I_Method0(osg::Object *, cloneType);
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop);
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj);
	I_Method0(const char *, className);
	I_Method0(const char *, libraryName);
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv);
	I_Method1(void, setNumColors, IN, int, numColors);
	I_Method0(int, getNumColors);
	I_Method1(void, setNumLabels, IN, int, numLabels);
	I_Method0(int, getNumLabels);
	I_Method1(void, setScalarsToColors, IN, osgSim::ScalarsToColors *, stc);
	I_Method0(const osgSim::ScalarsToColors *, getScalarsToColors);
	I_Method1(void, setTitle, IN, const std::string &, title);
	I_Method0(std::string, getTitle);
	I_Method1(void, setPosition, IN, const osg::Vec3 &, pos);
	I_Method0(const osg::Vec3 &, getPosition);
	I_Method1(void, setWidth, IN, float, width);
	I_Method0(float, getWidth);
	I_Method1(void, setAspectRatio, IN, float, aspectRatio);
	I_Method0(float, getAspectRatio);
	I_Method1(void, setOrientation, IN, osgSim::ScalarBar::Orientation, orientation);
	I_Method0(osgSim::ScalarBar::Orientation, getOrientation);
	I_Method1(void, setScalarPrinter, IN, osgSim::ScalarBar::ScalarPrinter *, sp);
	I_Method0(const osgSim::ScalarBar::ScalarPrinter *, getScalarPrinter);
	I_Method1(void, setTextProperties, IN, const osgSim::ScalarBar::TextProperties &, tp);
	I_Method0(const osgSim::ScalarBar::TextProperties &, getTextProperties);
	I_Method0(void, update);
	I_Property(float, AspectRatio);
	I_WriteOnlyProperty(int, NumColors);
	I_WriteOnlyProperty(int, NumLabels);
	I_Property(osgSim::ScalarBar::Orientation, Orientation);
	I_Property(const osg::Vec3 &, Position);
	I_WriteOnlyProperty(osgSim::ScalarBar::ScalarPrinter *, ScalarPrinter);
	I_WriteOnlyProperty(osgSim::ScalarsToColors *, ScalarsToColors);
	I_Property(const osgSim::ScalarBar::TextProperties &, TextProperties);
	I_ReadOnlyProperty(std::string, Title);
	I_Property(float, Width);
END_REFLECTOR

BEGIN_OBJECT_REFLECTOR(osgSim::ScalarBar::ScalarPrinter)
	I_BaseType(osg::Referenced);
	I_Constructor0();
	I_Method1(std::string, printScalar, IN, float, scalar);
END_REFLECTOR

BEGIN_VALUE_REFLECTOR(osgSim::ScalarBar::TextProperties)
	I_Constructor0();
	I_PublicMemberProperty(std::string, _fontFile);
	I_PublicMemberProperty(std::pair< int COMMA  int >, _fontResolution);
	I_PublicMemberProperty(float, _characterSize);
	I_PublicMemberProperty(osg::Vec4, _color);
END_REFLECTOR

