#include "headers/simulation.h"
#include "glm/glm.hpp"

using glm::dot;

Simulation::Simulation(int numberParticles) : 
	numberParticles(numberParticles),
	influenceRadiusScale(3.0f),
	hardCoreRadius(0.001f),
	targetNumberDensity(2.2f),
	stiffness(20.0f),
	nearToFar(2.0f),
	viscousGain(0.99f),
    ambientDensity(1.0f),
    radialViscosity(30.0f),
	gravityAcceleration(vec3(0.0f, 0.0f, -10.0f))
{
    particles.resize(numberParticles);
	accelerationAtParticles.resize(numberParticles, vec3(0.0f, 0.0f, 0.0f));
	densityAtParticles.resize(numberParticles, vec3(0.0f, 0.0f,0.0f));
    for (int i = 0; i < numberParticles; i++)
    {
    	particles[i] = new Particle((float)i/numberParticles, 0, 0);
    }
}

void Simulation::update(float timeStep)
{
	computeDensityAtParticles();
	computePressureGradientForce();
	applyExternalForces();
	for (int i = 0; i < numberParticles; i++)
	{
		particles[i]->updateVelocity(timeStep, accelerationAtParticles[i]);
	}
	diffuseAndDissipateVelocity(timeStep);
	for (int i = 0; i < numberParticles; i++)
	{
		particles[i]->updatePosition(timeStep);
	}
}

void Simulation::computeDensityAtParticles()
{
	const float influenceRadius = influenceRadiusScale * particles[0]->getRadius();
	const float influenceRadius2 = influenceRadius * influenceRadius;
	const float normFactor = 15.0f / ( Pi * influenceRadius * influenceRadius * influenceRadius );
	for (int i = 0; i < numberParticles; i++)
	{
		for (int j = i+1; j < numberParticles; j++)
		{
			const vec3  sep     = particles[i]->getPosition() - particles[j]->getPosition();
            const float dist2   = dot(sep, sep);
            if( dist2 < influenceRadius2 )
            {   // Particles are close enough to contribute density to each other.
                const float q    = 1.0f - sqrt(dist2) / influenceRadius;
                const float q2   = pow(q,2);
                const float q3   = q2 * q ;
                const float q4   = q2 * q2;
                // ASSERT( ( 0.0f <= q ) && ( q <= 1.0f ) );
                densityAtParticles[i].x      	+= q3 * normFactor * particles[j]->getVolume(); //number density
                densityAtParticles[i].y  		+= q4 * normFactor * particles[j]->getVolume(); //nearnumber density
                densityAtParticles[i].z        	+= q3 * normFactor * particles[j]->getMass();   // massdensity

                densityAtParticles[j].x     	+= q3 * normFactor * particles[i]->getVolume();
                densityAtParticles[j].y 		+= q4 * normFactor * particles[j]->getVolume();
                densityAtParticles[j].z         += q3 * normFactor * particles[i]->getMass();
            }
		}
	}
}

void Simulation::computePressureGradientForce()
{
	const float influenceRadius = influenceRadiusScale * particles[0]->getRadius();
	const float influenceRadius2 = influenceRadius * influenceRadius;
	const float hardCoreRadius2 = hardCoreRadius * hardCoreRadius;
	const float waveSpeed2 = stiffness;
	const float waveSpeed2Near = stiffness * nearToFar;
	for (int i = 0; i < numberParticles; i++)
	{
		for (int j = i+1; j < numberParticles; j++)
		{
			const vec3  sep     = particles[i]->getPosition() - particles[j]->getPosition();
            float dist2   = dot(sep, sep);
            if(     ( dist2 < hardCoreRadius2 )
                ||  ( abs(sep.x) < hardCoreRadius )
                ||  ( abs(sep.y) < hardCoreRadius )
                ||  ( abs(sep.z) < hardCoreRadius ) )
            {   // Particles too close. Pressure gradient would be zero.
                // Impose a random separation.
                // sep    += RandomSpread( sTinyJiggle ) ; //TODO
                dist2   = dot(sep, sep) ;
            }

            if( dist2 < influenceRadius2 )
            {   // Particles are close enough to apply pressure on each other.
                const float dist    = sqrt(dist2) ;
                const float q       = 1.0f - dist / influenceRadius ;
                const float q2      = pow(q,2) ;

                // ASSERT( ( 0.0f <= q ) && ( q <= 1.0f ) ) ;

                const vec3      dir         = sep / ( dist + FLT_EPSILON ) ;

                const float &   numDensA    = densityAtParticles[i].x ;
                const float &   numDensB    = densityAtParticles[j].x ;

                {   // Compute acceleration due to pressure gradient.
                    const float q3          = q2 * q  ;
                    const float & nearDensA = densityAtParticles[i].y ;
                    const float & nearDensB = densityAtParticles[j].y ;
                    const float pressure    = waveSpeed2     * ( numDensA + numDensB - 2.0f * targetNumberDensity ) ;
                    const float pressNear   = waveSpeed2Near * ( nearDensA + nearDensB ) ;
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
}

void Simulation::applyExternalForces()
{
	for (int i = 0; i < numberParticles; i++)
	{
        const float & density           = particles[i]->getDensity() ;
        const float   relativeDensity   = ( density - ambientDensity ) / density ;
        accelerationAtParticles[i] += gravityAcceleration * relativeDensity;
	}
}

void Simulation::diffuseAndDissipateVelocity(float timeStep)
{
	const float influenceRadius 	= influenceRadiusScale * particles[0]->getRadius();
	const float influenceRadius2 	= influenceRadius * influenceRadius;
	const float speedLimit2      	= 2.0f * stiffness ;
    const float radialViscosityGain = radialViscosity * timeStep;

	for (int i = 0; i < numberParticles; i++)
	{
		for (int j = i+1; j < numberParticles; j++)
		{
			vec3&  velA    = particles[i]->getVelocity() ;

            // ASSERT( ! IsNan( velA ) && ! IsInf( velA ) ) ;
            // ASSERT( ! IsInf( velA.Mag2() ) ) ;
            // ASSERT( velA.Mag2() < 1e8f ) ;

            const vec3  sep     = particles[i]->getPosition() - particles[j]->getPosition() ;
            const float dist2   = dot(sep,sep);
            if( dist2 < influenceRadius2 )
            {   // Particles are near enough to exchange velocity.
                const float     dist    = sqrt(dist2) ;
                const vec3      sepDir  = sep / dist ;
                vec3&           velB    = particles[j]->getVelocity() ;
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
	    vec3 & vel = particles[i]->getVelocity() ;
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