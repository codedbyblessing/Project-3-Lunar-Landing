#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"
#include <string>

class Entity
{
private:
    // Transform
    Vector2 mPosition;
    Vector2 mScale;
    Vector2 mColliderDimensions;

    // Texture
    Texture2D mTexture;
    TextureType mTextureType;

    // Physics
    Vector2 mVelocity;
    Vector2 mAcceleration;
    float   mGravity;
    float   mThrust;
    float   mFuel;
    float   mMaxFuel;
    float   mFuelUsage;   // fuel consumed per second

    // Game state
    bool    mActive;
    bool    mLandedSafe;
    bool    mInvalidLand;

    // Fixed timestep
    float   mPhysicsAccumulator;
    const float mFixedDeltaT = 1.0f / 60.0f;

    // Safe landing 
    float   mSlowLanding;
    float   mVelocity;

public:
    // Constructors
    Entity();
    Entity(Vector2 position, Vector2 scale, const char* texturePath);

    // Setters / getters
    Vector2 getPosition() const { return mPosition; }
    Vector2 getScale() const { return mScale; }
    Vector2 getColliderDimensions() const { return mColliderDimensions; }

    void setPosition(Vector2 pos) { mPosition = pos; }
    void setScale(Vector2 s) { mScale = s; }
    void setColliderDimensions(Vector2 d) { mColliderDimensions = d; }

    // This updates the delta time and speed of the image based off of its thrust
    void update(float deltaTime);
    void applyThrust(Vector2 thrustVec, float dt);

    //collision detection
    bool isColliding(Entity* other) const;
    void checkLandingOrCrash(Entity* platform);

    // fuel getters and setters
    void setFuel(float f) { mFuel = f; mMaxFuel = f; }
    float getFuel() const { return mFuel; }
    float getMaxFuel() const { return mMaxFuel; }

    bool isAlive() const { return mActive; }
    bool missionAccomplished() const { return mLandedSafe; }
    bool missionFailed() const { return mInvalidLand; }

    // THis accounts for the ships dyanimic moving
    //In the original game the ship lands slowly so that acceleration/velocty needs to be calcualted carefully
    void setGravity(float gravity) { mGravity = gravity; }
    void setThrust(float thrust) { mThrust = thrust; }
    void setFuelIntake(float fuel_intake) { mFuelUsage = fuel_intake; }
    void setSlowLanding(float velocity) { mSlowLanding = velocity; }
    void setSafeLateralVel(float lat_velocity) { mVelocity = lat_velocity; }


    void render();
    void renderColliderDebug() const;
};

#endif
