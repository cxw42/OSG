#include <osg/Viewport>
#include <osg/Vec4f>
#include <osg/ValueObject>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

namespace {

bool checkArea( const osg::Viewport& attr )
{
    return true;
}

bool readArea( osgDB::InputStream& is, osg::Viewport& attr )
{
    double x, y, w, h;
    is >> x >> y >> w >> h;
    attr.setViewport( x, y, w, h );
    return true;
}

bool writeArea( osgDB::OutputStream& os, const osg::Viewport& attr )
{
    os << attr.x() << attr.y() << attr.width() << attr.height() << std::endl;
    return true;
}

/// Vec4 get(): Return the viewport, packed as (x, y, width, height)
struct GetViewport: public osgDB::MethodObject {
    virtual bool run(osg::Object *objectPtr, osg::Parameters& ins,
                osg::Parameters& outs) const
    {
        osg::Viewport *vp = dynamic_cast<osg::Viewport*>(objectPtr);
        if(!vp) return false;

        osg::Vec4 retval(
            vp->x(),
            vp->y(),
            vp->width(),
            vp->height()
        );

        outs.push_back(new osg::Vec4fValueObject(retval));
        return true;
    }
}; //GetViewport

} // anon namespace

REGISTER_OBJECT_WRAPPER( Viewport,
                         new osg::Viewport,
                         osg::Viewport,
                         "osg::Object osg::StateAttribute osg::Viewport" )
{
    ADD_USER_SERIALIZER( Area );  // _x, _y, _width, _height

    ADD_METHOD_OBJECT( "get", GetViewport );
}
