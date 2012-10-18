#include <darc/remote_dispatcher.hpp>
#include <darc/message_service.hpp>

namespace darc
{

void RemoteDispatcher::handle_message_packet(const darc::ID& src_peer_id,
					      darc::buffer::shared_buffer data)
{
  inbound_data<serializer::boost_serializer, message_packet> msg_i(data);

  // Just send the buffer to the dispatcher. not the nicest way
  parent_service_->remote_message_recv(msg_i.get().tag_id, data);
}

void RemoteDispatcher::send_to(const ID& peer_id, const outbound_data_base& data)
{
  parent_service_->send_to(peer_id, data);
}

}
