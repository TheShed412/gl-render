#include "LoadedObject.hpp"
#include "ObjectLoader.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

#include <deque>
#include <algorithm>

using namespace Graphics::Managers;

/**
 * I need to add the ability to add mtl from objs here. 
 * http://www.programmersought.com/article/5130383977/
 * 
 * I need to taking the specular, ambient, diffuse light, and color from the mtl file.
 * I will also need new accompanying shader file to consume the light values properly. 
*/

using namespace Rendering;

LoadedObject::LoadedObject() {}
LoadedObject::~LoadedObject() {}

LoadedObject::LoadedObject(std::string filename) {
    this->filename = filename;
}
 
void LoadedObject::Create(){
    ctm = glm::mat4(1.0);
    lastAngles = glm::vec3(0.0);

    ctm_location = glGetUniformLocation(program, "ctm");
    projection_location = glGetUniformLocation(program, "projection");
    model_view_location = glGetUniformLocation(program, "modelView");

    loadObject();
}

void LoadedObject::setPosition(glm::vec3 pos) {
    // glm::vec4 top = this->ctm[0];
    // glm::vec4 midTop = this->ctm[1];
    // glm::vec4 midBot = this->ctm[2];
    glm::vec4 bot = this->ctm[3];
    // this->ctm = glm::mat4(
    //     top,
    //     midTop,
    //     midBot,
    //     glm::vec4(pos, bot.w)
    // );

    glm::vec3 posDiff = glm::vec3(pos.x - bot.x, pos.y - bot.y, pos.z - bot.z);
    this->SetTranslation(posDiff);
}

void LoadedObject::setScale(glm::vec3 scale) {
    // glm::vec4 top = this->ctm[0];
    // glm::vec4 midTop = this->ctm[1];
    // glm::vec4 midBot = this->ctm[2];

    this->SetScale(scale);
}

glm::vec3 toDeg(glm::vec3 inRads) {
    glm::vec3 inDeg = glm::vec3(0.0);

    inDeg.x = glm::degrees(inRads.x);
    inDeg.y = glm::degrees(inRads.y);
    inDeg.z = glm::degrees(inRads.z);

    return inDeg;
}

// Should be a vec3 of angles in degrees
void LoadedObject::setRotation(glm::vec3 eulerAngles) {

    float xRad = (lastAngles.x - eulerAngles.x);
    float yRad = (lastAngles.y - eulerAngles.y);
    float zRad = (lastAngles.z - eulerAngles.z);

    lastAngles = eulerAngles;

    Model::SetRotation(glm::quat(glm::vec3(xRad, yRad, zRad)));
    //Model::SetRotation(glm::quat(eulerAngles));
}

const glm::mat4* LoadedObject::GetCtm() {
    return &this->ctm;
}

void LoadedObject::SetCtm(glm::mat4* ctm) {
    this->ctm = *ctm;
}

// TODO: I need to pull this out in to a game logic class
void LoadedObject::processKeyboard(Movement direction, float deltaTime) {
    // TODO: get rid of this later, I'm lazy now lol
    // switch (direction)
    // {
    //     case FORWARD:
    //         //currentLocation += glm::vec3(0, 0, -0.1);
    //         ctm = glm::rotate(ctm, 0.05f, glm::vec3(-1.0, 0, 0));
    //         //camera->addToPosition(glm::vec3(0, 0, -0.1));
    //         //camera->setLookAt(currentLocation);
    //         break;
    //     case BACKWARD:
    //         ctm = glm::rotate(ctm, 0.05f, glm::vec3(1.0, 0, 0));
    //         //camera->setLookAt(currentLocation);
    //         break;
    //     case LEFT:
    //         ctm = glm::rotate(ctm, 0.05f, glm::vec3(0, 0, 1.0));
    //         break;
    //     case RIGHT:
    //         ctm = glm::rotate(ctm, 0.05f, glm::vec3(0, 0, -1.0));
    //         break;
        
    //     default:
    //         break;
    // }
}

void LoadedObject::Update(){
    model_view = camera->getModelView();
    //ctm = glm::translate(ctm, {0.1, 0, 0});
}

void LoadedObject::Draw(){
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(ctm, projection, model_view, this->light);
    }
    
}

std::vector<VertexFormat> LoadedObject::makeObject() {
    ObjectLoader* objLoader = new ObjectLoader();
    std::vector<VertexFormat> vertecies = objLoader->LoadObject(filename);
    vectors = vertecies.size();
    return vertecies;
}

void LoadedObject::processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

std::vector<TextureFormat> LoadedObject::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<TextureFormat> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures.size(); j++) {
            if(std::strcmp(textures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            TextureFormat texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

unsigned int LoadedObject::TextureFromFile(const char *path, const std::string &directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    Graphics::Managers::TexInfo* texData = this->textureManager->add(filename).get();
    unsigned char *data = texData->data;
    width = texData->width;
    height = texData->height;
    nrComponents = texData->nrComponents;
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        //stbi_image_free(data);
    }

    return textureID;
}

Mesh LoadedObject::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<VertexFormat> vertices;
    std::deque<unsigned int> indices;
    std::vector<TextureFormat> textures;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        VertexFormat vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if(mesh->mNormals) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture = vec;
        } else {
            vertex.texture = glm::vec2(0.0f, 0.0f);
        }
        // tangent
        if(mesh->mTangents) {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
        }
        // bitangent
        if(mesh->mBitangents) {
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitTangent = vector;
        }
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_front(face.mIndices[j]);
        }
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    Material mat;
	aiColor3D color;

    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    float shininess;
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat.Ka = glm::vec4(color.r, color.g, color.b,1.0);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	mat.Kd = glm::vec4(color.r, color.g, color.b,1.0);
	material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	mat.Ks = glm::vec4(color.r, color.g, color.b,1.0);
    material->Get(AI_MATKEY_SHININESS, shininess);
    mat.Ns = shininess;

    // std::cout<<glm::to_string(mat.Ka)<<std::endl;

    // 1. diffuse maps
    std::vector<TextureFormat> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<TextureFormat> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<TextureFormat> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<TextureFormat> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
    // return a mesh object created from the extracted mesh data
    // std::reverse(vertices.begin(), vertices.end());
    // std::reverse(indices.begin(), indices.end());
    // std::reverse(textures.begin(), textures.end());
    this->vertices = vertices;
    return Mesh(vertices, {indices.begin(), indices.end()}, textures, mat, program);
}

std::vector<VertexFormat> LoadedObject::loadObject() {
    std::vector<VertexFormat> vertecies;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        //return;
    }

    directory = filename.substr(0, filename.find_last_of('/'));

    processNode(scene->mRootNode, scene);

    return vertecies;
}

std::vector<VertexFormat> LoadedObject::getVerts() {
    return this->vertices;
}

void LoadedObject::setTextureManager(Graphics::Managers::TextureManager* manager) {
    this->textureManager = manager;
}