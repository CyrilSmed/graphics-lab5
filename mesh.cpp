/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>

#include "mesh.h"
#include "engine_common.h"

Mesh::MeshEntry::MeshEntry()
{
    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    NumIndices  = 0;
    MaterialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
    if (VB != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &IB);
    }
}

bool Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
                          const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();

<<<<<<< Updated upstream
    glGenBuffers(1, &VB);
  	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
    
    return GLCheckError();
=======
#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void Mesh::VertexBoneData::AddBoneData(uint BoneID, float Weight)
{
    for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i]     = BoneID;
            Weights[i] = Weight;
            return;
        }        
    }
    
    // should never get here - more bones than we have space for
    assert(0);
>>>>>>> Stashed changes
}

Mesh::Mesh()
{
<<<<<<< Updated upstream
=======
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = NULL;
>>>>>>> Stashed changes
}


Mesh::~Mesh()
{
    Clear();
}


void Mesh::Clear()
{
    for (uint i = 0 ; i < m_Textures.size() ; i++) {
        SAFE_DELETE(m_Textures[i]);
    }
}


bool Mesh::LoadMesh(const std::string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();
    
<<<<<<< Updated upstream
    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
                                                                aiProcess_GenSmoothNormals |
                                                                aiProcess_FlipUVs |
                                                                aiProcess_CalcTangentSpace);    
    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
=======
    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;    
  
    m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (m_pScene) {  
        m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();
        Ret = InitFromScene(m_pScene, Filename);
>>>>>>> Stashed changes
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
    }

    return Ret;
}

<<<<<<< Updated upstream
bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
=======

bool Mesh::InitFromScene(const aiScene* pScene, const string& Filename)
>>>>>>> Stashed changes
{  
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

<<<<<<< Updated upstream
=======
    vector<Vector3f> Positions;
    vector<Vector3f> Normals;
    vector<Vector2f> TexCoords;
    vector<VertexBoneData> Bones;
    vector<uint> Indices;
       
    uint NumVertices = 0;
    uint NumIndices = 0;
    
    // Count the number of vertices and indices
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
        m_Entries[i].NumIndices    = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex    = NumVertices;
        m_Entries[i].BaseIndex     = NumIndices;
        
        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);
        
>>>>>>> Stashed changes
    // Initialize the meshes in the scene one by one
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
<<<<<<< Updated upstream
        InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
    
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
=======
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
    }

    if (!InitMaterials(pScene, Filename)) {
        return false;
    }

    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    return GLCheckError();
}


void Mesh::InitMesh(uint MeshIndex,
                    const aiMesh* paiMesh,
                    vector<Vector3f>& Positions,
                    vector<Vector3f>& Normals,
                    vector<Vector2f>& TexCoords,
                    vector<VertexBoneData>& Bones,
                    vector<uint>& Indices)
{    
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    // Populate the vertex attribute vectors
    for (uint i = 0 ; i < paiMesh->mNumVertices ; i++) {
>>>>>>> Stashed changes
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D* pTangent  = &(paiMesh->mTangents[i]);

<<<<<<< Updated upstream
        Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
                 Vector2f(pTexCoord->x, pTexCoord->y),
                 Vector3f(pNormal->x, pNormal->y, pNormal->z),
                 Vector3f(pTangent->x, pTangent->y, pTangent->z));
        
        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
=======
        Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));        
    }
    
    LoadBones(MeshIndex, paiMesh, Bones);
    
    // Populate the index buffer
    for (uint i = 0 ; i < paiMesh->mNumFaces ; i++) {
>>>>>>> Stashed changes
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
    
    m_Entries[Index].Init(Vertices, Indices);
}

<<<<<<< Updated upstream
bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
=======

void Mesh::LoadBones(uint MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones)
{
    for (uint i = 0 ; i < pMesh->mNumBones ; i++) {                
        uint BoneIndex = 0;        
        string BoneName(pMesh->mBones[i]->mName.data);
        
        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;            
	        BoneInfo bi;			
			m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;            
            m_BoneMapping[BoneName] = BoneIndex;
        }
        else {
            BoneIndex = m_BoneMapping[BoneName];
        }                      
        
        for (uint j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++) {
            uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight  = pMesh->mBones[i]->mWeights[j].mWeight;                   
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }    
}


bool Mesh::InitMaterials(const aiScene* pScene, const string& Filename)
>>>>>>> Stashed changes
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (uint i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!m_Textures[i]->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
                }
            }
        }
    }

    return Ret;
}

void Mesh::Render()
{
<<<<<<< Updated upstream
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                 // position
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // texture coordinate
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); // normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32); // tangent

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
=======
    glBindVertexArray(m_VAO);
    
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        const uint MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());
        
        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

		glDrawElementsBaseVertex(GL_TRIANGLES, 
                                 m_Entries[i].NumIndices, 
                                 GL_UNSIGNED_INT, 
                                 (void*)(sizeof(uint) * m_Entries[i].BaseIndex), 
                                 m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}


uint Mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


uint Mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


uint Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
>>>>>>> Stashed changes
        }
    }
    
    assert(0);

    return 0;
}


<<<<<<< Updated upstream
        glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
=======
void Mesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
>>>>>>> Stashed changes
    }
            
    uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

<<<<<<< Updated upstream
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}
=======

void Mesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }
    
    uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void Mesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void Mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
{    
    string NodeName(pNode->mName.data);
    
    const aiAnimation* pAnimation = m_pScene->mAnimations[0];
        
    Matrix4f NodeTransformation(pNode->mTransformation);
     
    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
    
    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        Matrix4f ScalingM;
        ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);
        
        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);        
        Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        Matrix4f TranslationM;
        TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
        
        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }
       
    Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;
    
    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
        uint BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
    }
    
    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}


void Mesh::BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms)
{
    Matrix4f Identity;
    Identity.InitIdentity();
    
    float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    Transforms.resize(m_NumBones);

    for (uint i = 0 ; i < m_NumBones ; i++) {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}


const aiNodeAnim* Mesh::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }
    
    return NULL;
}
>>>>>>> Stashed changes
