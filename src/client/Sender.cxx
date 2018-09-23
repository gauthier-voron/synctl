#include "synctl/client/Sender.hxx"

#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/client/SendContext.hxx"


using synctl::OutputStream;
using synctl::Reference;
using synctl::client::SendContext;
using synctl::client::Sender;


bool Sender::traverse(OutputStream *out [[gnu::unused]],
		      Reference *dest [[gnu::unused]],
		      SendContext *context [[gnu::unused]])
{
	return false;
}
