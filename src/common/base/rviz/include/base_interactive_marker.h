#ifndef __BASE_INTERACTIVE_MARKER_H__
#define __BASE_INTERACTIVE_MARKER_H__


#include <ros/ros.h>

#include <interactive_markers/interactive_marker_server.h>
#include <interactive_markers/menu_handler.h>

#include <tf/transform_broadcaster.h>
#include <tf/tf.h>

#include <math.h>
#include "common/rviz/include/publish_pointcloud.h"

using namespace visualization_msgs;

using namespace std;

typedef struct PointXY
{
	double x;
	double y;
	PointXY() {
		x = 0;
		y = 0;
	}
	PointXY(double _x, double _y) {
		x = _x;
		y = _y;
	}

	void Set(double _x, double _y) {
		x = _x;
		y = _y;
	}
} PointXY;

extern std::vector<PointXY> object_vec;
extern std::vector<PointXY> pointxy_vec;
extern double  gwidth , glength;



using Point32Vec = std::vector<geometry_msgs::Point32>;

template<class Type>
void CalCurvePoint(float t, vector<PointXY> m_vecCtrlPt, Type& stPt)
{
	// 确保是二次曲线，2个顶点一个控制点
	assert(m_vecCtrlPt.size() == 3);

	// 计算曲线点坐标，此为2次算法，改变此处可以实现多次曲线
	float x = (float)m_vecCtrlPt[0].x * pow(1 - t, 2)   +
	          (float)m_vecCtrlPt[1].x * t * (1 - t) * 2 +
	          (float)m_vecCtrlPt[2].x * pow(t, 2);
	float y = (float)m_vecCtrlPt[0].y * pow(1 - t, 2)   +
	          (float)m_vecCtrlPt[1].y * t * (1 - t) * 2 +
	          (float)m_vecCtrlPt[2].y * pow(t, 2);
	stPt.x = x;
	stPt.y = y;
}

static void MakePointCloudCurve(vector<PointXY> &ctl_point, ObjectPointCloud<Point32Vec>  &object_pointcloud) {

	object_pointcloud.inner_points.clear();
	vector<PointXY> m_vecCtrlPt;
	m_vecCtrlPt.resize(3);
	for (int i = 0; i + 2 < ctl_point.size(); i = i + 2) {
		m_vecCtrlPt[0] = ctl_point[i + 0];
		m_vecCtrlPt[1] = ctl_point[i + 1];
		m_vecCtrlPt[2] = ctl_point[i + 2];
		float step = 0.01;
		for (float t = 0.0; t <= 1.0; t += step)
		{
			geometry_msgs::Point32 stPt;
			CalCurvePoint(t, m_vecCtrlPt, stPt);
			object_pointcloud.inner_points.push_back(stPt);
		}
	}
}

static void MakeObjectPointCloud(ObjectPointCloud<Point32Vec>  &object_pointcloud ) {
	ros::NodeHandle nh;
	nh.param("width", gwidth, gwidth);
	nh.param("length", glength, glength);

	object_pointcloud.Reset();

	for (int i = 0; i < object_vec.size(); i++) {
		// ROS_INFO_STREAM(  "-------init1 " << object_pointcloud.inner_points.size() );
		float dl_w = object_vec[i].x - gwidth / 2;
		float dr_h = object_vec[i].y - glength / 2;
		float ul_w = object_vec[i].x + gwidth / 2;
		float ur_h = object_vec[i].y + glength / 2;
		float d = 0.0;

		// ROS_INFO_STREAM(  "init1 " << dl_w << "," << dr_h << "," << ul_w << "," << ur_h);
		for (d = dl_w; d < ul_w; d += 0.1) {
			geometry_msgs::Point32 tmp;
			tmp.x = d;
			tmp.y = dr_h;
			object_pointcloud.inner_points.push_back(tmp);
		}

		// ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );


		for (d = dl_w; d < ul_w; d += 0.1) {
			geometry_msgs::Point32 tmp;
			tmp.x = d;
			tmp.y = ur_h;
			object_pointcloud.inner_points.push_back(tmp);
		}
		// ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );

		for (d = dr_h; d < ur_h; d += 0.1) {
			geometry_msgs::Point32 tmp;
			tmp.x = dl_w;
			tmp.y = d;
			object_pointcloud.inner_points.push_back(tmp);
		}
		// ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );

		for (d = dr_h; d < ur_h; d += 0.1) {
			geometry_msgs::Point32 tmp;
			tmp.x = ul_w;
			tmp.y = d;
			object_pointcloud.inner_points.push_back(tmp);
		}
	}


	// ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );

}


class BaseInteractiveMarker
{
public:
	BaseInteractiveMarker() {
		pointxy_vec.resize(7);
		server.reset( new interactive_markers::InteractiveMarkerServer("basic_controls", "", false) );
		tf::Vector3 position;

		position = tf::Vector3( 0, 0, 0);
		pointxy_vec[0].Set(0, 0);
		make6DofMarker("first" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );

		position = tf::Vector3( 4, 0, 0);
		pointxy_vec[1].Set(4, 0);
		make6DofMarker("second" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );


		position = tf::Vector3( 8, 0, 0);
		pointxy_vec[2].Set(8, 0);
		make6DofMarker("third" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );


		position = tf::Vector3( 12, 0, 0);
		pointxy_vec[3].Set(12, 0);
		make6DofMarker("four" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );


		position = tf::Vector3( 16, 0, 0);
		pointxy_vec[4].Set(16, 0);
		make6DofMarker("five" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );


		position = tf::Vector3( 20, 0, 0);
		pointxy_vec[5].Set(20, 0);
		make6DofMarker("six" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );


		position = tf::Vector3( 24, 0, 0);
		pointxy_vec[6].Set(24, 0);
		make6DofMarker("seven" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );


		object_vec.resize(5);
		position = tf::Vector3( 30, 5, 0);
		object_vec[0].Set(30, 5);
		make6DofMarker("object1" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );

		position = tf::Vector3( 30, 10, 0);
		object_vec[1].Set(30, 10);
		make6DofMarker("object2" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );

		position = tf::Vector3( 30, 15, 0);
		object_vec[2].Set(30, 15);
		make6DofMarker("object3" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );

		position = tf::Vector3( 30, 20, 0);
		object_vec[3].Set(30, 20);
		make6DofMarker("object4" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );

		position = tf::Vector3( 30, 25, 0);
		object_vec[4].Set(30, 25);
		make6DofMarker("object5" , false, visualization_msgs::InteractiveMarkerControl::MOVE_3D, position, false );

		server->applyChanges();
	}
	~BaseInteractiveMarker() {

	}
private:
	boost::shared_ptr<interactive_markers::InteractiveMarkerServer> server;
private:

// %Tag(Box)%
	Marker makeBox( InteractiveMarker &msg )
	{
		Marker marker;

		marker.type = Marker::CUBE;
		marker.scale.x = msg.scale * 0.45;
		marker.scale.y = msg.scale * 0.45;
		marker.scale.z = msg.scale * 0.45;

		marker.color.r = 0.5;
		marker.color.g = 0.5;
		marker.color.b = 0.5;
		marker.color.a = 1.0;

		return marker;
	}


// %Tag(processFeedback)%
	static void processFeedback( const visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback )
	{

		// std::ostringstream mouse_point_ss;
		// if ( feedback->mouse_point_valid )
		// {
		// 	mouse_point_ss << " at " << feedback->mouse_point.x
		// 	               << ", " << feedback->mouse_point.y
		// 	               << ", " << feedback->mouse_point.z
		// 	               << " in frame " << feedback->header.frame_id;
		// }

		switch ( feedback->event_type )
		{
		case visualization_msgs::InteractiveMarkerFeedback::MOUSE_UP:
			PointXY cur(feedback->mouse_point.x, feedback->mouse_point.y);
			if (feedback->marker_name == "first") pointxy_vec[0] = cur;
			if (feedback->marker_name == "second") pointxy_vec[1] = cur;
			if (feedback->marker_name == "third") pointxy_vec[2] = cur;
			if (feedback->marker_name == "four") pointxy_vec[3] = cur;
			if (feedback->marker_name == "five") pointxy_vec[4] = cur;
			if (feedback->marker_name == "six") pointxy_vec[5] = cur;
			if (feedback->marker_name == "seven") pointxy_vec[6] = cur;
			if (feedback->marker_name == "object1") object_vec[0] = cur;
			if (feedback->marker_name == "object2") object_vec[1] = cur;
			if (feedback->marker_name == "object3") object_vec[2] = cur;
			if (feedback->marker_name == "object4") object_vec[3] = cur;
			if (feedback->marker_name == "object5") object_vec[4] = cur;
			ROS_INFO_STREAM(  ": mouse up"  << feedback->marker_name << "." << feedback->mouse_point.x << "," << feedback->mouse_point.y);
			break;
		}

		// server->applyChanges();
	}


	InteractiveMarkerControl& makeBoxControl( InteractiveMarker &msg )
	{
		InteractiveMarkerControl control;
		control.always_visible = true;
		control.markers.push_back( makeBox(msg) );
		msg.controls.push_back( control );

		return msg.controls.back();
	}
// %EndTag(Box)%


// %Tag(6DOF)%
	void make6DofMarker(std::string name, bool fixed, unsigned int interaction_mode, const tf::Vector3& position, bool show_6dof )
	{
		InteractiveMarker int_marker;
		int_marker.header.frame_id = "base_link";
		tf::pointTFToMsg(position, int_marker.pose.position);
		int_marker.scale = 2;

		int_marker.name = name;
		int_marker.description = "Simple 6-DOF Control";

		// insert a box
		makeBoxControl(int_marker);
		int_marker.controls[0].interaction_mode = interaction_mode;

		InteractiveMarkerControl control;

		if ( fixed )
		{
			int_marker.description += "\n(fixed orientation)";
			control.orientation_mode = InteractiveMarkerControl::FIXED;
		}

		if (interaction_mode != visualization_msgs::InteractiveMarkerControl::NONE)
		{
			std::string mode_text;
			if ( interaction_mode == visualization_msgs::InteractiveMarkerControl::MOVE_3D )         mode_text = "MOVE_3D";
			if ( interaction_mode == visualization_msgs::InteractiveMarkerControl::ROTATE_3D )       mode_text = "ROTATE_3D";
			if ( interaction_mode == visualization_msgs::InteractiveMarkerControl::MOVE_ROTATE_3D )  mode_text = "MOVE_ROTATE_3D";
			int_marker.description = std::string("3D Control") + (show_6dof ? " + 6-DOF controls" : "") + "\n" + mode_text;
		}

		if (show_6dof)
		{
			control.orientation.w = 1;
			control.orientation.x = 1;
			control.orientation.y = 0;
			control.orientation.z = 0;
			control.name = "rotate_x";
			control.interaction_mode = InteractiveMarkerControl::ROTATE_AXIS;
			int_marker.controls.push_back(control);
			control.name = "move_x";
			control.interaction_mode = InteractiveMarkerControl::MOVE_AXIS;
			int_marker.controls.push_back(control);

			control.orientation.w = 1;
			control.orientation.x = 0;
			control.orientation.y = 1;
			control.orientation.z = 0;
			control.name = "rotate_z";
			control.interaction_mode = InteractiveMarkerControl::ROTATE_AXIS;
			int_marker.controls.push_back(control);
			control.name = "move_z";
			control.interaction_mode = InteractiveMarkerControl::MOVE_AXIS;
			int_marker.controls.push_back(control);

			control.orientation.w = 1;
			control.orientation.x = 0;
			control.orientation.y = 0;
			control.orientation.z = 1;
			control.name = "rotate_y";
			control.interaction_mode = InteractiveMarkerControl::ROTATE_AXIS;
			int_marker.controls.push_back(control);
			control.name = "move_y";
			control.interaction_mode = InteractiveMarkerControl::MOVE_AXIS;
			int_marker.controls.push_back(control);
		}

		server->insert(int_marker);
		server->setCallback(int_marker.name, &BaseInteractiveMarker::processFeedback);
	}
// %EndTag(6DOF)%

// %EndTag(processFeedback)%


};
















#endif