#include "Polygon.h"
#include <stack>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
namespace TPolygon {
	using namespace std;
	// ��������(˳ʱ��Ϊ������ʱ��Ϊ��) ��ʽ:��-ydx = -0.5(Yn+1 + Yn)(Xn+1 - Xn)
	double Area(const vector<glm::vec2>& _polygon) {
		if (_polygon.size()) return 0;
		double area = 0;
		size_t i, j;
		for (i = 0; i < _polygon.size(); i++) {
			j = (i + 1) % _polygon.size();
			area += (double)_polygon[i].x * _polygon[j].y;
			area -= (double)_polygon[i].y * _polygon[j].x;
		}
		return area / 2;
	}

	// �����ƫ�ƣ����ֱպ�(��β�������պ�)
	void Offset(const vector<glm::vec3>& polygon, float offsetValue, vector<glm::vec3>& out_polygon) {
		out_polygon = polygon;
		if (offsetValue == 0) { return; }
		size_t vertCount = polygon.size();
		bool closure = glm::length(polygon[0] - polygon[vertCount - 1]) < offsetValue / 100; //�Ƿ�պ�
		if (closure) {
			for (size_t i = 0; i < vertCount - 1; i++) {
				size_t index1 = ((int)i - 1) < 0 ? vertCount - 2 : i - 1;
				size_t index2 = i + 1;
				glm::vec3 vector1 = glm::normalize(polygon[index1] - polygon[i]);
				glm::vec3 vector2 = glm::normalize(polygon[index2] - polygon[i]);
				glm::vec3 offsetDir = glm::normalize(vector1 + vector2);
				glm::vec3 axis = glm::normalize((glm::cross(polygon[index1] - polygon[i], offsetDir)));
				glm::vec3 result = glm::angleAxis(glm::radians(90.0f), axis) * (polygon[index1] - polygon[i]); //����˳ʱ����ת90��
				float finalVal = offsetValue * glm::length(offsetDir) * glm::length(result) / glm::dot(offsetDir, result);
				out_polygon[i] = polygon[i] + offsetDir * finalVal;
			}
			out_polygon[vertCount - 1] = out_polygon[0];
		}
		else { //�Ǳպ�
			for (size_t i = 0; i < vertCount; i++) {
				if (i == 0 && !closure) { //�׵㴦��
					glm::vec3 axis = glm::normalize((glm::cross(polygon[0] - polygon[1], polygon[2] - polygon[1])));
					glm::vec3 result = glm::normalize((glm::angleAxis(glm::radians(90.0f), axis) * (polygon[0] - polygon[1]))); //����˳ʱ����ת90��
					out_polygon[0] = polygon[i] + result * offsetValue;
				}
				else if (i == vertCount - 1 && !closure) {//β�㴦��
					glm::vec3 axis = glm::normalize((glm::cross(polygon[i - 2] - polygon[i - 1], polygon[i] - polygon[i - 1])));
					glm::vec3 result = glm::normalize((glm::angleAxis(glm::radians(90.0f), axis) * (polygon[i - 1] - polygon[i]))); //����˳ʱ����ת90��
					out_polygon[i] = polygon[i] + result * offsetValue;
				}
				else {
					size_t index1 = ((int)i - 1) < 0 ? vertCount - 1 : (int)i - 1;
					size_t index2 = (i + 1) == vertCount ? 0 : i + 1;

					glm::vec3 vector1 = glm::normalize(polygon[index1] - polygon[i]);
					glm::vec3 vector2 = glm::normalize(polygon[index2] - polygon[i]);
					glm::vec3 offsetDir = glm::normalize(vector1 + vector2);
					glm::vec3 axis = glm::normalize((glm::cross(polygon[i - 1] - polygon[i], offsetDir)));
					glm::vec3 result = glm::angleAxis(glm::radians(90.0f), axis) * (polygon[i - 1] - polygon[i]); //����˳ʱ����ת90��
					float finalVal = offsetValue * glm::length(offsetDir) * glm::length(result) / glm::dot(offsetDir, result);
					out_polygon[i] = polygon[i] + offsetDir * finalVal;
				}
			}
		}
	}
}