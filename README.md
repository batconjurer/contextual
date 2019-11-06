![](other/logo.png)

# A context manager framework for C++

While C++ has traditionally done context managing via RAII and carefully scoping objects to ensure destructors are properly called, there are situations where context managing a la Python is more suitable. This header-only library provides for a Python-like context manager.

## Requirements

This library requires C++17 or greater. It was built and tested on Linux using g++; compatibility with other operating systems and compilers has not been confirmed.

## What is a context managaer?

To use this library properly it is important to understand what we are trying to acheive, what entities are involved, and their individual responsibilites as well as interactions.

Loosely speaking, a context manager is meant to provide a certian code block with temporary resources or changes that will be reverted or released at the end of the code block. This cleanup should be done even in the event of exceptions or unanticipated flows.

**Context:** This is the code block for which the temporary changes must be enabled. After the context goes out of scope, the cleanup of these changes must occur.

**Resource Manager:** This is an object that knows the logic of how to acquire and release the necessary resources as well as user-defined exception handling. It is this object that users define in Python. It has initiliazation, enter, and exit methods that must be user specified by inheriting from an interface.

**Data:** Self-explanatory, but this is a struct with the acquired resources that is provided to the context. It's manipulation is handled by the resource manager.

**Context Manager:** An object that uses the enter and exit functions of the Resource manager to provide the resources to the code block held by the context and to ensure cleanup and exception handling.

## How does it work?

A context manager must be given a resource manager plus a context. This is directly reflected in the syntax. The context manager in contextual is a class called `With`.

The resource manager must be implemented by the user. Contextual provides an templated interface called `IResource` with virtual enter and exit methods:

```c++
virtual void IResource<data>::enter() = 0;
virtual void IResource<data>::exit(std::optional<std::exception> e) = 0;
```
The user should create a derived class from this interface that specifies the logic they need for their usecase. 

The resources will be collected into a struct called `IData`. This struct is undefined in the header so that the user can define it however they wish. Ultimately, a pointer to this struct will be made accessible to the context. *Therefore, the users resource manager class must be derived from* `public IResource<IData>`.

Initialization `IResource` supports refernce and pointer passing to give some flexibility to the user. These methods can be overridden to provide extra logic.
```c++
IResource<data>() = default;
IResource<data>(data& resources) : resources(&resources){};
IResource<data>(data* resources) : resources(resources){};
```

Contexts are very simple in contextual. They are simply lambda functions passed given to the resource manager that forwards it on to the context manager. This lambda function must satisfy the type: 
```c++ 
std::function<void(IData*)> code_block;
```
The following gives an example of the usage
```c++
using namespace Contextual;
namespace Contextual{

    struct IData {
        ...
    };

    class Resource : public IResource<IData> {
        ...
    };
};

int main(){
    
    IData resource = ...;

    With OptionalName {
        Resource(resource)(
            [&](IData* data) {
                ...
            }
        )
    };
}
```

In the above, the `With` block can be a given a name (the `OptionalName` above), but this is not necessary. Also, a name cannot be repeated in the same scope (anonymous scoping can resolve this). However, naming the `With` block is discouraged as it leaves this instance in memory until it leaves scope, unlike unnamed blocks, which are destroyed immediately (again this is fixable with anonymous scoping).

In the lambda function, the `data` parameter is a pointer to the resource struct created by the Resource class via initialization / enter methods. At the end of the block, the exit method of `Resource` is called. Then the `Resource`, and `With` objects all leave scope and have their destructors called.

For more examples, see the **example.cpp** file in the top-level directory. It can be compiled by running `$ make example`. The tests in  **tests/test\_contextual\_basic.h** provide some further examples.

## Problematic usages

At the moment, there are a few ways to use this library that will have negative consequences (other than obvious abuses like overriding all core functionality in derived classes).

The problematic usages all have in common that one of necessary destructors does not get called. Instnaces of the classes `With` and `IResource` are meant to be temporary. They should not be forward declared, copied, or assigned unless a very particular use case demands it. 

Less obviously, if a `With` block is given a name, it's destructor will not be called at the end of the context. Therefore, if one wishes to use the optional name, the entire `With` block should be placed in an anonymous scope to make sure it does not continue consuming memory. 

Lastly, the `IResource` class only keeps a pointer to the data passed into its constructor. In order not to cause it to dangle, the data passed into constructor should be stored somewhere. If it is meant to be temporary, a good place is inside of the user define class inheriting from `IResource` as it will be dealllocated at the end of the `With` block.