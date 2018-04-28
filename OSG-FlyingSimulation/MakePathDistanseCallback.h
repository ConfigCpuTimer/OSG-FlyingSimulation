#pragma once

#include "AnimationTimeUpdateCallback.h"


class MakePathDistanseCallback :public AnimationTimeUpdateCallback {
protected:
	osg::ref_ptr<osg::Geode> m_geode;
	osg::Vec3 m_lastAdd;
	float m_fThreshold;
	unsigned int m_uiCount;
public:
	MakePathDistanseCallback(osg::Geode* geode);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};