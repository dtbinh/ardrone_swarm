/* Auto-generated by genmsg_cpp for file /home/futuhal57/ros/stacks/hector_common/hector_std_msgs/msg/UInt16Array.msg */
#ifndef HECTOR_STD_MSGS_MESSAGE_UINT16ARRAY_H
#define HECTOR_STD_MSGS_MESSAGE_UINT16ARRAY_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "std_msgs/Header.h"

namespace hector_std_msgs
{
template <class ContainerAllocator>
struct UInt16Array_ {
  typedef UInt16Array_<ContainerAllocator> Type;

  UInt16Array_()
  : header()
  , data()
  {
  }

  UInt16Array_(const ContainerAllocator& _alloc)
  : header(_alloc)
  , data(_alloc)
  {
  }

  typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
   ::std_msgs::Header_<ContainerAllocator>  header;

  typedef std::vector<uint16_t, typename ContainerAllocator::template rebind<uint16_t>::other >  _data_type;
  std::vector<uint16_t, typename ContainerAllocator::template rebind<uint16_t>::other >  data;


  typedef boost::shared_ptr< ::hector_std_msgs::UInt16Array_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::hector_std_msgs::UInt16Array_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct UInt16Array
typedef  ::hector_std_msgs::UInt16Array_<std::allocator<void> > UInt16Array;

typedef boost::shared_ptr< ::hector_std_msgs::UInt16Array> UInt16ArrayPtr;
typedef boost::shared_ptr< ::hector_std_msgs::UInt16Array const> UInt16ArrayConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::hector_std_msgs::UInt16Array_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::hector_std_msgs::UInt16Array_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace hector_std_msgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::hector_std_msgs::UInt16Array_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::hector_std_msgs::UInt16Array_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::hector_std_msgs::UInt16Array_<ContainerAllocator> > {
  static const char* value() 
  {
    return "2b9446334eb40dbaedf25a747be49905";
  }

  static const char* value(const  ::hector_std_msgs::UInt16Array_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x2b9446334eb40dbaULL;
  static const uint64_t static_value2 = 0xedf25a747be49905ULL;
};

template<class ContainerAllocator>
struct DataType< ::hector_std_msgs::UInt16Array_<ContainerAllocator> > {
  static const char* value() 
  {
    return "hector_std_msgs/UInt16Array";
  }

  static const char* value(const  ::hector_std_msgs::UInt16Array_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::hector_std_msgs::UInt16Array_<ContainerAllocator> > {
  static const char* value() 
  {
    return "Header header\n\
uint16[] data\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.secs: seconds (stamp_secs) since epoch\n\
# * stamp.nsecs: nanoseconds since stamp_secs\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
";
  }

  static const char* value(const  ::hector_std_msgs::UInt16Array_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct HasHeader< ::hector_std_msgs::UInt16Array_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct HasHeader< const ::hector_std_msgs::UInt16Array_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::hector_std_msgs::UInt16Array_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.header);
    stream.next(m.data);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct UInt16Array_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::hector_std_msgs::UInt16Array_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::hector_std_msgs::UInt16Array_<ContainerAllocator> & v) 
  {
    s << indent << "header: ";
s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "data[]" << std::endl;
    for (size_t i = 0; i < v.data.size(); ++i)
    {
      s << indent << "  data[" << i << "]: ";
      Printer<uint16_t>::stream(s, indent + "  ", v.data[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // HECTOR_STD_MSGS_MESSAGE_UINT16ARRAY_H

