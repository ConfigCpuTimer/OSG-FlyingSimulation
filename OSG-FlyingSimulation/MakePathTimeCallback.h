#pragma once

#include "AnimationTimeUpdateCallback.h"


class MakePathTimeCallback :public AnimationTimeUpdateCallback {
public:
	MakePathTimeCallback(osg::Geode* geo);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	osg::ref_ptr<osg::Geode> m_geode;
	float m_fLastAdd;
	float m_fAddSeconds;
};