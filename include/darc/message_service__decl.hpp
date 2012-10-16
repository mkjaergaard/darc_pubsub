#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <darc/basic_local_dispatcher__fwd.hpp>
#include <boost/bind.hpp>

#include <darc/id.hpp>
#include <darc/ns_service.hpp>
#include <darc/peer_service.hpp>
#include <darc/remote_dispatcher.hpp>

namespace darc
{

// fwd
template<typename T>
class PublisherImpl;

template<typename T>
class SubscriberImpl;

template<typename T>
class DispatcherGroup;
class BasicDispatcherGroup;

template<typename T>
class LocalDispatcher;

class MessageService : public boost::noncopyable, public darc::peer_service
{
private:
  typedef std::map<darc::ID, boost::shared_ptr<BasicDispatcherGroup> > DispatcherGroupListType;
  DispatcherGroupListType dispatcher_group_list_;

  boost::asio::io_service& io_service_;
  RemoteDispatcher remote_dispatcher_;

public:
  ns_service& nameserver_;

public:
  MessageService(peer& p, boost::asio::io_service& io_service, ns_service& ns_service) :
    peer_service(p),
    io_service_(io_service),
    remote_dispatcher_(p),
    nameserver_(ns_service)
  {
  }

  // /////////////////////////////////////////////
  // Remap peer_service calls to remote_dispatcher
  //
  void recv(const darc::ID& src_peer_id,
	    service_type service,
	    darc::buffer::shared_buffer data)
  {
    remote_dispatcher_.recv(src_peer_id, service, data);
  }

  // /////////////////////////////////////////////

  void post_new_tag_event(ID tag_id,
			  ID alias_id,
			  ID peer_id)
  {
    io_service_.post(boost::bind(&MessageService::new_tag_event, this, tag_id, alias_id, peer_id));
  }

  void new_tag_event(ID tag_id,
		     ID alias_id,
		     ID peer_id);

  template<typename T>
  LocalDispatcher<T>* attach(PublisherImpl<T> &publisher, const std::string &topic);

  template<typename T>
  LocalDispatcher<T>* attach(SubscriberImpl<T> &subscriber, const std::string &topic);

  template<typename T>
  void detach(SubscriberImpl<T> &subscriber)
  {
  }

  template<typename T>
  DispatcherGroup<T>* getDispatcherGroup(const std::string& topic);

  template<typename T>
  DispatcherGroup<T>* getDispatcherGroup(const tag_handle& tag);

};

}