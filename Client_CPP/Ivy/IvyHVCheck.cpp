#include "IvyHVCheck.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <algorithm>

void IvyHVCheck::ReadConfig()
{
	for (int index = 0; index < HV_HEIGHT_MAX * HV_VELOCITY_MAX; index++)
	{
		m_hv_tracking[index] = 0;
	}

	for (int index = 0; index < HV_POLYGONS_MAX; index++)
	{
		hv_polygon[index].clear();
	}



	try
	{
		char buffer[2048];
		char buffer2[2048];
		XPLMGetNthAircraftModel(0, buffer, buffer2);
		std::string ds = XPLMGetDirectorySeparator();
		std::string file_name = buffer2;

		std::size_t found = file_name.find_last_of(ds);

		if (found != std::string::npos)
		{
			file_name = file_name.substr(0, found + 1);
			file_name += m_ini_file;
			boost::property_tree::ptree pt;
			boost::property_tree::ini_parser::read_ini(file_name, pt);

			for (int poly = 0; poly < HV_POLYGONS_MAX; poly++)
			{
				for (int point = 0; point < HV_POINTS_MAX; point++)
				{
					std::string value_str = "HV_POLY_" + std::to_string(poly+1) + ".hv_point_" + std::to_string(point+1);
					try 
					{ 
						hv_entry entry;
						entry.height = pt.get<float>(value_str + "_h");
						entry.velocity = pt.get<float>(value_str + "_v");

						hv_polygon[poly].push_back(entry);
					}
					catch (...) 
					{ 
						
					}


				}
			}
			m_hv_diagram_found = true;
		}
	}
	catch (...)
	{
		IvyDebugString("Reading hv_diagram.ini file failed");
		m_hv_diagram_found = false;
	}
}

bool IvyHVCheck::CheckHV(float height, float velocity)
{
	

	if (m_hv_diagram_found == false) 
		return true;

	bool ret = true;

	for (int index = 0; index < HV_POLYGONS_MAX; index++)
		if (hv_polygon[index].size() > 1)
			if (Pnpoly(hv_polygon[index], abs(velocity), abs(height)) > 0)
				ret =  false;

	int v_index =(int) abs(velocity);
	int h_index = (int) abs(height);

	if ((v_index < HV_VELOCITY_MAX) && (h_index < HV_HEIGHT_MAX))
		m_hv_tracking[v_index + (h_index * HV_VELOCITY_MAX)] = ret == true? 1 : 2;

	return ret;
}

int IvyHVCheck::Pnpoly(std::vector<hv_entry>& points, double test_velocity, double test_height)
{
	int i, j, c = 0;
	for (i = 0, j = points.size() - 1; i < points.size(); j = i++) {
		if (((points[i].height > test_height) != (points[j].height > test_height)) &&
			(test_velocity < (points[j].velocity - points[i].velocity) * (test_height - points[i].height) / (points[j].height - points[i].height) + points[i].velocity))
			c = !c;
	}
	return c;
}
