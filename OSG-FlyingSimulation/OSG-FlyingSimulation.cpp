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


class MakePathTimeCallback :public AnimationTimeUpdateCallback {
public:
	MakePathTimeCallback(osg::Geode* geo) :
		m_geode(geo),
		m_fLastAdd(0.0f),
		m_fAddSeconds(0.08f) {
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
		float t = osg::Timer::instance()->delta_s(m_startTime, m_currentTime);

		if (m_fLastAdd + m_fAddSeconds <= t&&t <= 8.0f) {
			osg::Vec3 pos;
			m_sampler->getValueAt(t, pos);
			
			m_geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(pos, 0.5f)));//
			m_geode->dirtyBound();

			m_fLastAdd += m_fAddSeconds;
		}

		AnimationTimeUpdateCallback::operator()(node, nv);//
	}
protected:
	osg::ref_ptr<osg::Geode> m_geode;
	float m_fLastAdd;
	float m_fAddSeconds;
};


class MakePathDistanseCallback :public AnimationTimeUpdateCallback {
protected:
	osg::ref_ptr<osg::Geode> m_geode;
	osg::Vec3 m_lastAdd;
	float m_fThreshold;
	unsigned int m_uiCount;
public:
	MakePathDistanseCallback(osg::Geode* geode) :
		m_geode(geode),
		m_fThreshold(0.5f),
		m_uiCount(0) {
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
		static bool countReported = false;
		float t = osg::Timer::instance()->delta_s(m_startTime, m_currentTime);
		osg::Vec3 pos;
		m_sampler->getValueAt(t, pos);
		osg::Vec3 distance = m_lastAdd - pos;

	}
};


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

	geode->setStateSet(setupStateSet());

	root->setInitialBound(osg::BoundingSphere(osg::Vec3(10, 0, 20), 50));
	root->addChild(setupAnimtkNode(geode));
	root->addChild(geode);

	viewer.setSceneData(root);

	// tbm->setDistance(150);

	return viewer.run();
}

