// -*-c++-*-

/*
 * $Id$
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <osgDB/ClassInterface>
#include <osgDB/ObjectWrapper>
#include <osgDB/OutputStream>
#include <osgDB/Serializer>
#include <osgDB/WriteFile>
#include <osgDB/ReaderWriter>
#include <osgDB/Registry>
#include <osg/Notify>

#include <osg/Camera>

/// osg::Camera RenderOrder property, but as a string rather than RW_USER
class RenderOrderWhichSerializer:
    public osgDB::VirtualPropertySerializer<osg::Camera>
{
public:
    VPS_COMMON(RenderOrderWhichSerializer, osg::Camera, GET_SET, STRING)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        std::string utf8chars;          ///< the string of the new order
        osg::Camera::RenderOrder ro;    ///< the new order

        is >> utf8chars;        // works since #is is guaranteed to be binary

        if(utf8chars == "PRE_RENDER") {
            ro = osg::Camera::PRE_RENDER;
        } else if(utf8chars == "NESTED_RENDER") {
            ro = osg::Camera::NESTED_RENDER;
        } else if(utf8chars == "POST_RENDER") {
            ro = osg::Camera::POST_RENDER;
        } else {
            return false;
        }

        object.setRenderOrder(ro, object.getRenderOrderNum());
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        std::string s;

        osg::Camera::RenderOrder ro(object.getRenderOrder());
        switch(ro) {
            case osg::Camera::PRE_RENDER: s="PRE_RENDER"; break;
            case osg::Camera::NESTED_RENDER: s="NESTED_RENDER"; break;
            case osg::Camera::POST_RENDER: s="POST_RENDER"; break;
            default: return false;
        }

        os << s;        // works since #os is guaranteed to be binary
        return true;
    }

};

/// osg::Camera RenderOrderNum property
class RenderOrderNumSerializer: public osgDB::VirtualPropertySerializer<osg::Camera>
{
public:
    VPS_COMMON(RenderOrderNumSerializer, osg::Camera, GET_SET, INT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        int num;
        is >> num;
        object.setRenderOrder(object.getRenderOrder(), num);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getRenderOrderNum();
        return true;
    }

};

BEGIN_EXTEND_OBJECT_WRAPPER( scripting_osg_Camera, osg::Camera)
    ADD_VIRTUAL_PROPERTY_SERIALIZER( RenderOrderWhich, RenderOrderWhichSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( RenderOrderNum, RenderOrderNumSerializer );
    // TODO: BufferAttachmentMap
END_EXTEND_OBJECT_WRAPPER()

// vi: set ts=4 sts=4 sw=4 et ai: //
