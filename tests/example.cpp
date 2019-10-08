#include <contextual.h>
#include <algorithm>

struct Data {
	std::string username;
	std::string password;
};

class Resource : public BaseResource {
public:
	std::string _password = "xxxxxxxxx";

	void enter() override {
		std::swap(_password, resources->password);
	}

	void exit(std::exception& e) override {
		std::swap(_password, resources->password);
	}


	Resource(Data& resources): BaseResource::BaseResource(resources){};
};


int main(){
	Data first_user{"AmbarDas", "hickory76"};
	with PasswordHidden = Resource(first_user) + Context{
		[&](Data* resource){
			std::cout << "Username: " << resource->username << "\n";
			std::cout << "Password: " << resource->password << "\n";
		}
	};

	std::cout << "Username: " << first_user.username << "\n";
	std::cout << "Password: " << first_user.password << "\n";
}