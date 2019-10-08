#include <iostream>
#include <functional>
#include <optional>
#include <exception>
/*

A context manager is a class the manages two entities:
	1. resources -- temporary changes that must be made to execute a certain section of code
	2. a context -- a code block that is to be executed with temporarily held resources

Resources and contexts are given their own classes in this implementation. The Resource class
is responsible for the knowledge of resource acquistion and release. The context is responsible
for knowing the code block to be exectued.

A context manager must then perform the following actions:
	1. Direct the Resource class to acquire resources
	2. Make the resources available to the code block owned by the Context class
	3. Execute the code owned by the context
	4. Perform appropriate exception handling if necessary
	5. Release the resources and cleanup before exiting back to outer scope.



To implement:

	__init__: RAII

	__enter__: Trivial

	__exit__: TODO

*/

class with;
// The struct that will hold the resource data
struct Data;

struct Context {
private:
	std::function<void(Data*)> code_block;
	friend class with;
public:
	Context(std::function<void(Data*)> code_block): code_block(std::move(code_block)){};

};


// The resource manager class interface
class BaseResource {
public:
	friend class with;

	// The actual resources
	
	virtual void enter() = 0;
	virtual void exit(std::exception& e) = 0;


	Data* resources;
	BaseResource(): resources(nullptr){};
	BaseResource(Data& resources) : resources(&resources){};
	with operator+(const Context& context);
};

class with {
private:
	const Context* _context = nullptr;

public:
	// TODO: Make generic with type checking
	BaseResource* resource=nullptr;
	// The rule of five
	with() = delete;
	with(const with& other){
		try{
			// Execute the context
			other.resource->enter();
			other._context->code_block(other.resource->resources);
		} catch (std::exception& e) {
			// cleanup
			delete other._context;
			// TODO : Should this be done?
			try{
				// perform final user specifie actions
				other.resource->exit(e);
			} catch (std::exception& e) {
				// if use allows exception to propogate,
				// make sure resource object is deleted
				delete other.resource;
				throw e;
			}
				// if user suppresses all exceptions, make
				// sure that resource object is deleted
				delete other.resource;
		}
	}
	
	with& operator=(const with& other){
		try{
			// Execute the context
			other.resource->enter();
			other._context->code_block(other.resource->resources);
		} catch (std::exception& e) {
			// cleanup
			delete other._context;
			// TODO : Should this be done?
			try{
				// perform final user specifie actions
				other.resource->exit(e);
			} catch (std::exception& e) {
				// if use allows exception to propogate,
				// make sure resource object is deleted
				delete other.resource;
				throw e;
			}
				// if user suppresses all exceptions, make
				// sure that resource object is deleted
				delete other.resource;
		}
	}

	with operator=(const with&& other){
		
		try{
			// Execute the context
			other.resource->enter();
			other._context->code_block(other.resource->resources);
		} catch (std::exception& e) {
			// cleanup
			delete other._context;
			// TODO : Should this be done?
			try{
				// perform final user specifie actions
				other.resource->exit(e);
			} catch (std::exception& e) {
				// if use allows exception to propogate,
				// make sure resource object is deleted
				delete other.resource;
				throw e;
			}
				// if user suppresses all exceptions, make
				// sure that resource object is deleted
				delete other.resource;
		}
	}

	~with() = default;

	with(BaseResource* resource, const Context* context): resource(resource),
													_context(context) {};

};

with BaseResource::operator+(const Context& context){
	return with(this, &context);

}

/*
	Example usage

	with ContextManager = Resource(data struct) + Context{
	
		[&](auto resource){
			...code
		}
	} ;

*/