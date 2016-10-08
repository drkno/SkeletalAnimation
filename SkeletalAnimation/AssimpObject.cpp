#include "AssimpObject.h"

const aiScene* AssimpObject::loadModel(const char* fileName)
{
	auto scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr) exit(1);
	if (isAnimation) {
		secsPerTick = scene->mAnimations[0]->mTicksPerSecond == 0.0 ? 0.02 : 1.0 / scene->mAnimations[0]->mTicksPerSecond;
		animDuration = scene->mAnimations[0]->mDuration;
		get_bounding_box(scene, &scene_min, &scene_max);
	}
	for (auto i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		for (auto j = 0; j < mesh->mNumVertices; j++) {
			vertices.push_back(mesh->mVertices[j]);
			normals.push_back(mesh->mNormals[j]);
		}
	}
	return scene;
}

aiVector3D AssimpObject::getRootPos()
{
	return model->mRootNode->mTransformation * model->mMeshes[0]->mVertices[0];
}

AssimpObject::AssimpObject(const char* filename, bool iA, bool uB)
{
	isAnimation = iA;
	useBones = uB;
	model = loadModel(filename);
}

void AssimpObject::setSecsPerTick(float spt)
{
	secsPerTick = spt;
}

void AssimpObject::updateVerts()
{
	aiMesh* mesh;
	aiBone* bone;

	auto off = 0;

	for (auto i = 0; i < model->mNumMeshes; i++)
	{
		mesh = model->mMeshes[i];
		for (auto j = 0; j < mesh->mNumBones; j++)
		{
			bone = mesh->mBones[j];
			auto fullTransformationMatrix = bone->mOffsetMatrix;
			auto node = model->mRootNode->FindNode(bone->mName);
			do
			{
				fullTransformationMatrix = node->mTransformation * fullTransformationMatrix;
				node = node->mParent;
			} while (node != nullptr);
			auto transposeMatrix = fullTransformationMatrix;
			transposeMatrix.Transpose();
			transposeMatrix.Inverse();

			for (auto k = 0; k < bone->mNumWeights; k++)
			{
				auto vid = bone->mWeights[k].mVertexId;
				mesh->mVertices[vid] = fullTransformationMatrix * vertices[vid + off];
				mesh->mNormals[vid] = transposeMatrix * normals[vid + off];
			}
		}
		off += mesh->mNumVertices;
	}
}

void AssimpObject::updateAnimation(unsigned int tick)
{
	if (!isAnimation) return;
	auto anim = model->mAnimations[0];
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
		auto node = model->mRootNode->FindNode(chnl->mNodeName);
		node->mTransformation = matprod;
	}
	if (useBones)
		updateVerts();
}

void AssimpObject::render() const
{
	render(model, model->mRootNode);
}

void AssimpObject::releaseResources() const
{
	aiReleaseImport(model);
}

void AssimpObject::render(const aiScene* sc, const aiNode* nd) const
{
	aiMatrix4x4 m = nd->mTransformation;
	aiMesh* mesh;
	aiFace* face;

	aiTransposeMatrix4(&m); //Convert to column-major order
	glPushMatrix();
	glMultMatrixf(reinterpret_cast<float*>(&m)); //Multiply by the transformation matrix for this node

	// Draw all meshes assigned to this node
	for (auto n = 0; n < nd->mNumMeshes; n++)
	{
		mesh = sc->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if (mesh->HasNormals())
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);


		if (mesh->HasVertexColors(0))
			glEnable(GL_COLOR_MATERIAL);
		else
			glDisable(GL_COLOR_MATERIAL);

		//Get the polygons from each mesh and draw them
		for (auto k = 0; k < mesh->mNumFaces; k++)
		{
			face = &mesh->mFaces[k];
			GLenum face_mode;

			switch (face->mNumIndices)
			{
			case 1: face_mode = GL_POINTS;
				break;
			case 2: face_mode = GL_LINES;
				break;
			case 3: face_mode = GL_TRIANGLES;
				break;
			default: face_mode = GL_POLYGON;
				break;
			}

			glBegin(face_mode);

			for (auto i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				if (mesh->HasVertexColors(0))
				{
					glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
					glColor4fv(reinterpret_cast<GLfloat*>(&mesh->mColors[0][index]));
				}
				else
					glDisable(GL_COLOR_MATERIAL);
				if (mesh->HasNormals())
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}
	}

	// Draw all children
	for (auto i = 0; i < nd->mNumChildren; i++)
		render(sc, nd->mChildren[i]);

	glPopMatrix();
}
