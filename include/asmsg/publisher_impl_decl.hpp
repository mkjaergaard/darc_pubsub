#pragma once

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals.hpp>

namespace asmsg
{

template<typename T, class IDType>
class PublisherImpl
{
private:
  boost::asio::io_service &io_service_;
  MessageServiceType message_service_;
  IDType topic_id_;

  typename MessageServiceType::template DispatcherType<T> * dispatcher_; // ptr type?

public:
  PublisherImpl(boost::asio::io_service &io_service,
		MessageService &message_service,
		const IDType& topic_id) :
    io_service_(io_service),
    message_service_(message_service),
    topic_id_(topic_id)
  {
  }

  void attach();
  void detach();
  void publish(const boost::shared_ptr<const T&> msg);

};

}
