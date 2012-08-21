#include <iostream>
#include <string>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class serializable
{
public:
    typedef boost::archive::text_oarchive OutArchive;
    typedef boost::archive::text_iarchive InArchive;


    virtual void serialize(OutArchive& archive, const unsigned int version = 0) = 0;
    virtual void deserialize(InArchive& archive, const unsigned int version = 0) = 0;
};

class Lattice : public serializable
{
public:
    virtual void str(std::string indent=" ") = 0;
};

class IntLattice: public Lattice
{
public:
    int val;
    IntLattice() { }
    IntLattice(int _that) : Lattice(), val(_that) { }

    void serialize(OutArchive& archive, const unsigned int version = 0)
    {
        archive << val;
    }

    void deserialize(InArchive& archive, const unsigned int version = 0)
    {
        archive >> val;
    }

    void str(std::string indent=" ")
    {
        std::cout << "val:" << indent << val << std::endl;
    }
};

class FloatLattice: public Lattice
{
public:
    float fval;
    FloatLattice() { }
    FloatLattice(float _that) : fval(_that) { }

    void serialize(OutArchive& archive, const unsigned int version = 0)
    {
        archive << fval;
    }

    void deserialize(InArchive& archive, const unsigned int version = 0)
    {
        archive >> fval;
    }

    void str(std::string indent=" ")
    {
        std::cout << "fval:" << indent << fval << std::endl;
    }
};


int main()
{
    std::stringstream rw_stream;
    Lattice* lattice = new IntLattice(10);
    Lattice* flattice = new FloatLattice(2.1);

    Lattice::OutArchive out_archive(rw_stream);
    lattice->serialize(out_archive);
    flattice->serialize(out_archive);
   
    std::cout << rw_stream.str() << std::endl;

    Lattice::InArchive in_archive(rw_stream);
    Lattice* rilattice = new IntLattice();
    Lattice* rflattice = new FloatLattice();

    rilattice->deserialize(in_archive);
    rflattice->deserialize(in_archive);
    dynamic_cast<FloatLattice*>(rflattice)->str();
    return 0;
}
