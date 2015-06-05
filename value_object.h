/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/
#ifndef VALUE_OBJECT_H
#define VALUE_OBJECT_H

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/shared_ptr.hpp>

// Experimenting with boost serialization API
// Constructing small example mimicking requirements for Fuse
// Starting with a base value object and number of dervied objects
// Common usage requirment is serializing these polymorphic value objects
// 
// 1. On the sender side we either have  base shared  pointer to derived objects or
// derived shared pointer. In either case the derived objects should be serialized.
// On the receiver side the objects may either be deserialized into base shared_ptr
// or derived shared_ptr.
// 2. We might have a requirement to serialize a collection of shared_ptr
// Collection could be map/list/set

//! Base class
class ValueObject {
public:
  ValueObject() { }
  ValueObject(const ValueObject& that);
  virtual std::string str() = 0;

  // Serialization method required by boost serialization API
  // All derived objects must also implement this method
  // Template argument restricts this method from being polymorphic
  // Template argument could be instantiated to output and input archives
  // but a strong reason to keep it templated is to have a single method for
  // serialization and deserialization
  // Also not all derived value objects need require serialization
  // If a derived object was serialized using an archive::operator <</>> and if
  // the derived object does not implement this method it would result in compilation
  // error anyway and therefore this method is not required to be polymorphic. 
  // Also, all serializable objects should have a default constructor
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
  }
  virtual ~ValueObject() { }
};

typedef boost::shared_ptr<ValueObject> ValueObjectPtr;

class CPValueObject : public ValueObject {
  int value;
public:
  CPValueObject() { }
  CPValueObject(int _value) : ValueObject(), value(_value) { }
  CPValueObject(const CPValueObject& that) : ValueObject(that), value(that.value) { }
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & boost::serialization::base_object<ValueObject>(*this);
    ar & value;
  }
  std::string str();
};

typedef boost::shared_ptr<CPValueObject> CPValueObjectPtr;

class DotValueObject : public ValueObject {
  std::string dotid;
public:
  DotValueObject() { }
  DotValueObject(std::string _dotid) : ValueObject(), dotid(_dotid) { }
  DotValueObject(const DotValueObject& that) : ValueObject(that), dotid(that.dotid) { }

  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<ValueObject>(*this);
    ar & dotid;
  }
  std::string str();
};

typedef boost::shared_ptr<DotValueObject> DotValueObjectPtr;


#endif
