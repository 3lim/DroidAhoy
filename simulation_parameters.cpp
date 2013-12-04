#include "headers/simulation_parameters.h"

#include <sstream>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

SimulationParameters::SimulationParameters(const string& fileName) :
	initialSetup("square"),
	numberParticles(300),
	sceneWidth(2.0),
	sceneLength(2.0),
	kernelRadius(0.12),
	massDensity0(5.0),
	gravity(-10.0),
	viscosityScale(1.0),
	pressureScale(1.0),
	heightOffset(0.0),
	heightScale(1.0){
	if (fileName != ""){
		ifstream file( fileName );

		string line;
		
		if(file.is_open()){	
			cout << "File \"" << fileName << "\" loaded" << endl;
			bool present[12] = { false };
			while(getline(file, line)){
				if(!line.size()) continue;
				std::stringstream stream(line);
				string type;
				stream >> type;

				if (type == "initialSetup"){
					if (present[0]) 
						cout << "Warning : defining initialSetup multiple times." << endl;
					stream >> initialSetup;
					present[0] = true;
				}
				else if (type == "numberParticles"){

					if (present[1]) 
						cout << "Warning : defining numberParticles multiple times." << endl;
					stream >> numberParticles;
					present[1] = true;
				}
				else if (type == "sceneWidth"){
					if (present[2]) 
						cout << "Warning : defining sceneWidth multiple times." << endl;
					stream >> sceneWidth;
					present[2] = true;
				}
				else if (type == "sceneLength"){
					if (present[3]) 
						cout << "Warning : defining sceneLength multiple times." << endl;
					stream >> sceneLength;
					present[3] = true;
				}
				else if (type == "kernelRadius"){
					if (present[4]) 
						cout << "Warning : defining kernelRadius multiple times." << endl;
					stream >> kernelRadius;
					present[4] = true;
				}
				else if (type == "massDensity0"){
					if (present[5]) 
						cout << "Warning : defining massDensity0 multiple times." << endl;
					stream >> massDensity0;
					present[5] = true;
				}
				else if (type == "gravity"){
					if (present[6]) 
						cout << "Warning : defining gravity multiple times." << endl;
					stream >> gravity;
					present[6] = true;
				}
				else if (type == "rebound"){
					if (present[7]) 
						cout << "Warning : defining rebound multiple times." << endl;
					stream >> rebound;
					present[7] = true;
				}
				else if (type == "viscosityScale"){
					if (present[8]) 
						cout << "Warning : defining viscosityScale multiple times." << endl;
					stream >> viscosityScale;
					present[8] = true;
				} 
				else if (type == "pressureScale"){
					if (present[9]) 
						cout << "Warning : defining pressureScale multiple times." << endl;
					stream >> pressureScale;
					present[9] = true;
				} 
				else if (type == "heightOffset"){
					if (present[10]) 
						cout << "Warning : defining heightOffset multiple times." << endl;
					stream >> heightOffset;
					present[10] = true;
				} 
				else if (type == "heightScale"){
					if (present[11]) 
						cout << "Warning : defining heightScale multiple times." << endl;
					stream >> heightScale;
					present[11] = true;
				} 
				else if (type != "#" && type != ""){
					cout << "Unrecognized type \"" << type << "\". Ignoring." << endl;
				}
			}
			for (int i = 0; i < 12; i++){
				if (!present[i]){
					cout << "Warning : parameters file incomplete." << endl;
					break;
				}
			}
		}
		else{
			cout << "ERROR - parameters \"" << fileName << "\"failed to load!" << endl;
			cout << "Initializing by default" << endl;
		} 
	}
}