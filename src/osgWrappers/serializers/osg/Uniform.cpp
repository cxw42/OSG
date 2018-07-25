#include <osg/Uniform>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

static bool checkElements( const osg::Uniform& uniform )
{
    return uniform.getNumElements()>0;
}

static bool readElements( osgDB::InputStream& is, osg::Uniform& uniform )
{
    bool hasArray; is >> hasArray;
    if ( hasArray )
    {
        osg::ref_ptr<osg::Array> array = is.readArray();
        switch ( array->getType() )
        {
        case osg::Array::FloatArrayType:
            uniform.setArray( static_cast<osg::FloatArray*>(array.get()) ); break;
        case osg::Array::DoubleArrayType:
            uniform.setArray( static_cast<osg::DoubleArray*>(array.get()) ); break;
        case osg::Array::IntArrayType:
            uniform.setArray( static_cast<osg::IntArray*>(array.get()) ); break;
        case osg::Array::UIntArrayType:
            uniform.setArray( static_cast<osg::UIntArray*>(array.get()) ); break;
        case osg::Array::Int64ArrayType:
            uniform.setArray( static_cast<osg::Int64Array*>(array.get()) ); break;
        case osg::Array::UInt64ArrayType:
            uniform.setArray( static_cast<osg::UInt64Array*>(array.get()) ); break;
        default: break;
        }
    }
    return true;
}

static bool writeElements( osgDB::OutputStream& os, const osg::Uniform& uniform )
{
    if ( uniform.getFloatArray()!=NULL )
    {
        os << (uniform.getFloatArray()!=NULL);
        os.writeArray( uniform.getFloatArray() );
    }
    else if ( uniform.getDoubleArray()!=NULL )
    {
        os << (uniform.getDoubleArray()!=NULL);
        os.writeArray( uniform.getDoubleArray() );
    }
    else if ( uniform.getIntArray()!=NULL )
    {
        os << (uniform.getIntArray()!=NULL);
        os.writeArray( uniform.getIntArray() );
    }
    else if ( uniform.getUIntArray()!= NULL)
    {
        os << (uniform.getUIntArray()!=NULL);
        os.writeArray( uniform.getUIntArray() );
    }
    else if ( uniform.getInt64Array()!= NULL)
    {
        os << (uniform.getInt64Array()!=NULL);
        os.writeArray( uniform.getInt64Array() );
    }
    else // ( uniform.getUInt64Array()!= NULL)
    {
        os << (uniform.getUInt64Array()!=NULL);
        os.writeArray( uniform.getUInt64Array() );
    }
    return true;
}

/// Make the uniform's value accessible to scripts.
/// TODO use a PropByValSerializer or some such instead.
struct UniformValueMethod : public osgDB::MethodObject
{
    /// - Called as value(new_value_array), set the uniform's value to
    ///   new_value_array.  Even for single values, wrap them in arrays.
    ///   Alternatively, if called with a ValueObject holding a double, Vec2d,
    ///   Vec3d, Vec4d, or Matrixd, will attempt to set it to that value.
    /// - Called as value(), return the current value.
    /// \note Make sure to Type and NumElements are set before using either form.
    virtual bool run(   osg::Object* objectPtr
                      , osg::Parameters& inputParameters
                      , osg::Parameters& outputParameters) const
    {
        osg::Uniform *uniform = dynamic_cast<osg::Uniform*>(objectPtr);
        if(!uniform) return false;

        // Sanity checks
        if( uniform->getType() == osg::Uniform::UNDEFINED ) {
            OSG_NOTICE << "Can't call uniform:value() before setting Type" << std::endl;
            return false;
        }

        if( uniform->getNumElements() <= 0 ) {
            OSG_NOTICE << "Can't call uniform:value() before setting NumElements" << std::endl;
            return false;
        }

        if(inputParameters.size()<1) {          // getter
            if(uniform->getArray()) {
                outputParameters.push_back(uniform->getArray());
            // TODO create a TemplateValueObject based on the type
            } else {
                OSG_NOTICE << "Uniform:value(): Attempt to get nonexistent array" << std::endl;
                return false;
            }
        }
        else                                    // setter
        {
            osg::Object *newval = inputParameters[0].get();
            if(!newval) {
                OSG_NOTICE << "Cannot set uniform from provided NULL object" << std::endl;
                return false;
            }

            if( osg::Array* arr = dynamic_cast<osg::Array*>(newval) ) {
                if(!uniform->setArray(arr)) {
                    OSG_NOTICE << "Uniform:value(): Cannot set uniform to new array" << std::endl;
                    return false;
                }

            } else if( osg::ValueObject *vo = dynamic_cast<osg::ValueObject*>(newval) ) {
                // First, try some hard-coded alternatives to handle double->float.
                // This is driven by LuaScriptEngine, which provides only doubles,
                // not floats.
                // TODO handle this a much better way.
                osg::DoubleValueObject* dvo = dynamic_cast<osg::DoubleValueObject*>(newval);
                osg::Vec2dValueObject* v2dvo = dynamic_cast<osg::Vec2dValueObject*>(newval);
                osg::Vec3dValueObject* v3dvo = dynamic_cast<osg::Vec3dValueObject*>(newval);
                osg::Vec4dValueObject* v4dvo = dynamic_cast<osg::Vec4dValueObject*>(newval);

                if(dvo && uniform->getType() == osg::Uniform::FLOAT) {  // double->float
                    if(!uniform->set((float)(dvo->getValue()))) {
                        OSG_NOTICE << "Uniform:value(): Could not set float from double" << std::endl;
                        return false;
                    }
                } else
                if(v2dvo && uniform->getType() == osg::Uniform::FLOAT_VEC2) {  // dvec2 -> vec2
                    osg::Vec2d vec(v2dvo->getValue());    // no ctor creates Vec2f from Vec2d, so work component-wise
                    if(!uniform->set(osg::Vec2f(vec.x(), vec.y()))) {
                        OSG_NOTICE << "Uniform:value(): Could not set float vec2 from double vec2" << std::endl;
                        return false;
                    }
                } else
                if(v3dvo && uniform->getType() == osg::Uniform::FLOAT_VEC3) {  // dvec3 -> vec3
                    osg::Vec3d vec(v3dvo->getValue());    // no ctor creates Vec3f from Vec3d, so work component-wise
                    if(!uniform->set(osg::Vec3f(vec.x(), vec.y(), vec.z()))) {
                        OSG_NOTICE << "Uniform:value(): Could not set float vec3 from double vec3" << std::endl;
                        return false;
                    }
                } else
                if(v4dvo && uniform->getType() == osg::Uniform::FLOAT_VEC4) {  // dvec4 -> vec4
                    osg::Vec4d vec(v4dvo->getValue());    // no ctor creates Vec4f from Vec4d, so work component-wise
                    if(!uniform->set(osg::Vec4f(vec.x(), vec.y(), vec.z(), vec.w()))) {
                        OSG_NOTICE << "Uniform:value(): Could not set float vec4 from double vec4" << std::endl;
                        return false;
                    }
                } else
                {
                    osg::UniformFromValueObjectVisitor visitor(uniform);
                    vo->get(visitor);
                    if(!visitor.wasOk()) {
                        OSG_NOTICE << "Uniform:value(): Could not extract value from "
                            << newval->getCompoundClassName() << ".  Non-scalar?"
                            << std::endl;
                        return false;
                    }
                }
            } else {
                OSG_NOTICE << "Uniform:value(): Cannot set uniform from provided "
                        << newval->getCompoundClassName() << std::endl;
                return false;
            }

        }

        return true;
    }
};

REGISTER_OBJECT_WRAPPER( Uniform,
                         new osg::Uniform,
                         osg::Uniform,
                         "osg::Object osg::Uniform" )
{
    BEGIN_ENUM_SERIALIZER3( Type, UNDEFINED );
        ADD_ENUM_VALUE( FLOAT );
        ADD_ENUM_VALUE( FLOAT_VEC2 );
        ADD_ENUM_VALUE( FLOAT_VEC3 );
        ADD_ENUM_VALUE( FLOAT_VEC4 );

        ADD_ENUM_VALUE( DOUBLE );
        ADD_ENUM_VALUE( DOUBLE_VEC2 );
        ADD_ENUM_VALUE( DOUBLE_VEC3 );
        ADD_ENUM_VALUE( DOUBLE_VEC4 );

        ADD_ENUM_VALUE( INT );
        ADD_ENUM_VALUE( INT_VEC2 );
        ADD_ENUM_VALUE( INT_VEC3 );
        ADD_ENUM_VALUE( INT_VEC4 );

        ADD_ENUM_VALUE( UNSIGNED_INT );
        ADD_ENUM_VALUE( UNSIGNED_INT_VEC2 );
        ADD_ENUM_VALUE( UNSIGNED_INT_VEC3 );
        ADD_ENUM_VALUE( UNSIGNED_INT_VEC4 );

        ADD_ENUM_VALUE( BOOL );
        ADD_ENUM_VALUE( BOOL_VEC2 );
        ADD_ENUM_VALUE( BOOL_VEC3 );
        ADD_ENUM_VALUE( BOOL_VEC4 );

        ADD_ENUM_VALUE( FLOAT_MAT2 );
        ADD_ENUM_VALUE( FLOAT_MAT3 );
        ADD_ENUM_VALUE( FLOAT_MAT4 );
        ADD_ENUM_VALUE( FLOAT_MAT2x3 );
        ADD_ENUM_VALUE( FLOAT_MAT2x4 );
        ADD_ENUM_VALUE( FLOAT_MAT3x2 );
        ADD_ENUM_VALUE( FLOAT_MAT3x4 );
        ADD_ENUM_VALUE( FLOAT_MAT4x2 );
        ADD_ENUM_VALUE( FLOAT_MAT4x3 );

        ADD_ENUM_VALUE( DOUBLE_MAT2 );
        ADD_ENUM_VALUE( DOUBLE_MAT3 );
        ADD_ENUM_VALUE( DOUBLE_MAT4 );
        ADD_ENUM_VALUE( DOUBLE_MAT2x3 );
        ADD_ENUM_VALUE( DOUBLE_MAT2x4 );
        ADD_ENUM_VALUE( DOUBLE_MAT3x2 );
        ADD_ENUM_VALUE( DOUBLE_MAT3x4 );
        ADD_ENUM_VALUE( DOUBLE_MAT4x2 );
        ADD_ENUM_VALUE( DOUBLE_MAT4x3 );

        ADD_ENUM_VALUE( SAMPLER_1D );
        ADD_ENUM_VALUE( SAMPLER_2D );
        ADD_ENUM_VALUE( SAMPLER_3D );
        ADD_ENUM_VALUE( SAMPLER_CUBE );
        ADD_ENUM_VALUE( SAMPLER_1D_SHADOW );
        ADD_ENUM_VALUE( SAMPLER_2D_SHADOW );
        ADD_ENUM_VALUE( SAMPLER_1D_ARRAY );
        ADD_ENUM_VALUE( SAMPLER_2D_ARRAY );
        ADD_ENUM_VALUE( SAMPLER_CUBE_MAP_ARRAY );
        ADD_ENUM_VALUE( SAMPLER_1D_ARRAY_SHADOW );
        ADD_ENUM_VALUE( SAMPLER_2D_ARRAY_SHADOW );
        ADD_ENUM_VALUE( SAMPLER_2D_MULTISAMPLE );
        ADD_ENUM_VALUE( SAMPLER_2D_MULTISAMPLE_ARRAY );
        ADD_ENUM_VALUE( SAMPLER_CUBE_SHADOW );
        ADD_ENUM_VALUE( SAMPLER_CUBE_MAP_ARRAY_SHADOW );
        ADD_ENUM_VALUE( SAMPLER_BUFFER );
        ADD_ENUM_VALUE( SAMPLER_2D_RECT );
        ADD_ENUM_VALUE( SAMPLER_2D_RECT_SHADOW );

        ADD_ENUM_VALUE( INT_SAMPLER_1D );
        ADD_ENUM_VALUE( INT_SAMPLER_2D );
        ADD_ENUM_VALUE( INT_SAMPLER_3D );
        ADD_ENUM_VALUE( INT_SAMPLER_CUBE );
        ADD_ENUM_VALUE( INT_SAMPLER_1D_ARRAY );
        ADD_ENUM_VALUE( INT_SAMPLER_2D_ARRAY );
        ADD_ENUM_VALUE( INT_SAMPLER_CUBE_MAP_ARRAY );
        ADD_ENUM_VALUE( INT_SAMPLER_2D_MULTISAMPLE );
        ADD_ENUM_VALUE( INT_SAMPLER_2D_MULTISAMPLE_ARRAY );
        ADD_ENUM_VALUE( INT_SAMPLER_BUFFER );
        ADD_ENUM_VALUE( INT_SAMPLER_2D_RECT );

        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_1D );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_2D );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_3D );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_CUBE );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_1D_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_2D_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_BUFFER );
        ADD_ENUM_VALUE( UNSIGNED_INT_SAMPLER_2D_RECT );

        ADD_ENUM_VALUE( IMAGE_1D );
        ADD_ENUM_VALUE( IMAGE_2D );
        ADD_ENUM_VALUE( IMAGE_3D );
        ADD_ENUM_VALUE( IMAGE_2D_RECT );
        ADD_ENUM_VALUE( IMAGE_CUBE );
        ADD_ENUM_VALUE( IMAGE_BUFFER );
        ADD_ENUM_VALUE( IMAGE_1D_ARRAY );
        ADD_ENUM_VALUE( IMAGE_2D_ARRAY );
        ADD_ENUM_VALUE( IMAGE_CUBE_MAP_ARRAY );
        ADD_ENUM_VALUE( IMAGE_2D_MULTISAMPLE );
        ADD_ENUM_VALUE( IMAGE_2D_MULTISAMPLE_ARRAY );

        ADD_ENUM_VALUE( INT_IMAGE_1D );
        ADD_ENUM_VALUE( INT_IMAGE_2D );
        ADD_ENUM_VALUE( INT_IMAGE_3D );
        ADD_ENUM_VALUE( INT_IMAGE_2D_RECT );
        ADD_ENUM_VALUE( INT_IMAGE_CUBE );
        ADD_ENUM_VALUE( INT_IMAGE_BUFFER );
        ADD_ENUM_VALUE( INT_IMAGE_1D_ARRAY );
        ADD_ENUM_VALUE( INT_IMAGE_2D_ARRAY );
        ADD_ENUM_VALUE( INT_IMAGE_CUBE_MAP_ARRAY );
        ADD_ENUM_VALUE( INT_IMAGE_2D_MULTISAMPLE );
        ADD_ENUM_VALUE( INT_IMAGE_2D_MULTISAMPLE_ARRAY );

        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_1D );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_2D );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_3D );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_2D_RECT );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_CUBE );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_BUFFER );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_1D_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_2D_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_2D_MULTISAMPLE );
        ADD_ENUM_VALUE( UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY );

        ADD_ENUM_VALUE( UNDEFINED );
    END_ENUM_SERIALIZER();  // _type

    ADD_UINT_SERIALIZER( NumElements, 0 );  // _numElements
    ADD_USER_SERIALIZER( Elements );  // _floatArray, _doubleArray, _intArray, _uintArray, _int64Array, _uint64Array
    ADD_OBJECT_SERIALIZER( UpdateCallback, osg::UniformCallback, NULL );  // _updateCallback
    ADD_OBJECT_SERIALIZER( EventCallback, osg::UniformCallback, NULL );  // _eventCallback

    // Custom methods
    ADD_METHOD_OBJECT( "value", UniformValueMethod );
}
