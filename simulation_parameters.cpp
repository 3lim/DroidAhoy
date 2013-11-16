#include "headers/simulation_parameters.h"

#include <sstream>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

SimulationParameters::SimulationParameters(const string& fileName) :
	influenceRadiusScale(3.0f),
	hardCoreRadius(0.001f),
	targetNumberDensity(2.2f),
    ambientDensity(1.0f),
    nearToFar(2.0f),
    stiffness(20.0f),
    viscousGain(0.99f),
    radialViscosity(30.0f),
	gravityAcceleration(vec3(0.0f, 0.0f, -10.0f))
{
	if (fileName != "")
	{
		ifstream file( fileName );

		string line;
		
		if(file.is_open())
		{	
			cout << "File \"" << fileName << "\" loaded\n\n";
			bool present[9] = { false };
			while(getline(file, line))
			{
				if(!line.size()) continue;
				std::stringstream stream(line);
				string type;
				stream >> type;

				if (type == "influenceRadiusScale")
				{
					if (present[0]) 
						cout << "Warning : defining influenceRadiusScale multiple times." << endl;
					stream >> influenceRadiusScale;
					present[0] = true;
				}
				else if (type == "hardCoreRadius")
				{

					if (present[1]) 
						cout << "Warning : defining hardCoreRadius multiple times." << endl;
					stream >> hardCoreRadius;
					present[1] = true;
				}
				else if (type == "targetNumberDensity")
				{
					if (present[2]) 
						cout << "Warning : defining targetNumberDensity multiple times." << endl;
					stream >> targetNumberDensity;
					present[2] = true;
				}
				else if (type == "ambientDensity")
				{
					if (present[3]) 
						cout << "Warning : defining ambientDensity multiple times." << endl;
					stream >> ambientDensity;
					present[3] = true;
				}
				else if (type == "nearToFar")
				{
					if (present[4]) 
						cout << "Warning : defining nearToFar multiple times." << endl;
					stream >> nearToFar;
					present[4] = true;
				}
				else if (type == "stiffness")
				{
					if (present[5]) 
						cout << "Warning : defining stiffness multiple times." << endl;
					stream >> stiffness;
					present[5] = true;
				}
				else if (type == "viscousGain")
				{
					if (present[6]) 
						cout << "Warning : defining viscousGain multiple times." << endl;
					stream >> viscousGain;
					present[6] = true;
				}
				else if (type == "radialViscosity")
				{
					if (present[7]) 
						cout << "Warning : defining radialViscosity multiple times." << endl;
					stream >> radialViscosity;
					present[7] = true;
				}
				else if (type == "gravityAcceleration")
				{
					float x,y,z;
					if (present[8]) 
						cout << "Warning : defining gravityAcceleration multiple times." << endl;
					stream >> x >> y >> z;
					gravityAcceleration = vec3(x,y,z);
					present[8] = true;
				} 
				else if (type != "#")
				{
					cout << "Unrecognized type " << type << ". Ignoring." << endl;
				}
				for (int i = 0; i < 9; i++)
				{
					if (!present[i])
					{
						cout << "Warning : parameters file incomplete." << endl;
						break;
					}				 
				}
			}
		}
		else
		{
			cout << "ERROR - parameters \"" << fileName << "\"failed to load!" << endl;
			cout << "Initializing by default" << endl;
		} 
	}
}