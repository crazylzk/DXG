#ifndef IDXGFRAMELISTNER_H
#define IDXGFRAMELISTNER_H
class IDXGFrameListner
{

  public:
	  virtual ~IDXGFrameListner(){};
	  virtual int getUpdateOrder() = 0;

	  virtual void frameBegin() = 0;
	  virtual void frameUpdate() = 0;
	  virtual void frameEnd() = 0;

};

#endif