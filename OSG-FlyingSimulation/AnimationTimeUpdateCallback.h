#pragma once
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osg/Timer>

#include <osgAnimation/Sampler>


class AnimationTimeUpdateCallback :public osg::NodeCallback {
public:
	AnimationTimeUpdateCallback();//无参构造函数

	AnimationTimeUpdateCallback(//拷贝构造函数
		const AnimationTimeUpdateCallback& atuc,
		const osg::CopyOp& co = osg::CopyOp::SHALLOW_COPY
	);

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	void start();

	void stop();

protected:
	osg::ref_ptr<osgAnimation::Vec3CubicBezierSampler> m_sampler;
	osg::Timer_t m_startTime;
	osg::Timer_t m_currentTime;
	bool m_bPlaying;
	unsigned int m_uiLastUpdate;
};
