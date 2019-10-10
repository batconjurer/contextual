#include <contextual.h>
#include <stdexcept>

using namespace Contextual;


namespace Contextual {

	struct IData {
		std::string username;
		std::string password;
		bool logged_in = false;
	};



	class Resource : public IResource<IData> {
	private:
		std::string _password = "xxxxxxxxx";
		IData _data;
		bool hide=true;
		bool reraise = false;
		void enter() override {
			if (hide){
				std::swap(_password, resources->password);
			}
		}

		void exit(std::optional<std::exception> e) override {
			if (hide) {
				std::swap(_password, resources->password);
			}
			resources->logged_in = true;
			if (reraise) {
				throw e;
			}

		}
	public:

		Resource(IData &resources, bool hide=true, bool reraise=false): IResource<IData>(resources),
																  	    hide(hide){

		};
		Resource(IData &&resources): IResource<IData>(&resources){};
		Resource(std::string username, std::string password) : _data(IData{username, password}),
		 													   IResource<IData>(_data){};
	


	};



	class _With : public With {
		~_With(){
			
		}
	}
};


TEST_CASE("Test core functionality", "[core-functionaliy]"){

	IData data{"admin", "password123"};

	SECTION("Test resource acquisition"){
		With {
			Resource(data, false) + Context{
				[&](auto resource){
					REQUIRE(resource->username == "admin");
					REQUIRE(resource->password == "password123");
				}
			}
		};

	}

	SECTION("Test resource release normal"){
		With {
			Resource(data) + Context{
				[&](auto resource){
					REQUIRE(resource->username == "admin");
					REQUIRE(resource->password == "xxxxxxxxx");
				}
			}
		};

		REQUIRE(data.username == "admin");
		REQUIRE(data.password == "password123");
	}

	SECTION("Test resource release with exceptions"){
		With {
			Resource(data) + Context{
				[&](auto resource){
					REQUIRE(resource->username == "admin");
					REQUIRE(resource->password == "xxxxxxxxx");
					throw std::runtime_error("");
				}
			}
		};

		REQUIRE(data.username == "admin");
		REQUIRE(data.password == "password123");	
	}

	SECTION("Test side-effects"){
		REQUIRE(data.logged_in == false);
		With {
			Resource(data) + Context{
				[&](auto resource){
					resource->logged_in = true;
				}
			}
		};

		REQUIRE(data.logged_in == true);
	}

	SECTION("Test that code block can access variables in outer scope"){
		int GLOBAL_VAR = 0;
		
		With {
			Resource(data) + Context{
				[&](auto resource){
					++GLOBAL_VAR;
				}
			}
		};		
		REQUIRE(GLOBAL_VAR == 1);
	}

}

TEST_CASE("Test cleanup", "[cleanup]"){

	SECTION("Test destructors are called in anticipated use case"){
		REQUIRE(true);
	}

	SECTION("Test destructors are called even with trying to store with object"){
		REQUIRE(true);
	}

	SECTION("Test destructors are called even with exceptions"){
		REQUIRE(true);
	}
}

TEST_CASE("Test exception logic", "[exceptions]"){

	SECTION("Test exception suppression"){
		REQUIRE(true);
	}

	SECTION("Test exception propogation"){
		REQUIRE(true);
	}
}

TEST_CASE("Test syntax features", "[syntax]"){

	SECTION("Test nameless 'With' block"){
		REQUIRE(true);
	}
	
	SECTION("Test named 'With' block"){
		REQUIRE(true);
	}

	SECTION("Test IData emplace"){
		REQUIRE(true);
	}

	SECTION("Test internal IData construction"){
		REQUIRE(true);
	}

	SECTION("Test pointer initialization"){
		REQUIRE(true);
	}


}