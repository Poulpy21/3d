
#version 330

in vec2 voxelLowerLeftXY; //[0.1]x[0,1]

out VS_GS_VERTEX {
	vec3 uvw; //[0,1]x[0,1]x[0,1]
	vec4 f0123; //density values
	vec4 f4567;
	uint caseId; //marching cube case 0..255
} vertex_out;

uniform sampler3D density;

layout(std140) uniform generalData {
	vec3 textureSize;
	vec3 voxelGridSize;
	vec3 voxelDim;
};

void main(void) {

	vec3 voxelLowerLeft = vec3(voxelLowerLeftXY, gl_InstanceID/voxelGridSize.z); 

	vec4 step = vec4(1.0/voxelGridSize.xyz, 0.0);

	vec4 f0123 = vec4(
		texture(density, voxelLowerLeft - step.www).x,
		texture(density, voxelLowerLeft - step.wwz).x,
		texture(density, voxelLowerLeft - step.xwz).x,
		texture(density, voxelLowerLeft - step.xww).x
		);
	
	vec4 f4567 = vec4(
		texture(density, voxelLowerLeft - step.wyw).x,
		texture(density, voxelLowerLeft - step.wyz).x,
		texture(density, voxelLowerLeft - step.xyz).x,
		texture(density, voxelLowerLeft - step.xyw).x
		);

	uvec4 n0123 = uvec4(clamp(f0123*99999, 0.0, 1.0));
	uvec4 n4567 = uvec4(clamp(f4567*99999, 0.0, 1.0));

	uint caseId = (n0123.x << 0) | (n0123.y << 1) | (n0123.z << 2) | (n0123.w << 3)
		  | (n4567.x << 4) | (n4567.y << 5) | (n4567.z << 6) | (n4567.w << 7);

	
	vertex_out.uvw = voxelLowerLeft;
	vertex_out.f0123 = f0123;
	vertex_out.f4567 = f4567;
	vertex_out.caseId = caseId;
}
