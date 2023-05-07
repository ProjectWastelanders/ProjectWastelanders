#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 normals;
	layout (location = 3) in ivec4 boneIds;
	layout (location = 4) in vec4 weights;

	const int MAX_BONES = 100;
	const int MAX_BONE_INFLUENCE = 4;
	uniform mat4 finalBonesMatrices[MAX_BONES];
	
	uniform mat4 dirLightSpaceMatrix;
	uniform mat4 model;

	void main()
	{
		vec4 totalPosition = vec4(0.0f);
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (boneIds[i] == -1) 
				continue;

			if (boneIds[i] >= MAX_BONES)
			{
				totalPosition = vec4(aPos, 1.0f);
				break;
			}

			vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0f);
			totalPosition += localPosition * weights[i];
			vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normals;
		}

		if (totalPosition.x == 0.0f && totalPosition.y == 0.0f && totalPosition.z == 0.0f && totalPosition.w == 0.00f)
		{
			totalPosition = vec4(aPos, 1.0f);
		}
	
	
		gl_Position = dirLightSpaceMatrix * model * totalPosition;
	} 
#endif
#ifdef FRAGMENT_PROGRAM
	void main()
	{
	}
#endif