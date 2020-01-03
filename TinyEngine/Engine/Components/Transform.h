#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <vector>

namespace TEngine {
	class Transform : public Component {
	public:
		Transform();

		Transform* Parent() const;
		void SetParent(Transform* parent_, bool worldPositionStays = true);
		unsigned int ChildCount() const;
		Transform* Child(unsigned int index) const;

		const glm::vec3& LocalPos() const;
		const glm::quat& LocalRotation() const;
		const glm::vec3& LocalScale() const;
		glm::vec3 Positon() const;              //��������
		glm::quat Rotation() const;				//������ת��Ԫ��
		const glm::vec3& EulerAngles() const;	//����ŷ����

		const glm::vec3 Forwward() const;		//����ǰ����
		const glm::vec3 Right() const;			//����������
		const glm::vec3 Up() const;				//����������

		const glm::mat4& LocalToWorldMarix();
		const glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
		const glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);
		const glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle);

		void MoveTo(const float& x, const float& y, const float& z);
		void MoveTo(const glm::vec3& vec);
		void Move(const float& x, const float& y, const float& z);
		void Move(const glm::vec3& vec);
		void RotateTo(const glm::vec3& eulerAngles_);
		void RotateTo(const float& x, const float& y, const float& z);
		void RotateTo(const glm::quat& q);
		void Rotate(const glm::vec3& eulerAngles_);
		void Rotate(const glm::vec3& axis, const float value);
		void SetLocalScale(const glm::vec3& scale_);

	private:
		glm::vec3 localPositon;
		glm::quat localRotation;
		glm::vec3 eulerAngles;
		glm::vec3 localScale;
		bool modelMatrixDirt = true;
		glm::mat4 modelMatrix;

		Transform* parent = nullptr;
		std::vector<Transform*> childs;

	private:
		void SetLocalToWorldMarixDirt();
	};
}