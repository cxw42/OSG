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

#include <osg/GraphicsContext>
#include <osgProducer/GraphicsContextImplementation>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgProducer::GraphicsContextImplementation)
	I_BaseType(osg::GraphicsContext);
	I_Constructor1(IN, osg::GraphicsContext::Traits *, traits);
	I_Constructor1(IN, Producer::RenderSurface *, rs);
	I_Method0(Producer::RenderSurface *, getRenderSurface);
	I_Method0(const Producer::RenderSurface *, getRenderSurface);
	I_Method0(bool, realizeImplementation);
	I_Method0(bool, isRealizedImplementation);
	I_Method0(void, closeImplementation);
	I_Method0(void, makeCurrentImplementation);
	I_Method1(void, makeContextCurrentImplementation, IN, osg::GraphicsContext *, readContext);
	I_Method1(void, bindPBufferToTextureImplementation, IN, GLenum, buffer);
	I_Method0(void, swapBuffersImplementation);
	I_ReadOnlyProperty(Producer::RenderSurface *, RenderSurface);
END_REFLECTOR

