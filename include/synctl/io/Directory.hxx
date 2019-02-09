#ifndef _INCLUDE_SYNCTL_DIRECTORY_HXX_
#define _INCLUDE_SYNCTL_DIRECTORY_HXX_


#include <string>
#include <vector>

#include "synctl/io/FSException.hxx"


namespace synctl {


class Directory
{
	std::string  _path;


 public:
	class CreateException : public FSException
	{
	 public:
		CreateException();
		CreateException(const std::string &what);
		CreateException(const CreateException &other) = default;
		CreateException(CreateException &&other) = default;
		virtual ~CreateException() = default;

		CreateException &operator=(const CreateException &other)
		        = default;
		CreateException &operator=(CreateException &&other) = default;

		using FSException::what;
	};


	Directory();
	Directory(const std::string &path);
	Directory(const Directory &other) = default;

	Directory &operator=(const Directory &other) = default;


	bool exists() const;
	bool listable() const;

	void create() const;
	void remove() const;


	class ConstIterator
	{
		friend class Directory;


		void         *_handle = nullptr;
		std::string   _current;


		ConstIterator(void *handle);


	 public:
		ConstIterator();
		ConstIterator(const ConstIterator &other) = delete;
		ConstIterator(ConstIterator &&other);
		~ConstIterator();

		ConstIterator &operator=(const ConstIterator &other) = delete;
		ConstIterator &operator=(ConstIterator &&other);


		bool operator==(const ConstIterator &other) const;
		bool operator!=(const ConstIterator &other) const;


		ConstIterator &operator++();
		const std::string &operator*() const;
	};

	ConstIterator begin() const;
	ConstIterator end() const;
	
	std::vector<std::string> children() const;
	std::vector<std::string> trueChildren() const;
	std::vector<std::string> sortedChildren() const;
	std::vector<std::string> sortedTrueChildren() const;


	const std::string &path() const noexcept;
};


}


#endif
