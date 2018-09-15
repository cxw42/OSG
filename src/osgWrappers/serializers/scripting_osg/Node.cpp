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

#include <osg/Node>

namespace {

/// =========================================================================
/// Make Node methods accessible to scripts

/// Make Node::getOrCreateStateSet() accessible to scripts
struct GetOrCreateStateSet : public osgDB::MethodObject
{
    virtual bool run(   osg::Object* objectPtr
                      , osg::Parameters& //inputParameters - none used
                      , osg::Parameters& outputParameters) const
    {
        osg::Node *node = objectPtr->asNode();
        if(!node)
        {
            OSG_NOTICE << "Cannot call getOrCreateStateSet on non-Node" << std::endl;
            return false;
        }

        osg::StateSet *ss = node->getOrCreateStateSet();
        outputParameters.push_back(ss);
        return true;
    }
};

/// Make Node::dirtyBound() accessible to scripts
struct DirtyBound : public osgDB::MethodObject
{
    virtual bool run(   osg::Object* objectPtr
                      , osg::Parameters& //inputParameters - none used
                      , osg::Parameters& //outputParameters
                      ) const
    {
        osg::Node *node = objectPtr->asNode();
        if(!node)
        {
            OSG_NOTICE << "Cannot call dirtyBound on non-Node" << std::endl;
            return false;
        }

        node->dirtyBound();
        return true;
    }
};

/// Make Node::getBound() accessible to scripts
struct GetBound : public osgDB::MethodObject
{
    virtual bool run(   osg::Object* objectPtr
                      , osg::Parameters& //inputParameters - none used
                      , osg::Parameters& outputParameters) const
    {
        osg::Node *node = objectPtr->asNode();
        if(!node)
        {
            OSG_NOTICE << "Cannot call getBound on non-Node" << std::endl;
            return false;
        }

        osg::ref_ptr<osg::BoundingSphereValueObject> bound(
                new osg::BoundingSphereValueObject(node->getBound()));
        outputParameters.push_back(bound);
        return true;
    }
};

} //namespace

BEGIN_EXTEND_OBJECT_WRAPPER( scripting_osg_Node, osg::Node)
    // Members
#ifdef OSG_USE_FLOAT_BOUNDINGSPHERE
    ADD_BOUNDINGSPHEREF_SERIALIZER2( InitialBoundSphere, InitialBound, osg::BoundingSphere() );
#else
    ADD_BOUNDINGSPHERED_SERIALIZER2( InitialBoundSphere, InitialBound, osg::BoundingSphere() );
#endif

    // Methods
    ADD_METHOD_OBJECT( "getOrCreateStateSet", GetOrCreateStateSet );
    ADD_METHOD_OBJECT( "dirtyBound", DirtyBound );
    ADD_METHOD_OBJECT( "getBound", GetBound );
END_EXTEND_OBJECT_WRAPPER()

// vi: set ts=4 sts=4 sw=4 et ai: //
