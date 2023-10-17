#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <btBulletDynamicsCommon.h>

struct Vertices {
    GLfloat *vertices;
    GLint size;
};

class DebugDraw : public btIDebugDraw {
   public:
    GLfloat vertices_[32768];
    GLint index_ = 0;

    DebugDraw() {}

    virtual int getDebugMode(void) const { return btIDebugDraw::DBG_DrawWireframe; }

    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
        vertices_[index_++] = from.x();
        vertices_[index_++] = from.y();
        vertices_[index_++] = from.z();

        vertices_[index_++] = to.x();
        vertices_[index_++] = to.y();
        vertices_[index_++] = to.z();
    }

    virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
                                  btScalar distance, int lifeTime, const btVector3 &color) {}

    virtual void reportErrorWarning(const char *warningString) {}

    virtual void draw3dText(const btVector3 &location, const char *textString) {}

    virtual void setDebugMode(int debugMode) {}
};

class Physics {
   public:
    Physics() {
        broadphase_ = new btDbvtBroadphase();
        collision_configuration_ = new btDefaultCollisionConfiguration();
        dispatcher_ = new btCollisionDispatcher(collision_configuration_);
        solver_ = new btSequentialImpulseConstraintSolver();
        world_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_,
                                             collision_configuration_);
        world_->setGravity(btVector3(0, -9.8, 0));
    }

    void SpawnCapsule(int index, btTransform transform) {
        btCollisionShape *shape = new btCapsuleShape(0.5, 1);

        btVector3 inertia = {0.0f, 0.0f, 0.0f};

        shape->calculateLocalInertia(30.0f, inertia);

        btMotionState *motion_state = new btDefaultMotionState(transform);

        btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(30.0f, motion_state,
                                                                           shape, inertia);

        btRigidBody *body = new btRigidBody(rigidBodyConstructionInfo);

        body->setFriction(0.5f);
        body->setRestitution(0.0f);
        body->setRollingFriction(0.5f);
        body->setSpinningFriction(0.5f);

        body->forceActivationState(DISABLE_DEACTIVATION);
        body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

        body->setUserIndex(index);

        world_->addRigidBody(body);
    }

    void SpawnBox(int index, btTransform transform) {
        btCollisionShape *shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

        btVector3 inertia = {0.0f, 0.0f, 0.0f};

        shape->calculateLocalInertia(30.0f, inertia);

        btMotionState *motion_state = new btDefaultMotionState(transform);

        btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(30.0f, motion_state,
                                                                           shape, inertia);

        btRigidBody *body = new btRigidBody(rigidBodyConstructionInfo);

        body->setFriction(0.5f);
        body->setRestitution(0.0f);
        body->setRollingFriction(0.5f);
        body->setSpinningFriction(0.5f);

        body->forceActivationState(DISABLE_DEACTIVATION);
        body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

        body->setUserIndex(index);

        world_->addRigidBody(body);
    }

    Vertices GetCapsuleVertices() {
        btCollisionShape *shape = new btCapsuleShape(0.5, 1);

        btVector3 inertia = {0.0f, 0.0f, 0.0f};

        shape->calculateLocalInertia(30.0f, inertia);

        btMotionState *motion_state = new btDefaultMotionState();

        btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(30.0f, motion_state,
                                                                           shape, inertia);

        btRigidBody *body = new btRigidBody(rigidBodyConstructionInfo);

        btBroadphaseInterface *broadphase = new btDbvtBroadphase();
        btDefaultCollisionConfiguration *collision_configuration =
            new btDefaultCollisionConfiguration();
        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collision_configuration);
        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

        btDiscreteDynamicsWorld *world =
            new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);

        DebugDraw debug_draw;

        world->setDebugDrawer(&debug_draw);

        world->addRigidBody(body);

        world->debugDrawWorld();

        world->removeRigidBody(body);

        Vertices result;

        GLfloat *vertices = new GLfloat[debug_draw.index_];

        memcpy(vertices, debug_draw.vertices_, sizeof(GLfloat) * debug_draw.index_);

        result.vertices = vertices;
        result.size = debug_draw.index_ / 3;

        delete world;
        delete solver;
        delete dispatcher;
        delete collision_configuration;
        delete broadphase;

        return result;
    }

    Vertices GetBoxVertices() {
        btCollisionShape *shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

        btVector3 inertia = {0.0f, 0.0f, 0.0f};

        shape->calculateLocalInertia(30.0f, inertia);

        btMotionState *motion_state = new btDefaultMotionState();

        btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(30.0f, motion_state,
                                                                           shape, inertia);

        btRigidBody *body = new btRigidBody(rigidBodyConstructionInfo);

        btBroadphaseInterface *broadphase = new btDbvtBroadphase();
        btDefaultCollisionConfiguration *collision_configuration =
            new btDefaultCollisionConfiguration();
        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collision_configuration);
        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

        btDiscreteDynamicsWorld *world =
            new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);

        DebugDraw debug_draw;

        world->setDebugDrawer(&debug_draw);

        world->addRigidBody(body);

        world->debugDrawWorld();

        world->removeRigidBody(body);

        Vertices result;

        GLfloat *vertices = new GLfloat[debug_draw.index_];

        memcpy(vertices, debug_draw.vertices_, sizeof(GLfloat) * debug_draw.index_);

        result.vertices = vertices;
        result.size = debug_draw.index_ / 3;

        delete world;
        delete solver;
        delete dispatcher;
        delete collision_configuration;
        delete broadphase;

        return result;
    }

    void Tick() {}

    ~Physics() {
        delete world_;
        delete solver_;
        delete dispatcher_;
        delete collision_configuration_;
        delete broadphase_;
    }

   private:
    btBroadphaseInterface *broadphase_;
    btDefaultCollisionConfiguration *collision_configuration_;
    btCollisionDispatcher *dispatcher_;
    btSequentialImpulseConstraintSolver *solver_;
    btDiscreteDynamicsWorld *world_;
};

#endif