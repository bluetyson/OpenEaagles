//------------------------------------------------------------------------------
// Class: Tdb
//------------------------------------------------------------------------------
#ifndef __oe_Simulation_Tdb_H__
#define __oe_Simulation_Tdb_H__

#include "openeaagles/simulation/System.h"

namespace oe {
namespace Simulation {
   class Gimbal;
   class Player;

//------------------------------------------------------------------------------
// Class: Tdb
// Description: Target Data Block (TDB) (i.e., players of interest) used by
//              gimbals and sensors.
//
//    Contains a list of "players of interest" (i.e., targets) used by the gimbal,
//    and the associated geometry data, as seen by the gimbal.
// Factory name: Gimbal_Tdb
// 
// Tasks: 
// 
//    processPlayers() --- Process players-of-interest
//       Scan the provided player list and generates a sublist of target players
//       that were filtered by player type, max range, max angles, etc.
// 
//       All filter parameters (e.g., player types, max range, max angle off 
//       boresight, etc) are from our controlling gimbal object.
//
//       We'll use world position vectors (i.e., usingECEF()) if the gimbal's 
//       'useWorldCoordinates' slot was set (see Gimbal::isUsingWorldCoordinates())
//       or if our own player's gaming area position vector is not valid (see
//       Player::isPositionVectorValid())
//
//       If we're using gaming area position vectors (i.e., not usingECEF()) then
//       all target's with invalid gaming area position vectors are rejected.
//       
// 
//       (Background task)
//
//    computeBoresightData() --- 
//       Scan the target list, which was generated by processPlayers(), and 
//       computes gimbal boresight data for each target player.
// 
//       (Time-critical task -- should be in-sync at the end of the dynamics
//       phase or at the start of the transmit phase; i.e., all of the dynamics
//       must be completed before the LOS vectors, ranges, angles, etc. are
//       computed and used).
//
// Gimbal coordinates:
//       X+ is along the gimbal/sensor boresight
//       Y+ is to the right of the gimbal boresight
//       Z+ is the cross product of X into Y
//
//------------------------------------------------------------------------------
class Tdb : public Basic::Object
{
   DECLARE_SUBCLASS(Tdb,Basic::Object)

public:
   Tdb(const unsigned int maxTargets, const Gimbal* const gimbal);

   //------------------------------------------------------------------------------
   // Process players-of-interest --- Scan the provided player list generates a
   // filtered list of target players.
   // (Background task)
   // 
   // Notes:
   //------------------------------------------------------------------------------
   virtual unsigned int processPlayers(Basic::PairStream* const players);

   // ---
   // Data from processPlayers()
   // ---

   // Are we using ECEF to compute LOS vectors?
   bool usingECEF() const                             { return usingEcefFlg; }

   // Number of active targets
   unsigned int getNumberOfTargets() const            { return numTgts; }

   // The array of target pointer
   Player** getTargets()                              { return targets; }

   // The array of target pointer (const version)
   Player** const getTargets() const                  { return targets; }

   //------------------------------------------------------------------------------
   // Compute Boresight Data --- Scan the target list, which as been pre-processed by
   // processPlayers(), and compute gimbal boresight data (e.g., range, range rate,
   // normalized Line-Of-Sight (LOS) vector) for each target player.
   // (Time-critical task -- in sync with gimbal dynamics & transmit)
   //------------------------------------------------------------------------------
   virtual unsigned int computeBoresightData();

   // ---
   // Data from computeBoresightData()
   // ---

   // The array of target ranges (m)
   const double* getTargetRanges() const              { return ranges; }

   // The array of target range rates (m/s)
   const double* getTargetRangeRates() const          { return rngRates; }

   // The array of normalized LOS vectors (gimbal to target) in Gimbal coordinates ()
   const osg::Vec3d* getGimbalLosVectors() const      { return losG; }

   // The array of ownship to target normalized LOS vectors (ownship's NED)
   const osg::Vec3d* getLosVectors() const            { return losO2T; }

   // The array of target to ownship normalized LOS vectors (target's NED)
   const osg::Vec3d* getTargetLosVectors() const      { return losT2O; }

   // Compute angle off antenna boresight (radians)
   const double* getBoresightErrorAngles() const      { return aar; }

   // Compute azimuth off boresight (radians)
   const double* getBoresightAzimuthErrors() const    { return aazr; }

   // Compute elevation off boresight (radians)
   const double* getBoresightElevationErrors() const  { return aelr; }

protected:
   Tdb();
   void initData();

   // Sets our Gimbal
   virtual void setGimbal(const Gimbal* const gimbal); 

   // Clear the target data arrays
   virtual void clearArrays();

   // Resize the target data arrays
   // -- old data is lost
   virtual bool resizeArrays(const unsigned int newSize);

   const Player* ownship;     // Our ownship player (set using setGimbal())
   const Gimbal* gimbal;      // Our gimbal (set in setGimbal())

   bool usingEcefFlg;         // Using ECEF flag --
                              //   When gimbal's 'useWorld' is true or when our ownship's
                              //   local gaming area position is not valid

   Player**    targets;       // Target pointer
   unsigned int maxTargets;   // Max number of targets (i.e., size of the arrays)
   unsigned int numTgts;      // Number of targets

   osg::Vec3d*  losG;         // Normalized LOS vector (gimbal to target) in Gimbal coord 
   osg::Vec3d*  losO2T;       // Ownship to target normalized LOS vector (ownship's NED)
   osg::Vec3d*  losT2O;       // Target to ownship normalized LOS vector (target's NED) 

   double*     ranges;        // Range to target (meters)
   double*     rngRates;      // Range Rate (m/s)
   double*     aar;           // Compute angle off antenna boresight (radians)
   double*     aazr;          // Compute azimuth off boresight (radians)
   double*     aelr;          // Compute elevation off boresight (radians)

   // computeBoresightData() arrays
   double* xa;
   double* ya;
   double* za;
   double* ra2;
   double* ra;
};

} // End Simulation namespace
} // End oe namespace

#endif
