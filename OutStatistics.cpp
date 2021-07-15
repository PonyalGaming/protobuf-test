#include <iostream>
#include <fstream>
#include <string>
#include "visitStat.pb.h"
using namespace std;

// Iterates though all people in the AddressBook and prints info about them.
void ListVisitings(const auth::AuthCheck& user) {
  for (int i = 0; i < user.users_size(); i++) {
    const auth::AuthUser& person = user.users(i);

    cout << "Person name: " << person.name() << endl;
    cout << "  Browser: " << person.browser() << endl;
    cout << "  system: " << person.system() << endl;

    for (int j = 0; j < person.ip_size(); j++) {
      
      cout << "ip: " << person.ip(j) << endl;
    }
  }
}

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " VISITINGS_FILE" << endl;
    return -1;
  }

  auth::AuthCheck authUser;

  {
    // Read the existing visit list
    fstream input(argv[1], ios::in | ios::binary);
    if (!authUser.ParseFromIstream(&input)) {
      cerr << "Failed to parse visit list." << endl;
      return -1;
    }
  }

  ListVisitings(authUser);

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
 
