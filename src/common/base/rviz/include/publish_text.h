#ifndef __PUBLISH_TEXT_H__
#define __PUBLISH_TEXT_H__

#include "visualization_msgs/MarkerArray.h"
#include <string>
#include <iostream>
#include <math.h>

#include "publish_base_header.h"
template <class T>
struct PublishText
{
	visualization_msgs::MarkerArray publish_marker_array;
	T inner_text;
	pColorFunction pget_g_func;
	pColorFunction pget_r_func;
	pColorFunction pget_b_func;
	pToStringFunction ptostring_func;

	std::string DefaultToString(void*) {
		return "";
	}
	float DefaultGetGFunc(void*) {
		return 1.0;
	}

	float DefaultGetRFunc(void*) {
		return 1.0;
	}

	float DefaultGetBFunc(void*) {
		return 0.0;
	}
	PublishText() {
		publish_marker_array.markers.clear();
		pget_g_func = std::bind(&PublishText::DefaultGetGFunc, this, std::placeholders::_1);
		pget_r_func = std::bind(&PublishText::DefaultGetRFunc, this, std::placeholders::_1);
		pget_b_func = std::bind(&PublishText::DefaultGetBFunc, this, std::placeholders::_1);

		ptostring_func = std::bind(&PublishText::DefaultToString, this, std::placeholders::_1);
	}
	void Reset() {
		publish_marker_array.markers.clear();
	}
	void SetGFunc(pColorFunction usr_get_g_func) {
		pget_g_func = std::move(usr_get_g_func);
	}
	void SetRFunc(pColorFunction usr_get_r_func) {
		pget_r_func = std::move(usr_get_r_func);
	}
	void SetBFunc(pColorFunction usr_get_b_func) {
		pget_b_func = std::move(usr_get_b_func);
	}

	void SetToStringFunc(pToStringFunction usr_to_string_func) {
		ptostring_func = std::move(usr_to_string_func);
	}

	void publish(ros::Publisher &pub, const char &color = 'y', const std::string &_namespace = "base") {
		publish("base_link", pub, color, _namespace);
	}

	void publish(std::string frame_id, ros::Publisher &pub, const char &color = 'y', const std::string &_namespace = "base") {
		if (pub.getNumSubscribers() == 0 )  return;

		if (publish_marker_array.markers.size() > 400) {
			publish_marker_array.markers.clear();
		}

		if (inner_text.size() > publish_marker_array.markers.size()) {
			publish_marker_array.markers.resize(inner_text.size());
		}
		int counter = 0;
		for (auto &text : inner_text) {
			publish_marker_array.markers[counter].header.frame_id = frame_id;
			publish_marker_array.markers[counter].header.stamp = ros::Time::now();
			publish_marker_array.markers[counter].ns = _namespace;
			publish_marker_array.markers[counter].id = counter;
			publish_marker_array.markers[counter].lifetime = ros::Duration(1);
			publish_marker_array.markers[counter].action = visualization_msgs::Marker::ADD;
			double size = 1.5;
			// if (fabs(text.x) < 10) {
			// 	size = 1;
			// }
			// else if ((1.1 - (fabs(text.x) / 100.0)) > 1) {
			// 	size = 1.1 - (fabs(text.x) / 100.0);
			// }
			// else {
			// 	size = 1;
			// }
			publish_marker_array.markers[counter].scale.x = size;
			publish_marker_array.markers[counter].scale.y = size;
			publish_marker_array.markers[counter].scale.z = size;

			publish_marker_array.markers[counter].pose.position.x = text.x;
			publish_marker_array.markers[counter].pose.position.y = text.y;
			publish_marker_array.markers[counter].pose.position.z = 0;//text.z;

			publish_marker_array.markers[counter].type = visualization_msgs::Marker::TEXT_VIEW_FACING;
			publish_marker_array.markers[counter].text = ptostring_func((void *)&text);//text.ToString();
			publish_marker_array.markers[counter].pose.orientation.w = 1.0;
			switch (color) {
			case 'y':
				publish_marker_array.markers[counter].color.b = 0.0;
				publish_marker_array.markers[counter].color.g = 1.0;
				publish_marker_array.markers[counter].color.r = 1.0;
				break;
			case 'r':
				publish_marker_array.markers[counter].color.b = 0.0;
				publish_marker_array.markers[counter].color.g = 0.0;
				publish_marker_array.markers[counter].color.r = 1.0;
				break;
			case 'b':
				publish_marker_array.markers[counter].color.b = 1.0;
				publish_marker_array.markers[counter].color.g = 0.0;
				publish_marker_array.markers[counter].color.r = 0.0;
				break;
			case 'g':
				publish_marker_array.markers[counter].color.b = 0.0;
				publish_marker_array.markers[counter].color.g = 1.0;
				publish_marker_array.markers[counter].color.r = 0.0;
				break;
			case 'p':
				publish_marker_array.markers[counter].color.b = 0.5;
				publish_marker_array.markers[counter].color.g = 0.0;
				publish_marker_array.markers[counter].color.r = 0.5;
			case 'w':
				publish_marker_array.markers[counter].color.b = 1.0;
				publish_marker_array.markers[counter].color.g = 1.0;
				publish_marker_array.markers[counter].color.r = 1.0;
				break;
				break;
			default:
				publish_marker_array.markers[counter].color.b = pget_b_func((void *)&text);
				publish_marker_array.markers[counter].color.g = pget_g_func((void *)&text);
				publish_marker_array.markers[counter].color.r = pget_r_func((void *)&text);
				// publish_marker_array.markers[counter].color.b = 0.0;
				// publish_marker_array.markers[counter].color.g = 1.0;
				// publish_marker_array.markers[counter].color.r = 1.0;
			}

			publish_marker_array.markers[counter].color.a = 0.7;
			counter++;
		}

		for (unsigned int i = counter; i < publish_marker_array.markers.size(); i++) {
			publish_marker_array.markers[i].id = i;
			publish_marker_array.markers[i].action = visualization_msgs::Marker::DELETE;
			publish_marker_array.markers[i].pose.position.x = -9900;
		}
		pub.publish(publish_marker_array);
	}
};
























#endif
