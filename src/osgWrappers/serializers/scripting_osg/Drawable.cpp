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

#include <osg/Drawable>

BEGIN_EXTEND_OBJECT_WRAPPER( scripting_osg_Drawable, osg::Drawable)
#ifdef OSG_USE_FLOAT_BOUNDINGBOX
    ADD_BOUNDINGBOXF_SERIALIZER2( InitialBoundBox, InitialBound, osg::BoundingBox() );
#else
    ADD_BOUNDINGBOXD_SERIALIZER2( InitialBoundBox, InitialBound, osg::BoundingBox() );
#endif
END_EXTEND_OBJECT_WRAPPER()

// vi: set ts=4 sts=4 sw=4 et ai: //
