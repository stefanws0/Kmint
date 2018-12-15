#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <algorithm>
#include <map>

namespace kmint {
namespace pigisland {

map::map_node const &random_adjacent_node(map::map_node const &node) {
  int r = random_int(0, node.num_edges());
  return node[r].to();
}

map::map_node const &find_node_of_kind(map::map_graph const &graph, char kind) {
  auto i = std::find_if(graph.begin(), graph.end(), [kind](auto const &node) {
    return node.node_info().kind == kind;
  });
  return *i;
}

map::map_node const &find_random_mooring_place(map::map_graph const &graph) {
  return find_node_of_kind(graph, '1' + random_int(0, 3));
}

map::map_node const &find_shark_resting_place(map::map_graph const &graph) {
  return find_node_of_kind(graph, 'K');
}

const map::map_node* find_smallest_distance(std::map<const map::map_node*, float>& distances, std::vector<const map::map_node*>& closedList)
{
	const map::map_node* smallestDistance = nullptr;

	for (auto& pair : distances)
	{
		if (std::find(closedList.begin(), closedList.end(), pair.first) == closedList.end() && (smallestDistance == nullptr || pair.second < distances.find(smallestDistance)->second))
			smallestDistance = pair.first;
	}

	return smallestDistance;
}

std::unique_ptr<std::vector<const map::map_node*>> find_shortest_route(map::map_node const &current, map::map_node const &end)
{
	std::vector<const map::map_node*> openList;
	std::vector<const map::map_node*> closedList;
	std::map<const map::map_node*, float> distances;
	std::map<const map::map_node*, const map::map_node*> previousNode;
	bool foundEndNode = false;

	openList.emplace_back(&current);
	distances.insert(std::make_pair(&current, 0));
	previousNode.insert(std::make_pair(&current, nullptr));

	while (!openList.empty() || !foundEndNode)
	{
		const map::map_node* smallestDistance = find_smallest_distance(distances, closedList);
		openList.erase(std::remove(openList.begin(), openList.end(), smallestDistance), openList.end());

		for (int i = 0; i < smallestDistance->num_edges(); i++)
		{
			const map::map_node* neighbor = &(*smallestDistance)[i].to();
			previousNode.insert(std::make_pair(neighbor, smallestDistance));

			if (neighbor == &end)
				foundEndNode = true;

			float cost = distances.find(smallestDistance)->second + (*smallestDistance)[i].weight();

			if (distances.find(neighbor) == distances.end())
				distances.insert(std::make_pair(neighbor, cost));
			else if (cost < distances.find(neighbor)->second)
			{
				distances[neighbor] = cost;
				previousNode[neighbor] = smallestDistance;
			}
			
			if (std::find(closedList.begin(), closedList.end(), neighbor) == closedList.end())
				openList.emplace_back(neighbor);
		}

		closedList.push_back(smallestDistance);
		smallestDistance->tagged();
	}

	auto route = std::make_unique<std::vector<const map::map_node*>>();
	const map::map_node* tempNode = &end;

	while (tempNode != &current)
	{
		route->insert(route->begin(), tempNode);
		tempNode = previousNode.find(tempNode)->second;
	}

	return std::move(route);
}

int waiting_time(map::map_node const &node) {
  return static_cast<int>(node[0].weight());
}

} // namespace pigisland
} // namespace kmint
