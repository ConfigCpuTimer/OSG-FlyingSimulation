#include "AnimationTimeUpdateCallback.h"


AnimationTimeUpdateCallback::AnimationTimeUpdateCallback() ://无参构造函数
	m_sampler(new osgAnimation::Vec3CubicBezierSampler()),
	m_bPlaying(false),
	m_uiLastUpdate(0) {
}


AnimationTimeUpdateCallback::AnimationTimeUpdateCallback(//拷贝构造函数
	const AnimationTimeUpdateCallback& atuc,
	const osg::CopyOp& co = osg::CopyOp::SHALLOW_COPY
) :
	osg::Object(atuc, co),
	osg::NodeCallback(atuc, co),
	m_sampler(atuc.m_sampler),
	m_startTime(atuc.m_startTime),
	m_currentTime(atuc.m_currentTime),
	m_bPlaying(atuc.m_bPlaying),
	m_uiLastUpdate(atuc.m_uiLastUpdate) {
}


void AnimationTimeUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
	m_uiLastUpdate = nv->getFrameStamp()->getFrameNumber();
	m_currentTime = osg::Timer::instance()->tick();

	if (m_bPlaying&&m_sampler.get() && m_sampler->getKeyframeContainer()) {
		osg::ref_ptr<osg::MatrixTransform> mt = dynamic_cast<osg::MatrixTransform*> (node);
		if (mt) {
			osg::Vec3 result;
			float t = osg::Timer::instance()->delta_s(m_startTime, m_currentTime);
			float dur = m_sampler->getEndTime() - m_sampler->getStartTime();
			t = fmod(t, dur);//取余
			t += m_sampler->getStartTime();
			m_sampler->getValueAt(t, result);
			mt->setMatrix(osg::Matrix::translate(result));
		}
	}

	traverse(node, nv);
}


void AnimationTimeUpdateCallback::start() {
	m_startTime = osg::Timer::instance()->tick();
	m_currentTime = m_startTime;
	m_bPlaying = true;
}


void AnimationTimeUpdateCallback::stop() {
	m_currentTime = m_startTime;
	m_bPlaying = false;
}