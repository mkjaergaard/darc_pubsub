#pragma once

#include <boost/asio.hpp>
#include <asmsg/subscriber.hpp>
#include <asmsg/publisher.hpp>

namespace asmsg
{

template<typename T>
class DispatcherGroup;

template<typename T>
class LocalDispatcher
{
private:
  typedef std::vector<PublisherImpl<T> *> PublishersListType;
  typedef std::vector<SubscriberImpl<T> *> SubscribersListType;

  PublishersListType publishers_;
  SubscribersListType subscribers_;

  DispatcherGroup<T> * group_;

public: //todo:
  hns::TagHandle tag_;

public:
  LocalDispatcher(DispatcherGroup<T> * group, const hns::TagHandle& tag) :
    group_(group),
    tag_(tag)
  {
  }

  void setGroup(DispatcherGroup<T> * group)
  {
    group_ = group;
  }

  void attach(SubscriberImpl<T> &subscriber)
  {
    subscribers_.push_back(&subscriber);
  }

  void attach(PublisherImpl<T> &publisher)
  {
    publishers_.push_back(&publisher);
  }

  void dispatchLocally(const boost::shared_ptr<const T> &msg)
  {
    for(typename SubscribersListType::iterator it = subscribers_.begin();
	it != subscribers_.end();
	it++)
    {
      beam::glog<beam::Info>("POST");
      (*it)->postCallback(msg);
    }
  }

  void dispatchFromPublisher(const boost::shared_ptr<const T> &msg)
  {
    group_->dispatchToGroup(msg);
  }

};

}
