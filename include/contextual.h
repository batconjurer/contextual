#ifndef CONTEXTUAL_H
#define CONTEXTUAL_H

#include <iostream>
#include <functional>
#include <optional>
#include <exception>

#define with (void) With
/*

A context manager is a class the manages two entities:
	1. resources -- temporary changes that must be made to execute a certain section of code
	2. a context -- a code block that is to be executed with temporarily held resources

Resources are managed by a resource manager which is intended to be implemented as its own class
by the user. The IResource provides an interface for this class. The resource manager is
responsible for the knowledge of resource acquistion and release. It also forwards the context
to the context manager.

A context manager must then perform the following actions:
	1. Direct the IResource class to acquire resources
	2. Make the resources available to the code block defining the context
	3. Execute the context
	4. Perform appropriate exception handling if necessary
	5. Release the resources and cleanup before exiting back to outer scope.


A class With is defined so that its brace-initialization resembles a traditional
code block and gives the impression of With being a keyword. Most of its rule of five
operations are deleted to enforce this usage as it is not a traditional object, also
ensuring that the destructors of all temporary objects are called.

However, since With instantiates an instance, a compiler might issue a warning of an 
unused variable. To avoid this, a macro "with" is defined which the preprocessor changes
to (void) With. This removes the warnings. This is just some syntactic sugar and does not
work if the With class is given a name (although doing this is discouraged).

*/

namespace Contextual {

// Forward declaration of the With class
class With;
// The struct that will hold the resources acquired for the context,
// to be defined by user
struct IData;

/********************************************
*											*
* 	The resource manager class interface	*
*											*
********************************************/

template <class data>
class IResource {	
protected:
	// The actual resources
	
	data* resources;
	virtual void enter() = 0;
	virtual void exit(std::optional<std::exception> e) = 0;

public:
	friend class With;
	
	IResource<data>() = default;
	IResource<data>(data& resources) : resources(&resources){};
	IResource<data>(data* resources) : resources(resources){};
	
	With operator()(const std::optional<std::function<void(IData*)>>& code_block={});
	
};

/************************************
*									*
* 	The With class the emulates a 	*
*        keyword with code block 	*
*									*
************************************/

class With {
private:
	std::optional<std::function<void(IData*)>> _context = {};
		
public:
	IResource<IData>* resource;
	// The rule of five
	With() = delete;
	With(const With& other) = delete;
	With& operator=(const With& other) = delete;
	With& operator=(const With&& other) = delete;

	~With() = default;
	
	With(std::optional<std::function<void(IData*)>> context,
		 				 IResource<IData>* resource=nullptr):
													 		_context(context),
									 						resource(resource){
		try{
			// Execute the context
			resource->enter();
			if (_context){
				_context.value()(resource->resources);
			}
		} catch (std::exception& e) {
			// cleanup
			resource->exit(e);
			return;
		}
		resource->exit(std::nullopt);
		
	}

};

//********************************************************

template <class data>
With IResource<data>::operator()(const std::optional<std::function<void(IData*)>>& code_block){
	return With(std::move(code_block), this);

}

};

/********************************************************
	Example usage
*********************************************************
	With {
		Resource(IData struct) + Context{
	
			[&](auto resource){
				...code
			}
		}
	};

*********************************************************/

#endif