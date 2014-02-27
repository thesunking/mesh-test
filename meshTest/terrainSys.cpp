#include "terrainSys.h"
#include <array>
#include <cmath>


TerrainSys::TerrainSys() : side_length(200), scaleAmount(4.0f) {

	int rnd = rand();

	height0.set(0.00L, 0.100L, 20.00L, 1, rnd+1); //large trends
	height1.set(0.00L, 0.500L, 1.000L, 1, rnd+0); //mid   trends
	height2.set(0.00L, 1.000L, 0.200L, 1, rnd+2); //small trends

	temp.set(0.00L, 0.02L, 1.00L, 1, rnd+3);
	rain.set(0.00L, 0.02L, 1.00L, 1, rnd+4);

	createPerlinMesh();

}

double TerrainSys::getHeight(double x, double z) {

	double y0 = height0.getHeight(x, z);
	double y1 = height1.getHeight(x, z);
	double y2 = height2.getHeight(x, z);

	return y0 + y1 + y2;

}

void TerrainSys::createPerlinMesh() {

	printf("Creating terrain...");
	createHeightmap();
	printf(".");
	scaleMesh();
	printf(".");
	createIndices();
	printf(".");
	createNormals();
	printf(".");
	addTempAndRain();
	printf(".");
	coolHighPoints();
	printf(".");
	addRainShadows();
	printf(".");
	colorBiomes();
	printf(".");

	//calcs related to normal maps
	calcTextureCoords();
	printf(".");
	calcTangents();
	printf(".\n");


}


#pragma region HEIGHTMAP

void TerrainSys::createHeightmap() {

	for (int x = 0; x < side_length; ++x) {
		for (int z = 0; z < side_length; ++z) {
			
			float y = (float)getHeight(x, z);
			glm::vec3 point(x, y, z);

			points.push_back( point );
			
		}
	}

}

void TerrainSys::scaleMesh() {

	for (int i = 0; i < points.size(); ++i) {
		points[i].x *= scaleAmount;
		points[i].y *= scaleAmount;
		points[i].z *= scaleAmount;
	}

}

void TerrainSys::createIndices() {

	const unsigned int size = points.size();
	const unsigned int n    = sqrt(size);

	for (unsigned int x = 0; x < n-1; ++x) {
		for (unsigned int z = 0; z < n-1; ++z) {

			//index of corner of quad, other indices are found relative to this
			unsigned int i = x*n+z;

			//first triangle for quad
			indices.push_back( (GLuint)(i) );
			indices.push_back( (GLuint)(i+n) );
			indices.push_back( (GLuint)(i+n+1) );

			//second triange for quad
			indices.push_back( (GLuint)(i) );
			indices.push_back( (GLuint)(i+n+1) );
			indices.push_back( (GLuint)(i+1) );

		}
	}

}

void TerrainSys::createNormals() {

	unsigned int n = indices.size()/3;

	for (unsigned int i = 0; i < n; ++i) {
		
		glm::vec3 v0 = points[indices[3*i+2]] - points[indices[3*i]];
		glm::vec3 v1 = points[indices[3*i+1]] - points[indices[3*i]];
		glm::vec3 normal = glm::cross(v0, v1);
		normal = glm::normalize( normal );

		if ( normal.y < 0.0f ) { normal = -normal; }
		
		normals.push_back( normal );
		
	}

}

void TerrainSys::addTempAndRain() {

	for (std::vector<glm::vec3>::const_iterator i = points.cbegin(); i != points.cend(); ++i) {

		float t = temp.getHeight(i->x, i->z) + 0.5f;
		float r = rain.getHeight(i->x, i->z) + 0.5f;

		arrTemp.push_back(t);
		arrRain.push_back(r);

	}

}

void TerrainSys::coolHighPoints() {

	//adjust temperature for extreme heights
	for (int i = 0; i < points.size(); ++i) {
		float adj_ht = (points[i].y / scaleAmount);
		arrTemp[i] -= ( adj_ht * adj_ht * 0.01f );
	}

}

void TerrainSys::addRainShadows() {

	//adjust rainfall for rain shadows caused by mountains
	//assuming prevailing wind is in the +x direction
	for (int i = 0; i < points.size(); ++i) {
		
		float x = points[i].x / scaleAmount;
		float z = points[i].z / scaleAmount;

		float ht00 = getHeight( x       , z) * scaleAmount;
		float ht01 = getHeight( x + 1.0f, z) * scaleAmount;
		float ht02 = getHeight( x + 2.0f, z) * scaleAmount;
		float ht03 = getHeight( x + 3.0f, z) * scaleAmount;

		float dh01 = ht01 - ht00;
		float dh02 = ht02 - ht01;
		float dh03 = ht03 - ht02;

		float sum    = dh01 + dh02 + dh03;
		float scalar = 0.01f;

		arrRain[i] += sum * scalar;

	}

}

void TerrainSys::colorBiomes() {

	for (int i = 0; i < points.size(); ++i) {

		arrColor.push_back( arrTemp[i] );			//R
		arrColor.push_back( arrRain[i] );			//G
		arrColor.push_back( 1.0f - arrTemp[i] );	//B

	}

	arrTemp.clear();
	arrRain.clear();

}

#pragma endregion


void TerrainSys::calcTextureCoords() {

	for (int x = 0; x < side_length; ++x) {
		for (int z = 0; z < side_length; ++z) {

			//float u = (x % 2 == 0) ? 0.0f : 1.0f;
			//float v = (z % 2 == 0) ? 0.0f : 1.0f;

			float u = (float)x;
			float v = (float)z;


			uvs.push_back(glm::vec2(u, v));

		}
	}

}

void TerrainSys::calcTangents() {
	
	//n = number of triangles
	unsigned int n = indices.size() / 3;
	
	for (unsigned int i = 0; i < n; ++i) {

		glm::vec3 deltaPos1 = points[indices[3 * i + 2]] - points[indices[3 * i]];
		glm::vec3 deltaPos2 = points[indices[3 * i + 1]] - points[indices[3 * i]];

		glm::vec2 deltaUV1 = uvs[indices[3 * i + 2]] - uvs[indices[3 * i]];
		glm::vec2 deltaUV2 = uvs[indices[3 * i + 1]] - uvs[indices[3 * i]];

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		//if (tangent.x < 0.0f) { tangent.x = -tangent.x; }
		//if (tangent.y < 0.0f) { tangent.y = -tangent.y; }
		//if (tangent.z < 0.0f) { tangent.z = -tangent.z; }

		//if (bitangent.x < 0.0f) { bitangent.x = -bitangent.x; }
		//if (bitangent.y < 0.0f) { bitangent.y = -bitangent.y; }
		//if (bitangent.z < 0.0f) { bitangent.z = -bitangent.z; }

		tangents.push_back(tangent);
		bitangents.push_back(bitangent);

	}

	//now we have tangents and bitangents for every triangle
	//need to take those and calc an average tangent and bitangent for every vertex

	for (unsigned int vi = 0; vi < points.size(); ++vi) {

		//used to prevent trying to access a triangle that doesn't exist (e.g. when a vertex is on the edge, not all sides have adjacent triangles)
		bool triFlags[] = { true, true, true, true, true, true };

		if (floor(vi / side_length) == 0) {
			triFlags[0] = false;
			triFlags[1] = false;
			triFlags[2] = false;
		}

		if (vi % side_length == 0) {
			triFlags[0] = false;
			triFlags[1] = false;
			triFlags[3] = false;
		}

		if (floor(vi / side_length) == side_length - 1) {
			triFlags[3] = false;
			triFlags[4] = false;
			triFlags[5] = false;
		}

		if (vi % side_length == side_length - 1) {
			triFlags[2] = false;
			triFlags[4] = false;
			triFlags[5] = false;
		}

		glm::vec3 accum_T = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 accum_B = glm::vec3(0.0f, 0.0f, 0.0f);
		unsigned int number_of_averaged_values = 0;

		//'a' and 'b' are used to find the index of the triangles adjacent to the current vertex
		int a = 2 * (vi - floor(vi / side_length));
		int b = 2 * (vi - floor(vi / side_length)) - 2 * side_length;

		if (triFlags[0]) {
			accum_T += tangents[b];
			accum_B += bitangents[b];
			++number_of_averaged_values;
		}

		if (triFlags[1]) {
			accum_T += tangents[b + 1];
			accum_B += bitangents[b + 1];
			++number_of_averaged_values;
		}

		if (triFlags[2]) {
			accum_T += tangents[b + 2];
			accum_B += bitangents[b + 2];
			++number_of_averaged_values;
		}

		if (triFlags[3]) {
			accum_T += tangents[a - 1];
			accum_B += bitangents[a - 1];
			++number_of_averaged_values;
		}

		if (triFlags[4]) {
			accum_T += tangents[a];
			accum_B += bitangents[a];
			++number_of_averaged_values;
		}

		if (triFlags[5]) {
			accum_T += tangents[a + 1];
			accum_B += bitangents[a + 1];
			++number_of_averaged_values;
		}


		//add averaged tangent and bitangent to vectors
		avg_tan.push_back(glm::normalize(accum_T / (float)number_of_averaged_values));
		avg_bitan.push_back(glm::normalize(accum_B / (float)number_of_averaged_values));


	}


}


#pragma region GETS

std::vector<glm::vec3> TerrainSys::getPoints() { return points; }
std::vector<glm::vec3> TerrainSys::getNormals() { return normals; }
std::vector<float> TerrainSys::getColors() { return arrColor; }
std::vector<GLuint> TerrainSys::getIndices() { return indices; }
std::vector<glm::vec2> TerrainSys::getUVs() { return uvs; }
std::vector<glm::vec3> TerrainSys::getTangents() { return avg_tan; }
std::vector<glm::vec3> TerrainSys::getBitangents() { return avg_bitan; }

#pragma endregion




double TerrainSys::getFloorHeight(double x, double z) {

	//jumps directly to the required point in array
	//triangle needed will either be p_i or p_i+1
	int predicted_index = (int) (2 * ( floor(x/scaleAmount) * (side_length-1) + floor(z/scaleAmount) ));

	bool out_of_bounds = false;
	if (predicted_index < 0 ) {out_of_bounds = true;}
	else if (predicted_index >= (2*(side_length-1)*(side_length-1))) {out_of_bounds = true;}

	if (out_of_bounds == false) {
		for (int i = predicted_index; i < predicted_index+2; ++i) {

			unsigned int index0 = (unsigned int)(indices[3*i]);
			unsigned int index1 = (unsigned int)(indices[3*i+1]);
			unsigned int index2 = (unsigned int)(indices[3*i+2]);

			bool flag_x0 = (x > points[index0].x);
			bool flag_x1 = (x > points[index1].x);
			bool flag_x2 = (x > points[index2].x);

			bool flag_z0 = (z > points[index0].z);
			bool flag_z1 = (z > points[index1].z);
			bool flag_z2 = (z > points[index2].z);

			// if point is in the AABB of the triangle
			if (!( (flag_x0 == flag_x1 && flag_x0 == flag_x2) || (flag_z0 == flag_z1 && flag_z0 == flag_z2) )) {

				//compute vectors    
				glm::vec3 point = glm::vec3(x, 0.0f, z);
				glm::vec3 v0 = points[index2] - points[index0];
				glm::vec3 v1 = points[index1] - points[index0];
				glm::vec3 v2 = point		  - points[index0];

				//copy to flat triangle
				glm::vec3 v3 = v0;
				glm::vec3 v4 = v1;
				glm::vec3 v5 = v2;
				v3.y = 0.0f;
				v4.y = 0.0f;
				v5.y = 0.0f;

				//compute dot products
				float dot00 = glm::dot(v3,v3);
				float dot01 = glm::dot(v3,v4);
				float dot02 = glm::dot(v3,v5);
				float dot11 = glm::dot(v4,v4);
				float dot12 = glm::dot(v4,v5);

				//compute barycentric coordinates
				float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
				float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
				float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

				//check if point is in triangle
				if ((u >= 0) && (v >= 0) && (u + v < 1)) {

					return (points[index0].y + (points[index1].y - points[index0].y) * v + (points[index2].y - points[index0].y) * u);

				}
			}
		}
	}

	return 75.0L;

}

glm::vec3 TerrainSys::getNormal(double x, double z) {

	//jumps directly to the required point in array
	//triangle needed will either be p_i or p_i+1
	int predicted_index = (int) (2 * ( floor(x/scaleAmount) * (side_length-1) + floor(z/scaleAmount) ));

	bool out_of_bounds = false;
	if (predicted_index < 0 ) {out_of_bounds = true;}
	else if (predicted_index >= (2*(side_length-1)*(side_length-1))) {out_of_bounds = true;}

	if (out_of_bounds == false) {
		for (int i = predicted_index; i < predicted_index+2; ++i) {

			unsigned int index0 = (unsigned int)(indices[3*i]);
			unsigned int index1 = (unsigned int)(indices[3*i+1]);
			unsigned int index2 = (unsigned int)(indices[3*i+2]);

			bool flag_x0 = (x > points[index0].x);
			bool flag_x1 = (x > points[index1].x);
			bool flag_x2 = (x > points[index2].x);

			bool flag_z0 = (z > points[index0].z);
			bool flag_z1 = (z > points[index1].z);
			bool flag_z2 = (z > points[index2].z);

			// if point is in the AABB of the triangle
			if (!( (flag_x0 == flag_x1 && flag_x0 == flag_x2) || (flag_z0 == flag_z1 && flag_z0 == flag_z2) )) {

				//compute vectors    
				glm::vec3 point = glm::vec3(x, 0.0f, z);
				glm::vec3 v0 = points[index2] - points[index0];
				glm::vec3 v1 = points[index1] - points[index0];
				glm::vec3 v2 = point		  - points[index0];

				//copy to flat triangle
				glm::vec3 v3 = v0;
				glm::vec3 v4 = v1;
				glm::vec3 v5 = v2;
				v3.y = 0.0f;
				v4.y = 0.0f;
				v5.y = 0.0f;

				//compute dot products
				float dot00 = glm::dot(v3,v3);
				float dot01 = glm::dot(v3,v4);
				float dot02 = glm::dot(v3,v5);
				float dot11 = glm::dot(v4,v4);
				float dot12 = glm::dot(v4,v5);

				//compute barycentric coordinates
				float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
				float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
				float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

				//check if point is in triangle
				if ((u >= 0) && (v >= 0) && (u + v < 1)) {

					//point is in triangle!
					return normals[i];

				}
			}
		}
	} //end out_of_bounds if statement

	return glm::vec3(0.0f, 1.0f, 0.0f);

}
