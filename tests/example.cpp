#include <contextual.h>
#include <algorithm>
#include <memory>
#define eval [&](auto resource)

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
		void enter() override {
			std::swap(_password, resources->password);
		}

		void exit(std::optional<std::exception> e) override {
			std::swap(_password, resources->password);
			resources->logged_in = true;

		}
	public:

		Resource(IData &resources): IResource<IData>(resources){};
		Resource(IData &&resources): IResource<IData>(&resources){};
		Resource(std::string username, std::string password) : _data(IData{username, password}),
		 													   IResource<IData>(_data){};
	


	};
};
	

int main(){
	IData first_user{"admin", "password123"};
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

	std::cout << "\n====================================\n\n";

	With {
		Resource("admin", "password123") + Context {
			eval {
				std::cout << "Username: " << resource->username << "\n";
				std::cout << "Password: " << resource->password << "\n";
				resource->logged_in = true;
			}
		}
	};

}