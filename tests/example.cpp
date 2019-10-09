#include <contextual.h>
#include <algorithm>
#define eval [&](auto resource)

struct Data {
	std::string username;
	std::string password;
};

//typedef Context BaseContext<Data>;

class Resource : public BaseResource {
private:
	std::string _password = "xxxxxxxxx";

	void enter() override {
		
		std::swap(_password, resources->password);
	}

	void exit(std::optional<std::exception> e) override {
		std::swap(_password, resources->password);

	}
public:

	Resource(Data resources): BaseResource::BaseResource(&resources){};
};


int main(){
	Data first_user{"admin", "password123"};
	With {
		Resource(first_user) + Context {
			eval {
				std::cout << "Username: " << resource->username << "\n";
				std::cout << "Password: " << resource->password << "\n";
			}
		}
	};


	std::cout << "Username: " << first_user.username << "\n";
	std::cout << "Password: " << first_user.password << "\n";
}