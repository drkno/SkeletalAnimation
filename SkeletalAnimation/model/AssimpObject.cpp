#include "AssimpObject.h"

bool endsWith(string const &str, string const &end) {
	int l1 = str.length(), l2 = end.length();
	if (l1 >= l2) {
		return 0 == str.compare(l1 - l2, l2, end);
	}
	return false;
}

AssimpObject::AssimpObject(string filename, bool isAnimated)
{
	animated = isAnimated;
	bones = endsWith(filename, ".x");
	loadModel(filename);
}

AssimpObject::~AssimpObject()
{
	aiReleaseImport(obj);
}

void AssimpObject::loadModel(string fileName)
{
	obj = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (obj == nullptr)
	{
		throw "Model \"" + fileName + "\" could not be loaded.";
	}

	for (auto i = 0; i < obj->mNumMeshes; i++)
	{
		auto mesh = obj->mMeshes[i];
		for (auto j = 0; j < mesh->mNumVertices; j++)
		{
			vertices.push_back(mesh->mVertices[j]); // get all points and normals
			normals.push_back(mesh->mNormals[j]);
		}
	}

	if (animated)
	{
		secsPerTick = obj->mAnimations[0]->mTicksPerSecond == 0.0 ?
						DEFAULT_TICKS_PER_SECOND :
						1.0 / obj->mAnimations[0]->mTicksPerSecond;
		animDuration = obj->mAnimations[0]->mDuration;
		get_bounding_box(obj, &scene_min, &scene_max);
	}
}

void AssimpObject::update(unsigned int tick)
{
	if (!animated)
	{
		return; // this isn't an animation, so points are already correct
	}

	auto anim = obj->mAnimations[0];
	for (auto i = 0; i < anim->mNumChannels; i++)
	{
		auto chnl = anim->mChannels[i];
		auto posn = chnl->mPositionKeys[tick % chnl->mNumPositionKeys].mValue;
		auto rotn = chnl->mRotationKeys[tick % chnl->mNumRotationKeys].mValue;
		auto matPos = aiMatrix4x4();
		matPos.Translation(posn, matPos);
		auto matRotn3 = rotn.GetMatrix();
		auto matRot = aiMatrix4x4(matRotn3);
		auto matprod = matPos * matRot;
		auto node = obj->mRootNode->FindNode(chnl->mNodeName);
		node->mTransformation = matprod;
	}
	
	if (!bones)
	{
		return; // everything else is not needed
	}

	auto offset = 0;
	for (auto i = 0; i < obj->mNumMeshes; i++)
	{
		auto mesh = obj->mMeshes[i];
		for (auto j = 0; j < mesh->mNumBones; j++)
		{
			auto bone = mesh->mBones[j];
			auto currentNode = obj->mRootNode->FindNode(bone->mName);
			auto transformationMatrix = bone->mOffsetMatrix;
			while (currentNode != nullptr)
			{
				transformationMatrix = currentNode->mTransformation * transformationMatrix;
				currentNode = currentNode->mParent;
			}

			for (auto k = 0; k < bone->mNumWeights; k++)
			{
				auto v = bone->mWeights[k].mVertexId;
				mesh->mVertices[v] = transformationMatrix * vertices[v + offset];
				mesh->mNormals[v] = transformationMatrix * normals[v + offset];
			}
		}
		offset += mesh->mNumVertices;
	}
}

void AssimpObject::render() const
{
	render(obj, obj->mRootNode);
}

void AssimpObject::render(const aiScene* scene, const aiNode* node) const
{
	auto m = node->mTransformation;
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf(reinterpret_cast<float*>(&m));

	for (auto i = 0; i < node->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[node->mMeshes[i]];
		apply_material(scene->mMaterials[mesh->mMaterialIndex]);
		mesh->HasNormals() ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		mesh->HasVertexColors(0) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

		for (auto j = 0; j < mesh->mNumFaces; j++)
		{
			auto face = &mesh->mFaces[j];
			switch (face->mNumIndices)
			{
				case 1: glBegin(GL_POINTS); break;
				case 2: glBegin(GL_LINES); break;
				case 3: glBegin(GL_TRIANGLES); break;
				default: glBegin(GL_POLYGON); break;
			}

			for (auto k = 0; k < face->mNumIndices; k++)
			{
				int vertIndex = face->mIndices[k];
				if (mesh->HasVertexColors(0))
				{
					glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
					glColor4fv(reinterpret_cast<GLfloat*>(&mesh->mColors[0][vertIndex]));
				}
				else
				{
					glDisable(GL_COLOR_MATERIAL);
				}
					
				if (mesh->HasNormals())
				{
					glNormal3fv(&mesh->mNormals[vertIndex].x);
				}
				glVertex3fv(&mesh->mVertices[vertIndex].x);
			}

			glEnd();
		}
	}

	for (auto i = 0; i < node->mNumChildren; i++)
	{
		render(scene, node->mChildren[i]);
	}

	glPopMatrix();
}

aiVector3D AssimpObject::getObjectPosition() const
{
	return obj->mRootNode->mTransformation * obj->mMeshes[0]->mVertices[0];
}
