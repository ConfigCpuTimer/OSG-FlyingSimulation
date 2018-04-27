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
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgAnimation/Sampler>


class AnimationTimeUpdateCallback :public osg::NodeCallback {
public:
	AnimationTimeUpdateCallback() ://无参构造函数
		m_Sampler(new osgAnimation::Vec3CubicBezierSampler()),
		m_bPlaying(false),
		m_uiLastUpdate(0) {}

	AnimationTimeUpdateCallback(//拷贝构造函数
		const AnimationTimeUpdateCallback& atuc,
		const osg::CopyOp& co = osg::CopyOp::SHALLOW_COPY
	) :
		osg::Object(atuc, co),
		osg::NodeCallback(atuc, co),
		m_Sampler(atuc.m_Sampler),
		m_StartTime(atuc.m_StartTime),
		m_CurrentTime(atuc.m_CurrentTime),
		m_bPlaying(atuc.m_bPlaying),
		m_uiLastUpdate(atuc.m_uiLastUpdate) {}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
		m_uiLastUpdate = nv->getFrameStamp()->getFrameNumber();
		m_CurrentTime = osg::Timer::instance()->tick();

		if (m_bPlaying&&m_Sampler.get() && m_Sampler->getKeyframeContainer()) {
			osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*> (node);
			if (mt) {
				osg::Vec3 result;
				float t = osg::Timer::instance()->delta_s(m_StartTime, m_CurrentTime);
				float dur = m_Sampler->getEndTime() - m_Sampler->getStartTime();
				t = fmod(t, dur);//取余
				t += m_Sampler->getStartTime();
				m_Sampler->getValueAt(t, result);
				mt->setMatrix(osg::Matrix::translate(result));
			}
		}

		traverse(node, nv);
	}

	void start() {
		m_StartTime = osg::Timer::instance()->tick();
		m_CurrentTime = m_StartTime;
		m_bPlaying = true;
	}

	void stop() {
		m_CurrentTime = m_StartTime;
		m_bPlaying = false;
	}

protected:
	osg::ref_ptr<osgAnimation::Vec3CubicBezierSampler> m_Sampler;
	osg::Timer_t m_StartTime;
	osg::Timer_t m_CurrentTime;
	bool m_bPlaying;
	unsigned int m_uiLastUpdate;
};


class MakePathTimeCallback :public AnimationTimeUpdateCallback {
public:
	MakePathTimeCallback(osg::Geode* geo) :
		m_Geode(geo),
		m_fLastAdd(0.0f),
		m_fAddSeconds(0.08f) {}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
		float t = osg::Timer::instance()->delta_s(m_StartTime, m_CurrentTime);

		if (m_fLastAdd + m_fAddSeconds <= t&&t <= 8.0f) {
			osg::Vec3 pos;
			m_Sampler->getValueAt(t, pos);
			
			m_Geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(pos, 0.5f)));//
			m_Geode->dirtyBound();

			m_fLastAdd += m_fAddSeconds;
		}

		AnimationTimeUpdateCallback::operator()(node, nv);//
	}
protected:
	osg::ref_ptr<osg::Geode> m_Geode;
	float m_fLastAdd;
	float m_fAddSeconds;
};


class MakePathDistanseCallback :public AnimationTimeUpdateCallback {
protected:
	osg::ref_ptr<osg::Geode> m_Geode;
	osg::Vec3
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

