#include "BoneObject.h"

void BoneObject::loadModel(string fileName, bool isAnimation)
{
	this->object = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (this->object == nullptr) exit(1);
	if (isAnimation) {
		secsPerTick = this->object->mAnimations[0]->mTicksPerSecond == 0.0 ? 0.02 : 1.0 / this->object->mAnimations[0]->mTicksPerSecond;
		get_bounding_box(this->object, &scene_min, &scene_max);
	}
	for (auto i = 0; i < this->object->mNumMeshes; i++)
	{
		auto mesh = this->object->mMeshes[i];
		for (auto j = 0; j < mesh->mNumVertices; j++) {
			vertices.push_back(mesh->mVertices[j]);
			normals.push_back(mesh->mNormals[j]);
		}
	}
}

void BoneObject::update(int value)
{
	for (auto i = 0; i < this->object->mNumAnimations; i++)
	{
		auto anim = this->object->mAnimations[i];
		for (auto j = 0; j < anim->mNumChannels; j++)
		{
			auto chnl = anim->mChannels[j];
			auto posn = chnl->mPositionKeys[tick % chnl->mNumPositionKeys].mValue;
			auto rotn = chnl->mRotationKeys[tick % chnl->mNumRotationKeys].mValue;
			auto matPos = aiMatrix4x4();
			matPos.Translation(posn, matPos);
			auto matRotn3 = rotn.GetMatrix();
			auto matRot = aiMatrix4x4(matRotn3);
			auto matprod = matPos * matRot;
			auto node = this->object->mRootNode->FindNode(chnl->mNodeName);
			node->mTransformation = matprod;
		}
	}

	auto self = this;
	auto up = bind(&BoneObject::update, &self);
	glutTimerFunc(0, up, 0);

	function<void(int)> func = update;
	
	tick = (tick + 1) % static_cast<int>(anim->mDuration);
	glutPostRedisplay();
	glutTimerFunc(secsPerTick * 1000, update, 0);
}

BoneObject::BoneObject(string file)
{
	loadModel(file, true);
}


BoneObject::~BoneObject()
{
}









#define HALF_WIDTH 300
#define HALF_DEPTH 200



void updateVerts(const aiScene* scene)
{
	auto offset = 0;

	for (auto i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		for (auto j = 0; j < mesh->mNumBones; j++)
		{
			auto bone = mesh->mBones[j];
			auto offsetMatrix = bone->mOffsetMatrix;
			auto currentNode = scene->mRootNode->FindNode(bone->mName);
			while (currentNode != nullptr)
			{
				offsetMatrix = currentNode->mTransformation * offsetMatrix;
				currentNode = currentNode->mParent;

			}
			auto transposeMatrix = offsetMatrix;
			transposeMatrix.Transpose();
			transposeMatrix.Inverse();

			for (auto k = 0; k < bone->mNumWeights; k++)
			{
				auto vertex = bone->mWeights[k].mVertexId;
				mesh->mVertices[vertex] = offsetMatrix * vertices[vertex + offset];
				mesh->mNormals[vertex] = transposeMatrix * normals[vertex + offset];
			}
		}
		offset += mesh->mNumVertices;
	}
}

void render(const aiScene* sc, const aiNode* nd)
{
	aiMatrix4x4 m = nd->mTransformation;
	aiMesh* mesh;
	aiFace* face;

	aiTransposeMatrix4(&m); //Convert to column-major order
	glPushMatrix();
	glMultMatrixf(reinterpret_cast<float*>(&m)); //Multiply by the transformation matrix for this node

												 // Draw all meshes assigned to this node
	for (int n = 0; n < nd->mNumMeshes; n++)
	{
		mesh = sc->mMeshes[nd->mMeshes[n]];
		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
		mesh->HasNormals() ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		mesh->HasVertexColors(0) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

		//Get the polygons from each mesh and draw them
		for (int k = 0; k < mesh->mNumFaces; k++)
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

			for (int i = 0; i < face->mNumIndices; i++)
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
	for (int i = 0; i < nd->mNumChildren; i++)
		render(sc, nd->mChildren[i]);

	glPopMatrix();
}

void display()
{
	float pos[4] = { 50, 50, 50, 1 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// scale the whole asset to fit into our view frustum 
	float tmp = scene_max.x - scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	tmp = 1.f / tmp;


	auto total = 0;
	auto x = 0.0, y = 0.0, z = 0.0;
	for (auto i = 0; i < person->mNumMeshes; i++)
	{
		for (auto j = 0; j < person->mMeshes[i]->mNumVertices; j++)
		{
			total++;
			x += person->mMeshes[i]->mVertices[j].x;
			y += person->mMeshes[i]->mVertices[j].y;
			z += person->mMeshes[i]->mVertices[j].z;
		}
	}
	x /= total;
	y /= total;
	z /= total;
	gluLookAt(0, 0, 3, 0, 0, -5, 0, 1, 0);
	//gluLookAt(0, 0, 3, x, 0, z, 0, 1, 0);

	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
	glScalef(tmp, tmp, tmp);





	//	stage->display();

	updateVerts(person);
	render(person, person->mRootNode);
	glPushMatrix();
	glScalef(25, 25, 25);
	//render(thing, thing->mRootNode);
	glPopMatrix();



	auto verticies = person->mMeshes[0]->mVertices[0];
	cout << verticies.x << " " << verticies.y << " " << verticies.z << endl;
	//	verticies *= tmp;
	//gluLookAt(0, 0, 15, verticies.x, verticies.y, verticies.z, 0, 1, 0);

	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Assimp Test");
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	initialise();
	glutDisplayFunc(display);
	glutTimerFunc(secsPerTick * 1000, update, 0);
	glutMainLoop();

	aiReleaseImport(person);
	delete stage;
}
