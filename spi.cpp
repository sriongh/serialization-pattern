#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace boost {
    namespace serialization {
        class access;
    }
}

class Lattice
{
public:
    int _ival;
    Lattice() { }
    Lattice(int ival) : _ival(ival) { }

    friend class boost::serialization::access;

    // NOTE: cannot be made virtual due to templated definition for serialize
    template <class Archive>
    void serialize(Archive& ar, const unsigned version = 0)
    {
        ar & _ival;
    }

    virtual void str(std::string indent=" ")
    {
        std::cout << "val:" << indent << _ival << std::endl;
    }
};

class FiniteLattice: public Lattice
{
public:
    float _fval;
    FiniteLattice() { }
    FiniteLattice(int ival, float fval) : Lattice(ival), _fval(fval) { }

    friend class boost::serialization::access;

    // NOTE: cannot be made virtual due to templated definition for serialize
    template <class Archive>
    void serialize(Archive& ar, const unsigned version = 0)
    {
        ar & _ival & _fval;
    }

    virtual void str(std::string indent=" ")
    {
        std::cout << "val:" << indent << this->_ival << "," << indent << _fval << std::endl;
    }

};

int main()
{
    // objects to serialize
    Lattice* bw_lattice = new Lattice(10);
    Lattice* dw_lattice = new FiniteLattice(13, 15.2);
    // FiniteLattice* dw_lattice = new FiniteLattice(13, 15.2);

    // objects to de-serialize into
    Lattice* br_lattice = new Lattice();
    Lattice* dr_lattice = new FiniteLattice();

    // stream to serialize/de-serialize
    std::stringstream rw_stream;
    boost::archive::text_oarchive oarchive(rw_stream);

    // serialization
    // serialize method for Lattice will be generated
    oarchive & *bw_lattice & *dw_lattice;

    std::cout << rw_stream.str() << std::endl; // invoked the base class serialize method

    // only base class properties are deserialized due to object slicing
    boost::archive::text_iarchive iarchive(rw_stream);

    // de-serialization
    iarchive & *br_lattice & *dr_lattice;

    br_lattice->str();
    dr_lattice->str();
    
    return 0;
}
