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

#include <osg/Notify>
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

/// Specialization for double - promotes float to double
template<> struct VOGet<double>: public VOGetBase<double>
{
    virtual void apply(const double v) { value=v; ok=true; }
    virtual void apply(const float v) { value=v; ok=true; }
};

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

/// Matrix xformScale(Vec3 how_much): make a scaling matrix.
struct XformScale: public osgDB::MethodObject {
    virtual bool run(osg::Object* , osg::Parameters& ins, osg::Parameters& outs) const
    {
        if (ins.size()!=1) return false;

        // Unmarshal
        osg::Vec3d how_much;
        if(!extract(ins[0].get(), how_much)) return false;

        // Do the work
        osg::Matrix retval;
        retval.makeScale(how_much);

        output_value(retval, outs);
        return true;
    }
}; //XformScale

/// Matrix xformTranslate(Vec3 by_how_much): make a translation matrix.
struct XformTranslate: public osgDB::MethodObject {
    virtual bool run(osg::Object* , osg::Parameters& ins, osg::Parameters& outs) const
    {
        if (ins.size()!=1) return false;
        osg::Vec3d how_much;
        if(!extract(ins[0].get(), how_much)) return false;
        osg::Matrix retval;
        retval.makeTranslate(how_much);
        output_value(retval, outs);
        return true;
    }
}; //XformTranslate

/// Matrix xformRotate(...): make a rotation matrix.
/// Arg formats:
///     xformRotate(Vec3 from, Vec3 to)
///     xformRotate(double angle, Vec3 axis)
struct XformRotate: public osgDB::MethodObject {
    virtual bool run(osg::Object* , osg::Parameters& ins, osg::Parameters& outs) const
    {
        if (ins.size()!=2) return false;
        osg::Vec3d from, to, axis;
        double angle;
        osg::Matrix retval;

        if(extract(ins[0].get(), from)) {   //(Vec3 from, Vec3 to)
            if(!extract(ins[1].get(), to)) return false;
            retval.makeRotate(from, to);

        } else if(extract(ins[0].get(), angle)) {   //(double angle, Vec3 axis)
            if(!extract(ins[1].get(), axis)) return false;
            retval.makeRotate(angle, axis);

        } else {    // Invalid args
            OSG_NOTICE << "invalid args to XformRotate" << std::endl;
            return false;
        }

        output_value(retval, outs);
        return true;
    }
}; //XformRotate

////////////////////////////////////////////////////////////////////////////
// Quaternion method implementations

/// Quat quatRotate(...): make a rotation quaternion.
/// Arg formats:
///     quatRotate(double angle, Vec3 axis)
///     quatRotate(double angle1, Vec3 axis1, double angle2, Vec3 axis2,
///                 double angle3, Vec3 axis3)
struct QuatRotate: public osgDB::MethodObject {
    virtual bool run(osg::Object* , osg::Parameters& ins, osg::Parameters& outs) const
    {
        if(ins.size()==2) {
            double angle;
            osg::Vec3d axis;
            if(extract(ins[0].get(), angle) && extract(ins[1].get(), axis)) {
                osg::Quat q(angle, axis);
                output_value(q, outs);
                return true;
            }
            OSG_NOTICE << "quatRotate: invalid parameters (need float angle, vec3 axis)" << std::endl;
            return false;   // invalid parameters

        } else if(ins.size()==6) {
            double angles[3];
            osg::Vec3d axes[3];
            int argidx = -1;

            for(int i=0; i<2; ++i) {
                if(!extract(ins[++argidx].get(), angles[i])) {
                    OSG_NOTICE << "quatRotate: invalid parameter " << argidx << " (need double)" << std::endl;
                    return false;
                }
                if(!extract(ins[++argidx].get(), axes[i])) {
                    OSG_NOTICE << "quatRotate: invalid parameter " << argidx << " (need vec3)" << std::endl;
                    return false;
                }
            }

            osg::Quat q(angles[0], axes[0],
                        angles[1], axes[1],
                        angles[2], axes[2]);
            output_value(q, outs);
            return true;

        } else {
            OSG_NOTICE << "quatRotate: invalid parameter count" << std::endl;
            return false;   // invalid parameter count
        }

    }
}; //QuatRotate

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
    ADD_METHOD_OBJECT( "xformTranslate", XformTranslate );
    ADD_METHOD_OBJECT( "xformRotate", XformRotate );

    // - quat: Quaternion functions -
    ADD_METHOD_OBJECT( "quatRotate", QuatRotate );
}
