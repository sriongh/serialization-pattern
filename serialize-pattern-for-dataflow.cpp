#include <iostream>
#include <string>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

template <class Lattice>
class serializable
{
public:
    typedef boost::archive::text_oarchive OutArchive;
    typedef boost::archive::text_iarchive InArchive;
    Lattice* _lattice;
   
    serializable(Lattice* that) : _lattice(that)
    { 
    }

    template <typename Archive>
    void serialize(Archive& archive, const unsigned int version = 0)
    {
        static_cast<Lattice*>(_lattice)->serialize(archive);
    }

    template <typename Archive>
    void deserialize(Archive& archive, const unsigned int version = 0)
    {
        static_cast<Lattice*>(this)->deseralize(archive);
    }
};

class Lattice : public serializable<Lattice>
{
public:
    int a;
    Lattice() : serializable<Lattice> (this) {}
    Lattice(int _a) : serializable<Lattice> (this)
    {
        a = _a; 
    }

    void str(std::string indent=" ")
    {
        std::cout << "val :" << indent << a << std::endl;
    }

    template <typename Archive>
    void serialize(Archive& archive, const unsigned int version = 0)
    {
        static_cast<Lattice*>(_lattice)->save(archive);
    }

    template<typename Archive>
    void deserialize(Archive& archive, const unsigned int version = 0)
    {
        static_cast<Lattice*>(_lattice)->load(archive);
    }    

    virtual void save(OutArchive& archive, const unsigned int version = 0)
    {
        archive & a;
    }

    virtual void load(InArchive& archive, const unsigned int version = 0)
    {
        archive >> a;
    }
};

class FiniteLattice: public Lattice
{
public:
    float _fa;
    FiniteLattice() { }
    FiniteLattice(float _that) : Lattice(), _fa(_that) { }

    void save(OutArchive& archive, const unsigned int version = 0)
    {
        //static_cast<Lattice*>(_lattice)->save(archive);
        archive & _fa;
    }

    void load(InArchive& archive, const unsigned int version = 0)
    {
        //static_cast<Lattice*>(_lattice)->load(archive);
        archive & _fa;
    }

    void str(std::string indent=" ")
    {
        std::cout << "fval:" << indent << _fa << std::endl;
    }
};


int main()
{
    std::stringstream rw_stream;
    FiniteLattice* flattice = new FiniteLattice(2.1);

    Lattice::OutArchive out_archive(rw_stream);
    flattice->serialize(out_archive);
   
    std::cout << rw_stream.str() << std::endl;

    Lattice::InArchive in_archive(rw_stream);
    Lattice* rflattice = new FiniteLattice();

    rflattice->deserialize(in_archive);
    dynamic_cast<FiniteLattice*>(rflattice)->str();
    return 0;
}
