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

#include <osg/Vec4ub>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

#include <osg/io_utils>
	
TYPE_NAME_ALIAS(unsigned char, osg::Vec4ub::value_type);

BEGIN_VALUE_REFLECTOR(osg::Vec4ub)
	I_ReaderWriter(osgIntrospection::StdReaderWriter<reflected_type>);	// user-defined
	I_Comparator(osgIntrospection::PartialOrderComparator<reflected_type>);	// user-defined
	I_Constructor0();
	I_Constructor4(IN, osg::Vec4ub::value_type, x, IN, osg::Vec4ub::value_type, y, IN, osg::Vec4ub::value_type, z, IN, osg::Vec4ub::value_type, w);
	I_Method0(unsigned char *, ptr);
	I_Method0(const unsigned char *, ptr);
	I_Method4(void, set, IN, unsigned char, r, IN, unsigned char, g, IN, unsigned char, b, IN, unsigned char, a);
	I_Method0(unsigned char &, r);
	I_Method0(unsigned char &, g);
	I_Method0(unsigned char &, b);
	I_Method0(unsigned char &, a);
	I_Method0(unsigned char, r);
	I_Method0(unsigned char, g);
	I_Method0(unsigned char, b);
	I_Method0(unsigned char, a);
END_REFLECTOR

