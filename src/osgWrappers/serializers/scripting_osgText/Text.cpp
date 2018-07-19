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

#include <osgText/Text>

/// osg::Text BackdropOffset property: Horizontal offset as an RW_FLOAT value
class BackdropOffsetHSerializer: public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    VPS_COMMON(BackdropOffsetHSerializer, osgText::Text, GET_SET, FLOAT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        float hoffset;
        is >> hoffset;
        object.setBackdropOffset(hoffset, object.getBackdropVerticalOffset());
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getBackdropHorizontalOffset();
        return true;
    }

};

/// osg::Text BackdropOffset property: Vertical offset as an RW_FLOAT value
class BackdropOffsetVSerializer: public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    VPS_COMMON(BackdropOffsetVSerializer, osgText::Text, GET_SET, FLOAT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        float voffset;
        is >> voffset;
        object.setBackdropOffset(object.getBackdropHorizontalOffset(), voffset);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getBackdropVerticalOffset();
        return true;
    }

};

/// osg::Text ColorGradientCorners property: Top-left color as an RW_VEC4F value
class CGTopLeftSerializer: public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    VPS_COMMON(CGTopLeftSerializer, osgText::Text, GET_SET, VEC4F)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        osg::Vec4 value;
        is >> value;
        object.setColorGradientCorners(
                value,
                object.getColorGradientBottomLeft(),
                object.getColorGradientBottomRight(),
                object.getColorGradientTopRight()
        );
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getColorGradientTopLeft();
        return true;
    }

};

/// osg::Text ColorGradientCorners property: Bottom-left color as an RW_VEC4F value
class CGBottomLeftSerializer: public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    VPS_COMMON(CGBottomLeftSerializer, osgText::Text, GET_SET, VEC4F)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        osg::Vec4 value;
        is >> value;
        object.setColorGradientCorners(
                object.getColorGradientTopLeft(),
                value,
                object.getColorGradientBottomRight(),
                object.getColorGradientTopRight()
        );
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getColorGradientBottomLeft();
        return true;
    }

};

/// osg::Text ColorGradientCorners property: Bottom-right color as an RW_VEC4F value
class CGBottomRightSerializer: public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    VPS_COMMON(CGBottomRightSerializer, osgText::Text, GET_SET, VEC4F)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        osg::Vec4 value;
        is >> value;
        object.setColorGradientCorners(
                object.getColorGradientTopLeft(),
                object.getColorGradientBottomLeft(),
                value,
                object.getColorGradientTopRight()
        );
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getColorGradientBottomRight();
        return true;
    }

};

/// osg::Text ColorGradientCorners property: Top-right color as an RW_VEC4F value
class CGTopRightSerializer: public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    VPS_COMMON(CGTopRightSerializer, osgText::Text, GET_SET, VEC4F)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        osg::Vec4 value;
        is >> value;
        object.setColorGradientCorners(
                object.getColorGradientTopLeft(),
                object.getColorGradientBottomLeft(),
                object.getColorGradientBottomRight(),
                value
        );
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getColorGradientTopRight();
        return true;
    }

};

BEGIN_EXTEND_OBJECT_WRAPPER( scripting_osgText_Text, osgText::Text)
    ADD_VIRTUAL_PROPERTY_SERIALIZER( BackdropOffsetH, BackdropOffsetHSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( BackdropOffsetV, BackdropOffsetVSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( CGTopLeft, CGTopLeftSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( CGBottomLeft, CGBottomLeftSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( CGBottomRight, CGBottomRightSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( CGTopRight, CGTopRightSerializer );
END_EXTEND_OBJECT_WRAPPER()

// vi: set ts=4 sts=4 sw=4 et ai: //
