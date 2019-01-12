#ifndef _INCLUDE_SYNCTL_SENDCONTEXT_HXX_
#define _INCLUDE_SYNCTL_SENDCONTEXT_HXX_


#include <string>


namespace synctl {


class Filter;
class OutputStream;


class SendContext
{
	std::string    _root;
	OutputStream  *_stream;
	Filter        *_filter;


 public:
	SendContext();
	SendContext(const SendContext &other) = delete;
	SendContext(const std::string &root, OutputStream *stream,
		    Filter *filter);


	SendContext &operator=(const SendContext &other) = delete;
	

	// Return the soutput stream to send the data on.
	OutputStream *stream();

	// Indicates if a specific path should be sent in this context.
	// Only indicates logical send, independently of if the entry is
	// already referenced.
	// The given path must be relative to the result of root().
	bool contains(const std::string &path);

	// Indicates the root of the sent hierarchy.
	// Prepend the result to a path to obtain the path in the client fs.
	const std::string &root() const;
};


}


#endif
