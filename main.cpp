#include "Camera.h"
#include "DirectionalLight.h"
#include "GameManager.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "MovementComponent.h"
#include "Object.h"
#include "Scene.h"
#include "SlingShotComponent.h"
#include "SpiderManager.h"
#include "Spline.h"

int main(int argc, char *argv[]) {
    Game.initialize();

    // World Loading

    new DirectionalLight;

    Game.getActiveScene()->loadDefaultSkybox();
    Game.getActiveScene()->getCamera()->addComponent<MovementComponent>();
    Game.getActiveScene()->getCamera()->setLocalPosition(vec3(0, 1, 0));

    // Ground
    auto plane = new Object();
    plane->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/plane.fbx");
    plane->setLocalPosition(vec3(0, -1, 0));
    plane->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/grass.png");
    plane->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/Shaders/Grass.frag");

    // Spider wall
    auto wall = new Object();
    wall->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/cube.fbx");
    wall->setLocalPosition(vec3(0, 4.0f, 20));
    wall->setLocalScale(vec3(7, 5, 0.2f));
    wall->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/sphere_DefaultMaterial_BaseColor.png");
    wall->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/sphere_DefaultMaterial_Metallic.png");
    wall->getComponent<MeshRenderer>()->getMaterials()[0]->addTexture("assets/Textures/sphere_DefaultMaterial_Roughness.png");
    wall->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/Shaders/TexturedAlbedoMetallicRoughness.frag");

    // Slingshot
    auto sling = new Object();
    sling->addComponent<MeshRenderer>()->loadModel("assets/Models/SlingShot.fbx");
    sling->setLocalPosition(vec3(0, -1, 5));
    sling->setLocalScale(vec3(1.5f));
    sling->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/Shaders/SlingShot.frag");

    // Adding spline to wall
    wall->addComponent<SpiderManager>()->loadSpiders(4);

    sling->addComponent<SlingShotComponent>();

    Game.run();
    Game.terminate();
}
