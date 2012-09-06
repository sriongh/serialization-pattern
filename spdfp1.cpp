#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

class UniqueKeyGen
{
    static int key;
public:
    static int getUniqueID()
    {
        return UniqueKeyGen::key++;
    }
};

int UniqueKeyGen::key = 0;

class serializable
{
public:
    typedef boost::archive::text_oarchive OutArchive;
    typedef boost::archive::text_iarchive InArchive;


    virtual void serialize(OutArchive& archive, const unsigned int version = 0) = 0;
    virtual void serialize(InArchive& archive, const unsigned int version = 0) = 0;
};

class Lattice : public serializable
{
public:
    virtual std::string str(std::string indent=" ") = 0;
    virtual Lattice* copy() = 0;
};

class MemoryObject : public serializable
{
public:
    int key;
    MemoryObject() { key = -1; }
    MemoryObject(int _key) : key(_key) { }
    MemoryObject(const MemoryObject& that) { this->key = that.key; }
    bool operator< (const MemoryObject& that) { return this->key < that.key; }

    void serialize(OutArchive& archive, const unsigned int version = 0)
    {
        archive << key;
    }

    void serialize(InArchive& archive, const unsigned int version = 0)
    {
        archive >> key;
    }
};


class ProductLattice : public Lattice
{
    int size;
public:
    std::map<MemoryObject*, Lattice*> productlattice;    

    ProductLattice() { }
    ProductLattice(const ProductLattice& that)
    {
        this->productlattice = that.productlattice;
    }

    Lattice* copy()
    {
        return new ProductLattice(*(dynamic_cast<ProductLattice*>(this)));
    }

    void init(Lattice* perVarLattice, int size)
    {
        this->size = size;
        for(int i = 0; i < size; i++) {
            MemoryObject* _mobj = new MemoryObject(UniqueKeyGen::getUniqueID());
            Lattice* varLattice = perVarLattice->copy();
            productlattice.insert(std::pair<MemoryObject*, Lattice*> (_mobj, varLattice));
        }
    }

    // NOTE: map should be serialized
    void serialize(OutArchive& archive, const unsigned int version = 0)
    {

    }

    void serialize(InArchive& archive, const unsigned int version = 0)
    {

    }


    std::string str(std::string indent=", ")
    {
        std::stringstream ss;
        std::map<MemoryObject*, Lattice*>::iterator it;
        for(it = productlattice.begin(); it != productlattice.end(); it++) {
            ss << ((*it).first)->key << indent << ((*it).second)->str();
        }
        return ss.str();
    }
};

class IntLattice: public Lattice
{
public:
    int val;
    IntLattice() { }
    IntLattice(int _that) : Lattice(), val(_that) { }

    IntLattice(const IntLattice& that)
    {
        this->val = that.val;
    }

    Lattice* copy()
    {
        return new IntLattice(*dynamic_cast<IntLattice*>(this));
    }

    void serialize(OutArchive& archive, const unsigned int version = 0)
    {
        archive << val;
    }

    void serialize(InArchive& archive, const unsigned int version = 0)
    {
        archive >> val;
    }

    std::string str(std::string indent=" ")
    {
        std::stringstream ss;
        ss << "val:" << indent << val << std::endl;
        return ss.str();
    }
};

class FloatLattice: public Lattice
{
public:
    float fval;
    FloatLattice() { }
    FloatLattice(float _that) : fval(_that) { }
    FloatLattice(const FloatLattice& that)
    {
        this->fval = that.fval;
    }

    Lattice* copy()
    {
        return new FloatLattice(*dynamic_cast<FloatLattice*>(this));
    }

    void serialize(OutArchive& archive, const unsigned int version = 0)
    {
        archive << fval;
    }

    void serialize(InArchive& archive, const unsigned int version = 0)
    {
        archive >> fval;
    }

    std::string str(std::string indent=" ")
    {
        std::stringstream ss;
        ss << "fval:" << indent << fval << std::endl;
        return ss.str();
    }
};


int main()
{
    std::stringstream rw_stream;
    // Lattice* lattice = new IntLattice(10);
    // Lattice* flattice = new FloatLattice(2.1);

    // Lattice::OutArchive out_archive(rw_stream);
    // lattice->serialize(out_archive);
    // flattice->serialize(out_archive);
   
    // std::cout << rw_stream.str() << std::endl;

    // Lattice::InArchive in_archive(rw_stream);
    // Lattice* rilattice = new IntLattice();
    // Lattice* rflattice = new FloatLattice();

    // rilattice->deserialize(in_archive);
    // rflattice->deserialize(in_archive);
    // dynamic_cast<FloatLattice*>(rflattice)->str();

    ProductLattice *pLattice = new ProductLattice();
    pLattice->init(new IntLattice(10), 10);
    serializable::OutArchive out_archive(rw_stream);
    pLattice->serialize(out_archive);
    std::cout << rw_stream.str() << std::endl;

    return 0;
}
