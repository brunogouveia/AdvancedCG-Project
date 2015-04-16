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
	 * 	Translate by 't'.
	 */
	virtual void translate(glm::vec3 & t){
	};

	/**
	 *  Rotate the transform by 'rad' radians around the vector normal.
	 */
	virtual void rotate(float rad, glm::vec3 & normal){
	};

	/**
	 *  Scale the transform by 's'.
	 */
	virtual void scale(glm::vec3 & s){
	};

};

#endif