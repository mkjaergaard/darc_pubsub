#pragma once

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals.hpp>

#include <darc/pubsub/message_service__decl.hpp>

#include <darc/id.hpp>

#include <boost/make_shared.hpp>

namespace darc
{
namespace pubsub
{

template<typename T>
class publisher_impl
{
private:
  boost::asio::io_service &io_service_;
  message_service &message_service_;

  local_dispatcher<T> * dispatcher_; // ptr type?

public:
  publisher_impl(boost::asio::io_service &io_service,
                message_service &message_service) :
    io_service_(io_service),
    message_service_(message_service),
    dispatcher_(0)
  {
  }

  void attach(const std::string& topic)
  {
    dispatcher_ = message_service_.template attach<T>(*this, topic);
  }

  void detach()
  {
    if(dispatcher_ != 0)
    {
      message_service_.detach(*this, dispatcher_);
      dispatcher_ = 0;
    }
  }

  void publish(const boost::shared_ptr<const T> &msg)
  {
    if(dispatcher_ != 0)
    {
      dispatcher_->dispatch_from_publisher(msg);
    }
  }

  void publish(const T &msg)
  {
    if(dispatcher_ != 0)
    {
      dispatcher_->dispatch_from_publisher(msg);
    }
  }

};

template<typename T>
class publisher
{
private:
  boost::shared_ptr<publisher_impl<T> > impl_;
  bool ok_;

public:
  publisher() :
    ok_(false)
  {
  }

  publisher(boost::asio::io_service &io_service,
            message_service &message_service) :
    impl_(boost::make_shared<publisher_impl<T> >(boost::ref(io_service),
                                                 boost::ref(message_service))),
    ok_(true)
  {
  }

  void publish(const boost::shared_ptr<const T> &msg)
  {
    if(ok_)
    {
      impl_->publish(msg);
    }
  }

  void publish(const T &msg)
  {
    if(ok_)
    {
      impl_->publish(msg);
    }
  }

  void attach(const std::string& topic)
  {
    if(ok_)
    {
      impl_->attach(topic);
    }
  }

  void detach()
  {
    if(ok_)
    {
      impl_->detach();
    }
  }

};

}
}
