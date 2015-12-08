#pragma once
#include <boost/numeric/ublas/matrix.hpp>

class MeshMorph
{
public:
	MeshMorph(void);
	~MeshMorph(void);

	void init(float* vertexData, unsigned short vertexNum, unsigned short* featureIndices, unsigned short featureNum);
	void getMorphedData(float* out, float* newFeaturePos_);
	void getMorphedData(float* out, unsigned short* vertexIndices, unsigned short vertexNum, unsigned short* featureIndices, unsigned short featureNum, float* newFeaturePos_, bool morphAllVertices = true);

protected:
	float* vertexData;
	unsigned short vertexNum;
	unsigned short* featureIndices;
	unsigned short featureNum;
	float* Wx; //RBF weight vector for x coordinates
	float* Wy; //RBF weight vector for y coordinates
	float* Wz; //RBF weight vector for z coordinates
	float* minDist2; //helper structure
	boost::numeric::ublas::matrix<float> RBFmInv;

	float dist2(float* p1, float* p2);
	void solveRBF(unsigned short* featureIndices, unsigned short featureNum, float* newFeaturePos);
	inline float RBF_multiquadric(float dist2, float eps);
	inline float RBF_inv_quadric(float dist2, float eps);
	inline float RBF_inv_multiquadric(float dist2, float eps);
};

