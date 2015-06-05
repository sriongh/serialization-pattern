#include <iostream>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "value_object.h"
#include <boost/make_shared.hpp>
#include <boost/serialization/export.hpp>
#include <list>
#include <boost/serialization/list.hpp>

using namespace std;
using namespace boost;

BOOST_CLASS_EXPORT_GUID(DotValueObject, "dotvalueobject")
BOOST_CLASS_EXPORT_GUID(CPValueObject, "cpvalueobject")

void test_shared_ptr_serialization() {
  // Serialize
  cout << "test_shared_ptr_serialization()" << endl;
  stringstream ss;  
  {
    ::archive::text_oarchive oa(ss);
    // Registering derived class with the out archive
    // oa.register_type<DotValueObject>();

    ValueObjectPtr vo1_p = boost::make_shared<DotValueObject>("d1");
    DotValueObjectPtr dvo1_p = boost::make_shared<DotValueObject>("d2");
    DotValueObjectPtr dvo2_p = boost::make_shared<DotValueObject>("d3");

    // Serialization
    oa << vo1_p << dvo1_p << dvo2_p;

    cout << "Serialized\n" << vo1_p->str() << "\n" << dvo1_p->str() << "\n" << dvo2_p->str() << "\n";
  }
  
  // Deserialize
  {
    ::archive::text_iarchive ia(ss);
    // Registering derived class with in archive
    // ia.register_type<DotValueObject>();

    ValueObjectPtr vo1_p;
    DotValueObjectPtr dvo1_p;
    ValueObjectPtr vo2_p;
    ia >> vo1_p >> dvo1_p >> vo2_p;

    cout << "Deserialized\n" << vo1_p->str() << "\n" << dvo1_p->str() << "\n" << vo2_p->str() << "\n";
  }
}

string print_list(list<CPValueObjectPtr>& _list) {
  ostringstream oss;
  list<CPValueObjectPtr>::iterator li = _list.begin();
  oss << "[";
  for( ; li != _list.end(); ) {
    CPValueObjectPtr cpvo_p = *li;
    oss << cpvo_p->str();
    ++li;
    if(li != _list.end()) oss << ", ";
  }
  oss << "]\n";
  return oss.str();
}

void test_list_serialization() {
  cout << "\ntest_list_serialization()\n"; 
  stringstream ss;
  // Serialization
  {
    list<CPValueObjectPtr> cpValList;
    for(int i=0; i < 5; ++i) {
      CPValueObjectPtr cpvo_p = boost::make_shared<CPValueObject>(i);
      cpValList.push_back(cpvo_p);
    }

    ::archive::text_oarchive oa(ss);
    oa << cpValList;

    cout << "serialized\n" << print_list(cpValList) << endl;
  }

  // Deserialization
  {
    list<CPValueObjectPtr> cpValList;
    cout << "Before deserialization: cpValList.size()=" << cpValList.size() << endl;

    ::archive::text_iarchive ia(ss);
    ia >> cpValList;

    cout << "deserialized\n" << print_list(cpValList) << endl;
  }
}

int main() {
  test_shared_ptr_serialization();
  test_list_serialization();
  return 0;
}
