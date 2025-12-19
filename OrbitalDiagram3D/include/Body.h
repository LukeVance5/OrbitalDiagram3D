#pragma once
#include <string>
#include <memory>
#include "OrbitalDiagram3DExport.h"
#include <glm/glm.hpp>
#include <Trajectory.h>
#include <optional>
#ifndef BODY_H
#define BODY_H
class OD_API Body : public std::enable_shared_from_this<Body> {
	public:
		static std::shared_ptr<Body> create(const std::string& name,float radius,float mass,glm::vec3 position,glm::vec3 velocity,
			std::string type
		) {
			auto body = std::shared_ptr<Body>(
				new Body(name, radius, mass, position, velocity, type)
			);
			return body;
		}
		static std::shared_ptr<Body> create(const std::string& name,float radius,float mass,glm::vec3 position,glm::vec3 velocity,
			std::string type, unsigned int textureID
		) {
			auto body = std::shared_ptr<Body>(
				new Body(name, radius, mass, position,velocity, type)
			);

			body->textureID = textureID;
			return body;
		}
		static std::shared_ptr<Body> create(const std::string& name,float radius,float mass,glm::vec3 position,glm::vec3 velocity,
			std::string type,
			std::shared_ptr<Body> parent
		) {
			auto body = std::shared_ptr<Body>(
				new Body(name, radius, mass, position, velocity, type)
			);
			
			body->setParent(parent);
			body->setVectors();

			return body;
		}

		static std::shared_ptr<Body> create(const std::string& name,float radius,float mass,glm::vec3 position,glm::vec3 velocity,
			std::string type,std::shared_ptr<Body> parent, unsigned int textureID) {
			auto body = std::shared_ptr<Body>(
				new Body(name, radius, mass, position, velocity, type)
			);
			body->setParent(parent);
			body->setVectors();
			body->textureID = textureID;
			return body;
		}
		
		std::string type;
		std::string name;
		float mass;
		float radius;
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 spinaxis;
		unsigned int textureID = 0;
		void addChild(std::shared_ptr<Body> child);
		void removeChild(std::shared_ptr<Body> child);
		void setParent(std::shared_ptr<Body> parentBody);
		void removeParent();
		std::shared_ptr<Body> getParent();
		std::vector<std::shared_ptr<Body>> getChildren();
		void setTrajectory(Trajectory::TrajectoryStruct trajectory);
		std::optional<Trajectory::TrajectoryStruct> getTrajectory();
		void clearTrajectory();
	protected:
		Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::string type);
	private:
		void setVectors();
		std::weak_ptr<Body> parent;
		std::vector<std::shared_ptr<Body>> children;
		std::optional<Trajectory::TrajectoryStruct> trajectory; // Trajectory is optional since right now nothing should render if body is on escape trajectory
};
#endif 
