#ifndef _INCLUDE_SYNCTL_CLIENT_SENDCONTEXT_HXX_
#define _INCLUDE_SYNCTL_CLIENT_SENDCONTEXT_HXX_


#include <string>
#include <vector>

#include "synctl/Filter.hxx"


namespace synctl {

class OutputStream;
class Reference;

}


namespace synctl::client {


class SendContext
{
	// The path prefix for sent entries: filter not apply on this part.
	// If context is the root context, points to _path.
	std::string      *_root;

	// The path after the _root, indicating the entry.
	std::string       _path;

	synctl::Filter   *_filter = nullptr;
	Filter::Action    _action = Filter::Action::Accept;


	SendContext(std::string *root, const std::string &path,
		    synctl::Filter *filter, synctl::Filter::Action action);


 public:
	SendContext();
	SendContext(const std::string &root, synctl::Filter *filter);
	SendContext(const SendContext &other) = default;


	SendContext &operator=(const SendContext &other) = default;


	bool send(synctl::OutputStream *out, synctl::Reference *dest,
		  const std::string &name);
	bool send(synctl::OutputStream *out, synctl::Reference *dest,
		  const char *name);
	bool send(synctl::OutputStream *out, synctl::Reference *dest);


	const std::string path() const;

	Filter::Action action() const noexcept;
};


}


#endif
