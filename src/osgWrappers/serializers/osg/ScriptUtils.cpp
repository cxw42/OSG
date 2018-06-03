/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2017 Robert Osfield
 * This file Copyright (C) 2017 Chris White
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

/// osg::ScriptUtils provides access to C routines for matrix manipulation
/// and other useful functions.  It is not expected to be serialized.
/// This file implements those functions so that scripts can access them.

#include <osg/Matrix>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/ScriptUtils>
#include <osg/ValueObject>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

namespace {     // avoid namespace pollution from exported symbols

////////////////////////////////////////////////////////////////////////////
// Helper routines

// === Input parameter unmarshaling ===

/// Common fields for a templated extractor.
template<class ResultT>
struct VOGetBase: public osg::ValueObject::GetValueVisitor
{
    ResultT value;
    bool ok;
    VOGetBase(): ok(false) {}
};

/// Empty template base, so we can specialize it with the apply() functions
/// we need.
template<class ResultT>
struct VOGet: public VOGetBase<ResultT> {};

/// Specialization for Vec3d - promotes Vec3f to Vec3d
template<> struct VOGet<osg::Vec3d>: public VOGetBase<osg::Vec3d>
{
    virtual void apply(const osg::Vec3d& v) { value=v; ok=true; }
    virtual void apply(const osg::Vec3f& v) { value=v; ok=true; }
};

/// Templated extractor function
template<class ResultT>
bool extract(osg::Object* obj, ResultT& retval)
{
    osg::ValueObject *value_obj = dynamic_cast<osg::ValueObject*>(obj);
    if(!value_obj) return false;

    VOGet<ResultT> extractor;
    value_obj->get(extractor);
    if(!extractor.ok) return false;

    retval = extractor.value;
    return true;
}

// === Output parameter marshaling ===

// Append a value to an output-parameter array
template<class T>
void output_value(T& retval, osg::Parameters& outs)
{
    typedef osg::TemplateValueObject<T> VO;
    osg::ref_ptr<VO> retval_object(new VO(retval));
    outs.push_back(retval_object);
}

////////////////////////////////////////////////////////////////////////////
// View-matrix method implementations

/// Matrix viewLookAt(Vec3 eye, Vec3 center, Vec3 up): make a view matrix
/// a la gluLookAt().
struct ViewLookAt: public osgDB::MethodObject {
    virtual bool run(osg::Object* , osg::Parameters& ins, osg::Parameters& outs) const
    {
        if (ins.size()!=3) return false;

        // Unmarshal the Vec3ds
        osg::Vec3d eye, center, up;
            // take Vec3d even for Matrixf because VOGet<Vec3d> can promote
            // Vec3f values.

        if(!extract(ins[0].get(), eye)) return false;
        if(!extract(ins[1].get(), center)) return false;
        if(!extract(ins[2].get(), up)) return false;

        // Do the work
        osg::Matrix retval;
        retval.makeLookAt(eye, center, up);

        // Marshal the resulting view matrix
        output_value(retval, outs);
        return true;
    }
};

////////////////////////////////////////////////////////////////////////////
// Transformation-matrix method implementations

/// Matrix scale(Vec3 how_much): make a scaling matrix
struct XformScale: public osgDB::MethodObject {
    virtual bool run(osg::Object* , osg::Parameters& ins, osg::Parameters& outs) const
    {
        if (ins.size()!=1) return false;

        // Unmarshal
        osg::Vec3d how_much;
        if(!extract(ins[0].get(), how_much)) return false;

        // Do the work
        osg::Matrix retval;
        retval.scale(how_much);

        output_value(retval, outs);
        return true;
    }
};

} //end anon namespace

/// A ScriptUtils instance maintains no state visible to calling scripts.
/// All results are returned as output parameters.
/// For the benefit of scripting languages that do not support multiple
/// output parameters, each method returns at most one value.
REGISTER_OBJECT_WRAPPER( ScriptUtils,
                         new osg::ScriptUtils,
                         osg::ScriptUtils,
                         "osg::Object osg::ScriptUtils" )
{
    // - mat: General matrix functions -

    // - proj: Projection-matrix functions -

    // - view: View-matrix functions -
    ADD_METHOD_OBJECT( "viewLookAt", ViewLookAt );

    // - xform: Transformation-matrix functions -
    ADD_METHOD_OBJECT( "xformScale", XformScale );

}
