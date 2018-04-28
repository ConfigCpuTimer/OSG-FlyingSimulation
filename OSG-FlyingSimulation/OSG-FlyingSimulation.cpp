// OSG-FlyingSimulation.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*  -*-c++-*-
*  Copyright (C) 2008 Cedric Pinson <mornifle@plopbyte.net>
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
*
*  Authors:
*   Jeremy Moles  <jeremy@emperorlinux.com>
*   Cedric Pinson <mornifle@plopbyte.net>
*/

#include <iostream>

#include <osg/io_utils>
#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/MatrixTransform>

#include <osgAnimation/EaseMotion>//冗余标头，但文件定义的Motion类中的getValue()方法可以解释
#include <osgAnimation/Sampler>//Sampler中的getValueAt()

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

#include "AnimationTimeUpdateCallback.h"

#include "MakePathTimeCallback.h"

#include "MakePathDistanseCallback.h"


osg::MatrixTransform* setupAnimtkNode(osg::Geode* staticGeode)
{
	osg::Vec3 v[5];

	v[0] = osg::Vec3(0, 0, 0);
	v[1] = osg::Vec3(20, 40, 60);
	v[2] = osg::Vec3(40, 60, 20);
	v[3] = osg::Vec3(60, 20, 40);
	v[4] = osg::Vec3(0, 0, 0);

	osg::ref_ptr<osg::MatrixTransform> node = new osg::MatrixTransform();
	//osg::MatrixTransform* node = new osg::MatrixTransform();
	
	osg::ref_ptr<AnimationTimeUpdateCallback> callback = new MakePathDistanseCallback(staticGeode);
	//AnimationTimeUpdateCallback* callback = new MakePathDistanceCallback(staticGeode);
	
	osg::ref_ptr<osgAnimation::Vec3CubicBezierKeyframeContainer> keys = callback->m_sampler->getOrCreateKeyframeContainer();
	//osgAnimation::Vec3CubicBezierKeyframeContainer* keys = callback->m_sampler->getOrCreateKeyframeContainer();

	keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(0, osgAnimation::Vec3CubicBezier(
		v[0],
		v[0] + (v[0] - v[3]),
		v[1] - (v[1] - v[0])
	)));

	keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(2, osgAnimation::Vec3CubicBezier(
		v[1],
		v[1] + (v[1] - v[0]),
		v[2] - (v[2] - v[1])
	)));

	keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(4, osgAnimation::Vec3CubicBezier(
		v[2],
		v[2] + (v[2] - v[1]),
		v[3] - (v[3] - v[2])
	)));

	keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(6, osgAnimation::Vec3CubicBezier(
		v[3],
		v[3] + (v[3] - v[2]),
		v[4] - (v[4] - v[3])
	)));

	keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(8, osgAnimation::Vec3CubicBezier(
		v[4],
		v[4] + (v[4] - v[3]),
		v[0] - (v[0] - v[4])
	)));

	callback->start();
	node->setUpdateCallback(callback);

	osg::Geode* geode = new osg::Geode();

	//geode->setStateSet(setupStateSet());
	geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 2)));

	node->addChild(geode);

	return node;
}


int _tmain(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osgViewer::Viewer viewer(arguments);

	osgGA::TrackballManipulator* tbm = new osgGA::TrackballManipulator();

	viewer.setCameraManipulator(tbm);

	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());

	osg::Group* root = new osg::Group();
	osg::Geode* geode = new osg::Geode();

	//geode->setStateSet(setupStateSet());

	root->setInitialBound(osg::BoundingSphere(osg::Vec3(10, 0, 20), 50));
	root->addChild(setupAnimtkNode(geode));
	root->addChild(geode);

	viewer.setSceneData(root);

	// tbm->setDistance(150);

	return viewer.run();
}

