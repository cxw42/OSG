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

/// Text property, but as a UTF-8 string rather than as an RW_USER serializer
class Text8Serializer:
    public osgDB::VirtualPropertySerializer<osgText::Text>
{
public:
    typedef osgDB::VirtualPropertySerializer<osgText::Text> ParentType;
    Text8Serializer(const char* name):
        ParentType( name,
                    osgDB::BaseSerializer::GET_SET_PROPERTY,
                    osgDB::BaseSerializer::RW_STRING)
    {}

    virtual bool setPropertyFrom( osgDB::InputStream& is, TargetType& object )
    {
        std::string utf8chars;      ///< the new text
        is >> utf8chars;        // works since #is is guaranteed to be binary
        object.setText(utf8chars, osgText::String::ENCODING_UTF8);
        return true;
    }

    virtual bool getPropertyInto( osgDB::OutputStream& os, const TargetType& object )
    {
        std::string utf8chars;      ///< the text in #object
        utf8chars = object.getText().createUTF8EncodedString();
        os << utf8chars;        // works since #os is guaranteed to be binary
        return true;
    }

};

BEGIN_EXTEND_OBJECT_WRAPPER( TestPlugin_osgText_Text, osgText::Text)
    ADD_VIRTUAL_PROPERTY_SERIALIZER( Text8, Text8Serializer );
END_EXTEND_OBJECT_WRAPPER()


// vi: set ts=4 sts=4 sw=4 et ai: //
