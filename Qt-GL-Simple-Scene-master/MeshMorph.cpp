#include "MeshMorph.h"
#include <cstring>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

#define RBF_KERNEL RBF_multiquadric
//#define RBF_KERNEL RBF_inv_quadric
//#define RBF_KERNEL RBF_inv_multiquadric

 /* Matrix inversion routine.
 Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
template<class T>
bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse)
{
	typedef permutation_matrix<std::size_t> pmatrix;

	// create a working copy of the input
	matrix<T> A(input);

	// create a permutation matrix for the LU-factorization
	pmatrix pm(A.size1());

	// perform LU-factorization
	int res = lu_factorize(A, pm);
	if (res != 0)
		return false;

	// create identity matrix of "inverse"
	inverse.assign(identity_matrix<T> (A.size1()));

	// backsubstitute to get the inverse
	lu_substitute(A, pm, inverse);

	return true;
}


MeshMorph::MeshMorph() : Wx(0),Wy(0),Wz(0)
{
}


MeshMorph::~MeshMorph(void)
{
}

void MeshMorph::init(float* vertexData_, unsigned short vertexNum_, unsigned short* featureIndices_, unsigned short featureNum_)
{
	vertexNum = vertexNum_;
	vertexData = new float[3 * vertexNum];
	memcpy(vertexData, vertexData_, vertexNum * 3 * sizeof(float));
	featureNum = featureNum_;
	featureIndices = new unsigned short[featureNum];
	memcpy(featureIndices, featureIndices_, featureNum * sizeof(unsigned short));

	//std::cout << RBFmInv << std::endl<< std::endl;

	//unsigned short* indices = new unsigned short[featureNum_];
	//for(unsigned short i = 0; i < featureNum_; ++i)
	//{
	//	indices[i] = i;
	//}
	//solveRBF(indices, featureNum_, newFeaturePos_);	
	//delete indices;
}

float MeshMorph::dist2(float* p1, float* p2)
{
	return (p1[0]-p2[0]) * (p1[0]-p2[0]) + (p1[1]-p2[1]) * (p1[1]-p2[1]) + (p1[2]-p2[2]) * (p1[2]-p2[2]);
}

void  MeshMorph::getMorphedData(float* out, float* newFeaturePos_)
{
	unsigned short* indices = new unsigned short[featureNum];
	for(unsigned short i = 0; i < featureNum; ++i)
	{
		indices[i] = i;
	}
	solveRBF(indices, featureNum, newFeaturePos_);	
	delete indices;
	for(int i = 0; i < vertexNum; ++i)
	{
		out[i*3 + 0] = 0;
		out[i*3 + 1] = 0;
		out[i*3 + 2] = 0;
		for(int j = 0; j < featureNum; ++j)
		{
			float d2 = dist2(&vertexData[i*3], &vertexData[featureIndices[j]*3]);
			out[i*3 + 0] += RBF_KERNEL(d2, minDist2[j]) * Wx[j];
			out[i*3 + 1] += RBF_KERNEL(d2, minDist2[j]) * Wy[j];
			out[i*3 + 2] += RBF_KERNEL(d2, minDist2[j]) * Wz[j];
		}
	}
}

void MeshMorph::getMorphedData(float* out, unsigned short* vertexIndices, unsigned short vertexNum_, unsigned short* featureIndices_, unsigned short featureNum_, float* newFeaturePos_, bool morphAllVertices)
{
	solveRBF(featureIndices_, featureNum_, newFeaturePos_);
	if(morphAllVertices)
	{
		//Copy all vertex positions
		memcpy(out, vertexData, vertexNum * 3 * sizeof(float));
		for(int i = 0; i < vertexNum_; ++i)
		{
			out[vertexIndices[i]*3 + 0] = 0;
			out[vertexIndices[i]*3 + 1] = 0;
			out[vertexIndices[i]*3 + 2] = 0;
			for(int j = 0; j < featureNum_; ++j)
			{
				float d2 = dist2(&vertexData[vertexIndices[i]*3], &vertexData[featureIndices[featureIndices_[j]]*3]);
				out[vertexIndices[i]*3 + 0] += RBF_KERNEL(d2, minDist2[j]) * Wx[j];
				out[vertexIndices[i]*3 + 1] += RBF_KERNEL(d2, minDist2[j]) * Wy[j];
				out[vertexIndices[i]*3 + 2] += RBF_KERNEL(d2, minDist2[j]) * Wz[j];
			}
		}
	}
	else
	{
		//only output the morphed vertices
		for(int i = 0; i < vertexNum_; ++i)
		{
			out[i*3 + 0] = 0;
			out[i*3 + 1] = 0;
			out[i*3 + 2] = 0;
			for(int j = 0; j < featureNum_; ++j)
			{
				float d2 = dist2(&vertexData[vertexIndices[i]*3], &vertexData[featureIndices[featureIndices_[j]]*3]);
				out[i*3 + 0] += RBF_KERNEL(d2, minDist2[j]) * Wx[j];
				out[i*3 + 1] += RBF_KERNEL(d2, minDist2[j]) * Wy[j];
				out[i*3 + 2] += RBF_KERNEL(d2, minDist2[j]) * Wz[j];
			}
		}
	}
}


void MeshMorph::solveRBF(unsigned short* featureIndices_, unsigned short featureNum_, float* newFeaturePos_)
{
	matrix<float> RBFm(featureNum_, featureNum_);
	minDist2 = new float[featureNum_];	
	for(int i = 0; i < featureNum_; ++i)
	{
		minDist2[i] = FLT_MAX;
		for(int j = 0; j < featureNum_; ++j)
		{
			float d2 = dist2(&vertexData[featureIndices[featureIndices_[i]] * 3], &vertexData[featureIndices[featureIndices_[j]] * 3]);
			RBFm(i, j) = d2;
			if(i != j && d2 < minDist2[i])
				minDist2[i] = d2;
		}
	}
	//std::cout << RBFm << std::endl << std::endl;
	for(int i = 0; i < featureNum_; ++i)
	{
		for(int j = 0; j < featureNum_; ++j)
		{
			RBFm(i, j) = RBF_KERNEL(RBFm(i, j), minDist2[i]);
		}
	}

	//std::cout << RBFm << std::endl<< std::endl;

	RBFmInv.resize(featureNum_, featureNum_);
	
	InvertMatrix(RBFm, RBFmInv);

	if(Wx != 0) delete Wx;
	if(Wy != 0) delete Wy;
	if(Wz != 0) delete Wz;
	Wx = new float[featureNum_];
	Wy = new float[featureNum_];
	Wz = new float[featureNum_];

	for(int i = 0; i < featureNum_; ++i)
	{
		Wx[i] = 0;
		Wy[i] = 0;
		Wz[i] = 0;
		for(int j = 0; j < featureNum_; ++j)
		{
			Wx[i] += RBFmInv(j,i) * newFeaturePos_[featureIndices_[j]*3];
			Wy[i] += RBFmInv(j,i) * newFeaturePos_[featureIndices_[j]*3+1];
			Wz[i] += RBFmInv(j,i) * newFeaturePos_[featureIndices_[j]*3+2];
		}
	}
}

inline float MeshMorph::RBF_multiquadric(float dist2, float eps)
{
	return sqrt(eps + dist2);
}
inline float MeshMorph::RBF_inv_quadric(float dist2, float eps)
{
	return 1.0 / (eps + dist2);//sqrt(eps + dist2 > (100*eps) ? 100 * eps : dist2);
}
inline float MeshMorph::RBF_inv_multiquadric(float dist2, float eps)
{
	return 1.0 / sqrt(eps * 10 + dist2);
}
