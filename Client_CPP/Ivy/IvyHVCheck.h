#pragma once
#include "Ivy.h"

struct hv_entry
{
	float height = 0;
	float velocity = 0;
};

class IvyHVCheck
{
	std::string m_ini_file = "hv_diagram.ini";
	

	


public:
	std::vector<hv_entry> hv_polygon[HV_POLYGONS_MAX];
	int m_hv_tracking[HV_HEIGHT_MAX * HV_VELOCITY_MAX];
	bool m_hv_diagram_found = false;

	void ReadConfig();
	bool CheckHV(float height, float velocity);
	int Pnpoly(std::vector<hv_entry>& points, double test_velocity, double test_height);
};

