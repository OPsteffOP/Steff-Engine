#include "pch.h"
#include "PhysxSimulationEventCallback.h"

// TODO: SEND THESE EVENTS (FOR EXAPLE, CONTACT SHOULD BE SEND TO ALL COMPONENT IN BOTH GAMEOBJECTS)
void PhysxSimulationEventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{

}

void PhysxSimulationEventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
{

}

void PhysxSimulationEventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
{

}

void PhysxSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{

}

void PhysxSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{

}

void PhysxSimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{

}