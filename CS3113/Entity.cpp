#include "Entity.h"
#include <cmath>
#include <string>

// ---------- Constructors ----------
Entity::Entity()
    : mPosition{0.0f, 0.0f}, mScale{DEFAULT_SIZE, DEFAULT_SIZE},
      mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE}, mTextureType{SINGLE},
      mCurrentTexture{}, mShipState{IDLE},
      mVelocity{0.0f, 0.0f}, mAcceleration{0.0f, 0.0f},
      mGravity{120.0f}, mThrust{320.0f},
      mFuel{100.0f}, mMaxFuel{100.0f}, mFuelUsage{22.0f},
      mActive{true}, mLandedSafe{false}, mInvalidLand{false},
      mphysicsCalculations{0.0f}, msafeLands{70.0f}, mvelocity{80.0f}
{
}
Entity::Entity(Vector2 position, Vector2 scale,
               TextureType textureType,)
    : mPosition{position}, mScale{scale},
      mColliderDimensions{scale}, mTextureType{textureType},
      mShipState{IDLE},
      mVelocity{0.0f, 0.0f}, mAcceleration{0.0f, 0.0f},
      mGravity{120.0f}, mThrust{320.0f},
      mFuel{100.0f}, mMaxFuel{100.0f}, mFuelIntake{22.0f},
      mActive{true}, mLandedSafe{false}, mInvalidLand{false},
      mphysicsCalculations{0.0f}, msafeLands{70.0f}, mvelocity{80.0f}
{
   
    mTextures[IDLE]      = LoadTexture("assets/rocket.png");
    mTextures[FLYING] = mTextures[IDLE];
    mCurrentTexture       = mTextures[IDLE];


    
}

// ---------- renders ----------
void Entity::render() {
    if (mTextures.find(IDLE) != mTextures.end()) {
        Texture2Dthrust= mTextures[mShipState];
        Rectangle src = {0.0f, 0.0f, (float)t.width, (float)t.height};
        Rectangle dest = {mPosition.x, mPosition.y, mScale.x, mScale.y};
        Vector2 origin = {mScale.x / 2.0f, mScale.y / 2.0f};
        DrawTexturePro(t, src, dest, origin, 0.0f, WHITE);
    } 
}

void Entity::renderColliderDebug() const {
    Rectangle r = {mPosition.x - mColliderDimensions.x * 0.5f,
                   mPosition.y - mColliderDimensions.y * 0.5f,
                   mColliderDimensions.x, mColliderDimensions.y};
    DrawRectangleLinesEx(r, 2.0f, RED);
}

// ---------- The  Physics ----------
void Entity::applyThrust(Vector2 thrust, float dt) {
    if (mFuel <= 0.0f) return;

    // apply acceleration from thrust
    mAcceleration.x += thrust.x;
    mAcceleration.y += thrust.y;

    // consume fuel proportional to time
    mFuel -= mFuelIntake * dt;
    if (mFuel < 0.0f) mFuel = 0.0f;

    // switch to FLYING state if applicable
    mShipState = FLYING;
    if (mTextures.find(FLYING) != mTextures.end())
        mCurrentTexture = mTextures[FLYING];
}

void Entity::update(float deltaTime) {
    // fixed timestep integration
    mphysicsCalculations += deltaTime;
    while (mphysicsCalculations >= mFixedDt) {
        // apply gravity
        mAcceleration.y += mGravity;

        // combining acceleration and velocity
        mVelocity.x += mAcceleration.x * mFixedDt;
        mVelocity.y += mAcceleration.y * mFixedDt;


        const float damping = 1.0f;
        mVelocity.x *= (1.0f / (1.0f + damping * mFixedDt));

        // combining velocity nd position
        mPosition.x += mVelocity.x * mFixedDt;
        mPosition.y += mVelocity.y * mFixedDt;

        // reset acceleration
        mAcceleration = {0.0f, 0.0f};
        mphysicsCalculations -= mFixedDt;
    }

    // revert to idle texture if not in FLYING state
    if (mShipState == FLYING) {
        mShipState = IDLE;
        if (mTextures.find(IDLE) != mTextures.end())
            mCurrentTexture = mTextures[IDLE];
    }
}

// ---------- collisions detections ----------
bool Entity::isColliding(Entity* other) const {
    float ax = mPosition.x, ay = mPosition.y;
    float aw = mColliderDimensions.x, ah = mColliderDimensions.y;
    float bx = other->mPosition.x, by = other->mPosition.y;
    float bw = other->mColliderDimensions.x, bh = other->mColliderDimensions.y;

    float leftA = ax - aw * 0.5f, rightA = ax + aw * 0.5f;
    float topA = ay - ah * 0.5f, bottomA = ay + ah * 0.5f;
    float leftB = bx - bw * 0.5f, rightB = bx + bw * 0.5f;
    float topB = by - bh * 0.5f, bottomB = by + bh * 0.5f;

    if (leftA > rightB || rightA < leftB || topA > bottomB || bottomA < topB)
        return false;
    return true;
}

void Entity::checkLandingOrCrash(Entity* platform) {
    if (!mActive || !isColliding(platform)) return;

    float playerBottom = mPosition.y + (mColliderDimensions.y * 0.5f);
    float platformTop = platform->mPosition.y - (platform->mColliderDimensions.y * 0.5f);

    if (playerBottom <= platformTop + 8.0f) {
        float verticalSpeed = fabs(mVelocity.y);
        float lateralSpeed = fabs(mVelocity.x);
        if (verticalSpeed <= msafeLands && lateralSpeed <= mvelocity) {
            mLandedSafe = true;
            mActive = false;
        } else {
            mInvalidLand = true;
            mActive = false;
        }
    } else {
        mInvalidLand = true;
        mActive = false;
    }
}
