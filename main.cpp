#include <iostream>
#include <fstream>
#include "visitInfo.pb.h"

enum OS
{
	LINUX = 0,
	MACOS = 1,
	WINDOWS = 2,
	NULL_OS = 3
};

#if defined(_WIN32)
	#define PLATFORM_NAME OS::WINDOWS // Windows
#elif defined(_WIN64)
	#define PLATFORM_NAME OS::WINDOWS // Windows
#elif defined(__linux__)
	#define PLATFORM_NAME OS::LINUX // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#define PLATFORM_NAME OS::NULL_OS // Apple iOS
	#elif TARGET_OS_IPHONE == 1
		#define PLATFORM_NAME OS::NULL_OS // Apple iOS
	#elif TARGET_OS_MAC == 1
		#define PLATFORM_NAME OS::MACOS // Apple OSX
	#endif
#else
	#define PLATFORM_NAME OS::NULL_OS
#endif

using namespace::std;
void fillTheVisit(auth::AuthUser* user)
{
    cout<< "Enter name of person: ";
    getline(cin, *user->mutable_name());
	cout<<"Enter browser: ";
    getline(cin, *user->mutable_browser());

    while(true) 
    {
        cout<<"Enter ip (or blank to finish): ";
        string ipes;
        getline(cin, ipes);
        if(ipes.empty())
        {
            break;
        }
    
		user->add_ip(ipes);

		switch (PLATFORM_NAME) {
		case OS::WINDOWS:
			user->set_system(auth::AuthUser::WINDOWS);
			break;
		case OS::LINUX:
			user->set_system(auth::AuthUser::LINUX);
			break;
		case OS::MACOS:
			user->set_system(auth::AuthUser::MACOS);
			break;
		case OS::NULL_OS:
			user->set_system(auth::AuthUser::NULL_OS);
			break;
		default:
			user->set_system(auth::AuthUser::NULL_OS);
			break;
		}
	}
}
// Main function:  Reads the entire auth list from a file,
//   adds one user based on user input, then writes it back out to the same
//   file.

int main(int argc, char* argv[]) {
	GOOGLE_PROTOBUF_VERIFY_VERSION;

//	if (argc != 2) {
//		cerr << "Usage:  " << argv[0] << " AUTH_FILE" << endl;
//		return -1;
//	}

	auth::AuthCheck auth_check;

	{
		// Read the existing auth list.
		fstream input(argv[1], ios::in | ios::binary);
		if (!input) {
			cout << argv[1] << ": File not found.  Creating a new file." << endl;
		}
		else if (!auth_check.ParseFromIstream(&input)) {
			cerr << "Failed to parse auth list." << endl;
			return -1;
		}
	}

	// Add an address.
	fillTheVisit(auth_check.add_users());

	{
		// Write the new auth list back to disk.
		fstream output(argv[1], ios::out | ios::trunc | ios::binary);
		if (!auth_check.SerializeToOstream(&output)) {
		  cerr << "Failed to write auth list." << endl;
		  return -1;
		}
	}

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

