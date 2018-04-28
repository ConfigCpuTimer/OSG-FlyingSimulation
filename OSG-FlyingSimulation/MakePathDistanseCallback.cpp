#include "MakePathDistanseCallback.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>


MakePathDistanseCallback::MakePathDistanseCallback(osg::Geode* geode) :
	m_geode(geode),
	m_fThreshold(0.5f),
	m_uiCount(0) {
}

void MakePathDistanseCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
	static bool countReported = false;
	float t = osg::Timer::instance()->delta_s(m_startTime, m_currentTime);

	osg::Vec3 pos;
	osg::Vec3 distance = m_lastAdd - pos;
	m_sampler->getValueAt(t, pos);

	if (t <= 8.0f&&distance.length() >= m_fThreshold) {
		m_geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(pos, 0.25f)));//Â·¾¶Çò
		m_lastAdd = pos;
		m_uiCount++;
	}
	else if (t > 8.0f) {
		if (!countReported) std::cout << "Created " << m_uiCount << " nodes." << std::endl;
		countReported = true;
	}

	AnimationTimeUpdateCallback::operator()(node, nv);
}