#include "RenderTrajectories.h"
#include "Camera.h"
#include "OrbitalData.h"
#include "RenderContext.h"
RenderTrajectories::RenderTrajectories() {
	circleMesh = Circle::Instance();
	this->shader = std::make_shared<Shader>(
		"assets/shaders/vertex_trajectory.glsl",
		"assets/shaders/geometry_trajectory.glsl",
		"assets/shaders/fragment_trajectory.glsl"
	);
}

void RenderTrajectories::drawTrajectories(std::shared_ptr<Body> host, Camera* camera, glm::mat4 projection, int width, int height) {
	std::shared_ptr<Body> curr = camera->getTrackedBody();
	std::vector<OrbitData> orbits;
	if (camera->getTrackedBody() == nullptr) {
		curr = host;
	}
	createOrbitalData(orbits, curr, host, camera->cameraPos);
	if (orbits.empty())
		return;
	RenderContext ctx;
	ctx.shader = shader;
	ctx.camera = camera;
	ctx.projection = projection;
	ctx.screenWidth = static_cast<float>(width);
	ctx.screenHeight = static_cast<float>(height);
	circleMesh->addInstances(orbits);
	circleMesh->draw(ctx);
}

void RenderTrajectories::createOrbitalData(std::vector<OrbitData>& orbits, std::shared_ptr<Body> curr, std::shared_ptr<Body> host, glm::vec3 cameraPos) {
	std::vector<std::shared_ptr<Body>> children = curr->getChildren();
	if (curr != host) {
		if (curr->getTrajectory().has_value()) {
			const auto& trajOpt = curr->getTrajectory();
			if (trajOpt.has_value()) {
				const auto& traj = *trajOpt;
				OrbitData od{};
				od.semimajorAxis = traj.semiMajorAxis;
				od.eccentricity = traj.eccentricity;
				glm::mat3 rotation3 = calculateRotations(traj);
				glm::mat4 rotation = glm::mat4(rotation3);
				glm::mat4 model(1.0f);
				glm::vec3 parentRenderPos =
					curr->getParent()->position - cameraPos;
				model = glm::translate(model, parentRenderPos);
				od.model = model * rotation;
				od.color = glm::vec4(curr->orbitColor, 1.0f);
				orbits.push_back(od);
			}
		}
	}
	for (const auto& child : curr->getChildren()) {
		const auto& trajOpt = child->getTrajectory();
		if (trajOpt.has_value()) {
			const auto& traj = *trajOpt;
			OrbitData od{};
			od.semimajorAxis = traj.semiMajorAxis;
			od.eccentricity = traj.eccentricity;
			glm::mat3 rotation3 = calculateRotations(traj);
			glm::mat4 rotation = glm::mat4(rotation3);
			glm::mat4 model(1.0f);
			glm::vec3 parentRenderPos =
				child->getParent()->position - cameraPos;
			model = glm::translate(model, parentRenderPos);
			od.color = glm::vec4(child->orbitColor, 1.0f);
			od.model = model * rotation;
			orbits.push_back(od);
		}
	}
}
glm::mat3 RenderTrajectories::calculateRotations(
	const Trajectory::TrajectoryStruct& traj)
{
	// Angles in double
	double arg = static_cast<double>(traj.omega);
	double inc = static_cast<double>(traj.inclination);
	double Omega = static_cast<double>(traj.Omega);

	glm::dmat3 R1 = glm::transpose(glm::dmat3(
		std::cos(arg), 0.0, std::sin(arg),
		0.0, 1.0, 0.0,
		-std::sin(arg), 0.0, std::cos(arg)
	));

	glm::dmat3 R2 = glm::transpose(glm::dmat3(
		std::cos(inc), -std::sin(inc), 0.0,
		std::sin(inc), std::cos(inc), 0.0,
		0.0, 0.0, 1.0
	));

	glm::dmat3 R3 = glm::transpose(glm::dmat3(
		std::cos(Omega), 0.0, std::sin(Omega),
		0.0, 1.0, 0.0,
		-std::sin(Omega), 0.0, std::cos(Omega)
	));

	glm::dmat3 R = R3 * R2 * R1;

	// Cast once at the boundary
	return glm::mat3(R);
}