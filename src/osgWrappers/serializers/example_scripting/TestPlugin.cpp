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

#include <osgText/TextBase>

/// osg::TextBase Font property, but as a UTF-8 string filename rather than as
/// an RW_USER serializer
class FontNameSerializer:
    public osgDB::VirtualPropertySerializer<osgText::TextBase>
{
public:
    VPS_COMMON(FontNameSerializer, osgText::TextBase, GET_SET, STRING)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        std::string newname;
        is >> newname;
        object.setFont(newname);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        if(object.getFont()) {
            os << object.getFont()->getFileName();
        } else {
            os << "";
        }
        return true;
    }

};

/// osg::TextBase FontSize property: Width as an RW_UINT value
class ResolutionWSerializer: public osgDB::VirtualPropertySerializer<osgText::TextBase>
{
public:
    VPS_COMMON(ResolutionWSerializer, osgText::TextBase, GET_SET, UINT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        unsigned int width;
        is >> width;
        object.setFontResolution(width, object.getFontHeight());
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getFontWidth();
        return true;
    }

};

/// osg::TextBase FontSize property: Height as an RW_UINT value
class ResolutionHSerializer: public osgDB::VirtualPropertySerializer<osgText::TextBase>
{
public:
    VPS_COMMON(ResolutionHSerializer, osgText::TextBase, GET_SET, UINT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        unsigned int height;
        is >> height;
        object.setFontResolution(object.getFontWidth(), height);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getFontHeight();
        return true;
    }

};

/// osg::TextBase CharacterSize property: Height as an RW_UINT value
class CharHeightSerializer: public osgDB::VirtualPropertySerializer<osgText::TextBase>
{
public:
    VPS_COMMON(CharHeightSerializer, osgText::TextBase, GET_SET, FLOAT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        float height;
        is >> height;
        object.setCharacterSize(height);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getCharacterHeight();
        return true;
    }

};

/// osg::TextBase CharacterSize property: AspectRatio as an RW_UINT value
class CharAspectRatioSerializer: public osgDB::VirtualPropertySerializer<osgText::TextBase>
{
public:
    VPS_COMMON(CharAspectRatioSerializer, osgText::TextBase, GET_SET, FLOAT)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        float ratio;
        is >> ratio;
        object.setCharacterSize(object.getCharacterHeight(), ratio);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        os << object.getCharacterAspectRatio();
        return true;
    }

};

/// osg::TextBase Text property, but as a UTF-8 string rather than as an
/// RW_USER serializer
class TextUTF8Serializer:
    public osgDB::VirtualPropertySerializer<osgText::TextBase>
{
public:
    VPS_COMMON(TextUTF8Serializer, osgText::TextBase, GET_SET, STRING)

    virtual bool setPropertyFromStream( osgDB::InputStream& is, TargetType& object )
    {
        std::string utf8chars;      ///< the new text
        is >> utf8chars;        // works since #is is guaranteed to be binary
        object.setText(utf8chars, osgText::String::ENCODING_UTF8);
        return true;
    }

    virtual bool getPropertyIntoStream( osgDB::OutputStream& os, const TargetType& object )
    {
        std::string utf8chars;      ///< the text in #object
        utf8chars = object.getText().createUTF8EncodedString();
        os << utf8chars;        // works since #os is guaranteed to be binary
        return true;
    }

};

BEGIN_EXTEND_OBJECT_WRAPPER( TestPlugin_osgText_TextBase, osgText::TextBase)
    ADD_VIRTUAL_PROPERTY_SERIALIZER( FontName, FontNameSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( ResolutionW, ResolutionWSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( ResolutionH, ResolutionHSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( CharHeight, CharHeightSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( CharAspectRatio, CharAspectRatioSerializer );
    ADD_VIRTUAL_PROPERTY_SERIALIZER( TextUTF8, TextUTF8Serializer );
END_EXTEND_OBJECT_WRAPPER()


// vi: set ts=4 sts=4 sw=4 et ai: //
