#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos; 
	layout (location = 1) in mat4 model;
	
	uniform mat4 dirLightSpaceMatrix;

	void main()
	{
		gl_Position = dirLightSpaceMatrix * model * vec4(aPos, 1.0);
	} 
#endif
#ifdef FRAGMENT_PROGRAM
	void main()
	{
	}
#endif