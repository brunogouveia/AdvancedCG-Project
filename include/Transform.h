#ifndef TRANSFORM_H__
#define TRANSFORM_H__ 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Transform
{
public:
	/**
	 * 	Translate by a vector 't'.
	 */
	virtual void translate(glm::vec3 & t){
	};
	virtual void translate(float tx, float ty, float tz){
	};

	/**
	 *  Rotate the transform by 'rad' radians around the vector normal.
	 */
	virtual void rotate(float rad, glm::vec3 & normal){
	};

	/**
	 *  Scale the transform by a vector 's'.
	 */
	virtual void scale(glm::vec3 & s){
	};
	virtual void scale(float sx, float sy, float sz){
	};
};

#endif