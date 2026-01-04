#include "Body.h"
#include <algorithm>
Body::Body(const std::string& name, float radius, float mass, glm::vec3 position, glm::vec3 velocity, std::string type) {
	this->name = name;
	this->radius = radius;
	this->mass = mass;
	this->type = type;
	this->spinaxis = glm::vec3(0.0f, 1.0f, 0.0f);
	this->velocity = velocity;
	this->position = position;
}
// Finds the absolute position and velocity by traversing up the parent chain
void Body::setVectors() {
	std::shared_ptr<Body> curr = parent.lock();
	while (curr) {
		position += curr->position;
		velocity += curr->velocity;
		curr = curr->parent.lock();
	}
}
void Body::setParent(std::shared_ptr<Body> parentBody) {
	auto oldParent = parent.lock();
	if (oldParent != nullptr) {
		oldParent->removeChild(shared_from_this());
	}
	parent = parentBody;
	parentBody->addChild(shared_from_this());
}
void Body::addChild(std::shared_ptr<Body> child) {
	children.push_back(child);
}
void Body::removeChild(std::shared_ptr<Body> child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

std::shared_ptr<Body> Body::getParent() {
	return parent.lock();
}

void Body::removeParent() {
	if (auto p = parent.lock()) {
		p->removeChild(shared_from_this());
		parent.reset();
	}
}

void Body::setTrajectory(Trajectory::TrajectoryStruct trajectory) {
	this->trajectory = trajectory;
}

void Body::clearTrajectory() {
	this->trajectory = std::nullopt; // removes the value
}
std::optional<Trajectory::TrajectoryStruct> Body::getTrajectory() {
	return this->trajectory;
}
std::vector<std::shared_ptr<Body>>& Body::getChildren() {
	return children;
}

void Body::sortChildrenByDistanceRecursive()
{
	// Sort direct children by distance to *this*
	std::sort(children.begin(), children.end(),
		[this](const std::shared_ptr<Body>& a,
			const std::shared_ptr<Body>& b)
		{
			float da = distanceSq(a->position, this->position);
			float db = distanceSq(b->position, this->position);
			return da < db;
		}
	);

	// Recurse
	for (const auto& child : children)
	{
		child->sortChildrenByDistanceRecursive();
	}
}