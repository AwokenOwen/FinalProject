#include "MeshRenderer.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include "assimp/postprocess.h"

MeshRenderer::MeshRenderer(Object* parent) : Component(parent) {
}

void MeshRenderer::update()
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i]->setMaterial(m_materials[i]);
        m_meshes[i]->setParent(this);
        m_meshes[i]->draw();
    }
}

void MeshRenderer::loadModel(const string &path)
{
    if (const vector<Mesh*> results = Resource.getMeshFromMap(path); !results.empty())
    {
        m_meshes = results;

        for (int i = 0; i < m_meshes.size(); ++i) {
            m_materials.push_back(new Material);
        }
        return;
    }

    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_PreTransformVertices | aiProcess_Triangulate | aiProcess_FlipUVs) ;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    processNode(scene->mRootNode, scene, path);
    Resource.addMeshToMap(path, m_meshes);
}

void MeshRenderer::setShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath, const int index) const {
    m_materials[index]->setShaderProgram(vertexShaderPath, fragmentShaderPath);
}

vector<Material *> MeshRenderer::getMaterials() const {
    return m_materials;
}

void MeshRenderer::setMaterial(Material *material, const int index) {
    m_materials[index] = material;
}

void MeshRenderer::processNode(const aiNode* node, const aiScene* scene, const string &path)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
        m_materials.push_back(new Material);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, path);
    }
}

void MeshRenderer::loadCubeMap(const vector<const char*> &paths)
{
    loadModel("assets/defaultAssets/Models/cube.fbx");
    m_materials[m_materials.size() - 1]->setSkyboxTexture(paths);
    m_materials[m_materials.size() - 1]->setMaterialType(MaterialType::CUBE_MAP);
}

Mesh* MeshRenderer::processMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Mesh::Vertex> vertices;
    vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.m_position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.m_normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.m_texCoords = vec;
        }
        else
            vertex.m_texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    auto _mesh = new Mesh(vertices, indices);
    _mesh->setParent(this);

    return _mesh;
}
