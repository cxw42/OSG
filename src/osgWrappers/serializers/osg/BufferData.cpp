#include <osg/BufferObject>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

/// Dirty the buffer.  Call this when you have updated the contents of
/// the buffer.
struct DirtyBufferDataMethod : public osgDB::MethodObject
{
    /// No parameters.
    virtual bool run(   osg::Object* objectPtr
                      , osg::Parameters& //inputParameters
                      , osg::Parameters& //outputParameters
                    ) const
    {
        osg::BufferData *bufferdata = dynamic_cast<osg::BufferData*>(objectPtr);
        if(!bufferdata) return false;

        bufferdata->dirty();

        return true;
    }
};


REGISTER_OBJECT_WRAPPER( BufferData,
                         0,
                         osg::BufferData,
                         "osg::Object osg::BufferData" )
{
    {
        UPDATE_TO_VERSION_SCOPED( 147 )
        ADD_OBJECT_SERIALIZER(BufferObject, osg::BufferObject, NULL);
    }
    //
    // Custom methods
    ADD_METHOD_OBJECT( "dirty", DirtyBufferDataMethod );
}
