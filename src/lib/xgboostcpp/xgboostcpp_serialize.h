/*!
  @file   xgboostcpp_serialize.h
  @author David Hirvonen
  @brief  Declaration of serialization routines.

  \copyright Copyright 2014-2016 Elucideye, Inc. All rights reserved.
  \license{This project is released under the 3 Clause BSD License.}

*/

#ifndef __xgboostcpp_serialize_h__
#define __xgboostcpp_serialize_h__

#include <fstream>
#include <assert.h>

enum ArchiveType
{
#if defined(XGBOOSTER_SERIALIZE_WITH_BOOST)    
    kPBA,
#endif
#if defined(XGBOOSTER_SERIALIZE_WITH_CEREAL)
    kCPB,
#endif
    kUnknown
};

inline ArchiveType getType(const std::string &filename)
{
#if defined(XGBOOSTER_SERIALIZE_WITH_BOOST)
    if(filename.find(".pba.z") != std::string::npos)
    {
        return kPBA;
    }
#endif

#if defined(XGBOOSTER_SERIALIZE_WITH_CEREAL)
    if(filename.find(".cpb") != std::string::npos)
    {
        return kCPB;
    }
#endif

    return kUnknown;
}


// #####################
// ####### pba.z #######
// #####################

#if defined(XGBOOSTER_SERIALIZE_WITH_BOOST)

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "boost-pba/portable_binary_oarchive.hpp"
#include "boost-pba/portable_binary_iarchive.hpp"

template <typename T>
void load_pba_z(std::istream &is, T &object)
{
    boost::iostreams::filtering_stream<boost::iostreams::input> buffer;
    buffer.push(boost::iostreams::zlib_decompressor());
    buffer.push(is);
    portable_binary_iarchive ia(buffer);
    ia >> object;
}

template <typename T>
void load_pba_z(const std::string &filename, T &object)
{
    std::ifstream ifs(filename, std::ios::binary);
    assert(ifs); // TODO: throw
    load_pba_z(ifs, object);
}

template <typename T>
void save_pba_z(std::ostream &os, T &object)
{
    boost::iostreams::filtering_stream<boost::iostreams::output> buffer;
    buffer.push(boost::iostreams::zlib_compressor(boost::iostreams::zlib::best_compression));
    buffer.push(os);
    portable_binary_oarchive oa(buffer);
    oa << object;
}

template <typename T>
void save_pba_z(const std::string &filename, T &object)
{
    std::ofstream ofs(filename, std::ios::binary);
    assert(ofs); // TODO: throw
    save_pba_z(ofs, object);
}

#endif // XGBOOSTER_SERIALIZE_WITH_BOOST

// ####################
// ####### cpb ########
// ####################

#if defined(XGBOOSTER_SERIALIZE_WITH_CEREAL)

// http://uscilab.github.io/cereal/serialization_archives.html
#include "xgboostcpp/to_string.h" // android stdlib defs
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

template <typename T>
void load_cpb(std::istream &is, T &object)
{
    cereal::PortableBinaryInputArchive ia(is);
    ia >> object;
}

template <typename T>
void load_cpb(const std::string &filename, T &object)
{
    std::ifstream ifs(filename, std::ios::binary);
    assert(ifs);
    load_cpb(ifs, object);
}

template <typename T>
void save_cpb(std::ostream &os, T &object)
{
    cereal::PortableBinaryOutputArchive oa(os);
    oa << object;
}

template <typename T>
void save_cpb(const std::string &filename, T &object)
{
    std::ofstream ofs(filename, std::ios::binary);
    assert(ofs);
    save_cpb(ofs, object);
}

#endif // XGBOOSTER_SERIALIZE_WITH_BOOST

// ### load model ###
template <typename T>
void load_model(const std::string &filename, T &object)
{
    switch(getType(filename))
    {
#if defined(XGBOOSTER_SERIALIZE_WITH_BOOST)
    case kPBA: load_pba_z(filename, object); break;
#endif
#if defined(XGBOOSTER_SERIALIZE_WITH_CEREAL)
    case kCPB: load_cpb(filename, object); break;
#endif
    default: assert(false);
    }
}

// ### save model ###
template <typename T>
void save_model(const std::string &filename, T &object)
{
    switch(getType(filename))
    {
#if defined(XGBOOSTER_SERIALIZE_WITH_BOOST)
    case kPBA: save_pba_z(filename, object); break;
#endif
#if defined(XGBOOSTER_SERIALIZE_WITH_CEREAL)
    case kCPB: save_cpb(filename, object); break;
#endif
    default: assert(false);            
    }
}

#endif // __xgboostcpp_serialize_h__
