#include "MakePathTimeCallback.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>


MakePathTimeCallback::MakePathTimeCallback(osg::Geode* geo) :
	m_geode(geo),
	m_fLastAdd(0.0f),
	m_fAddSeconds(0.08f) {
}


void MakePathTimeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
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