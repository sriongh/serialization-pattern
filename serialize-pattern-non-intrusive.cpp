#include <iostream>
#include <string>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Lattice;

// forward declaration
namespace boost {
    namespace serialization {
        template <class Archive>
        void serialize(Archive&, Lattice*, const unsigned int=0);
    }
}

class Lattice
{
    int a;
public:
    Lattice() {}
    Lattice(int _a)  {a = _a; }
    void str(std::string indent=" ")
    {
        std::cout << "val :" << indent << a << std::endl;
    }
    template <class Archive>
    friend void boost::serialization::serialize(Archive&, Lattice*, const unsigned int version=0);    
};

// non-intrusive serialization
namespace boost {
    namespace serialization {
        template <class Archive>
        void serialize(Archive& ar, Lattice* lattice, const unsigned int version=0)
        {
            ar & lattice->a;
        }
    }
}

int main()
{
    Lattice *wlattice = new Lattice(14);
    Lattice *rlattice = new Lattice();

    std::stringstream w_str;
    // serialize wlattice into w_str
    boost::archive::text_oarchive oarchive(w_str);
    boost::serialization::serialize(oarchive, wlattice);
    rlattice->str();

    // deserialize wlattice from w_str to r_lattice
    boost::archive::text_iarchive iarchive(w_str);
    boost::serialization::serialize(iarchive, rlattice);

    rlattice->str();

    return 0;
}
