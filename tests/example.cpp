#include <contextual.h>
#include <algorithm>
#define eval [&](auto resource)

using namespace Contextual;

namespace Contextual {

	struct Data {
		std::string username;
		std::string password;
		bool logged_in = false;
	};



	class Resource : public IResource<Data> {
	private:
		std::string _password = "xxxxxxxxx";

		void enter() override {
			std::swap(_password, resources->password);
		}

		void exit(std::optional<std::exception> e) override {
			std::swap(_password, resources->password);
			resources->logged_in = true;

		}
	public:

		Resource(Data &resources): IResource<Data>(resources){};

	};
};



int main(){
	Data first_user{"admin", "password123"};
	std::cout << "Logged in: " << first_user.logged_in << "\n\n";
	With PasswordHidden {
		Resource(first_user) + Context {
			eval {
				std::cout << "Username: " << resource->username << "\n";
				std::cout << "Password: " << resource->password << "\n";
				resource->logged_in = true;
			}
		}
	};




	std::cout << "Username: " << first_user.username << "\n";
	std::cout << "Password: " << first_user.password << "\n";
	std::cout << "Logged in: " << first_user.logged_in << "\n";
}