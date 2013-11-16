#include "headers/simulation.h"
#include <iostream>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

using std::cout;
using std::endl;
using glm::dot;

Simulation::Simulation(int numberParticles, const string& parametersFile) : 
	numberParticles(numberParticles),
    parameters(parametersFile)
{
    accelerationAtParticles.resize(numberParticles, vec3(0.0f, 0.0f, 0.0f));
    densityAtParticles.resize(numberParticles, vec3(1.0f, 1.0f,0.0f));
    particles = new Particle[numberParticles];
    for (int i = 0; i < numberParticles; i++)
    {
     particles[i].getPosition() = vec3((float)i/numberParticles, 0, 0);
    }
}

void Simulation::update(float timeStep)
{
    accelerationAtParticles.resize(numberParticles, vec3(0.0f, 0.0f, 0.0f));
    densityAtParticles.resize(numberParticles, vec3(1.0f, 1.0f,0.0f));
	computeDensityAtParticles();
	computePressureGradientForce();
	applyExternalForces();
	for (int i = 0; i < numberParticles; i++)
	{
		particles[i].updateVelocity(timeStep, accelerationAtParticles[i]);
	}
	diffuseAndDissipateVelocity(timeStep);
	for (int i = 0; i < numberParticles; i++)
	{
		particles[i].updatePosition(timeStep);
    }
    cout << glfwGetTime() << endl;
}

void Simulation::computeDensityAtParticles()
{
	const float influenceRadius = parameters.getInfluenceRadiusScale() * particles[0].getRadius();
	const float influenceRadius2 = influenceRadius * influenceRadius;
	const float normFactor = 15.0f / ( Pi * influenceRadius * influenceRadius * influenceRadius );
    const float volume = particles[0].getVolume();
    const float mass = particles[0].getMass();
	for (int i = 0; i < numberParticles; i++)
	{
		for (int j = i+1; j < numberParticles; j++)
		{
			const vec3  sep     = particles[i].getPosition() - particles[j].getPosition();
            const float dist2   = dot(sep, sep);
            if( dist2 < influenceRadius2 )
            {   // Particles are close enough to contribute density to each other.
                const float q    = 1.0f - sqrt(dist2) / influenceRadius;
                const float q2   = pow(q,2);
                const float q3   = q2 * q ;
                const float q4   = q2 * q2;
                // ASSERT( ( 0.0f <= q ) && ( q <= 1.0f ) );
                densityAtParticles[i].x      	+= q3 * normFactor * volume; // number density
                densityAtParticles[i].y  		+= q4 * normFactor * volume; // near number density
                densityAtParticles[i].z        	+= q3 * normFactor * mass;   // massdensity

                densityAtParticles[j].x     	+= q3 * normFactor * volume;
                densityAtParticles[j].y 		+= q4 * normFactor * volume;
                densityAtParticles[j].z         += q3 * normFactor * mass;
            }
		}
	}
}

void Simulation::computePressureGradientForce()
{
	const float influenceRadius = parameters.getInfluenceRadiusScale() * particles[0].getRadius();
	const float influenceRadius2 = influenceRadius * influenceRadius;
    const float hardCoreRadius = parameters.getHardCoreRadius();
	const float hardCoreRadius2 = hardCoreRadius * hardCoreRadius;
	const float waveSpeed2 = parameters.getStiffness();
	const float waveSpeed2Near = parameters.getStiffness() * parameters.getNearToFar();
    const float targetNumberDensity = parameters.getTargetNumberDensity();
	for (int i = 0; i < numberParticles; i++)
	{
		for (int j = i+1; j < numberParticles; j++)
		{
			vec3  sep     = particles[i].getPosition() - particles[j].getPosition();
            float dist2   = dot(sep, sep);
            if(     ( dist2 < hardCoreRadius2 )
                ||  ( abs(sep.x) < hardCoreRadius )
                ||  ( abs(sep.y) < hardCoreRadius )
                ||  ( abs(sep.z) < hardCoreRadius ) )
            {   // Particles too close. Pressure gradient would be zero.
                // Impose a random separation.
                sep.x += hardCoreRadius * (float(rand()) / float(RAND_MAX) - 0.5f);
                sep.y += hardCoreRadius * (float(rand()) / float(RAND_MAX) - 0.5f);
                sep.z += hardCoreRadius * (float(rand()) / float(RAND_MAX) - 0.5f);
                dist2 = dot(sep, sep) ;
            }

            if( dist2 < influenceRadius2 )
            {   // Particles are close enough to apply pressure on each other.
                const float dist    = sqrt(dist2) ;
                const float q       = 1.0f - dist / influenceRadius ;
                const float q2      = pow(q,2) ;

                // ASSERT( ( 0.0f <= q ) && ( q <= 1.0f ) ) ;

                const vec3      dir         = sep / ( dist + FLT_EPSILON );
                
                // Compute acceleration due to pressure gradient.
                const float q3          = q2 * q  ;
                const float pressure    = waveSpeed2     * ( densityAtParticles[i].x + densityAtParticles[j].x - 2.0f * targetNumberDensity ) ;
                const float pressNear   = waveSpeed2Near * ( densityAtParticles[i].y + densityAtParticles[j].y ) ;
                const float dReg        = pressure  * q2 ;
                const float dNear       = pressNear * q3 ;
                const vec3  accel       = ( dReg + dNear ) * dir ;
                accelerationAtParticles[i] += accel ;
                accelerationAtParticles[j] -= accel ;
                // ASSERT( ! IsNan(accelerationAtParticles[i]) && ! IsInf(accelerationAtParticles[i]) ) ;
                // ASSERT( ! IsNan(accelerationAtParticles[j]) && ! IsInf(accelerationAtParticles[j]) ) ;
            }
		}
	}
}

void Simulation::applyExternalForces()
{
    const vec3& gravityAcceleration = parameters.getGravityAcceleration();
    const float ambientDensity = parameters.getAmbientDensity();
	for (int i = 0; i < numberParticles; i++)
	{
        const float & density           = particles[i].getDensity() ;
        const float   relativeDensity   = ( density - ambientDensity ) / density ;
        accelerationAtParticles[i] += gravityAcceleration * relativeDensity;
	}
}

void Simulation::diffuseAndDissipateVelocity(float timeStep)
{
	const float influenceRadius 	= parameters.getInfluenceRadiusScale() * particles[0].getRadius();
	const float influenceRadius2 	= influenceRadius * influenceRadius;
	const float speedLimit2      	= 2.0f * parameters.getStiffness() ;
    const float radialViscosityGain = parameters.getRadialViscosity() * timeStep;
    const float viscousGain         = parameters.getViscousGain();

	for (int i = 0; i < numberParticles; i++)
	{
		for (int j = i+1; j < numberParticles; j++)
		{
			vec3&  velA    = particles[i].getVelocity() ;

            // ASSERT( ! IsNan( velA ) && ! IsInf( velA ) ) ;
            // ASSERT( ! IsInf( velA.Mag2() ) ) ;
            // ASSERT( velA.Mag2() < 1e8f ) ;

            const vec3  sep     = particles[i].getPosition() - particles[j].getPosition() ;
            const float dist2   = dot(sep,sep);
            if( dist2 < influenceRadius2 )
            {   // Particles are near enough to exchange velocity.
                const float     dist    = sqrt(dist2) ;
                const vec3      sepDir  = sep / dist ;
                vec3&           velB    = particles[j].getVelocity() ;
                const vec3      velDiff = velA - velB ;
                const float     velSep  = dot(velDiff, sepDir) ;
                if( velSep < 0.0f )
                {   // Particles are approaching.
                    const float infl    = 1.0f - dist / influenceRadius ;
                    const float velSepA         = dot(velA, sepDir) ;                           // Component of velocity of particle A along separation direction.
                    const float velSepB         = dot(velB, sepDir) ;                           // Component of velocity of particle B along separation direction.
                    const float velSepTarget    = ( velSepA + velSepB ) * 0.5f ;            // Component of target velocity along separation direction.
                    const float diffSepA        = velSepTarget - velSepA ;                  // Difference between A's velocity-along-separation and target.
                    const float changeSepA      = radialViscosityGain * diffSepA * infl ;  // Amount of velocity change to apply.
                    const vec3  changeA         = changeSepA * sepDir ;                     // Velocity change to apply.
                    velA += changeA ;                                                       // Apply velocity change to A.
                    velB -= changeA ;                                                       // Apply commensurate change to B.
                    
                    // ASSERT( ! IsNan( velA ) && ! IsInf( velA ) ) ;
                    // ASSERT( ! IsNan( velB ) && ! IsInf( velB ) ) ;
                    // ASSERT( velA.Mag2() < 1e8f ) ;
                    // ASSERT( velB.Mag2() < 1e8f ) ;
                }
            }
		}
		// Dissipate velocity.
	    vec3 & vel = particles[i].getVelocity() ;
	    vel *= viscousGain ;
	    // Apply speed limit.
	    const float velMag2 =  dot(vel, vel);
	    if( velMag2 > speedLimit2 )
	    {   // Particle is going too fast.
	        const float reduction = sqrt( speedLimit2 / velMag2 ) ;
	        vel *= reduction ;
	    }
	}
}