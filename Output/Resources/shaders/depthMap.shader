#version 410 core
#ifdef VERTEX_PROGRAM
	layout (location = 0) in vec3 aPos;
	
	uniform mat4 dirLightSpaceMatrix;
	uniform mat4 model;

	void main()
	{
		gl_Position = dirLightSpaceMatrix * model * vec4(aPos, 1.0);
	} 
#endif
#ifdef FRAGMENT_PROGRAM

	void main()
	{
		//FragColor = gl_FragCoord.z;
	}
#endif