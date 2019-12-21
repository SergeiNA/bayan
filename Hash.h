#pragma once
#include <string>
#include <map>
#include <vector>

#include <boost/function.hpp>
#include <boost/functional/factory.hpp>

#include <boost/uuid/sha1.hpp>
#include <boost/functional/hash.hpp>
#include <boost/crc.hpp>

#include "Common.h"

class Hash{
public:
    virtual Hash_value compute(const std::vector<char>& block) = 0;
};

class BoostHash : public Hash{
public:
    Hash_value compute(const std::vector<char>& block) override{
        return std::to_string(boost_hasher(block));
    }

private:
    boost::hash<std::vector<char>> boost_hasher;
};

class CRCHasher : public Hash{
public:
    Hash_value compute(const std::vector<char>& block) override{
        boost::crc_32_type crc_hash;
        crc_hash.process_bytes(block.data(), block.size());
        return std::to_string(crc_hash.checksum());
    }
};

class SHA1Hash : public Hash{
public:
    Hash_value compute(const std::vector<char> &block) override{
        unsigned int res[5];
        boost::uuids::detail::sha1 sha1_hash;
        sha1_hash.process_bytes(block.data(), block.size());
        sha1_hash.get_digest(res);
        std::string result;
        result.reserve(5);
        for (size_t i = 0; i < 5; i++)
        {
            result.append(std::to_string(res[i]));
        }

        return result;
    }
};

class HashFactory {
private:
    HashType hashType;
    std::map<HashType, boost::function<Hash *()>> factory;

public:
    HashFactory(HashType hash_alg) : hashType{hash_alg}
    {
        factory[HashType::BOOST] = boost::factory<BoostHash *>();
        factory[HashType::CRC] = boost::factory<CRCHasher *>();
        factory[HashType::SHA1] = boost::factory<SHA1Hash *>();
    }
    std::unique_ptr<Hash> createHash()
    {
        std::unique_ptr<Hash> ptr(factory[hashType]());
        return ptr;
    }
};

inline std::map<std::string, HashType> hashTypeSelector{
    {"boost",HashType::BOOST},
    {"crc",HashType::CRC},
    {"sha1",HashType::SHA1}
};