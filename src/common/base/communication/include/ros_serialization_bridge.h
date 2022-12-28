#ifndef __ROS_SERIALIZATION_BRIDGE_H__
#define __ROS_SERIALIZATION_BRIDGE_H__

#include "ros/ros.h"
#include "ros/serialization.h"
#include "std_msgs/UInt8.h"
#include <vector>

class RosSerializationBridge
{
public:
	RosSerializationBridge();
	~RosSerializationBridge();

	template<class T>
	static int Serialization(std::vector<uint8_t> &buffer, const T &data) {
		int serial_size = ros::serialization::serializationLength(data);
		buffer.resize(serial_size);
		ros::serialization::OStream stream((uint8_t *)&buffer[0], serial_size);
		ros::serialization::serialize(stream, data);
		return serial_size;
	}

	template<class T>
	static int Serialization(uint8_t *buffer, int buffer_len, const T &data) {
		int serial_size = ros::serialization::serializationLength(data);
		if (serial_size >= buffer_len) {
			std::cout << "error buffer is not meet command please check " << serial_size << "|" << buffer_len << std::endl;
			exit(0);
		}
		ros::serialization::OStream stream(buffer, serial_size);
		ros::serialization::serialize(stream, data);
		return serial_size;
	}

	template<class T>
	static void DeSerialization(T &data, std::vector<uint8_t > &buffer) {
		ros::serialization::IStream stream((uint8_t *)&buffer[0], buffer.size());
		ros::serialization::Serializer<T>::read(stream, data);

	}

	template<class T>
	static void DeSerialization(T &data, uint8_t *buffer, int buffer_len) {
		ros::serialization::IStream stream(buffer, buffer_len);
		ros::serialization::Serializer<T>::read(stream, data);

	}

};























#endif

