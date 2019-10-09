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

class With;
// The struct that will hold the resource Basedata
struct Data;

//template <struct Data>
struct Context {
private:
	std::function<void(Data*)> code_block;
	friend class With;
public:
	Context(std::function<void(Data*)> code_block): code_block(std::move(code_block)){};
	
};


// The resource manager class interface
//template <struct Data>
class BaseResource {
private:
	std::optional<Context> ctxt = std::nullopt;
public:
	friend class With;

	// The actual resources
	
	virtual void enter() = 0;
	virtual void exit(std::optional<std::exception> e) = 0;


	Data* resources;
	BaseResource() = default;
	BaseResource(Data* resources) : resources(resources){
		
	};
	With operator+(const Context& context);
	
};

class With {
private:
	Context* _context = nullptr;

	void _run(){
		try{
			
			// Execute the context
			resource->enter();
			_context->code_block(resource->resources);
			
		} catch (std::exception& e) {
			// cleanup
			resource->exit(e);
			return;
		}
		resource->exit(std::nullopt);
	}

public:
	// TODO: Make generic with type checking
	BaseResource* resource=nullptr;
	// The rule of five
	With() = delete;
	With(const With& other) = delete;
	With& operator=(const With& other) = delete;
	With& operator=(const With&& other) = delete;

	~With() = default;

	With(BaseResource* resource, Context* context): resource(resource),
													_context(context) {
														_run();
													};

};

With BaseResource::operator+(const Context& context){
	ctxt = context;
	return With(this, &ctxt.value());

}

/*
	Example usage

	with {
		Resource(Basedata struct) + Context{
	
			[&](auto resource){
				...code
			}
		}
	};

*/